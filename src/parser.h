#include <parsertl/iterator.hpp>
#include "statement.h"
#include <optional>
#include <sstream>
#include <lexertl/iterator.hpp>
#include <parsertl/generator.hpp>

class Parser {
    public:
        static std::optional<Statement> parse(const std::string& input) {

            parsertl::rules prule;
            parsertl::state_machine psm;

            prule.token("INSERT");
            prule.token("SELECT");
            prule.token("BEGIN");
            prule.token("COMMIT");
            prule.token("ROLLBACK");
            prule.token("ID");
            prule.token("NAME");

            prule.push("command", "insert_stmt | select_stmt | begin_stmt | commit_stmt | rollback_stmt");
            auto cmd_insert = prule.push("insert_stmt", "INSERT ID NAME");
            auto cmd_select = prule.push("select_stmt", "SELECT");
            auto cmd_begin = prule.push("begin_stmt", "BEGIN");
            auto cmd_commit = prule.push("commit_stmt", "COMMIT");
            auto cmd_rollback = prule.push("rollback_stmt", "ROLLBACK");

            parsertl::generator::build(prule, psm);
            
            lexertl::rules lrules;
            lrules.push("insert", prule.token_id("INSERT"));
            lrules.push("select", prule.token_id("SELECT"));
            lrules.push("begin", prule.token_id("BEGIN"));
            lrules.push("commit", prule.token_id("COMMIT"));
            lrules.push("rollback", prule.token_id("ROLLBACK"));
            lrules.push("\\d+", prule.token_id("ID"));
            lrules.push("[a-zA-Z_][a-zA-Z0-9_]+", prule.token_id("NAME"));
            lrules.push("\\s+", lrules.skip());
            
            lexertl::state_machine lsm;
            lexertl::generator::build(lrules,lsm);

            lexertl::citerator iter(input.c_str(), input.c_str() + input.size(), lsm);
            parsertl::citerator piter(iter, psm);

            for (; piter->entry.action != parsertl::action::error && piter->entry.action != parsertl::action::accept; ++piter) {
                const std::size_t rule_ = piter->reduce_id();
                if (rule_ == cmd_insert) {
                    Statement stmt;
                    stmt.type = CommandType::INSERT;
                    auto id_token = piter.dollar(1);
                    auto name_token = piter.dollar(2);
                    stmt.id = std::stoi(id_token.first);
                    stmt.name = name_token.first;
                    return stmt;
                } else if (rule_ == cmd_select) {
                    Statement stmt;
                    stmt.type = CommandType::SELECT;
                    return stmt;
                } else if (rule_ == cmd_begin) {
                    Statement stmt;
                    stmt.type = CommandType::BEGIN_TRANSACTION;
                    return stmt;
                } else if (rule_ == cmd_commit) {
                    Statement stmt;
                    stmt.type = CommandType::COMMIT;
                    return stmt;
                } else if (rule_ == cmd_rollback) {
                    Statement stmt;
                    stmt.type = CommandType::ROLLBACK;
                    return stmt;
                }            
            }
            return std::nullopt; // Unrecognized command
        }
};
