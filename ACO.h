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
#include <chrono>

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

//最大释放蚂蚁个数，就是起点数
#define MULTI_ANT_MAX 7
#define MULTI_TIME_IMPORTANCE 0.5
#define MULTI_SAFE_IMPORTANCE 0.5
#define MULTI_SAFE_PARAMETER 0.1

#define EVALUATION_TIME 0.5
#define EVALUATION_SAFE 0.3

//是否更新图
#define CHANGE_GRAPH 0

//文件名
#define FILE_NAME "network_2.txt"
#define RECORD_NAME "GraphRecord.txt"
#define GRAPH_NAME "network_2.jpg"
#define THIS_TIME_RECORD_NAME "this_time.txt"

//是否有底层地图
#define PRINT_IMAGE 0
//method 1 draw
//method 0 dont draw

//是否绘制拓扑图
#define PRINT_GRAPH 0
//method 1 draw
//method 0 dont draw

//绘制蚂蚁过程
#define DRAW_PROCESS 1
//method 1 draw
//method 0 dont draw

//运行次数
#define RUN_TIME 10
//迭代次数
#define ITERATION_TIME 100


//更新图时间
#define CHANGE_GRAPH_TIME 100
//更新图名
#define RENEW_GRAPH_NAME "network_example_change.txt"

//输出最佳还是全部
#define WRITE_ALL_OR_BEST 2
//method 1 ALL
//method 2 BEST

//初始化信息素参数
#define PHEROMONE_INIT 1
//初始化信息素长度权重
#define PHEROMONE_INIT_LENGTH 0.4
//初始化信息素容量权重
#define PHEROMONE_INIT_CAPACITY 0.2
//初始化信息素宽度权重
#define PHEROMONE_INIT_WIDTH 0.2

//本地信息素挥发率
#define LOCAL_EVAPORATE_RATE 0.2
//全局信息挥发率
#define GLOBAL_EVAPORATE_RATE 0.2

//速度参数
#define VELOCITY_PARAMETER 0.2
//平均速度
#define AVERAGE_VELOCITY 2
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
//评价函数最大值
#define MAX_EVALUATION 200
//评价函数最小值
#define MIN_EVALUATION 0
//线性映射范围
#define LINEAR 100
//信息素权重
#define ALPHA 1.0
//启发式权重
#define BETA 3.0

#define GRAPH_WIDTH 1400
#define GRAPH_HEIGHT 450