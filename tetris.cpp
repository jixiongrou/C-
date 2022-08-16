#include<iostream>
#include<ctime>
#include<unistd.h>
#include<termio.h>
#include<conio.h>
#include<stdio.h>

using namespace std;

class Tetris
{
private:
	int nowID;
	int point[2];
	int top, left, right, bottom;

public:
	Tetris();
	bool Collision(int, int, int);
	void DrawMap();
	void Draw(int x, int y, int ShapeID);
	void ClearDraw(int, int, int);
	void Turn(int);
	void Updata();
	void LineRemove();
	void GameOver();
	void Run();
	void Pause();
	void InputScore();
};

const int shape[13][8]=
{
{0,0,0,1,0,2,0,3},{0,0,1,0,2,0,3,0},{0,0,0,1,0,2,1,2},{2,0,0,1,1,1,2,1},
{0,0,1,0,1,1,1,2},{0,0,1,0,2,0,0,1}	
};
const int high[15] = {4,1,3,2,3,2};
int map[56][27];

#define I1 0    //
#define I2 1

#define L1 2
#define L2 3
#define L3 4
#define L4 5

#define O 6

#define T1 7
#define T2 8
#define T3 9
#define T4 10

#define S1 11
#define S2 12

Tetris::Tetris()
{
	point[0] = 4;
	point[1] = 1;
	//score = 0;
	top = 0;
	left = 0;
	right = 14 * 2;
	bottom = 25;
}

void Tetris::Turn(int num)
{
	switch(num)
	{
		case I1:nowID = I2; break;
		case I2:nowID = I1; break;

		case L1:nowID = L2; break;
		case L2:nowID = L3; break;
		case L3:nowID = L4; break;
		case L4:nowID = L1; break;
		
		case O:nowID = O; break;
		
		case T1:nowID = T2; break;
		case T2:nowID = T3; break;
		case T3:nowID = T4; break;
		case T4:nowID = T1; break;
		
		case S1:nowID = S2; break;
		case S2:nowID = S1; break;
	}
}

void SetCursorPos(int XPos, int YPos)
{
	printf("\033[%d;%dH", YPos+1, XPos+1);
}

void Tetris::DrawMap()
{
	int i;

	for(i = left; i < right + 22; i = i + 2)
	{
		SetCursorPos(i, top);
		cout << "■";
		SetCursorPos(i, bottom);
		cout << "■";
	}
	
	for(i = top; i < bottom; i++)
	{
		SetCursorPos(left, i);
		cout << "■";
		SetCursorPos(right, i);
		cout << "■";
		SetCursorPos(right + 20, i);
		cout << "■";
	}
	for(i = right + 2; i < right + 20; i = i + 2)
	{
		SetCursorPos(i, 16);
		cout << "■";
	}
	
	SetCursorPos(right + 4, 18); cout << "W: Turn";
	SetCursorPos(right + 4, 19); cout << "A: Left";
	SetCursorPos(right + 4, 20); cout << "S: Tempo";
	SetCursorPos(right + 4, 21); cout << "D: Right";

}

bool Tetris::Collision(int x, int y, int shapeID)    //whether block touch another blocks or wall
{
	int nowx, nowy;
	for(int i = 0; i < 4; i++)
	{
		nowx = (x + shape[shapeID][2 * i]) * 2;
		nowy = y + shape[shapeID][2 * i + 1];
		if(!(nowx < right && nowx > left && nowy < bottom && nowy > top && !map[nowx][nowy]))
			return true;
	}
	return false;
}

void Tetris::LineRemove()
{
	int i, j, k;
	int flagRemove;

	for(j = point[1]; j < point[1] + high[nowID]; j++)	//block到底时，将block所在的几行进行判断，一旦有一行满了就break出来，进入消行
	{	
		flagRemove = 1;
		for(i = left + 2; i < right; i = i + 2)
		{
			if(map[i][j] == 0)
				flagRemove = 0;
		}
		if(flagRemove == 1)
		{
			for(k = j; k > top; k--)	
			{
				for(i = left + 2; i < right; i = i + 2)
				{	
					map[i][k] = map[i][k-1];
				}
			}
		
	
			for(k = 1; k <= j; k++)
			{
				for(i = left + 2; i < right; i = i + 2)
				{	
					SetCursorPos(i, k);
					if(map[i][k] == 1)
						cout << "■";
					else
						cout << "  ";
				}
			}
		}
	}
}

