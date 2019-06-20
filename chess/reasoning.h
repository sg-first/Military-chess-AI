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
	void changeProbNum(int sub, int d) //改变从属某类棋子的概率分
	{
		prob[sub] -= d;
		if (d < 0) //概率分不能为负
			prob[sub] = 0;
	}

	void otherDie(enemyChess *thatChess) //其它棋子死去导致本棋子概率分布变化
	{
		if (isDetermine() != -1) //如果当前棋子已经确定
			return; //不再处理
		{
			float sum = thatChess->sum();
			float d = 1 / sum;
			//对prob的所有维度减去d
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

	void equ(int type) //该棋子与某棋同尽（也就是被吃了）
	{
		setDie();
		if (type != zhadan)
		{
			if (type == dilei) //我方是地雷，只能与炸弹同尽
				determine(zhadan);
			else
				determine(type); //除此情况之外就一样
		}
	}

	void less(int type) //设定该棋子小于某棋（也就是被吃了）
	{
		setDie();
		if (type != dilei && type != zhadan) //不支持小于地雷炸弹
		{
			for (int i = ALL;i > type;i--)
				prob[i] = 0; //把大于type的所有概率分清零
		}
	}

	void more(int type) //设定该棋子大于某棋
	{
		prob[zhadan] = 0;

		if (type == zhadan) //不可能吃掉炸弹
			return;
		if (type == siling)
			determine(dilei); //把司令吃了一定是地雷
		else if (type == dilei) //把地雷吃了一定是工兵
			determine(gongbing);
		else
		{
			for (int i = 0;i < type;i++)
				prob[i] = 0; //把小于type的所有概率分清零
		}
	}

	void setDie()
	{
		isDie = true;
		x = -1;
		y = -1;
		//本棋子死亡，会导致【其它】棋子的概率分布变化
		for (enemyChess *i : allEnemyChess)
		{
			if (i != this)
				i->otherDie(this);
		}
	}

	void determine(int type) //确认该棋子为某棋
	{
		if (isDetermine() != -1) //如果当前棋子已经确定
			return; //不再处理
		else
		{
			for (float &i : prob)
				i = 0;
			prob[type] = 1;
			//新产生确定的棋子，会导致【其它】棋子的概率分布变化
			for (enemyChess *i : allEnemyChess)
			{
				if (i != this)
					i->changeProbNum(type, 1);
			}
		}
	}

	int isDetermine() //返回当前棋子的确定类型，-1为不确定
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

	float certainty() //返回当前棋子类型评估的确定性，整体不确定性用来动态确定search_depth
	{
		//使用标准差计算不确定度
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

	static enemyChess* findChess(int x, int y) //给坐标找一个棋子对象
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
