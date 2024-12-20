#include "B-Tree.h"
#include <iostream>

// B-Tree field

BTree::BTree(){
    root = new Node(); 
    // diskWrite(root);
    depth = 0;   
}

/*
    Iterates through each key starting from root. 
    If the key is found and the position is less that x.n, return the node.
    If the key is not found and the node is a leaf, return NULL.
    Otherwise, recursively pass the i-th child.
*/

Node* BTree::searchNodeKey(Node* nodeToSearch, int keyToSearch){
    auto i = 0;
    while(i < nodeToSearch->num_keys && keyToSearch > nodeToSearch->keys[i]){
        i++;
    }
    if(i < nodeToSearch->num_keys && keyToSearch == nodeToSearch->keys[i]){
        return nodeToSearch;
    }
    else if(nodeToSearch->isLeaf){
        return nullptr;
    }
    return searchNodeKey(nodeToSearch->children[i], keyToSearch);
}


void BTree::splitChild(Node* nonfullNode, int indexOfFull){
    Node* fullNode = nonfullNode->children[indexOfFull];
    Node* siblingNode = new Node;
    siblingNode->isLeaf = fullNode->isLeaf;
    siblingNode->num_keys = MIN_DEGREE - 1;
    for(auto i = 0; i < MIN_DEGREE - 1; i++){
        siblingNode->keys[i] = fullNode->keys[i + MIN_DEGREE];
    }
    if(!fullNode->isLeaf){
        for(auto j = 0; j < MIN_DEGREE; j++){
            siblingNode->children[j] = fullNode->children[j + MIN_DEGREE];
        }
    }
    siblingNode->num_keys = MIN_DEGREE - 1;
    for(auto c = nonfullNode->num_keys; c > indexOfFull ; c--){
        nonfullNode->children[c + 1] = nonfullNode->children[c];
        nonfullNode->keys[c] = nonfullNode->keys[c-1];
    }
    nonfullNode->children[indexOfFull+1] = siblingNode;
    nonfullNode->keys[indexOfFull] = fullNode->keys[MIN_DEGREE-1];
    nonfullNode->num_keys++;
    // diskWrite(fullNode);
    // diskWrite(siblingNode);
    // diskWrite(nonfullNode);
}


Node* BTree::splitRoot(BTree tree){
    Node* newRoot = new Node;
    newRoot->isLeaf = false;
    newRoot->keys = 0;
    newRoot->children[0] = tree.root;
    tree.root = newRoot;
    splitChild(newRoot, 0);
}

void BTree::insertNonFull(Node* nodeToInsertIn, int keyToInsert){
    int i = nodeToInsertIn->num_keys;
    if(nodeToInsertIn->isLeaf){
        while(i > 0 && keyToInsert < nodeToInsertIn->keys[i]){
            nodeToInsertIn->keys[i] = nodeToInsertIn->keys[i-1];
            i--;
        }
        nodeToInsertIn->keys[i] = keyToInsert;
        nodeToInsertIn->num_keys++;
        // diskWrite(nodeToSearchIn);
    }
    else{
        while(i > 0 && keyToInsert < nodeToInsertIn->keys[i-1]){
            i--;
        }
        // diskRead(nodeToSearchIn->children[i]);
        if(nodeToInsertIn->children[i]->num_keys == 2*MIN_DEGREE + 1){
            splitChild(nodeToInsertIn, i);
            if(keyToInsert > nodeToInsertIn->keys[i]){
                i++;
            }
        }
        insertNonFull(nodeToInsertIn->children[i], keyToInsert);
    }
}