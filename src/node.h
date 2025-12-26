#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <iostream>

enum class node_t: uint32_t {
    root,
    keyword,
    id,
    paren,
    curly,
    semicolon,
    comma,
    function_id,
    operator_id,
    syntax_id,
    label_id,
    type,
};

template<typename T>
std::basic_ostream<T>& operator << ( std::basic_ostream<T>& fillMe, node_t printMe){
    switch(printMe){
        case node_t::root        : fillMe << "root";        break;
        case node_t::keyword     : fillMe << "keyword";     break;
        case node_t::id          : fillMe << "id";          break;
        case node_t::paren       : fillMe << "paren";       break;
        case node_t::curly       : fillMe << "curly";       break;
        case node_t::semicolon   : fillMe << "semicolon";   break;
        case node_t::comma       : fillMe << "comma";       break;
        case node_t::function_id : fillMe << "function_id"; break;
        case node_t::operator_id : fillMe << "operator_id"; break;
        case node_t::syntax_id   : fillMe << "syntax_id";   break;
        case node_t::label_id    : fillMe << "label_id";    break;
        case node_t::type        : fillMe << "type";        break;
    }
    
    return fillMe;
}

struct AST_node{
    std::string text;
    uint32_t line_no;
    node_t type;
    std::vector<AST_node*> children;
    void print() const;
    void print_with_types() const;
};

#endif//NODE_H