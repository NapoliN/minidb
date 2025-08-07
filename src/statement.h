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
    std::optional<std::unique_ptr<ConditionNode>> condition = std::nullopt;

    std::unordered_map<std::string, std::string> id_value_map;
};