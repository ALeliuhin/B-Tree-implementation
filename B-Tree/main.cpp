#include "B-Tree.h"
#include <iostream>

int main(){
    BTree* newBTree = new BTree;
    while(true){
        // std::cout << "\033[2J\033[1;1H"; // Clear screen
        std::cout << "┌─────────────────────────────────┐" << std::endl;
        std::cout << "│            B-Tree Menu          │" << std::endl;
        std::cout << "├─────────────────────────────────┤" << std::endl;
        std::cout << "│ 1 │ Insert                      │" << std::endl;
        std::cout << "├───┼─────────────────────────────┤" << std::endl;
        std::cout << "│ 2 │ Delete                      │" << std::endl;
        std::cout << "├───┼─────────────────────────────┤" << std::endl;
        std::cout << "│ 3 │ Display                     │" << std::endl;
        std::cout << "├───┼─────────────────────────────┤" << std::endl;
        std::cout << "│ 4 │ Visualize                   │" << std::endl;
        std::cout << "├───┼─────────────────────────────┤" << std::endl;
        std::cout << "│ 5 │ Exit                        │" << std::endl;
        std::cout << "└───┴─────────────────────────────┘" << std::endl;
        
        int choice;
        std::cout << "Select an option: ";
        std::cin >> choice;

        switch(choice){
            case 1: {
                int key;
                std::cout << "Enter key to insert: ";
                std::cin >> key;
                newBTree->insert(key);
                break;
            }
            case 2: {
                int key;
                std::cout << "Enter key to delete: ";
                std::cin >> key;
                newBTree->removeKey(key);
                break;
            }
            case 3: {
                newBTree->indentedDisplay();
                break;
            }
            case 4:
                newBTree->visualize(newBTree->root);
                break;
            case 5:
                delete newBTree; // Free memory
                return 0;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return 0;
}
