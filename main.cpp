#include "basic.h"
#include "help.h"
#include "reasoning.h"

/* ************************************************************************ */
/* 函数功能：根据裁判反馈刷新棋盘（完成）										*/
/* 接口参数：																*/
/*     char *cInMessage 来自裁判的GO YXYX R YX命令							*/
/*     string cOutMessage 发给裁判的BESTMOVE YXYX命令						*/
/* ************************************************************************ */
void FreshMap(char *cInMessage,string cOutMessage)
{
    int x1,y1;				//起点
    int x2,y2;				//落点
    char result=-1;			//碰子结果
    if(cInMessage[0]=='G')	// GO 指令（对方要走，根据系统得到的信息更新）
    {
        if(cInMessage[3]>='A' && cInMessage[3]<='L')
        {
            y1=cInMessage[3]-'A';
            x1=cInMessage[4]-'0';
            y2=cInMessage[5]-'A';
            x2=cInMessage[6]-'0';
            result=cInMessage[8]-'0';		//碰子结果
            if(cInMessage[10]>='A' && cInMessage[10]<='L') //对方司令战死后显示军旗位置
            {
                int junqiY=cInMessage[10]-'A';
                int junqiX=cInMessage[11]-'0';
                cMap[junqiY][junqiX]='L';
                enemyChess* c=ecOp::findChess(junqiX,junqiY);
                c->determine(junqi);
            }

            switch(result)		//根据不同结果修改棋盘
            {
                case 0:			//对方棋子被己方吃掉
                {
                    enemyChess* c=ecOp::findChess(x1,y1);
                    c->less(ecOp::codeToSub(cMap[y2][x2])); //对方棋子小于己方棋子
                    cMap[y1][x1]='0'; //对方棋子消失，己方不必改变
                    break;
                }
                case 1:			//对方吃掉己方棋子
                {
                    enemyChess* c=ecOp::findChess(x1,y1);
                    c->more(ecOp::codeToSub(cMap[y2][x2])); //对方棋子大于己方棋子

                    cMap[y2][x2]=cMap[y1][x1]; //对方棋子移到己方位置（1是对方）
                    cMap[y1][x1]='0';
                    c->setPos(x2,y2);
                    break;
                }
                case 2:			//双方棋子对死
                {
                    enemyChess* c=ecOp::findChess(x1,y1);
                    c->equ(ecOp::codeToSub(cMap[y2][x2])); //对方棋子等于己方棋子
                    cMap[y1][x1]='0';
                    cMap[y2][x2]='0';
                    break;
                }
                case 3:			//对方移动棋子
                {
                    enemyChess* c=ecOp::findChess(x1,y1);
                    c->setPos(x2,y2);
                    cMap[y2][x2]=cMap[y1][x1];
                    cMap[y1][x1]='0';
                    break;
                }
            }
        }
    }
    if(cInMessage[0]=='R')	// RESULT 指令
    {
        //先根据cOutMessage记下之前移动的我方棋子
        y1=cOutMessage[9]-'A';
        x1=cOutMessage[10]-'0';
        y2=cOutMessage[11]-'A';
        x2=cOutMessage[12]-'0';

        //然后看看这个棋子的结果
        result=cInMessage[7]-'0'; //碰子结果
        if(cInMessage[8]==' ' && cInMessage[9]>='A' && cInMessage[9]<='L') //对方司令战死后显示军旗位置
            cMap[(cInMessage[9]-'A')][cInMessage[10]-'0']='L';
        switch(result)		//根据不同结果修改棋盘
        {
            case 0:			//己方棋子被对方吃掉
            {
                enemyChess* c=ecOp::findChess(x2,y2);
                c->more(ecOp::codeToSub(cMap[y1][x1])); //对方棋子大于己方棋子
                cMap[y1][x1]='0';
                break;
            }
            case 1:			//己方吃掉对方棋子
            {
                enemyChess* c=ecOp::findChess(x2,y2);
                c->less(ecOp::codeToSub(cMap[y1][x1])); //对方棋子小于己方棋子
                cMap[y2][x2]=cMap[y1][x1]; //2是新位置（敌方），1是老位置
                cMap[y1][x1]='0';
                break;
            }
            case 2:			//双方棋子对死
            {
                enemyChess* c=ecOp::findChess(x2,y2);
                c->equ(ecOp::codeToSub(cMap[y1][x1])); //对方棋子等于己方棋子
                cMap[y1][x1]='0';
                cMap[y2][x2]='0';
                break;
            }
            case 3:			//己方移动棋子
            {
                cMap[y2][x2]=cMap[y1][x1];
                cMap[y1][x1]='0';
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
string CulInfo(char *cInMessage,char *cVer)
{
    strcpy(cVer,cInMessage+5);
    return "Northeastern University"; //返回参赛队名
}

/* ************************************************************************ */
/* 函数功能：根据START指令,返回布局      									*/
/* 接口参数：																*/
/*     char *cInMessage 接收的START first time steps指令					*/
/*     int iFirst 先行权[0先行，1后行]									*/
/*     int iTime 行棋时间限制(单位秒)[1000,3600]							*/
/*     int iStep 进攻等待限制(单位步)[10,31]								*/
/* ************************************************************************ */
string CulArray(char *cInMessage,int &iFirst,int &iTime,int &iStep)
{
    iFirst=cInMessage[6]-'0';
    iTime=cInMessage[8]-'0';
    iTime=iTime*10+(cInMessage[9]-'0');
    iTime=iTime*10+(cInMessage[10]-'0');
    iTime=iTime*10+(cInMessage[11]-'0');
    iStep=cInMessage[13]-'0';
    iStep=iStep*10+(cInMessage[14]-'0');
    //布局这个可能没啥可改的
    if(iFirst==0)	//先手
        return "ARRAY abccddeeffggghhhiiijjkklj";
    else			//后手
        return "ARRAY cbacddeeffggghhhiiijjkklj";
}

/* ************************************************************************ */
/* 函数功能：计算己方最佳着法(本程序为示例算法,请参赛选手自行改进算法)          	*/
/* 接口参数：																*/
/*     char *cInMessage 来自裁判的 GO 命令									*/
/* ************************************************************************ */
string CulBestmove(char *cInMessage)
{
    string cOutMessage="BESTMOVE A0A0";
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<5;j++)
        {
            if(IsMyMovingChess(i,j) && !IsBaseCamp(i,j))  //己方不在大本营的可移动棋子
            {
                //可以前移:不在第一行,不在山界后,前方不是己方棋子,前方不是有棋子占领的行营
                if(i>0 && !IsAfterHill(i,j) && !IsMyChess(i-1,j) && !IsFilledCamp(i-1,j))
                {
                    cOutMessage[9]=i+'A';
                    cOutMessage[10]=j+'0';
                    cOutMessage[11]=(i-1)+'A';
                    cOutMessage[12]=j+'0';
                    return cOutMessage;
                }
                else
                {
                    //可以左移:不在最左列,左侧不是己方棋子,左侧不是被占用的行营
                    if(j>0 && !IsMyChess(i,j-1) && !IsFilledCamp(i,j-1))
                    {
                        cOutMessage[9]=i+'A';
                        cOutMessage[10]=j+'0';
                        cOutMessage[11]=i+'A';
                        cOutMessage[12]=(j-1)+'0';
                        return cOutMessage;
                    }
                    else
                    {
                        //可以右移://不在最右列,右侧不是己方棋子,右侧不是被占用的行营
                        if(j<4 && !IsMyChess(i,j+1) && !IsFilledCamp(i,j+1))
                        {
                            cOutMessage[9]=i+'A';
                            cOutMessage[10]=j+'0';
                            cOutMessage[11]=i+'A';
                            cOutMessage[12]=(j+1)+'0';
                            return cOutMessage;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    char cVer[200];			//协议版本
    int iFirst;				//先行权[0先行，1后行]
    int iTime;				//行棋时间限制(单位秒)[1000,3600]
    int iStep;				//进攻等待限制(单位步)[10,31]
    char cInMessage[200];   //输入通信内容
    string cOutMessage;  //输出通信内容

    cin.getline(cInMessage,200);		//获取来自裁判系统的指令 "GO 0000 0 00"

    while(cInMessage[0]>='A')
    {
        switch(cInMessage[0])
        {
            case 'I':								//INFO指令（完成）
            {
                cOutMessage = CulInfo(cInMessage,cVer);
                cout<<cOutMessage<<endl; //将"NAME "指令传递给裁判系统
                break;
            }
            case 'S':								//START 指令（完成）
            {
                cOutMessage = CulArray(cInMessage,iFirst,iTime,iStep);
                //初始化地图与敌方棋子概率分布
                InitMap(cOutMessage);
                ecOp::init();
                cout<<cOutMessage<<endl;
                break;
            }
            case 'G':								//GO 指令
            {
                FreshMap(cInMessage,cOutMessage);
                cOutMessage = CulBestmove(cInMessage);
                cout<<cOutMessage<<endl;
                break;
            }
            case 'R':								//RESULT 指令
            {
                FreshMap(cInMessage,cOutMessage);
                break;
            }
            case 'E':								//END 指令
                return 0;
            default:
                return 1;
        }

        cin.getline(cInMessage,200);	//获取来自裁判系统的指令"START" 或 "RESULT" 或 "GO" 或 "END"
    }
}
