#pragma once
#include "reasoning.h"

class assess
{
    static int codeToStrength(int type) //用于模拟走棋的固定子力值
    {
        if(type==zhadan)
            return shizhang; //炸弹等于师长
        else if(type==dilei)
            return tuanzhang; //地雷等于团长
        else
            return type; //别的下标就是按棋力大小排的，直接返回即可
    }
    static int codeToStrength2(int type) //用于局面评估的固定子力值
    {
        if (type == junqi)
            return 12;//军棋子力值应为0，在大本营的棋子-12
        if (type == zhadan || type == shizhang)
            return 22;
        if (type == tuanzhang || type == dilei)
            return 18;
        if (type == gongbing)
            return 10;
        if (type == paizhang)
            return 12;
        if (type == lianzhang)
            return 14;
        if (type == yingzhang)
            return 16;
        if (type == lvzhang)
            return 20;
		if (type == junzhang)
            return 24;
		if (type == siling)
            return 30;
    }

	static float valueLocation(int i, int j) //棋子所在位置增加值
    {
        if (IsAcrossRailway(i) || IsVerticalRailway(i,j))//铁路线+5
            return 5;
        else if (IsBaseCamp(i,j))//大本营-12
            return -12;
        else if(IsMyMoveCamp(i,j))//己方行营
            return 8;
        else if (IsEnemyMoveCamp(i,j)) //对方行营
            return 10;
        else 
            return 4;
    }

    static float valueMotivation(int type) //type变量类型？
    {
        if(type!=gongbing)
            return codeToStrength2(type)/4;
        else
            return codeToStrength2(type)/9;
    }

    static float valuelast3line(int i,int j)
    {
		if (i > 8 && cMap[i][j] != 'l')
		{
			return 15 / shortestpathtojunqi(i, j);
		}
		else
			return 0;
    }

	static float valueNear(int i, int j)
	{
		vector<pos> allPos = getNearPos(i, j);
		float eneMax = 0;
		float friMax = 0;
		for (pos p : allPos) 
		{
			int i2, j2;
			tie(i2, j2) = p;
			if (cMap[i2][j2] != '0')
			{
				if (cMap[i2][j2] == 'X')
				{
					float s = getChessStrength(ecOp::findChess(j2, i2), false);
					if (s > eneMax)
						eneMax = s;
				}
				else
				{
					float s = codeToStrength2(ecOp::codeToType(cMap[i2][j2]));
					if (s > friMax)
						friMax = s;
				}
			}
		}
		float value = 0;
		float myStrength = codeToStrength2(ecOp::codeToType(cMap[i][j]));
		if (eneMax >= myStrength)
		{
			value -= eneMax;//对方相邻棋子负影响
		}
		if (friMax > myStrength)
		{
			value += friMax / 2;
		}
		return value;
	}

public:
    static int ChessComparisons(char myc,enemyChess* enc) //比较我方与敌方棋子大小（0被敌方吃，1吃掉敌方，2对死）
    {
        int mytype=ecOp::codeToType(myc);
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
        //涉及炸弹的特判
        if(mytype==zhadan || encType==zhadan)
            return 2;
        else //不是地雷炸弹工兵的情况
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

    static float getChessStrength(enemyChess *chess, bool sim = true) //获取敌方棋子的棋力值，通过概率分布计算
    {
		if (chess->isDie)
			return 0;
		else
		{
			float score = 0;
			for (unsigned int i = 0; i < chess->prob.size(); i++)
			{
				float weight;
				if (sim)
					weight = assess::codeToStrength(i); //当前维度的权重
				else
					weight = assess::codeToStrength2(i);
				score += chess->prob[i] * weight;
			}
			writeFile("特种兵的日记.txt", "chess sum:" + to_string(chess->sum()));
			return score / chess->sum();
		}
    }

    static float valueEstimation(char cMap[12][5]) //局面评估
    {
        float sumvalue=0; //value单个子力值，sumvalue子力值求和
        for(int i=0;i<=11;i++)
        {
            for (int j = 0; j <= 4; j++)
            {
				float value = 0;
                if(IsMyChess(i,j))
                {
                    int type = ecOp::codeToType(cMap[i][j]);//cmap转换成type类型
					if (enemyChess::junqiEne != nullptr && enemyChess::junqiEne->isDie)
						value += 500;
					float f1 = codeToStrength2(type);
					float f2 = valueLocation(i, j);
					float f3 = valueMotivation(type);
					float f4 = valuelast3line(i, j);
					float f5 = valueNear(i, j);
					/*writeFile("特种兵的日记.txt", "codeToStrength2" + to_string(f1));
					writeFile("特种兵的日记.txt", "valueLocation" + to_string(f2));
					writeFile("特种兵的日记.txt", "valueMotivation" + to_string(f3));
					writeFile("特种兵的日记.txt", "valuelast3line" + to_string(f4));
					writeFile("特种兵的日记.txt", "valueNear" + to_string(f5));*/
                    value += f1 + f2 + f3 + f4 + f5;
                }
                sumvalue+=value;
            }
        }
        return sumvalue;
    }
};
