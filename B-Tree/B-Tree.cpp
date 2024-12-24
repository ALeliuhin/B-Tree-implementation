#include "B-Tree.h"
#include <iostream>
#include <fstream>

/*
    Constructor
*/
BTree::BTree(){
    root = new Node; 
    // diskWrite(root);
}

/*
    Functions to store nodes and edges in .dot format
*/

void writeNodeKeys(Node* x, std::ofstream& file){
    file << "\tnode" << x->unique_id << " [label=\" ";
        for(auto i = 0; i < x->num_keys; i++){
            if(i == x->num_keys - 1) {
                file << x->keys[i];
                continue;
            }
            file << x->keys[i] << " | ";
        }
        for(auto i = 2*MIN_DEGREE - 1; i > x->num_keys; i--){
            file << "|";
        }
        file << "\"]" << std::endl;
}

void writeNodeEdges(Node* x, std::ofstream& file){
    for(auto i = 0; i < x->num_keys + 1; i++){
        file << "\tnode" << x->unique_id << " -> ";
        file << "\tnode" << x->children[i]->unique_id << std::endl;
    }
}

void visualizeHelper(Node* x, std::ofstream& file){
    if(x == nullptr){
        return;
    }
    if(x->isLeaf){
        writeNodeKeys(x, file);
        return;
    }
    for(auto i = 0; i < x->num_keys+1; i++){
        visualizeHelper(x->children[i], file);
    }
    writeNodeKeys(x, file);
    writeNodeEdges(x, file);
}

void BTree::visualize(Node* x, std::string filename){
    std::ofstream file;
    file.open(filename);
    file << "digraph G {" << std::endl;
    file << "\tnode [shape = record,height = .1]" << std::endl;
    visualizeHelper(x, file);
    file << "}" << std::endl;
    file.close();
}

