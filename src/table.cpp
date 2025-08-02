#include "table.h"
#include <fstream>
#include <sstream>

/**
 * Table class to manage a collection of Row objects.
 */
void Table::insert(const Row& row) {
    rows.push_back(row);
}

std::vector<Row> Table::select_all() const {
    return rows;
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
