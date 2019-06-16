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

class recordStack
{
public:
    static stack<moveRecord>allRecord;

    static int push(int x1,int y1,int x2,int y2,bool isEmc) //压栈并模拟移动棋子，返回结果（0己方被对方吃，1对方被己方吃，2对死，3己方移动）
    {
        allRecord.push(moveRecord(x1,y1,x2,y2));

        //移动的合法性由策略判别，这里直接进行实际的移动操作
        char &newPos=cMap[y2][x2];
        char &oldPos=cMap[y1][x1];

        if(newPos=='0')
        {
            newPos=oldPos;
            oldPos='0';
            return 3;
        }
        else //我方棋子的情况策略中已经排除，下面处理敌方棋子
        {
            int result;
            if(isEmc==false)
                result=assess::ChessComparisons(oldPos,ecOp::findChess(x2,y2)); //这个函数是有实际的我方/敌方的顺序的，敌方下要反过来
            else //这个情况下，新位置是实际的我方，老位置是实际的敌方
                result=assess::ChessComparisons(newPos,ecOp::findChess(x1,y1));

            switch(result) //根据不同结果修改棋盘
            {
                case 0: //实际的敌方胜
                {
                    if(isEmc==true) //老位置是实际的敌方，前进到新位置成功
                    {
                        newPos=oldPos;
                        oldPos='0';
                    }
                    else
                        oldPos='0';
                    break;
                }
                case 1: //实际的我方胜
                {
                    if(isEmc==false) //老位置是实际的我方，前进到新位置成功
                    {
                        newPos=oldPos;
                        oldPos='0';
                    }
                    else
                        oldPos='0';
                    break;
                }
                case 2: //对死
                {
                    newPos='0';
                    oldPos='0';
                    break;
                }
            }
            return result;
        }
    }

    static void pop() //回退一步棋
    {
        allRecord.top().back();
        allRecord.pop();
    }

    static void popAll() //回退所有棋
    {
        while(allRecord.empty()==false)
            pop();
    }
};
stack<moveRecord> recordStack::allRecord;
