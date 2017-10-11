#pragma once
#include "Cell.h"
#include "ConstDefine.h"
#include "MBB.h"
#include "QueryResult.h"
#include "Trajectory.h"
#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include "BufferManager.h"



class Grid{
public:
	MBB range;
	float cell_size; //length of a cell
	int cell_num_x,cell_num_y; //�������ж��ٸ�cell
	int cellnum; //upper(area(grid)/area(cell))����֤�ܷ�������cell
	Cell* cellPtr; //�洢cell�����
	ofstream fout;//�ļ�����ӿ�
	int totalPointNum; //grid�ڵ����
	BufferManager buffer; //buffer����������
	Grid();
};

int initGrid(Grid *g,const MBB& mbb, float val_cell_size);
int addTrajectoryIntoCell(Grid *g, Trajectory &t);
int WhichCellPointIn(Grid *g, SamplePoint p);
int addDatasetToGrid(Grid *g,Trajectory* db,int traNum);
int writeCellsToFile(Grid *g,int* cellNo, int cellNum,string file);
//rangeQuery����������Bounding box������켣��źͶ�Ӧ˳���µĲ�����
int rangeQuery(Grid *g,MBB & bound, CPURangeQueryResult * ResultTable, int* resultSetSize);

