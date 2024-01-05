#include "ACO.h"

class Graph {

public:

    int vertex_num;                           //节点数
    int edge_num;                             //边数
    int start_vertex_num;                     //起点数
    int end_vertex_num;                       //终点数
    int ant_num;                              //总人数
    int matrix_length[MAX][MAX] = {0};        //图的矩阵,包含长度信息
    int matrix_capacity[MAX][MAX] = {0};      //图的矩阵,包含容量信息
    int bak_matrix_capacity[MAX][MAX] = {0};  //备份，图的矩阵,包含容量信息
    int matrix_width[MAX][MAX] = {0};         //图的矩阵，包含宽度信息
    int vertex_ant_num[MAX] = {0};            //记录每个节点的人数
    int bak_vertex_ant_num[MAX] = {0};        //备份，用于恢复每个节点的初始人数
//    int edge_visit[MAX][MAX] = {0};           //记录边是否被访问
    double pheromones[MAX][MAX] = {0};        //信息素矩阵char
    int start_vertex[MAX] = {0};              //记录所有起点
    int end_vertex[MAX] = {0};                //记录所有终点
    double initial_pheromones;                //初始信息素
    int dijkstra_next_point[MAX] = {0};       //由dijkstra构造的下一节点


    Graph();

    int init();

    int readGraphByFile(const string &fileName);    //通过文件读取图信息
    int renewGraphByFile(const string &fileName);   //通过文件更新图信息
    int initPheromones();                           //初始化信息素
    int evaporatePheromones();                      //信息素蒸发
    int updatePheromones();                         //信息素更新
    int resetVertexAntNum();                        //恢复初始人数
    double calculatePheromones(int i, int j);       //返回i,j路径间的原始信息素
    vector<int> dijkstra(int src, int dest);              //求v到其余顶点的最短路径和长度
    int minDistance(int dist[], bool sptSet[]);
    int getAllDijkstraNext();                       //得到所有节点的下一个节点
    int getPathLength(vector<int> path);            //获得路径对应的长度
};

