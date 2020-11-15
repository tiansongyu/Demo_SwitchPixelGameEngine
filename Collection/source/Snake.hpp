#pragma once 

#include"SwitchPixelGameEngine.h"
#include<string>
#include<list>
#include<map>
using namespace std;

static float speed_MAX = 0.0f;


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


class Snake :public SwitchPixelGameEngine
{
public:
	Snake()
	{

	}
private:
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

public:
	bool OnUserCreate()override
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
		return true;
	}
	bool OnUserUpdate(float fElapsedTime)override
	{
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
		DrawString(1000,30,"贪吃蛇");
		DrawString(1000,70,"上下左右控制方向");
		return true;
	}
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
};
