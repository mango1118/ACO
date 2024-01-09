#include "ACO.h"
#include "Ant.h"
class AntColony{
public:
    int iteration_num{};                      //迭代次数
    int path_select_method{};                 //路径选择方法
    int pheromone_update_method{};            //信息素更新方法
    int min_time;                             //最短迭代时间
    int min_hurt;
    vector<Ant> antList;                      //每轮迭代的蚂蚁
    vector<Ant> bestAntList;                  //最优轮对应的蚂蚁
    vector<Ant> paretoBestAntList;            //帕累托最优解蚂蚁集

    AntColony();
    int init();                                             //算法初始化
    int run();                                              //算法运行
    int iteration(Graph &graph);                            //迭代一次
    bool iterationFinish(Graph &graph);                     //判断迭代是否完成
    int setAntList(Graph &graph);                           //设置蚁群
    int bestAntReleasePheromone(Graph &graph);              //一轮迭代后，最佳蚂蚁释放信息素
    int collectBestSolution(Graph &graph);                  //记录最优解
    double evaluationFunction(Graph &graph, int time, int hurt);//计算评价函数

//    vector<Ant> calculateParetoBestAntList(Graph &graph);   //计算帕累托最优解
    bool dominates(const Ant &a, const Ant &b);

    //txt utils
    void antColonyWriteInFile(const string &fileName, const string &data);
//    bool isAlreadyInParetoOptimal(unordered_set<string> &visitedSet, const Ant &ant);
    unordered_map<int, Ant> findParetoOptimalPerStartVertex(const vector<Ant> &useAntList, int maxParetoSize, Graph &graph);
};