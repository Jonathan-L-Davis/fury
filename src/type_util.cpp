#include "type_util.h"
#include "parse_util.h"

#include <assert.h>

std::vector<std::string> get_function_param_types( const AST_node * const knowMe ){
    std::vector<std::string> retMe;
    
    //knowMe->print_with_types();
    assert(is_function_declaration(knowMe));
    
    int idx = 500000000000;// massive bad value to blow up if not redefined
    if(is_function_definition(knowMe))
        idx = knowMe->children.size()-2;
    else
        idx = knowMe->children.size()-1;
    
    for( int i = 0; i < knowMe->children[idx]->children[0]->children.size()-1; i++ )
        retMe.push_back( knowMe->children[idx]->children[0]->children[i]->text );
    
    return retMe;
}
