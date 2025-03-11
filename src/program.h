#ifndef PROGRAM_H
#define PROGRAM_H

#include "symbol_table.h"
#include "parse.h"

// This struct does not necessarily represent a fully completed compileable & executable program, merely a program fragment.
struct program{
    symbol_table the_context;
    AST_node root;
};

#endif//PROGRAM_H

