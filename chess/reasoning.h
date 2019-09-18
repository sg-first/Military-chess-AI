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
const float minUpdate = 0.5;

class enemyChess
{
private:
    void changeProbNum(unsigned int sub, float d) //改变从属某类棋子的概率分
    {
		if (prob[sub] > minUpdate && this->isDetermine() == -1) //minUpdate那个等于也不改所以是大于，而小于是被视为0
		{
			prob[sub] -= d;
			if (prob[sub] < minUpdate) //防止导致nan的整体密度1出现
				prob[sub] = minUpdate;
			//防止导致nan的整体密度1出现
			while (sum() <= 0.6)
			{
				for (float &i : prob)
					i *= 2;
			}
		}
    }

	static float getSubSum(unsigned int sub)
	{
		//在都已经归一化完的情况下这个才有效
		float sum = 0;
		for (enemyChess* i : allEnemyChess)
			sum += i->prob[sub];
		return sum;
	}

	void setProbNum(unsigned int sub, float d)
	{
		if (d == 0)
		{
			for (enemyChess* i : allEnemyChess) //要做下面的计算得先对所有的归一化
				i->normalization();
			for (enemyChess* i : allEnemyChess)
			{
				if (i != this && i->prob[sub]>minUpdate && this->isDetermine() == -1)
				{
					//X:更新的子是某种棋
					//F:之前碰的子不是某种棋
					float PF = 1 - prob[sub];
					float PX = i->prob[sub];
					float sum = getSubSum(sub);//所有是某种棋的和
					float PF_X = PF / (sum - PX);
					float PX2 = (PF_X * PX) / PF;
					i->prob[sub] = PX2;
				}
			}
		}
		prob[sub] = d;
	}

    void otherDie(enemyChess *thatChess) //其它棋子死去导致本棋子概率分布变化
    {
		if (this->isDetermine() == -1)
		{
			float fenzi = 1 / thatChess->sum();
			//对prob的所有维度减去d*prob[i]
			for (unsigned int i = 0; i < prob.size(); i++)
				changeProbNum(i, thatChess->prob[i] * fenzi);
		}
    }

public:
    enemyChess(int x, int y) : x(x), y(y) { this->prob = { 1,3,3,3,2,2,2,2,1,1,3,2 }; }
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
            //本棋子死亡，会导致【其它】棋子的概率分布变化
			if (this->isDetermine() == -1) //死亡的棋子确定意味着之前已经给其它棋子施加了它确定的影响,所以要不确定时
			{
				for (enemyChess* i : allEnemyChess) //要做下面的计算得先对所有的归一化
					i->normalization();
				for (enemyChess* i : allEnemyChess)
				{
					if (i != this)
						i->otherDie(this);
				}
			}
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
			setProbNum(type, 1);
            //新产生确定的棋子，会导致其它棋子的概率分布变化
            for (enemyChess *i : allEnemyChess)
            {
                if (i != this)
                    i->changeProbNum(type, 1);
            }
			if (type == junqi) //是军棋要标记上
				enemyChess::junqiEne = this;
        }
    }

    int isDetermine() //返回当前棋子的确定类型，-1为不确定
    {
        int type = -1;
        for (unsigned int i = 0;i < prob.size();i++)
        {
			if (type == -1 && !(prob[i] < minUpdate)) //比minUpdate都小说明肯定是设置的0
			{
				type = i;
				continue;
			}
			if (type != -1 && !(prob[i] < minUpdate))
				return -1;
        }
		if (type != -1)
			setProbNum(type, 1);
        return type;
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
            float sum = this->sum();
            float plan[12];
            float average = 0;
            float variance = 0;
            for (unsigned int p = 0; p < prob.size(); p++)
            {
                plan[p] = this->prob[p] / sum;
                average = average + plan[p];
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
