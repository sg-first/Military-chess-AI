#pragma once
#include <vector>
#include <array>
#include <math.h>
#include "help.h"
using namespace std;

const int siling = 9;
const int junzhang = 8;
const int shizhang = 7;
const int lvzhang = 6;
const int tuanzhang = 5;
const int yingzhang = 4;
const int lianzhang = 3;
const int paizhang = 2;
const int gongbing = 1;
const int junqi = 0;
#define ALL 9
const int dilei = 10;
const int zhadan = 11;

#define maxDepth 10
#define minDepth 2

class enemyChess;

vector<enemyChess*> allEnemyChess;
int aliveChess = 25;

class enemyChess
{
private:
	void changeProbNum(int sub, int d) //�ı����ĳ�����ӵĸ��ʷ�
	{
		prob[sub] -= d;
		if (d < 0) //���ʷֲ���Ϊ��
			prob[sub] = 0;
	}

	void otherDie(enemyChess *thatChess) //����������ȥ���±����Ӹ��ʷֲ��仯
	{
		//�����ǰ�����Ѿ�ȷ�����������Ǹ������Ѿ�ȷ��������������ȷ����ζ��֮ǰ�Ѿ�����������ʩ������ȷ����Ӱ�죩
		if (this->isDetermine() == -1 && thatChess->isDetermine() == -1)
		{
			float sum = thatChess->sum();
			float d = 1 / sum;
			//��prob������ά�ȼ�ȥd*prob[i]
			for (int i = 0; i < prob.size(); i++)
				changeProbNum(i, thatChess->prob[i] * d);
		}
	}

public:
	enemyChess(int x, int y) : x(x), y(y) { this->prob = { 1,3,3,3,2,2,2,2,1,1,3,2 }; }
	int x;
	int y;
	bool isDie = false;
	array<float, 12>prob;

	static enemyChess* junqiEne; //����ȷ����ָ�������

	float sum()
	{
		float sum = 0;
		for (float i : prob)
			sum += i;
		return sum;
	}

	void equ(int type,bool sim=false) //��������ĳ��ͬ����Ҳ���Ǳ����ˣ�
	{
		setDie(sim);
		if (type != zhadan)
		{
			if (type == dilei) //�ҷ��ǵ��ף�ֻ����ը��ͬ��
				determine(zhadan);
			else
				determine(type); //�������֮���һ��
		}
	}

	void less(int type,bool sim=false) //�趨������С��ĳ�壨Ҳ���Ǳ����ˣ�
	{
		setDie(sim);
		if (type != dilei && type != zhadan) //��֧��С�ڵ���ը��
		{
			for (int i = ALL;i > type;i--)
				prob[i] = 0; //�Ѵ���type�����и��ʷ�����
		}
	}

	void more(int type) //�趨�����Ӵ���ĳ��
	{
		prob[zhadan] = 0;

		if (type == zhadan) //�����ܳԵ�ը��
			return;
		if (type == siling)
			determine(dilei); //��˾�����һ���ǵ���
		else if (type == dilei) //�ѵ��׳���һ���ǹ���
			determine(gongbing);
		else
		{
			for (int i = 0;i < type;i++)
				prob[i] = 0; //��С��type�����и��ʷ�����
		}
	}

	void setDie(bool sim)
	{
		if (!isDie)
		{
			isDie = true;
			if(!sim)
				aliveChess--;
			x = -1;
			y = -1;
			//�������������ᵼ�¡����������ӵĸ��ʷֲ��仯
			for (enemyChess *i : allEnemyChess)
			{
				if (i != this)
					i->otherDie(this);
			}
		}
	}

	void determine(int type) //ȷ�ϸ�����Ϊĳ��
	{
		if (isDetermine() != -1) //�����ǰ�����Ѿ�ȷ��
			return; //���ٴ���
		else
		{
			if (type == junqi) //�Ǿ���Ҫ�����
				enemyChess::junqiEne = this;

			for (float &i : prob)
				i = 0;
			prob[type] = 1;
			//�²���ȷ�������ӣ��ᵼ�¡����������ӵĸ��ʷֲ��仯
			for (enemyChess *i : allEnemyChess)
			{
				if (i != this)
					i->changeProbNum(type, 1);
			}
		}
	}

	int isDetermine() //���ص�ǰ���ӵ�ȷ�����ͣ�-1Ϊ��ȷ��
	{
		int type = -1;
		for (unsigned int i = 0;i < prob.size();i++)
		{
			if (type == -1 && prob[i] != 0)
				type = i;
			if (type != -1 && prob[i] != 0)
				return -1;
		}
		return type;
	}

	void setPos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	float certainty() //���ص�ǰ��������������ȷ���ԣ����岻ȷ����������̬ȷ��search_depth
	{
		if (this->isDie)
			return 0;
		else
		{
			//ʹ�ñ�׼����㲻ȷ����
			float sum = this->sum();
			float plan[12];
			float average = 0;
			float variance = 0;
			for (int p = 0; p < prob.size(); p++)
			{
				plan[p] = this->prob[p] / sum;
				average = average + plan[p];
			}
			average = average / prob.size();
			for (int p = 0; p < prob.size(); p++)
				variance = variance + (plan[p] - average)*(plan[p] - average);
			variance = variance / prob.size();
			return sqrt(variance);
		}
	}

	enemyChess* copy()
	{
		enemyChess *newe = new enemyChess(this->x, this->y);
		newe->isDie = this->isDie;
		newe->prob = this->prob;
		return newe;
	}

	void reset(enemyChess *e)
	{
		this->x = e->x;
		this->y = e->y;
		this->isDie = e->isDie;
		this->prob = e->prob;
	}
};
enemyChess* enemyChess::junqiEne;

class ecOp
{
public:
	static int search_depth;

	static enemyChess* findChess(int x, int y) //��������һ�����Ӷ���
	{
		for (enemyChess* i : allEnemyChess)
		{
			if (i->x == x && i->y == y)
				return i;
		}
		return nullptr;
	}

	static int codeToType(char code)
	{
		if (code == 'a')
			return siling;
		if (code == 'b')
			return junzhang;
		if (code == 'c')
			return shizhang;
		if (code == 'd')
			return lvzhang;
		if (code == 'e')
			return tuanzhang;
		if (code == 'f')
			return yingzhang;
		if (code == 'g')
			return lianzhang;
		if (code == 'h')
			return paizhang;
		if (code == 'i')
			return gongbing;
		if (code == 'j')
			return dilei;
		if (code == 'k')
			return zhadan;
		if (code == 'l' || code == 'L')
			return junqi;
		return -1;
	}

	static float avgCertainty()
	{
		float sum = 0;
		for (enemyChess* c : allEnemyChess)
			sum += c->certainty();
		return sum / aliveChess;
	}

	static void adjustDepth()
	{
		/*search_depth = 33.33*avgCertainty() + 1;
		if (search_depth % 2 != 0) //������ż����
			search_depth++;
		writeFile("���ֱ����ռ�.txt", "���������������Ϊ��"+to_string(search_depth));*/
	}
};
int ecOp::search_depth;
