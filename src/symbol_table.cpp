#include "symbol_table.h"
#include <assert.h>
#include <iostream>



type_type scope_expression( const AST_node& frisk_me, symbol_table& fill_me );
type_type scope_function( const AST_node& frisk_me, symbol_table& fill_me );
type_type scope_typed_declaration( const AST_node& frisk_me, symbol_table& fill_me );
type_type scope_if( const AST_node& frisk_me, symbol_table& fill_me );
type_type scope_while( const AST_node& frisk_me, symbol_table& fill_me );
type_type scope_for( const AST_node& frisk_me, symbol_table& fill_me );
//type_type scope_goto( const AST_node& frisk_me, symbol_table& fill_me );//not yet

type_type type_expression( const AST_node& frisk_me, symbol_table& fill_me );
type_type type_function( const AST_node& frisk_me, symbol_table& fill_me );
type_type type_typed_declaration( const AST_node& frisk_me, symbol_table& fill_me );
type_type type_if( const AST_node& frisk_me, symbol_table& fill_me );
type_type type_while( const AST_node& frisk_me, symbol_table& fill_me );
type_type type_for( const AST_node& frisk_me, symbol_table& fill_me );
//type_type type_goto( const AST_node& frisk_me, symbol_table& fill_me );//not yet

type_type anal_expression( const AST_node& frisk_me, symbol_table& fill_me );
type_type anal_function( const AST_node& frisk_me, symbol_table& fill_me );
type_type anal_typed_declaration( const AST_node& frisk_me, symbol_table& fill_me );
type_type anal_if( const AST_node& frisk_me, symbol_table& fill_me );
type_type anal_while( const AST_node& frisk_me, symbol_table& fill_me );
type_type anal_for( const AST_node& frisk_me, symbol_table& fill_me );
//type_type anal_goto( const AST_node& frisk_me, symbol_table& fill_me );//not yet

symbol_table anal( const AST_node& frisk_me ){
    symbol_table fill_me;
    fill_me.scope = "";
    
    for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
        scope_expression( frisk_me.children[i], fill_me );//each child should be a separate expression.
    
    return fill_me;
}

/****************************************************************************\
|                               ANALYSIS RULES                               |
\****************************************************************************/

type_type scope_expression( const AST_node& frisk_me, symbol_table& fill_me ){
    
    type_type retMe = semantic_epsilon;
    
    switch(frisk_me.tok.type){
        case keyword:{
            if( frisk_me.tok.text == "function" ){
                retMe = scope_function(frisk_me,fill_me);
            }else
            if( frisk_me.tok.text == "if" ){
                scope_if(frisk_me,fill_me);
            }else
            if( frisk_me.tok.text == "for" ){
                scope_for(frisk_me,fill_me);
            }else
            if( frisk_me.tok.text == "while" ){
                scope_while(frisk_me,fill_me);
            }else assert(false);
        }break;
        case Operator:{
            if( frisk_me.tok.text == ";" ){
                    
            }else assert(false);
        }break;
        case scoping:{
            
            switch(fill_me.type){
                
                case scope_t_function:{//handled in scope_function
                }break;
                case scope_t_if:{//handled in scope_if
                }break;
                case scope_t_for:{
                }break;
                case scope_t_while:{
                }break;
                case scope_t_anonymous:{
                }break;
                default: assert(false);
            }
            
            if( frisk_me.tok.text == "{" ){
                for( unsigned int i = 0; frisk_me.children.size() > i && frisk_me.children[i].tok.text != "}"; i++ ){
                    scope_expression(frisk_me.children[i],fill_me);
                }
            }else assert(false);
        }break;
        case binary_operator:{
            
            for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
                scope_expression( frisk_me.children[i], fill_me );
            
        }break;
        case identifier:{
            assert( fill_me.id_exists( frisk_me.tok.text ) );
            for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
                scope_expression( frisk_me.children[i], fill_me );//*/
        }break;
        case literal:{
            // idk fam, probably wont be built into the lang, or will be really minimal
        }break;
        case type:{//*
            retMe = scope_typed_declaration(frisk_me,fill_me);//*/
        }break;
        case paren:{//*
            //for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
                //anal_typed_declaration(frisk_me.children[i],fill_me);//*/
        }break;
        default: {std::cout << frisk_me.tok.text << "\n";std::cout << frisk_me.tok.line_no << "\nError during scope analysis.";assert(false);}break;
    }
    
    
    return retMe;
}

type_type scope_function( const AST_node& frisk_me, symbol_table& fill_me ){
    
    type_type retMe = semantic_epsilon;
    
    const AST_node* start_node = &frisk_me;
    const AST_node* active = start_node;
    
    // pick off function keyword
    if( active->tok.text == "function" ){
        if( active->children.size() ){
            active = &active->children[ active->children.size()-2 ];
        }else assert(false);
    }else assert(false); // why was this function called?
    // conditionally pick off function return type
    // pick off function name
    symbol function_symbol;
    if( active->tok.type == identifier ){
        function_symbol = { "function", active->tok.text, (AST_node*)(void*)&frisk_me };
        //retMe = ;
        fill_me.add_symbol( function_symbol );
        fill_me.add_scope( active->tok.text, scope_t_function );
    }else assert(false);
    
    const AST_node* param_pack = &active->children[0];
    assert( param_pack->tok.text == "("  );
    
    for( unsigned int i = 0; param_pack->children.size() > i && param_pack->children[i].tok.text != ")"; i++ ){
        fill_me.sub_scopes[fill_me.sub_scopes.size()-1].add_symbol( {param_pack->children[i].tok.text ,param_pack->children[i].children[0].tok.text ,(AST_node*)(void*)&param_pack->children[i]} );
    }
    
    const AST_node* function_body = &active->children[1];
    type_type body_type = scope_expression(*function_body, fill_me.sub_scopes[fill_me.sub_scopes.size()-1] );
    
    assert( retMe == body_type );
    
    return retMe;
}

