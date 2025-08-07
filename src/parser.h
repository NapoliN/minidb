#include <optional>
#include "statement.h"

class Parser {
    public:
        static std::optional<Statement> parse(const std::string& input);
};
