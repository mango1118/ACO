#include "ACO.h"
#include "Graph.h"
#include "Ant.h"
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <string>
#include <cmath>
#include<windows.h>

#ifndef AXIS_H
#define AXIS_H

class Axis {
public:
    int iteration = 0;
    vector<int> former_iteration_times;
    vector<double> former_evaluation;

    Axis();
//    Axis(Graph &graph);

    void initEasyX(Graph &graph);                                                                   //初始化
    void insertVertex(const char *data, int x, int y, COLORREF color);                              //插入一个节点(可能用圆绘制节点)，包含标识符
    void insertLine(int x1, int y1, int x2, int y2);                                                //插入一条线，箭头指向后者
    void resetAllLine(Graph &graph);                                                                //重置坐标轴上的线，重绘图和坐标轴
    void insertPath(vector<int> path, Graph graph);                                                 //描绘蚂蚁路径
    void initAxis();                                                                                //绘制坐标轴
    void initGraph(Graph &graph);                                                                   //绘制图
    void insertEvaluation(Graph &graph, int iteration_times, double evaluation);                    //绘制评价函数曲线
    void insertAntLocation(Graph &graph, Ant ant);                                                  //绘制蚂蚁当前在图上的位置
    void insertAntListLocation(Graph &graph, vector<Ant> ants);                                     //绘制所有蚂蚁当前在图上的位置
    void initImage();                                                                               //绘制场景图

};

#endif // AXIS_H