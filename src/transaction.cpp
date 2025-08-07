#include "transaction.h"
#include "table.h"
#include <algorithm>

// TransactionManagerの実装

Transaction* TransactionManager::begin() {
    // 新しいトランザクションを作成
    std::unique_ptr<Transaction> txn(new Transaction());
    Transaction* txn_ptr = txn.get();
    active_transactions.push_back(std::move(txn));
    return txn_ptr;
}

void TransactionManager::commit(Transaction* transaction, Table& table) {
    if (!transaction) return;
    transaction->commit(table);
    // active_transactionsからtransactionを削除
    auto it = std::remove_if(active_transactions.begin(), active_transactions.end(),
        [transaction](const std::unique_ptr<Transaction>& ptr) {
            return ptr.get() == transaction;
        });
    active_transactions.erase(it, active_transactions.end());
}

void TransactionManager::rollback(Transaction* transaction) {
    if (!transaction) return;
    transaction->rollback();
    // active_transactionsからtransactionを削除
    auto it = std::remove_if(active_transactions.begin(), active_transactions.end(),
        [transaction](const std::unique_ptr<Transaction>& ptr) {
            return ptr.get() == transaction;
        });
    active_transactions.erase(it, active_transactions.end());
}

const std::vector<std::unique_ptr<Transaction>>& TransactionManager::get_active_transactions() const {
    return active_transactions;
}

// Transactionの実装

void Transaction::insert(const Row& row) {
    ChangeVector change_vector{ChangeType::INSERT, row};
    change_vectors.insert({std::to_string(row.id), change_vector});
}

void Transaction::update(const std::string& rowid, const Row& row) {
    ChangeVector change_vector{ChangeType::UPDATE, row};
    change_vectors[rowid] = change_vector;
}

void Transaction::commit(Table& table) {
    // insert_buffer内の行をTableに反映
    for (const auto& [rowid, cv] : change_vectors) {
        switch(cv.type) {
            case ChangeType::INSERT:
                table.insert(cv.row);
                break;
            case ChangeType::UPDATE:
                table.update(rowid, cv.row);
                break;
            // 他のChangeTypeがあればここに追加
        }
    }
    table.save("data.csv"); // Save after commit
    change_vectors.clear();
}

void Transaction::rollback() {
    change_vectors.clear();
}
