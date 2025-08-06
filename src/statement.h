#pragma once
#include <string>
#include <optional>
#include <memory>
#include "condition.h"

enum class CommandType {
    INSERT,
    SELECT,
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
    std::optional<std::unique_ptr<ConditionNode>> condition;
};