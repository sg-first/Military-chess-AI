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