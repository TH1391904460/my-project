#include"TimedBipartiteGraph.h"

TimedBipartiteGraph::TimedBipartiteGraph(){}

TimedBipartiteGraph::TimedBipartiteGraph(unordered_map<int,Node> U,unordered_map<int,Node> V):U(U),V(V){}//构造二分团

TimedBipartiteGraph::TimedBipartiteGraph(const string& filename){//读文件
    ifstream ifs(filename);
    if(!ifs.is_open()){
        cout<<"错误:无法打开文件"<<endl;
        return;
    }
    string line;
    // int count=0;
    while(getline(ifs,line)){
        // count++;
        if(line.empty()||line[0]=='%')//跳过空行和注释
            continue;
        istringstream iss(line);//将字符串包装为输入流对象
        int u,v,l,t;
        if(iss>>u>>v>>l>>t){
            auto it_u=U.find(u);
            if(it_u==U.end())
                U.emplace(u, Node(u, 'u'));
            U[u].addTime(t);
            U[u].addN_node(v);
            auto it_v=V.find(v);
            if(it_v==V.end())
                V.emplace(v, Node(v, 'v'));
            V[v].addTime(t);
            V[v].addN_node(u);
            if(E.find(u)==E.end()||E[u].find(v)==E[u].end())
                E[u][v]=Edge(u,v);
            E[u][v].edgeTimes.emplace(t);
        }else{
            cout<<"错误:忽略格式错误的行"<<endl;
        }
    }
    ifs.close();
}

void TimedBipartiteGraph::insertEdge(Edge *e){//不涉及时间戳
    if(U.find(e->u)==U.end())
        U.emplace(e->u,Node(e->u,'u'));
    if(V.find(e->v)==V.end())
        V.emplace(e->v,Node(e->v,'v'));
    U[e->u].addN_node(e->v);
    V[e->v].addN_node(e->u);
    if(E.find(e->u)==E.end()||E[e->u].find(e->v)==E[e->u].end())
        E[e->u][e->v]=Edge(e->u,e->v);
}

void TimedBipartiteGraph::deleteNode(Node &n){//删除节点,改变邻居节点的时间戳和邻居集
    unordered_set<int> w=n.N_node;
    for(auto it=w.begin();it!=w.end();it++){//遍历n的邻居
        if(n.type=='u'){
            for(auto it_e=E[n.ID][*it].edgeTimes.begin();it_e!=E[n.ID][*it].edgeTimes.end();it_e++)//删时间戳
                V[*it].deleteTime(*it_e);
            V[*it].deleteN_node(n.ID);//删邻居集
            E[n.ID].erase(*it);//删边
        }else{
            for(auto it_e=E[*it][n.ID].edgeTimes.begin();it_e!=E[*it][n.ID].edgeTimes.end();it_e++)
                U[*it].deleteTime(*it_e);
            U[*it].deleteN_node(n.ID);
            E[*it].erase(n.ID);//删边
        }
    }
    if (n.type == 'u') {//删节点,若为u需多删一个外层map索引,避免为空
        E.erase(n.ID); 
        U.erase(n.ID);
    }else{
        V.erase(n.ID);
    }
}

void TimedBipartiteGraph::deleteEdge(Edge &e){//删边,改变两端点的邻居集和时间戳
    U[e.u].deleteN_node(e.v);
    if(U[e.u].N_node.empty()){
        U.erase(e.u);
    }else{
        for(auto it=e.edgeTimes.begin();it!=e.edgeTimes.end();it++)
            U[e.u].deleteTime(*it);
    }
    V[e.v].deleteN_node(e.u);
    if(V[e.v].N_node.empty()){
        V.erase(e.v);
    }else{
        for(auto it=e.edgeTimes.begin();it!=e.edgeTimes.end();it++)
            V[e.v].deleteTime(*it);
    }
    E[e.u].erase(e.v);
}

