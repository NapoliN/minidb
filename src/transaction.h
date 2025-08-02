#pragma once
#include "table.h"
#include <vector>
#include <memory>

class Transaction;

// TransactionManagerの責務：Transactionの生成・管理・削除
class TransactionManager {
public:
    // 新しいトランザクションを開始して返す
    Transaction* begin();

    // 指定したトランザクションをコミット（Tableに反映し、リストから削除）
    void commit(Transaction* transaction, Table& table);

    // 指定したトランザクションをロールバック（バッファ破棄＋リストから削除）
    void rollback(Transaction* transaction);

    // 進行中のトランザクション一覧（参照のみ返す）
    const std::vector<std::unique_ptr<Transaction>>& get_active_transactions() const;

private:
    // 進行中のトランザクションを一括管理（unique_ptrで安全な所有権管理）
    std::vector<std::unique_ptr<Transaction>> active_transactions;
};

class Transaction {
public:
    // トランザクションに行を挿入
    void insert(const Row& row);

    // トランザクションをコミットして、変更をテーブルに反映
    void commit(Table& table);

    // トランザクションをロールバック（バッファを破棄）
    void rollback();

    std::vector<Row> get_inserted_rows() const {
        return insert_buffer;
    }

private:
    // このトランザクションでinsertされた行のバッファ
    std::vector<Row> insert_buffer;
};