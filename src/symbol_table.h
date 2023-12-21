#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>

#include "parse.h"

enum _type:uint32_t{
    semantic_epsilon = 0,//void
    semantic_function = 1,//includes operators,
    semantic_byte = 2,
    semantic_dual = 3,
    semantic_quad = 4,
    semantic_oct = 5,
    semantic_struct = 6,//includes unions
    semantic_type = 7,// type is type? generic? not used for the first version.
};

struct symbol{
    std::string type;
    std::string name;
    std::string value;
};

struct symbol_table{

    symbol_table* parent; //non owning

    symbol_table();
    symbol_table( symbol_table*, std::string scope );

    std::string scope;
    std::vector<symbol_table> sub_scopes;
    std::vector<symbol> symbols;//may need to be more sophisticated than a string

    std::string get_full_scope();//returns absolute scope

    void add_symbol(symbol);
    void add_scope(std::string scope);

};

// intentionally not const
void anal( AST_node& frisk_me, symbol_table& fill_me );

#endif//SYMBOL_TABLE_H