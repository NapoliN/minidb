#pragma once
#include <vector>
#include "condition.h"
#include "row.h"

/**
 * Table class to manage a collection of Row objects.
 */
class Table {
    public:
    void insert(const Row& row);
    std::vector<Row> select() const;
    std::vector<Row> select(const std::unique_ptr<ConditionNode>& condition) const;
    void update(std::string rowid, const Row& row);
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    private:
    std::vector<Row> rows;
};