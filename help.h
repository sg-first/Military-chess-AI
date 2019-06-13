#pragma once
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

void writeFile(string path,string content)
{
    ofstream ofile;               //定义输出文件
    ofile.open(path,std::ios::app);     //作为输出文件打开
    ofile<<content<<endl;   //标题写入文件
    ofile.close();                //关闭文件
}
