#include "parse_util.h"
#include "type_util.h"
#include "assert.h"


///-////////////////////////////////////////////////// These functions do not modify AST_nodes and are pure checks.  ////////////////////////////////////////////////////////////////////////////////

/**
 * 
 *  A node is terminated IFF it's right most bottom most child node is a semicolon ';'.
 * 
**/
bool is_terminated(const AST_node* const amITerminated){
    
    const AST_node* const checkMe = get_rightmost_bottommost(amITerminated);
    
    if( checkMe->text == ";" ){
        assert( checkMe->children.size() == 0 );// terminal tokens should have no children by definition. -- this triggering indicates a compiler error
        return true;
    }
    
    return false;
}

/// need a valid_parenthesis & valid_curly_bracket concept.

bool is_closed_parenthesis(const AST_node* checkMe){
    
    if( checkMe->children.size()>0 && checkMe->children[checkMe->children.size()-1]->text == ")"  ) return true;
    return false;
}

bool is_closed_curly_bracket(const AST_node* const amIClosed){
    
    const AST_node* const checkMe = get_rightmost_bottommost_non_terminal(amIClosed);// lazy, should do the one layer end checking.
    if( amIClosed->text != "{" ) return false;
    if( checkMe->text == "}" ){
        return true;
    }
    
    return false;
}

bool is_unterminated_closed_parentehsis(const AST_node* const amIClosed){
    
    const AST_node* const checkMe = get_rightmost_bottommost(amIClosed);
    if( amIClosed->text != "(" ) return false;
    if( checkMe->text == ")" ){
        return true;
    }
    
    return false;
}

/**
 * 
 *  A node is terminable IFF it would be valid to add a semicolon ';' to the right most bottom most child node.
 * 
**/
bool is_terminable(const AST_node* const checkMe, const symbol_table* const context){
    return !is_terminated(checkMe) && is_valid(checkMe,context);
}

/**
 * 
 *  A node is valid if it is a 'complete' Fury expression
 * 
**/
bool is_valid(const AST_node* const checkMe, const symbol_table* const context){
    
    if(is_function_declaration(checkMe))
        return true;
    
    if(is_function_definition(checkMe))
        return true;
    
    if(is_function_call(checkMe,context))
        return true;
    
    if(is_operator_declaration(checkMe))
        return true;
    
    if(is_operator_definition(checkMe))
        return true;
    
    //if(is_operator_call(checkMe,context))
    //    return true;
    
    if(is_syntax_declaration(checkMe))
        return true;
    
    if(is_type_declaration(checkMe,context))
        return true;
    
    if(is_comma_expression(checkMe))
        return true;
    
    if(is_closed_parenthesis(checkMe))
        return true;
    
    if(is_closed_curly_bracket(checkMe))
        return true;
    
    if(is_return_statement(checkMe,context))
        return true;
    
    if(is_id(checkMe,context)&&checkMe->type!=node_t::function_id)
        return true;
    
    if(is_if_statement(checkMe))
        return true;
    
    if(is_if_else_statement(checkMe))
        return true;
    
    if(is_for_loop(checkMe))
        return true;
    
    if(is_while_loop(checkMe))
        return true;
    
    return false;
}

/**
 * 
 *  A function definition should also pass this check. A definition is defined as a declaration & a body.
 * 
**/
bool is_function_declaration(const AST_node* const checkMe){
    
    //using comments for pictures of tree structure after each check.
    
    if( checkMe->text != "function" ){
        return false;
    }
    
    /**
    * 
    * function
    * 
    **/
    if( checkMe->children.size() == 0 ){
        return false;
    }
    
    /**
    * 
    * function
    *     ...
    **/
    
    int f_id_count = 0;
    const AST_node* f_id;
    for( int i = 0; i < checkMe->children.size(); i++ ){
        if( checkMe->children[i]->type == node_t::function_id ){
            f_id_count++;
            f_id = checkMe->children[i];
            assert(i>=0 && i < 2);// only the type can come before the id
        }
    }
    
    assert(f_id_count == 1);
    
    /**
    * 
    * function
    *     [return type]
    *     f_id
    *     [function body]
    **/
    
    if(f_id->children.size()!=1 || f_id->children[0]->text != "(") return false;
    
    /**
    * 
    * function
    *     [return type]
    *     f_id
    *         (
    *     [function body]
    **/
    
    if( ( f_id->children[0]->children.size()==1 && f_id->children[0]->children[0]->text == ")" ) || ( f_id->children[0]->children.size()==2 && f_id->children[0]->children[1]->text == ")" ) ) return true;
    
    /**
    * 
    * function
    *     [return type]
    *     f_id
    *         (
    *             )
    *     [function body]
    **/
    return false;
}