void TimedBipartiteGraph::BReduction(int alpha,int beta,int theta){//获得θ-周期(β,α)-core 子图
    queue<Node*> Q;
    int k=0;
    for(auto it=U.begin();it!=U.end();it++){
        if(it->second.getNodePTS(theta).empty()||it->second.N_node.size()<beta){
            Q.push(&it->second);
            it->second.isExit=false;
        }
    }
    for(auto it=V.begin();it!=V.end();it++){
        if(it->second.getNodePTS(theta).empty()||it->second.N_node.size()<alpha){
            Q.push(&it->second);
            it->second.isExit=false;
        }
    }
    while(!Q.empty()){
        Node *n=Q.front();
        Q.pop();
        unordered_set<int> w=n->N_node;
        if(n->type=='u'){
            k=alpha;
        }else{
            k=beta;
        }
        deleteNode(*n);
        for(auto it=w.begin();it!=w.end();it++){//处理邻居节点,度或PTS不满足时加入删除队列
            if(k==alpha&&(V[*it].getNodePTS(theta).empty()||V[*it].N_node.size()<k)&&V[*it].isExit){
                Q.push(&V[*it]);
                V[*it].isExit=false;
            }else if(k==beta&&(U[*it].getNodePTS(theta).empty()||U[*it].N_node.size()<k)&&U[*it].isExit){
                Q.push(&U[*it]);
                U[*it].isExit=false;
            }        
        }
    }
}

void TimedBipartiteGraph::BReduction(int alpha,int beta){//获得(β,α)-core 子图(不检查节点的PTS)
    queue<Node*> Q;
    int k=0;
    for(auto it=U.begin();it!=U.end();it++){
        if(it->second.N_node.size()<beta){
            Q.push(&it->second);
            it->second.isExit=false;
        }
    }
    for(auto it=V.begin();it!=V.end();it++){
        if(it->second.N_node.size()<alpha){
            Q.push(&it->second);
            it->second.isExit=false;
        }
    }
    while(!Q.empty()){
        Node *n=Q.front();
        Q.pop();
        unordered_set<int> w=n->N_node;
        char type=n->type;
        if(n->type=='u'){
            k=alpha;
        }else{
            k=beta;
        }
        deleteNode(*n);
        for(auto it=w.begin();it!=w.end();it++){//处理邻居节点,度不满足时加入删除队列
            if(type=='u'&&V[*it].N_node.size()<k&&V[*it].isExit){
                Q.push(&V[*it]);
                V[*it].isExit=false;
            }else if(type=='v'&&U[*it].N_node.size()<k&&U[*it].isExit){
                Q.push(&U[*it]);
                U[*it].isExit=false;
            }        
        }
    }
}

unordered_set<int> TimedBipartiteGraph::getNNset(Node &n){//获得某个节点的二跳邻居
    unordered_set<int> NN;
    if(n.type=='u'){
        for(auto it=n.N_node.begin();it!=n.N_node.end();it++){
            for(auto it_n=V[*it].N_node.begin();it_n!=V[*it].N_node.end();it_n++)
                NN.emplace(*it_n);
        }
    }else{
        for(auto it=n.N_node.begin();it!=n.N_node.end();it++){
            for(auto it_n=U[*it].N_node.begin();it_n!=U[*it].N_node.end();it_n++)
                NN.emplace(*it_n);
        }
    }
    NN.erase(n.ID);
    return NN;
}

void TimedBipartiteGraph::backTrack(vector<int>& nums,vector<vector<int>>& result,vector<int>& current,int index,int beta){//回溯法找所有β长度的组合
    if(current.size()==beta){
        result.push_back(current);
        return;
    }
    for(int i=index;i<nums.size();i++){
        current.push_back(nums[i]);
        backTrack(nums,result,current,i+1,beta);
        current.pop_back();
    }
}

