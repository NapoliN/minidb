#pragma once
#include <string>
#include <optional>
#include <memory>
#include <unordered_map>

#include "condition.h"

enum class CommandType {
    INSERT,
    SELECT,
    UPDATE,
    DELETE,
    BEGIN_TRANSACTION,
    COMMIT,
    ROLLBACK,
};



struct Statement {
    CommandType type;
    // for insert
    std::optional<int> id;
    std::optional<std::string> name;
     // for select
    std::unique_ptr<ConditionNode> condition = nullptr;

    std::unordered_map<std::string, std::string> id_value_map;
};