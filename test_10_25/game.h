#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//棋盘按最大难度开空间，实际大小由所选难度决定
#define ROW 16//最大行数(困难)
#define COL 30//最大列数(困难)

#define ROWS ROW + 2
#define COLS COL + 2

#define EASY_GAME 10//简单：10个雷
#define MIDDLE_GAME 40//中等：40个雷
#define DIFFICULT_GAME 99//困难：99个雷

//初始化棋盘
void InitBoard(char board[ROWS][COLS], int row, int col, char set);
 
//打印棋盘
void DisplayBoard(char board[ROWS][COLS], int row, int col);

//布置雷
void SetMine(char mine[ROWS][COLS], int row, int col, int count);
//排查雷
void FindMine(char show[ROWS][COLS], char mine[ROWS][COLS], int row, int col, int count);
