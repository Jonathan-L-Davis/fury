#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <stdint.h>
#include <vector>


//only keywords should exist. The others have to be defined by default in the symbol table.
extern std::vector<std::string> keywords;

extern std::vector<std::string> operators;

extern std::vector<std::string> types;

enum token_type:uint32_t{
    epsilon = 0,
    keyword = 1,
    Operator = 2,
    type = 3,
    identifier = 4,
    parser = 5,
    scoping = 6,
    literal = 7,
    error = 8,
    root = 9,
    sigma = 10,
    paren = 11,
    binary_operator = 12,
};

struct token{
    std::string text;
    uint32_t line_no;
    token_type type;
};

std::vector<token> lex( const std::string &filename );

token_type get_token_type( std::string type_me );

#endif//TOKEN_H