bool is_function_partial_declaration(const AST_node* const checkMe){
    
    //using comments for pictures of tree structure after each check.
    
    if( checkMe->text != "function" ){
        return false;
    }
    /**
    * 
    * function
    * 
    **/
    if( checkMe->children.size() == 0 ){
        return false;
    }
    
    /**
    * 
    * function
    *     ...
    **/
    
    int f_id_count = 0;
    int f_id_idx = 0;
    const AST_node* f_id;
    for( int i = 0; i < checkMe->children.size(); i++ ){
        if( checkMe->children[i]->type == node_t::function_id ){
            f_id_count++;
            f_id = checkMe->children[i];
            f_id_idx = i;
            assert(i>=0 && i < 2);// only the type can come before the id
            if(i!=checkMe->children.size()-1) return false;// id is the last thing in the function partial. No body or arguments yet.
        }
    }
    
    if( f_id_idx == 1 )
        assert( checkMe->children[0]->type == node_t::type );
    
    /**
    * 
    * function
    *     [return type]
    *     f_id
    **/
    assert(f_id_count == 1);
    
    if(f_id->children.size()!=0) return false;
    
    return true;
}

/**
 * 
 *  A definition is defined as a declaration & a body/{}.
 * 
**/
bool is_function_definition(const AST_node* const checkMe){
    if(!is_function_declaration(checkMe))
        return false;
    if(checkMe->children[ checkMe->children.size()-1 ]->text != "{")
        return false;
    if( is_closed_curly_bracket(checkMe->children[ checkMe->children.size()-1 ]) )
        return true;
    return false;
}

bool is_function_call(const AST_node* const checkMe, const symbol_table* const context){
    
    if( !context->function_id_exists(checkMe->text) )
        return false;
    
    if( checkMe->children.size() != 1 )
        return false;
    
    if( checkMe->children[0]->text != "(" )
        return false;
    
    assert( checkMe->children[0]->children.size() > 0 );
    
    auto k = checkMe->children[0]->children.size()-1;
    assert(checkMe->children[0]->children[k]->text == ")");
    
    return true;
}

bool is_syntax_declaration(const AST_node* const checkMe){
    
    //using comments for pictures of tree structure after each check.
    
    if( checkMe->text != "syntax" ){
        return false;
    }
    
    /**
    * 
    * function
    * 
    **/
    if( checkMe->children.size() == 0 ){
        return false;
    }
    
    /**
    * 
    * function
    *     ...
    **/
    
    int s_id_count = 0;
    const AST_node* s_id;
    for( int i = 0; i < checkMe->children.size(); i++ ){
        if( checkMe->children[i]->type == node_t::syntax_id ){
            s_id_count++;
            s_id = checkMe->children[i];
            assert(i>=0 && i < 2);// only the type can come before the id
        }
    }
    
    assert(s_id_count == 1);
    
    /**
    * 
    * function
    *     [return type]
    *     f_id
    *     [function body]
    **/
    
    if(s_id->children.size()!=1 || s_id->children[0]->text != "(") return false;
    
    /**
    * 
    * function
    *     [return type]
    *     f_id
    *         (
    *     [function body]
    **/
    
    if( s_id->children[0]->children.size()==2 && s_id->children[0]->children[0]->text == "byte" && s_id->children[0]->children[1]->text == ")" ) return true;
    
    /**
    * 
    * function
    *     [return type]
    *     s_id
    *         (
    *             byte        
    *             )
    *     [function body]
    **/
    return false;
}

