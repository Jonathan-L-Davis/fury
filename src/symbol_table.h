#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>

#include "node.h"

enum symbol_type:uint32_t{
    sym_t_byte     = 1,
    sym_t_dual     = 2,
    sym_t_quad     = 3,
    sym_t_oct      = 4,
    sym_t_struct   = 5,
    sym_t_function = 6,
    sym_t_operator = 7,
    sym_t_syntax   = 8,
    sym_t_label    = 9,
    sym_t_type    = 10,
};

struct symbol{
    
    symbol_type sym_type;// used for differentiating what bucket the sym 
    
    std::string type;// 'evaluated type', think functions return type. After the function evaluates, it sort of is it's eval type, even though it's 'real' type is 'function T * (...)'
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
    
    std::vector<symbol> bytes;
    std::vector<symbol> duals;
    std::vector<symbol> quads;
    std::vector<symbol> octs;
    std::vector<symbol> structs;
    std::vector<symbol> functions;
    std::vector<symbol> operators;
    std::vector<symbol> syntaxes;
    std::vector<symbol> labels;
    std::vector<symbol> types;
    
    std::string get_full_scope();//returns absolute scope
    
    //in this particular scope.
    bool contains_id(std::string) const;
    
    bool contains_byte(std::string) const;
    bool contains_dual(std::string) const;
    bool contains_quad(std::string) const;
    bool contains_oct(std::string) const;
    bool contains_struct(std::string) const;
    bool contains_function(std::string) const;
    bool contains_operator(std::string) const;
    bool contains_syntax(std::string) const;
    bool contains_label(std::string) const;
    bool contains_type(std::string) const;
    
    bool contains_scope(std::string) const;
    
    //in this or parent scope.
    bool id_exists(std::string) const;
    
    bool byte_exists(std::string) const;
    bool dual_exists(std::string) const;
    bool quad_exists(std::string) const;
    bool oct_exists(std::string) const;
    bool struct_exists(std::string) const;
    bool function_exists(std::string) const;
    bool operator_exists(std::string) const;
    bool syntax_exists(std::string) const;
    bool label_exists(std::string) const;
    bool type_exists(std::string) const;
    
    bool scope_exists(std::string find_me) const;
    //
    
    
    symbol get_function(std::string) const;
    symbol get_byte(std::string) const;
    symbol get_dual(std::string) const;
    symbol get_quad(std::string) const;
    symbol get_oct(std::string) const;
    
    //
    
    void add_symbol(symbol);
    void add_scope(std::string scope, scope_type);
    
    bool id_starts_with_substr(std::string);
    bool type_starts_with_substr(std::string);
    
    void print();
    
};

#endif//SYMBOL_TABLE_H
