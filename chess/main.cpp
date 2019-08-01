#include "pch.h"

#include "basic.h"
#include "help.h"
#include "reasoning.h"
#include "simulate.h"

bool findJunqi = false;
bool isgongzu;
int rounds = 0;
float ff1 = 0, ff2 = 0, ff3 = 0, ff4 = 0, ff5 = 0, ff6 = 0;

void outputAllneeds()		//此输出函数使用$号作为分割表格的标志
{
	string content1, content2, content3, content4, content5, content6;
	int usNum = 0, eneNum = 0;
	for (int i = 0; i <= 11; i++)		//输出棋盘
	{
		for (int j = 0; j <= 4; j++)
		{
			if ((char(cMap[i][j]) >= 'A') && (char(cMap[i][j]) <= 'Z'))
				content1 += string(1, char(49)) + string(1, char(51)) + " ";
			else if ((char(cMap[i][j]) == 'j'))
				content1 += string(1, char(49)) + string(1, char(48)) + " ";
			else if ((char(cMap[i][j]) == 'k'))
				content1 += string(1, char(49)) + string(1, char(49)) + " ";
			else if ((char(cMap[i][j]) == 'l'))
				content1 += string(1, char(49)) + string(1, char(50)) + " ";
			else
				content1 += string(1, char(cMap[i][j]) - 48) + " ";
		}
		content1 += "\n";
	}
	content1 += "$";
	writeFile("特种兵的日记.txt", content1);
	for (int i = 0; i < size(allEnemyChess); i++)		//输出概率表
	{
		float sumenemychess = (allEnemyChess[i]->sum());
		for (int j = 0; j < 12; j++)
		{
			content2 += to_string((allEnemyChess[i]->prob[j])) + " ";
		}
		content2 += "\n";
	}
	content2 += "$";
	writeFile("特种兵的日记.txt", content2);
	for (int i = 0; i < size(allEnemyChess); i++)		//输出敌方棋子位置
	{
		content3 += to_string(allEnemyChess[i]->y) + " " + to_string(allEnemyChess[i]->x) + " ";		//此处按照先行后列进行输出
		content3 += "\n";
	}
	content3 += "$";
	writeFile("特种兵的日记.txt", content3);
	for (int i = 0; i <= 11; i++)		//输出双方总棋子数
	{
		for (int j = 0; j <= 4; j++)
		{
			if (IsMyChess(i, j))
				usNum += 1;
			if (IsEneChess(i, j))
				eneNum += 1;
		}
	}
	content4 += to_string(usNum) + " " + to_string(eneNum) + "\n";		//先输出我方总棋子数  再输出敌方总棋子数
	content4 += "$";
	writeFile("特种兵的日记.txt", content4);
	content5 += to_string(rounds) + "\n";		//输出回合数
	content5 += "$";
	writeFile("特种兵的日记.txt", content5);
	content6 += to_string(ff1) + " " + to_string(ff2) + " " + to_string(ff3) + " " + to_string(ff4) + " " + to_string(ff5) + " " + to_string(ff6) + "\n";		//输出局面评估函数值
	content6 += "@";		//回合结束使用@进行分割
	writeFile("特种兵的日记.txt", content6);
	ff1 = 0;
	ff2 = 0;
	ff3 = 0;
	ff4 = 0;
	ff5 = 0;
	ff6 = 0;
}

