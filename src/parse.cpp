#include <string>
#include <fstream>
#include <iostream>

#include "parse.h"
#include "parse_util.h"
#include "symbol_table.h"
#include "program.h"

#include <vector>
#include <assert.h>

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
};

program parse(std::string file_name){
    program retMe;
    retMe.root = {{":--:",0,root},{}};// should be the only node/token with a line_no of 0.
    std::fstream file(file_name);// If I cared about performance I could at a minimum preallocate the number of bytes in the file.
    
    std::vector<uint8_t> file_buffer;
    std::vector<AST_node*> nodePool = {};// for constucting nodes without forcing them into a tree immediately
    
    uint8_t q;
    while(file.read((char*)&q,1)){
        file_buffer.push_back(q);//should obviously preallocate the size in one go & 1 memcpy.
    }
    
    std::cout << "Parsing file: function.fury\n";
    std::cout << "----------------------------------------------------------------------------------\n";
    for( auto c : file_buffer ) std::cout << c;
    uint32_t line_no = 1;
    std::string completeToken;
    std::string currentToken;
    
    for(uint8_t byte : file_buffer){
        
        if( !( byte=='\n' || (byte>=' ' && byte<127) ) ){// fuck your \r -- but seriously, the 127 restriction is to allow for forward compatibility with utf-8, will do 'proper' character handling eventually. If you're reading this, please take a moment of silence fore the Unicode Consortium, our unsung heroes.
            std::cout << (int) byte << std::endl;
            assert(byte=='\n' || ( byte>=' ' && byte<127) );
        }
        
        if(retMe.the_context.syntax_exists(currentToken+(char)byte)){
            std::cout << "Syntax execution is not supported yet.\n";
            std::exit(-1);
        }
        
        if(byte == ' ' || byte == '\n'){
            completeToken = currentToken;
            currentToken = "";
            
            if(byte == '\n') line_no++;
        }else if(byte=='('||byte==')'||byte=='{'||byte=='}'||byte==','||byte==';'){// characters are forced to be single character tokens.
            if(currentToken!=""){
                AST_node* n = new AST_node;
                *n = {currentToken,line_no,node_t::id};
                nodePool.push_back(n);// not a general solution. Need to fix.
                currentToken="";
            }
            AST_node* n = new AST_node;
            node_t node_type = ( (byte=='('||byte==')') ? node_t::paren : (byte=='{'||byte=='}') ? node_t::curly : (byte==',') ? node_t::comma : node_t::semicolon );
            *n = {std::string(1,byte),line_no,node_type};
            nodePool.push_back(n);
            completeToken = "Sick hack, since this token won't be read, it doesn't matter what it is so long as it's not empty.";
            
        }else{
            currentToken += byte;
        }
        
        if( completeToken == "" ) continue;
        
        // there is a better way to generalize for keywords, but I only need this one for right now. This is helping me work out the general shift reduce structure first. Then I can organize my thousand rules.
        if( completeToken == "function" ){
            
            AST_node* node = new AST_node;
            
            node->line_no = line_no;
            node->text = completeToken;
            node->type = node_t::keyword;
            
            nodePool.push_back(node);
        }
        
        
        
        
        // if we haven't completed a token, & we have a match for a syntax.
        if( completeToken == "" && retMe.the_context.syntax_exists(currentToken) ) assert(false);// need the interpreter to continue this feature further.
        completeToken = "";// don't reparse a completed token, duh.
        
        /// ----------------------------------------------------    Reduction Rules Start Here    ---------------------------------------------------------- ///
        int i = 0;
        do{
            i = 0;
            for( i = 0; i < nodePool.size(); i++ ){
                
                if( is_terminated(nodePool[i]) )
                    continue;
                
                if( nodePool[i]->text=="("){
                    if( i<nodePool.size()-1 && nodePool[i+1]->text == ")" ){
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        nodePool.erase(nodePool.begin()+i+1);
                        break;
                    }
                }
                
                if( nodePool[i]->text=="{"){
                    bool valid_sequence = false;
                    int start_sequence = i+1;
                    int end_sequence   = start_sequence;
                    while( end_sequence<nodePool.size()-1 ){
                        
                        if( nodePool[end_sequence]->text == "}" ){
                            valid_sequence = true;
                            break;
                        }
                        
                        if( !is_terminated(nodePool[end_sequence]) )
                            break;
                        end_sequence++;
                    }
                    
                    if(valid_sequence){
                        for( int j = start_sequence; j < end_sequence+1; j++ )
                            nodePool[i]->children.push_back( nodePool[j] );
                        nodePool.erase(nodePool.begin()+start_sequence,nodePool.begin()+end_sequence);
                        break;
                    }
                    
                }
                
                if( nodePool[i]->text=="function" ){
                    
                    if( is_function_declaration(nodePool[i]) && !is_function_definition(nodePool[i]) && !is_terminated(nodePool[i]) &&
                        i+1 < nodePool.size() && is_closed_curly_bracket(nodePool[i+1]) ){
                        
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
                        break;
                    }
                    
                    if( i+2<nodePool.size() && !is_function_declaration(nodePool[i]) &&
                        
                        nodePool[i+1]->type == node_t::id &&
                        
                        nodePool[i+2]->text == "(" &&
                        nodePool[i+2]->children.size()>0 && nodePool[i+2]->children[nodePool[i+2]->children.size()-1]->text == ")"
                    ){
                        
                        nodePool[i+1]->children.push_back(nodePool[i+2]);
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        
                        retMe.the_context.add_symbol({sym_t_function,"function",nodePool[i+1]->text,nodePool[i]});
                        
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
                        break;
                    }
                }
                
                if( retMe.the_context.function_exists(nodePool[i]->text) && nodePool[i]->children.size() == 0 ){// is a function
                    
                    if( i+1<nodePool.size() &&
                        
                        nodePool[i+1]->type == node_t::paren && nodePool[i+1]->text == "(" &&
                        nodePool[i+1]->children.size()>0 && nodePool[i+1]->children[nodePool[i+1]->children.size()-1]->text == ")"
                    ){
                        
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        
                        
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
                        break;
                    }
                }
                
                if( is_terminable(nodePool[i],&retMe.the_context) &&
                    i+1 < nodePool.size() && nodePool[i+1]->text == ";"
                ){
                    AST_node* end_node = get_rightmost_bottommost(nodePool[i]);
                    end_node->children.push_back(nodePool[i+1]);
                    
                    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
                    
                    break;
                }
                
            }
        }while(i<nodePool.size());
    }
    retMe.root.children = nodePool;
    return retMe;
}


