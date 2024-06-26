#include "Ant.h"

#define  k

Ant::Ant(int now_vertex) {
    init(now_vertex);
    route.push_back(now_vertex);
}

int Ant::init(int now_vertex) {
    for (int i = 0; i < MAX; i++) {
        visited_vertex[i] = 0;
    }
    visited_vertex[now_vertex] = 1;
    return 0;
}

int Ant::goToNextVertex(Graph &graph) {
    int temp = chooseNextVertex(graph);
    if (temp < 0) return -1;    //需要等待一秒
    else next_vertex = temp;
    route.push_back(next_vertex);
    on_edge = true;
    graph.vertex_ant_num[now_vertex]--;
    graph.matrix_capacity[now_vertex][next_vertex]--;

    if (graph.matrix_danger[now_vertex][next_vertex] > 0) {
        // 初始化随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        // 以受伤的概率将标志设置为true
        if (dis(gen) < graph.matrix_danger[now_vertex][next_vertex]) {
            hurt = true;
        }
    }
    velocity = getVelocity(graph);
    left_time = getNeedTime(graph);

    return 0;
}

int Ant::chooseNextVertex(Graph &graph) {
    if (SELECT_NEXT_METHOD == 1) {
        return getMaxPheromonesVertex(graph);
    } else if (SELECT_NEXT_METHOD == 2) {
        return getRouletteVertex(graph);
    }
}

int Ant::updatePheromones(Graph &graph) {
    double pheromones = graph.pheromones[now_vertex][next_vertex];
//    double cost = (PHEROMONE_UPDATE_CAPACITY * graph.matrix_capacity[now_vertex][next_vertex]) /
//                  (PHEROMONE_UPDATE_LENGTH * graph.matrix_length[now_vertex][next_vertex]
//                   + PHEROMONE_UPDATE_TIME * getNeedTime(graph));
//    pheromones = (1 - LOCAL_EVAPORATE_RATE) * pheromones - (1 / cost) * COST_PARAMETER;
//    pheromones = (1 - LOCAL_EVAPORATE_RATE) * pheromones - cost * COST_PARAMETER;
//    pheromones = (1 - LOCAL_EVAPORATE_RATE) * pheromones + LOCAL_EVAPORATE_RATE * graph.initial_pheromones;
    pheromones = (1 - LOCAL_EVAPORATE_RATE) * pheromones + LOCAL_EVAPORATE_RATE * graph.initial_pheromones;
//    pheromones = ((1 - LOCAL_EVAPORATE_RATE) * pheromones + LOCAL_EVAPORATE_RATE * graph.initial_pheromones) * 0.9;
    graph.pheromones[now_vertex][next_vertex] = pheromones;
    return 0;
}

double Ant::goOneSecond(Graph &graph) {
    //更新速度和时间
    double left_length = left_time * velocity;
    velocity = getVelocity(graph);
    left_time = left_length / velocity;
    road_time = graph.matrix_length[now_vertex][next_vertex] / velocity;

    left_time -= 1;
    if (left_time <= 0) {
        left_time = 0;
        //到达终点了
        if (arriveEndVertex(graph))arrive = true;
        updatePheromones(graph);
        arriveVertex(graph);
    }
    return left_time;
}

int Ant::arriveVertex(Graph &graph) {
    former_now_vertex = now_vertex;
    former_next_vertex = next_vertex;
    graph.vertex_ant_num[next_vertex]++;
    graph.matrix_capacity[now_vertex][next_vertex]++;
    now_vertex = next_vertex;
    on_edge = false;
    visited_vertex[now_vertex] = 1;
    return 0;
}

double Ant::getVelocity(Graph &graph) {
    double v = 0;
//    int length = graph.matrix_length[now_vertex][next_vertex];
//    int width = graph.matrix_width[now_vertex][next_vertex];
    double capacity = graph.bak_matrix_capacity[now_vertex][next_vertex];
    double true_capacity = graph.matrix_capacity[now_vertex][next_vertex];
    int edge_ant_num = capacity - true_capacity;
//    double density = edge_ant_num / length;
    //Greenshields模型
    v = AVERAGE_VELOCITY * (1 - ((capacity - true_capacity) / capacity));
    //如果受伤了，速度会下降
    if (hurt)
        v *= HURT_VELOCITY_RATE;
//    v = AVERAGE_VELOCITY * exp(-density * VELOCITY_PARAMETER);
//    v = VELOCITY_PARAMETER * (VELOCITY_CAPACITY_WEIGHT * true_capacity + VELOCITY_WIDTH_WEIGHT * width);
    return v;
}

double Ant::getNeedTime(Graph &graph) {
    double time = 0;
    int length = graph.matrix_length[now_vertex][next_vertex];
    time = length / velocity;
    road_time = length / velocity;
    return time;
}

bool Ant::arriveEndVertex(Graph &graph) {
    for (int i = 0; i < graph.end_vertex_num; i++) {
        if (next_vertex == graph.end_vertex[i])
            return true;
    }
    return false;
}