void TimedBipartiteGraph::PBlisting(vector<TimedBipartiteGraph>& C,int alpha,int beta,unordered_set<int> w,vector<unordered_set<int>> commonN,unordered_map<int,Node>& L){
    if(L.size()==alpha){
        unordered_map<int,Node> R;
        vector<int> nums(commonN[L.size()-1].begin(),commonN[L.size()-1].end());
        vector<vector<int>> result;
        vector<int> current;
        backTrack(nums,result,current,0,beta);
        for(auto it=result.begin();it!=result.end();it++){
            for(auto it_n=it->begin();it_n!=it->end();it_n++){
                R.emplace(*it_n,V[*it_n]);
            }
            C.emplace_back(L,R);//将L,R形成的二分团放入集合中
            R.clear();
        }
        return;
    }
    for(auto it=w.begin();it!=w.end();){
        commonN[L.size()].clear();
        for(auto it_n=commonN[L.size()-1].begin();it_n!=commonN[L.size()-1].end();it_n++){//找并集
            if(U[*it].N_node.find(*it_n)!=U[*it].N_node.end())
                commonN[L.size()].emplace(*it_n);
        }
        int temp=*it;
        it=w.erase(it);
        if(commonN[L.size()].size()<beta||w.size()<(alpha-L.size()-1)){
            continue;
        }else{
            L.emplace(temp,U[temp]);
            PBlisting(C,alpha,beta,w,commonN,L);
            L.erase(temp);
        }

    }
}

void TimedBipartiteGraph::BEnumeration(int alpha,int beta,vector<TimedBipartiteGraph>& C){//从周期子图中获得(α,β)-二分团
    unordered_map<int,unordered_set<int>> NN;
    vector<unordered_set<int>> commonN;
    unordered_map<int,Node> L;
    commonN.resize(alpha);
    for(auto it=U.begin();it!=U.end();it++)
        NN.emplace(it->first,getNNset(it->second));
    for(auto it=U.begin();it!=U.end();it++){
        if(NN[it->first].size()>=alpha-1){
            commonN[0]=it->second.N_node;
            L[it->first]=it->second;
            PBlisting(C,alpha,beta,NN[it->first],commonN,L);
            L.clear();
        }
        for(auto it_n=NN.begin();it_n!=NN.end();it_n++)//更新NN
            it_n->second.erase(it->first);   
    }
    return;
}

bool TimedBipartiteGraph::isExist(vector<int>& time,unordered_set <int>& e){
    for(auto it=time.begin();it!=time.end();it++){
        if(e.find(*it)==e.end())
            return false;
    }
    return true;
}

vector<TimedBipartiteGraph> TimedBipartiteGraph::getTimesGraph(int theta){//从θ-周期(β,α)-core图中枚举周期子图
    unordered_set<int> temp;
    for(auto it=U.begin();it!=U.end();it++){
        for(auto it_u=it->second.nodeTimes.begin();it_u!=it->second.nodeTimes.end();it_u++){
            temp.emplace(it_u->first);
        }
    }
    vector<int> v(temp.begin(),temp.end());
    sort(v.begin(),v.end());
    vector<int> timeSet;
    vector<vector<int>> set;
    for(int i=v.size()-1;i>=1;i--){//枚举所有周期
        for(int j=i-1;j>=0;j--){
            int gap=v[i]-v[j],count=2;
            timeSet.push_back(v[i]);
            timeSet.push_back(v[j]);
            if(theta!=2){
                while(find(v.begin(),v.end(),v[i]-count*gap)!=v.end()){
                    timeSet.push_back(v[i]-count*gap);
                    count++;
                    if(count==theta){
                        set.push_back(timeSet);
                        break;
                    }
                }
            }else{
                set.push_back(timeSet);
            }
            timeSet.clear();
        }
    }           
    vector<TimedBipartiteGraph> G_set;
    for(auto it=set.begin();it!=set.end();it++){
        TimedBipartiteGraph G=*this;
        for(auto it_u=E.begin();it_u!=E.end();it_u++){//检查每一条边是否有该周期
            for(auto it_v=it_u->second.begin();it_v!=it_u->second.end();it_v++){
                if(!isExist(*it,it_v->second.edgeTimes)){
                    G.deleteEdge(it_v->second);
                }   
            }
        }
        if(!G.U.empty())
            G_set.push_back(G);
    }
    return G_set;
}

vector<TimedBipartiteGraph> TimedBipartiteGraph::BCombined(int alpha,int beta,int theta){
    BReduction(alpha,beta,theta);
    vector<TimedBipartiteGraph> G_set=getTimesGraph(theta);
    vector<TimedBipartiteGraph> result;
    for(auto it=G_set.begin();it!=G_set.end();it++)
        it->BEnumeration(alpha,beta,result);
    return result;
}

