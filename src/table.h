#include <vector>
#include "row.h"

/**
 * Table class to manage a collection of Row objects.
 */
class Table {
    public:
    void insert(const Row& row) {
        rows.push_back(row);
    }
    std::vector<Row> select_all() const {
        return rows;
    }

    private:
    std::vector<Row> rows;
};
