#pragma once
#include"Edge.h"
class TimedBipartiteGraph;
#include"TimedBipartiteGraph.h"

class LElists{
private:
    int gap;
    struct LEnode{
        vector<Edge*> Edges;
        int s,length;
        vector<LEnode*>children;
    };
    unordered_map<int,unordered_map<int,LEnode*>> table;
public:
    LElists(int gap);

    LEnode* creatNode(vector<int> PT);

    void insertEdge(Edge* e);

    void deleteEdge(Edge* e);

    void getEdge(LEnode*node,TimedBipartiteGraph &G);

    void getGraph(int length,vector<TimedBipartiteGraph>& G_set);
};