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
	(*ofile) << content << endl;   //标题写入文件
}

string readFile(string file)
{
	ifstream infile;
	infile.open(file.data());   //将文件流对象与文件连接起来 
	string content;
	while (getline(infile, content));
	infile.close();             //关闭文件输入流
	return content;
}