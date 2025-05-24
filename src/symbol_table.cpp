#include "symbol_table.h"
#include "parse_util.h"
#include "type_util.h"
#include <assert.h>
#include <iostream>

void symbol_table::add_symbol( symbol add_me ){
    //*
    for( unsigned int i = 0; i < sub_scopes.size(); i++){
        if( sub_scopes[i].scope == add_me.name ){
            add_me.value->print_with_types();
            if( !(add_me.sym_type == sym_t_function || add_me.sym_type == sym_t_operator ) ){// basically allows overloading of overloadable functionals. The grammer rules have to be responsible and do their own validity checking.
                assert(false);
            }
        }
    }
    
    switch(add_me.sym_type){
        case sym_t_byte:{
            for( unsigned int i = 0; i < bytes.size(); i++){
                if( bytes[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            bytes.push_back(add_me);
        }break;
        case sym_t_dual:{
            for( unsigned int i = 0; i < duals.size(); i++){
                if( duals[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            duals.push_back(add_me);
        }break;
        case sym_t_quad:{
            for( unsigned int i = 0; i < quads.size(); i++){
                if( quads[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            quads.push_back(add_me);
        }break;
        case sym_t_oct:{
            for( unsigned int i = 0; i < octs.size(); i++){
                if( octs[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            octs.push_back(add_me);
        }break;
        case sym_t_struct:{
            for( unsigned int i = 0; i < structs.size(); i++){
                if( structs[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            structs.push_back(add_me);
        }break;
        case sym_t_function:{
            for( unsigned int i = 0; i < functions.size(); i++){
                if( functions[i].name == add_me.name ){
                    if( is_function_definition(add_me.value) && is_function_definition(functions[i].value) ){
                        assert(false);// defining a previously defined function.
                    }else{
                        functions[i] = add_me;// need to handle namespaces for function overloads, can't support overloads correctly until that happens
                        break;
                    }
                }
            }
            
            functions.push_back(add_me);
        }break;
        case sym_t_operator:{
            for( unsigned int i = 0; i < operators.size(); i++){
                if( operators[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            operators.push_back(add_me);
        }break;
        case sym_t_syntax:{
            for( unsigned int i = 0; i < syntaxes.size(); i++){
                if( syntaxes[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            syntaxes.push_back(add_me);
        }break;
        case sym_t_label:{
            for( unsigned int i = 0; i < labels.size(); i++){
                if( labels[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            labels.push_back(add_me);
        }break;
        case sym_t_type:{
            for( unsigned int i = 0; i < labels.size(); i++){
                if( labels[i].name == add_me.name ){
                    assert(false);
                }
            }
            
            types.push_back(add_me);
        }break;
    }
}

void symbol_table::add_scope(std::string scope,scope_type type ){
    sub_scopes.push_back( symbol_table(this,scope,type) );
}

std::string symbol_table::get_full_scope(){
    
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
    
    if(bytes.size()>0) std::cout << sym_tbl_indent << "bytes:\n";
    for( unsigned int i = 0; i < bytes.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << bytes[i].name << "\n";
    }
    
    if(duals.size()>0) std::cout << sym_tbl_indent << "duals:\n";
    for( unsigned int i = 0; i < duals.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << duals[i].name << "\n";
    }
    
    if(quads.size()>0) std::cout << sym_tbl_indent << "quads:\n";
    for( unsigned int i = 0; i < quads.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << quads[i].name << "\n";
    }
    
    if(octs.size()>0) std::cout << sym_tbl_indent << "octs:\n";
    for( unsigned int i = 0; i < octs.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << octs[i].name << "\n";
    }
    
    if(structs.size()>0) std::cout << sym_tbl_indent << "structs:\n";
    for( unsigned int i = 0; i < structs.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << structs[i].name << "\n";
    }
    
    if(functions.size()>0) std::cout << sym_tbl_indent << "functions:\n";
    for( unsigned int i = 0; i < functions.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << functions[i].name << "\n";
    }
    
    if(operators.size()>0) std::cout << sym_tbl_indent << "operators:\n";
    for( unsigned int i = 0; i < operators.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << operators[i].name << "\n";
    }
    
    if(syntaxes.size()>0) std::cout << sym_tbl_indent << "syntaxes:\n";
    for( unsigned int i = 0; i < syntaxes.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << syntaxes[i].name << "\n";
    }
    
    if(labels.size()>0) std::cout << sym_tbl_indent << "labels:\n";
    for( unsigned int i = 0; i < labels.size(); i++ ){
        std::cout << sym_tbl_indent << "    " << labels[i].name << "\n";
    }
    
    if(types.size()>0) std::cout << sym_tbl_indent << "types:\n";
    for( unsigned int i = 0; i < types.size(); i++ ){
        std::cout << "    ";
        types[i].print();
    }
    
    if(sub_scopes.size()>0) std::cout << sym_tbl_indent << "sub-scopes\n";
    sym_tbl_indent += "    ";
    for( unsigned int i = 0; i < sub_scopes.size(); i++ )
        sub_scopes[i].print();
    
    sym_tbl_indent.resize(sym_tbl_indent.size()-4);
}

void symbol::print(){
    std::string type_str = "<";
    
    for( std::set<std::string>::iterator iter = type.begin(); iter != type.end(); iter = std::next(iter) ){
        type_str += *iter;
        if( std::next(iter) != type.end() )
            type_str += ",";
    }
    type_str += ">";
    
    std::cout << sym_tbl_indent << "|" << type_str << "|" << name << "|" << value << "|\n";
}

// is id in local scope
bool symbol_table::contains_id(std::string id) const{
    return contains_byte(id) || contains_dual(id) || contains_quad(id) || contains_oct(id) || contains_struct(id) || contains_function(id) || contains_operator(id) || contains_syntax(id) || contains_label(id);
}

bool symbol_table::contains_byte(std::string id) const{
    for( unsigned int i = 0; i < bytes.size(); i++ )
        if( bytes[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_dual(std::string id) const{
    for( unsigned int i = 0; i < duals.size(); i++ )
        if( duals[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_quad(std::string id) const{
    for( unsigned int i = 0; i < quads.size(); i++ )
        if( quads[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_oct(std::string id) const{
    for( unsigned int i = 0; i < bytes.size(); i++ )
        if( bytes[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_struct(std::string id) const{
    for( unsigned int i = 0; i < structs.size(); i++ )
        if( structs[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_function(std::string id) const{
    for( unsigned int i = 0; i < functions.size(); i++ )
        if( functions[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_operator(std::string id) const{
    for( unsigned int i = 0; i < operators.size(); i++ )
        if( operators[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_syntax(std::string id) const{
    for( unsigned int i = 0; i < syntaxes.size(); i++ )
        if( syntaxes[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_label(std::string id) const{
    for( unsigned int i = 0; i < labels.size(); i++ )
        if( labels[i].name == id )
            return true;
    return false;
}

bool symbol_table::contains_type(std::string id) const{
    for( unsigned int i = 0; i < types.size(); i++ )
        if( types[i].name == id )
            return true;
    return false;
}

// is id in any scope
bool symbol_table::id_exists(std::string id) const{
    return byte_exists(id) || dual_exists(id) || quad_exists(id) || oct_exists(id) || struct_exists(id) || function_id_exists(id) || operator_exists(id) || syntax_exists(id) || label_exists(id) || type_exists(id);
}

bool symbol_table::byte_exists(std::string id) const{
    
    for( unsigned int i = 0; i < bytes.size(); i++ ){
        if( bytes[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->byte_exists(id);
    return false;
    
}

bool symbol_table::dual_exists(std::string id) const{
    
    for( unsigned int i = 0; i < duals.size(); i++ ){
        if( duals[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->dual_exists(id);
    return false;
    
}

bool symbol_table::quad_exists(std::string id) const{
    
    for( unsigned int i = 0; i < quads.size(); i++ ){
        if( quads[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->quad_exists(id);
    return false;
    
}

bool symbol_table::oct_exists(std::string id) const{
    
    for( unsigned int i = 0; i < octs.size(); i++ ){
        if( octs[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->oct_exists(id);
    return false;
    
}

bool symbol_table::struct_exists(std::string id) const{
    
    for( unsigned int i = 0; i < structs.size(); i++ ){
        if( structs[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->struct_exists(id);
    return false;
    
}

bool symbol_table::function_id_exists(std::string id) const{
    
    for( unsigned int i = 0; i < functions.size(); i++ ){
        if( functions[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->function_id_exists(id);
    return false;
    
}

bool symbol_table::function_exists(std::string id,std::vector<std::string>params) const{
    
    for( unsigned int i = 0; i < functions.size(); i++ ){
        if( functions[i].name == id && get_function_param_types(functions[i].value) == params )
            return true;
    }
    
    if( scope != "" ) return parent->function_exists(id,params);
    return false;
    
}

bool symbol_table::operator_exists(std::string id) const{
    
    for( unsigned int i = 0; i < operators.size(); i++ ){
        if( operators[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->operator_exists(id);
    return false;
    
}

bool symbol_table::syntax_exists(std::string id) const{
    
    for( unsigned int i = 0; i < syntaxes.size(); i++ ){
        if( syntaxes[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->syntax_exists(id);
    return false;
    
}

bool symbol_table::label_exists(std::string id) const{
    
    for( unsigned int i = 0; i < labels.size(); i++ ){
        if( labels[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->label_exists(id);
    return false;
    
}

bool symbol_table::type_exists(std::string id) const{
    
    for( unsigned int i = 0; i < types.size(); i++ ){
        if( types[i].name == id )
            return true;
    }
    
    if( scope != "" ) return parent->type_exists(id);
    return false;
    
}


bool symbol_table::id_starts_with_substr(std::string find_me){
    /*
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name.size() >= find_me.size() && symbols[i].name.substr(0,find_me.size()) == find_me )
            return true;
    }
    
    if( scope != "" ) return parent->id_starts_with_substr(find_me);//*/
    return false;
    
}

bool symbol_table::type_starts_with_substr(std::string find_me){
    /*
    for( unsigned int i = 0; i < symbols.size(); i++ ){
        if( symbols[i].name.size() >= find_me.size() && symbols[i].name.substr(0,find_me.size()) == find_me  )
            return true;
    }
    
    if( scope != "" ) return parent->id_starts_with_substr(find_me);//*/
    return false;
    
}

symbol_table& symbol_table::get_subscope(std::string findMe){
    
    for( int i = 0; i < sub_scopes.size(); i++ ){
        if( sub_scopes[i].scope == findMe )
            return sub_scopes[i];
    }
    
    assert(false);
}

bool symbol_table::contains_scope(std::string find_me) const{
    
    for( unsigned int i = 0; i < sub_scopes.size(); i++ ){
        if( sub_scopes[i].scope == find_me )
            return true;
    }
    
    return false;
}

bool symbol_table::scope_exists(std::string find_me) const{
    
    for( unsigned int i = 0; i < sub_scopes.size(); i++ ){
        if( sub_scopes[i].scope == find_me )
            return true;
    }
    
    if( scope != "" ) return parent->contains_scope(find_me);
    return false;
}

// assumes function exists.
symbol symbol_table::get_function(std::string getMe, std::vector<std::string> param_types) const{
    
    for( int i = 0; i < functions.size(); i++){
        if( functions[i].name == getMe )
            return functions[i];
    }
    
    return parent->get_function(getMe,param_types);// will null dereference if there isn't one.
}

// assumes byte exists.
symbol symbol_table::get_byte(std::string getMe) const{
    
    for( int i = 0; i < bytes.size(); i++){
        if( bytes[i].name == getMe )
            return bytes[i];
    }
    
    return parent->get_byte(getMe);// will null dereference if there isn't one.
}

// assumes byte exists.
symbol symbol_table::get_dual(std::string getMe) const{
    
    for( int i = 0; i < duals.size(); i++){
        if( duals[i].name == getMe )
            return duals[i];
    }
    
    return parent->get_dual(getMe);// will null dereference if there isn't one.
}

// assumes byte exists.
symbol symbol_table::get_quad(std::string getMe) const{
    
    for( int i = 0; i < quads.size(); i++){
        if( quads[i].name == getMe )
            return quads[i];
    }
    
    return parent->get_quad(getMe);// will null dereference if there isn't one.
}

// assumes byte exists.
symbol symbol_table::get_oct(std::string getMe) const{
    
    for( int i = 0; i < octs.size(); i++){
        if( octs[i].name == getMe )
            return octs[i];
    }
    
    return parent->get_oct(getMe);// will null dereference if there isn't one.
}

bool symbol_table::inside_syntax(){
    if(type==scope_t_syntax)
        return true;
    
    if(parent==nullptr)
        return false;
    
    return parent->inside_syntax();
}
