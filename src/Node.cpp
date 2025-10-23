#include"Node.h"

Node::Node(){}

Node::Node(int ID,char type):ID(ID),type(type){}
    
void Node::addTime(int time){
    auto it=nodeTimes.find(time);
    if(it==nodeTimes.end())
        nodeTimes[time]=1;
    else
        nodeTimes[time]++;
}

void Node::deleteTime(int time){
    if(--nodeTimes[time]==0)
        nodeTimes.erase(time);
}

void Node::addN_node(int n){
    N_node.insert(n);
}

void Node::deleteN_node(int n){
    N_node.erase(n);
}

vector<vector<int>> Node::getNodePTS(int length){//获取节点的周期时间支撑集
    vector<int> times;
    vector<vector<int>> PTS;
    for(auto it=nodeTimes.begin();it!=nodeTimes.end();it++)
        times.push_back(it->first);
    sort(times.begin(),times.end());
    for(int i=times.size()-1;i>=1;i--){
        for(int j=i-1;j>=0;j--){
            int gap=times[i]-times[j],count=2;
            while(find(times.begin(),times.end(),times[i]-gap*count)!=times.end())
                count++;
            if(count>=length){
                bool flag=true;
                for(auto it=PTS.begin();it!=PTS.end();it++){//检查是否有重复
                    if((*it)[0]==times[i]-gap*(count-1)&&(*it)[1]==gap)
                        flag=false;
                }
                if(flag)
                    PTS.push_back({times[i]-gap*(count-1),gap,count});
            }
        }
    }
    this->PTS=PTS;
    return PTS;
}
