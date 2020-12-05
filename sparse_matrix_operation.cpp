#include<iostream>
#include<string>

using namespace std;

int matrixA[100][100], matrixB[100][100], matrixC[100][100];  /* 宣告三個100*100的矩陣A、B、C，用來存輸入 */
bool no_result = false;

#define MAX_TERMS 10001  /* maximum number of term+1 */
typedef struct {       // 定義稀疏矩陣儲存元素的結構
    int row;
    int col;
    int value;
} term;
term a[MAX_TERMS];	// 稀疏矩陣 A
term b[MAX_TERMS];  // 稀疏矩陣 B
term c[MAX_TERMS];  // 稀疏矩陣 C
term d[MAX_TERMS];  // 存運算用
term e[MAX_TERMS];  // 存運算用

/*******************/
/** 建造稀疏矩陣  **/
/*******************/
void CreateA(int max_row, int max_col){
    a[0].row = max_row+1;  a[0].col = max_col+1;  a[0].value = 0;    // 初始化稀疏矩陣狀態
    int index=1;    // a[0]記錄稀疏矩陣的狀態，因此從a[1]開始放
    for( int i=0 ; i<a[0].row ; i++ ){
        for( int j=0 ; j<a[0].col ; j++ ){
            if( matrixA[i][j]!=0 ){     // 非零元素才放入稀疏矩陣
                a[index].row = i;
                a[index].col = j;
                a[index].value = matrixA[i][j];
                a[0].value++;           // 每放入一個元素就+1
                index++;                // 每放入一個元素就+1
            }
        }
    }
};
void CreateB(int max_row, int max_col){
    b[0].row = max_row+1;  b[0].col = max_col+1;  b[0].value = 0;
    int index=1;
    for( int i=0 ; i<b[0].row ; i++ ){
        for( int j=0 ; j<b[0].col ; j++ ){
            if( matrixB[i][j]!=0 ){
                b[index].row = i;
                b[index].col = j;
                b[index].value = matrixB[i][j];
                b[0].value++;
                index++;
            }
        }
    }
};
void CreateC(int max_row, int max_col){
    c[0].row = max_row+1;  c[0].col = max_col+1;  c[0].value = 0;
    int index=1;
    for( int i=0 ; i<a[0].row ; i++ ){
        for( int j=0 ; j<a[0].col ; j++ ){
            if( matrixC[i][j]!=0 ){
                c[index].row = i;
                c[index].col = j;
                c[index].value = matrixC[i][j];
                c[0].value++;
                index++;
            }
        }
    }
};
/*******************/
/** 轉置稀疏矩陣  **/
/*******************/
void transpose( term a[], term b[] ){
/** b is set to the transpose of a **/
    int n, i, j, currentb;
    n = a[0].value;         // total number of elements
    b[0].row = a[0].col;    // rows in b = columns in a
    b[0].col = a[0].row;    // columns in b = rows in a
    b[0].value = n;
    if( n > 0 ){            // non zero matrix
        currentb = 1;
        for( i = 0 ; i < a[0].col ; i++ ){
        // transpose by the columns in a
            for( j = 1 ; j <= n ; j++ ){
            // find elements from the current column
                if( a[j].col == i ){
                // element is in current column, add it to b
                    b[currentb].row = a[j].col;
                    b[currentb].col = a[j].row;
                    b[currentb].value = a[j].value;
                    currentb++;
                }
            }
        }
    }
};
/**********************/
/** 快速轉置稀疏矩陣 **/
/**********************/
void fast_transpose( term a[], term b[] ){
    int row_terms[100], starting_pos[100];
    int i, j, num_cols = a[0].col, num_terms = a[0].value;
    b[0].row = num_cols;
    b[0].col = a[0].row;
    b[0].value = num_terms;
    if( num_terms > 0 ){ /* nonzero matrix */
        for( i = 0 ; i < num_cols ; i++ )
            row_terms[i] = 0;
        for( i = 1 ; i <= num_terms ; i++ )
            row_terms[a[i].col]++;
        starting_pos[0] = 1;
        for( i = 1 ; i < num_cols ; i++ )
            starting_pos[i] = starting_pos[i-1] + row_terms[i-1];
        for( i = 1 ; i <= num_terms ; i++ ){
            j = starting_pos[a[i].col]++;
            b[j].row = a[i].col;
            b[j].col = a[i].row;
            b[j].value = a[i].value;
        }
    }
};
/************************************/
/** 儲存運算結果到指定的稀疏矩陣   **/
/************************************/
void storesum( term d[], int *totald, int row, int column, int *sum ){
/** if *sum !=0, then it along with its row and column position is stored as the *totald+1 entry in d **/
    if( *sum ){
        d[++*totald].row = row;
        d[*totald].col = column;
        d[*totald].value = *sum;
        *sum = 0;
    }
};
/*********************/
/** 比大小並回傳值  **/
/*********************/
int COMPARE(int x, int y){
    if( x < y )
        return -1;
    else if( x == y )
        return 0;
    else
        return 1;
};
/*******************/
/** 稀疏矩陣相加 **/
/*******************/
void madd( term a[], term b[], term d[]){
    int i = 1, j = 1, totald = 0, sum = 0;
    int rows_a = a[0].row, cols_a = a[0].col, totala = a[0].value;
    int rows_b = b[0].row, cols_b = b[0].col, totalb = b[0].value;

    if( rows_a!=rows_b || cols_a!=cols_b ){	// A、B的行列不相等的話不能運算
        cout<<"No result"<<endl;
        no_result = true;
        return;
    }

    while( (i<=totala) && (j<=totalb) ){	// 每個元素都要處理
        if( a[i].row == b[j].row ){     	// row 相等
            if( a[i].col < b[j].col ){  	// a[i] 的 col 小於 b[j] 的 col，將 a[i] 加到 d 中
                sum = a[i].value;
                storesum( d, &totald, a[i].row, a[i].col, &sum);
                i++;						// 每放入一個 a 元素，已處理的數量++
            }
            else if( a[i].col > b[j].col ){ // a[i] 的 col 大於 b[j] 的 col，將 b[j] 加到 d 中
                sum = b[j].value;
                storesum( d, &totald, b[j].row, b[j].col, &sum);
                j++;						// 每放入一個 b 元素，已處理的數量++
            }
            else{   // a[i] 的 col 等於 b[j] 的 col，相加再加到 d 中
                sum = a[i].value + b[j].value;
                if( sum!=0 )   // 不等於0才加進去
                    storesum( d, &totald, a[i].row, a[i].col, &sum);
                i++;
                j++;
            }
        }
        else if( a[i].row < b[j].row ){ 	// a[i] 的 row 小於 b[j] 的 row，將 a[i] 加到 d 中
            sum = a[i].value;
            storesum( d, &totald, a[i].row, a[i].col, &sum);
            i++;
        }
        else{   // a[i] 的 row 大於 b[j] 的 row，將 b[j] 加到 d 中
            sum = b[j].value;
            storesum( d, &totald, b[j].row, b[j].col, &sum);
            j++;
        }
    }
    while( i<=totala ){		// 如果還有 a 的元素沒處理完，全部依序放進 d
        sum = a[i].value;
        storesum( d, &totald, a[i].row, a[i].col, &sum);
        i++;
    }
    while( j<=totalb ){		// 如果還有 b 的元素沒處理完，全部依序放進 d
        sum = b[j].value;
        storesum( d, &totald, b[j].row, b[j].col, &sum);
        j++;
    }

    d[0].row = rows_a;		// 記錄狀態
    d[0].col = cols_a;
    d[0].value = totald;
};
/*******************/
/** 稀疏矩陣相乘 **/
/*******************/
void mmult( term a[], term b[], term d[]){
/** multiply rwo sparse matrices **/
    int i, j, column, totalb = b[0].value, totald = 0;
    int rows_a = a[0].row, cols_a = a[0].col, totala = a[0].value;
    int cols_b = b[0].col;
    int row_begin = 1, row = a[1].row, sum = 0;

    if( cols_a != b[0].row ){		// a 的 row 不等於 b 的 col，不能運算
        cout<<"No result"<<endl;
        no_result = true;
        return;
    }
    term new_b[MAX_TERMS];
    fast_transpose(b, new_b);

    /* set boundary condition */
    a[totala+1].row = rows_a;
    new_b[totalb+1].row = cols_b;
    new_b[totalb+1].col = 0;
    new_b[totalb+1].value = 0;

    for( i = 1 ; i <= totala ; ){
        column = new_b[1].row;
        for( j = 1 ; j <= totalb+1 ; ){

        /* multiply row of a by column of b */
            if( a[i].row != row ){
                storesum( d, &totald, row, column, &sum);
                i = row_begin;
                for( ; new_b[j].row == column ; j++ )
                    ;
                column = new_b[j].row;
            }
            else if( new_b[j].row != column ){
                storesum( d, &totald, row, column, &sum);
                i = row_begin;
                column = new_b[j].row;
            }
            else{
                switch( COMPARE(a[i].col, new_b[j].col) ){
                    case -1:  /* go to next term in a */
                            i++; break;
                    case 0:   /* add terms, go to next term in a and b */
                            sum += ( a[i++].value * new_b[j++].value );
                            break;
                    case 1:   /* advance to next term in b */
                            j++;
                            break;
                }
            }
        }   /* end of for j<=totalb+1 */
        for( ; a[i].row == row ; i++ )
            ;
        row_begin = i;  row = a[i].row;
    }   /* end of for i<=totala */
    d[0].row = rows_a;
    d[0].col = cols_b;
    d[0].value = totald;
};
/*******************/
/** 輸出運算結果  **/
/*******************/
void print_result( term result[] ){
    if(!no_result){					// 如果有結果才輸出
        if( result[0].value == 0 )	// 如果矩陣元素全為零
            cout<<"All element are 0"<<endl;
        else{
            for( int i = 1 ; i <= result[0].value ; i++ )
                cout<<result[i].row<<" "<<result[i].col<<" "<<result[i].value<<endl;
        }
    }
};
/**************************************/
/** 讀取輸入到矩陣中，並創造稀疏矩陣 **/
/** 呼叫加法、乘法、輸出             **/
/**************************************/
int main(){

    /* 初始化 A、B、C 三個矩陣元素，全部為0 */
    for( int i=0 ; i<100 ; i++ ){
        for( int j=0 ; j<100 ; j++ ){
            matrixA[i][j] = 0;
            matrixB[i][j] = 0;
            matrixC[i][j] = 0;
        }
    }

	/** 讀取測資輸入並放進矩陣中 **/
    string input;                            // 存放一整行輸入
    int matrix_number = 1;                   // 記錄現在儲存到第幾個矩陣，首先為第一個
    int row_A = -1, row_B = -1, row_C = -1;  // 記錄矩陣有幾個row，一律初始為-1，有讀到東西才從0開始
    int col_A = -1, col_B = -1, col_C = -1;  // 記錄矩陣有幾個column

    while( getline(cin,input) ){    // 用 getline()一次抓一行，放在while判斷條件裡面，抓完測資後沒東西抓就會自動跳出迴圈了

        if( input=="|" ){           // 讀到"|"，表示要讀取下個矩陣了，矩陣編號+1，且因為這行不用處理矩陣元素，使用continue直接執行下一圈迴圈
            matrix_number++;
            continue;
        }
        switch(matrix_number){      // 每讀到新的一行，當前矩陣的row數就加1，col數歸零
        /*A*/case 1:
                 row_A++;  col_A = -1;
                 break;
        /*B*/case 2:
                 row_B++;  col_B = -1;
                 break;
        /*C*/case 3:
                 row_C++;  col_C = -1;
                 break;
        }
		
        for( int i=0 ; i<input.size() ; i++ ){              // 處理input字串，如果是數字就放進矩陣中
            if( input[i]!=' ' ){                            // 當前字元不是空格才處理
                switch(matrix_number){                      // 先判斷要放進第幾個矩陣
               /*A*/case 1:
                        col_A++;                            // 讀到元素，col數加一
                        if( input[i]=='-' ){                // 如果這個字元是負號，表示數字在下一個字元
                            i++;                            // 移到下一個字元
                            matrixA[row_A][col_A] = (int(input[i])-48) - 2*(int(input[i])-48);
                                                            // int(數字字元)會把他轉成ASCII碼，例如int('9')會轉成57，這時只要減去'0'的ASCII碼，也就是48，就可以將字元轉成數字了
                                                            // 正整數減去兩倍的自己就可以得到負整數
                        }
                        else
                            matrixA[row_A][col_A] = int(input[i])-48;  // 正整數
                        break;
               /*B*/case 2:
                        col_B++;
                        if( input[i]=='-' ){
                            i++;
                            matrixB[row_B][col_B] = (int(input[i])-48) - 2*(int(input[i])-48);
                        }
                        else
                            matrixB[row_B][col_B] = int(input[i])-48;
                        break;
               /*C*/case 3:
                        col_C++;
                        if( input[i]=='-' ){
                            i++;
                            matrixC[row_C][col_C] = (int(input[i])-48) - 2*(int(input[i])-48);
                        }
                        else
                            matrixC[row_C][col_C] = int(input[i])-48;
                        break;
                }
            }
        }
    }

	/** 輸入處理完，開始建立稀疏矩陣 **/
    CreateA( row_A , col_A );
    CreateB( row_B , col_B );
    if( matrix_number==3 )      // 有第三個矩陣才建立稀疏矩陣 C
        CreateC( row_C , col_C );

	/** 稀疏矩陣運算，加法與乘法，算好後輸出 **/
    switch(matrix_number){				// 先判斷有幾個矩陣要做運算
        case 2:							// 共有兩個矩陣
            madd( a , b , d );			// a + b 放到 d
            print_result( d );			// 輸出加法結果 d
            no_result = false;			// bool 重設
            cout<<"----------"<<endl;
            mmult( a , b , e );			// A * B 放到 e
            print_result( e );			// 輸出乘法結果 e
            break;
        case 3:							// 共有三個矩陣
            madd( a , b , d );			// a + b 放到 d
            madd( c , d , e );			// d + c 放到 e
            print_result( e );			// 輸出加法結果 e
            no_result = false;			// bool 重設
            cout<<"----------"<<endl;
            mmult( a , b , d );			// a * b 放到 d
            mmult( d , c , e );			// d * c 放到 e
            print_result( e );			// 輸出乘法結果 e
            no_result = false;
			break;
    }

    return 0;
}