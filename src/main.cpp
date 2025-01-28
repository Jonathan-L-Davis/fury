#include <iostream>

#include "lex.h"
#include "parse.h"
#include "symbol_table.h"
#include <vector>
#include <assert.h>

/*
    the example vectors are just token streams,
    tokenizer needs to be rewritten to produce these,
    had a lot of lang design to figure out before these could be written appropriately
    will probably change again anyway
//*/

bool is_valid_literal( std::string str );
bool is_hex(uint8_t c);

std::string token_type_to_string(uint32_t convertMe){
    switch(convertMe){
        case epsilon        : return "epsilon        ";
        case keyword        : return "keyword        ";
        case Operator       : return "Operator       ";
        case type           : return "type           ";
        case identifier     : return "identifier     ";
        case parser         : return "parser         ";
        case scoping        : return "scoping        ";
        case literal        : return "literal        ";
        case error          : return "error          ";
        case root           : return "root           ";
        case sigma          : return "sigma          ";
        case paren          : return "paren          ";
        case binary_operator: return "binary_operator";
    }
    assert(false);//yuh
}

//AST_node something_new(std::string,symbol_table);
AST_node parse2(std::string,symbol_table&);

int main(int argc, char**argv){
    
    symbol_table the_context;
    
    AST_node node = parse2("function.fury",the_context);//something_new("./function.fury",the_context);
    
    node.print();
    
    return 0;
}
