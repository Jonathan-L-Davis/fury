#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>

#include "node.h"
bool operator_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

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
    
    std::set<std::string> type;
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
    scope_t_root = 5,
    scope_t_operator = 6,
    scope_t_syntax = 7,
    scope_t_struct = 8,
};


/**    ASSUMES THAT parent == nullptr IFF scope== ""    **/
struct symbol_table{
    
    symbol_table* parent; //non owning
    AST_node* node;
    scope_type type;
    symbol_table();
    symbol_table( symbol_table*, std::string scope, scope_type T, AST_node* node );
    
    std::string scope;
    std::vector<std::string> signature;
    std::vector<symbol_table*> sub_scopes;
    
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
    
    //std::map<std::string,symbol> id_to_symbol;
    
    std::string get_full_scope();//returns absolute scope
    
    //in this or parent scope.
    bool id_exists(std::string) const;
    
    bool byte_exists(std::string) const;
    bool dual_exists(std::string) const;
    bool quad_exists(std::string) const;
    bool oct_exists(std::string) const;
    bool struct_exists(std::string) const;
    bool function_id_exists(std::string) const;
    bool operator_exists(std::string) const;
    bool syntax_exists(std::string) const;
    bool label_exists(std::string) const;
    bool type_exists(std::string) const;
    
    bool scope_exists(std::string find_me) const;
    
    std::vector<symbol> get_symbol(std::string);
    symbol_table& get_subscope(std::string);
    symbol_table& get_subscope(std::string,std::vector<std::string>);
    
    std::vector<symbol> get_ops() const;
    
    bool types_equal(const AST_node* const a, const AST_node* const b) const;
    
    AST_node* get_type(const AST_node* const typeMe) const;
    AST_node* get_type_of_if(const AST_node* const typeMe);
    AST_node* get_type_of_for(const AST_node* const typeMe);
    AST_node* get_type_of_while(const AST_node* const typeMe);
    AST_node* get_type_of_return(const AST_node* const typeMe);
    AST_node* get_type_of_function_decl(const AST_node* const typeMe);
    AST_node* get_type_of_operator_decl(const AST_node* const typeMe);
    AST_node* get_type_of_syntax_decl(const AST_node* const typeMe);
    AST_node* get_type_of_function_call(const AST_node* const typeMe);
    AST_node* get_type_of_operator_call(const AST_node* const typeMe);
    AST_node* get_type_of_syntax_call(const AST_node* const typeMe);
    AST_node* get_type_of_id(const AST_node* const typeMe);
    AST_node* get_type_of_decl(const AST_node* const typeMe);
    AST_node* get_type_of_parenthesis(const AST_node* const typeMe);
    AST_node* get_type_of_curly_bracket(const AST_node* const typeMe);
    
    void add_symbol(symbol);
    void remove_symbol(symbol);
    symbol_table* add_scope(std::string scope, scope_type T, AST_node* node);
    
    void print();
    
};

#endif//SYMBOL_TABLE_H
