#pragma once
#include"Node.h"

class Node;

class Edge{
    friend class TimedBipartiteGraph;
    friend class LElists;
private:
    int u,v;
    bool isExit=true;
    unordered_set <int> edgeTimes;
    vector<vector<int>> PTS;
public:
    Edge();

    Edge(int u,int v);

    vector<vector<int>> getEdgePTS(int length);//获取边的周期时间支撑集
};