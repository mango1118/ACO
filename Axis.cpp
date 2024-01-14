#include "Axis.h"

Axis::Axis() {

}
//Axis::Axis(Graph &graph) {
//    initEasyX(graph);
//}

void Axis::initEasyX(Graph &graph) {
    initgraph(GRAPH_WIDTH, GRAPH_HEIGHT);
    // ���ð�ɫ����
    setbkcolor(WHITE);
    cleardevice();
    // �����ߵ���ɫΪ��ɫ
    setcolor(BLACK);
//    initGraph(graph);
    initAxis();
}

void Axis::insertVertex(const char *data, int x, int y, COLORREF color) {
    settextstyle(20, 0, _T("Times New Roman"));
    // ���������ɫ
    setfillcolor(color);
    // ���ñ߿�������ʽ
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
    fillcircle(x, y, 12);
    //���Ʊ߿�
    circle(x, y, 12);

    //���ñ���
    setbkmode(TRANSPARENT);
    outtextxy(x - 5, y - 8, data);
}

void Axis::insertLine(int x1, int y1, int x2, int y2) {
    // ����������ɫ
    setcolor(BLACK);
    // �����ߵ���ʼ����յ�����
    int startX = x1, startY = y1;
    int endX = x2, endY = y2;
    // ��������
    line(startX, startY, endX, endY);
}

void Axis::resetAllLine(Graph &graph) {
    cleardevice();
    initImage();
//    Sleep(2000);
//    initGraph(graph);
    initAxis();

}

void Axis::insertPath(vector<int> path, Graph graph) {
//    resetAllLine(graph);
    for (int i = 0; i < path.size() - 1; i++) {
        //�ҵ����path[i]���յ�path[i+1]
        int start = path[i];
        int end = path[i + 1];
        //�ҵ������յ��Ӧ������
        int start_x = graph.location[start].first;
        int start_y = graph.location[start].second;
        int end_x = graph.location[end].first;
        int end_y = graph.location[end].second;
        //��insertLine������
        insertLine(start_x, start_y, end_x, end_y);
    }
}

void Axis::initAxis() {

    settextstyle(32, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT);
    outtextxy(260, 375, _T("��������"));
    outtextxy(1010, 375, _T("���ۺ���"));

//    // �������������
//    line(50, 350, 550, 350);  // x��
//    line(50, 350, 50, 50);  // y��
//    // ����������̶�
//    for (int i = 50; i <= 550; i += 50) {
//        line(i, 355, i, 345);   // x��̶�
//    }
//    for (int i = 50; i <= 350; i += 50) {
//        line(45, i, 55, i);   // y��̶�
//    }

    // �����Ҳ�������
    line(800, 350, 1300, 350);  // x��
    line(800, 350, 800, 50);  // y��

    settextstyle(16, 0, _T("����"));
    int num = 0;
    // ����������̶�
    for (int i = 800; i <= 1300; i += 50) {
        line(i, 355, i, 345);   // x��̶�
        outtextxy(i - 3, 355, _T(to_string(num).c_str()));
        num += 10;
    }

    num = 120;
    for (int i = 50; i <= 350; i += 50) {
        line(795, i, 805, i);   // y��̶�
        if (num != 0)
            outtextxy(770, i - 5, _T(to_string(num).c_str()));
        num -= 20;
    }
    // ���������ϱ�ע����
    outtextxy(1310, 340, _T("��������"));
    outtextxy(805, 32, _T("����ֵ"));
}

void Axis::initGraph(Graph &graph) {
    for (int i = 0; i < graph.vertex_num; i++) {
        COLORREF color = RGB(192, 192, 192);
        for (int j = 0; j < graph.start_vertex_num; j++) //����Ƿ������
            if (graph.start_vertex[j] == i)
                color = RGB(240, 135, 132);
        for (int j = 0; j < graph.end_vertex_num; j++)   //����Ƿ����յ�
            if (graph.end_vertex[j] == i)
                color = RGB(72, 172, 255);
        // �ڴ��ݸ� insertVertex ֮ǰ�� char ת��Ϊ�ַ���
        string identifierString(1, graph.identifier[i]);
        insertVertex(
                identifierString.c_str(), graph.location[i].first, graph.location[i].second, color);
    }
}

