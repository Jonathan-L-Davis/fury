#include "type_util.h"
#include "parse_util.h"

#include <assert.h>
#include <algorithm>
#include <iterator>


std::vector<std::string> get_function_param_types( const AST_node * const knowMe ){
    std::vector<std::string> retMe;
    
    //knowMe->print_with_types();
    assert(is_function_declaration(knowMe));
    
    int idx = 1783793664;// massive bad value to blow up if not redefined
    if(is_function_definition(knowMe))
        idx = knowMe->children.size()-2;
    else
        idx = knowMe->children.size()-1;
    
    for( int i = 0; i < knowMe->children[idx]->children[0]->children.size()-1; i++ )
        retMe.push_back( knowMe->children[idx]->children[0]->children[i]->text );
    
    return retMe;
}

std::vector<std::string> get_operator_param_types( const AST_node * const checkMe ){
    std::vector<std::string> retMe;
    
    //knowMe->print_with_types();
    //assert(is_operator_declaration(checkMe));
    
    const AST_node* o_id;
    for( int i = 0; i < checkMe->children.size(); i++ ){
        if( checkMe->children[i]->type == node_t::operator_id ){
            o_id = checkMe->children[i];
        }
    }
    
    for(auto n : o_id->children){
        if(n->text=="(" && n->children.size()>1){
            retMe.push_back("(");
            if(n->children[0]->text==",")
                for(auto m:n->children)
                    retMe.push_back(m->text);
            retMe.push_back(")");
        }else{
            retMe.push_back(n->text);
        }
    }
    
    return retMe;
}

std::set<std::string> get_type(const AST_node * const typeMe, symbol_table& context){
        
    if(is_if_statement(typeMe))
        return get_type_of_if(typeMe,context);
    
    if(is_for_loop(typeMe))
        return get_type_of_for(typeMe,context);
    
    if(is_while_loop(typeMe))
        return get_type_of_while(typeMe,context);
    
    if(is_operator_call(typeMe,&context))
        return get_type_of_operator_call(typeMe,context);
    
    if(is_function_call(typeMe,&context))
        return get_type_of_operator_call(typeMe,context);
    
//    if(is_syntax_call(typeMe))
//        return get_type_of_syntax_call(typeMe,context);
    
    if(is_id(typeMe,&context))
        return get_type_of_id(typeMe,context);
    
    if(is_closed_parenthesis(typeMe))
        return get_type_of_parenthesis(typeMe,context);
    
    if(is_closed_curly_bracket(typeMe))
        return get_type_of_curly_bracket(typeMe,context);
    
    if(typeMe->text==";")
        return {};
    
    typeMe->print_with_types();
    context.print();
    assert(false&&"Failed to find type of expression.");
}

std::set<std::string> get_type_of_if(const AST_node * const typeMe, symbol_table& context){
    assert(is_if_statement(typeMe));
    
    if(typeMe->children.size()==1)
        return {};
    
    if(!is_if_else_statement(typeMe))
        return get_type(*(typeMe->children.end()-1),context);
    
    std::set<std::string> s1 = get_type(typeMe->children[2],context);
    std::set<std::string> s2 = get_type((*(typeMe->children.end()-1))->children[0],context);
    
    s1.insert(s2.begin(),s2.end());
    
    return s1;
}

std::set<std::string> get_type_of_for(const AST_node * const typeMe, symbol_table& context){
    assert(is_for_loop(typeMe));
    return get_type(typeMe->children[3],context);// should really pass the context for the for loop, but we don't have that working just yet.
}

std::set<std::string> get_type_of_while(const AST_node * const typeMe, symbol_table& context){
    assert(is_while_loop(typeMe));
    assert(typeMe->children.size()<=2);
    if(typeMe->children.size()==2)
        return get_type(typeMe->children[1],context);// should really pass the context for the for loop, but we don't have that working just yet.
    return {};
}

std::set<std::string> get_type_of_function_call(const AST_node * const typeMe, symbol_table& context){
    return {};
}

std::set<std::string> get_type_of_operator_call(const AST_node * const typeMe, symbol_table& context){
    return {};
}

std::set<std::string> get_type_of_syntax_call(const AST_node * const typeMe, symbol_table& context){
    return {};
}

std::set<std::string> get_type_of_id(const AST_node * const typeMe, symbol_table& context){
    return {};// this is why we need the context, ugh, it's not hard, just have to implement a look up in the symbol table.
}

std::set<std::string> get_type_of_parenthesis(const AST_node * const typeMe, symbol_table& context){
    assert(is_closed_parenthesis(typeMe));
    if(typeMe->children.size()>1)
        return get_type(typeMe->children[0],context);
    return {};
}

std::set<std::string> get_type_of_curly_bracket(const AST_node * const typeMe, symbol_table& context){
    assert(is_closed_curly_bracket(typeMe));
    if(typeMe->children.size()>1)
        return get_type(typeMe->children[typeMe->children.size()-2],context);
    return {};
}
