#include "grammer.h"
#include "parse_util.h"
#include "type_util.h"

#include <cassert>

bool paren_closure_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void paren_closure_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool curly_bracket_closure_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void curly_bracket_closure_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool termination_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void termination_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool function_partial_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void function_partial_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool function_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void function_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool function_definition_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void function_definition_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool function_call_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void function_call_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool comma_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void comma_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool type_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void type_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool syntax_partial_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void syntax_partial_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool syntax_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void syntax_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool syntax_definition_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void syntax_definition_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool return_statement_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void return_statement_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool if_statement_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void if_statement_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool if_else_statement_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void if_else_statement_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool for_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void for_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool while_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void while_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool operator_partial_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void operator_partial_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool operator_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void operator_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool operator_definition_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void operator_definition_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

bool operator_call_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);
void operator_call_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int index);

symbol_table fury_default_context(){
    symbol_table retMe;
    
    // the basic complete types
    retMe.add_symbol({sym_t_type,{"type"},"byte",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"dual",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"quad",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"oct",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"type",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"label",nullptr});
    
    /* the basic incomplete types - don't need to
    retMe.add_symbol({sym_t_type,{"type"},"function",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"operator",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"syntax",nullptr});
    retMe.add_symbol({sym_t_type,{"type"},"struct",nullptr});//*/
    
    return retMe;
}

void grammer::add_layer(int i,parse_dir dir){
    
    assert( i>0 && i<=rules.size() );
    
    rules.insert( rules.begin()+i,{{/*God do I hate init braces sometimes*/}});
    direction.insert(direction.begin()+i,dir);
}

void grammer::add_rule(int i,rule addMe){
    assert( i>=0 && i<rules.size() );
    rules[i].push_back(addMe);
}

grammer fury_grammer(){
    grammer retMe;
    
    retMe.direction = {parse_dir::forward,parse_dir::forward};
    retMe.rules = { {}, {} };
    
    retMe.add_layer( 1, parse_dir::backward );
    
    retMe.add_rule(2,{"paren-closure", paren_closure_applies, paren_closure_folding} );
    retMe.add_rule(2,{"curly-bracket-closure", curly_bracket_closure_applies, curly_bracket_closure_folding} );
    
    retMe.add_rule(2,{"operator-partial", operator_partial_applies, operator_partial_folding} );
    retMe.add_rule(2,{"function-partial", function_partial_applies, function_partial_folding} );
    retMe.add_rule(2,{"syntax-partial", syntax_partial_applies, syntax_partial_folding} );
    
    retMe.add_rule(2,{"type-declaration", type_declaration_applies, type_declaration_folding} );
    
    
    
    retMe.add_rule(1,{"operator-declaration", operator_declaration_applies, operator_declaration_folding} );
    retMe.add_rule(1,{"operator-definition", operator_definition_applies, operator_definition_folding} );
    retMe.add_rule(1,{"function-declaration", function_declaration_applies, function_declaration_folding} );
    retMe.add_rule(1,{"function-definition", function_definition_applies, function_definition_folding} );
    retMe.add_rule(1,{"syntax-declaration", syntax_declaration_applies, syntax_declaration_folding} );
    retMe.add_rule(1,{"syntax-definition", syntax_definition_applies, syntax_definition_folding} );

    retMe.add_rule(1,{"function-call", function_call_applies, function_call_folding});
    retMe.add_rule(1,{"operator-call", operator_call_applies, operator_call_folding});
    
    retMe.add_rule(1,{"if-statement", if_statement_applies, if_statement_folding} );
    retMe.add_rule(1,{"if-else-statement", if_else_statement_applies, if_else_statement_folding} );
    retMe.add_rule(1,{"return-statement", return_statement_applies, return_statement_folding} );
    retMe.add_rule(1,{"for-loop", for_applies, for_folding});
    retMe.add_rule(1,{"while-loop", while_applies, while_folding});
    
    
    
    retMe.add_rule(0,{"comma", comma_applies, comma_folding} );
    retMe.add_rule(0,{"termination", termination_applies, termination_folding} );//*/
    return retMe;
}

