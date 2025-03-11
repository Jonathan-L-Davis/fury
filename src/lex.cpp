#include "lex.h"
#include <fstream>
#include <iostream>
#include <algorithm>

#include <assert.h>

std::vector<std::string> keywords
{
    "byte",
    "dual",
    "else",
    "export",
    "for",
    "function",
    "goto",
    "if",
    "import",
    "label"
    "namespace",
    "oct",
    "operator",
    "quad",
    "return",
    "struct",
    "type",
    "while",
};

std::vector<std::string> operators
{
    "+",//arithmetic
    "-",// subtraction and negation
    "*",
    "/",
    "%",
    "=",

    ">>",//bitwise
    "<<",
    ">O>",
    "<O<",
    "&",
    "|",
    "~",
    "^",

    "==",//comparison
    "!=",
    ">",
    "<",
    ">=",
    "<=",

    ",",//sub-expression separator
    ";",//expression terminal

    "::",//scoping, add later
    ".",//member access & scoping current
    "..",//scoping backstep
};

//binary operators
std::vector<std::string> boperators
{
    "+",//arithmetic
    "-",
    "*",
    "/",
    "%",
    "=",

    ">>",//bitwise
    "<<",
    ">O>",
    "<O<",
    "&",
    "|",
    "^",

    "==",//comparison
    "!=",
    ">",
    "<",
    ">=",
    "<=",

    ",",//sub-expression separator - probably not adding to the language.

    /// Can be added later. Don't need to add for the first version.
    "::",//scoping, add later
    ".",//member access & scoping current
    "..",//scoping backstep
};

//not used now, may be moved to parsing code
//I'm still learning how a compiler should work
std::vector<std::string> types =
{
    "byte",
    "dual",
    "quad",
    "oct",

    "struct",
    "union",
};

bool in_range(uint8_t c){
    return c >= ' ' && c <= '~';
}