/* ************************************************************************ */
/* 函数功能：根据裁判反馈刷新棋盘（完成）										*/
/* 接口参数：																*/
/*     char *cInMessage 来自裁判的GO YXYX R YX命令							*/
/*     string cOutMessage 发给裁判的BESTMOVE YXYX命令						*/
/* ************************************************************************ */
void FreshMap(char* cInMessage, string cOutMessage)
{
	int x1, y1;				//起点
	int x2, y2;				//落点
	char result = -1;			//碰子结果
	if (cInMessage[0] == 'G')	// GO 指令（对方要走，根据系统得到的信息更新）
	{
		if (cInMessage[3] >= 'A' && cInMessage[3] <= 'L')
		{
			y1 = cInMessage[3] - 'A';
			x1 = cInMessage[4] - '0';
			y2 = cInMessage[5] - 'A';
			x2 = cInMessage[6] - '0';
			result = cInMessage[8] - '0';		//碰子结果
			if (!findJunqi && cInMessage[10] >= 'A' && cInMessage[10] <= 'L') //对方司令战死后显示军旗位置
			{
				findJunqi = true;
				enemyChess* c = ecOp::findChess(x1, y1);
				c->determine(siling);
				int junqiY = cInMessage[10] - 'A';
				int junqiX = cInMessage[11] - '0';
				cMap[junqiY][junqiX] = 'L';
				c = ecOp::findChess(junqiX, junqiY);
				c->determine(junqi);
			}

			enemyChess* c = ecOp::findChess(x1, y1);
			switch (result)		//根据不同结果修改棋盘
			{
			case 0:			//对方棋子被己方吃掉
			{
				isgongzu = false;
				c->less(ecOp::codeToType(cMap[y2][x2])); //对方棋子小于己方棋子
				cMap[y1][x1] = '0'; //对方棋子消失，己方不必改变
				ecOp::adjustDepth();
				break;
			}
			case 1:			//对方吃掉己方棋子
			{
				isgongzu = false;
				c->more(ecOp::codeToType(cMap[y2][x2])); //对方棋子大于己方棋子
				c->setPos(x2, y2);
				cMap[y2][x2] = cMap[y1][x1]; //对方棋子移到己方位置（1是对方）
				cMap[y1][x1] = '0';
				ecOp::adjustDepth();
				break;
			}
			case 2:			//双方棋子对死
			{
				isgongzu = false;
				c->equ(ecOp::codeToType(cMap[y2][x2])); //对方棋子等于己方棋子
				cMap[y1][x1] = '0';
				cMap[y2][x2] = '0';
				ecOp::adjustDepth();
				break;
			}
			case 3:			//对方移动棋子
			{
				c->setPos(x2, y2);
				cMap[y2][x2] = cMap[y1][x1];
				cMap[y1][x1] = '0';
				ecOp::adjustDepth();
				break;
			}
			}
		}
	}
	if (cInMessage[0] == 'R')	// RESULT 指令
	{
		//先根据cOutMessage记下之前移动的我方棋子
		y1 = cOutMessage[9] - 'A';
		x1 = cOutMessage[10] - '0';
		y2 = cOutMessage[11] - 'A';
		x2 = cOutMessage[12] - '0';

		//然后看看这个棋子的结果
		result = cInMessage[7] - '0'; //碰子结果
		if (!findJunqi && cInMessage[8] == ' ' && cInMessage[9] >= 'A' && cInMessage[9] <= 'L') //对方司令战死后显示军旗位置
		{
			findJunqi = true;
			enemyChess* c = ecOp::findChess(x2, y2);
			c->determine(siling);
			int junqiY = cInMessage[9] - 'A';
			int junqiX = cInMessage[10] - '0';
			cMap[junqiY][junqiX] = 'L';
			c = ecOp::findChess(junqiX, junqiY);
			c->determine(junqi);
		}
		switch (result)		//根据不同结果修改棋盘
		{
		case 0:			//己方棋子被对方吃掉
		{
			isgongzu = false;
			enemyChess* c = ecOp::findChess(x2, y2);
			c->more(ecOp::codeToType(cMap[y1][x1])); //对方棋子大于己方棋子
			cMap[y1][x1] = '0';
			ecOp::adjustDepth();
			break;
		}
		case 1:			//己方吃掉对方棋子
		{
			isgongzu = false;
			enemyChess* c = ecOp::findChess(x2, y2);
			c->less(ecOp::codeToType(cMap[y1][x1])); //对方棋子小于己方棋子
			cMap[y2][x2] = cMap[y1][x1]; //2是新位置（敌方），1是老位置
			cMap[y1][x1] = '0';
			ecOp::adjustDepth();
			break;
		}
		case 2:			//双方棋子对死
		{
			isgongzu = false;
			enemyChess* c = ecOp::findChess(x2, y2);
			c->equ(ecOp::codeToType(cMap[y1][x1])); //对方棋子等于己方棋子
			cMap[y1][x1] = '0';
			cMap[y2][x2] = '0';
			ecOp::adjustDepth();
			break;
		}
		case 3:			//己方移动棋子
		{
			cMap[y2][x2] = cMap[y1][x1];
			cMap[y1][x1] = '0';
			break;
		}
		}
	}
}
/* ************************************************************************ */
/* 函数功能：根据INFO指令,返回参赛队名（完成）									*/
/* 接口参数：																*/
/*     char *cInMessage 接收的INFO ver指令								*/
/* ************************************************************************ */
string CulInfo(char* cInMessage, char* cVer)
{
	strcpy_s(cVer, 200, cInMessage + 5);
	return "SUT"; //返回参赛队名
}

