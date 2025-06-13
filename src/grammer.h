#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "node.h"
#include "symbol_table.h"
#include <string>
#include <vector>

typedef bool (*reduction_applies)(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
typedef void (*reduction)        (std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

struct rule{
    std::string name;
    reduction_applies rule_applies;
    reduction apply_rule;
};

enum class parse_dir: bool{forward,backward};

struct grammer{
    
    void add_layer(int i,parse_dir dir);
    void add_rule(int i,rule addMe);
    
    std::vector<rule>& get_layer(int i);
    
    std::vector<std::vector<rule>> rules;
    std::vector<parse_dir> direction;
};

grammer fury_grammer();
symbol_table fury_default_context();

#endif//GRAMMER_H
