#include<iostream>

using namespace std;

#define MAX_STACK_SIZE 100  /* 最大的Stack大小 */
#define MAX_MAZE_SIZE 22    /* 最大的迷宮大小(包含邊界) */

int maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE] = {0}, mark[MAX_MAZE_SIZE][MAX_MAZE_SIZE] = {0};     /* 二維陣列maze、和負責記錄有沒有走過的mark，全部初始值為 0 */
int EXIT_ROW, EXIT_COL;     /* 終點 */
int PASS_ROW, PASS_COL;     /* 指定點 */
bool PASS = false;          /* 判斷有沒有通過指定點用的布林值 */

typedef struct {
	short int vert;
	short int horiz;
} offsets;
offsets moves[8];           /* array of moves for each direction */

typedef struct {
	short int row;
	short int col;
	short int dir;
} element;
element Stack[MAX_STACK_SIZE];  /* 創造一個stack，用來儲存路徑 */
int top = -1;

bool IsEmpty(){
    /* If stack is empty then return true, else return false. */
    if( top==-1 )
        return true;
    else
        return false;
};
bool IsFull(){
    /* If stack is full then return true, else return false. */
    if( top>=MAX_STACK_SIZE-1 )
        return true;
    else
        return false;
};
void push(element item){
    /* push an item to the global stack */
    if( IsFull() ){
        cout<<"Can't push because the stack is full."<<endl;
        return;
    }
    Stack[++top] = item;
};
element pop(){
    /* pop an item from the global stack */
    if( IsEmpty() )
        cout<<"Can't pop because the stack is empty."<<endl;
    else
        return Stack[top--];
};
void path(){
    /* 開始找路徑 */
    int i, row = 1, col = 1, next_row = 0, next_col = 0, dir, found = false;
    element position;
    mark[1][1] = 1;  top = 0;
    Stack[0].row = 1;   Stack[0].col = 1;  Stack[0].dir = 1;    /* stack一開始放起點 */
    while( top>-1 && !found ){
        position = pop();
        row = position.row;  col = position.col;  dir = position.dir;

        while( dir<8 && !found ){
            /* 有八個方向，用迴圈把每個方向都掃一遍 */
            next_row = row + moves[dir].vert;
            next_col = col + moves[dir].horiz;
            if( next_row == EXIT_ROW && next_col == EXIT_COL ){  
                if( PASS == true )
                    found = true;
                else
                    dir = 8;       
            }
            else if( !maze[next_row][next_col] && !mark[next_row][next_col] ){      

                mark[next_row][next_col] = 1;                                        

                position.row = row;  position.col = col;  position.dir = ++dir;
                push( position );                                                   

                if( position.row == PASS_ROW && position.col == PASS_COL )          
                    PASS = true;

                row = next_row;  col = next_col;  dir = 0;                          
            }
            else
                ++dir;       
        }
        if( row == PASS_ROW && col == PASS_COL )    
            PASS = false;
    }
    if( found ){
        /* 輸出路徑，照投影片打 */
        for( int i = 0 ; i <= top ; i++ )
            cout << Stack[i].row << " " << Stack[i].col << endl;
        cout << row << " " << col << endl;
        cout << EXIT_ROW << " " << EXIT_COL;
    }
    else 
        cout << "The maze does not have a path";
};
int main(){

 
    int m = 0, p = 0;
    cin >> m >> p >> PASS_ROW >> PASS_COL;

	if( PASS_ROW == 0 && PASS_COL == 0 )     
		PASS = true;

/* 矩陣輸入放到二維陣列maze中，從maze的[1][1]為基準點開始放 */
    for( int i = 1 ; i <= m ; i++ )
        for( int j = 1 ; j <= p ; j++ )
            cin >> maze[i][j];

/* maze周圍要補 1 */
    for( int i = 0 ; i <= m+1 ; i++ ){
        for( int j = 0 ; j <= p+1 ; j++ ){
            if( i == 0 || i == m+1 )        /* 第一個row、最後一個row */
                maze[i][j] = 1;
            if( j == 0 || j == p+1 )        /* 第一個col、最後一個col */
                maze[i][j] = 1;
        }
    }

/* 初始化終點 */
    EXIT_ROW = m;  EXIT_COL = p;

/* 初始化八個方向的位移值 */
    moves[0].vert = -1;  moves[0].horiz =  0;    /*   上 */
    moves[1].vert = -1;  moves[1].horiz =  1;    /* 右上 */
    moves[2].vert =  0;  moves[2].horiz =  1;    /*   右 */
    moves[3].vert =  1;  moves[3].horiz =  1;    /* 右下 */
    moves[4].vert =  1;  moves[4].horiz =  0;    /*   下 */
    moves[5].vert =  1;  moves[5].horiz = -1;    /* 左下 */
    moves[6].vert =  0;  moves[6].horiz = -1;    /*   左 */
    moves[7].vert = -1;  moves[7].horiz = -1;    /* 左上 */

/* 執行找路徑的function */
    path();

    return 0;
}
