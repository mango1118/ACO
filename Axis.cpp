#include "Axis.h"

Axis::Axis() {

}
//Axis::Axis(Graph &graph) {
//    initEasyX(graph);
//}

void Axis::initEasyX(Graph &graph) {
    initgraph(1250, 450);
    // 设置白色背景
    setbkcolor(WHITE);
    cleardevice();
    // 设置线的颜色为黑色
    setcolor(BLACK);
    initGraph(graph);
    initAxis();
}

void Axis::insertVertex(int x, int y, const char *data, COLORREF color) {
    settextstyle(20,0,_T("Times New Roman"));
    // 设置填充颜色
    setfillcolor(color);
    // 设置边框线条样式
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
    fillcircle(x, y, 12);
    //绘制边框
    circle(x, y, 12);

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
        int start_x = graph.location[start].first;
        int start_y = graph.location[start].second;
        int end_x = graph.location[end].first;
        int end_y = graph.location[end].second;
        //用insertLine插入线
        insertLine(start_x,start_y,end_x,end_y);
    }
}

void Axis::initAxis() {
    settextstyle(32, 0, _T("微软雅黑"));
    setbkmode(TRANSPARENT);
    outtextxy(260, 375, _T("迭代过程"));
    outtextxy(860, 375, _T("评价函数"));
    // 绘制左侧坐标轴
    line(50, 350, 550, 350);  // x轴
    line(50, 350, 50, 50);  // y轴

    // 绘制坐标轴刻度
    for (int i = 50; i <= 550; i += 50) {
        line(i, 355, i, 345);   // x轴刻度
    }

    for (int i = 50; i <= 350; i += 50) {
        line(45, i, 55, i);   // y轴刻度
    }
    // 绘制右侧坐标轴
    line(650, 350, 1150, 350);  // x轴
    line(650, 350, 650, 50);  // y轴

    settextstyle(16, 0, _T("宋体"));
    int num = 0;
    // 绘制坐标轴刻度
    for (int i = 650; i <= 1150; i += 50) {
        line(i, 355, i, 345);   // x轴刻度
        outtextxy(i - 3, 355, _T(to_string(num).c_str()));
        num += 10;
    }

    num = 120;
    for (int i = 50; i <= 350; i += 50) {
        line(645, i, 655, i);   // y轴刻度
        if (num != 0)
            outtextxy(620, i - 5, _T(to_string(num).c_str()));
        num -= 20;
    }
    // 在坐标轴上标注文字
    outtextxy(1160, 340, _T("迭代次数"));
    outtextxy(655, 32, _T("函数值"));
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
        insertVertex(graph.location[i].first, graph.location[i].second,
                     identifierString.c_str(), color);
    }
}

void Axis::insertEvaluation(Graph &graph, int iteration_times, double evaluation) {
    if (iteration == 0) {
        former_iteration_times.push_back(iteration_times);
        former_evaluation.push_back(evaluation);
        // 设置填充颜色
        setfillcolor(RGB(192, 192, 192));
        setcolor(BLACK);
        // 设置边框线条样式
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
        fillcircle(650 + iteration_times * 5, 350 - evaluation * 2.5, 4);
        //绘制边框
        circle(650 + iteration_times * 5, 350 - evaluation * 2.5, 4);
    } else {
        former_iteration_times.push_back(iteration_times);
        former_evaluation.push_back(evaluation);
        // 设置填充颜色
        setfillcolor(RGB(192, 192, 192));
        setcolor(BLACK);
        // 设置边框线条样式
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
        fillcircle(650 + iteration_times * 5, 350 - evaluation * 2.5, 4);
        //绘制边框
        for(int i = 0; i < former_iteration_times.size() - 1; i++){
            fillcircle(650 + former_iteration_times[i] * 5, 350 - former_evaluation[i] * 2.5, 4);
            circle(650 + former_iteration_times[i] * 5, 350 - former_evaluation[i] * 2.5, 4);
            insertLine(650 + former_iteration_times[i] * 5, 350 - former_evaluation[i] * 2.5, 650 + former_iteration_times[i+1] * 5,
                       350 - former_evaluation[i+1] * 2.5);
        }

        //将前一个点和现在的点连线

/*        //将前一个点和现在的点连线
        insertLine(650 + iteration_times * 5, 350 - evaluation * 2.5, 650 + former_iteration_times * 5,
                   350 - former_evaluation * 2.5);*/
    }
    iteration++;
}

void Axis::insertAntLocation(Graph &graph, Ant ant) {
    //找到起点和终点在图上的x和y
    int x1 = graph.location[ant.now_vertex].first;
    int y1 = graph.location[ant.now_vertex].second;
    int x2 = graph.location[ant.next_vertex].first;
    int y2 = graph.location[ant.next_vertex].second;
    //找到需要时间和剩余时间的比例
    double proportion = ant.left_time / ant.road_time;

    //找到x和y的对应比例
    int current_x = int(x1 + proportion * (x2 - x1));
    int current_y = int(y1 + proportion * (y2 - y1));
    //绘制一个圆
    setfillcolor(RGB(192, 192, 192));
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
    fillcircle(current_x, current_y, 2);
    circle(current_x, current_y, 2);
}

void Axis::insertAntListLocation(Graph &graph, vector<Ant> ants) {
    //清空图
    resetAllLine(graph);
    for(const Ant &ant : ants){
        //找到起点和终点在图上的x和y
        int x1 = graph.location[ant.now_vertex].first;
        int y1 = graph.location[ant.now_vertex].second;
        int x2 = graph.location[ant.next_vertex].first;
        int y2 = graph.location[ant.next_vertex].second;
        //找到需要时间和剩余时间的比例
        double proportion = 1-(ant.left_time / ant.road_time);

        //找到x和y的对应比例
        int current_x = int(x1 + proportion * (x2 - x1));
        int current_y = int(y1 + proportion * (y2 - y1));
        //绘制一个圆
        setfillcolor(RGB(192, 192, 192));
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
        fillcircle(current_x, current_y, 2);
        circle(current_x, current_y, 2);
    }
}
