#include<iostream>
#include<string>
#include<vector>

#include <chrono>
#include <functional>

#include"TimedBipartiteGraph.h"
using namespace std;

template<typename Func, typename... Args>
double measure_execution_time(Func&& func, int iterations, Args&&... args) {
    using namespace std::chrono;
    
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func(std::forward<Args>(args)...); // 调用目标函数（完美转发参数）
    }
    auto end = high_resolution_clock::now();
    
    duration<double> total_time = end - start;
    return total_time.count() / iterations; // 返回平均时间（秒）
}

void test1(int alpha,int beta,int theta){
    string filename="out.txt";
    TimedBipartiteGraph* G=new TimedBipartiteGraph(filename);
    G->BCombined(alpha,beta,theta);
}

void test2(int alpha,int beta,int theta){
    string filename="out.txt";
    TimedBipartiteGraph* G=new TimedBipartiteGraph(filename);
    G->test();
}

int main(){
    // string filename="out.txt";
    // TimedBipartiteGraph* G=new TimedBipartiteGraph(filename);
    // G->test();

    const int iterations = 1;
    // 测量函数1的时间
    // double time1 = measure_execution_time(test1, iterations, 2,2,2);
    
    // 测量函数2的时间
    double time2 = measure_execution_time(test2, iterations, 2,2,2);

    // 输出结果
    // std::cout << "函数 BCombined 平均执行时间（" << iterations << "次）: " 
    //           << time1 * 1e6 << " 微秒\n";  // 转换为微秒更直观
    std::cout << "函数 EEnumeration 平均执行时间（" << iterations << "次）: " 
              << time2  << " 秒\n";

    // if (time2 > 0) {
    //     std::cout << "效率对比：BCombined 比 EEnumeration 慢 " 
    //               << time1 / time2 << " 倍\n";
    // }
    return 0;
}