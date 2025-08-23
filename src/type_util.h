#ifndef TYPE_UTIL_H
#define TYPE_UTIL_H

#include <vector>
#include <string>
#include <set>
#include "node.h"
#include "symbol_table.h"

std::vector<std::string> get_function_param_types( const AST_node * const knowMe );
std::vector<std::string> get_operator_param_types( const AST_node * const knowMe );
std::set<std::string> get_type(const AST_node * const typeMe, symbol_table& context);

std::set<std::string> get_type_of_if(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_for(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_while(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_function_call(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_operator_call(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_syntax_call(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_id(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_parenthesis(const AST_node * const typeMe, symbol_table& context);
std::set<std::string> get_type_of_curly_bracket(const AST_node * const typeMe, symbol_table& context);

#endif//TYPE_UTIL_H