/*
    Functions to display the nodes in terminal
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


Node* BTree::splitRoot(){
    Node* newRoot = new Node;
    newRoot->isLeaf = false;
    newRoot->num_keys = 0;
    newRoot->children[0] = root;
    root = newRoot;
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

void BTree::insert(int keyToInsert){
    Node* currentRoot = root;
    if(currentRoot->num_keys == 2*MIN_DEGREE - 1){
        Node* newRoot = splitRoot();
        insertNonFull(newRoot, keyToInsert);
    }
    else{
        insertNonFull(currentRoot, keyToInsert);
    }
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

int BTree::findKey(Node* nodeToSearch, int keyToSearch){
    auto i = 0;
    while(i < nodeToSearch->num_keys && keyToSearch > nodeToSearch->keys[i]){
        i++;
    }
    return i;
}

int BTree::findPredecessor(Node* x, int indexOfKey){
    Node* current = x->children[indexOfKey];
    while(!current->isLeaf){
        current = current->children[current->num_keys];
    }
    return current->keys[current->num_keys-1];
}

int BTree::findSuccessor(Node* x, int indexOfKey){
    Node* current = x->children[indexOfKey+1];
    while(!current->isLeaf){
        current = current->children[0];
    }
    return current->keys[0];
}


void BTree::borrowFromPrev(Node* parent, int indexToReplace){
    Node* child = parent->children[indexToReplace];
    Node* sibling = parent->children[indexToReplace-1];
    for(auto i = child->num_keys-1; i >= 0; --i){
        child->keys[i+1] = child->keys[i];
    }
    if(!child->isLeaf){
        for(auto j = child->num_keys; j >= 0; --j){
            child->children[j+1] = child->children[j];
        }
    }
    child->keys[0] = parent->keys[indexToReplace-1];
    if(!child->isLeaf){
        child->children[0] = sibling->children[sibling->num_keys];
    }
    parent->keys[indexToReplace-1] = sibling->keys[sibling->num_keys-1];
    child->num_keys++;
    sibling->num_keys--;
}

void BTree::borrowFromNext(Node* parent, int indexToReplace){
    Node* child = parent->children[indexToReplace];
    Node* sibling = parent->children[indexToReplace+1];
    child->keys[child->num_keys] = parent->keys[indexToReplace];
    if(!child->isLeaf){
        child->children[child->num_keys+1] = sibling->children[0];
    }
    parent->keys[indexToReplace] = sibling->keys[0];
    for(auto i = 1; i < sibling->num_keys; ++i){
        sibling->keys[i-1] = sibling->keys[i];
    }
    if(!sibling->isLeaf){
        for(auto j = 1; j <= sibling->num_keys; ++j){
            sibling->children[j-1] = sibling->children[j];
        }
    }
    child->num_keys++;
    sibling->num_keys--;
}

void BTree::merge(Node* parent, int indexOfKey){
    Node* child = parent->children[indexOfKey];
    Node* sibling = parent->children[indexOfKey+1];
    child->keys[MIN_DEGREE-1] = parent->keys[indexOfKey];
    for(auto i = 0; i < MIN_DEGREE-1; ++i){
        child->keys[i+MIN_DEGREE] = sibling->keys[i];
    }
    if(!child->isLeaf){
        for(auto j = 0; j < MIN_DEGREE; ++j){
            child->children[j+MIN_DEGREE] = sibling->children[j];
        }
    }
    for(auto i = indexOfKey+1; i < parent->num_keys; ++i){
        parent->keys[i-1] = parent->keys[i];
    }
    for(auto i = indexOfKey+2; i <= parent->num_keys; ++i){
        parent->children[i-1] = parent->children[i];
    }
    child->num_keys = 2*MIN_DEGREE - 1;
    parent->num_keys--;
    delete sibling;
    // diskWrite(child);
    // diskWrite(parent);
}

void BTree::fill(Node* x, int indexOfKey){
    if(indexOfKey != 0 && x->children[indexOfKey-1]->num_keys >= MIN_DEGREE){
        borrowFromPrev(x, indexOfKey);
    }
    else if(indexOfKey != x->num_keys && x->children[indexOfKey+1]->num_keys >= MIN_DEGREE){
        borrowFromNext(x, indexOfKey);
    }
    else{
        if(indexOfKey != x->num_keys){
            merge(x, indexOfKey);
        }
        else{
            merge(x, indexOfKey-1);
        }
    }
}

void BTree::removeFromLeaf(Node* x, int indexOfKey){
    for(auto i = indexOfKey + 1; i < x->num_keys; ++i){
        x->keys[i-1] = x->keys[i];
    }
    x->num_keys--;
}

void BTree::removeKeyHelper(Node* x, int keyToDelete){
    int indexOfKey = findKey(x, keyToDelete);
    if(indexOfKey < x->num_keys && x->keys[indexOfKey] == keyToDelete){
        if(x->isLeaf){
            removeFromLeaf(x, indexOfKey);
        }
        else{
            removeFromNonLeaf(x, indexOfKey);
        }
    }
    else{
        if(x->isLeaf){
            std::cout << "Key not found in the tree." << std::endl;
            return;
        }
        bool isLastChild = (indexOfKey == x->num_keys);
        if(x->children[indexOfKey]->num_keys < MIN_DEGREE){
            fill(x, indexOfKey);
        }
        if(isLastChild && indexOfKey > x->num_keys){
            removeKeyHelper(x->children[indexOfKey-1], keyToDelete);
        }
        else{
            removeKeyHelper(x->children[indexOfKey], keyToDelete);
        }
    }
}

void BTree::removeFromNonLeaf(Node* x, int indexOfKey){
    int key = x->keys[indexOfKey];
    if(x->children[indexOfKey]->num_keys >= MIN_DEGREE){
        int predecessor = findPredecessor(x, indexOfKey);
        x->keys[indexOfKey] = predecessor;
        removeKeyHelper(x->children[indexOfKey], predecessor);
    }
    else if(x->children[indexOfKey+1]->num_keys >= MIN_DEGREE){
        int successor = findSuccessor(x, indexOfKey);
        x->keys[indexOfKey] = successor;
        removeKeyHelper(x->children[indexOfKey+1], successor);
    }
    else{
        merge(x, indexOfKey);
        removeKeyHelper(x->children[indexOfKey], key);
    }
}

void BTree::removeKey(int keyToDelete){
    Node* currentRoot = root;
    removeKeyHelper(currentRoot, keyToDelete);
}