#pragma once
/* ************************************************************************ */
/* 己方棋子编码约定:														*/
/*	a司令,b军长,c师长,d旅长,e团长,f营长,g连长,h排长,i工兵,j地雷k炸弹,l军旗      */
/* 对方方棋子编码约定:														*/
/*	A司令,B军长,C师长,D旅长,E团长,F营长,G连长,H排长,I工兵,J地雷K炸弹,L军旗      */
/*	X未知对方棋子,0空棋位													*/
/* ************************************************************************ */
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <string>
#include <functional>
#include <vector>
#include <tuple>
#include "reasoning.h"
using namespace std;

char cMap[12][5]; //棋盘
typedef function<int(int, int)> basicFun;
typedef tuple<int, int, int, int> moveTup;
typedef tuple<int, int> pos;

/* ************************************************************************ */
/* 函数功能：i,j位置是否本方棋子											*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1己方棋子，0空棋位或对方棋子											*/
/* ************************************************************************ */
int IsMyChess(int i, int j)
{
	if (cMap[i][j] >= 'a'&& cMap[i][j] <= 'l') //warn:crossline
		return 1;
	else
		return 0;
}

//是否为敌方棋子
int IsEmeChess(int i, int j)
{
	return cMap[i][j] == 'X';
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否本方可移动的棋子									*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1己方可移动棋子(司令,军长,...,工兵,炸弹)，0军旗,地雷,对方棋子或空棋位*/
/* ************************************************************************ */
int IsMyMovingChess(int i, int j)
{
	if (cMap[i][j] >= 'a' && cMap[i][j] <= 'i' || cMap[i][j] == 'k') //warn:crossline
		return 1;
	else
		return 0;
}

//是否为敌方可移动棋子
int IsEmeMovingChess(int i, int j)
{
	if (cMap[i][j] == 'X')
	{
		enemyChess* c = ecOp::findChess(j, i);
		int type = c->isDetermine();
		if (type == dilei || type == junqi)
			return 0;
		else
			return 1;
	}
	return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否山界后的兵站										*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1处于山界后，0不处于山界后											*/
/* ************************************************************************ */
int IsAfterHill(int i, int j)
{
	if (i * 5 + j == 31 || i * 5 + j == 33)
		return 1;
	else
		return 0;
}

int IsBeforeHill(int i, int j)
{
	if (i * 5 + j == 26 || i * 5 + j == 28)
		return 1;
	else
		return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否行营												*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1是行营，0不是行营													*/
/* ************************************************************************ */
int IsMoveCamp(int i, int j)
{
	if (i * 5 + j == 11 || i * 5 + j == 13 || i * 5 + j == 17 || i * 5 + j == 21 || i * 5 + j == 23 || i * 5 + j == 36 || i * 5 + j == 38 || i * 5 + j == 42 || i * 5 + j == 46 || i * 5 + j == 48)
		return 1;
	else
		return 0;
}
int IsMyMoveCamp(int i, int j)
{
	if (i * 5 + j == 36 || i * 5 + j == 38 || i * 5 + j == 42 || i * 5 + j == 46 || i * 5 + j == 48)
		return 1;
	else
		return 0;
}
int IsEnemyMoveCamp(int i, int j)
{
	if (i * 5 + j == 11 || i * 5 + j == 13 || i * 5 + j == 17 || i * 5 + j == 21 || i * 5 + j == 23)
		return 1;
	else
		return 0;
}
/* ************************************************************************ */
/* 函数功能：i,j位置是否大本营											*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1是大本营，0不是大本营												*/
/* ************************************************************************ */
int IsBaseCamp(int i, int j)
{
	if (i * 5 + j == 1 || i * 5 + j == 3 || i * 5 + j == 56 || i * 5 + j == 58)
		return 1;
	else
		return 0;
}
int IsMyBaseCamp(int i, int j)
{
	if (i * 5 + j == 56 || i * 5 + j == 58)
		return 1;
	else
		return 0;
}
int IsEnemyBaseCamp(int i, int j)
{
	if (i * 5 + j == 1 || i * 5 + j == 3)
		return 1;
	else
		return 0;
}
/* ************************************************************************ */
/* 函数功能：i,j位置是否有棋子占位的行营										*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1有棋子占位的行营,0不是行营或是空行营								*/
/* ************************************************************************ */
int IsFilledCamp(int i, int j)
{
	if (IsMoveCamp(i, j) && cMap[i][j] != '0') //warn:crossline
		return 1;
	else
		return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否有铁路          									*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1有铁路,0无铁路                      								*/
/* ************************************************************************ */
int IsAcrossRailway(int i)
{
	if (i == 1 || i == 5 || i == 6 || i == 10)
		return 1;
	else
		return 0;
}
int IsVerticalRailway(int i,int j)
{
	if ((j == 0 || j == 4)&&(i>0&&i<11))
		return 1;
	else
		return 0;
}
int IsEngineerRailway(int i, int j)
{
	if (j == 2 && (i == 5 || i == 6))
		return 1;
	else
		return 0;
}
int shortestpathtojunqi(int i, int j)
{
	return abs(11 - i) + abs(3 - j); //未考虑斜向路线
}
/* ************************************************************************ */
/* 函数功能：双方布局后棋局初始化（完成）										*/
/* 接口参数：																*/
/*     char *cOutMessage 布局字符序列										*/
/* ************************************************************************ */
void InitMap(string cOutMessage) //这个是用之前计算好的数据处理，所以是cOutMessage
{
	int i, j, k;
	for (i = 0;i < 6;i++)	//标记对方棋子
		for (j = 0;j < 5;j++)
			if (IsMoveCamp(i, j))
				cMap[i][j] = '0';
			else
				cMap[i][j] = 'X';
	k = 6;
	for (i = 6;i < 12;i++)	//标记己方棋子
		for (j = 0;j < 5;j++)
			if (IsMoveCamp(i, j))
				cMap[i][j] = '0';
			else
				cMap[i][j] = cOutMessage[k++];
}

vector<pos> getNearPos(int i, int j)
{
	vector<pos>result;
	//可以前移
	if (i > 0 && !IsAfterHill(i, j))
	{
		result.push_back(make_tuple(i - 1, j));
	}
	//可以左移
	if (j > 0)
	{
		result.push_back(make_tuple(i, j-1));
	}
	//可以右移
	if (j < 4)
	{
		result.push_back(make_tuple(i, j+1));
	}
	//可以后移
	if (i < 11 && !IsBeforeHill(i, j))
	{
		result.push_back(make_tuple(i+1, j));
	}
	//可以左上进行营
	if (IsMoveCamp(i - 1, j - 1))
	{
		result.push_back(make_tuple(i-1, j-1));
	}
	//可以右上进行营
	if (IsMoveCamp(i - 1, j + 1))
	{
		result.push_back(make_tuple(i-1, j+1));
	}
	//可以左下进行营
	if (IsMoveCamp(i + 1, j - 1))
	{
		result.push_back(make_tuple(i+1, j-1));
	}
	//可以右下进行营
	if (IsMoveCamp(i + 1, j + 1))
	{
		result.push_back(make_tuple(i+1, j+1));
	}
	if (IsMoveCamp(i, j))
	{
		result.push_back(make_tuple(i-1, j-1));
		result.push_back(make_tuple(i-1, j+1));
		result.push_back(make_tuple(i+1, j-1));
		result.push_back(make_tuple(i+1, j+1));
	}
	return result;
}