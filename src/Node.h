#pragma once
#include<unordered_set>
#include<unordered_map>
#include<vector>
#include<algorithm>
using namespace std;

class Edge;

class Node{
    friend class Edge;
    friend class TimedBipartiteGraph;
private:
    int ID;
    char type;
    bool isExit=true;
    vector<vector<int>> PTS;
    unordered_map<int,int> nodeTimes;//节点的时间戳,第一个int存时间戳,第二个int存该时间戳对应的个数
    unordered_set<int> N_node;//邻居节点
public:
    Node();

    Node(int ID,char type);
    
    void addTime(int time);

    void deleteTime(int time);

    void addN_node(int n);

    void deleteN_node(int n);

    vector<vector<int>> getNodePTS(int length);//获取节点的周期时间支撑集

};