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

std::vector<token> example_1 = { {"if",0,keyword},{"(",0,paren},{"0",0,literal},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping}
                                        };
;

std::vector<token> example_2 = { {"if",0,keyword},{"(",0,paren},{"0",0,literal},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        {"else",0,keyword},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_3 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_4 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{"byte",0,type},{"a",0,identifier},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_5 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{"byte",0,type},{"a",0,identifier},{",",0,Operator},{"byte",0,type},{"b",0,identifier},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_6 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{"byte",0,type},{"a",0,identifier},{",",0,Operator},{"byte",0,type},{"b",0,identifier},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"return",0,keyword},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_7 = { {"while",0,keyword},{"(",0,paren},{"0",0,literal},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping}
                                        };
;

std::vector<token> example_8 = { {"for",0,keyword},{"(",0,paren},{"a",0,identifier},{";",0,Operator},{"a",0,identifier},{";",0,Operator},{"a",0,identifier},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping}
                                        };
;

std::vector<token> example_9 = { {"for",0,keyword},{"(",0,paren},{"0",0,literal},{";",0,Operator},{"0",0,literal},{";",0,Operator},{"0",0,literal},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,binary_operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping}
                                        };
;

bool is_valid_literal( std::string str );
bool is_hex(uint8_t c);
std::vector<token> lex( const std::string &file_name );
int main(int argc, char**argv){
    
    auto tokens = lex("./file.txt");
    int start_pos = 0;
    
    AST_node parsed_file = parse(tokens,start_pos);
    parsed_file.print();
    
    symbol_table sym_tbl = anal(parsed_file);
    sym_tbl.print();
    
    /*for( symbol sym : sym_tbl.symbols ){
        std::cout << "|" << sym.name << "|\n";
    }//*/
    
    return 0;
    // these checks are important, but should be moved from here.
    assert( is_valid_literal( "0" ) );
    assert( is_valid_literal( "0x0123456789ABCDEF" ) );
    assert( is_valid_literal( "0b010101010101010101" ) );
    assert(!is_valid_literal( "0b0120120101201010101010" ) );
    assert( is_valid_literal( "0b01111111111111111" ) );
    assert(!is_valid_literal( "0xFEDCbA9876543210" ) );
    assert( is_valid_literal( "0x0123" ) );
    assert( is_valid_literal( "0x0098765" ) );
    assert( is_valid_literal( "0103948573098457" ) );
    assert(!is_valid_literal( "o5iu234pio5u3" ) );
    assert(!is_valid_literal( "n" ) );
    assert(!is_valid_literal( "ten" ) );
    assert( is_valid_literal( "9876543210" ) );
    assert( is_valid_literal( "0123456789" ) );
    assert( is_valid_literal( "10000000000000000000" ) );
}
