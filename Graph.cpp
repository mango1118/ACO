#include "Graph.h"

int Graph::readGraphByFile(const string &fileName) {
    int i = 0;
    int temp = 0;
    ifstream inputFile(fileName);
    istringstream iss;
    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << std::endl;
        return -1;
    }
    string line;

    // 从文件中读取节点数和边数
    getline(inputFile, line);
    iss.str(line);
    iss >> vertex_num >> edge_num;
    iss.clear();

    // 从文件中读取起点数和终点数
    getline(inputFile, line);
    iss.str(line);
    iss >> start_vertex_num >> end_vertex_num;
//    cout << start_vertex_num << endl;
    iss.clear();

    //读入起点数组
    getline(inputFile, line);
    iss.str(line);
    while (iss >> temp) {
        start_vertex[i++] = temp;
        // 过滤空格
        iss.ignore();
    }
    i = 0;
    iss.clear();

    //读入起点人数，得到总人数
    getline(inputFile, line);
    iss.str(line);
    while (iss >> temp) {
        vertex_ant_num[start_vertex[i++]] = temp;
        ant_num += temp;
        iss.ignore();
    }
    copy(begin(vertex_ant_num), end(vertex_ant_num), bak_vertex_ant_num);

    i = 0;
    iss.clear();

    //读入终点数组
    getline(inputFile, line);
    iss.str(line);
    while (iss >> temp) {
        end_vertex[i++] = temp;
        // 过滤空格
        iss.ignore();
    }
    i = 0;
    iss.clear();

    //读入所有边
    for (i = 0; i < edge_num; i++) {
        getline(inputFile, line);
        iss.str(line);
        int start = 0;
        int end = 0;
        iss >> start;
        iss >> end;
        iss >> matrix_length[start][end];
        iss >> matrix_capacity[start][end];
        bak_matrix_capacity[start][end] = matrix_capacity[start][end];
        iss >> matrix_width[start][end];
        iss >> matrix_danger[start][end];
        iss.clear();
    }

    // 关闭文件
    inputFile.close();

    return 0;
}

int Graph::renewGraphByFile(const string &fileName) {
    return 0;
}

int Graph::initPheromones() {
    int i = 0;
    int j = 0;
    double re_pheromone = 0;
    for (i = 0; i < start_vertex_num; i++) {
        for (j = 0; j < end_vertex_num; j++) {
            vector<int> temp_path = dijkstra(start_vertex[i], end_vertex[j]);
            int length = 0;
            for (int k = 0; k < temp_path.size() - 1; k++) {
//                length += matrix_length[i][j];
                length += matrix_length[temp_path[k+1]][temp_path[k]];
            }
            re_pheromone += length * temp_path.size();
        }
    }
    initial_pheromones = 1 / re_pheromone;
    for (i = 0; i < vertex_num; i++) {
        for (j = 0; j < vertex_num; j++) {
            if (matrix_length[i][j] == 0) {
                continue;
            } else {
                pheromones[i][j] = 1 / re_pheromone;
//                pheromones[i][j] = calculatePheromones(i, j);
            }
        }
    }
    return 0;
}

int Graph::evaporatePheromones() {
    int i = 0;
    int j = 0;
    for (i = 0; i < vertex_num; i++) {
        for (j = 0; j < vertex_num; j++) {
            if (matrix_length[i][j] == 0) {
                continue;
            } else {
                pheromones[i][j] = (1.0 - LOCAL_EVAPORATE_RATE) * pheromones[i][j];
            }
        }
    }
}

int Graph::updatePheromones() {
    return 0;
}

Graph::Graph() {
    init();
}

int Graph::init() {
    readGraphByFile(FILE_NAME);
    initPheromones();
    getAllDijkstraNext();
    return 0;
}

int Graph::resetVertexAntNum() {
    copy(begin(bak_vertex_ant_num), end(bak_vertex_ant_num), vertex_ant_num);
    return 0;
}

double Graph::calculatePheromones(int i, int j) {
    return PHEROMONE_INIT *
           (PHEROMONE_INIT_CAPACITY * matrix_capacity[i][j] + PHEROMONE_INIT_WIDTH * matrix_width[i][j]) /
           (PHEROMONE_INIT_LENGTH * matrix_length[i][j]);;
}

vector<int> Graph::dijkstra(int src, int dest) {

    int dist[vertex_num];
    int parent[vertex_num];
    bool sptSet[vertex_num];
    for (int i = 0; i < vertex_num; i++)
        dist[i] = INT_MAX, sptSet[i] = false, parent[i] = -1;
    dist[src] = 0;
    for (int count = 0; count < vertex_num - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (int v = 0; v < vertex_num; v++)
            if (!sptSet[v] && matrix_length[u][v] && dist[u] != INT_MAX && dist[u] + matrix_length[u][v] < dist[v])
                dist[v] = dist[u] + matrix_length[u][v], parent[v] = u;
    }
    // 存储每个节点对应到终点的最短路径下一跳
//    for (int i = 0; i < vertex_num; i++) {
//        int j = i;
//        while (j != -1) {
//            dijkstra_next_point[i] = j;
//            j = parent[j];
//        }
//    }
//    printSolution(dist, parent, src, dest);

    vector<int> path;
    int j = dest;
    while (j != -1) {
        path.push_back(j);
        j = parent[j];
    }
    return path;
}

int Graph::minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < vertex_num; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

int Graph::getAllDijkstraNext() {
    for (int i = 0; i < vertex_num - end_vertex_num; i++) {
        //先得到每个点到终点的dijkstra路径集合
        vector<vector<int>> dijkstraPathList;
        for (int j = 0; j < end_vertex_num; j++)
            dijkstraPathList.push_back(dijkstra(i, end_vertex[j]));

        //遍历每个路径，找到最短的路径
        vector<int> min = dijkstraPathList[0];
        int min_length = getPathLength(dijkstraPathList[0]);
        int index = 0;
        for(int k = 0; k < dijkstraPathList.size(); k++){
            int temp_length = getPathLength(dijkstraPathList[k]);
            if(temp_length < min_length){
                min_length = temp_length;
                min = dijkstraPathList[k];
            }
        }
        //得到i节点的下一跳
        int next_hop = min[min.size() - 2];
        dijkstra_next_point[i] = next_hop;
    }
}

int Graph::getPathLength(vector<int> path){
    int length = 0;
    for(int i = 0; i < path.size() - 1; i++){
        length += matrix_length[path[i+1]][path[i]];
    }
}



//int Graph::getAllDijkstraNext() {
//    for (int i = 0; i < vertex_num - end_vertex_num; i++) {
//        for (int j = 0; j < end_vertex_num; j++) {
//            int length = 0;
//            vector<int> path = dijkstra(i, end_vertex[j]);
//
//            // Check if the path is valid
//            if (path.size() > 1) {
//                // Store the length of the path
//                for (int k = 0; k < path.size() - 1; k++) {
//                    length += matrix_length[path[k]][path[k + 1]];
//                }
//            }
//        }
//    }
//}