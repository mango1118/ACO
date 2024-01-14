
#include "AntColony.h"

int AntColony::init() {
//    this->iteration_num = ITERATION_TIME;
    return 0;
}

int AntColony::run() {
    Graph graph;
    setAntList(graph);
    axis.initEasyX(graph);
//    Axis axis(graph);

    for (int run_time = 0; run_time < RUN_TIME; run_time++) {
        //用于计算平均时间
        long all_time = 0;
        long all_hurt = 0;
        min_time = LONG_MAX;
        int hurt_ant = INT32_MAX;
        hurt_ant = INT32_MAX;
        string print_run = "--------------------run time: " + to_string(run_time + 1) + " -------------------- \n";
        antColonyWriteInFile(RECORD_NAME, print_run);
        cout << print_run << endl;
        for (int i = 0; i < ITERATION_TIME; i++) {
            setAntList(graph);
            int temp_time = iteration(graph);
            all_time += temp_time;
            int temp_hurt = 0;
            for (Ant &element: antList) {
                if (element.hurt) {
                    temp_hurt++;
                }
            }
            all_hurt += temp_hurt;
            //记录最优解
            if (evaluationFunction(graph, temp_time, temp_hurt) < evaluationFunction(graph, min_time, hurt_ant)) {
                collectBestSolution(graph);
                axis.resetAllLine(graph);
                axis.insertEvaluation(graph, i, evaluationFunction(graph, temp_time, temp_hurt));
/*                for (int k = 0; k < MULTI_ANT_MAX; k++) {
                    axis.insertPath(bestAntList[k].route, graph);
                }*/
                for(int k = 0; k < graph.ant_num; k++){
                    axis.insertPath(antList[k].route, graph);
                }
                Sleep(1000);
            }
            //最佳的蚂蚁释放信息素
            bestAntReleasePheromone(graph);
            if (evaluationFunction(graph, temp_time, temp_hurt) < evaluationFunction(graph, min_time, hurt_ant)) {
//                axis.insertEvaluation(graph, i, evaluationFunction(graph, temp_time, temp_hurt));
//                Sleep(500);
                min_time = min(min_time, temp_time);
                hurt_ant = min(hurt_ant, temp_hurt);
                string print_line =
                        "iteration time: " + to_string(i + 1) + "\t min time: " + to_string(temp_time) +
                        //                        "\t hurt ant: " + to_string(temp_hurt) + "\t target function: " + to_string(0.1 * temp_time + 0.5 * temp_hurt);
                        "\t hurt ant: " + to_string(temp_hurt) + "\t target function: " +
                        to_string(evaluationFunction(graph, temp_time, temp_hurt));
/*                string print_line =
                        "iteration time: " + to_string(i + 1) + "\t min time: " + to_string(min_time) + "\t hurt ant: " +
                        to_string(hurt_ant);*/
                cout << print_line << endl;
                antColonyWriteInFile(RECORD_NAME, print_line);
            }
            //最后绘制一次评价函数
            if (i == ITERATION_TIME - 1) {
                axis.insertEvaluation(graph, i, evaluationFunction(graph, min_time, hurt_ant));
            }
            graph.resetVertexAntNum();  //重置初始节点人数
//            graph.evaporatePheromones();    //蒸发信息素
        }
//        cout << "best time: " << min_time << "\t average time: " << all_time / ITERATION_TIME << endl;
        string best_time =
                "average time: " + to_string(all_time / ITERATION_TIME)
                + "\t average hurt: " + to_string(all_hurt / ITERATION_TIME);
//        string best_time =
//                "best time: " + to_string(min_time) + "\t average time: " + to_string(all_time / ITERATION_TIME)
//                + "\t average hurt: " + to_string(all_hurt / ITERATION_TIME);
        antColonyWriteInFile(RECORD_NAME, best_time);
        cout << best_time << endl;
        //恢复图的初始信息素
        graph.initPheromones();
    }
    return 0;
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
        if(CHANGE_GRAPH == 1){
            if (time == CHANGE_GRAPH_TIME){
                changeGraph(graph);
            }
        }
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
        if (DRAW_PROCESS == 1) {
            //绘制蚂蚁当前位置
            axis.insertAntListLocation(graph, antList);
        }
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

    unordered_map<int, Ant> paretoList = findParetoOptimalPerStartVertex(antList, MULTI_ANT_MAX, graph);

    for (auto &pair: paretoList) {
        pair.second.leaveRoutePheromones(graph);
    }

    return 0;
}

