#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>

#include "parse.h"

struct symbol{
    std::string type;
    std::string name;
    AST_node* value;
    
    void print();
    
};

enum scope_type:uint32_t{
    scope_t_function = 0,
    scope_t_if = 1,
    scope_t_for = 2,
    scope_t_while = 3,
    scope_t_anonymous = 4,// random {} blocks, quite useful in C++.
    scope_type_root = 5,
    scope_type_operator = 6,
    scope_type_syntax = 7,
};


/**    ASSUMES THAT parent == nullptr IFF scope== ""    **/
struct symbol_table{
    
    symbol_table* parent; //non owning
    scope_type type;
    symbol_table();
    symbol_table( symbol_table*, std::string scope, scope_type );
    
    std::string scope;
    std::vector<symbol_table> sub_scopes;
    std::vector<symbol> symbols;//may need to be more sophisticated than a string
    
    std::string get_full_scope();//returns absolute scope
    
    bool contains_id(std::string);//in this particular scope.
    bool id_exists(std::string find_me);//in this or parent scope.
    bool contains_scope(std::string);
    bool scope_exists(std::string find_me);
    void add_symbol(symbol);
    void add_scope(std::string scope, scope_type);
    bool id_starts_with_substr(std::string);
    bool type_starts_with_substr(std::string);
    
    void print();
    
};

#endif//SYMBOL_TABLE_H