bool is_syntax_definition(const AST_node* const checkMe){
    if(!is_syntax_declaration(checkMe))
        return false;
    if(checkMe->children[ checkMe->children.size()-1 ]->text != "{")
        return false;
    if( is_closed_curly_bracket(checkMe->children[ checkMe->children.size()-1 ]) )
        return true;
    return false;
}


bool is_syntax_partial_declaration(const AST_node* const checkMe){
    
    //using comments for pictures of tree structure after each check.
    
    if( checkMe->text != "syntax" ){
        return false;
    }
    /**
    * 
    * syntax
    * 
    **/
    if( checkMe->children.size() == 0 ){
        return false;
    }
    
    /**
    * 
    * syntax
    *     ...
    **/
    
    int s_id_count = 0;
    int s_id_idx = 0;
    const AST_node* s_id;
    for( int i = 0; i < checkMe->children.size(); i++ ){
        if( checkMe->children[i]->type == node_t::syntax_id ){
            s_id_count++;
            s_id = checkMe->children[i];
            s_id_idx = i;
            assert(i>=0 && i < 2);// only the type can come before the id
            if(i!=checkMe->children.size()-1) return false;// id is the last thing in the function partial. No body or arguments yet.
        }
    }
    
    if( s_id_idx == 1 )
        assert( checkMe->children[0]->type == node_t::type );
    
    /**
    * 
    * syntax
    *     [return type]
    *     f_id
    **/
    assert(s_id_count == 1);
    
    if(s_id->children.size()!=0) return false;
    
    return true;
}

bool is_operator_partial_declaration(const AST_node* const checkMe){
    
    //using comments for pictures of tree structure after each check.
    
    if( checkMe->text != "operator" ){
        return false;
    }
    /**
    * 
    * operator
    * 
    **/
    if( checkMe->children.size() == 0 ){
        return false;
    }
    
    /**
    * 
    * operator
    *     ...
    **/
    
    int o_id_count = 0;
    int o_id_idx = 0;
    const AST_node* o_id;
    for( int i = 0; i < checkMe->children.size(); i++ ){
        if( checkMe->children[i]->type == node_t::operator_id ){
            o_id_count++;
            o_id = checkMe->children[i];
            o_id_idx = i;
            assert(i>=0 && i < 2);// only the type can come before the id
            if(i!=checkMe->children.size()-1) return false;// id is the last thing in the function partial. No body or arguments yet.
        }
    }
    
    if( o_id_idx == 1 )
        assert( checkMe->children[0]->type == node_t::type );
    
    /**
    * 
    * operator
    *     [return type]
    *     o_id
    **/
    assert(o_id_count == 1);
    
    if(o_id->children.size()!=0) return false;
    
    return true;
}

/**
 * 
 *  An operator definition should also pass this check. A definition is defined as a declaration & a body.
 * 
**/
bool is_operator_declaration(const AST_node* const checkMe){
    
    //using comments for pictures of tree structure after each check.
    
    if( checkMe->text != "operator" ){
        return false;
    }
    
    /**
    * 
    * operator
    * 
    **/
    if( checkMe->children.size() == 0 ){
        return false;
    }
    
    /**
    * 
    * operator
    *     ...
    **/
    
    int o_id_count = 0;
    const AST_node* o_id;
    for( int i = 0; i < checkMe->children.size(); i++ ){
        if( checkMe->children[i]->type == node_t::operator_id ){
            o_id_count++;
            o_id = checkMe->children[i];
            assert(i>=0 && i < 2);// only the type can come before the id
        }
    }
    
    assert(o_id_count == 1);
    
    /**
    * 
    * function
    *     [return type]
    *     o_id
    *         (
    *     [function body]
    **/
    
    /**
    * 
    * function
    *     [return type]
    *     o_id
    *         (
    *             )
    *     [function body]
    **/
    
    std::string total_text = o_id->text;
    std::string found_text;
    for(auto n:o_id->children) if(n->type==node_t::id) found_text+=n->text;
    
    //assert(total_text==found_text&&"Malformed operator declaration");
    
    return true;
}

