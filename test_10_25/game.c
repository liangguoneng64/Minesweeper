#define _CRT_SECURE_NO_WARNINGS 1
#include "game.h"

void InitBoard(char board[ROWS][COLS], int row, int col, char set)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			board[i][j] = set;
		}
	}
}

void DisplayBoard(char board[ROWS][COLS], int row, int col)
{
	int i = 0;
	int j = 0;
	printf("--------扫雷游戏-----------\n");
	//打印列号
	for (j = 0; j <= col; j++)
	{
		printf("%d ", j); 
	}
	printf("\n");

	for (i = 1; i <= row; i++)
	{
		printf("%d ", i);//打印行号
		for (j = 1; j <= col; j++)
		{
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void SetMine(char mine[ROWS][COLS], int row, int col, int count)
{
	while (count)
	{
		int x = rand() % row + 1;
		//通过取模运算，将随机数限定在0到row-1之间
		//注意第0行、第0列，以及第10行、第10列是不布雷的边界，如果直接不加1，雷可能会被埋在0行或0列
		int y = rand() % col + 1;
		if (mine[x][y] == '0')
		{
			mine[x][y] = '1';
			count--;
		}
	}
}

int GetMineCount(char mine[ROWS][COLS], int x, int y)
{
	return(mine[x - 1][y] + mine[x - 1][y - 1] + mine[x][y - 1] + mine[x + 1][y - 1] +
	mine[x + 1][y] + mine[x + 1][y + 1] + mine[x][y + 1] + mine[x - 1][y + 1] - 8 * '0');

}

//展开一片: 从(x,y)开始排查，若周围没有雷，则自动展开周围8个格子，直到遇到数字格或边界
static void ExpandBoard(char show[ROWS][COLS], char mine[ROWS][COLS], int row, int col, int x, int y, int* pwin)
{
	if (x < 1 || x > row || y < 1 || y > col)//越界
	{
		return;
	}
	if (show[x][y] != '*')//已排查过或已标记，不重复展开
	{
		return;
	}
	if (mine[x][y] == '1')//是雷，交给FindMine处理
	{
		return;
	}
	int num = GetMineCount(mine, x, y);
	show[x][y] = num + '0';
	(*pwin)++;//每展开一个格子，胜利进度加1
	if (num == 0)//周围没有雷，继续向周围8个格子展开
	{
		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				ExpandBoard(show, mine, row, col, i, j, pwin);
			}
		}
	}
}

void FindMine(char show[ROWS][COLS], char mine[ROWS][COLS], int row, int col, int count)
{
	int x = 0;
	int y = 0;
	int mode = 0;//操作码: 1排查 2标记
	int win = 0;
	time_t start = time(NULL);
	//记录排雷开始时间,这里不能用colck(),因为它统计的是CPU占用时间，
	//玩家数等待期间不会走动

	while (win < row * col - count)
	{
		printf("请输入坐标和操作(1排查 2标记)，如: 5 5 2\n");
		scanf("%d %d %d", &x, &y, &mode);
		if (mode == 2)//标记雷：把*变成!，对已标记的坐标再操作一次则取消标记
		{
			if (x >= 1 && x <= row && y >= 1 && y <= col)//坐标合法
			{
				if (show[x][y] == '*')
				{
					show[x][y] = '!';
					printf("已在(%d,%d)插旗标记\n", x, y);
				}
				else if (show[x][y] == '!')
				{
					show[x][y] = '*';
					printf("已取消(%d,%d)的标记\n", x, y);
				}
				else
				{
					printf("该坐标已被排查，无法标记!\n");
				}
				DisplayBoard(show, row, col);
			}
			else//坐标非法
			{
				printf("非法坐标，清重新输入\n");
			}
			continue;
		}
		if (mode != 1)//操作码只能取1或2
		{
			printf("操作码错误，请输入1(排查)或2(标记)\n");
			continue;
		}
		if (x >= 1 && x <= row && y >= 1 && y <= col)//坐标合法
		{
			if (show[x][y] == '*')//坐标未排查过
			{
				//第一次排雷踩到雷时，把雷挪到别的安全格子，避免第一次就炸死
				if (win == 0 && mine[x][y] == '1')
				//用win是否为0判断是否第一次排雷，因为非法坐标、重复坐标都不会增加win的值
				{
					mine[x][y] = '0';
					while (1)
					{
						int mx = rand() % row + 1;
						int my = rand() % col + 1;
						if (mine[mx][my] == '0' && (mx != x || my != y))//排除原坐标，避免把雷挪回原地
						{
							mine[mx][my] = '1';
							break;
						}
					}
				}
				if (mine[x][y] != '1')//不是雷
				{
					//统计周围雷的个数，若周围没有雷则自动展开一片
					ExpandBoard(show, mine, row, col, x, y, &win);
					DisplayBoard(show, row, col);
					printf("已用时 %d 秒\n", (int)difftime(time(NULL), start));
				}
				else//是雷
				{
					printf("很遗憾，你被炸死了\n");
					printf("已用时 %d 秒\n", (int)difftime(time(NULL), start));
					DisplayBoard(show, row, col);
					break;
				}
			}
				else if (show[x][y] == '!')//坐标被标记
				{
					printf("该坐标已被标记，请先取消标记再排查!\n");
				}
				else//坐标被排查过
				{
					printf("该坐标已被排查!\n");
				}
		}
		else//坐标非法
		{
			printf("非法坐标，清重新输入\n");
		}
	}
	if (win == row * col - count)
	{
		printf("恭喜你，排雷成功!\n");
		printf("总用时 %d 秒\n", (int)difftime(time(NULL), start));
		DisplayBoard(show, row, col);
	}
}
