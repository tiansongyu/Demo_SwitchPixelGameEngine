#include "SwitchPixelGameEngine.h"
#include "A_Star.hpp"
#include "Maze.hpp"
#include "Snake.hpp"
#include "Tetris.hpp"

struct GAMELIST
{
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
};


class Collection : public SwitchPixelGameEngine
{
public:
    Collection()
    {
       
    }
    virtual bool OnUserDestroy() override
    {
        delete picture;
        delete[] nodes;
        delete[] m_maze;
        delete[] gamelist;
        return true;
    }
    void A_Star_reset()
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

    }
    void maze_reset()
    {
        m_Width = 40;
        m_Height = 25;
		
		m_maze = new int[m_Width * m_Height];
		memset(m_maze, 0x00, m_Width * m_Height * sizeof(int));

		int x = rand() % m_Width;
		int y = rand() % m_Height;
		m_stack.push(make_pair(x, y)); //初始化第一个点

		m_maze[y*m_Width+x ] = CELL_VISITED;
		m_nVisitedCells = 1;
    }
    void snake_reset()
    {
        
        map_snake += "################";
		map_snake += "#.........#....#";
		map_snake += "#...#..........#";
		map_snake += "#..........#...#";
		map_snake += "#..............#";
		map_snake += "#..............#";
		map_snake += "#..............#";
		map_snake += "#..............#";
		map_snake += "#..#...........#";
		map_snake += "#..............#";
		map_snake += "#.....#........#";
		map_snake += "#...........#..#";
		map_snake += "#..............#";
		map_snake += "#..............#";
		map_snake += "################";

		snake.push_front(body(6, 5));
		snake.push_front(body(6, 6));
		snake.push_front(body(6, 7));

		tmp_food.x = 4;
		tmp_food.y = 3;
    }
    void Tetris_reset()
    {
        //俄罗斯方块start
        //create map
        
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "#.........#";
		map += "###########";
		//create block
		Tetris_block[0] += "..0.";
		Tetris_block[0] += "..0.";
		Tetris_block[0] += "..0.";
		Tetris_block[0] += "..0.";

		Tetris_block[1] += "....";
		Tetris_block[1] += "..1.";
		Tetris_block[1] += ".111";
		Tetris_block[1] += "....";

		Tetris_block[2] += ".2..";
		Tetris_block[2] += ".2..";
		Tetris_block[2] += ".22.";
		Tetris_block[2] += "....";

		Tetris_block[3] += "..3.";
		Tetris_block[3] += "..3.";
		Tetris_block[3] += ".33.";
		Tetris_block[3] += "....";

		Tetris_block[4] += ".4..";
		Tetris_block[4] += ".44.";
		Tetris_block[4] += "..4.";
		Tetris_block[4] += "....";

		Tetris_block[5] += "..5.";
		Tetris_block[5] += ".55.";
		Tetris_block[5] += ".5..";
		Tetris_block[5] += "....";
        //俄罗斯方块end
    }
    virtual bool OnUserCreate() override
    {
        picture = new SgeSprite(ScreenWidth()-194 , ScreenHeight()-203,"romfs:/wukong_2.png");

        Tetris_reset();
        snake_reset();
        A_Star_reset();
        maze_reset();
        SetFontSize(LARGE_FONT);
        gamelist = new GAMELIST[4];
        

        gamelist[0].x1 = 360;
        gamelist[0].y1 = 160 ;
        gamelist[0].x2 = 360;
        gamelist[0].y2 = 210;
        gamelist[0].x3 = 400;
        gamelist[0].y3 = 185;

        gamelist[1].x1 = 360;
        gamelist[1].y1 = 160 + 100;
        gamelist[1].x2 = 360;
        gamelist[1].y2 = 210 + 100;
        gamelist[1].x3 = 400;
        gamelist[1].y3 = 185 + 100;

        gamelist[2].x1 = 360;
        gamelist[2].y1 = 160 + 100 + 100;
        gamelist[2].x2 = 360;
        gamelist[2].y2 = 210 + 100 + 100;
        gamelist[2].x3 = 400;
        gamelist[2].y3 = 185 + 100 + 100;

        gamelist[3].x1 = 360;
        gamelist[3].y1 = 160 + 100 + 100 + 100;
        gamelist[3].x2 = 360;
        gamelist[3].y2 = 210 + 100 + 100 + 100;
        gamelist[3].x3 = 400;
        gamelist[3].y3 = 185 + 100 + 100 + 100;
        return true;
    }
    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        if((KeyDown(KEY_A) || running) && current_index == 1)
        {
            if(KeyDown(KEY_B))
                {
                    running = false;
                    SetBlockSize(1,1);
                    return true;
                }
            SetBlockSize(6,6);
            running = true;
            ClearScreen();
            int old_direct = direct;
            int new_direct;
            if (KeyDown(KEY_UP ))
            {
                new_direct = UP;
                if ((old_direct + new_direct) % 2 == 1)
                    direct = UP;
            }
            if (KeyDown(KEY_DOWN))
            {
                new_direct = DOWN;
                if ((old_direct + new_direct) % 2 == 1)
                    direct = DOWN;
            }
            if (KeyDown(KEY_LEFT))
            {
                new_direct = LEFT;
                if ((old_direct + new_direct) % 2 == 1)
                    direct = LEFT;
            }
            if (KeyDown(KEY_RIGHT))
            {
                new_direct = RIGHT;
                if ((old_direct + new_direct) % 2 == 1)
                    direct = RIGHT;
            }
            FillCircle(tmp_food.x*nBlock_snake + (nBlock_snake / 2),tmp_food.y*nBlock_snake + (nBlock_snake / 2),3,FG_RED);

            for(int x = 0;x<nWight_snake;x++)
                for(int y = 0;y<nHeight_snake;y++)
                {
                    switch (map_snake[y*nWight_snake + x])
                    {
                        case '#':
                            Fill(x*nBlock_snake, y*nBlock_snake, (x + 1)*nBlock_snake, (y + 1)*nBlock_snake, FG_WHITE);
                            break;
                        default:
                            break;
                    }
                }
        
            if (snake.front().x == tmp_food.x&&snake.front().y == tmp_food.y)
            {
                body tmp_body;
                tmp_body.x = tmp_food.x;
                tmp_body.y = tmp_food.y;
                snake.push_back(tmp_body);
                bool get = false;
                int x;
                int y;
                while (!get)
                {
                    x = rand() % 15 + 1;
                    y = rand() % 14 + 1;

                    for (auto tmp : snake)
                    {
                        if (tmp.x == x&&tmp.y == y)
                            continue;
                        if (map_snake[y*nWight_snake + x] != '#')
                            get = true;
                    }
                }
                tmp_food.x = x;
                tmp_food.y = y;
            }
            for (auto tmp = snake.begin(); tmp != snake.end(); tmp++)
            {

                if (map_snake[tmp->y*nWight_snake + tmp->x] == '#')
                {
                    //dead and reset 
                    return dead();
                }
                Fill(tmp->x*nBlock_snake, tmp->y*nBlock_snake, (tmp->x + 1)*nBlock_snake, (tmp->y + 1)*nBlock_snake, FG_GREEN);
            }
            speed_MAX += fElapsedTime;
            if (speed_MAX > speed_low)
            {
                speed_MAX = 0.0f;			
                switch (direct)
                {
                case UP:
                    new_body.x = snake.front().x ;
                    new_body.y = snake.front().y - 1;
                    if (eat_yourself())
                        return true;
                    snake.push_front(new_body);
                    snake.pop_back();
                    break;
                case DOWN:
                    new_body.x = snake.front().x;
                    new_body.y = snake.front().y + 1;
                    if (eat_yourself())
                        return true;

                    snake.push_front(new_body);
                    snake.pop_back();
                    break;
                case LEFT:
                    new_body.x = snake.front().x-1;
                    new_body.y = snake.front().y ;
                    if (eat_yourself())
                        return true;

                    snake.push_front(new_body);
                    snake.pop_back();
                    break;
                case RIGHT:
                    new_body.x = snake.front().x+1;
                    new_body.y = snake.front().y;
                    if (eat_yourself())
                        return true;
                    snake.push_front(new_body);
                    snake.pop_back();
                    break;
                default:
                    break;
                }
            }
            DrawString(900,40,"贪吃蛇");
            DrawString(900,80,"上下左右控制方向");
            DrawString(900,1200,"按下B返回");

            return true;
        }
        if((KeyDown(KEY_A) || running) && current_index == 2)
        {
            if(KeyDown(KEY_B))
                {
                    running = false;
                    SetBlockSize(1,1);
                    return true;
                }
            SetBlockSize(6,6);
            running = true;
            auto offset = [&](int x, int y) {
                return (m_stack.top().second + y) * m_Width + (m_stack.top().first + x);
            };
            vector<int> neighbours;

            if (m_nVisitedCells < m_Width * m_Height)
            {
                //上
                if (m_stack.top().second > 0 && (m_maze[offset(0,-1)] & CELL_VISITED) == 0)
                {
                    neighbours.push_back(0);
                }
                //下
                if (m_stack.top().second < m_Height-1 && (m_maze[offset(0, +1)] & CELL_VISITED) == 0)
                {
                    neighbours.push_back(1);
                }
                //左
                if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
                {
                    neighbours.push_back(2);
                }
                //右
                if (m_stack.top().first < m_Width -1&& (m_maze[offset(+1, 0)] & CELL_VISITED) == 0)
                {
                    neighbours.push_back(3);
                }

                if (!neighbours.empty())
                {
                    int index = neighbours[rand() % neighbours.size()];

                    switch (index)
                    {
                    case 0://上

                        m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATHS;
                        m_maze[offset(0, 0)] |= CELL_PATHN;
                        m_stack.push(make_pair(m_stack.top().first + 0, m_stack.top().second  -1));
                        break;
                    case 1://下

                        m_maze[offset(0, 1)] |= CELL_VISITED | CELL_PATHN;
                        m_maze[offset(0, 0)] |= CELL_PATHS;
                        m_stack.push(make_pair(m_stack.top().first + 0, m_stack.top().second + 1));
                        break;
                    case 2: //左

                        m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATHE;
                        m_maze[offset(0, 0)] |= CELL_PATHW;
                        m_stack.push(make_pair(m_stack.top().first  -1, m_stack.top().second + 0 ));
                        break;
                    case 3: //右

                        m_maze[offset(1, 0)] |= CELL_VISITED | CELL_PATHW;
                        m_maze[offset(0, 0)] |= CELL_PATHE;
                        m_stack.push(make_pair(m_stack.top().first + 1, m_stack.top().second + 0));
                        break;
                    }
                    m_nVisitedCells++;
                }
                else
                    m_stack.pop();
            }
            else
            {
                while (1)
                {
                    if (!m_stack.empty())
                        m_stack.pop();
                    else
                        break;
                }
                

                m_Width = 40;
                m_Height = 25;

                m_maze = new int[m_Width * m_Height];
                memset(m_maze, 0x00, m_Width * m_Height * sizeof(int));

                int x = rand() % m_Width;
                int y = rand() % m_Height;
                m_stack.push(make_pair(x, y)); //初始化第一个点

                m_maze[y * m_Width + x] = CELL_VISITED;
                m_nVisitedCells = 1;
            }
            //Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
            ClearScreen();
            for(int x =0;x<m_Width;x++)
                for (int y = 0; y < m_Height; y++)
                {
                    for (int py = 0; py < path_width; py++)
                        for (int px = 0; px < path_width  ; px++)
                        {
                            if (m_maze[y * m_Width + x] & CELL_VISITED)
                                Draw(x * block_size + px, y * block_size + py, FG_YELLOW);
                            else
                                Draw(x * block_size + px, y * block_size + py, FG_BLUE);
                        }
                    for (int p = 0; p < path_width; p++)
                    {
                        if (m_maze[y * m_Width + x] & CELL_PATHE)
                        {
                            Draw(x * block_size + path_width, y * block_size + p, FG_YELLOW);
                        }
                        if (m_maze[y * m_Width + x] & CELL_PATHS)
                        {
                            Draw(x * block_size + p, y * block_size + path_width, FG_YELLOW);
                        }
                    }
                }

                for (int py = 0; py < path_width; py++)
                    for (int px = 0; px < path_width; px++)
                        Draw(m_stack.top().first * (path_width + 1) + px, m_stack.top().second * (path_width + 1) + py,FG_GREEN); // Draw Cell
            DrawString(960,60,"迷宫生成算法");
            DrawString(960,100,"按下B返回");

            return true;
        }
        if((KeyDown(KEY_A) || running) && current_index == 3)
        {
            if(KeyDown(KEY_B))
            {
                running = false;
                SetBlockSize(1,1);
                return true;
            }
            SetBlockSize(5,5);
            running = true;

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
            DrawString(720,40,"A-星算法，最短路径生成");
            DrawString(720,80, "按住X，点击屏幕设置起点");
            DrawString(720,120, "按住Y，点击屏幕设置终点");
            DrawString(720,160, "按下B返回");
            return true;
        }
        if((KeyDown(KEY_A) || running) && current_index == 0)
        {
            if(KeyDown(KEY_B))
            {
                running = false;
                SetBlockSize(1,1);
                return true;
            }
            SetBlockSize(6,6);
            running = true;

            ClearScreen();
            if (KeyDown(KEY_LEFT))
            {
                blockX -= (CheckCollision(display_block, retation, blockX - 1, blockY) ? 1 : 0);
                time_count = 0;
            }
            if (KeyDown(KEY_RIGHT))
            {
                blockX += (CheckCollision(display_block, retation, blockX + 1, blockY) ? 1 : 0);
                time_count = 0;
            }
                blockY += (KeyHeld(KEY_DOWN)&&CheckCollision(display_block, retation, blockX , blockY+1) ? 1 : 0);
        
            retation += (KeyDown(KEY_X)&&CheckCollision(display_block, retation+1>3?0:retation+1, blockX, blockY ) ? 1 : 0);
            if (retation > 3)
                retation = 0;

            for(int x = 0;x<nWight;x++)
                for (int y = 0; y < nHeight; y++)
                {
                    switch (map[y*nWight + x])
                    {
                    case '#':
                        Fill(x*nBlock, y*nBlock, (x + 1)*nBlock, (y + 1)*nBlock, FG_WHITE);
                        break;
                    case '0':
                        Fill(x*nBlock, y*nBlock, (x + 1)*nBlock, (y + 1)*nBlock, FG_RED);
                        break;
                    case '1':
                        Fill(x*nBlock, y*nBlock, (x + 1)*nBlock, (y + 1)*nBlock, FG_GREEN);
                        break;
                    case '2':
                        Fill(x*nBlock, y*nBlock, (x + 1)*nBlock, (y + 1)*nBlock, FG_BLUE);
                        break;
                    case '3':
                        Fill(x*nBlock, y*nBlock, (x + 1)*nBlock, (y + 1)*nBlock, FG_CYAN);
                        break;
                    case '4':
                        Fill(x*nBlock, y*nBlock, (x + 1)*nBlock, (y + 1)*nBlock, FG_MAGENTA);
                        break;

                    }
                }
            for(int x= 0;x<4;x++)
                for (int y = 0; y < 4; y++)
                {
                    int map_number = rotating(x, y, retation);
                    switch (Tetris_block[display_block][map_number])
                    {
                        case '0':
                            Fill((x + blockX)*nBlock, (y + blockY)*nBlock, (x + blockX + 1)*nBlock, (y + blockY + 1)*nBlock, FG_RED);
                            break;
                        case '1':
                            Fill((x + blockX)*nBlock, (y + blockY)*nBlock, (x + blockX + 1)*nBlock, (y + blockY + 1)*nBlock, FG_GREEN);
                            break;
                        case '2':
                            Fill((x + blockX)*nBlock, (y + blockY)*nBlock, (x + blockX + 1)*nBlock, (y + blockY + 1)*nBlock, FG_BLUE);
                            break;
                        case '3':
                            Fill((x + blockX)*nBlock, (y + blockY)*nBlock, (x + blockX + 1)*nBlock, (y + blockY + 1)*nBlock, FG_CYAN);
                            break;
                        case '4':
                            Fill((x + blockX)*nBlock, (y + blockY)*nBlock, (x + blockX + 1)*nBlock, (y + blockY + 1)*nBlock, FG_MAGENTA);
                            break;
                    }
                }
            

            time_count += fElapsedTime * 100;
            if (time_count > speed)
            {
                if (!CheckCollision(display_block,retation,blockX,blockY+1))
                {


                    //�·���
                    if (CheckCollision(rand() % 4, 0, 4,0))
                    {
                        //�����鱣���ڵ�ͼ��
                        for (int px = 0; px < 4; px++)
                            for (int py = 0; py < 4; py++)
                            {
                                int number = rotating(px, py, retation);
                                int fi = (py + blockY)*nWight + (px + blockX);
                                if (Tetris_block[display_block][number] != '.')
                                    map[fi] = Tetris_block[display_block][number];
                            }

                        blockX = 4;
                        blockY = 0;
                        display_block = rand() % 4;
                        retation = 0;
                    }
                    else
                    {
                        //geme over;
                    }
                    //���е÷�
                    for (int y = 0; y < nHeight - 1; y++)
                    {
                        bool get_sorce = true;
                        int  get_y;
                        for (int x = 1; x < nWight; x++)
                        {
                            if (map[y*nWight + x] == '.')
                            {
                                get_sorce = false;
                                break;
                            }
                            get_y = y;
                        }
                        if (get_sorce)
                        {
                            for (int y = get_y; y > 0; y--)
                            {
                                for (int x = 1; x < nWight; x++)
                                {
                                    map[y*nWight + x] = map[(y - 1)*nWight + x];
                                }
                            }
                            sorce++;
                        }
                    }
                }
                if (CheckCollision(display_block, retation, blockX, blockY + 1))
                {
                    blockY++;
                }
                time_count = 0;
            }

            DrawString(900,40,"俄罗斯方块");
            DrawString(900,80,"上下左右控制方向");
            DrawString(900,120,"按下X变形");
            DrawString(900,160, "按下B返回");

            return true;
        }
        ClearScreen();


        if(KeyDown(KEY_UP))
        {
            current_index--;
            if(current_index == -1)
                current_index = 3;
        }
        if(KeyDown(KEY_DOWN))
        {
            current_index++;
            if(current_index == 4)
                current_index = 0;
        }
        DrawString(440, 200, "俄罗斯方块");
        DrawString(440, 300, "贪吃蛇");
        DrawString(440, 400, "迷宫生成");
        DrawString(440, 500, "A-星寻路算法");

        FillTriangle(
        gamelist[current_index].x1,
        gamelist[current_index].y1,
        gamelist[current_index].x2,
        gamelist[current_index].y2,
        gamelist[current_index].x3,
        gamelist[current_index].y3,
        FG_YELLOW);

        DrawSprite(picture); 

        return true;
    }
