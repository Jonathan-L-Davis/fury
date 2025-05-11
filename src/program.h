#ifndef PROGRAM_H
#define PROGRAM_H

#include "symbol_table.h"
#include "node.h"

// This struct does not necessarily represent a fully completed compilable & executable program, merely a program fragment.
struct program{
    std::string file_name;
    symbol_table context;
    AST_node root;
};

#endif//PROGRAM_H

