/**

How on earth is the following code turned into an AST?

```
function f(){
    f();
};

```

need a new context stack.
need context.
need default context.
this function doesn't need me to have a default context.
this function doesn't need me to have built in types (yet).
probably the simplest code I could make compile first.


functions and comptime let me do generics.
not really comfortable with that idea yet, but it's the right way to do it.

```
function function T identity(T) (type T){
    
    return function T f(T retMe){return retMe; };
    
};
```

interestingly the type T is not defined until after it is used. Must be careful to make sure it's defined immediately in the parameter bracket.
probably need some comptime to make sure it works out. but not sure where to make comptime.

```
function function T identity(T) ( comptime type T){
    
    return function T f(T retMe){return retMe; };
    
};
```
this forces the type T to be set at compile time when the function is called.

generics will come later.

back to the original code.

the "function" keyword is read in byte by byte, then recognized as a keyword.
we then enter a "function parsing context" expecting the following tokens to define a function.

how do I parse the original code when the following is allowed?

```
function type g(type T){
    return T;
};

function g(byte) f(){
    g(byte) retMe = 0;
    return retMe;
};

function g(g(byte)) h(){
    g(g(byte)) retMe = 0;
    return retMe;
};

```

can't do anything with runtime generics yet, but also can't parse this with assumptions on parenthesis showing up in certain places.
we can have arbitrary parenthesis before we get to the parameter declarations. and I don't want to do backtracking ever at all.

ignoring that lexing problem, looking at tokens we see
```
function f ( ) { f ( ) ; } ;
```

the following is equivalent in fury:

```
function f ( ) f ( ) ;
```

f needs to be added to the symbol table right after the parenthesis in the definition. as seen below.

```
function f ( ) <add f() to symbol table here> f ( ) ;
```

this means that parsing happens at the same time as semantic analysis.
**/



#include "lex.h"
#include "parse.h"
#include "symbol_table.h"

#include <fstream>
#include <assert.h>
#include <iostream>

AST_node something_new(std::string file_name,symbol_table context){
    AST_node retMe;
    std::vector<token> tokens;

    std::fstream file(file_name,std::ios::in);
    
    file.seekg(0,std::ios::end);
    uint64_t file_size = file.tellg();
    file.seekg(0,std::ios::beg);
    
    token current_token;
    for( uint64_t i = 0; i < file_size; i++ ){
        uint8_t byte;
        file.read((char*)&byte,1);
        
        if( !in_range(byte) && !is_ws(byte) ) {
            std::cout << "ascii character \"" << char(byte) << "\" is out of acceptable range ' ' to '~' or in hex \"0x" << std::hex << int(byte) << "\" \"0x20 to 0x7E\" and is not white space." << std::endl;
            assert( false );
        };
        
        
        
        if( is_keyword( current_token.text ) && !context.id_starts_with_substr( current_token.text+std::string(1,byte) ) ){
            
            if( current_token.text.size() > 0 ){
                current_token.type = get_token_type( current_token.text );
                tokens.push_back( current_token );
                current_token = token();
            }
        }
        
        if( is_ws( byte ) ){
            
            if( current_token.text.size() > 0 ){
                current_token.type = get_token_type( current_token.text );
                tokens.push_back( current_token );
                current_token = token();
            }
            
            continue;
        }
        
        if( byte == ';' ){
            
            if( current_token.text.size() > 0 ){
                current_token.type = get_token_type( current_token.text );
                tokens.push_back( current_token );
                current_token = token();
            }
            
            current_token.text += byte;
            
            tokens.push_back(current_token);
            current_token = token();
            continue;
            
        }
        current_token.text += byte;
        
        
    }
    
    if( current_token.text.size() > 0 ){
        current_token.type = get_token_type( current_token.text );
        tokens.push_back( current_token );
        current_token = token();
    }
    
    
    return retMe;
    
}
