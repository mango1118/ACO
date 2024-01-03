#include "AntColony.h"

int AntColony::init() {
    this->iteration_num = ITERATION_TIME;
    return 0;
}

int AntColony::run() {
    Graph graph;
    setAntList(graph);
    for (int run_time = 0; run_time < RUN_TIME; run_time++) {
        cout << "--------------------run time: " << run_time + 1 << " -------------------- \n";
        for (int i = 0; i < ITERATION_TIME; i++) {
            setAntList(graph);
            int temp_time = iteration(graph);
            min_time = min(min_time, temp_time);
            if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 7 || i == 9 || i == 19 || i == 29 || i == 49 ||
                i == 79 || i == 99) {
                cout << "iteration time:" << i << "\t min time: " << min_time << "\t this time: " << temp_time << endl;
            }
            graph.resetVertexAntNum();  //重置初始节点人数
            graph.evaporatePheromones();    //蒸发信息素
        }
    }
}

AntColony::AntColony() {
}

int AntColony::iteration(Graph &graph) { //返回本次迭代时间
    int time = 0;
    bool flag = false;
    while (!flag) {
        if (iterationFinish(graph)) {
            flag = true;
            continue;
        }
        time++;
        for (Ant &ant: antList) {
            if (ant.arrive) {
                continue;
            } else if (ant.on_edge) {
                ant.goOneSecond(graph);
            } else {
                int can_go = ant.goToNextVertex(graph);
                if (can_go < 0) {//需要等待1秒
                    continue;
                }
            }
        }
        graph.evaporatePheromones();
    }
    return time;
}

bool AntColony::iterationFinish(Graph &graph) {
    int temp = 0;
    for (int i = 0; i < graph.end_vertex_num; i++) {
        temp += graph.vertex_ant_num[graph.end_vertex[i]];
    }
    if (temp == graph.ant_num) {
        return true;
    } else {
        return false;
    }
}

int AntColony::setAntList(Graph &graph) {
    antList.clear();
    for (int i = 0; i < graph.start_vertex_num; i++) {
        for (int j = 0; j < graph.vertex_ant_num[graph.start_vertex[i]]; j++) {
            Ant antInstance(i);
            antInstance.now_vertex = graph.start_vertex[i];
            antList.push_back(antInstance);
        }
    }
    return 0;
}
