#include <string>
#include <fstream>
#include <iostream>

#include "lex.h"
#include "parse.h"
#include "parse_util.h"
#include "symbol_table.h"
#include "program.h"
#include "grammer.h"

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
    retMe.root = {{":--:",0,root},{}};
    std::fstream file(file_name);
    
    std::vector<uint8_t> file_buffer;
    std::vector<AST_node*> nodePool = {};
    
    
    uint8_t q;
    while(file.read((char*)&q,1)){
        file_buffer.push_back(q);
    }
    
    std::cout << "Parsing file: function.fury\n";
    std::cout << "----------------------------------------------------------------------------------\n";
    for( auto c : file_buffer ) std::cout << c;
    std::cout << "\n";
    
    uint32_t line_no = 1;
    std::string completeToken;
    std::string currentToken;
    
    for(uint8_t byte : file_buffer){
        
        if( !( byte=='\n' || (byte>=' ' && byte<127) ) ){
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
        }else if(byte=='('||byte==')'||byte=='{'||byte=='}'||byte==','||byte==';'){// these characters are forced to be single character tokens, cannot be part of other tokens. period.
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
        
        if( is_keyword(completeToken) ){
            
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
            restart_reductions:;
            i = 0;
            for( i = 0; i < nodePool.size(); i++ ){
                
                if( is_terminated(nodePool[i]) )
                    continue;
                
                std::vector<rule> rules = fury_grammer_rules();
                for( int j = 0; j < rules.size(); j++ ){
                    if( rules[i].rule_applies(nodePool,&retMe.the_context,i) ){
                        rules[i].apply_rule(nodePool,&retMe.the_context,i);
                        goto restart_reductions;
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
                    
                    if( i+2<nodePool.size() && is_function_partial_declaration(nodePool[i]) && !is_function_declaration(nodePool[i]) &&
                        nodePool[i+1]->type == node_t::id &&
                        is_closed_parenthesis(nodePool[i+2])
                    ){
                        nodePool[i+1]->type = node_t::function_id;
                        
                        nodePool[i+1]->children.push_back(nodePool[i+2]);
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        
                        retMe.the_context.add_symbol({sym_t_function,"function",nodePool[i+1]->text,nodePool[i]});
                        
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
                        break;
                    }
                    
                    if( !is_function_partial_declaration(nodePool[i]) && !is_function_declaration(nodePool[i]) ){}
                }
                
                if( nodePool[i]->text=="byte" ){
                    
                    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
                        assert( !retMe.the_context.id_exists(nodePool[i+1]->text) );
                        
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        retMe.the_context.add_symbol({sym_t_byte,"byte",nodePool[i+1]->text,nodePool[i]});
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
                        break;
                    }
                    
                }
                
                if( nodePool[i]->text=="dual" ){
                    
                    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
                        assert( !retMe.the_context.id_exists(nodePool[i+1]->text) );
                        
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        retMe.the_context.add_symbol({sym_t_dual,"dual",nodePool[i+1]->text,nodePool[i]});
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
                        break;
                    }
                    
                }
                
                if( nodePool[i]->text=="quad" ){
                    
                    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
                        assert( !retMe.the_context.id_exists(nodePool[i+1]->text) );
                        
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        retMe.the_context.add_symbol({sym_t_quad,"quad",nodePool[i+1]->text,nodePool[i]});
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
                        break;
                    }
                    
                }
                
                if( nodePool[i]->text=="oct" ){
                    
                    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
                        assert( !retMe.the_context.id_exists(nodePool[i+1]->text) );
                        
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        retMe.the_context.add_symbol({sym_t_oct,"oct",nodePool[i+1]->text,nodePool[i]});
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
                        break;
                    }
                    
                }
                
                if( retMe.the_context.function_exists(nodePool[i]->text) && nodePool[i]->children.size() == 0 ){// is a function
                    
                    if( i+1<nodePool.size() && is_closed_parenthesis(nodePool[i+1])
                    ){
                        
                        nodePool[i]->children.push_back(nodePool[i+1]);
                        
                        
                        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
                        break;
                    }
                }
                
                if( is_terminable(nodePool[i],&retMe.the_context) &&
                    i+1 < nodePool.size() && is_empty_comma(nodePool[i+1]) &&
                    i+2 < nodePool.size() && is_terminable(nodePool[i+2],&retMe.the_context)
                ){
                    if( nodePool[i]->text == "," ){
                        nodePool[i]->children.push_back(nodePool[i+2]);
                        delete nodePool[i+1];// pointer fun
                    }else{
                        nodePool[i+1]->children.push_back(nodePool[i]);
                        nodePool[i+1]->children.push_back(nodePool[i+2]);
                        
                        nodePool[i] = nodePool[i+1];
                    }
                    
                    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
                    
                    break;
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
    
    for( AST_node* node : nodePool ) assert( is_terminated(node) );// if this fails you have bad grammer.
    
    retMe.root.children = nodePool;
    return retMe;
}


