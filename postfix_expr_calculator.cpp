#include<iostream>
#include<string>

#define MAX_STACK_SIZE 100  /* maximum stack size */

using namespace std;

/** 宣告全域變數 **/
typedef enum {
    Lparen, Rparen, Plus, Minus, Times, Divide, Mod, Eos, Operand
} precedence;

int Stack[MAX_STACK_SIZE];  /* global stack */
string expr;                /* 存放測資輸入，function主要運行的字串 */
string postfix_expr;        /* 存放postfix表示式 */
int top;

int isp[8] = {0,19,12,12,13,13,13,0};           /* in-stack precedence */
int icp[8] = {20,19,12,12,13,13,13,0};          /* incoming precedence */

/** 所有會用到的function **/
bool check();                                   /* 檢查運算式是否合法 */
precedence get_token( char *symbol, int *n );   /* 從運算式抓token */
precedence get_remaining_token( int n );        /* 運算式抓完之後,stack內可能還有剩下的運算符號,一併輸出 */
void stack_full();
void stack_empty();
void push(int item);
int pop();
void print_token(int token_num);                /* 輸出後序表示式的token,這邊我的程式不拿來輸出,拿來接字串用,之後一併輸出,同學也可以在這邊先輸出 */
void postfix();                                 /* 中序表示式轉後序表示式 */
int eval();                                     /* 計算後序表示式的答案 */

