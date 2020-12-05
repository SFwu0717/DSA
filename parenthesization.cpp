#include <iostream>
#include <iomanip>
#include <array>
using namespace std;


//turn in(string) to two arrs => coef and oprt
void tran_to_arr(int coef[], int opr[], int len, string in){
    int i=0, j=0;
    for(int k=0; k<len; k++){
        if( isdigit(in[k]) ){
            coef[i++] = in[k] - '0';
        }else{
            opr[j++] = in[k];
        }
    }
}

int index(int x, int y, int len){
    return ( x * len + y );
}

//return the value of a[opr]b
int cal(int a, int b, char opr){
    switch (opr) {
        case '+':
            return a+b;
            break;
        case '-':
            return a-b;
            break;
        case '*':
            return a*b;
            break;
        default:
            cout<<"there's an invalid operator\n";
            exit(1);
    }
}

//dynamically fill table and b_table
void dp(int coef[], int opr[], int len, int table[], int b_table[]){
    int L, i, j, k, temp;
    for(L = 1; L < len; L++){
        for(i = 0; i< len-L; i++){
            j = i + L;
            for(k = i; k < j ;k++){
                
                temp = cal(table[index( i, k, len )],
                           table[index( k+1, j, len )],
                           opr[k]);
                
                if(temp > table[index( i, j, len )]){
                    table[index( i, j, len )] = temp;
                    //for(k=i; k<j; k++)
                    b_table[index( i, j, len )] = k;
                }
            }
        }
    }
}

//by b_table, find entities that are adjacent to asterisk, give a mark that we need to parenthesize them
void divide(int i, int j, int len, int *i_mark, int oprt[], int b_table[], int mark[]){
    if(i == j) return;
    if( (oprt[i-1]&&oprt[i-1]=='*') || (oprt[j]&&oprt[j]=='*') ){
        for(int k=i; k<j; k++){
            if(oprt[k]=='*'){
                goto NEXT;
            }
        }
        mark[*i_mark] = i; (*i_mark)++;
        mark[*i_mark] = j; (*i_mark)++;
    }
NEXT:
    int k = b_table[index(i, j, len)];
    
    divide(i, k, len, i_mark, oprt, b_table, mark);
    divide(k+1, j, len, i_mark, oprt, b_table, mark);
}

//parenthesize the expression
//by what kind, how many of parenthesis should be add at before or after the coefficient at [i]
void parenthesize(int coef[], int oprt[], int mark[], int result[], int i_mark, int len){
    typedef struct{
        //left parenthesis or not
        bool L;
        //how many parenthesis should be add at before or after the cofficient at [i]
        int number;
    }gua;
    //a arr of the self-defined struct with lenght equaling to coef[]
    gua *gua_arr = new gua[len];
    //initialize
    for(int i=0; i<len; i++){
        gua_arr[i].L = NULL;
        gua_arr[i].number = 0;
    }
    
    //turn the mark[] into a arr that record what kind and how many parenthesis should be add at [i]
    for(int i=0; i<i_mark; i++){
        //data in mark is paired
        if(i%2 == 0){
            gua_arr[mark[i]].L = true;
            gua_arr[mark[i]].number++;
        }else{
            gua_arr[mark[i]].L = false;
            gua_arr[mark[i]].number++;
        }
    }
    //print
    for(int i=0; i<len; i++){
        //need to be parenthesized
        if(gua_arr[i].number!=0){
            //left one
            //add before coef
            if(gua_arr[i].L==true){
                for(int j=0; j<gua_arr[i].number; j++) cout<<'(';
                cout<<coef[i]<<char(oprt[i]);
            }
            //right one
            //add after coef
            else{
                cout<<coef[i];
                for(int j=0; j<gua_arr[i].number; j++) cout<<')';
                cout<<char(oprt[i]);
            }
        }
        //need no parenthesis, print in plain
        else{
            cout<<coef[i];
            if(oprt[i]) cout<<char(oprt[i]);
        }
    }
}

int main() {
    string in;
    cout << "Please enter the expression:\n";
    cin >> in;
    
    int len = int( in.length() / 2 + 1 );
    int *coef, *oprt;
    coef = new int[len+1];
    oprt = new int[len];

    //turns in(string) into two arrs => coef and oprt
    tran_to_arr(coef, oprt, len*2+1, in);
    
    int *table = new int[len * len];
    //tracing the breakpoint of the optimal sub-expression
    int *b_table = new int[len * len];
    
    //initialize tables
    for(int i=0; i<len; i++){
        for(int j=0; j<len; j++){
            
            //table[index(i, j, len)] = INT_MIN;
            table[index(i, j, len)] = INT_MIN;
            b_table[index(i, j, len)] = 0;
            
            //initialize diagonal line of table
            if( i==j ){
                table[index(i, j, len)] = coef[i];
            }
        }
    }
    //dynamically fill table and b_table
    dp(coef, oprt, len, table, b_table);
    
    int *mark = new int[len*2];
    int *i_mark; *i_mark = 0;
    divide(0, len-1, len, i_mark, oprt, b_table, mark);
    
    int *result = new int[len*3];
    parenthesize(coef, oprt, mark, result, *i_mark, len);
    cout<<" = "<<table[index(0, len-1, len)]<<endl;
    
  
    return 0;
}
