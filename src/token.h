#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <stdint.h>
#include <vector>

extern std::vector<std::string> keywords;

extern std::vector<std::string> operators;

extern std::vector<std::string> types;

enum token_type:uint32_t{
    null = 0,
    keyword = 1,
    Operator = 2,
    type = 3,
    identifier = 4,
};

struct token{
    std::string text;
    uint32_t line_no;
    token_type type;
};

std::vector<token> tokenize( const std::string &filename );

token_type get_token_type( std::string type_me );

#endif//TOKEN_H
