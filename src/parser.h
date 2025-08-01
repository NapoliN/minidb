#pragma once
#include "statement.h"
#include <optional>
#include <sstream>

class Parser {
    public:
        static std::optional<Statement> parse(const std::string& input) {
            std::istringstream iss(input);
            std::string cmd;
            iss >> cmd;

            if (cmd == "insert") {
                int id;
                std::string name;
                if (iss >> id >> name) {
                    return Statement{CommandType::INSERT, id, name};
                } else {
                    return std::nullopt; // パース失敗
                }
            } else if (cmd == "select") {
                return Statement{CommandType::SELECT, 0, ""};
            } else {
                return std::nullopt;
            }
        }
};