type_type scope_typed_declaration( const AST_node& frisk_me, symbol_table& fill_me ){

    type_type retMe = semantic_epsilon;

    const AST_node* active = &frisk_me;
    bool is_assignment = false;
    
    std::string type;
    std::string variable_id;
    AST_node* value;
    
    if( active->tok.type == ::type ){
        type = active->tok.text;
        active = &frisk_me.children[0];
        
        if( type == "byte" )
            retMe = semantic_byte;
        if( type == "dual" )
            retMe = semantic_dual;
        if( type == "quad" )
            retMe = semantic_quad;
        if( type == "oct" )
            retMe = semantic_oct;
        
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
    if ( is_assignment );
    assert( retMe == scope_expression( *active ,fill_me.sub_scopes[fill_me.sub_scopes.size()-1]) );
    assert( retMe != semantic_epsilon );
    
    return retMe;
}

type_type scope_if( const AST_node& frisk_me, symbol_table& fill_me ){

    type_type retMe = semantic_epsilon;

    const AST_node* active = &frisk_me;
    if( frisk_me.tok.text == "if" ){
        assert(frisk_me.children.size()>0);
        active = &frisk_me.children[0];
        std::string scope_name = ":if";
        bool name_exists = true;
        int i = 0;
        while(name_exists){
            
            if( !fill_me.contains_scope( scope_name + std::to_string(i) ) ){
                name_exists = false;
                fill_me.add_scope( scope_name + std::to_string(i), scope_t_if );
            }
            i++;
        }
    }else assert(false);
    
    if( active->tok.text == "(" ){
        assert(active->children.size()>0);
        scope_expression(active->children[0],fill_me.sub_scopes[fill_me.sub_scopes.size()-1]);
    }else assert(false);
    if( frisk_me.children.size() == 2 )
        scope_expression(frisk_me.children[1],fill_me.sub_scopes[fill_me.sub_scopes.size()-1]);
    else {
        assert( frisk_me.children.size() == 3 );
        scope_expression(frisk_me.children[1],fill_me.sub_scopes[fill_me.sub_scopes.size()-1]);//true clause
        scope_expression(frisk_me.children[2].children[0],fill_me.sub_scopes[fill_me.sub_scopes.size()-1]);//false clause
    }
    
    return retMe;
}

type_type scope_while( const AST_node& frisk_me, symbol_table& fill_me ){
    const AST_node* active = &frisk_me;
    
    if( frisk_me.tok.text == "while" ){
        active = (const AST_node*)(void*)(&active->children[0]);
        std::string scope_name = ":while";
        bool name_exists = true;
        int i = 0;
        while(name_exists){
            
            if( !fill_me.contains_scope( scope_name + std::to_string(i) ) ){
                name_exists = false;
                fill_me.add_scope( scope_name + std::to_string(i), scope_t_for );
            }
            i++;
        }
    }else assert(false);
    
    if( active->tok.text == "(" ){
        
    }else assert(false);
    scope_expression( active->children[0], fill_me.sub_scopes[fill_me.sub_scopes.size()-1] );// conditional
    
    return scope_expression( frisk_me.children[1], fill_me.sub_scopes[fill_me.sub_scopes.size()-1] );// body
}

type_type scope_for( const AST_node& frisk_me, symbol_table& fill_me ){

    type_type retMe;
    const AST_node* active = &frisk_me;
    
    if( frisk_me.tok.text == "for" ){
        active = (const AST_node*)(void*)(&active->children[0]);
        std::string scope_name = ":for";
        bool name_exists = true;
        int i = 0;
        while(name_exists){
            
            if( !fill_me.contains_scope( scope_name + std::to_string(i) ) ){
                name_exists = false;
                fill_me.add_scope( scope_name + std::to_string(i), scope_t_for );
            }
            i++;
        }
    }else assert(false);
    
    if( active->tok.text == "(" ){
        
    }else assert(false);
    scope_expression( active->children[0], fill_me.sub_scopes[fill_me.sub_scopes.size()-1] );// first for field, variable declaration
    
    retMe = scope_expression( frisk_me.children[1], fill_me.sub_scopes[fill_me.sub_scopes.size()-1] );// for body
    
    scope_expression( active->children[1], fill_me.sub_scopes[fill_me.sub_scopes.size()-1] );// second for field, conditional
    scope_expression( active->children[2], fill_me.sub_scopes[fill_me.sub_scopes.size()-1] );// third for field, iteration
    
    return retMe;
}

type_type scope_goto( const AST_node& frisk_me, symbol_table& fill_me ){
    //not implemented for v0.0
    return semantic_epsilon;
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

bool symbol_table::contains_id(std::string find_me){
    
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name == find_me )
            return true;
    }
    
    return false;
    
}

bool symbol_table::id_exists(std::string find_me){
    
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name == find_me )
            return true;
    }
    
    if( scope != "" ) return parent->id_exists(find_me);
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
