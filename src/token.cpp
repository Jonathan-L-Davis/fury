#include "token.h"
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
    "goto",//not used? meh
    "if",
    "import",
    "oct",
    "quad",
    "return",
    "struct",
    "union",
    "while",
};

std::vector<std::string> operators
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
bool _alpha_numeric(uint8_t c){
    return _alpha(c) || numeric(c);
}

bool is_valid_identifier( std::string str ){
    if( str.size() == 0 ) return false;//epsilon string check

    if( !_alpha(str[0]) ) return false;

    for( unsigned int i = 1; i < str.size(); i++ ){
        if( !_alpha_numeric(str[0]) ) return false;
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

bool ws( char am_i_space ){//common whitespaces
    return ( am_i_space == ' ' || ( 0x7 <= am_i_space && am_i_space <= 0xD ) );
}

std::vector<token> tokenize2temp( const std::string &file_name ){
    std::vector<token> retMe;

    std::fstream file(file_name,std::ios::in);

    char current_character = 0;
    token current_token;
    bool buffer, extra_loop = 0;//extra loop grabs last character
    for( int line_no = 1; (buffer = (bool) file.get(current_character) ) || extra_loop ; extra_loop = buffer ){
    std::cout << line_no << " ";
    std::cout << " " << extra_loop << " " << buffer << "\n";
        current_token.line_no = line_no;
        if( ws(current_character) || !buffer ){// if on ws or last character push token
            if( current_token.text.size() > 0 )
                retMe.push_back(current_token);
            current_token = {};
            if( current_character == '\n' )
                line_no++;
        }
        else{
            current_token.text += current_character;
        }

        if( false )//triggers on ws and on end of file
            retMe.push_back(current_token);
    }

    return retMe;
}

std::vector<token> tokenize( const std::string &file_name ){

    std::vector<token> retMe;

    std::fstream file(file_name,std::ios::in);

    std::string line;
    int line_depth = 0;
    int line_no = 1;

    auto next_token = [&line,&line_depth]{
        if(line == "")//avoid epsilon string issues
            return std::string("");

        if( std::isspace(line[line_depth]) ){//advance until first non-ws
            while( (unsigned)line_depth < line.length() && std::isspace(line[line_depth]) )
                line_depth++;
        }

        int start_depth = line_depth;

        if( (unsigned)line_depth >= line.length() )//ignore ws that terminates a line
            return std::string("");

        //grab all non-ws chars
        while( (unsigned)line_depth < line.length() && !std::isspace(line[line_depth]) )
            line_depth++;

        return line.substr(start_depth,line_depth-start_depth);
    };

    while( std::getline( file, line) ){//line by line
        for( line_depth = 0; (unsigned)line_depth < line.length();  ){//tokenize only via ws breaks
            auto text = next_token();
            token tok = {text,(unsigned)line_no,get_token_type(text)};
            if(tok.text.size()>0 ) {//don't push empty tokens, last token generated by a given line is a 'epsilon' token
                retMe.push_back(tok);
            }
            std::cout << tok.text.size() << "|\n";
        }
        line_no++;
    }

    return retMe;
}

// not all types are used, at this point built in types are only recognized as keywords
token_type get_token_type( std::string type_me ){
    if( type_me == "" )
        return epsilon;
    if( is_keyword( type_me ) )
        return keyword;
    if( is_operator( type_me ) )
        return Operator;
    if( is_valid_identifier( type_me ) )
        return identifier;
    if( is_valid_literal( type_me ) )
        return literal;
    if( is_scoping( type_me ) )
        return paren;
    return error;
}
