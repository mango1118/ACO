#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <random>

using namespace std;

#define MAX 50
#define MAX_TIME 2147483647

//路径选择方法
#define PATH_SELECTED_METHOD 1
//method 1 represents ACO				ACO蚁群算法
//method 2 represents greedy method		贪心算法
//method 3 represents dijkstra			dijkstra算法

//信息素更新方法
#define PHEROMONE_UPDATE_METHOD 1
//method 1 the best solution only		最佳路径更新
//method 2 the rank-based ant			蚂蚁排名更新
//method 3 the elitist ant				精英蚂蚁更新

//选择路径方法
#define SELECT_NEXT_METHOD 2
//method 1 the max pheromone
//method 2 roulette next

//文件名
#define FILE_NAME "network_example.txt"
#define RECORD_NAME "GraphRecord.txt"

//起点数，终点数
#define START_VERTEX_NUM 2
#define END_VERTEX_NUM 7

//路径长度范围
#define EDGE_LENGTH_MAX 9
#define EDGE_LENGTH_MIN 1

//路径容量范围
#define EDGE_CAPACITY_MAX 9
#define EDGE_CAPACITY_MIN 1

//运行次数
#define RUN_TIME 10
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
//信息素挥发率
#define EVAPORATE_RATE 0.3
//速度参数
#define VELOCITY_PARAMETER 0.1
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
#define COST_PARAMETER 0.005
//轮盘赌比例参数
#define RANDOM_RATE 0.9