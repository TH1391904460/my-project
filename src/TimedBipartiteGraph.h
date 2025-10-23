#pragma once
#include<unordered_map>
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<queue>
#include"Node.h"
#include"Edge.h"
class LElists;
#include"LElists.h"
#include"Trie.h"
using namespace std;

class TimedBipartiteGraph{
private:
    unordered_map<int,Node> U,V;//节点集
    unordered_map<int,unordered_map<int,Edge>> E;//边集
    unordered_map<int,LElists*> LE;//保存边的链表
    Trie* trie=new Trie();
public:
    TimedBipartiteGraph();

    TimedBipartiteGraph(unordered_map<int,Node> U,unordered_map<int,Node> V);//构造二分团

    TimedBipartiteGraph(const string& filename);//读文件

    void insertEdge(Edge *e);//不涉及时间戳

    void deleteNode(Node &n);//删除节点,改变邻居节点的时间戳和邻居集

    void deleteEdge(Edge &e);//删边,改变两端点的邻居集和时间戳

    void BReduction(int alpha,int beta,int theta);//获得θ-周期(β,α)-core 子图

    void BReduction(int alpha,int beta);//获得(β,α)-core 子图(不检查节点的PTS)

    unordered_set<int> getNNset(Node &n);//获得某个节点的二跳邻居

    void backTrack(vector<int>& nums,vector<vector<int>>& result,vector<int>& current,int index,int beta);//回溯法找所有β长度的组合

    void PBlisting(vector<TimedBipartiteGraph>& C,int alpha,int beta,unordered_set<int> w,vector<unordered_set<int>> commonN,unordered_map<int,Node>& L);

    void BEnumeration(int alpha,int beta,vector<TimedBipartiteGraph>& C);//从周期子图中获得(α,β)-二分团

    bool isExist(vector<int>& time,unordered_set<int>& e);

    vector<TimedBipartiteGraph> getTimesGraph(int theta);//从θ-周期(β,α)-core图中枚举周期子图

    vector<TimedBipartiteGraph> BCombined(int alpha,int beta,int theta);

    void initLE(int theta);//为所有可能的间隔创建LE-lists

    vector<TimedBipartiteGraph> EEnumeration(int alpha,int beta,int theta);

    void test();
};