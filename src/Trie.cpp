#include"Trie.h"

bool Trie::findPTS(vector<int>& times,vector<vector<int>>& PTS){
    bool flag=true;
    node*temp;
    if(times.empty())
        return false;
    for(auto it=times.begin();it!=times.end()&&flag;it++){//查找Trie里是否存有当前时间戳序列的PTS
        if(it==times.begin()){
            if(root.find(*it)!=root.end()){
                temp=&root.find(*it)->second;
            }else{
                flag=false;
            }
        }else{
            if(temp->children.find(*it)!=temp->children.end()){
                temp=&temp->children.find(*it)->second;
            }else{
                flag=false;
            }
        }
    }
    if(flag&&!temp->PTS.empty()){
        PTS=temp->PTS;
        return true;
    }else{
        return false;
    }
}

void Trie::addPTS(vector<int>& times,vector<vector<int>> PTS){
    node*temp;
    for(auto it=times.begin();it!=times.end();it++){
        if(it==times.begin()){
            if(root.find(*it)!=root.end()){
                temp=&root.find(*it)->second;
            }else{
                temp = &root[*it];
            }
        }else{
            if(temp->children.find(*it)!=temp->children.end()){
                temp=&temp->children.find(*it)->second;
            }else{
                temp=&temp->children[*it];
            }
        }
    }
    if(!times.empty())
        temp->PTS=PTS;
}