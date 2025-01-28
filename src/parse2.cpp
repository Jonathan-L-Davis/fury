#include <string>
#include <fstream>
#include <iostream>

#include "lex.h"
#include "parse.h"
#include "symbol_table.h"
#include <vector>
#include <assert.h>

AST_node parse2(std::string file_name, symbol_table& the_context){
    AST_node retMe = {{":",0,root},{}};// should be the only node/token with a line_no of 0. 
    AST_node* currentNode = &retMe;
    std::fstream file(file_name);
    
    std::vector<uint8_t> file_buffer;
    std::vector<token> tokenPool = {};
    std::vector<AST_node> nodeStack = {};
    
    uint8_t q;
    while(file.read((char*)&q,1)){
        file_buffer.push_back(q);
    }
    
    uint32_t line_no = 1;
    token currentToken;
    for(uint8_t byte : file_buffer){
        
        // construct tokens
        std::cout << "|" << currentNode->tok.text << "|" << currentNode->tok.type << std::endl;
        if( currentToken.text == "" ) currentToken.line_no = line_no;
        if( byte == '\n' ) line_no++;
        
        if( byte == ' ' || byte == '\n' ){// if white space, only assuming this one kind of ws for now.
            if( currentToken.text != ""){
                if( currentToken.text == "function" ) currentToken.type = keyword; else assert(false);
                tokenPool.push_back(currentToken);
                std::cout << currentToken.text << std::endl;
                currentToken = token();
                goto end_token_construction;
            }
        }
        else if( currentNode->tok.text == "function" ){
            assert( currentNode->tok.type == keyword );
            
            std::cout << currentToken.text << std::endl;
            if( currentNode->children.size() == 0 ){// looking for type
                std::string byte_str;byte_str.push_back(byte);
                if( !the_context.type_starts_with_substr(currentToken.text+byte_str ) ){// no known type, means we have a new id
                    if(!is_alpha_numeric_or_(byte)){
                        currentToken.type = identifier;
                        tokenPool.push_back(currentToken);
                        std::cout << currentToken.text << std::endl;
                        currentToken = token();
                        currentToken.text += byte;
                        goto end_token_construction;
                    }
                    
                    
                    
                }else if( the_context.id_starts_with_substr(currentToken.text ) ){// we have either a complete type or a new id, if no type
                    
                    assert(false);// id or type, dunno
                    
                }
            }else assert(false);
        }
        currentToken.text += byte;
        
        end_token_construction:;
        // construct AST_nodes
        if(tokenPool.size()==0) continue;std::cout << "handling tokens\n";
        std::cout << tokenPool[0].text << std::endl;
        if(tokenPool[0].text == "function"){
            nodeStack.push_back({tokenPool[0],{}});
            tokenPool.erase(tokenPool.begin());
        }
        
        // construct AST's
        
        if(nodeStack.size()==0) continue;std::cout << "handling AST_nodes\n";
        std::cout << nodeStack[0].tok.text << std::endl;
        if(nodeStack[0].tok.text == "function"){
            std::cout << "pqzy\n";
            currentNode->children.push_back(nodeStack[0]);
            currentNode = &currentNode->children[currentNode->children.size()-1];
            nodeStack.erase(nodeStack.begin());
        }
    }
    std::cout << currentToken.text << std::endl;
    
    return retMe;
}

