#include "B-Tree.h"
#include <iostream>

int main(){
    BTree* newBTree = new BTree;
    while(true){
        // std::cout << "\033[2J\033[1;1H";
        std::cout << "------------- B-Tree -------------" << std::endl;
        std::cout << "1. Insert" << std::endl;
        std::cout << "2. Display" << std::endl;
        std::cout << "3. Visualize" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        int choice;
        std::cin >> choice;
        switch(choice){
            case 1:
                int key;
                std::cout << "Enter key to insert: ";
                std::cin >> key;
                newBTree->insert(key);
                break;
            case 2:
                newBTree->indentedDisplay();
                break;
            case 3:
                newBTree->visualize(newBTree->root);
                break;
            case 4:
                return 0;
            default:
                std::cout << "Invalid choice" << std::endl;
        }
    }
    return 0;
}