
#include "AntColony.h"

int AntColony::init() {
    this->iteration_num = ITERATION_TIME;
    return 0;
}

int AntColony::run() {
    Graph graph;
    setAntList(graph);
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
            if (temp_time < min_time || temp_hurt < hurt_ant) {
                collectBestSolution(graph);
            }
            //最佳的蚂蚁释放信息素
            bestAntReleasePheromone(graph);
            if (temp_time < min_time || temp_hurt < hurt_ant) {
                min_time = min(min_time, temp_time);
                hurt_ant = min(hurt_ant, temp_hurt);
                string print_line =
                        "iteration time: " + to_string(i + 1) + "\t min time: " + to_string(temp_time) +
                        "\t hurt ant: " + to_string(temp_hurt) + "\t target function: " + to_string(0.1 * temp_time + 0.5 * temp_hurt);
/*                string print_line =
                        "iteration time: " + to_string(i + 1) + "\t min time: " + to_string(min_time) + "\t hurt ant: " +
                        to_string(hurt_ant);*/
                cout << print_line << endl;
                antColonyWriteInFile(RECORD_NAME, print_line);
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
//        for(int i = 0; i < NEW_RECORD_NUM; i++){
//            if(time == NEW_RECORD_TIME_1)
//                graph.renewGraphByFile(NEW_RECORD_1);
//        }
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
//    int k = GLOBAL_RELEASE_ANT;
    //只有排名前k的蚂蚁才能留下全局信息素
//    for (int i = 0; i < min(k, static_cast<int>(antList.size())); ++i) { antList[i].leaveRoutePheromones(graph, k); }
//    return 0;
//    for (int i = 0; i < min(k, static_cast<int>(bestAntList.size())); ++i) {
//        bestAntList[i].leaveRoutePheromones(graph, k);
//    }
    vector<Ant> paretoList = findParetoOptimal(antList, MULTI_ANT_MAX);
    for (Ant &element: paretoList) {
        element.leaveRoutePheromones(graph);
    }
    return 0;
}

int AntColony::collectBestSolution(Graph &graph) {

//    copy(antList.begin(), antList.end(), back_inserter(bestAntList));
//    sort(bestAntList.begin(), bestAntList.end(), [](const Ant &a, const Ant &b) {
//        return a.arrive_time < b.arrive_time;
//    });

    vector<Ant> paretoAntList = findParetoOptimal(antList, MULTI_ANT_MAX);
    copy(paretoAntList.begin(), paretoAntList.end(), back_inserter(bestAntList));

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

// 寻找帕累托最优解
vector<Ant> AntColony::findParetoOptimal(const std::vector<Ant> &useAntList, int maxParetoSize) {
    std::vector<Ant> paretoSet;

    if (MULTI_TARGET == 1) {
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
    } else if (MULTI_TARGET == 2) {
        // Return the solution with the shortest time
        Ant shortestTimeAnt = useAntList[0];
        for (const Ant &ant: useAntList) {
            if (ant.arrive_time < shortestTimeAnt.arrive_time) {
                shortestTimeAnt = ant;
            }
        }
        paretoSet.push_back(shortestTimeAnt);
    } else if (MULTI_TARGET == 3) {
        // Return the safest solution (for example, the one with the least hurt)
/*        Ant safestAnt = useAntList[0];
        for (const Ant& ant : useAntList) {
            if (ant.hurt < safestAnt.hurt) {
                safestAnt = ant;
            }
        }
        paretoSet.push_back(safestAnt);*/
        //返回受伤蚂蚁
        for (const Ant &ant: useAntList) {
            if (ant.hurt) {
                paretoSet.push_back(ant);
                if (paretoSet.size() > maxParetoSize) {
                    break;
                }
            }
        }
    }

    return paretoSet;
}


/*
bool AntColony::isParetoOptimal(const Ant &ant1, const Ant &ant2) {
    return (ant1.arrive_time <= ant2.arrive_time && ant1.hurt <= ant2.hurt) &&
           (ant1.arrive_time < ant2.arrive_time || ant1.hurt < ant2.hurt);
}

// 函数名称：findParetoOptimal
// 函数描述：根据帕累托最优解的原理，从给定的蚂蚁列表中找到帕累托最优解集合。
// 参数：
//   antList：存储蚂蚁信息的向量，每个蚂蚁具有到达时间（arrive_time）和受伤状态（hurt）。
// 返回值：帕累托最优解集合，以向量形式返回。
vector<Ant> AntColony::findParetoOptimal(const vector<Ant> &useAntList) {
    // 存储帕累托最优解的向量
    vector<Ant> paretoOptimalSet;

    // 存储已访问解的哈希集合，用于排除重复解
    unordered_set<string> visitedSet;

    // 遍历蚂蚁列表
    for (const Ant &ant: useAntList) {
        // 判断当前蚂蚁是否为帕累托最优解的候选解
        bool isParetoOptimalCandidate = true;

        // 检查当前蚂蚁与其他蚂蚁的关系，排除劣势解
        for (const Ant &otherAnt: useAntList) {
            if (&ant != &otherAnt && isParetoOptimal(otherAnt, ant)) {
                isParetoOptimalCandidate = false;
                // 不再需要中断循环，继续判断其他蚂蚁
            }
        }
        // 将候选解加入帕累托最优解集合
        paretoOptimalSet.push_back(ant);
    }

    // 根据 MULTI_TARGET 的值进行不同的排序和筛选
    if (MULTI_TARGET == 2) {
        // 返回时间最短的解
        sort(paretoOptimalSet.begin(), paretoOptimalSet.end(), [](const Ant &a, const Ant &b) {
            return a.arrive_time < b.arrive_time;
        });
        return {paretoOptimalSet.front()};
    } else if (MULTI_TARGET == 3) {
        // 返回最安全的解
        sort(paretoOptimalSet.begin(), paretoOptimalSet.end(), [](const Ant &a, const Ant &b) {
            return a.hurt < b.hurt;
        });
        return {paretoOptimalSet.front()};
    } else {
        // 默认返回多目标优化解
        return paretoOptimalSet;
    }

    // 返回帕累托最优解集合
    return paretoOptimalSet;
}
*/
