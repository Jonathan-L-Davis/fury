#include "node.h"
#include <assert.h>
#include <iostream>
#include <iomanip>
#include "symbol_table.h"

std::string indent = "";
void AST_node::print() const {
    std::cout << indent << text << "\n";
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ )
        children[i]->print();
    indent.resize(indent.size()-4);
}

void AST_node::print_with_types() const{
    std::ios_base::fmtflags stream_state( std::cout.flags() );
    
    std::cout << std::left << std::setw(12) << type;
    std::cout.flags( stream_state );
    std::cout << "|" << indent << text << "\n";
    
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ )
        children[i]->print_with_types();
    indent.resize(indent.size()-4);
}

void AST_node::print_with_types(symbol_table* S) const{
    std::ios_base::fmtflags stream_state( std::cout.flags() );
    
    std::cout << std::left << std::setw(12) << type;
    auto n = S->get_type(this);
    std::cout.flags( stream_state );
    std::cout << "|" << std::setw(12) << (n?n->text:"N/A");
    delete n;
    std::cout.flags( stream_state );
    std::cout << "|" << indent << text << "\n";
    
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ ){
        children[i]->print_with_types(S);
        
    }
    indent.resize(indent.size()-4);
}
