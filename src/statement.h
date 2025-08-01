#pragma once
#include <string>
#include <optional>

enum class CommandType {
    INSERT,
    SELECT,
};

struct Statement {
    CommandType type;
    // for insert
    std::optional<int> id;
    std::optional<std::string> name;
};