bool paren_closure_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context,int i){
    return nodePool[i]->text=="(" && !is_closed_parenthesis(nodePool[i]) &&
    (   ( i+1 < nodePool.size() && nodePool[i+1]->text == ")" ) || // empty ()
        ( i+2 < nodePool.size() && nodePool[i+2]->text == ")" && is_terminable(nodePool[i+1],context[context.size()-1]) ) ); // enclosed (expr)
}

void paren_closure_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    
    assert( paren_closure_applies(nodePool,context,i) );
    
    // one of these must be true if it's being called.
    if( i<nodePool.size()-1 && nodePool[i+1]->text == ")" ){
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool.erase(nodePool.begin()+i+1);
        return;
    }
    if( i<nodePool.size()-2 && nodePool[i+2]->text == ")" && is_terminable(nodePool[i+1],context[context.size()-1]) ){
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool[i]->children.push_back(nodePool[i+2]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
        return;
    }
    assert(false);
}

bool curly_bracket_closure_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context,int i){
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

void curly_bracket_closure_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>&  context, int i){
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

bool termination_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context,int i){
    return is_terminable(nodePool[i],context[context.size()-1]) && i+1 < nodePool.size() && nodePool[i+1]->text == ";";
}

void termination_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert( termination_applies(nodePool,context,i)  );
    
    AST_node* end_node = get_rightmost_bottommost(nodePool[i]);
    end_node->children.push_back(nodePool[i+1]);
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    
    // should really have some sort of general functional/control flow check here. Should also include if's, elses, and for's
    // actually, on second thought maybe it's best to not have such things, since if's would potentially need multiple scoped popped off.
    if( nodePool[i]->text == "function" )
        context.resize(context.size()-1);
    
    /*
    if( nodePool[i]->text == "operator" )
        context.resize(context.size()-1);//*/
}

bool function_partial_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return nodePool[i]->text == "function" && nodePool[i]->children.size() == 0 && (
    (i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ) ||
    (i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type )
    );
}

void function_partial_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert( function_partial_applies(nodePool,context,i) );
    
    bool is_folded_correctly = false;
    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
        nodePool[i+1]->type = node_t::function_id;
        
        context[context.size()-1]->add_symbol({sym_t_function,{""},nodePool[i+1]->text,nodePool[i]});
        context[context.size()-1]->add_scope(nodePool[i+1]->text,scope_type::scope_t_function);
        context.push_back( &context[context.size()-1]->get_subscope(nodePool[i+1]->text) );
        
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
        is_folded_correctly = true;
    }
    else
    if( i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type ){
        nodePool[i+2]->type = node_t::function_id;
        
        context[context.size()-1]->add_symbol({sym_t_function,{nodePool[i+1]->text},nodePool[i+2]->text,nodePool[i]});
        context[context.size()-1]->add_scope(nodePool[i+2]->text,scope_type::scope_t_function);
        context.push_back( &context[context.size()-1]->get_subscope(nodePool[i+2]->text) );
        
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

bool function_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return (is_function_partial_declaration(nodePool[i]) && !is_function_declaration(nodePool[i]) &&
            i+1 < nodePool.size() && is_closed_parenthesis(nodePool[i+1]) );
}

void function_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(function_declaration_applies(nodePool,context,i));
    
    int f_id_idx = nodePool[i]->children.size()-1;
    nodePool[i]->children[f_id_idx]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool function_definition_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return (is_function_declaration(nodePool[i]) && !is_function_definition(nodePool[i]) &&
            i+1 < nodePool.size() && is_closed_curly_bracket(nodePool[i+1]) );
}

void function_definition_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(function_definition_applies(nodePool,context,i));
    
    
    // need to handle scoping more like operators are now handled. Meaning I have some revisions to do in the earlier function fragments.
    //symbol_table& current_scope = **(context.end()-1);
    //symbol_table& func_scope    = (current_scope.get_subscope(get_func_id(nodePool[i]),get_func_signature(nodePool[i])));
    
    //move_curly_bracket_declarations(nodePool[i+1],current_scope,func_scope);
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool function_call_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return context[context.size()-1]->function_id_exists(nodePool[i]->text) && nodePool.size()>i+1&& is_closed_parenthesis(nodePool[i+1]);
}

