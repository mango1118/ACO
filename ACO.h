#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "EasyX/include/easyx.h"

using namespace std;

#define MAX 30

//路径选择方法
#define PATH_SELECTED_METHOD 1
//method 1 represents ACO				ACO蚁群算法
//method 2 represents greedy method		贪心算法
//method 3 represents dijkstra			dijkstra算法

//选择路径方法
#define SELECT_NEXT_METHOD 2
//method 1 the max pheromone
//method 2 roulette next

//多目标优化
#define MULTI_TARGET 1
//method 1 多目标

#define MULTI_ANT_MAX 10
#define MULTI_TIME_IMPORTANCE 0.5
#define MULTI_SAFE_IMPORTANCE 0.5
#define MULTI_SAFE_PARAMETER 0.1

#define EVALUATION_TIME 0.1
#define EVALUATION_SAFE 0.5

//文件名
#define FILE_NAME "network_example.txt"
#define RECORD_NAME "GraphRecord.txt"

//更新文件数
#define NEW_RECORD_NUM 0
//更新文件名
#define NEW_RECORD_1 "network_example_1.txt"
//更新文件的时间
#define NEW_RECORD_TIME_1 150

//运行次数
#define RUN_TIME 1
//迭代次数
#define ITERATION_TIME 100

//初始化信息素参数
#define PHEROMONE_INIT 1
//初始化信息素长度权重
#define PHEROMONE_INIT_LENGTH 0.4
//初始化信息素容量权重
#define PHEROMONE_INIT_CAPACITY 0.3
//初始化信息素宽度权重
#define PHEROMONE_INIT_WIDTH 0.3
//本地信息素挥发率
#define LOCAL_EVAPORATE_RATE 0.1
//全局信息挥发率
#define GLOBAL_EVAPORATE_RATE 0.3
//速度参数
#define VELOCITY_PARAMETER 0.2
//平均速度
#define AVERAGE_VELOCITY 1
//计算速度的宽度权重
#define VELOCITY_WIDTH_WEIGHT 0.5
//计算速度的容量权重
#define VELOCITY_CAPACITY_WEIGHT 0.5
//更新信息素的时间参数
#define PHEROMONE_UPDATE_TIME 0.5
//更新信息素的路径长度参数
#define PHEROMONE_UPDATE_LENGTH 0.25
//更新信息素的容量参数
#define PHEROMONE_UPDATE_CAPACITY 0.25
//代价参数
#define COST_PARAMETER 0.0001
//轮盘赌比例参数
#define RANDOM_RATE 0.9
//全局释放信息素的蚂蚁个数
#define GLOBAL_RELEASE_ANT 1
//最小容量
#define MIN_CAPACITY 1
//受伤速度比例
#define HURT_VELOCITY_RATE 0.5