void Axis::insertEvaluation(Graph &graph, int iteration_times, double evaluation) {
    if (iteration == 0) {
        former_iteration_times.push_back(iteration_times);
        former_evaluation.push_back(evaluation);
        // ���������ɫ
        setfillcolor(RGB(192, 192, 192));
        setcolor(BLACK);
        // ���ñ߿�������ʽ
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
        fillcircle(800 + iteration_times * 5, 350 - evaluation * 2.5, 4);
        //���Ʊ߿�
        circle(800 + iteration_times * 5, 350 - evaluation * 2.5, 4);
    } else {
        former_iteration_times.push_back(iteration_times);
        former_evaluation.push_back(evaluation);
        // ���������ɫ
        setfillcolor(RGB(192, 192, 192));
        setcolor(BLACK);
        // ���ñ߿�������ʽ
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
        fillcircle(800 + iteration_times * 5, 350 - evaluation * 2.5, 4);
        //���Ʊ߿�
        for (int i = 0; i < former_iteration_times.size() - 1; i++) {
            fillcircle(800 + former_iteration_times[i] * 5, 350 - former_evaluation[i] * 2.5, 4);
            circle(800 + former_iteration_times[i] * 5, 350 - former_evaluation[i] * 2.5, 4);
            insertLine(800 + former_iteration_times[i] * 5, 350 - former_evaluation[i] * 2.5,
                       800 + former_iteration_times[i + 1] * 5,
                       350 - former_evaluation[i + 1] * 2.5);
        }

        //��ǰһ��������ڵĵ�����

/*        //��ǰһ��������ڵĵ�����
        insertLine(650 + iteration_times * 5, 350 - evaluation * 2.5, 650 + former_iteration_times * 5,
                   350 - former_evaluation * 2.5);*/
    }
    iteration++;
}

void Axis::insertAntLocation(Graph &graph, Ant ant) {
    //�ҵ������յ���ͼ�ϵ�x��y
    int x1 = graph.location[ant.now_vertex].first;
    int y1 = graph.location[ant.now_vertex].second;
    int x2 = graph.location[ant.next_vertex].first;
    int y2 = graph.location[ant.next_vertex].second;
    //�ҵ���Ҫʱ���ʣ��ʱ��ı���
    double proportion = ant.left_time / ant.road_time;

    //�ҵ�x��y�Ķ�Ӧ����
    int current_x = int(x1 + proportion * (x2 - x1));
    int current_y = int(y1 + proportion * (y2 - y1));
    //����һ��Բ
    setfillcolor(RGB(192, 192, 192));
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
    fillcircle(current_x, current_y, 2);
    circle(current_x, current_y, 2);
}

void Axis::insertAntListLocation(Graph &graph, vector<Ant> ants) {
    //���ͼ
    resetAllLine(graph);
    for (const Ant &ant: ants) {
        //�ҵ������յ���ͼ�ϵ�x��y
        int x1 = graph.location[ant.now_vertex].first;
        int y1 = graph.location[ant.now_vertex].second;
        int x2 = graph.location[ant.next_vertex].first;
        int y2 = graph.location[ant.next_vertex].second;
        //�ҵ���Ҫʱ���ʣ��ʱ��ı���
        double proportion = 1 - (ant.left_time / ant.road_time);

        //�ҵ�x��y�Ķ�Ӧ����
        int current_x = int(x1 + proportion * (x2 - x1));
        int current_y = int(y1 + proportion * (y2 - y1));
        //����һ��Բ
        setfillcolor(RGB(192, 192, 192));
        setlinestyle(PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 2);
        fillcircle(current_x, current_y, 2);
        circle(current_x, current_y, 2);
    }
}

void Axis::initImage() {
    if (PRINT_IMAGE == 1) {
        //����ͼ
        IMAGE img;
        loadimage(&img, _T(GRAPH_NAME), 700, 400);
        putimage(50, 0, &img);
    }
}
