#pragma once
/* ************************************************************************ */
/* �������ӱ���Լ��:														*/
/*	a˾��,b����,cʦ��,d�ó�,e�ų�,fӪ��,g����,h�ų�,i����,j����kը��,l����      */
/* �Է������ӱ���Լ��:														*/
/*	A˾��,B����,Cʦ��,D�ó�,E�ų�,FӪ��,G����,H�ų�,I����,J����Kը��,L����      */
/*	Xδ֪�Է�����,0����λ													*/
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

char cMap[12][5]; //����
typedef function<int(int, int)> basicFun;
typedef tuple<int, int, int, int> moveTup;
typedef tuple<int, int> pos;

/* ************************************************************************ */
/* �������ܣ�i,jλ���Ƿ񱾷�����											*/
/* �ӿڲ�����																*/
/*     char cMap[12][5] ���̾���											*/
/*     int i,j ����λ�����к�												*/
/* ����ֵ��																	*/
/*     1�������ӣ�0����λ��Է�����											*/
/* ************************************************************************ */
int IsMyChess(int i, int j)
{
	if (cMap[i][j] >= 'a'&& cMap[i][j] <= 'l') //warn:crossline
		return 1;
	else
		return 0;
}

//�Ƿ�Ϊ�з�����
int IsEmeChess(int i, int j)
{
	return cMap[i][j] == 'X';
}

/* ************************************************************************ */
/* �������ܣ�i,jλ���Ƿ񱾷����ƶ�������									*/
/* �ӿڲ�����																*/
/*     char cMap[12][5] ���̾���											*/
/*     int i,j ����λ�����к�												*/
/* ����ֵ��																	*/
/*     1�������ƶ�����(˾��,����,...,����,ը��)��0����,����,�Է����ӻ����λ*/
/* ************************************************************************ */
int IsMyMovingChess(int i, int j)
{
	if (cMap[i][j] >= 'a' && cMap[i][j] <= 'i' || cMap[i][j] == 'k') //warn:crossline
		return 1;
	else
		return 0;
}

//�Ƿ�Ϊ�з����ƶ�����
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
/* �������ܣ�i,jλ���Ƿ�ɽ���ı�վ										*/
/* �ӿڲ�����																*/
/*     int i,j ����λ�����к�												*/
/* ����ֵ��																	*/
/*     1����ɽ���0������ɽ���											*/
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
/* �������ܣ�i,jλ���Ƿ���Ӫ												*/
/* �ӿڲ�����																*/
/*     int i,j ����λ�����к�												*/
/* ����ֵ��																	*/
/*     1����Ӫ��0������Ӫ													*/
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
/* �������ܣ�i,jλ���Ƿ��Ӫ											*/
/* �ӿڲ�����																*/
/*     int i,j ����λ�����к�												*/
/* ����ֵ��																	*/
/*     1�Ǵ�Ӫ��0���Ǵ�Ӫ												*/
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
/* �������ܣ�i,jλ���Ƿ�������ռλ����Ӫ										*/
/* �ӿڲ�����																*/
/*     char cMap[12][5] ���̾���											*/
/*     int i,j ����λ�����к�												*/
/* ����ֵ��																	*/
/*     1������ռλ����Ӫ,0������Ӫ���ǿ���Ӫ								*/
/* ************************************************************************ */
int IsFilledCamp(int i, int j)
{
	if (IsMoveCamp(i, j) && cMap[i][j] != '0') //warn:crossline
		return 1;
	else
		return 0;
}

/* ************************************************************************ */
/* �������ܣ�i,jλ���Ƿ�����·          									*/
/* �ӿڲ�����																*/
/*     char cMap[12][5] ���̾���											*/
/*     int i,j ����λ�����к�												*/
/* ����ֵ��																	*/
/*     1����·,0����·                      								*/
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
	return abs(11 - i) + abs(3 - j); //δ����б��·��
}
/* ************************************************************************ */
/* �������ܣ�˫�����ֺ���ֳ�ʼ������ɣ�										*/
/* �ӿڲ�����																*/
/*     char *cOutMessage �����ַ�����										*/
/* ************************************************************************ */
void InitMap(string cOutMessage) //�������֮ǰ����õ����ݴ���������cOutMessage
{
	int i, j, k;
	for (i = 0;i < 6;i++)	//��ǶԷ�����
		for (j = 0;j < 5;j++)
			if (IsMoveCamp(i, j))
				cMap[i][j] = '0';
			else
				cMap[i][j] = 'X';
	k = 6;
	for (i = 6;i < 12;i++)	//��Ǽ�������
		for (j = 0;j < 5;j++)
			if (IsMoveCamp(i, j))
				cMap[i][j] = '0';
			else
				cMap[i][j] = cOutMessage[k++];
}

vector<pos> getNearPos(int i, int j)
{
	vector<pos>result;
	//����ǰ��
	if (i > 0 && !IsAfterHill(i, j))
	{
		result.push_back(make_tuple(i - 1, j));
	}
	//��������
	if (j > 0)
	{
		result.push_back(make_tuple(i, j-1));
	}
	//��������
	if (j < 4)
	{
		result.push_back(make_tuple(i, j+1));
	}
	//���Ժ���
	if (i < 11 && !IsBeforeHill(i, j))
	{
		result.push_back(make_tuple(i+1, j));
	}
	//�������Ͻ���Ӫ
	if (IsMoveCamp(i - 1, j - 1))
	{
		result.push_back(make_tuple(i-1, j-1));
	}
	//�������Ͻ���Ӫ
	if (IsMoveCamp(i - 1, j + 1))
	{
		result.push_back(make_tuple(i-1, j+1));
	}
	//�������½���Ӫ
	if (IsMoveCamp(i + 1, j - 1))
	{
		result.push_back(make_tuple(i+1, j-1));
	}
	//�������½���Ӫ
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