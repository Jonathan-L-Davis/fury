#include "grammer.h"
#include "parse_util.h"
#include <cassert>

bool paren_closure_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void paren_closure_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

std::vector<rule> fury_grammer_rules(){
    std::vector<rule> retMe;
    
    retMe.push_back( {"paren-closure", paren_closure_applies, paren_closure_folding} );
    
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
