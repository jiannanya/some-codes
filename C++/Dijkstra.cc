#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>

constexpr int node_num = 6;
constexpr int infinity = std::numeric_limits<int>::max();

// using Gragh = struct{
//     int node_num;
//     int** node_matrix;
// };

constexpr int matrix_graph[node_num][node_num] = {
    0,          10,         infinity,   4,          infinity,   infinity,
    10,         0,          8,          2,          6,          infinity,
    infinity,   8,          10,         15,         1,          5,
    4,          2,          15,         0,          6,          infinity,
    infinity,   6,          1,          6,          0,          12,
    infinity,   infinity,   5,          infinity,   12,         0

};

int distance[node_num] = {infinity - 1,infinity - 1,infinity - 1,infinity - 1,infinity - 1,infinity - 1};

void Dijkstra(const int &start_node,const int (&matrix_graph)[node_num][node_num] ){

    distance[start_node] = 0;
    
    bool visited[node_num] = {false,false,false,false,false,false};



    for(int i=0;i<node_num;i++){

        int mini_index = infinity;
        int mini_value = infinity;

        for(int k=0;k<node_num;k++){
            if(!visited[k] && distance[k] < mini_value){
                    mini_index = k;
                    mini_value = distance[k];    
                    //std::cout<<"test"<<mini_value<<std::endl;
            }
        }
        //std::cout<< i<<" mini_index "<<mini_index<<std::endl; 
        //if(mini_value == infinity)break;

        
        for(int k=0;k<node_num;k++){
            // 需要处理infinity加法导致的栈溢出变成负数
            distance[k] = std::min(distance[k], matrix_graph[mini_index][k]==infinity?infinity:distance[mini_index]+matrix_graph[mini_index][k]);
            //std::cout<<i<<" "<<distance[k]<<std::endl;
        }

         visited[mini_index] = true;

    }



    
}

int main(int argc, char**argv){
    int start_node = 0;
    std::cout<<"请输入起始节点: ";
    std::cin>>start_node;
    Dijkstra(start_node,matrix_graph);
    for(int i=0;i<node_num;i++)std::cout<<distance[i]<<" ";
    std::cout<<std::endl;

    return 0;
}
