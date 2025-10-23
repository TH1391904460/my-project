#include"Edge.h"

Edge::Edge(){}

Edge::Edge(int u,int v):u(u),v(v){}

vector<vector<int>> Edge::getEdgePTS(int length){//获取节点的周期时间支撑集
    vector<int> times;
    vector<vector<int>> PTS;
    for(auto it=edgeTimes.begin();it!=edgeTimes.end();it++)
        times.push_back(*it);
    sort(times.begin(),times.end());
    for(int i=times.size()-1;i>=1;i--){
        for(int j=i-1;j>=0;j--){
            int gap=times[i]-times[j],count=2;
            while(find(times.begin(),times.end(),times[i]-gap*count)!=times.end())
                count++;
            if(count>=length){
                bool flag=true;
                for(auto it=PTS.begin();it!=PTS.end();it++){
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