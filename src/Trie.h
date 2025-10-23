#pragma once
#include<unordered_map>
#include<vector>
using namespace std;

class Trie{
private:
    struct node{
        unordered_map<int,node> children;
        vector<vector<int>> PTS;
    };
    unordered_map<int,node> root;
public:
    bool findPTS(vector<int>& times,vector<vector<int>>& PTS);

    void addPTS(vector<int>& times,vector<vector<int>> PTS);
};