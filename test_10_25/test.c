#define _CRT_SECURE_NO_WARNINGS 1
#include "game.h"

//打印菜单
void meun()
{
	printf("--------------------------\n");
	printf("-------1.开始游戏---------\n");
	printf("-------0.退出游戏---------\n");
	printf("--------------------------\n");
}

//游戏核心逻辑
void game()
{
	char mine[ROWS][COLS];//存放布置雷的信息
	char show[ROWS][COLS];//存放排查出的雷的信息

	int row = 0;
	int col = 0;
	int count = 0;
	int level = 0;

	//选择难度
	printf("请选择难度:>\n");
	printf("1.简单(9*9, %d雷)  2.中等(16*16, %d雷)  3.困难(30*16, %d雷)\n", EASY_GAME, MIDDLE_GAME, DIFFICULT_GAME);
	if (scanf("%d", &level) != 1)
	{
		//清空未读取的信息
		while (getchar() != '\n')
		{
			;
		}
		printf("选择错误，返回主菜单\n");
		return;
	}
	switch (level)
	{
	case 1:
		row = 9; col = 9; count = EASY_GAME;
		break;
	case 2:
		row = 16; col = 16; count = MIDDLE_GAME;
		break;
	case 3:
		row = 16; col = 30; count = DIFFICULT_GAME;
		break;
	default:
		printf("选择错误，返回主菜单\n");
		return;
	}

	//初始化雷的信息
	InitBoard(mine, ROWS, COLS, '0');
	//初始化显示信息
	InitBoard(show, ROWS, COLS, '*');

	//打印雷的信息
	DisplayBoard(show, row, col);
	//布置雷
	SetMine(mine, row, col, count);

	//排查雷
	FindMine(show, mine, row, col, count);
}

int main(void)
{
	srand((unsigned int)time(NULL));
	int input = 2;
	do
	{
		meun();
		printf("请选择:>");
		if (scanf("%d", &input) != 1)
		{
			printf("选择错误，请输入0或1\n");
			//清空前面的输入，但是未读取的信息
			while (getchar() != '\n')
			{
				;
			}
			input = 2;
			continue;
		}
		switch (input)
		{
		case 1:
			game();
			break;
		case 0:
			printf("退出游戏\n");
			break;
		default:
			printf("选择错误，请输入0或1\n");
		}
	} while (input);
	return 0;
}