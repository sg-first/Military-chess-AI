#pragma once
#include <array>
#include <stack>
#include "basic.h"
#include "assess.h"
using namespace std;

class moveRecord
{
public:
	moveRecord(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2), c1(cMap[y1][x1]), c2(cMap[y2][x2]) {}
	int x1;
	int y1;
	int x2;
	int y2;
	char c1;
	char c2;

	void back()
	{
		cMap[y1][x1] = c1;
		cMap[y2][x2] = c2;
	}
};

class recordStack
{
public:
	static stack<moveRecord>allRecord;

	static int push(int x1, int y1, int x2, int y2, bool isEmc) //ѹջ��ģ���ƶ����ӣ����ؽ����0�������Է��ԣ�1�Է��������ԣ�2������3�����ƶ���
	{
		allRecord.push(moveRecord(x1, y1, x2, y2));

		//�ƶ��ĺϷ����ɲ����б�����ֱ�ӽ���ʵ�ʵ��ƶ�����
		char &newPos = cMap[y2][x2];
		char &oldPos = cMap[y1][x1];

		if (newPos == '0')
		{
			newPos = oldPos;
			oldPos = '0';
			return 3;
		}
		else //�ҷ����ӵ�����������Ѿ��ų������洦��з�����
		{
			int result;
			if (isEmc == false)
				result = assess::ChessComparisons(oldPos, ecOp::findChess(x2, y2)); //�����������ʵ�ʵ��ҷ�/�з���˳��ģ��з���Ҫ������
			else //�������£���λ����ʵ�ʵ��ҷ�����λ����ʵ�ʵĵз�
				result = assess::ChessComparisons(newPos, ecOp::findChess(x1, y1));

			switch (result) //���ݲ�ͬ����޸�����
			{
			case 0: //ʵ�ʵĵз�ʤ
			{
				if (isEmc == true) //��λ����ʵ�ʵĵз���ǰ������λ�óɹ�
				{
					newPos = oldPos;
					oldPos = '0';
				}
				else
					oldPos = '0';
				break;
			}
			case 1: //ʵ�ʵ��ҷ�ʤ
			{
				if (isEmc == false) //��λ����ʵ�ʵ��ҷ���ǰ������λ�óɹ�
				{
					newPos = oldPos;
					oldPos = '0';
				}
				else
					oldPos = '0';
				break;
			}
			case 2: //����
			{
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
