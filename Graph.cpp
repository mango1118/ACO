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
        iss >> matrix_width[start][end];
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
    for (i = 0; i < vertex_num; i++) {
        for (j = 0; j < vertex_num; j++) {
            if (matrix_length[i][j] == 0) {
                continue;
            } else {
                pheromones[i][j] = PHEROMONE_INIT *
                                   (PHEROMONE_INIT_CAPACITY * matrix_capacity[i][j] + PHEROMONE_INIT_WIDTH * matrix_width[i][j]) /
                                   (PHEROMONE_INIT_LENGTH * matrix_length[i][j]);
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
                pheromones[i][j] = (1.0 - EVAPORATE_RATE) * pheromones[i][j];
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

    return 0;
}

