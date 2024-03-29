#include "ACO.h"
#include "Graph.h"

class Ant{
public:
    int now_vertex;                         //目前所处节点
    int next_vertex;                        //下一个目标节点
    bool on_edge = false;                   //是否在边上
    double left_time;                       //在路上的剩余时间
    double velocity;                        //在路上的速度
    int visited_vertex[MAX] = {0};          //记录走过的节点
    bool arrive = false;                    //到达终点
    int arrive_time = 0;                    //到达终点所需时间
    vector<int> route;                      //走过的路径
    bool hurt = false;                      //是否受伤

    explicit Ant(int now_vertex);
    int init(int now_vertex);
    int chooseNextVertex(Graph &graph);                 //选择下一个节点，返回下一个节点
    int goToNextVertex(Graph &graph);                   //前往下一个节点，设置ant的成员变量
    int updatePheromones(Graph &graph);                 //更新路径信息素
    int goOneSecond(Graph &graph);                      //前进一秒
    int arriveVertex(Graph &graph);                     //到达目标点
    double getVelocity(Graph &graph);                   //计算速度
    double getNeedTime(Graph &graph);                   //计算剩余时间
    int getMaxPheromonesVertex(Graph &graph);           //得到信息素最大的下一个节点
    int getGreedyVertex(Graph &graph);                  //贪心算法得到下一个节点
    int getDijkstraVertex(Graph &graph);                //dijkstra得到下一个节点
    int getRandomVertex(Graph &graph);                  //得到随机节点
    int getRouletteVertex(Graph &graph);                //轮盘赌
    bool arriveEndVertex(Graph &graph);                 //判断是否到达终点
    int leaveRoutePheromones(Graph &graph);      //在路径上更新全局信息素
};