private:
    uint32_t current_index = 0;
    GAMELIST* gamelist;

    //俄罗斯方块start
private:
	string map;
	int nWight = 11;
	int nHeight = 15;
	int nBlock = 8;

	int blockX = 4;
	int blockY = 0;

	int display_block = 0;
	int retation = 0 ;

	int time_count = 0.0f;
	int speed = 50.0f;

	string Tetris_block[6];

	int sorce = 0;


    int  rotating(int px, int py, int rotating)
	{
		int map_number;
		switch (rotating)
		{
		case 0 :
			map_number = 4 * py + px;
			break;
		case 1:
			map_number = 12 - 4 * px + py;
			break;
		case 2:
			map_number = 15-4*py-px;
			break;
		case 3:
			map_number = 3-py+4*px;
			break;
		}
		return map_number;
	}
	bool CheckCollision(int block_type, int rotation, int nPosX, int nPosY)
	{
		for(int x=0;x<4;x++)
			for (int y = 0; y < 4; y++)
			{
				int num = rotating(x, y, rotation);
				int fi = (y + nPosY)*nWight + (x + nPosX);
				if(x+nPosX>= 0 && x+nPosX<nWight)
					if(y+nPosY>=0 &&y+nPosY<nHeight)
						if (map[fi] != '.'&&Tetris_block[block_type][num] != '.')
							return false;
			}
		return true;
	}
    //俄罗斯方块end
    //A-星算法start
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
    //A-星算法end

    //maze start

    int m_Width;
	int m_Height;

	int path_width = 3;
	int block_size = 4;

	int* m_maze;  //建立int数组在存放每个块的属性
	enum
	{
		//用十六进制数来表示方向和访问性，牛
		CELL_PATHN = 0x01,    //向上可访问
		CELL_PATHE = 0x02,	  //向右可访问
		CELL_PATHS = 0x04,	  //向下可访问
		CELL_PATHW = 0x08,    //向左可访问
		CELL_VISITED = 0x10   //是否被访问过
	};
	int m_nVisitedCells;  //记录被访问过的块数

	stack<pair<int, int >> m_stack;  //栈记录访问过的点

    //maze end

    //snake start
    float speed_MAX = 0.0f;


    struct body
    {
        int x;
        int y;
        body(int _x, int _y) :x(_x), y(_y) {};
        body() {};
    }new_body;
    struct food
    {
        int x;
        int y;
        food(int _x, int _y) :x(_x), y(_y) {};
        food() {};
    };

    string map_snake;
	int nBlock_snake = 8;
	int nWight_snake = 16;
	int nHeight_snake = 15;
	float speed_low = 0.2f;
	food tmp_food;
	enum direction
	{
		UP, LEFT, DOWN, RIGHT
	};
	
	int direct = DOWN;

	list<body> snake;  


	bool dead()
	{
		direct = DOWN;
		snake.clear();
		snake.push_front(body(6, 5));
		snake.push_front(body(6, 6));
		snake.push_front(body(6, 7));
		return true;
	}
	bool eat_yourself()
	{
		for (auto tmp : snake)
		{
			if (new_body.x == tmp.x&& new_body.y == tmp.y)
				return dead();
		}
		return false;
	}
    //snake end
    bool running = false;

    SgeSprite* picture;
};

int main()
{
    Collection example;
    example.ConstructConsole(1,1);
    example.GameThread();
    return true;
}