#pragma once
#include <vector>
#include <array>
#include <math.h>
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
#define SIZE 12

#define maxDepth 10
#define minDepth 2

class enemyChess;

vector<enemyChess*> allEnemyChess;

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
		if (isDetermine() != -1) //�����ǰ�����Ѿ�ȷ��
			return; //���ٴ���
		{
			float sum = thatChess->sum();
			float d = 1 / sum;
			//��prob������ά�ȼ�ȥd
			for (int i = 0; i < SIZE; i++)
				changeProbNum(i, thatChess->prob[i] * d);
		}
	}

public:
	enemyChess(int x, int y) : x(x), y(y) { this->prob = { 1,3,3,3,2,2,2,2,1,1,3,2 }; }
	int x;
	int y;
	bool isDie = false;
	array<float, 12>prob;

	float sum()
	{
		float sum = 0;
		for (float i : prob)
			sum += i;
		return sum;
	}

	void equ(int type) //��������ĳ��ͬ����Ҳ���Ǳ����ˣ�
	{
		setDie();
		if (type != zhadan)
		{
			if (type == dilei) //�ҷ��ǵ��ף�ֻ����ը��ͬ��
				determine(zhadan);
			else
				determine(type); //�������֮���һ��
		}
	}

	void less(int type) //�趨������С��ĳ�壨Ҳ���Ǳ����ˣ�
	{
		setDie();
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

	void setDie()
	{
		isDie = true;
		x = -1;
		y = -1;
		//�������������ᵼ�¡����������ӵĸ��ʷֲ��仯
		for (enemyChess *i : allEnemyChess)
		{
			if (i != this)
				i->otherDie(this);
		}
	}

	void determine(int type) //ȷ�ϸ�����Ϊĳ��
	{
		if (isDetermine() != -1) //�����ǰ�����Ѿ�ȷ��
			return; //���ٴ���
		else
		{
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
		//ʹ�ñ�׼����㲻ȷ����
		float sum = this->sum();
		float plan[SIZE];
		float average = 0;
		float variance = 0;
		for (int p = 0;p < SIZE;p++)
		{
			plan[p] = this->prob[p] / sum;
			average = average + plan[p];
		}
		average = average / SIZE;
		for (int p = 0;p < SIZE;p++)
			variance = variance + (plan[p] - average)*(plan[p] - average);
		variance = variance / SIZE;
		return sqrt(variance);
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
		return sum / allEnemyChess.size();
	}

	static void adjustDepth()
	{
		search_depth = 33.33*avgCertainty() + 1;
	}
};
int ecOp::search_depth;
