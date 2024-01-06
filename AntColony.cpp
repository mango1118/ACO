
#include "AntColony.h"

int AntColony::init() {
    this->iteration_num = ITERATION_TIME;
    return 0;
}

int AntColony::run() {
    Graph graph;
    setAntList(graph);
    for (int run_time = 0; run_time < RUN_TIME; run_time++) {
        long all_time = 0;
        min_time = MAX_TIME;
        cout << "--------------------run time: " << run_time + 1 << " -------------------- \n";
        for (int i = 0; i < ITERATION_TIME; i++) {
            setAntList(graph);
            int temp_time = iteration(graph);
            all_time += temp_time;
            //记录最优解
            if(temp_time < min_time){
                collectBestSolution(graph);
            }
            //最佳的蚂蚁释放信息素
            bestAntReleasePheromone(graph);
            if (temp_time < min_time) {
                min_time = min(min_time, temp_time);
                cout << "iteration time: " << i + 1 << "\t min time: " << min_time << endl;
            }

//            min_time = min(min_time, temp_time);
//            if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 7 || i == 9 || i == 19 || i == 29 || i == 49 ||
//                i == 79 || i == 99) {
//                cout << "iteration time:" << i + 1<< "\t min time: " << min_time << "\t this time: " << temp_time << endl;
//            }

//            min_time = min(min_time, temp_time);
//            cout << "iteration time:" << i + 1<< "\t min time: " << min_time << "\t this time: " << temp_time << endl;

            graph.resetVertexAntNum();  //重置初始节点人数
//            graph.evaporatePheromones();    //蒸发信息素
        }
        cout <<"best time: " << min_time << "\t average time: " << all_time / ITERATION_TIME << endl;
        //恢复图的初始信息素
        graph.initPheromones();
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
                ant.arrive_time++;
                ant.goOneSecond(graph);
            } else {
                ant.arrive_time++;
                int can_go = ant.goToNextVertex(graph);
                if (can_go < 0) {//需要等待1秒
                    continue;
                }
            }
        }
//        graph.evaporatePheromones();
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

int AntColony::bestAntReleasePheromone(Graph &graph) {
    // 按arrive_time升序排序
//    sort(antList.begin(), antList.end(), [](const Ant &a, const Ant &b) {
//        return a.arrive_time < b.arrive_time;
//    });
    int k = GLOBAL_RELEASE_ANT;
    //只有排名前k的蚂蚁才能留下全局信息素
//    for (int i = 0; i < std::min(k, static_cast<int>(antList.size())); ++i) { antList[i].leaveRoutePheromones(graph, k); }
//    return 0;
    for (int i = 0; i < std::min(k, static_cast<int>(bestAntList.size())); ++i) { bestAntList[i].leaveRoutePheromones(graph, k); }
    return 0;
}

int AntColony::collectBestSolution(Graph &graph) {
    copy(antList.begin(), antList.end(), back_inserter(bestAntList));
    sort(bestAntList.begin(), bestAntList.end(), [](const Ant &a, const Ant &b) {
        return a.arrive_time < b.arrive_time;
    });
    return 0;
}
