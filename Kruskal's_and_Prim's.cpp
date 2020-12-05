#include<iostream>
#include<vector>
#include <algorithm>

using namespace std;

typedef struct Edge{
    int Start;
    int End;
    int Weight;
};

bool compare_weight( const Edge &a , const Edge &b ){
    return a.Weight < b.Weight;
}

class Undirected_Graph {

public:

int V, totalE;
vector<Edge> E;

void Create(){
    int v_start, v_end, edge_weight;
    cin >> V >> totalE;

    while( cin >> v_start ){
        cin >> v_end >> edge_weight;
        Insert_Edge( v_start , v_end , edge_weight );
    }
}

void Insert_Edge(int _start, int _end, int weight){
    Edge temp1, temp2;
    temp1.Start = _start;  temp1.End =   _end;  temp1.Weight = weight;
    temp2.Start =   _end;  temp2.End = _start;  temp2.Weight = weight;
    E.push_back( temp1 );   E.push_back( temp2 );
}

void Delete_Edge(int _start, int _end){
    for( int i = 0 ; i < E.size() ; i++ ){
        if( E[i].Start == _start && E[i].End == _end ){
            E.erase( E.begin() + i );
            break;
        }
    }
    for( int i = 0 ; i < E.size() ; i++ ){
        if( E[i].Start == _end && E[i].End == _start ){
            E.erase( E.begin() + i );
            break;
        }
    }
}

Edge Choose_least_edge( int index ){
    return E[index];
}

};

class Minimun_Spanning_Tree{

public:

int MST_V;
int **MST_adjmatrix;
vector<Edge> MST;
bool *visited;
bool cycle = false;

void Create( int V ){
    MST_V = V;
    MST_adjmatrix = new int*[MST_V];
    for( int i = 0 ; i < MST_V ; i++ )
        MST_adjmatrix[i] = new int[MST_V];

    for( int i = 0 ; i < MST_V ; i++ )
        for( int j = 0 ; j < MST_V ; j++ )
            MST_adjmatrix[i][j] = 0;
}

void Insert_Edge( int _start , int _end , int weight ){
    MST_adjmatrix[_start][_end] = weight;
    MST_adjmatrix[_end][_start] = weight;
    Edge temp;
    temp.Start = _start;  temp.End = _end;  temp.Weight = weight;
    MST.push_back(temp);
}

void Delete_Edge( int _start , int _end ){
    MST_adjmatrix[_start][_end] = 0;
    MST_adjmatrix[_end][_start] = 0;
    for( int i = 0 ; i < MST.size() ; i++ ){
        if( MST[i].Start == _start && MST[i].End == _end )
            MST.erase( MST.begin() + i );
    }
    for( int i = 0 ; i < MST.size() ; i++ ){
        if( MST[i].Start == _end && MST[i].End == _start )
            MST.erase( MST.begin() + i );
    }
}

bool DFS( int Last , int Start , int origin ){
    visited[Start] = 1;
    for( int i = 0 ; i < MST_V ; i++ ){
        if( MST_adjmatrix[Start][i] != 0 && i != Last ){
            if( i == origin )
                cycle = true;
            if( !visited[i] )
                DFS( Start , i , origin );
        }
    }
    return cycle;
}

bool Find_Cycle( int Start ){

    cycle = false;

    visited = new bool[MST_V];
    for( int i = 0 ; i < MST_V ; i++ )
        visited[i] = 0;

    int Last = -1, origin = Start;
    return DFS( Last , Start , origin );
}

int Total_Edge(){
    return MST.size();
}

void Kruskal( Undirected_Graph G ){
    Edge edge;
    bool valid = true;
    sort( G.E.begin() , G.E.end() , compare_weight );

    while( Total_Edge() < MST_V - 1 ){
        if( G.E.size() == 0 ){
            valid = false;
            break;
        }
        edge = G.Choose_least_edge(0);

        if( edge.Start > edge.End )
            Insert_Edge( edge.End , edge.Start , edge.Weight );
        else
            Insert_Edge( edge.Start , edge.End , edge.Weight );

        G.Delete_Edge( edge.Start , edge.End );

        if( Find_Cycle(edge.Start) == true )
            Delete_Edge( edge.Start , edge.End );
    }
    if( valid )
        Output();
    else
        cout << "No Spanning Tree.";
}

void Prim( Undirected_Graph G ){
    bool TV[MST_V] = {0};
    TV[0] = 1;
    bool valid = true;

    Edge edge;
    sort( G.E.begin() , G.E.end() , compare_weight );

    while( valid && Total_Edge() < MST_V - 1 ){

        int i = 0;
        do{
            if( i == G.E.size() ){
                valid = false;
                break;
            }
            edge = G.Choose_least_edge( i++ );
        }while( (i <= G.E.size()) && (TV[edge.Start] == 0) || (TV[edge.End] == 1) );

        if(valid){
            TV[edge.End] = 1;
            Insert_Edge( edge.Start , edge.End , edge.Weight );
        }
    }
    if(valid){
        Output();
        cout << endl;
    }
    else
        cout << "No Spanning Tree." << endl;
}

void Output(){
    for( int i = 0 ; i < MST.size() ; i++ ){
        cout << MST[i].Start << " " << MST[i].End << " " << MST[i].Weight << endl;
    }
    cout << Get_Cost() << endl;
}

int Get_Cost(){
    int cost = 0;
    for( int i = 0 ; i < MST.size() ; i++ )
        cost += MST[i].Weight;
    return cost;
}

};

int main(){

    Undirected_Graph G;
    G.Create();

    Minimun_Spanning_Tree T1;
    T1.Create( G.V );
    T1.Prim( G );

    Minimun_Spanning_Tree T2;
    T2.Create( G.V );
    T2.Kruskal( G );

    return 0;
}
