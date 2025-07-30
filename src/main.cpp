#include <iostream>
#include <string>

int main() {
    std::string input;
    while (true) {
        std::cout << "db> ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        if (input.find("insert") == 0) {
            std::cout << "Insert command (dummy)\n";
        } else if (input == "select") {
            std::cout << "Select command (dummy)\n";
        } else {
            std::cout << "Unrecognized command\n";
        }
    }
    return 0;
}