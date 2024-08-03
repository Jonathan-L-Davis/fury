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
}

AST_node something_new(std::string,symbol_table);

int main(int argc, char**argv){
    
    symbol_table the_context;
    
    AST_node something_new("./function.fury",the_context);
    
    return 0;
    
    auto tokens = lex("./the_context.txt");
    int start_pos = 0;
    
    for(token tok : tokens){
        std::cout << token_type_to_string(tok.type) << ":" << tok.text << std::endl;
    }
    
    std::cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
    
    //std::cout << operators.size() << "|" << keywords.size() << "|" << types.size() << std::endl;
    
    AST_node parsed_file;
    try{
    parsed_file = parse(tokens,start_pos);
    }catch(char f){}
    parsed_file.print();
    
    std::cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
    
    parsed_file.print_with_types();
    
    
    symbol_table sym_tbl = anal(parsed_file);
    sym_tbl.print();
    
    return 0;
}
