#include "B-Tree.h"
#include <iostream>

// B-Tree field

BTree::BTree(){
    root = new Node; 
    // diskWrite(root);
}

/*
    Iterates through each key starting from root. 
    If the key is found and the position is less that x.n, return the node.
    If the key is not found and the node is a leaf, return NULL.
    Otherwise, recursively pass the i-th child.
*/

void BTree::display(Node *x, int indent)
{
    if (x == nullptr)
        return;
    for (int i = 0; i < x->num_keys; i++)
    {
        if (!x->isLeaf)
            display((x->children)[i], indent + 2);
        for (int j = 0; j < indent; j++)
            std::cout << ' ';
        std::cout << (x->keys)[i] << '\n';
    }
    if (!x->isLeaf)
        display((x->children)[x->num_keys], indent + 2);
}

void BTree::inorderDisplay(Node *x)
{
    for (int i = 0; i < x->num_keys; i++)
    {
        if (!x->isLeaf)
            inorderDisplay((x->children)[i]);
        std::cout << (x->keys)[i] << ' ';
    }
    if (!x->isLeaf)
        inorderDisplay((x->children)[x->num_keys]);
}

void BTree::indentedDisplay()
{
    std::cout << "The B-tree is" << std::endl;
    display(root, 0);
}

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
    fullNode->num_keys = MIN_DEGREE - 1;
    for(auto i = nonfullNode->num_keys; i > indexOfFull; i--){
        nonfullNode->children[i + 1] = nonfullNode->children[i];
        nonfullNode->keys[i] = nonfullNode->keys[i-1];
    }
    nonfullNode->children[indexOfFull+1] = siblingNode;
    nonfullNode->keys[indexOfFull] = fullNode->keys[MIN_DEGREE-1];
    nonfullNode->num_keys++;
    // diskWrite(fullNode);
    // diskWrite(siblingNode);
    // diskWrite(nonfullNode);
}


Node* BTree::splitRoot(BTree* tree){
    Node* newRoot = new Node;
    newRoot->isLeaf = false;
    newRoot->num_keys = 0;
    newRoot->children[0] = tree->root;
    tree->root = newRoot;
    splitChild(newRoot, 0);
    return newRoot;
}

void BTree::insertNonFull(Node* nodeToInsertIn, int keyToInsert){
    int i = nodeToInsertIn->num_keys;
    if(nodeToInsertIn->isLeaf){
        while(i > 0 && keyToInsert < nodeToInsertIn->keys[i-1]){
            nodeToInsertIn->keys[i] = nodeToInsertIn->keys[i-1];
            i--;
        }
        nodeToInsertIn->keys[i] = keyToInsert;
        nodeToInsertIn->num_keys++;
        // diskWrite(nodeToInsertIn);
    }
    else{
        while(i > 0 && keyToInsert < nodeToInsertIn->keys[i-1]){
            i--;
        }
        // diskRead(nodeToInsertIn->children[i]);
        if(nodeToInsertIn->children[i]->num_keys == 2*MIN_DEGREE - 1){
            splitChild(nodeToInsertIn, i);
            if(keyToInsert > nodeToInsertIn->keys[i]){
                i++;
            }
        }
        insertNonFull(nodeToInsertIn->children[i], keyToInsert);
    }
}

void BTree::insert(BTree* tree, int keyToInsert){
    Node* root = tree->root;
    if(root->num_keys == 2*MIN_DEGREE - 1){
        Node* newRoot = splitRoot(tree);
        insertNonFull(newRoot, keyToInsert);
    }
    else{
        insertNonFull(root, keyToInsert);
    }
}