void TimedBipartiteGraph::initLE(int theta){//为所有可能的间隔创建LE-lists
    unordered_set<int> gaps;
    for(auto it=E.begin();it!=E.end();it++){//检查所有边的所有间隔
        for(auto it_n=it->second.begin();it_n!=it->second.end();it_n++){
            vector<int> times;
            copy(it_n->second.edgeTimes.begin(),it_n->second.edgeTimes.end(),back_inserter(times));
            sort(times.begin(),times.end());
            bool flag=trie->findPTS(times,it_n->second.PTS);
            if(!flag){
                trie->addPTS(times,it_n->second.getEdgePTS(theta));
            }
            for(auto it_m=it_n->second.PTS.begin();it_m!=it_n->second.PTS.end();it_m++){
                gaps.emplace((*it_m)[1]);
            }
        }
    }
    for(auto it=gaps.begin();it!=gaps.end();it++){
        LElists*L=new LElists(*it);
        LE[*it]=L;
    }
}

vector<TimedBipartiteGraph> TimedBipartiteGraph::EEnumeration(int alpha,int beta,int theta){
    BReduction(alpha,beta);
    queue<Edge*> Q;
    initLE(theta);
    for(auto it=E.begin();it!=E.end();it++){
        for(auto it_n=it->second.begin();it_n!=it->second.end();it_n++){
            if(it_n->second.PTS.empty()){
                Q.emplace(&(it_n->second));
                it_n->second.isExit=false; 
            }else{
                for(auto it_m=it_n->second.PTS.begin();it_m!=it_n->second.PTS.end();it_m++)
                    LE[(*it_m)[1]]->insertEdge(&(it_n->second));//根据该边的所有PT,将其插入对应LElist中
            }
        }
    }
    while(!Q.empty()){
        Edge*e=Q.front();
        Q.pop();
        int u=e->u,v=e->v;
        for(auto it=e->PTS.begin();it!=e->PTS.end();it++)//分别在LElists上和图上删边
            LE[(*it)[1]]->deleteEdge(e);
        deleteEdge(*e);
        if(U.find(u)!=U.end()){
            if(U[u].N_node.size()<beta){//若u的度数不满足,将其所有边加入待删队列Q中
                for(auto it=U[u].N_node.begin();it!=U[u].N_node.end();it++){
                    if(E[u][*it].isExit){//若该边没有在Q队列中,将其加进去
                        Q.emplace(&E[u][*it]);
                        E[u][*it].isExit=false;
                    }
                }
            }
        }
        if(V.find(v)!=V.end()){
            if(V[v].N_node.size()<alpha){//若v的度数不满足,将其所有边加入待删队列Q中
                for(auto it=V[v].N_node.begin();it!=V[v].N_node.end();it++){
                    if(E[*it][v].isExit){//若该边没有在Q队列中,将其加进去
                        Q.emplace(&E[*it][v]);
                        E[*it][v].isExit=false;
                    }
                }
            }
        }
    }
    vector<TimedBipartiteGraph> result;
    for(auto it=LE.begin();it!=LE.end();it++){
        vector<TimedBipartiteGraph> G_set;
        it->second->getGraph(theta,G_set);
        for(auto it_n=G_set.begin();it_n!=G_set.end();it_n++)
            it_n->BEnumeration(alpha,beta,result);
    }
    return result;
}

void TimedBipartiteGraph::test(){
    // vector<TimedBipartiteGraph> result=BCombined(2,3,3);
    vector<TimedBipartiteGraph> result=EEnumeration(3,3,2);
    for(auto it=result.begin();it!=result.end();it++){
        for(auto it_u=it->U.begin();it_u!=it->U.end();it_u++){
            cout<<it_u->first<<"    ";
        }
        cout<<endl;
        for(auto it_v=it->V.begin();it_v!=it->V.end();it_v++){
            cout<<it_v->first<<"    ";
        }
        cout<<endl;
        cout<<endl;
    }
}
