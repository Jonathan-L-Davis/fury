#include "grammer.h"
#include "parse_util.h"
#include <cassert>

bool paren_closure_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void paren_closure_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool curly_bracket_closure_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void curly_bracket_closure_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool termination_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void termination_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool function_partial_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void function_partial_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool function_declaration_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void function_declaration_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool function_definition_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void function_definition_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool comma_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void comma_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
/*
bool syntax_partial_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void syntax_partial_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool syntax_declaration_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void syntax_declaration_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);

bool syntax_definition_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
void syntax_definition_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int index);
//*/
std::vector<rule> fury_grammer_rules(){
    std::vector<rule> retMe;
    
    retMe.push_back( {"paren-closure", paren_closure_applies, paren_closure_folding} );
    retMe.push_back( {"curly-bracket-closure", curly_bracket_closure_applies, curly_bracket_closure_folding} );
    retMe.push_back( {"termination", termination_applies, termination_folding} );
    retMe.push_back( {"function-partial", function_partial_applies, function_partial_folding} );// function partials out of pure tokens -- with scoping implications.
    retMe.push_back( {"function-declaration", function_declaration_applies, function_declaration_folding} );// function declarations out of function partials & function parameter clauses.
    retMe.push_back( {"function-definition", function_definition_applies, function_definition_folding} );// function definitions out of declarations & body definitions.
    retMe.push_back( {"comma", comma_applies, comma_folding} );
    
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

bool function_partial_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    return nodePool[i]->text == "function" && nodePool[i]->children.size() == 0 && (
    (i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ) ||
    (i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type )
    );
}

void function_partial_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    assert( function_partial_applies(nodePool,context,i) );
    
    bool is_folded_correctly = false;
    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
        nodePool[i+1]->type = node_t::function_id;
        
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
        is_folded_correctly = true;
    }
    else
    if( i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type ){
        nodePool[i+2]->type = node_t::function_id;
        
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool[i]->children.push_back(nodePool[i+2]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
        is_folded_correctly = true;
    }
    
    if(is_folded_correctly){
        
        return;
    }
    assert(false);
}

bool function_declaration_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    return (is_function_partial_declaration(nodePool[i]) && !is_function_declaration(nodePool[i]) &&
            i+1 < nodePool.size() && is_closed_parenthesis(nodePool[i+1]) );
}

void function_declaration_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    assert(function_declaration_applies(nodePool,context,i));
    
    int f_id_idx = nodePool[i]->children.size()-1;
    nodePool[i]->children[f_id_idx]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool function_definition_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    return (is_function_declaration(nodePool[i]) && !is_function_definition(nodePool[i]) &&
            i+1 < nodePool.size() && is_closed_curly_bracket(nodePool[i+1]) );
}

void function_definition_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    assert(function_definition_applies(nodePool,context,i));
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool comma_applies(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    return is_terminable(nodePool[i],context) &&
            i+1 < nodePool.size() && is_empty_comma(nodePool[i+1]) &&
            i+2 < nodePool.size() && is_terminable(nodePool[i+2],context);
}

void comma_folding(std::vector<AST_node*>& nodePool, symbol_table* context, int i){
    if( nodePool[i]->text == "," ){
        nodePool[i]->children.push_back(nodePool[i+2]);
        delete nodePool[i+1];// pointer fun
    }else{
        nodePool[i+1]->children.push_back(nodePool[i]);
        nodePool[i+1]->children.push_back(nodePool[i+2]);
        
        nodePool[i] = nodePool[i+1];
    }
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
}
