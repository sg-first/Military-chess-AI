/* ************************************************************************ */
/* 己方棋子编码约定:														*/
/*	a司令,b军长,c师长,d旅长,e团长,f营长,g连长,h排长,i工兵,j地雷k炸弹,l军旗      */
/* 对方方棋子编码约定:														*/
/*	A司令,B军长,C师长,D旅长,E团长,F营长,G连长,H排长,I工兵,J地雷K炸弹,L军旗      */
/*	X未知对方棋子,0空棋位													*/
/* ************************************************************************ */
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

char cMap[12][5]; //棋盘

/* ************************************************************************ */
/* 函数功能：i,j位置是否本方棋子											*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1己方棋子，0空棋位或对方棋子											*/
/* ************************************************************************ */
int IsMyChess(char cMap[12][5],int i,int j)
{
    if(cMap[i][j]>='a'&& cMap[i][j]<='l')
        return 1;
    else
        return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否本方可移动的棋子									*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1己方可移动棋子(司令,军长,...,工兵,炸弹)，0军旗,地雷,对方棋子或空棋位*/
/* ************************************************************************ */
int IsMyMovingChess(char cMap[12][5],int i,int j)
{
    if(cMap[i][j]>='a' && cMap[i][j]<='i' || cMap[i][j]=='k')
        return 1;
    else
        return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否山界后的兵站										*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1处于山界后，0不处于山界后											*/
/* ************************************************************************ */
int IsAfterHill(int i,int j)
{
    if(i*5+j==31 || i*5+j==33)
        return 1;
    else
        return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否行营												*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1是行营，0不是行营													*/
/* ************************************************************************ */
int IsMoveCamp(int i,int j)
{
    if(i*5+j==11 || i*5+j==13 || i*5+j==17 || i*5+j==21 || i*5+j==23 || i*5+j==36 || i*5+j==38 || i*5+j==42 || i*5+j==46 || i*5+j==48)
        return 1;
    else
        return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否大本营											*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1是大本营，0不是大本营												*/
/* ************************************************************************ */
int IsBaseCamp(int i,int j)
{
    if(i*5+j==1 || i*5+j==3 || i*5+j==56 || i*5+j==58)
        return 1;
    else
        return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否有棋子占位的行营										*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1有棋子占位的行营,0不是行营或是空行营								*/
/* ************************************************************************ */
int IsFilledCamp(char cMap[12][5],int i,int j)
{
    if(IsMoveCamp(i,j) && cMap[i][j]!='0')
        return 1;
    else
        return 0;
}

/* ************************************************************************ */
/* 函数功能：双方布局后棋局初始化（完成）										*/
/* 接口参数：																*/
/*     char *cOutMessage 布局字符序列										*/
/* ************************************************************************ */
void InitMap(string cOutMessage) //这个是用之前计算好的数据处理，所以是cOutMessage
{
    int i,j,k;
    for(i=0;i<6;i++)	//标记对方棋子
        for(j=0;j<5;j++)
            if(IsMoveCamp(i,j))
                cMap[i][j]='0';
            else
                cMap[i][j]='X';
    k=6;
    for(i=6;i<12;i++)	//标记己方棋子
        for(j=0;j<5;j++)
            if(IsMoveCamp(i,j))
                cMap[i][j]='0';
            else
                cMap[i][j]=cOutMessage[k++];

}

/* ************************************************************************ */
/* 函数功能：根据裁判反馈刷新棋盘（完成）										*/
/* 接口参数：																*/
/*     char *cInMessage 来自裁判的GO YXYX R YX命令							*/
/*     string cOutMessage 发给裁判的BESTMOVE YXYX命令						*/
/* ************************************************************************ */
void FreshMap(char *cInMessage,string cOutMessage)
{
    char x1,y1;				//起点
    char x2,y2;				//落点
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
                cMap[(cInMessage[10]-'A')][cInMessage[11]-'0']='L';
            switch(result)		//根据不同结果修改棋盘
            {
                case 0:			//对方棋子被己方吃掉
                    cMap[y1][x1]='0';
                    break;
                case 1:			//对方吃掉己方棋子
                    cMap[y2][x2]=cMap[y1][x1];
                    cMap[y1][x1]='0';
                    break;
                case 2:			//双方棋子对死
                    cMap[y1][x1]='0';
                    cMap[y2][x2]='0';
                    break;
                case 3:			//对方移动棋子
                    cMap[y2][x2]=cMap[y1][x1];
                    cMap[y1][x1]='0';
                    break;
            }
        }
    }
    if(cInMessage[0]=='R')	// RESULT 指令
    {
        //先根据cOutMessage记下之前移动的棋子
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
                cMap[y1][x1]='0';
                break;
            case 1:			//己方吃掉对方棋子
                cMap[y2][x2]=cMap[y1][x1];
                cMap[y1][x1]='0';
                break;
            case 2:			//双方棋子对死
                cMap[y1][x1]='0';
                cMap[y2][x2]='0';
                break;
            case 3:			//己方移动棋子
                cMap[y2][x2]=cMap[y1][x1];
                cMap[y1][x1]='0';
                break;
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
            if(IsMyMovingChess(cMap,i,j) && !IsBaseCamp(i,j))  //己方不在大本营的可移动棋子
            {
                //可以前移:不在第一行,不在山界后,前方不是己方棋子,前方不是有棋子占领的行营
                if(i>0 && !IsAfterHill(i,j) && !IsMyChess(cMap,i-1,j) && !IsFilledCamp(cMap,i-1,j))
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
                    if(j>0 && !IsMyChess(cMap,i,j-1) && !IsFilledCamp(cMap,i,j-1))
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
                        if(j<4 && !IsMyChess(cMap,i,j+1) && !IsFilledCamp(cMap,i,j+1))
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
                InitMap(cOutMessage);
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