int Ant::getRouletteVertex(Graph &graph) {
    // 使用随机设备作为种子
    std::random_device rd;
    // 使用随机设备生成引擎
    std::mt19937 gen(rd());
    // 定义生成随机数的分布范围（浮点数）
    std::uniform_real_distribution<double> dis(0.0, 1.0); // 生成0到1之间的浮点数
    // 生成随机数
    double random_number = dis(gen);
    int next_point = -1;

//    if (random_number > RANDOM_RATE) {//轮盘赌
//        next_point = getRandomVertex(graph);
//    } else {
//        if (PATH_SELECTED_METHOD == 1) { //ACO
//            next_point =getMaxPheromonesVertex(graph);
//        } else if (PATH_SELECTED_METHOD == 2) { //greedy
//            next_point =getGreedyVertex(graph);
//        } else if (PATH_SELECTED_METHOD == 3) { //dijkstra
//            next_point =getDijkstraVertex(graph);
//        }
//    }

    if (PATH_SELECTED_METHOD == 1) { //ACO
        if (random_number > RANDOM_RATE) {//轮盘赌
            next_point = getRandomVertex(graph);
        } else {
            next_point = getMaxPheromonesVertex(graph);
        }
    } else if (PATH_SELECTED_METHOD == 2) { //greedy
        next_point = getGreedyVertex(graph);
    } else if (PATH_SELECTED_METHOD == 3) { //dijkstra
        next_point = getDijkstraVertex(graph);
    }

    return next_point;
}

int Ant::getRandomVertex(Graph &graph) {
    double max_pheromone = std::numeric_limits<double>::lowest();
    int max_pheromone_node = -1;
    bool flag = false;
    std::vector<int> candidate_nodes; // 用于存储符合条件的节点索引
    for (int i = 0; i < graph.vertex_num; i++) {
        // 未访问且存在路径且路径容量大于1的节点
        if (!visited_vertex[i] && graph.matrix_length[now_vertex][i] != 0 &&
            graph.matrix_capacity[now_vertex][i] > MIN_CAPACITY) {
            double pheromone = graph.pheromones[now_vertex][i];
            if (pheromone > max_pheromone) {
                max_pheromone = pheromone;
                max_pheromone_node = i;
                flag = true;
            }
            // 将符合条件的节点索引添加到候选节点列表
            candidate_nodes.push_back(i);
        }
    }
    if (flag) {
        // 从候选节点中随机选择一个节点
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, candidate_nodes.size() - 1);
        int random_index = dis(gen);
        return candidate_nodes[random_index];
    } else {
        // 没有找到可行的节点
        return -1;
    }
}

int Ant::leaveRoutePheromones(Graph &graph) {
    for (size_t i = 0; i < route.size() - 1; ++i) {
        int current_node = route[i];
        int next_node = route[i + 1];

        if (MULTI_TARGET == 1) {
            //受伤了，减信息素
            if (hurt) {
                graph.pheromones[current_node][next_node] =
                        MULTI_TIME_IMPORTANCE *
                        ((1 - GLOBAL_EVAPORATE_RATE) * graph.pheromones[current_node][next_node] +
                         (1 / double(arrive_time)))
                        +
                        MULTI_SAFE_IMPORTANCE * graph.pheromones[current_node][next_node] * (1 - MULTI_SAFE_PARAMETER);
            } else { //没受伤，加信息素
                graph.pheromones[current_node][next_node] =
                        MULTI_TIME_IMPORTANCE *
                        ((1 - GLOBAL_EVAPORATE_RATE) * graph.pheromones[current_node][next_node] +
                         (1 / double(arrive_time)))
                        +
                        MULTI_SAFE_IMPORTANCE * graph.pheromones[current_node][next_node] * (1 + MULTI_SAFE_PARAMETER);
            }
/*            graph.pheromones[current_node][next_node] =
                    MULTI_TIME_IMPORTANCE * ((1 - GLOBAL_EVAPORATE_RATE) * graph.pheromones[current_node][next_node] +
                                             (1 / double(arrive_time)))
                    + MULTI_SAFE_IMPORTANCE * graph.pheromones[current_node][next_node] * 1.2;;*/
        }
//        else if (MULTI_TARGET == 2) {
//            graph.pheromones[current_node][next_node] =
//                    (1 - GLOBAL_EVAPORATE_RATE) * graph.pheromones[current_node][next_node] + (1 / double(arrive_time));
//        } else if (MULTI_TARGET == 3) {
//            if (hurt) {
//                graph.pheromones[current_node][next_node] =
//                        graph.pheromones[current_node][next_node] * (1 - MULTI_SAFE_PARAMETER);
//            } else { //没受伤，加信息素
//                graph.pheromones[current_node][next_node] =
//                        graph.pheromones[current_node][next_node] * (1 + MULTI_SAFE_PARAMETER);
//            }
//        }

//
//        graph.pheromones[current_node][next_node] =
//                (1 - GLOBAL_EVAPORATE_RATE) * graph.pheromones[current_node][next_node] + (1 / double(arrive_time));
//        graph.pheromones[current_node][next_node] = graph.pheromones[current_node][next_node] + (1 / double(arrive_time));
//                (1 - GLOBAL_EVAPORATE_RATE) * graph.pheromones[current_node][next_node] + (1 / double(arrive_time));
    }
    return 0;
}

