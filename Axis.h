#ifndef AXIS_H
#define AXIS_H

#include "ACO.h"
#include "Graph.h"
//#include "Graph.h"
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <string>
#include <cmath>
#include<windows.h>

class Axis {
public:
    int width;
    int height;

    Axis(Graph &graph);

    void initEasyX(Graph &graph);                                               //初始化
    void insertVertex(int x, int y, const char *data, COLORREF color);          //插入一个节点(可能用圆绘制节点)，包含标识符
    void insertLine(int x1, int y1, int x2, int y2);                            //插入一条线，箭头指向后者
    void resetAllLine(Graph &graph);                                            //重置坐标轴上的线，重绘图和坐标轴
    void insertPath(vector<int> path, Graph graph);                             //描绘蚂蚁路径
    void initAxis();                                                            //绘制坐标轴
    void initGraph(Graph &graph);                                               //绘制图

};

#endif // AXIS_H