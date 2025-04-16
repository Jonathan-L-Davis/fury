#include "grammer.h"
#include "parse_util.h"
#include <cassert>

bool paren_closure_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void paren_closure_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool curly_bracket_closure_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void curly_bracket_closure_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool termination_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void termination_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

std::vector<rule> fury_grammer_rules(){
    std::vector<rule> retMe;
    
    retMe.push_back( {"paren-closure", paren_closure_applies, paren_closure_folding} );
    retMe.push_back( {"curly-bracket-closure", curly_bracket_closure_applies, curly_bracket_closure_folding} );
    retMe.push_back( {"termination", termination_applies, termination_folding} );
    
    return retMe;
}

bool paren_closure_applies(std::vector<AST_node*>& nodePool, symbol_table* context,int i){
    return nodePool[i]->text=="(" && !is_closed_parenthesis(nodePool[i]) &&
    (   ( i+1 < nodePool.size() && nodePool[i+1]->text == ")" ) || // empty ()
        ( i+2 < nodePool.size() && nodePool[i+2]->text == ")" && is_terminable(nodePool[i+1],context) ) ); // enclosed (expr)
}

void paren_closure_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    
    assert( nodePool[i]->text == "(" );
    
    // one of these must be true if it's being called.
    if( i<nodePool.size()-1 && nodePool[i+1]->text == ")" ){
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool.erase(nodePool.begin()+i+1);
        return;
    }
    if( i<nodePool.size()-2 && nodePool[i+2]->text == ")" && is_terminable(nodePool[i+1],context) ){
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool[i]->children.push_back(nodePool[i+2]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
        return;
    }
    assert(false);
}

bool curly_bracket_closure_applies(std::vector<AST_node*>& nodePool, symbol_table* context,int i){
    if( nodePool[i]->text!="{" || is_closed_curly_bracket(nodePool[i]) )
        return false;
    
    int j = i+1;
    while( j < nodePool.size() ){
        
        if( nodePool[j]->text=="}" ) return true;
        if( !is_terminated(nodePool[j]) ) return false;
        
        j++;
    }
    
    return false;
}

void curly_bracket_closure_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    assert( curly_bracket_closure_applies(nodePool,context,i) );
    
    bool valid_sequence = false;
    int start_sequence  = i+1;
    int end_sequence    = start_sequence;
    while( end_sequence<nodePool.size() ){
        
        if( nodePool[end_sequence]->text == "}" ){
            valid_sequence = true;
            break;
        }
        
        end_sequence++;
    }
    
    if(valid_sequence){
        for( int j = start_sequence; j < end_sequence+1; j++ )
            nodePool[i]->children.push_back( nodePool[j] );
        nodePool.erase(nodePool.begin()+start_sequence,nodePool.begin()+end_sequence+1);
        return;
    }
    
    assert(false);
}

bool termination_applies(std::vector<AST_node*>& nodePool, symbol_table* context,int i){
    return is_terminable(nodePool[i],context) && i+1 < nodePool.size() && nodePool[i+1]->text == ";";
}

void termination_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    assert( termination_applies(nodePool,context,i)  );
    
    AST_node* end_node = get_rightmost_bottommost(nodePool[i]);
    end_node->children.push_back(nodePool[i+1]);
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
}
