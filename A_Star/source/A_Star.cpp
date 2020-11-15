#include"SwitchPixelGameEngine.h"
#include<vector>
#include<list>
using namespace std;
class Astar :public SwitchPixelGameEngine
{
public:
	Astar()
	{
	}
private:
	struct sNode
	{
		bool bObstacle = false;
		bool bVisited = false;
		float fGlobalGoal;
		float fLocalGoal;
		int x;
		int y;
		vector<sNode*> vecNeighbours;
		sNode* parent;
    };

	int nNodeSize = 9;
	int nNodeBorder = 2;

	sNode* nodes = nullptr;
	int nMapWidth = 16;
	int nMapHeight = 16;
	
	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;

public:
	bool Solve_AStar()
	{
		// Reset Navigation Graph - default all node states
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y * nMapWidth + x].parent = nullptr;	// No parents
			}

		auto distance = [](sNode* a, sNode* b) // For convenience
		{
			return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};

		auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
		{
			return distance(a, b);
		};

		// Setup starting conditions
		sNode* nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true; // We only explore a node once


			// Check each of this node's neighbors...
			for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
			{
				// ... and only if the neighbors is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbors potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what 
				// the neighbors currently has set, update the neighbors to use this node
				// as the path source, and set its distance scores as necessary
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbors being tested has changed, so
					// update the neighbor's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the also will realize this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}
		}

		return true;
	}


	virtual bool OnUserCreate()override
	{
		nodes = new sNode[nMapHeight * nMapWidth];
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				nodes[y * nMapHeight + x].x = x;
				nodes[y * nMapHeight + x].y = y;
				nodes[y * nMapHeight + x].bObstacle = false;
				nodes[y * nMapHeight + x].parent = nullptr;
				nodes[y * nMapHeight + x].bVisited = false;
			}
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				if(y>0)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
				if(y<nMapHeight-1)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
				if (x > 0)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y ) * nMapWidth + (x - 1)]);
				if (x < nMapWidth - 1)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y ) * nMapWidth + (x + 1 )]);
			}
		nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
		nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth - 2];


		return true;
	}
	virtual bool OnUserUpdate(float fElapsedTime)override
	{


		if (m_mouse[0].bReleased)
		{
			int nSelectedNodeX = mouse_pos_x / nNodeSize;
			int nSelectedNodeY = mouse_pos_y / nNodeSize;
			if(nSelectedNodeX>=0 && nSelectedNodeX<nMapWidth)
				if (nSelectedNodeY >=0 && nSelectedNodeY < nMapHeight)
				{

					if (KeyHeld(KEY_X))
						nodeStart = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
					else if(KeyHeld(KEY_Y))
						nodeEnd = &nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX];
					else
						nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle = !nodes[nSelectedNodeY * nMapWidth + nSelectedNodeX].bObstacle;
					Solve_AStar();
				}
		}

		ClearScreen();
		
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				for (auto n : nodes[y * nMapWidth + x].vecNeighbours)
				{
					DrawLine(x * nNodeSize + nNodeSize / 2, y * nNodeSize + nNodeSize / 2,
						n->x * nNodeSize + nNodeSize / 2, n->y * nNodeSize + nNodeSize / 2,
						FG_BLUE
					);
				}
			}


		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,
					(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, 
					nodes[y*nMapWidth+x].bObstacle ? FG_DARK_GREY : FG_DARK_BLUE);
				if(nodes[y*nMapWidth+x].bVisited)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,
						(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
						FG_BLUE);

				if(&nodes[y*nMapWidth+x]== nodeStart)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,
						(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
						FG_DARK_RED);
				if (&nodes[y * nMapWidth + x] == nodeEnd)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,
						(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
						FG_DARK_YELLOW);
			}
		if (nodeEnd != nullptr)
		{
			sNode* p = nodeEnd;
			while (p->parent != nullptr)
			{
				DrawLine(p->x * nNodeSize + nNodeSize / 2, p->y * nNodeSize + nNodeSize / 2,
					p->parent->x * nNodeSize + nNodeSize / 2, p->parent->y * nNodeSize + nNodeSize / 2,
					FG_GREEN
				);
				p = p->parent;
			}
		}
		DrawString(800,40,"A-星算法，最短路径生成");
		DrawString(800,70, "按住X，点击屏幕设置起点");
		DrawString(800,100, "按住Y，点击屏幕设置终点");

		return true;
	}
};

int main()
{
	Astar demo;
	demo.ConstructConsole(5,5);
	demo.GameThread();
	return 0;
}