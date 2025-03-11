#include "parse.h"
#include <assert.h>
#include <iostream>

std::string indent = "";
void AST_node::print(){
    std::cout << indent << text << "\n";
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ )
        children[i].print();
    indent.resize(indent.size()-4);
}

void AST_node::print_with_types(){
    std::cout << indent << type << "" << text << "\n";
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ )
        children[i].print();
    indent.resize(indent.size()-4);
}


