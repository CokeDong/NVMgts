#ifndef PREPROCESS_H
#define PREPROCESS_H
#include<fstream>
#include "ConstDefine.h"
#include"Trajectory.h"
using namespace std;

typedef struct VLLT
{
    string vid;
    float lon;
    float lat;
    int time;
}VLLT;

struct tidLinkTable
{
    int tid;
    tidLinkTable* next = NULL;
};

class PreProcess
{
    public:
        ifstream fin;
        ofstream fout;
        int maxTid = 0; //当前最大的轨迹编号
        Trajectory* tradb;
        PreProcess();
        bool init(string fileName,string outFileName, Trajectory* tradb);
		bool updateMapBound(float lon,float lat);
		bool validPoint(float lon, float lat);
        virtual ~PreProcess();
        VLLT getTraInfoFromString(string s);
		float xmin, xmax, ymin, ymax;


    protected:

    private:
};

#endif // PREPROCESS_H
