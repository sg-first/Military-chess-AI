#pragma once
#include "reasoning.h"

class assess
{
    //warn:注意，以下三个函数中所说的“棋力值”只用来比大小，和论文里局面评估的棋力值无关

    static int codeToStrength(int type) //获取某类棋子的棋力值
    {
        if(type==zhadan)
            return siling; //炸弹等于司令
        else
            return type; //别的下标就是按棋力大小排的，直接返回即可
    }

public:
    static int ChessComparisons(char myc,enemyChess* enc) //比较我方与敌方棋子大小（0己方被对方吃，1对方被己方吃，2对死）
    {
        int mytype=ecOp::codeToSub(myc);
        int encType=enc->isDetermine();
        //涉及工兵地雷的特判
        if(mytype==gongbing)
        {
            //目前仅能在确定敌方棋子类型的情况下特判启用
            if(encType==gongbing || encType==zhadan)
                return 2;
            else if(encType==dilei)
                return 1;
            else
                return 0;
        }
        else if(mytype==dilei)
        {
            if(encType==gongbing)
                return 0;
            else if(encType==zhadan)
                return 2;
            else
                return 1;
        }
        else //不是工兵的情况
        {
            float myStrength=assess::codeToStrength(mytype);
            float enemyStrength=getChessStrength(enc);
            if(myStrength<enemyStrength)
                return 0;
            else if(myStrength>enemyStrength)
                return 1;
            else
                return 2;
        }
    }

    static float getChessStrength(enemyChess* chess) //获取敌方棋子的棋力值，通过概率分布计算
    {
        float score=0;
        for(int i=0;i<chess->prob.size();i++)
        {
            float weight=assess::codeToStrength(ecOp::codeToSub(i)); //当前维度的权重
            score+=chess->prob[i]*weight;
        }
        return score/chess->sum();
    }
};