void Tetris::Updata()
{
	int nowx, nowy;

	for(int i = 0; i < 4; i++)    //when block not fall down, updata values in the map
	{
		nowx = point[0] + shape[nowID][i * 2];
		nowy = point[1] + shape[nowID][i * 2 + 1];
		SetCursorPos((nowx) * 2, nowy);
		cout << "■";
		map[nowx * 2][nowy] = 1;
	}
	
	LineRemove();

}

void Tetris::Draw(int x, int y, int ShapeID)
{
	int nowx, nowy;

	for(int i = 0; i < 4; i++)
	{
		nowx = x + shape[ShapeID][2 * i];
		nowy = y + shape[ShapeID][2 * i + 1];
		SetCursorPos((nowx) * 2, nowy);
		cout << "■";
		cout.flush();
	}
}

void Tetris::ClearDraw(int x, int y, int shapeID)
{
	int nowx, nowy;

	for(int i = 0; i < 4; i++)
	{
		nowx = x + shape[nowID][2 * i];
		nowy = y + shape[nowID][2 * i + 1];
		SetCursorPos((nowx) * 2, nowy);
		cout << "  ";
		cout.flush();
	}
}

void Tetris::Pause()
{
	while(1)
	{
		if(kbhit())
			break;
	}
}

void Tetris::GameOver()
{
	Pause();
	for(int i = left + 2; i < right; i = i + 2)
	{
		for(int j = top + 1; j < bottom; j++)
		{
			map[i][j] = 0;
			SetCursorPos(i, j);
			cout << "  ";
		}
	}

}	


void Tetris::Run()
{
	printf("\033[?25l");
	
	int nextID;

	srand(time(NULL));

	nowID = rand() % 6;
	nextID = rand() % 6;

	Draw(point[0], point[1], nowID);
	Draw(18, 4, nextID);

	int count = 150;
	int i = 0;
	char c;
	int tempTurn;
	while(1)
	{		
		system("stty -icanon -echo");
		usleep(10000);
		if(i >= 20)
		{
			i = 0;
			ClearDraw(point[0], point[1], nowID);
			point[1]++;
			if(Collision(point[0], point[1], nowID))
			{	
				point[1]--;
				Updata();
				if(point[1] - high[nextID] <= 0)
				{
					for(int j = 1; j < point[1]; j++)
					{
						for(int k = 0; k < 4; k++)
						{
							if(shape[nextID][2 * k + 1] == high[nextID] - j)
							{
								SetCursorPos((point[0] + shape[nextID][2 * k]) * 2, shape[nextID][2 * k + 1]);
								cout << "■";
							}
						}
					}
					GameOver();
	
				}
				for(int i = 2; i < right; i = i + 2)
				{
					for(int j = 1; j < bottom; j++)
					{
						SetCursorPos(60 + i,j);
						cout << map[i][j];
					}
				}		
				point[0] = 4;
				point[1] = 1;
				nowID = nextID;
				nextID = rand() % 6;
				ClearDraw(18, 4, nowID);
				Draw(18, 4, nextID);
			}

			Draw(point[0], point[1], nowID);
		}

		if(kbhit())
		{
			c = getch();
			system("stty icanon echo");
			
			if(c == 'd' && !Collision(point[0] + 1, point[1], nowID))
			{
				ClearDraw(point[0], point[1], nowID);
				point[0]++;
				Draw(point[0], point[1], nowID);
			}

			if(c == 'a' && !Collision(point[0] - 1, point[1], nowID))
			{
				ClearDraw(point[0], point[1], nowID);
				point[0]--;
				Draw(point[0], point[1], nowID);
			}


			if(c == 's' && !Collision(point[0], point[1] + 1, nowID))
			{
				ClearDraw(point[0], point[1], nowID);
				point[1]++;
				Draw(point[0], point[1], nowID);
			}
			
			if(c == 'w')
			{	
				ClearDraw(point[0], point[1], nowID);
				tempTurn =nowID;
				Turn(nowID);
				if(Collision(point[0], point[1], nowID))
					nowID = tempTurn;
				Draw(point[0], point[1], nowID);

			}
			
			if(c == 'p')
			{
				Pause();
			}
			
			if(c == 'q')
				printf("\033[?25h");
		}		
		i++;
	}
}

int main()
{
	Tetris game;
	game.DrawMap();
	game.Run();
	return 0;
}
