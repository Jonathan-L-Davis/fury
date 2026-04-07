#include <string>
#include <fstream>
#include <iostream>

#include "parse.h"
#include "parse_util.h"
#include "symbol_table.h"
#include "program.h"
#include "grammer.h"

#include <vector>
#include <assert.h>

bool termination_applies(std::vector<AST_node*>&, std::vector<symbol_table*>&,int);
bool if_head_applies(std::vector<AST_node*>&, std::vector<symbol_table*>&,int);
bool if_statement_applies(std::vector<AST_node*>&, std::vector<symbol_table*>&,int);

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
    std::vector<AST_node*> finishedNodes = {};
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
        
        // excludes unicode for now. 
        if( !( byte=='\n' || (byte>=' ' && byte<127) ) ){
            std::cout << (int) byte << std::endl;
            assert(byte=='\n' || ( byte>=' ' && byte<127) );
        }
        
        // whitespace breaks a token off.
        if(byte == ' ' || byte == '\n'){
            completeToken = currentToken;
            currentToken = "";
            
            if(byte == '\n') line_no++;
        }else if(byte=='('||byte==')'||byte=='{'||byte=='}'||byte==','||byte==';'){// these characters are forced to be single character tokens, cannot be part of other tokens. period.
            if(currentToken!=""){
                AST_node* node = new AST_node;
                node_t node_type = node_t::id;
                if(is_keyword(currentToken)) node_type = node_t::keyword;
                if(context_stack[context_stack.size()-1]->syntax_exists(currentToken+(char)byte)) node_type = node_t::syntax_id;
                *node = {currentToken,line_no,node_type};
                nodePool.push_back(node);
                
                if(currentToken=="if"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("if",scope_t_if,node));
                }else if(currentToken=="else"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("else",scope_t_else,node));
                }else if(currentToken=="for"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("for",scope_t_for,node));
                }else if(currentToken=="while"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("while",scope_t_while,node));
                }
                
                currentToken="";
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
                
                if(completeToken=="if"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("if",scope_t_if,node));
                }else if(completeToken=="else"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("else",scope_t_else,node));
                }else if(completeToken=="for"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("for",scope_t_for,node));
                }else if(completeToken=="while"){
                    symbol_table& context = **(context_stack.end()-1);
                    context_stack.push_back(context.add_scope("while",scope_t_while,node));
                }
                
            }else if(completeToken=="("||completeToken==")")
                node->type = node_t::paren;
            else if(completeToken=="{"){
                
                node->type = node_t::curly;
                
                std::string scope = "anon";
                scope_type new_scope_t = scope_t_anonymous;
                AST_node* node = nullptr;
                
                if(nodePool.size()>0&&!is_terminated(nodePool[nodePool.size()-1])&&is_syntax_declaration(nodePool[nodePool.size()-1])){
                    
                    node = nodePool[nodePool.size()-1];
                    
                    new_scope_t = scope_t_syntax;
                    scope = "body";
                }
                if(nodePool.size()>0&&!is_terminated(nodePool[nodePool.size()-1])&&is_function_declaration(nodePool[nodePool.size()-1])) {
                    
                    node = nodePool[nodePool.size()-1];
                    
                    new_scope_t = scope_t_function;
                    scope = "body";
                }
                if(nodePool.size()>0&&!is_terminated(nodePool[nodePool.size()-1])&&is_operator_declaration(nodePool[nodePool.size()-1])) {
                    
                    node = nodePool[nodePool.size()-1];
                    
                    new_scope_t = scope_t_operator;
                    scope = "body";
                }
                
                symbol_table& context = **(context_stack.end()-1);
                context_stack.push_back(context.add_scope(scope,new_scope_t,node));
                
            }else if(completeToken=="}")
                node->type = node_t::curly;
            else if(completeToken==","){
                node->type = node_t::comma;
                
                
                int idx = nodePool.size()-1;
                symbol_table* context = *(context_stack.end()-1);
                if(nodePool.size()>=1 && nodePool[idx]==context->node) context_stack.resize(context_stack.size()-1);// escapes the first child of a comma. Subsequent child scopes can be correctly popped during folding.
            }else if(completeToken==";")
                node->type = node_t::semicolon;
            else if(context_stack[context_stack.size()-1]->syntax_exists(currentToken)){
                node->type = node_t::syntax_id;
                assert(false&&"Syntax execution not yet supported.");
            }else node->type = node_t::id;
            
            nodePool.push_back(node);
        }
        
        
        // if we haven't completed a token, & we have a match for a syntax.
        completeToken = "";// don't reparse a completed token, duh.
        grammer g = fury_grammer();
        /// ----------------------------------------------------    Reduction Rules Start Here    ---------------------------------------------------------- ///
        
        top:;
        int lowest_layer = 2;
        bool ends_in_terminal = false;
        
        auto last_idx = [&](){
            return int(nodePool.size())-1;
        };
        
        auto first_idx = [&](){
            for(int i = int(nodePool.size())-1; i >= 0; i--)
                if(nodePool[i]->text=="{"&&!is_closed_curly_bracket(nodePool[i]))// might also have to consider if/else/for/while as scoping. Not sure yet how those scopes will play with the rest of the language. For now it's an extra knob to turn when I get stuck debugging. Probably should just be ignored though.
                    return i;
            return 0;
        };
        
        if(nodePool.size()>0&&nodePool[nodePool.size()-1]->text==";") ends_in_terminal = true;
        
        if(ends_in_terminal){// prevents ',' nodes from grabbing too quickly. This prevents comma separated function definitions from stopping before getting the body of the second function.
            lowest_layer = 0;
        }
        
        bool modified = false;
        do{
            modified = false;
            
            for( int i = g.rules.size()-1; i >= lowest_layer; i--){
                std::vector<rule>& rules = g.rules[i];
                for( int j = 0; j < rules.size(); j++){
                    if(modified) goto top;
                    
                    if(g.direction[i]==parse_dir::forward){
                        t1:;
                        for( int k = first_idx(); k < last_idx(); k++ ){
                            if( rules[j].rule_applies(nodePool,context_stack,k) ){
                                
                                rules[j].apply_rule(nodePool,context_stack,k);
                                
                                modified = true;
                                goto t1;
                            }
                        }
                    }else if(g.direction[i]==parse_dir::backward){
                        t2:;
                        for( int k = last_idx(); k >= first_idx(); k-- ){
                            if( rules[j].rule_applies(nodePool,context_stack,k) ){
                                
                                rules[j].apply_rule(nodePool,context_stack,k);
                                
                                modified = true;
                                goto t2;
                            }
                        }
                    }else{std::cout << "Parse direction must be either forward or backward.\n";std::exit(-1);}
                }
            }
        }while(modified);
        
        // this is effectively an optimization, but also avoids bugs.
        while( nodePool.size()>0 ){
            AST_node* node = nodePool[0];
            if(is_terminated(node))
                finishedNodes.push_back(node);
            else
                break;
            
            nodePool.erase(nodePool.begin());
        }
        
    }
    retMe.root.children = finishedNodes;
    
    assert(context_stack.size()==1);
    
    return retMe;
}


