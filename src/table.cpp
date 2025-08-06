#include "table.h"
#include <memory>
#include <fstream>
#include <sstream>

/**
 * Table class to manage a collection of Row objects.
 */
void Table::insert(const Row& row) {
    rows.push_back(row);
}

/**
 * Select all rows from the table.
 * @return A vector containing all Row objects in the table.
 */
std::vector<Row> Table::select() const {
    return rows;
}

/**
 * Select rows from the table based on a condition.
 * @param condition The condition to filter rows.
 * @return A vector containing Row objects that match the condition.
 */
std::vector<Row> Table::select(const std::unique_ptr<ConditionNode>& condition) const {
    std::vector<Row> result;
    for (const auto& row : rows) {
        if (condition->eval(row)) {
            result.push_back(row);
        }
    }
    return result;
}

bool Table::save(const std::string& filename) const {
    // Implementation for saving the table to a file
    std::ofstream file(filename);
    if(!file.is_open())return false;
    
    // preimplementation
    // save as CSV format
    for (const auto& row : rows) {
        file << row.id << "," << row.name << "\n";
    } 
    return true;
}

bool Table::load(const std::string& filename) {
    // Implementation for loading the table from a file
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    rows.clear();
    std::string line;

    // preimplementation
    // load from CSV format
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        Row row;
        std::string id_str, name;
        if (std::getline(ss, id_str, ',') && std::getline(ss, name)) {
            row.id = std::stoi(id_str);
            row.name = name;
            rows.push_back(row);
        }
    }
    return true;
}
