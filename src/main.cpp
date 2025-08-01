#include <iostream>
#include "table.h"
#include "parser.h"

int main() {
    std::string input;
    Table table = Table(); // Create a Table instance
    
    while (true) {
        std::cout << "db> ";
        std::getline(std::cin, input);

        if (input == "exit") break;
        auto stmt_opt = Parser::parse(input);
        if (!stmt_opt) {
            std::cout << "Error: Unrecognized command\n";
            continue;
        }

        // Process the statement
        const auto& stmt = *stmt_opt;
        if (stmt.type == CommandType::INSERT) {
            if (stmt.id && stmt.name) {
                Row row{*stmt.id, *stmt.name};
                table.insert(row);
                std::cout << "Inserted row with id: " << *stmt.id << " and name: " << *stmt.name << "\n";
            } else {
                std::cout << "Error: Missing id or name for insert command\n";
            }
        } else if (stmt.type == CommandType::SELECT) {
            auto rows = table.select_all();
            if (rows.empty()) {
                std::cout << "No rows found\n";
            } else {
                for (const auto& row : rows) {
                    std::cout << "Row id: " << row.id << ", name: " << row.name << "\n";
                }
            }
        } else {
            std::cout << "Unknown command type\n";
        }
    }
    return 0;
}