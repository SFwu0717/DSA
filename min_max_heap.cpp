#include<iostream>

using namespace std;

#define MAX_ELEMENTS 200

typedef struct {
    int key;
} element;
element heap[MAX_ELEMENTS];
int n = 0;
bool Max_or_Min;

void Insert( element , int * );
element Extract( int * );
void DecreaseKey_and_Heapify( int , int );
void Print();

int main(){
    string action;
    int key;
    element temp;

    cin >> action;
    if( action == "MaxHeap:")   Max_or_Min = true;
    else                        Max_or_Min = false;

    while( (cin >> key) && (key != -1) ){
        temp.key = key;
        Insert( temp , &n );
    }

    while( cin >> action ){
        if( action == "ExtractMax" || action == "ExtractMin" )
            cout << Extract(&n).key << endl;
        else if( action == "Insert:" ){
            cin >> key;
            temp.key = key;
            Insert( temp , &n );
        }
        else if( action == "DecreaseKey:" ){
            int decrease_element, offset;
            cin >> decrease_element >> offset;
            DecreaseKey_and_Heapify( decrease_element , offset );
        }
        else
            Print();
    }

    return 0;
}

void Insert( element item , int *n ){
    int i;
    i = ++(*n);
    while( (i!=1) && Max_or_Min?(item.key > heap[i/2].key):(item.key < heap[i/2].key) ){
        heap[i] = heap[i/2];
        i/=2;
    }
    heap[i] = item;
}

element Extract( int *n ){
    int parent, child;
    element item, temp;

    item = heap[1];
    temp = heap[(*n)--];
    parent = 1;
    child = 2;
    while( child <= *n ){
        if( (child < *n) && Max_or_Min?(heap[child].key < heap[child+1].key):(heap[child].key > heap[child+1].key) )
            child++;
        if( Max_or_Min? (temp.key >= heap[child].key):(temp.key <= heap[child].key) )
            break;
        heap[parent] = heap[child];
        parent = child;
        child *= 2;
    }
    heap[parent] = temp;
    return item;
}

void DecreaseKey_and_Heapify( int goal , int offset ){
    int index;
    for( index = 1 ; index <= MAX_ELEMENTS ; index++ ){
        if( heap[index].key == goal ){
            heap[index].key -= offset;
            break;
        }
    }
    while( (index!=1) && (heap[index].key < heap[index/2].key) ){
        swap(heap[index] , heap[index/2]);
        index/=2;
    }
}

void Print(){
    for( int i = 1 ; i <= n ; i++ )
        cout << heap[i].key << " ";
    cout << endl;
}
