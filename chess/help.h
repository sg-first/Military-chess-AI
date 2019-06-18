#pragma once
#include <fstream>
#include <iomanip>
#include <string>
#include "basic.h"
using namespace std;

void writeFile(string path, string content)
{
	ofstream ofile;               //��������ļ�
	ofile.open(path, std::ios::app);     //��Ϊ����ļ���
	ofile << content << endl;   //����д���ļ�
	ofile.close();                //�ر��ļ�
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
	writeFile("���ֱ����ռ�.txt", content);
}