/* ************************************************************************ */
/* 函数功能：根据START指令,返回布局      									*/
/* 接口参数：																*/
/*     char *cInMessage 接收的START first time steps指令					*/
/*     int iFirst 先行权[0先行，1后行]									*/
/*     int iTime 行棋时间限制(单位秒)[1000,3600]							*/
/*     int iStep 进攻等待限制(单位步)[10,31]								*/
/* ************************************************************************ */
string CulArray(char* cInMessage, int& iFirst, int& iTime, int& iStep)
{
	iFirst = cInMessage[6] - '0';
	iTime = cInMessage[8] - '0';
	iTime = iTime * 10 + (cInMessage[9] - '0');
	iTime = iTime * 10 + (cInMessage[10] - '0');
	iTime = iTime * 10 + (cInMessage[11] - '0');
	iStep = cInMessage[13] - '0';
	iStep = iStep * 10 + (cInMessage[14] - '0');
	//布局这个可能没啥可改的
	if (iFirst == 0)	//先手
		return "ARRAY " + readFile("first.txt");
	else			//后手
		return "ARRAY " + readFile("second.txt");
}

float AlphaBeta(int remainDepth, float alpha, float beta, moveTup& aiAction) //搜索用的递归函数
{
	if (remainDepth == 0) //到达搜索深度
	{
		float guzhi = assess::valueEstimation(cMap);
		return  guzhi;//返回局面评估
	}

	bool isEme = remainDepth % 2;//根据深度判断当前玩家（规定深度为偶数，0为我方1为敌方）
	int x1, y1, x2, y2;

	//对于每一步走法
	auto everyDo = [&]()
	{
		if (!assess::isEneDilei(y2, x2))
		{
			recordStack::push(x1, y1, x2, y2, isEme); //实行这步走法
			float value = -AlphaBeta(remainDepth - 1, -beta, -alpha, aiAction); //递归调用，获取这步走法的局面评估
			recordStack::pop(); //回溯这步棋
			if (ecOp::search_depth == remainDepth && value > alpha)
				aiAction = make_tuple(x1, y1, x2, y2); //若此时是最顶层，则记录最佳走法，贪心策略
			if (value > alpha) //更新最大值
				alpha = value;
		}
	};

	basicFun isMovingChess, isChess, isInvChess;
	//看看轮到我方走还是敌方走，决定用哪个基础判定函数
	if (isEme) //敌方
	{
		isMovingChess = IsEmeMovingChess;
		isChess = IsEneChess;
		isInvChess = IsMyChess;
	}
	else //我方
	{
		isMovingChess = IsMyMovingChess;
		isChess = IsMyChess;
		isInvChess = IsEneChess;
	}

	for (int i = 0; i < 12; i++) //对于当前己方的每个棋子
	{
		for (int j = 0; j < 5; j++)
		{
			if (isMovingChess(i, j) && !IsBaseCamp(i, j))  //己方不在大本营的可移动棋子
			{
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以前移:不在第一行,不在山界后,前方不是己方棋子,前方不是有棋子占领的行营
				if (i > 0 && !IsVerticalRailway(i, j) && !IsAfterHill(i, j) && !isChess(i - 1, j) && !IsFilledCamp(i - 1, j))
				{
					y2 = i - 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				else
				{
					for (int k = 1; y2 > 0 && y2 < 11 && IsVerticalRailway(y2, x2) && IsVerticalRailway(y2 - 1, j) && !IsAfterHill(y2, j) && !isChess(y2 - 1, j)
						&& !IsFilledCamp(y2 - 1, j); k++)
					{
						y2 = i - k;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
						if (isInvChess(y2, x2)) //当前位置已经是敌方棋子，不能再前进
							break;
					}

				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以左移:不在最左列,左侧不是己方棋子,左侧不是被占用的行营
				if (j > 0 && !IsAcrossRailway(i) && !isChess(i, j - 1) && !IsFilledCamp(i, j - 1))
				{
					x2 = j - 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				else
				{
					for (int k = 1; x2 > 0 && IsAcrossRailway(i) && !isChess(i, x2 - 1) && !IsFilledCamp(i, x2 - 1); k++)
					{
						x2 = j - k;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
						if (isInvChess(y2, x2)) //前方已经是敌方棋子，不能再前进
							break;
					}
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以右移:不在最右列,右侧不是己方棋子,右侧不是被占用的行营
				if (j < 4 && !IsAcrossRailway(i) && !isChess(i, j + 1) && !IsFilledCamp(i, j + 1))
				{
					x2 = j + 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				else
				{
					for (int k = 1; x2 < 4 && IsAcrossRailway(i) && !isChess(i, x2 + 1) && !IsFilledCamp(i, x2 + 1); k++)
					{
						x2 = j + k;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
						if (isInvChess(y2, x2)) //前方已经是敌方棋子，不能再前进
							break;
					}
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以后移:不在最后列,不在山界前,后侧不是己方棋子,后侧不是被占用的行营
				if (i < 11 && !IsVerticalRailway(i, j) && !IsBeforeHill(i, j) && !isChess(i + 1, j) && !IsFilledCamp(i + 1, j))
				{
					y2 = i + 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				else
				{
					for (int k = 1; y2 < 11 && y2 > 0 && IsVerticalRailway(y2, x2) && IsVerticalRailway(y2 + 1, j) && !IsBeforeHill(y2, j) && !isChess(y2 + 1, j) &&
						!IsFilledCamp(y2 + 1, j); k++)
					{
						//						writeFile("特种兵的日记.txt", "考察循环后移");
						y2 = i + k;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
						if (isInvChess(y2, x2)) //前方已经是敌方棋子，不能再前进
							break;
					}
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以左上进行营:左上不是被占用的行营且它是行营
				if (IsMoveCamp(i - 1, j - 1) && !IsFilledCamp(i - 1, j - 1))
				{
					y2 = i - 1;
					x2 = j - 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以右上进行营:右上不是被占用的行营且它是行营
				if (IsMoveCamp(i - 1, j + 1) && !IsFilledCamp(i - 1, j + 1))
				{
					y2 = i - 1;
					x2 = j + 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以左下进行营:左下不是被占用的行营且它是行营
				if (IsMoveCamp(i + 1, j - 1) && !IsFilledCamp(i + 1, j - 1))
				{
					y2 = i + 1;
					x2 = j - 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以右下进行营:右下不是被占用的行营且它是行营
				if (IsMoveCamp(i + 1, j + 1) && !IsFilledCamp(i + 1, j + 1))
				{
					y2 = i + 1;
					x2 = j + 1;
					everyDo();
					if (alpha >= beta) //剪枝
						return alpha;
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以左上出行营:目前位置为行营且左上不是己方棋子
				if (IsMoveCamp(i, j) && !isChess(i - 1, j - 1))
				{
					if (!(IsMoveCamp(i - 1, j - 1) && IsFilledCamp(i - 1, j - 1))) //如果是行营行营里不能有子
					{
						y2 = i - 1;
						x2 = j - 1;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
					}
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以右上出行营:目前位置为行营且右上不是己方棋子
				if (IsMoveCamp(i, j) && !isChess(i - 1, j + 1))
				{
					if (!(IsMoveCamp(i - 1, j + 1) && IsFilledCamp(i - 1, j + 1)))
					{
						y2 = i - 1;
						x2 = j + 1;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
					}
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以左下出行营:目前位置为行营且左下不是己方棋子
				if (IsMoveCamp(i, j) && !isChess(i + 1, j - 1))
				{
					if (!(IsMoveCamp(i + 1, j - 1) && IsFilledCamp(i + 1, j - 1)))
					{
						y2 = i + 1;
						x2 = j - 1;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
					}
				}
				y1 = i; x1 = j; y2 = i; x2 = j;
				//可以右下出行营:目前位置为行营且右下不是己方棋子
				if (IsMoveCamp(i, j) && !isChess(i + 1, j + 1))
				{
					if (!(IsMoveCamp(i + 1, j + 1) && IsFilledCamp(i + 1, j + 1)))
					{
						y2 = i + 1;
						x2 = j + 1;
						everyDo();
						if (alpha >= beta) //剪枝
							return alpha;
					}
				}
			}
		}
	}
	return alpha; //返回子节点中的最大值
}

moveTup minimax() //极大极小搜索启动
{
	moveTup result = make_tuple(0, 0, 0, 0);
	AlphaBeta(ecOp::search_depth, -100000, 100000, result);
	int x1, y1, x2, y2;
	tie(x1, y1, x2, y2) = result;
	//	writeFile("特种兵的日记.txt", "得到最优走法" + to_string(x1) + "," + to_string(y1)+" "+to_string(x2)+","+to_string(y2));
	return result;
}

moveTup gongzu() //拱卒
{
	int x1, y1, x2, y2;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			y1 = i; x1 = j; y2 = i; x2 = j;
			if (IsMyMovingChess(i, j) && !IsBaseCamp(i, j))  //己方不在大本营的可移动棋子
			{
				//可以前移:不在第一行,不在山界后,前方不是己方棋子,前方不是有棋子占领的行营
				if (i > 0 && !IsAfterHill(i, j) && !IsMyChess(i - 1, j) && !IsFilledCamp(i - 1, j))
				{
					y2 = i - 1;
					return make_tuple(x1, y1, x2, y2);
				}
				else
				{
					//可以左移:不在最左列,左侧不是己方棋子,左侧不是被占用的行营
					if (j > 0 && !IsMyChess(i, j - 1) && !IsFilledCamp(i, j - 1))
					{
						x2 = j - 1;
						return make_tuple(x1, y1, x2, y2);
					}
					else
					{
						//可以右移://不在最右列,右侧不是己方棋子,右侧不是被占用的行营
						if (j < 4 && !IsMyChess(i, j + 1) && !IsFilledCamp(i, j + 1))
						{
							x2 = j + 1;
							return make_tuple(x1, y1, x2, y2);
						}
					}
				}
			}
		}
	}
	return make_tuple(x1, y1, x2, y2);
}

/* ************************************************************************ */
/* 函数功能：计算己方最佳着法(本程序为示例算法,请参赛选手自行改进算法)          	*/
/* 接口参数：																*/
/*     char *cInMessage 来自裁判的 GO 命令									*/
/* ************************************************************************ */
string CulBestmove()
{
	int x1, y1, x2, y2;
	string cOutMessage = "BESTMOVE A0A0";

	if (isgongzu)
		tie(x1, y1, x2, y2) = gongzu();
	else
		tie(x1, y1, x2, y2) = minimax();

	cOutMessage[9] = y1 + 'A';
	cOutMessage[10] = x1 + '0';
	cOutMessage[11] = y2 + 'A';
	cOutMessage[12] = x2 + '0';
	return cOutMessage;
}

void init()
{
	ecOp::search_depth = minDepth;
	for (int i = 0; i <= 5; i++)
	{
		for (int j = 0; j <= 4; j++)
		{
			if (cMap[i][j] == 'X')
				allEnemyChess.push_back(new enemyChess(j, i));
		}
	}
	isgongzu = true;
	enemyChess::junqiEne = nullptr;
}

int main()
{
	char cVer[200];			//协议版本
	int iFirst;				//先行权[0先行，1后行]
	int iTime;				//行棋时间限制(单位秒)[1000,3600]
	int iStep;				//进攻等待限制(单位步)[10,31]
	char cInMessage[200];   //输入通信内容
	string cOutMessage;  //输出通信内容

	cin.getline(cInMessage, 200);		//获取来自裁判系统的指令 "GO 0000 0 00"

	while (cInMessage[0] >= 'A')
	{
		switch (cInMessage[0])
		{
		case 'I':								//INFO指令（完成）
		{
			cOutMessage = CulInfo(cInMessage, cVer);
			writeFile("zhiling.txt", cOutMessage);
			cout << cOutMessage << endl; //将"NAME "指令传递给裁判系统
			break;
		}
		case 'S':								//START 指令（完成）
		{
			cOutMessage = CulArray(cInMessage, iFirst, iTime, iStep);
			//初始化
			InitMap(cOutMessage);
			init();
			writeFile("zhiling.txt", cOutMessage);
			cout << cOutMessage << endl;
			break;
		}
		case 'G':								//GO 指令
		{
			FreshMap(cInMessage, cOutMessage);
			rounds++;
			outputAllneeds();
			cOutMessage = CulBestmove();
			writeFile("zhiling.txt", cOutMessage);
			cout << cOutMessage << endl;
			break;
		}
		case 'R':								//RESULT 指令
		{
			FreshMap(cInMessage, cOutMessage);
			rounds++;
			outputAllneeds();
			break;
		}
		case 'E':								//END 指令
		{
			string jieguo = string(1, cInMessage[4]);
			writeFile("特种兵的日记.txt", jieguo);
			return 0;
		}
		default:
			return 1;
		}

		cin.getline(cInMessage, 200);	//获取来自裁判系统的指令"START" 或 "RESULT" 或 "GO" 或 "END"
	}
}