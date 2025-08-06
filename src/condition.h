#pragma once
#include <string>
#include <memory>
#include "row.h"

enum class ConditionType {
    EQUAL,
};

// 抽象基底クラス
struct ConditionNode {
    virtual bool eval(const Row& row) const = 0;
    virtual ~ConditionNode() = default;
};

struct LeafCondition : ConditionNode {
    std::string column, value;
    ConditionType op;
    bool eval(const Row& row) const override {
       if (column == "id"){
            if(op == ConditionType::EQUAL) {
                return row.id == std::stoi(value);
            }
       }
        else if (column == "name") {
            if(op == ConditionType::EQUAL) {
                return row.name == value;
            }
        }
        // 他の条件も追加可能
        return false; // 条件に合致しない場合はfalseを返す
    }
};

struct AndCondition : ConditionNode {
    std::unique_ptr<ConditionNode> left, right;
    bool eval(const Row& row) const override {
        return left->eval(row) && right->eval(row);
    }
};

struct OrCondition : ConditionNode {
    std::unique_ptr<ConditionNode> left, right;
    bool eval(const Row& row) const override {
        return left->eval(row) || right->eval(row);
    }
};