void function_call_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(function_call_applies(nodePool,context,i));
    
    nodePool[i+1]->type = node_t::function_id;
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool comma_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return is_terminable(nodePool[i],context[context.size()-1]) &&
            i+1 < nodePool.size() && is_empty_comma(nodePool[i+1]) &&
            i+2 < nodePool.size() && is_terminable(nodePool[i+2],context[context.size()-1]);
}

void comma_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    if( nodePool[i]->text == "," ){
        nodePool[i]->children.push_back(nodePool[i+2]);
        delete nodePool[i+1];// pointer fun
    }else{
        if( nodePool[i]->text == "function" )
            context.resize(context.size()-1);
        
        if( nodePool[i]->text == "operator" )
            context.resize(context.size()-1);
        
        nodePool[i+1]->children.push_back(nodePool[i]);
        nodePool[i+1]->children.push_back(nodePool[i+2]);
        
        nodePool[i] = nodePool[i+1];
    }
    
    if( nodePool[i+2]->text == "function" )
        context.resize(context.size()-1);
    
    if( nodePool[i+2]->text == "operator" )
        context.resize(context.size()-1);
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
}

bool type_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    if( nodePool[i]->type!=node_t::type || nodePool[i]->children.size() != 0 )
        return false;
    
    if( i+1 >= nodePool.size() || nodePool[i+1]->type != node_t::id )
        return false;
    
    return true;
}

void type_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(type_declaration_applies(nodePool,context,i));
    
    symbol_type sym_type;
    if( nodePool[i]->text == "byte" ) sym_type = sym_t_byte;
    if( nodePool[i]->text == "dual" ) sym_type = sym_t_dual;
    if( nodePool[i]->text == "quad" ) sym_type = sym_t_quad;
    if( nodePool[i]->text == "oct" ) sym_type = sym_t_oct;
    if( nodePool[i]->text == "type" ) sym_type = sym_t_type;
    if( nodePool[i]->text == "label" ) sym_type = sym_t_label;
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    
    context[context.size()-1]->add_symbol({sym_type,{nodePool[i]->text},nodePool[i+1]->text,nodePool[i]});
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
}

bool syntax_partial_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return nodePool[i]->text == "syntax" && nodePool[i]->children.size() == 0 && (
    (i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ) ||
    (i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type )
    );
}

void syntax_partial_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert( syntax_partial_applies(nodePool,context,i) );
    
    bool is_folded_correctly = false;
    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
        nodePool[i+1]->type = node_t::syntax_id;
        
        context[context.size()-1]->add_symbol({sym_t_syntax,{""},nodePool[i+1]->text,nodePool[i]});
        context[context.size()-1]->add_scope(nodePool[i+1]->text,scope_type::scope_t_syntax);
        context.push_back( &context[context.size()-1]->get_subscope(nodePool[i+1]->text) );
        
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
        is_folded_correctly = true;
    }
    else
    if( i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type ){
        nodePool[i+2]->type = node_t::syntax_id;
        
        context[context.size()-1]->add_symbol({sym_t_syntax,{nodePool[i+1]->text},nodePool[i+2]->text,nodePool[i]});
        context[context.size()-1]->add_scope(nodePool[i+2]->text,scope_type::scope_t_syntax);
        context.push_back( &context[context.size()-1]->get_subscope(nodePool[i+2]->text) );
        
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

bool syntax_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return (is_syntax_partial_declaration(nodePool[i]) && !is_syntax_declaration(nodePool[i]) &&
            i+1 < nodePool.size() && is_closed_parenthesis(nodePool[i+1]) ) &&
            nodePool[i+1]->children.size() == 2 && nodePool[i+1]->children[0]->text == "byte";// type checking the param type of syntax, which can only be a single byte type.
}

void syntax_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(syntax_declaration_applies(nodePool,context,i));
    
    int s_id_idx = nodePool[i]->children.size()-1;
    nodePool[i]->children[s_id_idx]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool syntax_definition_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return (is_syntax_declaration(nodePool[i]) && !is_syntax_definition(nodePool[i]) &&
            i+1 < nodePool.size() && is_closed_curly_bracket(nodePool[i+1]) );
}