// A-Za-z
bool alpha(uint8_t c){
    if( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') )
        return true;
    return false;
}
// _ and A-Za-z
bool _alpha(uint8_t c){
    if( alpha(c) || c == '_' )
        return true;
    return false;
}

// 0-9
bool numeric(uint8_t c){
    if( c >= '0' && c <= '9' )
        return true;
    return false;
}
// 0-F
bool is_hex(uint8_t c){
    if( (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') )// || (c >= 'a' && c <= 'f') )// no support for a-f lower case, that's just ugly hex
        return true;
    return false;
}

// 0-1
bool is_bin(uint8_t c){
    if( c >= '0' && c <= '1' )
        return true;
    return false;
}

// snake case for alpha numeric, no _
bool alpha_numeric(uint8_t c){
    return alpha(c) || numeric(c);
}

// for _ A-Za-z0-9
bool is_alpha_numeric_or_(uint8_t c){
    return _alpha(c) || numeric(c);
}

bool is_valid_identifier( std::string str ){
    if( str.size() == 0 ) return false;//epsilon string check

    if( !_alpha(str[0]) ) return false;

    for( unsigned int i = 1; i < str.size(); i++ ){
        if( !is_alpha_numeric_or_(str[0]) ) return false;
    }

    return true;
}

bool is_keyword( std::string str ){
    if( std::find(keywords.begin(), keywords.end(), str ) != keywords.end() )
        return true;
    return false;
}

bool is_operator( std::string str ){
    if( std::find(operators.begin(), operators.end(), str ) != operators.end() )
        return true;
    return false;
}

bool is_binary_operator( std::string str ){
    if( std::find(boperators.begin(), boperators.end(), str ) != boperators.end() )
        return true;
    return false;
}

bool is_type( std::string str ){
    if( std::find(types.begin(), types.end(), str ) != types.end() )
        return true;
    return false;
}

bool is_valid_literal( std::string str ){
    if( str.size() == 0 ) return false;

    unsigned int pos = 0;

    if( numeric(str[pos]) );
    else return false;// can only contain numeric chars at this point.
    pos++;



    bool hex = false;
    bool bin = false;
    if( str[0] == '0' ){
        if( str.size() > 1 ){//check if second char is x or b
                 if( str[1] == 'b' )
                bin = true;
            else if( str[1] == 'x' ){
                hex = true;
            }else if( !hex && !bin && !numeric(str[1]) )
                return false;// reject all literals that aren't bin, hex, or numeric on second char;

        }
        else//only one numeric character
            return true;//is valid
    }

    pos++;
    assert( pos == 2 );

    if(hex){
        if( str.size() < 3 ) return false;// reject "0x"
        for( ; pos < str.size(); pos++ )
            if( !is_hex( str[pos] ) )
                return false;
        return true;
    }else
    if(bin){
        if( str.size() < 3 ) return false;// reject "0x"
        for( ; pos < str.size(); pos++ )
            if( !is_bin( str[pos] ) )
                return false;
        return true;
    }else{
        for( ; pos < str.size(); pos++ )
            if( !numeric( str[pos] ) )
                return false;
        return true;
    }

    assert(false);//you should never reach the bottom of this function, you should be caught in the preceding if else structure.
}

bool is_scoping( std::string str ){
    if( str == "{" || str == "}" )
       return true;
    return false;
}

bool is_paren( std::string str ){
    if( str == "(" || str == ")" )
       return true;
    return false;
}

bool is_ws( char am_i_space ){//common whitespaces
    return ( am_i_space == ' ' || ( 0x9 <= am_i_space && am_i_space <= 0xD ) );
}

std::vector<token> lex( const std::string &file_name ){
    std::vector<token> retMe;

    std::fstream file(file_name,std::ios::in);
    
    //*
    char current_character = 0;
    token current_token;
    current_token.line_no = 1;
    bool buffer, extra_loop = 0;//extra loop grabs last character
    for( int line_no = 1; (buffer = (bool) file.get(current_character) ) || extra_loop ; extra_loop = buffer ){
        //std::cout << current_character << "\n";

        //find operators as substrings, since they aren't always separated by ws, like "type varname = id;"
        current_token.line_no = line_no;
        int i = 0;
        for( std::string s : operators ){
            std::string::size_type pos;
            if( (pos = current_token.text.find(s) ) != std::string::npos ){i++;

                token tmp;
                tmp.line_no = line_no;
                tmp.text = current_token.text.substr( 0, pos );
                tmp.type = get_token_type( tmp.text );

                retMe.push_back(tmp);

                tmp.line_no = line_no;
                tmp.text = current_token.text.substr( pos, pos + s.size() );
                tmp.type = get_token_type( tmp.text );

                retMe.push_back(tmp);

                current_token.text = current_token.text.substr( pos + s.size(), current_token.text.size() );

                break;//assuming that there is only one nested per main loop, don't want to think about more than one occuring
            }
        }assert( i <= 1 );

        if( is_ws(current_character) || !buffer ){// if on ws or last character push token
            if( current_token.text.size() > 0 )
                retMe.push_back(current_token);
            current_token = {};
            if( current_character == '\n' ){
                line_no++;
                current_token.line_no = line_no;
            }
        }else{//this tokenization model will be broken later, badly

            //affect normal lexing
            if( current_character == '{' or current_character == '}' ){
                if( current_token.text.size() > 0 ){
                    retMe.push_back(current_token);
                    current_token = {};
                    current_token.line_no = line_no;
                }
                current_token.type = scoping;
                current_token.text += current_character;
                retMe.push_back(current_token);
                current_token = {};
                current_token.line_no = line_no;
                continue;
            }

            if( current_character == '(' or current_character == ')' ){
                if( current_token.text.size() > 0 ){
                    retMe.push_back(current_token);
                    current_token = {};
                }
                current_token.type = paren;
                current_token.text += current_character;
                retMe.push_back(current_token);
                current_token = {};
                current_token.line_no = line_no;
                continue;
            }

            current_token.text += current_character;

            if( is_keyword( current_token.text ) ){
                current_token.type = get_token_type( current_token.text );
                retMe.push_back(current_token);
                current_token = {};
                current_token.line_no = line_no;
            }else
            if( is_operator( current_token.text ) ){//ternary for binary vs other operators, affects parsing, lot of work left to do with operators
                current_token.type = is_binary_operator(current_token.text)?binary_operator:Operator;
                retMe.push_back(current_token);
                current_token = {};
                current_token.line_no = line_no;
            }else
            if( is_valid_identifier( current_token.text ) ){
                current_token.type = identifier;
            }else
            if( is_valid_literal( current_token.text ) ){
                current_token.type = literal;
            }
            else current_token.type = epsilon;

        }
    }

    return retMe;//*/
}

// not all types are used, at this point built in types are only recognized as keywords
token_type get_token_type( std::string type_me ){
    if( type_me == "" )
        return epsilon;
    if( is_type( type_me ) )
        return type;
    if( is_keyword( type_me ) )
        return keyword;
    //if( is_binary_operator( type_me ) )
    //    return binary_operator;
    if( is_operator( type_me ) )
        return identifier;
    if( is_valid_identifier( type_me ) )
        return identifier;
    if( is_valid_literal( type_me ) )
        return literal;
    if( is_scoping( type_me ) )
        return scoping;
    if( is_paren(type_me ) )
        return paren;
    return error;
}