/********************************************************************************************/
/** 使用 while( getline() )一行一行抓測資,每抓一行執行一輪以下動作：                    **/
/** (1) 檢查輸入的運算式是否合法,不合法就輸出"The expression is invalid." 並結束程式    **/
/** (2) 是合法運算式,呼叫postfix()轉成後序表示式                                         **/
/** (3) 呼叫eval()計算後序表示式的答案,並輸出題目要求的輸出格式                         **/
/********************************************************************************************/
int main(){

    while( getline(cin,expr) ) {
        if( check() == true ) {                 /* 呼叫check()檢查expr是否為有效運算式，是的話才會做轉換和運算 */
            int answer = 0;                     /* 存放答案,每算一行測資都要歸零 */

            postfix();                          /* 中序轉後序 (expr -> postfix_expr) */
            expr = "";                          /* 清空expr */
            expr = postfix_expr + ' ' ;         /* 在後序表示式最後面補eos，成為新的expr (做此步是因為eval()也是使用expr來運行) */
            answer = eval();                    /* 計算後序表示式的答案 */

            cout<< postfix_expr << " = " << answer << endl;  /* 輸出題目規定的格式 */
        }
        else  /* 非法運算式 */
            cout<< "The expression is invalid." << endl;
    }
    return 0;
};
/**************************************************************************************/
/** 檢查expr是否合法                                                                **/
/** "簡單檢查"先檢查運算式的字元是否合法,以及括號有沒有左右配對                  **/
/** "進階檢查"會進一步檢查字元前後的關係、格式是否合法,例如不能連續兩個運算符號 **/
/** 注意: 進階檢查只適用在個位數的情況下                                          **/
/*************************************************************************************/
bool check(){
    bool valid = true;
    int operand_num = 0, operator_num = 0, paren_num = 0;    /** 數字的數量、運算符號的數量、括號的數量 **/
    int tokenType[expr.size()] = {0};                        /** 用來存放expr每個字元的type: '('=0, ')'=1, 運算符號=2, 數字=3, Eos=4 **/

/** 簡單檢查 **/
    if( expr.size() < 4 || expr[expr.size()-1] != ' ' )      /** expr的長度小於4，或是最後面沒有Eos,非法 **/
        valid = false;
    /** 根據字元處理，存放字元的類型以及計算類型的數量，如果有非法字元就把valid設成false結束檢查 **/
    for( int i = 0 ; ( valid && i < expr.size() ) ; i++ ){
        switch( expr[i] ){
            case '(' : tokenType[i] = 0;  paren_num++;     break;
            case ')' : tokenType[i] = 1;  paren_num--;     break;
            case '+' : tokenType[i] = 2;  operator_num++;  break;
            case '-' : tokenType[i] = 2;  operator_num++;  break;
            case '/' : tokenType[i] = 2;  operator_num++;  break;
            case '*' : tokenType[i] = 2;  operator_num++;  break;
            case '%' : tokenType[i] = 2;  operator_num++;  break;
            case '0' : tokenType[i] = 3;  operand_num++;   break;
            case '1' : tokenType[i] = 3;  operand_num++;   break;
            case '2' : tokenType[i] = 3;  operand_num++;   break;
            case '3' : tokenType[i] = 3;  operand_num++;   break;
            case '4' : tokenType[i] = 3;  operand_num++;   break;
            case '5' : tokenType[i] = 3;  operand_num++;   break;
            case '6' : tokenType[i] = 3;  operand_num++;   break;
            case '7' : tokenType[i] = 3;  operand_num++;   break;
            case '8' : tokenType[i] = 3;  operand_num++;   break;
            case '9' : tokenType[i] = 3;  operand_num++;   break;
            case ' ' : if( i != expr.size()-1 )  valid = false;  tokenType[i] = 4;  break;   /** Eos不是在expr最後面，就是非法 **/
            default  : valid = false;                                                        /** 除了上述這些字元以外，也就是沒有進入其中一個case，就是非法字元 **/
        }
    }
    if( valid && ( paren_num!=0 || (operand_num - operator_num)!=1 ))   /** 括號沒有兩兩配對、或是運算元沒有剛好比運算子多一個，非法 **/
        valid = false;

    if( valid == false )    /** 如果簡單檢查就非法了，直接return非法 **/
        return false;

/** 進階檢查，檢查字元的位置與前後關係，使用tokenType來檢查 **/
/** '(' = 0 , ')' = 1 , 運算符號 = 2 , 數字 = 3               **/
    if( ( tokenType[0]==1 || tokenType[0]==2 ) )                            /** expr第一個字元不是左括號或數字,非法 **/
        return false;
    if( tokenType[expr.size()-2] == 0 || tokenType[expr.size()-2] == 2 )    /** Eos前一個字元不是右括號或數字,非法 **/
        return false;
    /** 從expr第二個字元開始檢查前後關係，檢查到Eos前一個字元為止，若發現非法就終止迴圈 **/
    for( int i = 1 ; ( valid && i < expr.size()-1 ) ; i++ ){
        switch( tokenType[i] ){
            case 0: /** '(' 左括號，前面若是右括號或數字，後面若是右括號或運算符號，非法 **/
                    if( tokenType[i-1] == 1 || tokenType[i-1] == 3 )  valid = false;
                    if( tokenType[i+1] == 1 || tokenType[i+1] == 2 )  valid = false;
                    break;
            case 1: /** ')' 右括號，前面若是左括號或運算符號，後面若是左括號或數字，非法 **/
                    if( tokenType[i-1] == 0 || tokenType[i-1] == 2 )  valid = false;
                    if( tokenType[i+1] == 0 || tokenType[i+1] == 3 )  valid = false;
                    break;
            case 2: /** 運算符號 ，前面若是左括號或運算符號，後面若是右括號或運算符號，非法 **/
                    if( tokenType[i-1] == 0 || tokenType[i-1] == 2 )  valid = false;
                    if( tokenType[i+1] == 1 || tokenType[i+1] == 2 )  valid = false;
                    break;
            case 3: /** 數字 ，前面若是右括號或數字，後面若是左括號或數字，非法 **/
                    if( tokenType[i-1] == 1 || tokenType[i-1] == 3 )  valid = false;
                    if( tokenType[i+1] == 0 || tokenType[i+1] == 3 )  valid = false;
                    break;
        }
    }

    return valid;   /* 進階檢查完畢，回傳是否合法 */
};
/*************************************************************************************************/
/** 從expr中抓token並return                                                                     **/
/** 使用pass by reference更改n的值,細節請參考 https://openhome.cc/Gossip/CppGossip/PassBy.html **/
/*************************************************************************************************/
precedence get_token( char *symbol, int *n ) {
    *symbol = expr[(*n)++];         /* 從expr中一個一個抓字元,根據字元return */
    switch( *symbol ) {
        case '(' : return Lparen;
        case ')' : return Rparen;
        case '+' : return Plus;
        case '-' : return Minus;
        case '/' : return Divide;
        case '*' : return Times;
        case '%' : return Mod;
        case ' ' : return Eos;
        default  : return Operand;
    }
};
/*******************************************************************************/
/** 轉換成後序表示式時,處理完expr後stack中可能還有剩餘的operator沒pop出來  **/
/** 這裡將pop出來的東西轉換型別變成precedence的type                         **/
/*******************************************************************************/
precedence get_remaining_token( int n ){
    switch( n ){
        case 0 : return Lparen;
        case 1 : return Rparen;
        case 2 : return Plus;
        case 3 : return Minus;
        case 4 : return Times;
        case 5 : return Divide;
        case 6 : return Mod;
        case 7 : return Eos;
    }
};
void stack_full(){
    cout << "Stack is full." << endl;
    exit(0);
};
void stack_empty(){
    cout << "Stack is empty." << endl;
    exit(0);
};
void push(int item){
    if( top >= MAX_STACK_SIZE-1 )
        stack_full();
    Stack[++top] = item;
};
int pop(){
    if( top == -1 )
        stack_empty();
    return Stack[top--];
};
/****************************************************************************************************/
/** print_token() 投影片裡是將 token 輸出出來                                                     **/
/** 助教這裡改成將 token 接到 postfix_expr 後面,轉換完最後才一併輸出 postfix_expr                **/
/** 注意: switch 的每個 case 最後面一定要有 break,除非直接return東西像get_remaining_token()那樣  **/
/*****************************************************************************************************/
void print_token(int token_num){
    switch( token_num ){
        case 0: postfix_expr = postfix_expr + '(';   break;
        case 1: postfix_expr = postfix_expr + ')';   break;
        case 2: postfix_expr = postfix_expr + '+';   break;
        case 3: postfix_expr = postfix_expr + '-';   break;
        case 4: postfix_expr = postfix_expr + '*';   break;
        case 5: postfix_expr = postfix_expr + '/';   break;
        case 6: postfix_expr = postfix_expr + '%';   break;
        case 7: postfix_expr = postfix_expr + ' ';   break;
    }
};
/*************************************************************************/
/** postfix() 將中序表示式轉換成後序表示式                            **/
/*************************************************************************/
void postfix(){
    char symbol;
    precedence token;
    int n = 0;           // 中序表示式的index
    top = 0;             // stack top
    postfix_expr = "";   // 用來存放後序表示式,先初始化為空字串
    Stack[0] = Eos;      // 先把eos放到stack裏頭

    /** 用for迴圈從輸入的expr中序表示式字串一個一個抓token,直到抓到Eos為止(這個Eos不會被丟進stack裡) **/
    for( token = get_token( &symbol , &n ) ; token != Eos ; token = get_token( &symbol , &n ) ){
        /** token是數字,直接丟進postfix_expr **/
        if( token == Operand ){
            postfix_expr = postfix_expr + symbol;
        }
        /** token是右括號,不斷把stack中的元素pop出來並丟進postfix_expr,直到碰到左括號,最後把左括號pop出來 **/
        else if( token == Rparen ){
            while ( Stack[top] != Lparen )
                print_token( pop() );

            pop();
        }
        /** token是operator,先把stack中優先權大於等於自己的operator pop出來並丟進postfix_expr中,最後再把token push進stack **/
        else{
            while( isp[Stack[top]] >= icp[token] )
                print_token( pop() );

            push( token );
        }
    }
    /** expr抓完處理完後,stack中可能會有剩餘的operator還沒pop出來,這邊把他們全部pop出來並接到postfix_expr後面,直到一開始丟進stack[0]中的Eos為止 **/
    while( ( token = get_remaining_token( pop() )) != Eos )
        print_token( token );
};
/****************************************************************************************************/
/** eval()用來計算expr的答案,注意: 這裡的expr已經在main()裡頭換成postfix_expr,是後序表示式      **/
/****************************************************************************************************/
int eval(){
    precedence token;
    char symbol;
    int op1, op2;   // operand
    int n = 0;      // expr的index
    top = 0;        // stack top
    token = get_token( &symbol , &n );  //從expr中抓token

    while( token!=Eos ){
        /** token是數字,轉成int後push進stack中 **/
        if( token == Operand )
            push( symbol-'0' );
        /** token是運算符號,從stack中pop兩個數字出來做運算,將運算結果push進stack中 **/
        else{
            op2 = pop();
            op1 = pop();
            switch( token ){
                case Plus:   push( op1+op2 );  break;   /* 加 */
                case Minus:  push( op1-op2 );  break;   /* 減 */
                case Times:  push( op1*op2 );  break;   /* 乘 */
                case Divide: push( op1/op2 );  break;   /* 除 */
                case Mod:    push( op1%op2 );  break;   /* mod */
            }
        }
        token = get_token( &symbol , &n );  //從expr中抓token
    }
    return pop();  /** 已經處理完expr了, return 運算結果 **/
};
