#include "symbol_table.h"
#include <assert.h>
#include <iostream>

void symbol_table::add_symbol( symbol add_me ){
    
    for( unsigned int i = 0; i < sub_scopes.size(); i++){
        if( sub_scopes[i].scope == add_me.name ){
            assert(false); // no
        }
    }
    
    for( unsigned int i = 0; i < symbols.size(); i++){
        if( symbols[i].name == add_me.name ){
            assert(false); // no
        }//will have to handle function/operator overloading here eventually.
    }
    
    // symbol does not exist in this scope yet
    symbols.push_back(add_me);
    
}

void symbol_table::add_scope(std::string scope,scope_type type ){
    sub_scopes.push_back( symbol_table(this,scope,type) );
}

std::string symbol_table::get_full_scope(){//returns absolute scope
    
    if ( scope != "" ){
        return parent->get_full_scope() + "::" + scope;
    }
    return scope;
}

symbol_table::symbol_table(){
    parent = nullptr;
    this->scope = "";
}

symbol_table::symbol_table( symbol_table* parent_ptr, std::string scope, scope_type type ){
    parent = parent_ptr;
    this->scope = scope;
    this->type = type;
}

std::string sym_tbl_indent = "";
void symbol_table::print(){
    std::cout << sym_tbl_indent << get_full_scope() << "\n";
    sym_tbl_indent += "    ";
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        std::cout << sym_tbl_indent; symbols[i].print();
    }
    
    for( unsigned int i = 0; i < sub_scopes.size(); i++ )
        sub_scopes[i].print();
    sym_tbl_indent.resize(sym_tbl_indent.size()-4);
}

void symbol::print(){
    std::cout << "|" << type << "|" << name << "|" << value << "|\n";
}

// is id in local scope
bool symbol_table::contains_id(std::string find_me){
    
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name == find_me )
            return true;
    }
    
    return false;
    
}

// is id in any scope
bool symbol_table::id_exists(std::string find_me){
    
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name == find_me )
            return true;
    }
    
    if( scope != "" ) return parent->id_exists(find_me);
    return false;
    
}

bool symbol_table::id_starts_with_substr(std::string find_me){
    
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name.size() >= find_me.size() && symbols[i].name.substr(0,find_me.size()) == find_me )
            return true;
    }
    
    if( scope != "" ) return parent->id_starts_with_substr(find_me);
    return false;
    
}

bool symbol_table::type_starts_with_substr(std::string find_me){
    
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name.size() >= find_me.size() && symbols[i].name.substr(0,find_me.size()) == find_me  )
            return true;
    }
    
    if( scope != "" ) return parent->id_starts_with_substr(find_me);
    return false;
    
}

bool symbol_table::contains_scope(std::string find_me){
    
    for( unsigned int i = 0; i < sub_scopes.size(); i++ ){
        if( sub_scopes[i].scope == find_me )
            return true;
    }
    
    return false;
}

bool symbol_table::scope_exists(std::string find_me){
    
    for( unsigned int i = 0; i < sub_scopes.size(); i++ ){
        if( sub_scopes[i].scope == find_me )
            return true;
    }
    
    if( scope != "" ) return parent->contains_scope(find_me);
    return false;
}