void syntax_definition_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(syntax_definition_applies(nodePool,context,i));
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool return_statement_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return nodePool[i]->text=="return" && !is_return_statement(nodePool[i],*(context.end()-1)) && i+1<nodePool.size() && nodePool[i+1] && is_valid(nodePool[i+1],context[context.size()-1]);
}

void return_statement_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(return_statement_applies(nodePool,context,i));
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

bool if_statement_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return nodePool[i]->text=="if" && !is_if_statement(nodePool[i]) && i+2<nodePool.size() && is_closed_parenthesis(nodePool[i+1]) && is_terminable(nodePool[i+2],*(context.end()-1));
}

void if_statement_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    
    assert(if_statement_applies(nodePool,context,i));
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    nodePool[i]->children.push_back(nodePool[i+2]);
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
    
    return;
}

bool if_else_statement_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return is_if_statement(nodePool[i]) && !is_if_else_statement(nodePool[i]) && i+2<nodePool.size() && nodePool[i+1]->text=="else" && is_valid(nodePool[i+2],*(context.end()-1));
}

void if_else_statement_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    
    assert(if_else_statement_applies(nodePool,context,i));
    
    nodePool[i+1]->children.push_back(nodePool[i+2]);
    nodePool[i]->children.push_back(nodePool[i+1]);
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
    
    return;
}

bool for_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return  nodePool[i]->text=="for" && i+6<nodePool.size() &&
            nodePool[i+1]->text=="(" && nodePool[i+1]->children.size()==0 &&
            is_valid(nodePool[i+2],*(context.end()-1)) && is_terminated(nodePool[i+2]) &&
            is_valid(nodePool[i+3],*(context.end()-1)) && is_terminated(nodePool[i+3]) &&
            is_valid(nodePool[i+4],*(context.end()-1)) && is_terminable(nodePool[i+4],*(context.end()-1)) &&
            nodePool[i+5]->text==")" && nodePool[i+5]->children.size()==0 &&
            is_valid(nodePool[i+6],*(context.end()-1)) && is_terminable(nodePool[i+6],*(context.end()-1));
}

void for_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(for_applies(nodePool,context,i));
    
    delete nodePool[i+1];
    delete nodePool[i+5];
    
    nodePool[i]->children.push_back(nodePool[i+2]);
    nodePool[i]->children.push_back(nodePool[i+3]);
    nodePool[i]->children.push_back(nodePool[i+4]);
    nodePool[i]->children.push_back(nodePool[i+6]);
    
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+7);
}

bool while_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return nodePool[i]->text=="while" && !is_while_loop(nodePool[i]) && 
            (
                (i+2<nodePool.size() && is_closed_parenthesis(nodePool[i+1]) && is_terminable(nodePool[i+2],*(context.end()-1))) ||
                (i+1<nodePool.size() && is_closed_parenthesis(nodePool[i+1]) && is_terminated(nodePool[i+1]))
            );
}

void while_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(while_applies(nodePool,context,i));
    
    if( is_terminated(nodePool[i+1]) ){
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    }else{
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool[i]->children.push_back(nodePool[i+2]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);
    }
    
}

// yeah, this one's complicated.
bool operator_partial_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return nodePool[i]->text == "operator" && nodePool[i]->children.size() == 0 && (
    (i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ) ||
    (i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type )
    );
}