int AntColony::collectBestSolution(Graph &graph) {

//    copy(antList.begin(), antList.end(), back_inserter(bestAntList));
//    sort(bestAntList.begin(), bestAntList.end(), [](const Ant &a, const Ant &b) {
//        return a.arrive_time < b.arrive_time;
//    });

    unordered_map<int, Ant> paretoAntList = findParetoOptimalPerStartVertex(antList, MULTI_ANT_MAX, graph);
    bestAntList.clear();
    // 遍历 paretoAntList 将最佳蚂蚁添加到 bestAntList 中
    for (const auto &pair: paretoAntList) {
        bestAntList.push_back(pair.second);
    }
//    copy(paretoAntList.begin(), paretoAntList.end(), back_inserter(bestAntList));
    return 0;
}

void AntColony::antColonyWriteInFile(const string &fileName, const string &data) {
    ofstream outfile;
    // 使用 ios::app 模式打开文件，实现追加写入
    outfile.open(fileName, ios::app);
    // 检查文件是否成功打开
    if (!outfile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }
    // 写入数据并换行
    outfile << data << endl;
    // 关闭文件
    outfile.close();
}

/*bool AntColony::isAlreadyInParetoOptimal(unordered_set<string>& visitedSet, const Ant& ant) {
    string key = to_string(ant.arrive_time) + "_" + (ant.hurt ? "1" : "0");

    if (visitedSet.find(key) != visitedSet.end()) {
        return true;
    }

    visitedSet.insert(key);
    return false;
}*/


// 判断是否支配
bool AntColony::dominates(const Ant &a, const Ant &b) {
    return (a.arrive_time < b.arrive_time && a.hurt < b.hurt);
}

/*// 寻找帕累托最优解
vector<Ant> AntColony::findParetoOptimal(const std::vector<Ant> &useAntList, int maxParetoSize) {
    std::vector<Ant> paretoSet;
    // Multi-objective optimization, keep all Pareto optimal solutions
    for (const Ant &currentAnt: useAntList) {
        bool isDominated = false;
        for (const Ant &otherAnt: useAntList) {
            if (&currentAnt != &otherAnt && dominates(otherAnt, currentAnt)) {
                isDominated = true;
                break;
            }
        }
        if (!isDominated) {
            paretoSet.push_back(currentAnt);
            if (paretoSet.size() >= maxParetoSize) {
                break; // Stop when the desired size is reached
            }
        }
    }
    return paretoSet;
}*/

// 寻找每个起点对应的帕累托最优解
std::unordered_map<int, Ant>
AntColony::findParetoOptimalPerStartVertex(const std::vector<Ant> &useAntList, int maxParetoSize, Graph &graph) {
    std::unordered_map<int, Ant> paretoSetPerStartVertex;

    // Multi-objective optimization, keep Pareto optimal solutions for each start vertex
    for (int startVertex = 0; startVertex < graph.start_vertex_num; ++startVertex) {
        // Filter ants that have the current start vertex
        std::vector<Ant> antsWithStartVertex;
        for (const Ant &ant: useAntList) {
            if (ant.route.empty()) {
                continue; // Skip ants with no routes
            }

            if (ant.route.front() == startVertex) {
                antsWithStartVertex.push_back(ant);
            }
        }

        // Find Pareto optimal solutions for the current start vertex
        for (const Ant &currentAnt: antsWithStartVertex) {
            bool isDominated = false;
            for (const Ant &otherAnt: antsWithStartVertex) {
                if (&currentAnt != &otherAnt && dominates(otherAnt, currentAnt)) {
                    isDominated = true;
                    break;
                }
            }
            if (!isDominated) {
                // Store the Pareto optimal ant for the current start vertex
                paretoSetPerStartVertex.emplace(startVertex, currentAnt);
                if (paretoSetPerStartVertex.size() >= maxParetoSize) {
                    break; // Stop when the desired size is reached for the current start vertex
                }
            }
        }
    }

    return paretoSetPerStartVertex;
}

double AntColony::evaluationFunction(Graph &graph, int time, int hurt) {
    double temp = time * EVALUATION_TIME + hurt * EVALUATION_SAFE;
    temp = (temp - MIN_EVALUATION) / (MAX_EVALUATION - MIN_EVALUATION) * LINEAR;
    return temp;
}

int AntColony::changeGraph(Graph &graph) {
    graph.renewGraph(RENEW_GRAPH_NAME);
    return 0;
}
