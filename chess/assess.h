#pragma once
#pragma once
#include "reasoning.h"

extern float ff1, ff2, ff3, ff4, ff5, ff6;
float eneMax = 0;

class assess
{
	static int codeToStrength(int type) //用于模拟走棋的固定子力值
	{
		if (type == zhadan)
			return shizhang; //炸弹等于师长
		else if (type == dilei)
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
		if (IsAcrossRailway(i) || IsVerticalRailway(i, j))//铁路线+5
			return 5;
		else if (IsBaseCamp(i, j))//大本营-12
			return -12;
		else if (IsMyMoveCamp(i, j))//己方行营
			return 8;
		else if (IsEnemyMoveCamp(i, j)) //对方行营
			return 10;
		else
			return 4;
	}

	static float valueMotivation(int type) //type变量类型？
	{
		if (type != gongbing)
			return codeToStrength2(type) / 4;
		else
			return codeToStrength2(type) / 9;
	}

	static float valuelast3line(int i, int j)
	{
		if (i > 8 && cMap[i][j] != 'l')
		{
			return 15 / shortestpathtojunqi(i, j);
		}
		else
			return 0;
	}
	static float valuecrosshill(int i)
	{
		if (i <= 5 && i >= 3)
			return 55 * (6 - i) - eneMax;
		else if (i < 3)
			return 70 * (6 - i) - eneMax;
		else
			return 0;
	}

	static float valueNear(int i, int j)
	{
		vector<pos> allPos = getNearPos(i, j);
		eneMax = 0;
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
	static float valuefuckdilei(int i, int j)
	{

	}

public:
	static int ChessComparisons(char myc, enemyChess* enc) //比较我方与敌方棋子大小（0被敌方吃，1吃掉敌方，2对死）
	{
		int mytype = ecOp::codeToType(myc);
		int encType = enc->isDetermine();
		//涉及工兵地雷的特判
		if (mytype == gongbing)
		{
			//目前仅能在确定敌方棋子类型的情况下特判启用
			if (encType == gongbing || encType == zhadan)
				return 2;
			else if (encType == dilei)
				return 1;
			else
				return 0;
		}
		else if (mytype == dilei)
		{
			if (encType == gongbing)
				return 0;
			else if (encType == zhadan)
				return 2;
			else
				return 1;
		}
		//涉及炸弹的特判
		if (mytype == zhadan || encType == zhadan)
			return 2;
		else //不是地雷炸弹工兵的情况
		{
			float myStrength = assess::codeToStrength(mytype);
			float enemyStrength = getChessStrength(enc);
			if (myStrength < enemyStrength)
				return 0;
			else if (myStrength > enemyStrength)
				return 1;
			else
				return 2;
		}
	}

	static float getChessStrength(enemyChess* chess, bool sim = true) //获取敌方棋子的棋力值，通过概率分布计算
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
			return score / chess->sum();
		}
	}

	static std::tuple<float, float, float, float, float, float, float, float>
		valueEstimation(char cMap[12][5]) //局面评估
	{
		float ff1 = 0, ff2 = 0, ff3 = 0, ff4 = 0, ff5 = 0, ff6 = 0, ff7 = 0; //每一项的总评分
		for (int i = 0; i <= 11; i++)
		{
			for (int j = 0; j <= 4; j++)
			{
				if (IsMyChess(i, j))
				{
					int type = ecOp::codeToType(cMap[i][j]);//cmap转换成type类型
					if (enemyChess::junqiEne != nullptr && enemyChess::junqiEne->isDie)
						ff7 += 1000;
					ff1 += codeToStrength2(type);
					ff2 += valueLocation(i, j);
					ff3 += valueMotivation(type);
					ff4 += valuelast3line(i, j);
					ff5 += valueNear(i, j);
					ff6 += valuecrosshill(i);
				}
			}
		}
		return make_tuple(ff1, ff2, ff3, ff4, ff5, ff6, ff7,
			ff1 + ff2 + ff3 + ff4 + ff5 + ff6 + ff7);
	}

	static bool isEneDilei(int i, int j)
	{
		if (cMap[i][j] == 'X')
		{
			auto e = ecOp::findChess(j, i);
			if (e->isDetermine() == dilei)
				return true;
		}
		return false;
	}
};
