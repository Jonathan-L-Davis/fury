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

bool termination_applies(std::vector<AST_node*>&,symbol_table*,int);

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
    retMe.file_name = file_name;
    retMe.root = {{":--:",0,root},{}};
    retMe.context = fury_default_context();
    std::fstream file(file_name);
    
    std::vector<uint8_t> file_buffer;
    std::vector<AST_node*> nodePool = {};
    std::vector<symbol_table*> context_stack = {&retMe.context};
    
    
    uint8_t q;
    while(file.read((char*)&q,1)){
        file_buffer.push_back(q);
    }
    
    //std::cout << "Parsing file: function.fury\n";
    //std::cout << "----------------------------------------------------------------------------------\n";
    //for( auto c : file_buffer ) std::cout << c;
    //std::cout << "\n";
    
    uint32_t line_no = 1;
    std::string completeToken;
    std::string currentToken;
    
    for(uint8_t byte : file_buffer){
        
        if( !( byte=='\n' || (byte>=' ' && byte<127) ) ){
            std::cout << (int) byte << std::endl;
            assert(byte=='\n' || ( byte>=' ' && byte<127) );
        }
        
        if(byte == ' ' || byte == '\n'){
            completeToken = currentToken;
            currentToken = "";
            
            if(byte == '\n') line_no++;
        }else if(byte=='('||byte==')'||byte=='{'||byte=='}'||byte==','||byte==';'){// these characters are forced to be single character tokens, cannot be part of other tokens. period.
            if(currentToken!=""){
                AST_node* n = new AST_node;
                node_t node_type = node_t::id;
                if(is_keyword(completeToken)) node_type = node_t::keyword;
                if(context_stack[context_stack.size()-1]->syntax_exists(currentToken+(char)byte)) node_type = node_t::syntax_id;
                *n = {currentToken,line_no,node_type};
                currentToken="";
                nodePool.push_back(n);
            }
            completeToken = std::string(1,byte);
        }else{
            currentToken += byte;
        }
        
        if( completeToken == "" ) continue;
        
        {
            AST_node* node = new AST_node;
            
            node->line_no = line_no;
            node->text = completeToken;
            if( is_keyword(completeToken) ){
                node->type = node_t::keyword;
                if( is_keyword_type(completeToken) ){
                    node->type = node_t::type;
                }
            }else if(byte=='('||byte==')')
                node->type = node_t::paren;
            else if(byte=='{'||byte=='}')
                node->type = node_t::curly;
            else if(byte==',')
                node->type = node_t::comma;
            else if(byte==';')
                node->type = node_t::semicolon;
            else if(context_stack[context_stack.size()-1]->syntax_exists(currentToken))
                node->type = node_t::syntax_id;
            else node->type = node_t::id;
            
            nodePool.push_back(node);
        }
        
        
        // if we haven't completed a token, & we have a match for a syntax.
        completeToken = "";// don't reparse a completed token, duh.
        grammer g = fury_grammer();
        /*/ ----------------------------------------------------    Reduction Rules Start Here    ---------------------------------------------------------- ///
        int i = 0;
        do{
            i = 0;
            for( i = 0; i < nodePool.size(); i++ ){
                
                std::vector<rule> rules = fury_grammer_rules();
                for( int j = 0; j < rules.size(); j++ ){
                    if( rules[j].rule_applies(nodePool,context_stack,i) ){
                        rules[j].apply_rule(nodePool,context_stack,i);
                        i = -1;
                        break;
                    }
                }
            }
        }while(i<nodePool.size());//*/
    }
    
    //std::cout << "--------------------------------------------------------------------------------\n";for( AST_node* node : nodePool ){node->print_with_types();};
    //for( AST_node* node : nodePool ) assert( is_terminated(node) );// if this fails you have bad grammer.
    
    retMe.root.children = nodePool;
    return retMe;
}


