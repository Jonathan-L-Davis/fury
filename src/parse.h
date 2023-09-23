#ifndef PARSE_H
#define PARSE_H

#include <vector>
#include <string>
#include "token.h"

struct AST_node{
    token tok;
    std::vector<AST_node> children;
};

AST_node parse(const std::vector<token> &tokens);

#endif//PARSE_H