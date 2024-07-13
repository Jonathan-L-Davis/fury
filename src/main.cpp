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
std::vector<token> lex( const std::string &file_name );
int main(int argc, char**argv){
    
    auto tokens = lex("./file.txt");
    int start_pos = 0;
    
    AST_node parsed_file = parse(tokens,start_pos);
    parsed_file.print();
    
    std::cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << std::endl;
    
    symbol_table sym_tbl = anal(parsed_file);
    sym_tbl.print();
    
    /*for( symbol sym : sym_tbl.symbols ){
        std::cout << "|" << sym.name << "|\n";
    }//*/
    
    return 0;
}
