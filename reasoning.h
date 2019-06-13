#pragma once
#include <vector>
#include <array>
using namespace std;

const int siling=8;
const int junzhang=7;
const int shizhang=6;
const int lvzhang=5;
const int tuanzhang=4;
const int yingzhang=3;
const int lianzhang=2;
const int paizhang=1;
const int gongbing=0;
#define all 9
const int dilei=9;
const int zhadan=10;
const int junqi=11;

//暗棋推理：得为对方的每个子确定ID
//对方每个子ID=初始x+初始y
//查询需要的是：从目前位置查ID，从ID确定概率分布

class enemyChess;

vector<enemyChess*> allEnemyChess;

class enemyChess
{
private:
    void changeProbNum(int sub,int d) //改变从属某类棋子的概率分
    {
        prob[sub]-=d;
        if(d<0) //概率分不能为负
            prob[sub]=0;
    }

    void otherDie() //其它棋子死去导致本棋子概率分布变化
    {
        if(isDetermine()!=-1) //如果当前棋子已经确定
            return; //不再处理
        {
            float sum=this->sum();
            float d=1/sum;
            //对prob的所有维度减去d
            for(int i=0;i<junqi+1;i++)
                changeProbNum(i,d);
        }
    }

public:
    enemyChess(int x,int y) : x(x),y(y) { this->prob={3,3,3,2,2,2,2,1,1,3,2,1}; }
    int x;
    int y;
    bool isDie=false;
    array<float,12>prob;

    float sum()
    {
        float sum=0;
        for(float i : prob)
            sum+=i;
        return sum;
    }

    void equ(int type) //该棋子与某棋同尽（也就是被吃了）
    {
        setDie();
        if(type==dilei) //我方是地雷，只能与炸弹同尽
            determine(zhadan);
        else
            determine(type); //除此情况之外就一样
    }

    void less(int type) //设定该棋子小于某棋（也就是被吃了）
    {
        setDie();
        if(type!=dilei && type!=zhadan) //不支持小于地雷炸弹
        {
            for(int i=all-1;i>type;i--)
                prob[i]=0; //把大于type的所有概率分清零
        }
    }

    void more(int type) //设定该棋子大于某棋
    {
        prob[junqi]=0;
        prob[zhadan]=0;

        if(type==zhadan) //不可能吃掉炸弹
            return;
        if(type==siling)
            determine(dilei); //把司令吃了一定是地雷
        else if(type==dilei) //把地雷吃了一定是工兵
            determine(gongbing);
        else
        {
            for(int i=0;i<type;i++)
                prob[i]=0; //把小于type的所有概率分清零
        }
    }

    void setDie()
    {
        isDie=true;
        x=-1;
        y=-1;
        //本棋子死亡，会导致【其它】棋子的概率分布变化
        for(enemyChess *i : allEnemyChess)
        {
            if(i!=this)
                i->otherDie();
        }
    }

    void determine(int type) //确认该棋子为某棋
    {
        if(isDetermine()!=-1) //如果当前棋子已经确定
            return; //不再处理
        else
        {
            for(float &i : prob)
                i=0;
            prob[type]=1;
            //新产生确定的棋子，会导致【其它】棋子的概率分布变化
            for(enemyChess *i : allEnemyChess)
            {
                if(i!=this)
                    i->changeProbNum(type,1);
            }
        }
    }

    int isDetermine() //返回当前棋子的确定类型，-1为不确定
    {
        int type=-1;
        for(unsigned int i=0;i<prob.size();i++)
        {
            if(type==-1 && prob[i]!=0)
                type=i;
            if(type!=-1 && prob[i]!=0)
                return -1;
        }
        return type;
    }

    void setPos(int x,int y)
    {
        this->x=x;
        this->y=y;
    }
};


class ecOp
{
public:
    static enemyChess* findChess(int x,int y) //给坐标找一个棋子对象
    {
        for(enemyChess* i : allEnemyChess)
        {
            if(i->x==x && i->y==y)
                return i;
        }
        return nullptr;
    }

    static void init() //开局时初始化所有敌方棋子对象
    {
        //x为0-4，y为A-F（0-5）
        for(int i=0;i<=4;i++)
        {
            for(int j=0;j<=5;j++)
            {
                if(i==1&&(j==2||j==4))
                    continue;
                else if(i==3&&(j==2||j==4))
                    continue;
                else
                    allEnemyChess.push_back(new enemyChess(i,j));
            }
        }
    }

    static int codeToSub(char code)
    {
        if(code=='a')
            return siling;
        if(code=='b')
            return junzhang;
        if(code=='c')
            return shizhang;
        if(code=='d')
            return lvzhang;
        if(code=='e')
            return tuanzhang;
        if(code=='f')
            return yingzhang;
        if(code=='g')
            return lianzhang;
        if(code=='h')
            return paizhang;
        if(code=='i')
            return gongbing;
        if(code=='j')
            return dilei;
        if(code=='k')
            return zhadan;
        if(code=='l'||code=='L')
            return junqi;
        return -1;
    }
};
