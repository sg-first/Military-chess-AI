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
	(*ofile) << content << endl;   //����д���ļ�
}

string readFile(string file)
{
	ifstream infile;
	infile.open(file.data());   //���ļ����������ļ��������� 
	string content;
	while (getline(infile, content));
	infile.close();             //�ر��ļ�������
	return content;
}