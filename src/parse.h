#ifndef PARSE_H
#define PARSE_H

#include <vector>
#include <string>
#include "token.h"

struct AST_node{
    token tok;
    std::vector<AST_node> children;
    void print();
};

AST_node parse_expression(const std::vector<token> &tokens, int &start_pos);

AST_node parse_if(const std::vector<token> &tokens, int &start_pos);

#endif//PARSE_H