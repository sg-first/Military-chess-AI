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
	void setProbNum(unsigned int sub, float d)
	{
		if (this->prob[sub] != d && isDetermine() == -1)
		{
			for (enemyChess* i : allEnemyChess) //要做下面的计算得先对所有的归一化
				i->normalization();
			this->prob[sub] = d;
			//this->normalization(); //设的不是0就是1，设完不用再归一化了
			subNormalization(sub); //再对sub归一化
		}
	}

	void outputProb()
	{
		string content = "";
		for (auto i : prob)
			content += to_string(i) + " ";
		writeFile("特种兵的日记.txt", content);
	}

public:
	static void subNormalization(unsigned int sub)
	{
		float sum = 0;
		float k = enemyChess::chessProb[sub];
		for (enemyChess* i : allEnemyChess)
		{
			if (i->prob[sub] != 1)
				sum += i->prob[sub];
			else
				k -= 1;
		}
		for (enemyChess* i : allEnemyChess)
		{
			if (i->prob[sub] != 1)
			{
				if (sum != 0)
					i->prob[sub] = i->prob[sub] / sum * k;
				else
					i->prob[sub] = 0;
			}
		}
	}

	static array<int,12> chessProb;
    enemyChess(int x, int y) : x(x), y(y) 
	{ 
		this->prob = { 1,3,3,3,2,2,2,2,1,1,3,2 };
		for (float& i : prob)
			i /= 25;
	}
    int x;
    int y;
    bool isDie = false;
    array<float, 12>prob;

    static enemyChess* junqiEne; //军棋确定把指针放这里

    float sum()
    {
        float sum = 0;
        for (float i : prob)
            sum += i;
        return sum;
    }

    void equ(int type,bool sim=false) //该棋子与某棋同尽（也就是被吃了）
    {
        if (type != zhadan)
        {
            if (type == dilei) //我方是地雷，只能与炸弹同尽
                determine(zhadan);
            else
                determine(type); //除此情况之外就一样
        }
		setDie(sim);
    }

    void less(int type,bool sim=false) //设定该棋子小于某棋（也就是被吃了）
    {
        if (type != dilei && type != zhadan) //不可能小于地雷炸弹
        {
			setProbNum(zhadan, 0); //既然比别人小肯定不是炸弹
            for (int i = ALL;i > type;i--)
				setProbNum(i, 0); //把大于type的所有概率分清零
			setDie(sim);
        }
    }

    void more(int type) //设定该棋子大于某棋
    {
		if (type != zhadan) //不可能吃掉炸弹
		{
			setProbNum(zhadan, 0);
			if (type == siling)
				determine(dilei); //把司令吃了一定是地雷
			else if (type == dilei) //把地雷吃了一定是工兵
				determine(gongbing);
			else
			{
				for (int i = 0;i < type;i++)
					setProbNum(i, 0); //把小于type的所有概率分清零
			}
		}
    }

    void setDie(bool sim)
    {
        if (!isDie)
        {
            isDie = true;
            if(!sim)
                aliveChess--;
			setPos(-1, -1);
        }
    }

    void determine(unsigned int sub) //确认该棋子为某棋
    {
        if (isDetermine() != -1) //如果当前棋子已经确定
            return; //不再处理
        else
        {
			for (unsigned int i = 0; i < prob.size(); i++)
			{
				if (i != sub)
					setProbNum(i, 0);
			}
			setProbNum(sub, 1); //此时相当于已经对概率向量归一化（里面会调用subNormalization）
			if (sub == junqi) //是军棋要标记上
				enemyChess::junqiEne = this;
        }
    }

    int isDetermine() //返回当前棋子的确定类型，-1为不确定
    {
        int sub = -1;
        for (unsigned int i = 0;i < prob.size();i++)
        {
			if (sub == -1 && prob[i] != 0)
			{
				sub = i;
				continue;
			}
			if (sub != -1 && prob[i] != 0)
				return -1;
        }
		if (sub != -1)
			prob[sub] = 1;
        return sub;
    }

    void setPos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

	void normalization()
	{
		float sum = this->sum();
		if (sum != 1)
		{
			for (float& i : prob)
				i = i / sum;
		}
	}

    float certainty() //返回当前棋子类型评估的确定性，整体不确定性用来动态确定search_depth
    {
        if (this->isDie)
            return 0;
        else
        {
            //使用标准差计算不确定度
			this->normalization();
            float plan[12];
            float average = 0;
            float variance = 0;
            for (unsigned int p = 0; p < prob.size(); p++)
            {
                average = average + this->prob[p];
            }
            average = average / prob.size();
            for (unsigned int p = 0; p < prob.size(); p++)
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
array<int, 12> enemyChess::chessProb;

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
        return sum / aliveChess;
    }

    static void adjustDepth() //fix:深度动态调整暂时取消
    {
        /*search_depth = 33.33*avgCertainty() + 1;
        if (search_depth % 2 != 0) //必须是偶数层
            search_depth++;
        writeFile("特种兵的日记.txt", "调整到的搜索深度为："+to_string(search_depth));*/
    }
};
int ecOp::search_depth;
