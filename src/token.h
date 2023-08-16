#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <stdint.h>
#include <vector>

enum token_type:uint32_t{
    unknown = 0,
    identifier = 1,
};

struct token{
    std::string text;
    uint32_t line_no;
    token_type type;
};

std::vector<token> tokenize( const std::string &filename );

#endif//TOKEN_H
