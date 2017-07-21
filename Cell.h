#pragma once
#include "ConstDefine.h"
#include "MBB.h"
#include <string>
#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;
typedef struct subTra {
	int traID;
	int startpID;
	int endpID;
	int numOfPoint;//ÿ���ӹ켣��ĸ���
	subTra* next;
}subTra;


typedef struct Cell{
	int cell_x;//cell������
	int cell_y;//cell������
	MBB mbb;
	int subTraNum; //�ӹ켣�ĸ���
	int totalPointNum; //cell�ڵ����
	subTra subTraEntry;//����cell�������������
	subTra* subTraPtr;//��ǰ��������ָ��λ��
	subTra* subTraTable;//ת��Ϊ�������������
	ofstream fout;//�ļ��ӿ�
}Cell;

bool initialCell(Cell *c,int x, int y, const MBB& val_mbb);
int addSubTra(Cell *c,int traID, int startIdx, int endIdx, int numOfPoints);
int buildSubTraTable(Cell *c);//��ȡ�����й켣֮��������洢
int writeCellToFile(Cell *c,string fileName);


//class Cell
//{
//public:
//	Cell();
//	Cell(int x, int y,const MBB& val_mbb);
//	bool initial(int x, int y, const MBB& val_mbb);
//	int addSubTra(int traID, int startIdx, int endIdx, int numOfPoints);
//	int buildSubTraTable();//��ȡ�����й켣֮��������洢
//	int writeCellToFile(string fileName);
//	~Cell();
//
//
//
//
//
//#ifdef _CELL_BASED_STORAGE
//	//ÿ��cell��point���������ݵ���ʼ����ֹλ��
//	int pointRangeStart;
//	int pointRangeEnd;
//#endif // _CELL_BASED_STORAGE
//
//};

