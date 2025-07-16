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
std::set<std::string> get_operator_declaration_type(const AST_node * const typeMe, symbol_table& context);

#endif//TYPE_UTIL_H
