#include "B-Tree.h"

int main(){
    BTree* newBTree = new BTree;
    newBTree->insert(newBTree, 12);
    newBTree->insert(newBTree, 15);
    newBTree->indentedDisplay();
    newBTree->insert(newBTree, 2);
    newBTree->insert(newBTree, 51);
    newBTree->insert(newBTree, 1);
    newBTree->indentedDisplay();
    newBTree->insert(newBTree, 33);
    newBTree->insert(newBTree, 23);
    newBTree->insert(newBTree, 13);
    newBTree->insert(newBTree, 3);
    newBTree->insert(newBTree, 4);
    newBTree->insert(newBTree, 5);
    newBTree->insert(newBTree, 6);
    newBTree->insert(newBTree, 7);
    newBTree->insert(newBTree, 24);
    newBTree->indentedDisplay();
    return 0;
}