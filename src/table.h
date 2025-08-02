#pragma once
#include <vector>
#include "row.h"

/**
 * Table class to manage a collection of Row objects.
 */
class Table {
    public:
    void insert(const Row& row);
    std::vector<Row> select_all() const;
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    private:
    std::vector<Row> rows;
};
