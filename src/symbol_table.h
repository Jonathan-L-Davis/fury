#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>

struct symbol_table{
    std::string scope;
    std::vector<symbol_table> nested_scope;
    std::vector<std::string> ids;//may need to be more sophisticated than a string
};

#endif//SYMBOL_TABLE_H