bool is_operator_definition(const AST_node* const checkMe){
    if(!is_operator_declaration(checkMe))
        return false;
    if(checkMe->children[ checkMe->children.size()-1 ]->text != "{")
        return false;
    if( is_closed_curly_bracket(checkMe->children[ checkMe->children.size()-1 ]) )
        return true;
    return false;
}

bool is_if_statement(const AST_node* const checkMe){
    if(checkMe->text!="if")
        return false;
    
    if(checkMe->children.size()<1||checkMe->children.size()>3)
        return false;
    
    if(!is_closed_parenthesis(checkMe->children[0]))
        return false;
    
    // expression checks assume the validity of sub-expressions.
    //if(checkMe->children.size()>1 && !is_valid(checkMe->children[1],context)
    //    return false;
    
    
    return true;
}

bool is_if_else_statement(const AST_node* const checkMe){
    if(!is_if_statement(checkMe))
        return false;
    
    if( (*(checkMe->children.end()-1))->text!="else")
        return false;
    
    // expression checks assume the validity of sub-expressions. Don't know how I feel about that. Probably means I need to always propagate context, which does make the parse util api more consistent.
    //if(checkMe->children.size()>1 && !is_valid(checkMe->children[1],context)
    //    return false;
    
    return true;
}

bool is_for_loop(const AST_node* const checkMe){
    
    if( checkMe->text!="for" )
        return false;
    
    if( checkMe->children.size()!=4)
        return false;
    
    auto c = checkMe->children;
    if( !is_terminated(c[0]) || !is_terminated(c[1]) || is_terminated(c[2]) )
        return false;
    
    return true;
}

bool is_while_loop(const AST_node* const checkMe){
    if(checkMe->text!="while")
        return false;
    
    if(checkMe->children.size()<1||checkMe->children.size()>2)
        return false;
    
    return true;
}

bool is_return_statement(const AST_node* const checkMe, const symbol_table* const context){
    
    if(checkMe->text!="return")
       return false;
    
    if( checkMe->children.size()==1 && is_valid(checkMe->children[0],context) )
        return true;
    
    return false;
}

bool is_id( const AST_node* const checkMe, const symbol_table* const context){
    return context->id_exists(checkMe->text);
}

bool is_type_declaration(const AST_node* const checkMe, const symbol_table* const context){
    
        if( checkMe->type!=node_t::type )
        return false;
    
    if( !context->type_exists(checkMe->text) )
        return false;
    
    if( checkMe->children.size() != 1 || checkMe->children[0]->type != node_t::id )
        return false;
    
    return true;
}

bool is_empty_comma(const AST_node* const checkMe){
    if(checkMe->text != "," || checkMe->children.size()!=0)
        return false;
    
    return true;
}

bool is_comma_expression(const AST_node* const checkMe){
    if(checkMe->text != "," || checkMe->children.size()<2)// complete comma expressions must contain 2 or more sub-expressions.
        return false;
    
    return true;
}

bool is_operator_call(const AST_node* const checkMe, const symbol_table* const context){
    return false;
}

///-////////////////////////////////////////////////// These functions may modify AST_nodes or aren't pure checks.  ////////////////////////////////////////////////////////////////////////////////
AST_node* get_rightmost_bottommost( const AST_node* const traverseMe){
    
    AST_node* retMe = const_cast<AST_node*>(traverseMe);
    
    while(retMe->children.size()>0)
        retMe = retMe->children[retMe->children.size()-1];
    
    return retMe;
}

AST_node* get_rightmost_bottommost_non_terminal( const AST_node* const traverseMe){
    
    AST_node* retMe = const_cast<AST_node*>(traverseMe);
    
    while(retMe->children.size()>0){
        if( retMe->children[retMe->children.size()-1]->text == ";" )
            return retMe;
        retMe = retMe->children[retMe->children.size()-1];
    }
    
    return retMe;
}

