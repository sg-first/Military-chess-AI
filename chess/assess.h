#pragma once
#include "reasoning.h"

class assess
{
    static int codeToStrength(int type) //����ģ������Ĺ̶�����ֵ
    {
        if(type==zhadan)
            return shizhang; //ը������ʦ��
        else if(type==dilei)
            return tuanzhang; //���׵����ų�
        else
            return type; //����±���ǰ�������С�ŵģ�ֱ�ӷ��ؼ���
    }
    static int codeToStrength2(int type) //���ھ��������Ĺ̶�����ֵ
    {
        if (type == junqi)
            return 12;//��������ֵӦΪ0���ڴ�Ӫ������-12
        if (type == zhadan || type == shizhang)
            return 22;
        if (type == tuanzhang || type == dilei)
            return 18;
        if (type == gongbing)
            return 10;
        if (type == paizhang)
            return 12;
        if (type == lianzhang)
            return 14;
        if (type == yingzhang)
            return 16;
        if (type == lvzhang)
            return 20;
		if (type == junzhang)
            return 24;
		if (type == siling)
            return 30;
    }

	static float valueLocation(int i, int j) //��������λ������ֵ
    {
        if (IsAcrossRailway(i) || IsVerticalRailway(i,j))//��·��+5
            return 5;
        else if (IsBaseCamp(i,j))//��Ӫ-12
            return -12;
        else if(IsMyMoveCamp(i,j))//������Ӫ
            return 8;
        else if (IsEnemyMoveCamp(i,j)) //�Է���Ӫ
            return 10;
        else 
            return 4;
    }

    static float valueMotivation(int type) //type�������ͣ�
    {
        if(type!=gongbing)
            return codeToStrength2(type)/4;
        else
            return codeToStrength2(type)/9;
    }

    static float valuelast3line(int i,int j)
    {
		if (i > 8 && cMap[i][j] != 'l')
		{
			return 15 / shortestpathtojunqi(i, j);
		}
		else
			return 0;
    }

	static float valueNear(int i, int j)
	{
		vector<pos> allPos = getNearPos(i, j);
		float eneMax = 0;
		float friMax = 0;
		for (pos p : allPos) 
		{
			int i, j;
			tie(i, j) = p;
			if (cMap[i][j] != '0')
			{
				if (cMap[i][j] == 'X')
				{
					float s = getChessStrength(ecOp::findChess(j, i), false);
					if (s > eneMax)
						eneMax = s;
				}
				else
				{
					float s = codeToStrength2(ecOp::codeToType(cMap[i][j]));
					if (s > friMax)
						friMax = s;
				}
			}
		}
		float value = 0;
		float myStrength = codeToStrength2(ecOp::codeToType(cMap[i][j]));
		if(eneMax>=myStrength)
			value -= eneMax;//�Է��������Ӹ�Ӱ��
		if (friMax > myStrength)
			value += friMax / 2;
		return value;
	}

public:
    static int ChessComparisons(char myc,enemyChess* enc) //�Ƚ��ҷ���з����Ӵ�С��0���з��ԣ�1�Ե��з���2������
    {
        int mytype=ecOp::codeToType(myc);
        int encType=enc->isDetermine();
        //�漰�������׵�����
        if(mytype==gongbing)
        {
            //Ŀǰ������ȷ���з��������͵��������������
            if(encType==gongbing || encType==zhadan)
                return 2;
            else if(encType==dilei)
                return 1;
            else
                return 0;
        }
        else if(mytype==dilei)
        {
            if(encType==gongbing)
                return 0;
            else if(encType==zhadan)
                return 2;
            else
                return 1;
        }
        //�漰ը��������
        if(mytype==zhadan || encType==zhadan)
            return 2;
        else //���ǵ���ը�����������
        {
            float myStrength=assess::codeToStrength(mytype);
            float enemyStrength=getChessStrength(enc);
            if(myStrength<enemyStrength)
                return 0;
            else if(myStrength>enemyStrength)
                return 1;
            else
                return 2;
        }
    }

    static float getChessStrength(enemyChess *chess, bool sim = true) //��ȡ�з����ӵ�����ֵ��ͨ�����ʷֲ�����
    {
		writeFile("���ֱ����ռ�.txt", "����з�����ֵ��ʼ��"+to_string((int)chess));
        float score = 0;
        for (unsigned int i = 0; i < chess->prob.size(); i++)
        {
            float weight;
            if (sim)
                weight = assess::codeToStrength(i); //��ǰά�ȵ�Ȩ��
            else
                weight = assess::codeToStrength2(i);
            score += chess->prob[i] * weight;
        }
		writeFile("���ֱ����ռ�.txt", "������"+to_string(score)+" "+to_string(chess->sum()));
        return score / chess->sum();
    }

    static float valueEstimation(char cMap[12][5]) //��������
    {
        float sumvalue=0; //value��������ֵ��sumvalue����ֵ���
        for(int i=0;i<=11;i++)
        {
            for (int j = 0; j <= 4; j++)
            {
				float value = 0;
                if(IsMyChess(i,j))
                {
					writeFile("���ֱ����ռ�.txt", "�����ֵ�����У�"+to_string(i)+","+to_string(j));
                    int type = ecOp::codeToType(cMap[i][j]);//cmapת����type����
					writeFile("���ֱ����ռ�.txt", "codeToStrength2��"+to_string(codeToStrength2(type)));
					writeFile("���ֱ����ռ�.txt", "valueLocation��"+to_string(valueLocation(i, j)));
					writeFile("���ֱ����ռ�.txt", "valueMotivation��"+to_string(valueMotivation(type)));
					writeFile("���ֱ����ռ�.txt", "valuelast3line��"+to_string(valuelast3line(i, j)));
					writeFile("���ֱ����ռ�.txt", "valueNear��"+to_string(valueNear(i, j)));
                    value = value + codeToStrength2(type) + valueLocation(i, j) + valueMotivation(type) + valuelast3line(i,j) + valueNear(i,j);
					writeFile("���ֱ����ռ�.txt", "Ŀǰ���ӹ�ֵ��"+to_string(value));
                }
                sumvalue+=value;
            }
        }
        return sumvalue;
    }
};
