#pragma once
#define MAX_TRAJ_SIZE 1000
#define MAXLENGTH 512
//MAXGAP是最大轨迹内时间间隔，如果超过这个间隔应该被视为两条轨迹
#define MAXGAP 3600

#define EPSILON 10
#define MAXTHREAD 512
#define MAXJOBSNUM 10000

#include <stdio.h>
#include <string>
extern "C"{
#include "p_mmap.h"
}
//test:以cell为基础存储
#define _CELL_BASED_STORAGE
//test:Similarity query based on naive grid，以定大小的grid来索引
//#define _SIMILARITY

typedef struct Point {
	float x;
	float y;
	uint32_t time;
	uint32_t tID;
}Point;

typedef struct SPoint {
	float x;
	float y;
	uint32_t tID;
}SPoint;



