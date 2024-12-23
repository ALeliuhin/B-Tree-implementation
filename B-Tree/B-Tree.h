#pragma once
#include <string>

const int MIN_DEGREE = 3;

class Node{
    public:
        int unique_id;
        int num_keys;
        int* keys;    
        Node** children;
        bool isLeaf;
        Node(){
            static int id = 10;
            num_keys = 0;
            isLeaf = true;
            keys = new int[2*MIN_DEGREE - 1];
            children = new Node*[2*MIN_DEGREE];
            unique_id = id++;
        }
};

class BTree{
    public:
        Node* root;
        int depth;

        BTree();

        Node* searchNodeKey(Node* nodeToSearch, int keyToSearch);

        Node* splitRoot();
        void splitChild(Node* nonfullNode, int indexOfFull);
        void insertNonFull(Node* nodeToInsertIn, int keyToInsert);
        void insert(int keyToInsert);
        
        void display(Node *x, int indent);
        void inorderDisplay(Node *x);
        void indentedDisplay();
        void visualize(Node* x, std::string filename = "btree.dot");
};
