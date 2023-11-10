#include <iostream>

#include "token.h"
#include "parse.h"
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
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping}
                                        };
;

std::vector<token> example_2 = { {"if",0,keyword},{"(",0,paren},{"0",0,literal},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        {"else",0,keyword},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_3 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_4 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{"byte",0,type},{"a",0,identifier},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_5 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{"byte",0,type},{"a",0,identifier},{",",0,Operator},{"byte",0,type},{"b",0,identifier},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_6 = { {"function",0,keyword},{"byte",0,type},{"func_name",0,identifier},
                                        {"(",0,paren},{"byte",0,type},{"a",0,identifier},{",",0,Operator},{"byte",0,type},{"b",0,identifier},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"return",0,keyword},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping},
                                        };
;

std::vector<token> example_7 = { {"while",0,keyword},{"(",0,paren},{"0",0,literal},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping}
                                        };
;

std::vector<token> example_8 = { {"for",0,keyword},{"(",0,paren},{"a",0,identifier},{";",0,Operator},{"a",0,identifier},{";",0,Operator},{"a",0,identifier},{";",0,Operator},{")",0,paren},
                                        {"{",0,scoping},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator},
                                        {"}",0,scoping}
                                        };
;

bool is_valid_literal( std::string str );
bool is_hex(uint8_t c);

int main(int argc, char**argv){

    //auto tokens = tokenize("./file2.txt");

    std::string type;



    int start_pos = 0;
    AST_node ex1 = parse_expression(example_1,start_pos);
    ex1.print();

    start_pos = 0;
    AST_node ex2 = parse_expression(example_2,start_pos);
    ex2.print();

    start_pos = 0;
    AST_node ex3 = parse_expression(example_3,start_pos);
    ex3.print();

    start_pos = 0;
    AST_node ex4 = parse_expression(example_4,start_pos);
    ex4.print();

    start_pos = 0;
    AST_node ex5 = parse_expression(example_5,start_pos);
    ex5.print();

    start_pos = 0;
    AST_node ex6 = parse_expression(example_6,start_pos);
    ex6.print();

    start_pos = 0;
    AST_node ex7 = parse_expression(example_7,start_pos);
    ex7.print();

    start_pos = 0;
    AST_node ex8 = parse_expression(example_8,start_pos);
    ex8.print();



    /*
    std::string hex_test = "0123456789ABCDEF";
    for( int i = 0; i < 16; i++ ){
        std::cout << is_hex(hex_test[i]) << "\n";
    }//*/

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


    /*
    for( auto token : tokens ){

        switch( token.type ){
            case epsilon:{
                type = "epsilon";
            }break;
            case keyword:{
                type = "keyword";
            }break;
            case Operator:{
                type = "operator";
            }break;
            case token_type::type:{
                type = "type";
            }break;
            case identifier:{
                type = "identifier";
            }break;
            case parser:{
                type = "parser";
            }break;
        }

        std::cout << token.line_no << " : \"" << token.text << "\" : " << type << "\n";
    }//*/
}
