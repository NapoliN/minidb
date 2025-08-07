#include <iostream>
#include "parser.h"
#include "transaction.h"
#include "table.h"

int main() {
    std::string input;
    Table table = Table(); // Create a Table instance
    table.load("data.csv"); // Load existing data from file
    
    TransactionManager t_manager = TransactionManager(); // Create a TransactionManager instance
    Transaction* current_transaction = nullptr;

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
        std::vector<Row> rows;
        switch (stmt.type) {
            case CommandType::BEGIN_TRANSACTION: {
                if(current_transaction) {
                    std::cout << "A transaction is already in progress. Please commit or rollback first.\n";
                    break;
                }
                current_transaction = t_manager.begin();
                std::cout << "Transaction started.\n";
                break;
            }
            case CommandType::COMMIT: {
                if(!current_transaction) {
                    std::cout << "No active transaction to commit.\n";
                    break;
                }
                t_manager.commit(current_transaction, table);
                current_transaction = nullptr; // Reset current transaction after commit
                std::cout << " All changes were committed.\n";
                break;
            }
            case CommandType::ROLLBACK: {
                if(!current_transaction) {
                    std::cout << "No active transaction to rollback.\n";
                    break;
                }
                t_manager.rollback(current_transaction);
                current_transaction = nullptr; // Reset current transaction after rollback
                std::cout << "All changes were discarded.\n";
                break;
            }
            case CommandType::INSERT:
                if (stmt.id && stmt.name) {
                    Row row{*stmt.id, *stmt.name};
                    // Transaction mode
                    if(current_transaction){
                        current_transaction->insert(row);
                    }
                    // Non-transaction mode
                    else {
                        table.insert(row);
                        table.save("data.csv"); // Save after insert
                    }
                    std::cout << "Inserted row with id: " << *stmt.id << " and name: " << *stmt.name << "\n";
                } else {
                    std::cout << "Error: Missing id or name for insert command\n";
                }
                break;
            case CommandType::SELECT:
                if(stmt.condition.has_value()) {
                    rows = table.select(stmt.condition.value());
                }else {
                    // If no condition is provided, select all rows
                    rows = table.select();
                }
                // Transaction mode
                if(current_transaction) {
                    // In transaction mode, we need to add the rows from the transaction buffer
                    for (const auto& [rowid, cv] : current_transaction->get_change_vectors()) {
                        switch (cv.type) {
                            case ChangeType::INSERT:
                                if(stmt.condition.has_value() && !stmt.condition.value()->eval(cv.row)) {
                                    continue; // Skip if condition does not match
                                }
                                rows.push_back(cv.row); // Add the inserted row
                                break;
                            case ChangeType::UPDATE:
                                // Find the row to update
                                auto it = std::find_if(rows.begin(), rows.end(), [&](const Row& r) { return std::to_string(r.id) == rowid; });
                                if(stmt.condition.has_value() && !stmt.condition.value()->eval(cv.row)) {
                                    continue; // Skip if condition does not match
                                }
                                if (it != rows.end()) {
                                    *it = cv.row; // Update the row
                                }
                                break;
                            // Handle other ChangeTypes if necessary
                        }
                    }
                }

                if (rows.empty()) {
                    std::cout << "No rows found\n";
                } else {
                    for (const auto& row : rows) {
                        std::cout << "Row id: " << row.id << ", name: " << row.name << "\n";
                    }
                }

                break;
            case CommandType::UPDATE: {
                if (stmt.id_value_map.empty()) {
                    std::cout << "Error: specify at least one column to update\n";
                    break;
                }
                if(stmt.condition.has_value()){
                    rows = table.select(stmt.condition.value());
                }else{
                    // If no condition is provided, select all rows
                    rows = table.select();
                }
                for (const auto& row : rows) {
                    std::string rowid = std::to_string(row.id);
                    Row update_row = row;
                    for (const auto& [column, value] : stmt.id_value_map) {
                        if (column == "id") {
                            update_row.id = std::stoi(value);
                        }
                        else if (column == "name") {
                            update_row.name = value;
                        }
                    }
                    // Transaction mode
                    if(current_transaction) {
                        current_transaction->update(rowid, update_row);
                    } else {
                        table.update(rowid, update_row);
                    }
                }
                table.save("data.csv"); // Save after update
                std::cout << rows.size() << " rows updated.\n";
                break;
            }
            default:
                std::cout << "Unknown command type\n";
                break;
        }
    }
    return 0;
}