#include "symbol_table.h"
#include <assert.h>

void anal( AST_node& frisk_me, symbol_table& fill_me ){



    // fill symbol_table
    if( frisk_me.tok.type == type || frisk_me.tok.type == keyword ){
        fill_me.symbols.push_back({"type",frisk_me.tok.text,""});
    }
    
    if( frisk_me.tok.text == ":root" ){
        ;// ?
    }
    
    //*
    for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
        anal( frisk_me.children[i], fill_me );//*/
    
    //do clean up
    
    return;
}


void symbol_table::add_symbol( symbol add_me ){
    
    for(int i = 0; i < sub_scopes.size(); i++){
        if( sub_scopes[i].scope == add_me.name ){
            assert(false); // no
        }
    }
    
    for(int i = 0; i < symbols.size(); i++){
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
    this->scope = nullptr;
}

symbol_table::symbol_table( symbol_table* parent_ptr, std::string scope ){
    parent = parent_ptr;
    this->scope = scope;
}