void operator_partial_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert( operator_partial_applies(nodePool,context,i) );
    
    bool is_folded_correctly = false;
    if( i+1<nodePool.size() && nodePool[i+1]->type == node_t::id ){
        nodePool[i+1]->type = node_t::operator_id;
        
        context[context.size()-1]->add_symbol({sym_t_operator,{""},nodePool[i+1]->text,nodePool[i]});
        
        nodePool[i]->children.push_back(nodePool[i+1]);
        nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
        is_folded_correctly = true;
    }
    else
    if( i+2<nodePool.size() && nodePool[i+2]->type == node_t::id && nodePool[i+1]->type == node_t::type ){
        nodePool[i+2]->type = node_t::operator_id;
        
        context[context.size()-1]->add_symbol({sym_t_operator,{nodePool[i+1]->text},nodePool[i+2]->text,nodePool[i]});
        
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

bool operator_declaration_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    if(!is_operator_partial_declaration(nodePool[i])) return false;
    
    AST_node* op_id = *(nodePool[i]->children.end()-1);
    std::string total_text = op_id->text;
    std::string found_text = "";
    
    for( i++; i < nodePool.size(); i++ ){
        if(nodePool[i]->type==node_t::id) found_text += nodePool[i]->text;
        
        if(nodePool[i]->text=="("&&!is_closed_parenthesis(nodePool[i])) return false;// don't succeed if parameters aren't reduced yet.
        
        if( found_text!=total_text.substr(0,std::min(total_text.size(),found_text.size())) ) break;// we have a bad declaration, or aren't done reducing params yet.
        
        if( found_text==total_text && i+1<nodePool.size() && (is_closed_curly_bracket(nodePool[i+1]) || nodePool[i+1]->text==";") )
            return true;
    }
    
    return false;
}

void operator_declaration_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(operator_declaration_applies(nodePool,context,i));
    int start_idx = i+1;
    AST_node* op_id = *(nodePool[i]->children.end()-1);
    std::string total_text = op_id->text;
    std::string found_text = "";
    
    for( i++; i < nodePool.size(); i++ ){
        if(nodePool[i]->type==node_t::id){
            op_id->children.push_back(nodePool[i]);
            found_text+=nodePool[i]->text;
        }
        
        if(nodePool[i]->text=="("){
            assert( is_closed_parenthesis(nodePool[i]) );
            op_id->children.push_back(nodePool[i]);
        }
        
        if( found_text==total_text && (is_closed_curly_bracket(nodePool[i+1]) || nodePool[i+1]->text==";" || nodePool[i+1]->text=="," || nodePool[i+1]->text=="{" ) ){
            nodePool.erase( nodePool.begin()+start_idx, nodePool.begin()+i+1 );
            
            context[context.size()-1]->add_scope(op_id->text,scope_type::scope_t_operator);
            std::vector<std::string> signature;
            for(int i = 0;i<op_id->children.size();i++){
                
                if(op_id->children[i]->text!="("){// add op id's
                    op_id->children[i]->type = node_t::operator_id;
                    signature.push_back(op_id->children[i]->text);
                }
                else{// add parameters
                    AST_node* params = op_id->children[i];
                    if(params->children[0]->text==",")
                        params=params->children[0];
                    
                    for(AST_node* node:params->children){
                        signature.push_back(node->text);
                    }
                }
            }
            //  Take type declarations from symbol table and push them into sub scope.
            symbol_table& op_scope = (*(context.end()-1))->get_subscope(found_text,{});
            op_scope.signature=signature;
            
            move_operator_param_declarations(op_id,**(context.end()-1),op_scope);
            
            return;
        }
        
        if( found_text!=total_text.substr(0,std::min(total_text.size(),found_text.size()) ) ){
            std::cout << found_text << ":" << total_text << "\n";
            assert("Bad operator declaration, id's don't match, likely a bug in operator_declaration_applies, this should never execute."&&false);
        }
    }
    
    assert(false);
}

bool operator_definition_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    return is_operator_declaration(nodePool[i])&&i+1<nodePool.size()&&is_closed_curly_bracket(nodePool[i+1]);
}

void operator_definition_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    assert(operator_definition_applies(nodePool,context,i));
    
    symbol_table& current_scope = **(context.end()-1);
    symbol_table& op_scope      = (current_scope.get_subscope(get_op_id(nodePool[i]),get_op_signature(nodePool[i])));
    
    move_curly_bracket_declarations(nodePool[i+1],current_scope,op_scope);
    
    nodePool[i]->children.push_back(nodePool[i+1]);
    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+2);
    
    return;
}

