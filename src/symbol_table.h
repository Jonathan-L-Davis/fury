#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>

#include "parse.h"

enum type_type:uint32_t{
    semantic_epsilon = 0,//void
    semantic_function = 1,//includes operators,
    semantic_byte = 2,
    semantic_dual = 3,
    semantic_quad = 4,
    semantic_oct = 5,
    semantic_struct = 6,//includes unions
    semantic_type = 7,// type is type? generic? not used for the first version.
    semantic_label = 8,// used for goto's, labels will be explicitly marked like variables. should be able to make them pointers somehow.
    semantic_literal = 9,
};

struct type_t{
    type_type my_type = semantic_epsilon;
};//not needed yet. as soon as things can pass back functions or structs, this'll be a nasty refactor, but I need something working before that even matters.

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
    scope_t_anonymous = 4,
    scope_type_root = 5,
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

symbol_table anal( const AST_node& frisk_me );

#endif//SYMBOL_TABLE_H
