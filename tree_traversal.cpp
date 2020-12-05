#include<iostream>

using namespace std;

typedef struct node *tree_pointer;
typedef struct node{
    int data;
    int level;
    tree_pointer left_child, right_child;
};
tree_pointer root, search_node;

int parent_data, left_child_data, right_child_data;
bool valid_tree = true;
int Tree_height = 0;
int degree_2_node = 0, Tree_leaves = 0;

void add_node( tree_pointer , int , int );
void inorder( tree_pointer , int );
void preorder( tree_pointer );

int main(){

    while( cin >> parent_data ){
        cin >> left_child_data >> right_child_data;

        if( root == NULL ){
            root = (tree_pointer)new node;
            root->data = parent_data;
            root->level = 1;
            add_node( root , left_child_data , right_child_data );
        }
        else{
            inorder( root , parent_data );
            if( search_node ){
                add_node( search_node , left_child_data , right_child_data );
                search_node = NULL;
            }
            else{
                valid_tree = false;
                break;
            }
        }
    }
    if( root == NULL )
        cout << "Tree is empty";
    else if( valid_tree == false )
        cout << "Invalid tree";
    else {
        preorder( root );
        Tree_leaves = degree_2_node + 1;
        cout << Tree_height << endl;
        cout << Tree_leaves ;
    }

    return 0;
};

void add_node( tree_pointer parent , int left_data , int right_data ){
    tree_pointer left, right;

    if( left_data == 0 )
        left = NULL;
    else{
        left = (tree_pointer)new node;
        left->data = left_data;
        left->level = parent->level + 1;
        left->left_child = NULL;
        left->right_child = NULL;
    }
    if( right_data == 0 )
        right = NULL;
    else{
        right = (tree_pointer)new node;
        right->data = right_data;
        right->level = parent->level + 1;
        right->left_child = NULL;
        right->right_child = NULL;
    }

    parent->left_child = left;
    parent->right_child = right;

    if( left && right )
        degree_2_node++;
};

void inorder( tree_pointer ptr , int goal_data ){
    if( ptr ){
        inorder( ptr->left_child , goal_data );
        if( ptr->data == goal_data )
            search_node = ptr;
        inorder( ptr->right_child , goal_data );
    }
};

void preorder( tree_pointer ptr ){
    if( ptr ){
        if( ptr->level > Tree_height )
            Tree_height = ptr->level;
        preorder(ptr->left_child);
        preorder(ptr->right_child);
    }
};
