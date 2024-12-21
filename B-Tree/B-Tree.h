#pragma once

const int MIN_DEGREE = 3;

class Node{
    public:
        int num_keys;
        int* keys;    
        Node** children;
        bool isLeaf;
        Node(){
            num_keys = 0;
            isLeaf = true;
            keys = new int[2*MIN_DEGREE - 1];
            children = new Node*[2*MIN_DEGREE];
        }
};

class BTree{
    public:
        Node* root;
        int depth;

        BTree();

        Node* searchNodeKey(Node* nodeToSearch, int keyToSearch);

        Node* splitRoot(BTree* tree);
        void splitChild(Node* nonfullNode, int indexOfFull);
        void insertNonFull(Node* nodeToInsertIn, int keyToInsert);
        void insert(BTree* tree, int keyToInsert);

        void display(Node *x, int indent);
        void inorderDisplay(Node *x);
        void indentedDisplay();
};
