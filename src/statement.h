#pragma once
#include <string>
#include <optional>

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
};