#include <parsertl/iterator.hpp>
#include "statement.h"
#include <optional>
#include <sstream>
#include <memory>
#include <lexertl/iterator.hpp>
#include <parsertl/generator.hpp>

class Parser {
    public:
        static std::optional<Statement> parse(const std::string& input) {

            parsertl::rules prule;
            parsertl::state_machine psm;

            // Terminal tokens
            prule.token("INSERT");
            prule.token("SELECT");
            prule.token("BEGIN");
            prule.token("COMMIT");
            prule.token("ROLLBACK");

            prule.token("WHERE");
            prule.token("EQUAL");
            prule.token("VALUE");
            prule.token("IDENTIFIER");
            prule.token("NUMBER");
            prule.token("STRING");
            prule.token("'('");
            prule.token("')'");
            prule.token("','");

            // Non-terminal rules
            auto root = prule.push("command", "insert_stmt | select_stmt | begin_stmt | commit_stmt | rollback_stmt");
            auto cmd_insert = prule.push("insert_stmt", "INSERT '(' values ')'");
            auto cmd_select = prule.push("select_stmt", "SELECT where_clause");
            auto cmd_begin = prule.push("begin_stmt", "BEGIN");
            auto cmd_commit = prule.push("commit_stmt", "COMMIT");
            auto cmd_rollback = prule.push("rollback_stmt", "ROLLBACK");
            auto clause_where = prule.push("where_clause", "WHERE condition");
            prule.push("where_clause", ""); // Empty where clause is allowed

            auto op_equal = prule.push("condition", "IDENTIFIER EQUAL value");
            auto node_number = prule.push("value", "NUMBER");
            auto node_string = prule.push("value", "STRING");

            prule.push("values", "value");
            prule.push("values", "values ',' value");

            /**
            auto cond_and = prule.push("condition", "condition AND condition");
            auto cond_or = prule.push("condition", "condition OR condition");
            auto cond_not = prule.push("condition", "NOT condition");
            */
            parsertl::generator::build(prule, psm);
            
            lexertl::rules lrules;
            /**
             * command tokens
             */
            lrules.push("insert", prule.token_id("INSERT"));
            lrules.push("select", prule.token_id("SELECT"));
            lrules.push("begin", prule.token_id("BEGIN"));
            lrules.push("commit", prule.token_id("COMMIT"));
            lrules.push("rollback", prule.token_id("ROLLBACK"));

            /**
             * clause tokens
             */
            lrules.push("where", prule.token_id("WHERE"));

            lrules.push("\\s+", lrules.skip());
            lrules.push("[(]", prule.token_id("'('"));
            lrules.push("[)]", prule.token_id("')'"));
            lrules.push(",", prule.token_id("','"));
            lrules.push("=", prule.token_id("EQUAL"));

            /**
             * identifier and value tokens
             */
            lrules.push("[a-zA-Z_][a-zA-Z0-9_]*", prule.token_id("IDENTIFIER"));
            lrules.push("\\\"[^\\\"]+\\\"", prule.token_id("STRING"));
            lrules.push("\\d+", prule.token_id("NUMBER"));

            lexertl::state_machine lsm;
            lexertl::generator::build(lrules,lsm);

            lexertl::citerator iter(input.c_str(), input.c_str() + input.size(), lsm);
            parsertl::citerator piter(iter, psm);

            std::vector<std::unique_ptr<ConditionNode>> conditions_stack;
            std::vector<std::string> values_stack;

            Statement stmt;

            for (; piter->entry.action != parsertl::action::error && piter->entry.action != parsertl::action::accept; ++piter) {
                const std::size_t rule_ = piter->reduce_id();
                if (rule_ == cmd_insert) {
                    stmt.type = CommandType::INSERT;
                    std::string name_token = values_stack.back();
                    values_stack.pop_back();     
                    int id_token = std::stoi(values_stack.back());
                    values_stack.pop_back();  
                    stmt.id = id_token;
                    stmt.name = name_token; 
                    return stmt;
                }
                else if (rule_ == cmd_select) {
                    stmt.type = CommandType::SELECT;
                    return stmt;
                } else if (rule_ == cmd_begin) {
                    stmt.type = CommandType::BEGIN_TRANSACTION;
                    return stmt;
                } else if (rule_ == cmd_commit) {
                    stmt.type = CommandType::COMMIT;
                    return stmt;
                } else if (rule_ == cmd_rollback) {
                    stmt.type = CommandType::ROLLBACK;
                    return stmt;
                } 
                
                else if (rule_ == clause_where) {
                    stmt.condition = std::move(conditions_stack.back());

                    conditions_stack.pop_back();
                }
                
                else if (rule_ == op_equal) {
                    // Create a LeafCondition for the equal operation
                    auto column_name = piter.dollar(0).str(); // get identifier
                    auto value = values_stack.back();
                    values_stack.pop_back();
                    std::unique_ptr<LeafCondition> leaf_condition = std::make_unique<LeafCondition>();
                    leaf_condition->column = column_name;
                    leaf_condition->value = value;
                    leaf_condition->op = ConditionType::EQUAL;
                    
                    conditions_stack.emplace_back(std::move(leaf_condition));
                }

                else if(rule_ == node_number) {
                    // Create a new Row object and add it to the values stack
                    std::string value = piter.dollar(0).str();
                    values_stack.push_back(value);
                }
                else if (rule_ == node_string) {
                    // Create a new Row object and add it to the values stack
                    std::string value = piter.dollar(0).str();
                    values_stack.push_back(value.substr(1, value.length() - 2)); // Remove quotes
                }
                
            }
            return std::nullopt; // Unrecognized command
        }
};
