#ifndef PARSE_UTIL_H
#define PARSE_UTIL_H

#include "node.h"
#include "symbol_table.h"

bool is_terminated(const AST_node* const checkMe);
bool is_unterminated_closed_parentehsis(const AST_node* const amIClosed);
bool is_terminable(const AST_node* const checkMe, const symbol_table* const context);
bool is_function_declaration(const AST_node* const checkMe);
bool is_function_definition(const AST_node* const checkMe);
bool is_function_call(const AST_node* const checkMe, const symbol_table* const context);
bool is_closed_curly_bracket(const AST_node* const amIClosed);

AST_node* get_rightmost_bottommost(const AST_node* const checkMe);
AST_node* get_rightmost_bottommost_non_terminal( const AST_node* const traverseMe);

#endif//PARSE_UTIL_H
