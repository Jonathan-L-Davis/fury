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
        case Operator:{
            if( frisk_me.tok.text == ";" ){
                    
            }else assert(false);
        }break;
        case binary_operator:{
            
            for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
                anal_expression( frisk_me.children[i], fill_me );
            std::cout << "operator scoping\n";
            
        }break;
        case identifier:{
            assert( fill_me.contains_id( frisk_me.tok.text ) );
            for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
                anal_expression( frisk_me.children[i], fill_me );
        }break;
        case literal:{
            
        }break;
        case type:{
            anal_typed_declaration(frisk_me,fill_me);
        }break;
        default: std::cout << frisk_me.tok.text << "\n",assert(false);
    }
    
}

void anal_function( const AST_node& frisk_me, symbol_table& fill_me ){
    const AST_node* start_node = &frisk_me;
    const AST_node* active = start_node;
    
    // pick off function keyword
    if( active->tok.text == "function" ){
        if( active->children.size() ){
            active = &active->children[0];
        }else assert(false);
    }else assert(false); // why was this function called?
    // conditionally pick off function return type
    // pick off function name
    symbol function_symbol;
    if( active->tok.type == identifier ){
        function_symbol = { "function", active->tok.text, (AST_node*)(void*)&frisk_me };
        fill_me.add_symbol( function_symbol );
        fill_me.add_scope( active->tok.text );
    }
    
}

void anal_typed_declaration( const AST_node& frisk_me, symbol_table& fill_me ){
    const AST_node* active = &frisk_me;
    
    std::string type;
    std::string variable_id;
    AST_node* value;
    
    if( active->tok.type == ::type ){
        type = active->tok.text;
        active = &frisk_me.children[0];
    }
    else assert(false);
    
    if( active->tok.type == identifier ){
        variable_id = active->tok.text;
        active = &frisk_me.children[1];
    }else assert(false);
    
    //*
    if( active->tok.text != ";" ){
        value = (AST_node*)(void*)active;
    }//*/
    
    if( !fill_me.contains_id( variable_id ) ){
        fill_me.add_symbol({ type, variable_id, value });
    }else assert(false);
    
    anal_expression( *active ,fill_me.sub_scopes[fill_me.sub_scopes.size()-1]);
    
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
    
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name == find_me )
            return true;
    }
    
    if( parent == nullptr ) return false;
    return parent->contains_id(find_me);
    
}
