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

bool is_operator_partial_declaration(const AST_node* const checkMe);
bool is_operator_declaration(const AST_node* const checkMe);
bool is_operator_definition(const AST_node* const checkMe);
bool is_operator_call(const AST_node* const checkMe, const symbol_table* const context);

bool is_syntax_partial_declaration(const AST_node* const checkMe);
bool is_syntax_declaration(const AST_node* const checkMe);
bool is_syntax_definition(const AST_node* const checkMe);

bool is_if_statement(const AST_node* const checkMe);
bool is_if_else_statement(const AST_node* const checkMe);

bool is_for_loop(const AST_node* const checkMe);
bool is_while_loop(const AST_node* const checkMe);

bool is_return_statement(const AST_node* const checkMe, const symbol_table* const context);

bool is_closed_parenthesis(const AST_node* checkMe);
bool is_closed_curly_bracket(const AST_node* const amIClosed);

bool is_type_declaration(const AST_node* const checkMe, const symbol_table* const context);

bool is_empty_comma(const AST_node* const checkMe);
bool is_comma_expression(const AST_node* const checkMe);

bool is_complete_type(const AST_node* const checkMe);

bool is_id( const AST_node* const checkMe, const symbol_table* const context);

AST_node* get_rightmost_bottommost(const AST_node* const checkMe);
AST_node* get_rightmost_bottommost_non_terminal( const AST_node* const traverseMe);

std::string get_op_id(const AST_node* const idMe);
std::vector<std::string> get_op_signature(const AST_node* const signMe);

std::string get_func_id(const AST_node* const idMe);
std::vector<std::string> get_func_signature(const AST_node* const signMe);

void move_operator_param_declarations(const AST_node* const op_id, symbol_table& src, symbol_table& dst);
void move_curly_bracket_declarations(const AST_node* const op_id, symbol_table& src, symbol_table& dst);

#endif//PARSE_UTIL_H
