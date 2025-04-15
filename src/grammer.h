#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "node.h"
#include "symbol_table.h"
#include <string>
#include <vector>

typedef bool (*reduction_applies)(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
typedef void (*reduction)        (std::vector<AST_node*>& nodePool, symbol_table* context, int index);

struct rule{
    std::string name;
    reduction_applies rule_applies;
    reduction apply_rule;
};

std::vector<rule> fury_grammer_rules();

#endif//GRAMMER_H
