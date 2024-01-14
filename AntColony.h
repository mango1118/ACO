#include "ACO.h"
#include "Ant.h"
#include "Axis.h"

class AntColony{
public:

    int min_time;                             //最短迭代时间
    double min_evaluation;                    //最小评价函数
    vector<Ant> antList;                      //每轮迭代的蚂蚁
    vector<Ant> bestAntList;                  //最优轮对应的蚂蚁
    vector<Ant> paretoBestAntList;            //帕累托最优解蚂蚁集
    Axis axis;

    AntColony();
    int init();                                             //算法初始化
    int run();                                              //算法运行
    int iteration(Graph &graph);                            //迭代一次
    bool iterationFinish(Graph &graph);                     //判断迭代是否完成
    int setAntList(Graph &graph);                           //设置蚁群
    int bestAntReleasePheromone(Graph &graph);              //一轮迭代后，最佳蚂蚁释放信息素
    int collectBestSolution(Graph &graph);                  //记录最优解
    double evaluationFunction(Graph &graph, int time, int hurt);//计算评价函数
//    int adjustAntVelocity(Graph &graph, int former_now_vertex, int former_next_vertex);                    //蚂蚁到达一个点，调整图容量和路径上的蚂蚁速度，剩余时间

//    bool betterEvaluation(Graph &graph);

    bool dominates(const Ant &a, const Ant &b);

    //txt utils
    void antColonyWriteInFile(const string &fileName, const string &data);

    unordered_map<int, Ant> findParetoOptimalPerStartVertex(const vector<Ant> &useAntList, int maxParetoSize, Graph &graph);
};