std::string get_op_id(const AST_node* const idMe){
    assert(is_operator_declaration(idMe));
    
    if(is_operator_definition(idMe))
        return (*(idMe->children.end()-2))->text;
    return (*(idMe->children.end()-1))->text;
}

std::vector<std::string> get_op_signature(const AST_node* const signMe){
    std::vector<std::string> retMe;
    
    AST_node* op_id;
    if(is_operator_definition(signMe))
        op_id = (*(signMe->children.end()-2));
    else
        op_id = (*(signMe->children.end()-1));
    
    for( auto n:op_id->children ){
        if(n->text=="(")
            for(auto m:n->children)
                retMe.push_back(m->text);
        else
            retMe.push_back(n->text);
    }
    
    return retMe;
}

std::string get_func_id(const AST_node* const idMe){
    assert(is_function_declaration(idMe));
    
    if(is_function_definition(idMe))
        return (*(idMe->children.end()-2))->text;
    return (*(idMe->children.end()-1))->text;
}

std::vector<std::string> get_func_signature(const AST_node* const signMe){
    std::vector<std::string> retMe;
    
    AST_node* op_id;
    if(is_operator_definition(signMe))
        op_id = (*(signMe->children.end()-2));
    else
        op_id = (*(signMe->children.end()-1));
    
    for( auto n:op_id->children ){
        if(n->text=="(")
            for(auto m:n->children)
                retMe.push_back(m->text);
        else
            retMe.push_back(n->text);
    }
    
    return retMe;
}

void move_operator_param_declarations(const AST_node* const op_id, symbol_table& src, symbol_table& dst){
    for(int i = 0;i<op_id->children.size();i++){
        
        if(op_id->children[i]->text=="("){
            AST_node* params = op_id->children[i];
            if(params->children[0]->text==",")
                params=params->children[0];
            
            for(AST_node* node:params->children){
                if(node->text==")")break;
                std::vector<symbol> sym_list = src.get_symbol(node->children[0]->text);
                assert(sym_list.size()>0);
                symbol sym;
                bool match_exists = false;
                for(symbol S:sym_list){
                    if( S.type.size()==1 && *S.type.begin()==node->text ){
                        sym=S;
                        match_exists=true;
                    }
                }
                
                if(!match_exists)
                    assert(false&&"No matching symbol for this declaration.");
                
                dst.add_symbol(sym);
                src.remove_symbol(sym);
            }
        }
    }
}

std::vector<AST_node*> get_sub_declarations(const AST_node* const node, symbol_table& context){
    assert( is_closed_curly_bracket(node) || is_closed_parenthesis(node) || is_comma_expression(node) );
    
    std::vector<AST_node*> retMe;
    
    for(auto n:node->children){
        if(is_type_declaration(n,&context))
            retMe.push_back(n);
        if( is_closed_curly_bracket(n) || is_closed_parenthesis(n) || is_comma_expression(n) )// conveniently, all the structures are similar enough to have the same loop
            for(auto m:get_sub_declarations(n,context))
                retMe.push_back(m);
    }
    
    return retMe;
}

// I know this is a bad solution because it misses the nested {} & () & , expressions. It fails to completely handle declarations. I should handle {} recursively, and , & ( with a for loop.
void move_curly_bracket_declarations(const AST_node* const bracket, symbol_table& src, symbol_table& dst){
    assert(is_closed_curly_bracket(bracket));
    
    std::vector<AST_node*> declarations = get_sub_declarations(bracket,src);
    
    for(AST_node* node:declarations){
        if(node->text==")")break;
        std::vector<symbol> sym_list = src.get_symbol(node->children[0]->text);
        assert(sym_list.size()>0);
        symbol sym;
        bool match_exists = false;
        for(symbol S:sym_list){
            if( S.type.size()==1 && *S.type.begin()==node->text ){
                sym=S;
                match_exists=true;
            }
        }
        
        if(!match_exists)
            assert(false&&"No matching symbol for this declaration.");
        
        dst.add_symbol(sym);
        src.remove_symbol(sym);
    }
    
}
