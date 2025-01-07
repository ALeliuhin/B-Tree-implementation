#include "B-Tree.h"
#include <iostream>

int main(){
    BTree* newBTree = new BTree;
    std::system("clear");
    while(true){
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
        std::cout << "\033[4;36mSelect an option\033[0m: ";
        std::cin >> choice;

        switch(choice){
            case 1: {
                int key;
                std::cout << "\033[1;34mEnter key to insert: \033[0m";
                std::cin >> key;
                newBTree->insert(key);
                std::system("clear"); 
                std::cout << "\033[1;34mKey inserted successfully.\033[0m\n"; 
                break;
            }
            case 2: {
                int key;
                std::cout << "\033[1;34mEnter key to delete: \033[0m";
                std::cin >> key;
                newBTree->removeKey(key);
                std::system("clear");
                std::cout << "\033[1;34mKey deleted successfully.\033[0m\n";
                break;
            }
            case 3: {
                std::system("clear"); 
                newBTree->indentedDisplay();
                break;
            }
            case 4:
                newBTree->visualize(newBTree->root);
                std::system("clear"); 
                break;
            case 5:
                delete newBTree; 
                return 0;
            default:
                std::system("clear"); 
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return 0;
}
