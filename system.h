#pragma once
#include <array>
#include "basic.h"
#include "assess.h"
using namespace std;

//模拟移动棋子，返回结果（0己方被对方吃，1对方被己方吃，2对死，3己方移动）
int simulateMove(int x1,int y1,int x2,int y2)
{
    //移动的合法性由策略判别，这里直接进行实际的移动操作
    char &newPos=cMap[y2][x2];
    char &myChess=cMap[y1][x1];

    if(newPos=='0')
    {
        newPos=myChess;
        myChess='0';
        return 3;
    }
    else //我方棋子的情况策略中已经排除，下面处理敌方棋子
    {
        int result=assess::ChessComparisons(myChess,ecOp::findChess(x2,y2));
        switch(result)		//根据不同结果修改棋盘
        {
            case 0:
            {
                myChess='0';
                break;
            }
            case 1:
            {
                newPos=myChess;
                myChess='0';
                break;
            }
            case 2:
            {
                newPos='0';
                myChess='0';
                break;
            }
        }
        return result;
    }
}
