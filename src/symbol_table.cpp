#include "symbol_table.h"
#include <assert.h>
#include <iostream>



void anal_expression( const AST_node& frisk_me, symbol_table& fill_me );
void anal_function( const AST_node& frisk_me, symbol_table& fill_me );
void anal_typed_declaration( const AST_node& frisk_me, symbol_table& fill_me );
void anal_if( const AST_node& frisk_me, symbol_table& fill_me );
void anal_while( const AST_node& frisk_me, symbol_table& fill_me );
void anal_for( const AST_node& frisk_me, symbol_table& fill_me );
//void anal_goto( const AST_node& frisk_me, symbol_table& fill_me );//not yet

symbol_table anal( const AST_node& frisk_me ){
    symbol_table fill_me;
    fill_me.scope = "";
    
    for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
        anal_expression( frisk_me.children[i], fill_me );//each child should be a separate expression.
    
    return fill_me;
}

void anal_expression( const AST_node& frisk_me, symbol_table& fill_me ){
    
    switch(frisk_me.tok.type){
        case keyword:{
            if( frisk_me.tok.text == "function" ){
                anal_function(frisk_me,fill_me);
            }
        }break;
        case type:{
            anal_typed_declaration(frisk_me,fill_me);
        }break;
        default: assert(false);
    }
    
}

void anal_function( const AST_node& frisk_me, symbol_table& fill_me ){
    const AST_node* start_node = &frisk_me;
    const AST_node* current_node = start_node;
    
    // pick off function keyword
    if( current_node->tok.text == "function" ){
        current_node = &current_node->children[0];
    }else assert(false); // why was this function called?
    // conditionally pick off function return type
    // pick off function name
    if( current_node->tok.type == identifier ){
        symbol temp = { "function", current_node->tok.text, "" };
        fill_me.add_symbol( temp );
        current_node = &current_node->children[0];
    }
    // pick off 
    
}

void anal_typed_declaration( const AST_node& frisk_me, symbol_table& fill_me ){
    const AST_node* active = &frisk_me;
    
    std::string variable_id = "";
    if( fill_me.contains_id( variable_id ) ){
        
    }
}

void anal_if( const AST_node& frisk_me, symbol_table& fill_me ){
    
}

void anal_while( const AST_node& frisk_me, symbol_table& fill_me ){
    
}

void anal_for( const AST_node& frisk_me, symbol_table& fill_me ){
    
}

void anal_goto( const AST_node& frisk_me, symbol_table& fill_me ){
    //not implemented for v0.0
}

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

void symbol_table::add_scope(std::string scope){
    sub_scopes.push_back( symbol_table(this,scope) );
}

std::string symbol_table::get_full_scope(){//returns absolute scope
    std::string retMe;
    
    retMe = scope;
    for( symbol_table* scope_ptr = this->parent; scope_ptr != nullptr; scope_ptr = scope_ptr->parent ){
        retMe = scope_ptr->scope + retMe;
    }
    
    return retMe;
}

symbol_table::symbol_table(){
    parent = nullptr;
    this->scope = "";
}

symbol_table::symbol_table( symbol_table* parent_ptr, std::string scope ){
    parent = parent_ptr;
    this->scope = scope;
}

std::string sym_tbl_indent = "";
void symbol_table::print(){
    std::cout << sym_tbl_indent << "\"" << scope << "\"\n";
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

bool symbol_table::contains_id(std::string find_me){
    
    bool retMe = false;
    
    for( int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name == find_me )
            retMe = true;
    }
    
    return retMe || parent?parent->contains_id( find_me ):false;
}
