#include "type_util.h"
#include "parse_util.h"

#include <assert.h>



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
    if(typeMe->text=="operator")
        return get_operator_declaration_type(typeMe,context);
        
    
    
    typeMe->print_with_types();
    context.print();
    assert(false&&"Failed to find type of expression.");
}

std::set<std::string> get_operator_declaration_type(const AST_node * const typeMe, symbol_table& context){
    std::set<std::string> retMe;
    
    if(typeMe->children.size()>0){
        if(typeMe->children[0]->type==node_t::type){
            retMe.insert(typeMe->children[0]->text);
        }
    }
    
    return retMe;
}
