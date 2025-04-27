#ifndef PARSE_UTIL_H
#define PARSE_UTIL_H

#include "node.h"
#include "symbol_table.h"

bool is_terminated(const AST_node* const checkMe);
bool is_unterminated_closed_parentehsis(const AST_node* const amIClosed);
bool is_terminable(const AST_node* const checkMe, const symbol_table* const context);
bool is_valid(const AST_node* const checkMe, const symbol_table* const context);

bool is_function_partial_declaration(const AST_node* const checkMe);
bool is_function_declaration(const AST_node* const checkMe);
bool is_function_definition(const AST_node* const checkMe);
bool is_function_call(const AST_node* const checkMe, const symbol_table* const context);

bool is_closed_parenthesis(const AST_node* checkMe);
bool is_closed_curly_bracket(const AST_node* const amIClosed);

bool is_type_declaration(const AST_node* const checkMe, const symbol_table* const context);

bool is_empty_comma(const AST_node* const checkMe);
bool is_comma_expression(const AST_node* const checkMe);

bool is_complete_type(const AST_node* const checkMe);

AST_node* get_rightmost_bottommost(const AST_node* const checkMe);
AST_node* get_rightmost_bottommost_non_terminal( const AST_node* const traverseMe);

#endif//PARSE_UTIL_H
