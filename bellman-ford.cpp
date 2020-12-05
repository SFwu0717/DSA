#include <iostream>
#include <ctime>
#include <iomanip>
#include <queue>
#include <fstream>

using namespace std;

struct node{
    int no;
    int w;
    node *next;
};

node* new_node(int no, int w){
    node* temp = new node;
    temp->no = no;
    temp->w = w;
    temp->next = nullptr;
    return temp;
}


double bellman_ford(node *adj_list, int sour, int dest, int V, bool *flag_ptr, int parent[], int distance[], int *node_ptr){
    clock_t begin = clock();
    
    //initialize
    for(int i=0; i<V; i++){
        parent[i] = NULL;
        distance[i] = 1e8;
    }
    //起始點
    distance[sour] = 0;
    
    node *temp;
    for(int i=0; i<V-1; i++){
        for(int j=0; j<V; j++){
            temp = adj_list[j].next;
            while(temp){
                if(distance[temp->no] > distance[j] + temp->w){
                    distance[temp->no] = distance[j] + temp->w;
                    parent[temp->no] = j;
                }
                temp = temp->next;
            }
        }
    }
    
    clock_t end = clock();
    for(int j=0; j<V; j++){
        temp = adj_list[j].next;
        while(temp){
            if(distance[temp->no] > distance[j] + temp->w){
                *flag_ptr = true;
                if(! *node_ptr) *node_ptr = temp->no;
            }
            temp = temp->next;
        }
    }

    double dur = double(end - begin) / CLOCKS_PER_SEC;
    return dur;
}

double mod_bellman_ford(node *adj_list, int sour, int dest, int V, int parent[], int distance[]){
    
    
    queue<int> q;//使用queue當作整理順序的工具
    int exist[V];//紀錄下誰已經被丟進queue過了
    //initialize
    for(int i=0; i<V; i++){
        parent[i] = NULL;
        distance[i] = 1e8;
        exist[i] = false;
    }
    //起始點
    distance[sour] = 0;
    exist[sour] = true;
    q.push(sour);
    
    clock_t begin = clock();
    node *temp;
    while(!q.empty()){
        int head = q.front();
        temp = adj_list[head].next;//把該點可連接的邊relax一遍
        while(temp){
            if(distance[temp->no] > distance[head] + temp->w){
                distance[temp->no] = distance[head] + temp->w;
                parent[temp->no] = head;
                
                if(!exist[temp->no]) {//若接到的點還沒被當作head過誒，放入queue
                    q.push(temp->no);
                    exist[temp->no] = true;
                }
            }
            temp = temp->next;
        }
        q.pop();//pop掉，exist[pop.front()]不變，以免重複
    }
    
    clock_t end = clock();
    double dur = double(end - begin) / CLOCKS_PER_SEC;
    return dur;
}
    

int main(){
    int V, E, sour, dest;
    /*ifstream fin;
    fin.open("input.txt");
    fin >> V >> E >> sour >> dest;*/

    
    cin >> V >> E;
    cin >> sour >> dest;
    node adj_list[V];
    //initialize
    for(int i=0; i<V; i++){
        adj_list[i].no = 0;
        adj_list[i].w = 0;
        adj_list[i].next = nullptr;
    }
    
    //forming the adj_list
    int f, t, w;
    node *nnode;
    node *temp;
    for(int i=0; i<E; i++){
        cin >> f >> t >> w;
        //cin >> f >> t >> w;
        nnode = new_node(t, w);
        if(!adj_list[f].next){
            adj_list[f].next = nnode;
        }else{
            temp = adj_list[f].next;
            while(temp->next){
                temp = temp->next;
            }
            temp->next = nnode;
        }
    }
    
    for(int i=0; i<V; i++){
        temp = adj_list[i].next;
        cout << i <<": ";
        while(temp){
            cout << temp->no<<'('<<temp->w<<')'<<"->";
        }
        cout<<endl;
    }
    
    bool flag = false;   //detect if there's a negative weight cycle
    bool *flag_ptr = &flag;
    int nc_node = NULL;
    int *node_ptr = &nc_node;
    
    //do the original bellman-ford's alg
    int parent[V];
    int distance[V];
    double dur =
        bellman_ford(adj_list, sour, dest, V, flag_ptr, parent, distance, node_ptr);
    
    //do the modified bellman-ford's alg
    int mod_parent[V];
    int mod_distance[V];
    double mod_dur =
        mod_bellman_ford(adj_list, sour, dest, V, mod_parent, mod_distance);

    cout << "original time: " << setprecision(6) << dur * 1e6
            << " microseconds\n";
    cout << "modified time: " << setprecision(6) << mod_dur * 1e6
            << " microseconds\n";
    
    if(!flag){
        //cout the shortest path
        int path[V];
        int t_weight = 0;
        int cur = dest;
        int k = 0;
        path[k++] = cur;
        while(cur != sour){
            cur = parent[cur];
            path[k++] = cur;
        }
        
        for(int i=k-1; i>=0; i--){
            cout << path[i] << " ";
        }
        cout<<endl;
        
        //add up the weights
        int head, tail;
        for(int i=k-1; i>0; i--){
            head = path[i];
            tail = path[i-1];
            temp = adj_list[head].next;
            while(temp->no != tail){
                temp = temp->next;
            }
            t_weight += temp->w;
        }
        cout << t_weight <<endl;
        
    }else{
        //cout members of the negative weight cycle
        cout << "There is a negative weight cycle.\n";
        int cycle[V];
        bool m[V];
        for(int i=0; i<V; i++){
            m[V] = false;
        }
        int c = 0;
        while(!m[nc_node]){
            m[nc_node] = true;
            cycle[c++] = nc_node;
            nc_node = parent[nc_node];
        }
        for(int i=c-1; i>=0; i--){
            cout << cycle[i] <<' ';
        }
        cout << endl;
    }
    return 0;
}
