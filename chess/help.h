#pragma once
#include <fstream>
#include <iomanip>
#include <string>
#include "basic.h"
using namespace std;

ofstream *ofile=nullptr;

void writeFile(string path, string content)
{
	if (ofile == nullptr)
	{
		ofile = new ofstream;
		ofile->open(path, std::ios::app);
	}
	(*ofile) << content << endl;   //����д���ļ�
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