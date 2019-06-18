#pragma once
#include <fstream>
#include <iomanip>
#include <string>
#include "basic.h"
using namespace std;

void writeFile(string path, string content)
{
	ofstream ofile;               //定义输出文件
	ofile.open(path, std::ios::app);     //作为输出文件打开
	ofile << content << endl;   //标题写入文件
	ofile.close();                //关闭文件
}

void outputCMap()
{
	string content;
	for (int i = 0; i <= 11; i++)
	{
		for (int j = 0; j <= 4; j++)
		{
			content += string(1,char(cMap[i][j])) + " ";
		}
		content += "\n";
	}
	writeFile("特种兵的日记.txt", content);
}