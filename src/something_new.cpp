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

how do you get the inner function?


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
    AST_node retMe = { { std::string("\0root",5), 0, root }, {} };
    std::vector<token> tokens;
    std::vector<AST_node*> node_stack = {&retMe};
    AST_node *current_node = &retMe;
    
    auto pick_off_token = [&](){
        
        // might not be used yet, will manually do this a few times, and see if it's meaningful to try to make into a function. Might be different each time.
        
    };
    
    // deliberately not using vector to force file_buffer max size. Want to crash on large inputs right now. Obviously not a final feature, just bounds the complexity I have to take on.
    uint8_t file_buffer[1024*1024];
    
    std::fstream file(file_name,std::ios::in);
    
    file.seekg(0,std::ios::end);
    uint64_t file_size = file.tellg();
    file.seekg(0,std::ios::beg);
    
    assert( file_size <= 1024*1024 );
    
    for( uint64_t i = 0; i < file_size; i++ ){
        file.read((char*)&file_buffer[i],1);
    }
    
    token current_token;
    uint64_t line_no = 1;
    for( uint64_t i = 0; i < file_size; i++ ){
        current_token.text += file_buffer[i];
        
        // check if current token + next char is a valid identifier/start of identifier.
        if( (i+1 < file_size) && context.id_starts_with_substr( current_token.text + (char) file_buffer[i] ) ){
            assert(false);//obviously should be handled, but not yet.
        }
        
        // else check for keyword.
        if( is_keyword(current_token.text) ){
            if( current_token.text == "function" ){
                
                current_token.type = keyword;
                current_token.line_no = line_no;
                
                current_node->children.push_back({current_token,{}});
                
                current_token = token();
            }
            current_token = token();
            continue;
        }
        
        // else check for identifier
        if( context.id_exists( current_token.text ) ){
            assert(false);
        }
        
        // else check for white space. I hope whitespace never makes it into a token identifier. Would be ass. Should be fine in strings, but eh, we'll see. Single character only should help with that.
        // whitespace doesn't generate tokens directly so nothing needs to be updated in the AST.
        if( current_token.text.size() == 1 && is_ws( current_token.text[0] ) ){
            if(current_token.text[0]=='\n')line_no++;
            current_token = token();
            continue;
        }
        
    }
    
    if( current_token.text.size() > 0 ){
        current_token.type = get_token_type( current_token.text );
        tokens.push_back( current_token );
        current_token = token();
    }
    
    
    return retMe;
    
}

