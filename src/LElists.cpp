#include"LElists.h"

LElists::LElists(int gap):gap(gap){}

LElists::LEnode* LElists::creatNode(vector<int> PT){
    LEnode*node=new LEnode;
    node->s=PT[0];
    node->length=PT[2];
    table[node->length][node->s] = node;
    if(PT[2]>2){//只有PT长度大于2的节点才有父母节点指向
        LEnode*fnode,*mnode;
        if(table.find(PT[2]-1)==table.end()||table[PT[2]-1].find(PT[0])==table[PT[2]-1].end()){
            fnode=creatNode({PT[0],PT[1],PT[2]-1});
        }else{
            fnode=table[PT[2]-1][PT[0]];
        }
        fnode->children.push_back(node);
        if(table.find(PT[2]-1)==table.end()||table[PT[2]-1].find(PT[0]+1)==table[PT[2]-1].end()){
            mnode=creatNode({PT[0]+1,PT[1],PT[2]-1});
        }else{
            mnode=table[PT[2]-1][PT[0]+1];
        }
        mnode->children.push_back(node);
    }
    return node;
}

void LElists::insertEdge(Edge* e){
    for(auto it=e->PTS.begin();it!=e->PTS.end();it++){
        if((*it)[1]==gap){
            LEnode*node;
            if(table.find((*it)[2])==table.end()||table[(*it)[2]].find((*it)[0])==table[(*it)[2]].end()){//如果不存在该节点,创建节点
                node=creatNode(*it);
            }else{
                node=table[(*it)[2]][(*it)[0]];
            }
            node->Edges.push_back(e);    
        }
    }
}

void LElists::deleteEdge(Edge* e){
    for(auto it=e->PTS.begin();it!=e->PTS.end();it++){
        if((*it)[1]==gap){
            if(table.find((*it)[2])!=table.end()&&table[(*it)[2]].find((*it)[0])!=table[(*it)[2]].end()){//从边的PT定位该边所在位置
                auto it_n=find(table[(*it)[2]][(*it)[0]]->Edges.begin(),table[(*it)[2]][(*it)[0]]->Edges.end(),e);
                if(it_n!=table[(*it)[2]][(*it)[0]]->Edges.end())
                    table[(*it)[2]][(*it)[0]]->Edges.erase(it_n);
            }
        }
    }
}

void LElists::getEdge(LEnode*node,TimedBipartiteGraph &G){//从当前节点开始向下遍历,将途中所有边插入图中
    for(auto it=node->Edges.begin();it!=node->Edges.end();it++)
        G.insertEdge(*it);
    for(auto it=node->children.begin();it!=node->children.end();it++)
        getEdge(*it,G);
}

void LElists::getGraph(int length,vector<TimedBipartiteGraph>& G_set){
    auto it=table.find(length);
    if(it!=table.end()){
        for(auto it_n=it->second.begin();it_n!=it->second.end();it_n++){
            TimedBipartiteGraph G;
            getEdge(it_n->second,G);
            G_set.push_back(G);
        }
    }
}