int op_applies(symbol S, symbol_table context, std::vector<AST_node*>& nodePool, int i){
    
    AST_node* op = S.value;
    //op->print_with_types();
    if(!is_operator_declaration(op))
        return 0;
    
    if((*(op->children.end()-1))->text=="{")// should have a safety assert here to make sure the size is at least 2.
        op=*(op->children.end()-2);
    else
        op=*(op->children.end()-1);
    
    // grab all types & strings. Might be worth dereferencing the pointer into a local variable to ingore threading concerns.
    std::vector<AST_node*> tokens;
    for(AST_node* c:op->children){
        if(c->type==node_t::operator_id||c->type==node_t::id){
            tokens.push_back(c);
        }
        if(c->text=="("){
            if(c->children.size()>=1&&c->children[0]->text==","){
                //c->print_with_types();
                assert(c->children.size()==1&&"Comma's shouldn't have siblings.\n");
                c=c->children[0];
            }
            
            for( int i = 0; i < c->children.size()-1; i++ ){
                tokens.push_back(c->children[i]);
            }
        }
    }
    
    int k = 0;
    for(int j = i; j < nodePool.size()&&k<tokens.size(); j++,k++ ){// no I don't *need* 2 indices, but it's easier to track that way.
        if((nodePool[j]->type==node_t::id||nodePool[j]->type==node_t::id)&&nodePool[j]->text==tokens[k]->text)
            continue;
        
        typeset a = context.get_type(nodePool[j]);
        typeset b = context.get_type(tokens[k]);
        
        if( a==b )// need to match type to parameter ID
            continue;
        else return 0;
    }
    
    return k;
}

bool operator_call_applies(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    
    if(is_terminated(nodePool[i]))return false;
    
    symbol_table& sym_tbl = **(context.end()-1);
    
    std::vector<symbol> op_list = sym_tbl.get_ops();
    
    for(auto& op: op_list)
        if( op_applies(op,sym_tbl,nodePool,i)>0 ) return true;
    
    return false;
}

void operator_call_folding(std::vector<AST_node*>& nodePool, std::vector<symbol_table*>& context, int i){
    /*
    std::cout << "-------------------------------------------------------------------\n";
    for(auto n: nodePool) n->print_with_types();//*/
    assert(operator_call_applies(nodePool,context,i));
    /// We want the operator that consumes the most tokens/expressions/nodes (maximal munch). Then we want to prioritize the one in the lowest namespace (shadowing).
    symbol_table& sym_tbl = **(context.end()-1);
    std::vector<symbol> op_list = sym_tbl.get_ops();
    int max = 0;
    
    for(auto S:op_list) max = std::max(max,op_applies(S,sym_tbl,nodePool,i));
    
    // grab the first one to hit the max, that should be the lowest scope match. (Hopefully only 1 match each time.)
    AST_node* op;
    for(auto S:op_list) if(max==op_applies(S,sym_tbl,nodePool,i)){ op = S.value;break;};
    
    assert(is_operator_declaration(op)&&"How can we be folding an operator call if we don't have a declaration for it?");
    
    if((*(op->children.end()-1))->text=="{")// should have a safety assert here to make sure the size is at least 2.
        op=*(op->children.end()-2);
    else
        op=*(op->children.end()-1);
    
    // grab all types & strings. Might be worth dereferencing the pointer into a local variable to ingore threading concerns.
    std::vector<AST_node*> tokens;
    for(AST_node* c:op->children){
        if(c->type==node_t::operator_id||c->type==node_t::id){
            tokens.push_back(c);
        }
        if(c->text=="("){
            if(c->children.size()>=1&&c->children[0]->text==","){
                //c->print_with_types();
                assert(c->children.size()==1&&"Comma's shouldn't have siblings.\n");
                c=c->children[0];
            }
            
            for( int i = 0; i < c->children.size()-1; i++ ){
                tokens.push_back(c->children[i]);
            }
        }
    }
    
    AST_node* call = new AST_node;
    
    *call = *op;
    call->children={};
    
    for(int q = 0; q < max; q++)
        call->children.push_back(nodePool[i+q]);
    
    nodePool.erase(nodePool.begin()+i,nodePool.begin()+i+max);
    nodePool.insert(nodePool.begin()+i,call);
    //std::exit(-1);
}


