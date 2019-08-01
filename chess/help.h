#pragma once
#include <fstream>
#include <iomanip>
#include <string>
#include "basic.h"
using namespace std;

ofstream *ofile=nullptr;
string lastPath;

void writeFile(string path, string content)
{
	if (ofile == nullptr || path!=lastPath)
	{
		lastPath = path;
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
	string lineContent;
	while (getline(infile, lineContent))
		content += lineContent + "\n";
	infile.close();             //关闭文件输入流
	return content.substr(0, content.size() - 1);
}