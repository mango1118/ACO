#include "Axis.h"

Axis::Axis(Graph &graph) {
    initEasyX(graph);
}

void Axis::initEasyX(Graph &graph) {
    initgraph(1200, 800);
    // 设置白色背景
    setbkcolor(WHITE);
    cleardevice();
    // 设置线的颜色为黑色
    setcolor(BLACK);
    initGraph(graph);
    initAxis();
}

void Axis::insertVertex(int x, int y, const char *data, COLORREF color) {
    // 设置填充颜色
    setfillcolor(color);
    // 设置边框线条样式
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
    fillcircle(x, y, 10);
    //绘制边框
    circle(x, y, 10);

    //设置背景
    setbkmode(TRANSPARENT);
    outtextxy(x - 3, y - 8, data);
}

void Axis::insertLine(int x1, int y1, int x2, int y2) {
    // 设置线条颜色
    setcolor(BLACK);
    // 定义线的起始点和终点坐标
    int startX = x1, startY = y1;
    int endX = x2, endY = y2;
    // 绘制线条
    line(startX, startY, endX, endY);
}

void Axis::resetAllLine(Graph &graph) {
    cleardevice();
//    Sleep(2000);
    initGraph(graph);
    initAxis();
}

void Axis::insertPath(vector<int> path, Graph graph) {
    for (int i = 0; i < path.size() - 1; i++) {
        //找到起点path[i]和终点path[i+1]
        int start = path[i];
        int end = path[i + 1];
        //找到起点和终点对应的坐标
        int start_x = graph.position[start].first;
        int start_y = graph.position[start].second;
        int end_x = graph.position[end].first;
        int end_y = graph.position[end].second;
        //用insertLine插入线
        insertLine(start_x,start_y,end_x,end_y);
    }
}

void Axis::initAxis() {
    // 绘制坐标轴
    line(100, 700, 1100, 700);  // x轴
    line(100, 700, 100, 100);  // y轴
    // 绘制坐标轴刻度
    for (int i = 100; i <= 1100; i += 50) {
        line(i, 695, i, 705);   // x轴刻度
    }
    for (int i = 100; i <= 700; i += 50) {
        line(95, i, 105, i);   // y轴刻度
    }
    // 在坐标轴上标注文字
    outtextxy(1120, 700, "X");
    outtextxy(100, 75, "Y");
}

void Axis::initGraph(Graph &graph) {
    for (int i = 0; i < graph.vertex_num; i++) {
        COLORREF color = RGB(192, 192, 192);
        for (int j = 0; j < graph.start_vertex_num; j++) //检查是否是起点
            if (graph.start_vertex[j] == i)
                color = RGB(240, 135, 132);
        for (int j = 0; j < graph.end_vertex_num; j++)   //检查是否是终点
            if (graph.end_vertex[j] == i)
                color = RGB(72, 172, 255);
        // 在传递给 insertVertex 之前将 char 转换为字符串
        string identifierString(1, graph.identifier[i]);
        insertVertex(graph.position[i].first, graph.position[i].second,
                     identifierString.c_str(), color);
    }
}
