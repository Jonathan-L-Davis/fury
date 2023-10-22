#include <iostream>

#include "token.h"
#include "parse.h"
#include <vector>
#include <assert.h>

std::vector<token> example_1 = { {"if",0,keyword},{"(",0,enclosing},{"0",0,literal},{")",0,enclosing},
                                        {"{",0,enclosing},
                                        {"a",0,identifier},{"=",0,Operator},{"b",0,identifier},{";",0,Operator}
                                        ,{"}",0,enclosing}
                                        };

bool is_valid_literal( std::string str );
bool is_hex(uint8_t c);

int main(int argc, char**argv){

    auto tokens = tokenize("./file2.txt");

    std::string type;



    int start_pos = 0;
    AST_node ex1 = parse_expression(example_1,start_pos);
    ex1.print();
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

    std::cout << ex1.children.size() << "\n";

    /*for( auto token : tokens ){

        switch( token.type ){
            case null:{
                type = "null";
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
