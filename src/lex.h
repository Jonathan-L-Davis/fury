#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <stdint.h>
#include <vector>


//only keywords should exist. The others have to be defined by default in the symbol table.
extern std::vector<std::string> keywords;

extern std::vector<std::string> operators;

extern std::vector<std::string> types;

/**    HELPER FUNCTIONS    **/

bool in_range(uint8_t c);
// A-Za-z
bool alpha(uint8_t c);
// _ and A-Za-z
bool _alpha(uint8_t c);
// 0-9
bool numeric(uint8_t c);
// 0-F
bool is_hex(uint8_t c);
// 0-1
bool is_bin(uint8_t c);
// snake case for alpha numeric, no _
bool alpha_numeric(uint8_t c);
// for _ A-Za-z0-9
bool is_alpha_numeric_or_(uint8_t c);
bool is_valid_identifier( std::string str );
bool is_keyword( std::string str );
bool is_keyword_type( std::string str );
bool is_operator( std::string str );
bool is_binary_operator( std::string str );
bool is_type( std::string str );
bool is_valid_literal( std::string str );
bool is_scoping( std::string str );
bool is_paren( std::string str );
bool is_ws( char am_i_space );

#endif//TOKEN_H
