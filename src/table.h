#include <vector>
#include "row.h"

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
