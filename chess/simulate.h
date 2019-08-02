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
		if (cMap[y2][x2] == 'X' || cMap[y2][x2] == 'L') //�ǵз�Ҫ���ݵз��ĸ�������
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

	static int push(int x1, int y1, int x2, int y2, bool isEne) //ѹջ��ģ���ƶ����ӣ����ؽ����0�������Է��ԣ�1�Է��������ԣ�2������3�����ƶ���
	{
		allRecord.push(moveRecord(x1, y1, x2, y2));

		//�ƶ��ĺϷ����ɲ����б�����ֱ�ӽ���ʵ�ʵ��ƶ�����
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
		else //Ŀ������ҷ����ӵ�����������Ѿ��ų������洦��з�����
		{
			int result;
			if (isEne == false)
				result = assess::ChessComparisons(oldPos, ecOp::findChess(x2, y2)); //�����������ʵ�ʵ��ҷ�/�з���˳��ģ��з���Ҫ������
			else //�������£���λ����ʵ�ʵ��ҷ�����λ����ʵ�ʵĵз�
				result = assess::ChessComparisons(newPos, ecOp::findChess(x1, y1));

			switch (result) //���ݲ�ͬ����޸�����
			{
			case 0: //ʵ�ʵĵз�ʤ
			{
				if (isEne) //��λ����ʵ�ʵĵз����з�ǰ������λ�óɹ�
				{
					enemyChess *e = ecOp::findChess(x1, y1);
					e->setPos(x2, y2);
					newPos = oldPos;
					oldPos = '0';
				}
				else //��λ����ʵ�ʵ��ҷ���ǰ��ʧ��
					oldPos = '0';
				break;
			}
			case 1: //ʵ�ʵ��ҷ�ʤ
			{
				if (!isEne) //��λ����ʵ�ʵ��ҷ����ҷ�ǰ������λ�óɹ�
				{
					enemyChess *e = ecOp::findChess(x2, y2);
					e->setDie(true);
					newPos = oldPos;
					oldPos = '0';
				}
				else //��λ����ʵ�ʵĵз���ǰ��ʧ��
				{
					enemyChess *e = ecOp::findChess(x1, y1);
					e->setDie(true);
					oldPos = '0';
				}
				break;
			}
			case 2: //����
			{
				enemyChess *e;
				if (isEne) //��λ���ǵз�
				{
					e = ecOp::findChess(x1, y1);
				}
				else //��λ���ǵз�
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

	static void pop() //����һ����
	{
		allRecord.top().back();
		allRecord.pop();
	}

	static void popAll() //����������
	{
		while (allRecord.empty() == false)
			pop();
	}
};
stack<moveRecord> recordStack::allRecord;
