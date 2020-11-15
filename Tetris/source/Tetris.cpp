#include"SwitchPixelGameEngine.h"
#include<string>
using namespace std;
class Tetris :public SwitchPixelGameEngine
{
public: 
	Tetris()
	{
		
	}
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
public:
	bool OnUserCreate()override
	{
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


		return true;
	}
	bool OnUserUpdate(float fEspetime)override
	{
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
	
		retation += (KeyDown(KEY_A)&&CheckCollision(display_block, retation+1>3?0:retation+1, blockX, blockY ) ? 1 : 0);
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
		

		time_count += fEspetime * 100;
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
		return true;	
	}
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
};


int main()
{
	Tetris demo;
	demo.ConstructConsole(6, 6);
	demo.GameThread();


	return 0;
}