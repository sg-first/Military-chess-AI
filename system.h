#pragma once
#include <array>
#include <stack>
#include "basic.h"
#include "assess.h"
using namespace std;

class moveRecord
{
public:
    moveRecord(int x1,int y1,int x2,int y2) : x1(x1),y1(y1),x2(x2),y2(y2),c1(cMap[y1][x1]),c2(cMap[y2][x2]) {}
    int x1;
    int y1;
    int x2;
    int y2;
    char c1;
    char c2;

    void back()
    {
        cMap[y1][x1]=c1;
        cMap[y2][x2]=c2;
    }
};

stack<moveRecord>allRecord;

//模拟移动棋子，返回结果（0己方被对方吃，1对方被己方吃，2对死，3己方移动）
int simulateMove(int x1,int y1,int x2,int y2)
{
    allRecord.push(moveRecord(x1,y1,x2,y2)); //记录这步棋
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
