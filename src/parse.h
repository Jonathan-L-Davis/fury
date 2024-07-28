#ifndef PARSE_H
#define PARSE_H

#include <vector>
#include <string>
#include "lex.h"

struct AST_node{
    token tok;
    std::vector<AST_node> children;
    void print();
    void print_with_types();
};

AST_node parse(const std::vector<token> &tokens, int &start_pos);
AST_node parse_expression(const std::vector<token> &tokens, int &start_pos);

#endif//PARSE_H
