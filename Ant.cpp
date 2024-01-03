#include "Ant.h"

Ant::Ant(int now_vertex) {
    init(now_vertex);
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
    on_edge = true;
    graph.vertex_ant_num[now_vertex]--;
    graph.matrix_capacity[now_vertex][next_vertex]--;
    velocity = getVelocity(graph);
    left_time = getNeedTime(graph);
    return 0;
}

int Ant::chooseNextVertex(Graph &graph) {
    int flag = getMaxPheromonesVertex(graph);
    return flag;
}

int Ant::updatePheromones(Graph &graph) {
    double pheromones = graph.pheromones[now_vertex][next_vertex];
    double cost = (PHEROMONE_UPDATE_CAPACITY * graph.matrix_capacity[now_vertex][next_vertex]) /
                  (PHEROMONE_UPDATE_LENGTH * graph.matrix_length[now_vertex][next_vertex]
                   + PHEROMONE_UPDATE_TIME * getNeedTime(graph));
    pheromones = (1 - EVAPORATE_RATE) * pheromones + (1 / cost) * COST_PARAMETER;
    graph.pheromones[now_vertex][next_vertex] = pheromones;
    return 0;
}

int Ant::goOneSecond(Graph &graph) {
    left_time -= 1;
    if (left_time <= 0) {
        left_time = 0;
        //到达终点了
        if (arriveEndVertex(graph))arrive = true;
        updatePheromones(graph);
        arriveVertex(graph);
    }
    return 0;
}

int Ant::arriveVertex(Graph &graph) {
    graph.vertex_ant_num[next_vertex]++;
    graph.matrix_capacity[now_vertex][next_vertex]++;
    now_vertex = next_vertex;
    on_edge = false;
    visited_vertex[now_vertex] = 1;
    return 0;
}

double Ant::getVelocity(Graph &graph) {
    double v = 0;
    int width = graph.matrix_width[now_vertex][next_vertex];
    int capacity = graph.matrix_capacity[now_vertex][next_vertex];
    v = VELOCITY_PARAMETER * (VELOCITY_CAPACITY_WEIGHT * capacity + VELOCITY_WIDTH_WEIGHT * width);
    return v;
}

double Ant::getNeedTime(Graph &graph) {
    double time = 0;
    int length = graph.matrix_length[now_vertex][next_vertex];
    time = length / velocity;
    return time;
}

int Ant::getMaxPheromonesVertex(Graph &graph) {
    double max_pheromone = numeric_limits<double>::lowest();
    int max_pheromone_node = -1;
    bool flag = false;
    for (int i = 0; i < graph.vertex_num; i++) {
        //未访问且存在路径且路径容量大于0的节点
        if (!visited_vertex[i] && graph.matrix_length[now_vertex][i] != 0 && graph.matrix_capacity[now_vertex][i] > 0) {
            double pheromone = graph.pheromones[now_vertex][i];
            if (pheromone > max_pheromone) {
                max_pheromone = pheromone;
                max_pheromone_node = i;
                flag = true;
            }
        }
    }
    if(flag) {
        return max_pheromone_node;
    }else{ //没有找到可行的节点
        return -1;
    }
}

bool Ant::arriveEndVertex(Graph &graph) {
    for (int i = 0; i < graph.end_vertex_num; i++) {
        if (next_vertex == graph.end_vertex[i])
            return true;
    }
    return false;
}

