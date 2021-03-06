#pragma once
#include <array>
#include <stack>
#include "basic.h"
#include "assess.h"
using namespace std;

class moveRecord
{
public:
	moveRecord(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2), c1(cMap[y1][x1]), c2(cMap[y2][x2]) 
	{
		if (cMap[y2][x2] == 'X' || cMap[y2][x2] == 'L') //是敌方要备份敌方的概率数据
		{
			this->oldEne2 = ecOp::findChess(x2, y2);
			this->backupEne2 = oldEne2->copy();
		}
		if (cMap[y1][x1] == 'X' || cMap[y1][x1] == 'L')
		{
			this->oldEne1 = ecOp::findChess(x1, y1);
			this->backupEne1 = oldEne1->copy();
		}
	}
	int x1;
	int y1;
	int x2;
	int y2;
	char c1;
	char c2;
	enemyChess *backupEne1 = nullptr;
	enemyChess *oldEne1 = nullptr;
	enemyChess *backupEne2 = nullptr;
	enemyChess *oldEne2 = nullptr;

	void back()
	{
		cMap[y1][x1] = c1;
		cMap[y2][x2] = c2;
		if (oldEne1 != nullptr)
		{
			oldEne1->reset(backupEne1);
			delete backupEne1;
		}
		if (oldEne2 != nullptr)
		{
			oldEne2->reset(backupEne2);
			delete backupEne2;
		}
	}
};

class recordStack
{
public:
	static stack<moveRecord>allRecord;

	static int push(int x1, int y1, int x2, int y2, bool isEne) //压栈并模拟移动棋子，返回结果（0己方被对方吃，1对方被己方吃，2对死，3己方移动）
	{
		allRecord.push(moveRecord(x1, y1, x2, y2));

		//移动的合法性由策略判别，这里直接进行实际的移动操作
		char &newPos = cMap[y2][x2];
		char &oldPos = cMap[y1][x1];

		if (newPos == '0')
		{
			if (isEne)
			{
				enemyChess *e = ecOp::findChess(x1, y1);
				e->setPos(x2, y2);
			}
			newPos = oldPos;
			oldPos = '0';
			return 3;
		}
		else //目标地是我方棋子的情况策略中已经排除，下面处理敌方棋子
		{
			int result;
			if (isEne == false)
				result = assess::ChessComparisons(oldPos, ecOp::findChess(x2, y2)); //这个函数是有实际的我方/敌方的顺序的，敌方下要反过来
			else //这个情况下，新位置是实际的我方，老位置是实际的敌方
				result = assess::ChessComparisons(newPos, ecOp::findChess(x1, y1));

			switch (result) //根据不同结果修改棋盘
			{
			case 0: //实际的敌方胜
			{
				if (isEne) //老位置是实际的敌方，敌方前进到新位置成功
				{
					enemyChess *e = ecOp::findChess(x1, y1);
					e->setPos(x2, y2);
					newPos = oldPos;
					oldPos = '0';
				}
				else //老位置是实际的我方，前进失败
					oldPos = '0';
				break;
			}
			case 1: //实际的我方胜
			{
				if (!isEne) //老位置是实际的我方，我方前进到新位置成功
				{
					enemyChess *e = ecOp::findChess(x2, y2);
					e->setDie(true);
					newPos = oldPos;
					oldPos = '0';
				}
				else //老位置是实际的敌方，前进失败
				{
					enemyChess *e = ecOp::findChess(x1, y1);
					e->setDie(true);
					oldPos = '0';
				}
				break;
			}
			case 2: //对死
			{
				enemyChess *e;
				if (isEne) //老位置是敌方
				{
					e = ecOp::findChess(x1, y1);
				}
				else //新位置是敌方
				{
					e = ecOp::findChess(x2, y2);
				}
				e->setDie(true);
				newPos = '0';
				oldPos = '0';
				break;
			}
			}
			return result;
		}
	}

	static void pop() //回退一步棋
	{
		allRecord.top().back();
		allRecord.pop();
	}

	static void popAll() //回退所有棋
	{
		while (allRecord.empty() == false)
			pop();
	}
};
stack<moveRecord> recordStack::allRecord;