//int Ant::getMaxPheromonesVertex(Graph &graph) {
//    // 初始化最大信息素值为负无穷
//    double max_pheromone = numeric_limits<double>::lowest();
//    // 记录具有最大信息素值的节点，默认为无效节点
//    int max_pheromone_node = -1;
//    // 标志，用于检查是否找到符合条件的节点
//    bool flag = false;
//
//    // 遍历所有节点
//    for (int i = 0; i < graph.vertex_num; i++) {
//        // 检查条件：节点未被访问、存在路径、路径容量大于1
//        // 大于1是因为计算信息素不能为0，修改启发式后可能可以为0
//        if (!visited_vertex[i] && graph.matrix_length[now_vertex][i] != 0 &&
//            graph.matrix_capacity[now_vertex][i] > MIN_CAPACITY) {
////        if (!visited_vertex[i] && graph.matrix_length[now_vertex][i] != 0) {
//            // 获取当前边上的信息素值
//            double pheromone = graph.pheromones[now_vertex][i];
//
//            // 检查是否为最大信息素值
//            if (pheromone > max_pheromone) {
//                // 更新最大信息素值和对应的节点
//                max_pheromone = pheromone;
//                max_pheromone_node = i;
//                // 设置标志为true，表示找到符合条件的节点
//                flag = true;
//            }
//        }
//    }
//    // 检查是否找到符合条件的节点
//    if (flag) {
//        // 返回具有最大信息素值的节点
//        return max_pheromone_node;
//    } else {
//        // 没有找到可行的节点
//        return -1;
//    }
//}

//应该找到启发式与信息素的乘积最大值
int Ant::getMaxPheromonesVertex(Graph &graph) {
    // 初始化最大信息素值为负无穷

    double max_product = numeric_limits<double>::lowest();
//    double max_pheromone = numeric_limits<double>::lowest();
    // 记录具有最大信息素值的节点，默认为无效节点
    int max_product_node = -1;
    // 标志，用于检查是否找到符合条件的节点
    bool flag = false;

    // 遍历所有节点
    for (int i = 0; i < graph.vertex_num; i++) {
        // 检查条件：节点未被访问、存在路径、路径容量大于1
        // 大于1是因为计算信息素不能为0，修改启发式后可能可以为0
        if (!visited_vertex[i] && graph.matrix_length[now_vertex][i] != 0 &&
            graph.matrix_capacity[now_vertex][i] > MIN_CAPACITY) {
//        if (!visited_vertex[i] && graph.matrix_length[now_vertex][i] != 0) {
            // 获取当前边上的信息素值
            double pheromone = graph.pheromones[now_vertex][i];
            // 获取当前边上的启发式信息
            // 启发式 = 1 / （路径长度 * 路径密度）
            double heuristics = 1 / ((double (graph.matrix_length[now_vertex][i]) *
                                     (double(graph.bak_matrix_capacity[now_vertex][i]) - double(graph.matrix_capacity[now_vertex][i])) /
                                     (double(graph.matrix_length[now_vertex][i]))));
            // 获取乘积
            double product = pow(pheromone, ALPHA) * pow(heuristics, BETA);
            // 检查是否为最大乘积
            if (product > product) {
                // 更新最大信息素值和对应的节点
                max_product = product;
                max_product_node = i;
                // 设置标志为true，表示找到符合条件的节点
                flag = true;
            }
//            // 检查是否为最大信息素值
//            if (pheromone > max_pheromone) {
//                // 更新最大信息素值和对应的节点
//                max_pheromone = pheromone;
//                max_pheromone_node = i;
//                // 设置标志为true，表示找到符合条件的节点
//                flag = true;
//            }
        }
    }
    // 检查是否找到符合条件的节点
    if (flag) {
        // 返回具有最大信息素值的节点
        return max_product_node;
    } else {
        // 没有找到可行的节点
        return -1;
    }
}

int Ant::getDijkstraVertex(Graph &graph) {
    int next_point = -1;
    next_point = graph.dijkstra_next_point[now_vertex];
    if (graph.matrix_capacity[now_vertex][next_point] > MIN_CAPACITY) {
        return next_point;
    } else {
        return -1;
    }
}

int Ant::getGreedyVertex(Graph &graph) {
    int next_point = -1;
    int temp = INT32_MAX;
    for (int i = 0; i < graph.vertex_num; i++) {
//        if(graph.matrix_length[now_vertex][i] != 0 && graph.matrix_capacity[now_vertex][i] > MIN_CAPACITY){
        if (graph.matrix_length[now_vertex][i] != 0) {
            if (temp > graph.matrix_length[now_vertex][i]) {
                next_point = i;
                temp = graph.matrix_length[now_vertex][i];
            }
        }
    }
    if (graph.matrix_capacity[now_vertex][next_point] > MIN_CAPACITY) {
        return next_point;
    } else {
        return -1;
    }
//    return next_point;
}


