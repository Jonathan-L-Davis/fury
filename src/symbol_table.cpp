#include "symbol_table.h"
#include "parse_util.h"

#include <assert.h>
#include <iostream>



void symbol_table::add_symbol( symbol add_me ){
    //*
    
    for( unsigned int i = 0; i < sub_scopes.size(); i++){
        if( sub_scopes[i]->scope == add_me.name ){
            //add_me.value->print_with_types();
            if( !(add_me.sym_type == sym_t_function || add_me.sym_type == sym_t_operator ) ){// basically allows overloading of overloadable functionals. The grammer rules have to be responsible and do their own validity checking.
                assert(false);
            }
        }
    }
    
    switch(add_me.sym_type){
        case sym_t_byte:{
            for( unsigned int i = 0; i < bytes.size(); i++){
                if( bytes[i].name == add_me.name ){
//                    assert(false);
                }
            }
            
            bytes.push_back(add_me);
        }break;
        case sym_t_dual:{
            for( unsigned int i = 0; i < duals.size(); i++){
                if( duals[i].name == add_me.name ){
//                    assert(false);
                }
            }
            
            duals.push_back(add_me);
        }break;
        case sym_t_quad:{
            for( unsigned int i = 0; i < quads.size(); i++){
                if( quads[i].name == add_me.name ){
//                    assert(false);
                }
            }
            
            quads.push_back(add_me);
        }break;
        case sym_t_oct:{
            for( unsigned int i = 0; i < octs.size(); i++){
                if( octs[i].name == add_me.name ){
//                    assert(false);
                }
            }
            
            octs.push_back(add_me);
        }break;
        case sym_t_struct:{
            for( unsigned int i = 0; i < structs.size(); i++){
                if( structs[i].name == add_me.name ){
//                    assert(false);
                }
            }
            
            structs.push_back(add_me);
        }break;
        case sym_t_function:{
            for( unsigned int i = 0; i < functions.size(); i++){
                if( functions[i].name == add_me.name ){
                    if( is_function_definition(add_me.value) && is_function_definition(functions[i].value) ){
//                        assert(false);// defining a previously defined function.
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
                    // loop through each param, see if it's the same type.
                    
                    assert(operators[i].value->children.size()>0);
                    
                    auto a = get_type(add_me.value);
                    auto b = get_type(operators[i].value);
                    
                    bool same_signature = types_equal(a,b);
                    
                    if( same_signature && is_operator_definition(add_me.value) && is_operator_definition(operators[i].value) ){
                        assert(false);// defining a previously defined operator.
                    }else if(same_signature){
                        operators[i] = add_me;// need to handle namespaces for function overloads, can't support overloads correctly until that happens
                        goto end_operator;
                    }
                }
            }
            
            operators.push_back(add_me);
            end_operator:;
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
//                    assert(false);
                }
            }
            
            labels.push_back(add_me);
        }break;
        case sym_t_type:{
            for( unsigned int i = 0; i < labels.size(); i++){
                if( labels[i].name == add_me.name ){
//                    assert(false);
                }
            }
            
            types.push_back(add_me);
        }break;
    }
}

//*
void symbol_table::remove_symbol(symbol removeMe){
    std::erase_if(bytes,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(duals,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(quads,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(octs,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(structs,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(syntaxes,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(functions,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(operators,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(types,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
    std::erase_if(labels,[&](symbol checkMe){return removeMe.name==checkMe.name&&removeMe.sym_type==checkMe.sym_type&&removeMe.type==checkMe.type&&removeMe.value==checkMe.value;});
}//*/

symbol_table* symbol_table::add_scope(std::string new_scope,scope_type type, AST_node* node ){
    sub_scopes.push_back( new symbol_table(this,new_scope,type,node) );
    //std::cout << scope << "\n" << ((uint64_t)(&*(sub_scopes.end()-1))%8) << "\n";
    return *(sub_scopes.end()-1);
}

std::string symbol_table::get_full_scope(){
    assert(parent!=this);
    if ( scope != "" ){
        return parent->get_full_scope() + "::" + scope;
    }
    return scope;
}

symbol_table::symbol_table(){
    parent = nullptr;
    this->scope = "";
}

symbol_table::symbol_table( symbol_table* parent_ptr, std::string scope, scope_type type, AST_node* node ){
    parent = parent_ptr;
    this->scope = scope;
    this->type = type;
    this->node = node;
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
        sub_scopes[i]->print();
    
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

symbol_table& symbol_table::get_subscope(std::string findMe){
    
    for( int i = 0; i < sub_scopes.size(); i++ ){
        if( sub_scopes[i]->scope == findMe )
            return *sub_scopes[i];
    }
    
    assert(false);
}

std::vector<symbol> symbol_table::get_symbol(std::string getMe){
    std::vector<symbol> retMe;
    
    for(symbol sym:bytes){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:duals){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:quads){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:octs){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:structs){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:syntaxes){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:functions){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:operators){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:labels){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    for(symbol sym:types){
        if(sym.name==getMe)
            retMe.push_back(sym);
    }
    
    return retMe;
}

symbol_table& symbol_table::get_subscope(std::string id,std::vector<std::string> params){
    
    //assert(parent!=nullptr&&"Working on that nasty operator scoping problem.");
    auto iter = std::find_if(sub_scopes.rbegin(),sub_scopes.rend(),[&]
        (const symbol_table* checkMe){
            bool signature_matches = true;
            for( int i = 0; i < params.size() && i < signature.size(); i++ ){
                if(params[i]!=signature[i])signature_matches=false;
            }
            
            return (checkMe->scope==id&&signature_matches);
        });
    if(iter==sub_scopes.rend()){
        for(auto p:params) std::cout << p << ",";
        assert(false&&"There are no matching subscopes");
    }
    
    return **iter;
}
    
bool id_exists(std::string s){
    return false;
}
    
std::set<std::string> get_id_type(const AST_node* const typeMe){
    assert(id_exists(typeMe->text));
    return {};
}

std::vector<symbol> symbol_table::get_ops() const{
    std::vector<symbol> retMe = operators;
    
    std::vector<symbol> parent_retMe;
    if(parent)
        parent_retMe = parent->get_ops();
    
    retMe.insert(retMe.end(),parent_retMe.begin(),parent_retMe.end());
    
    return retMe;
}

bool symbol_table::types_equal(const AST_node* const a, const AST_node* const b) const {
    if(a==nullptr) return false;
    if(b==nullptr) return false;
    
    // deliberately after the nullptr. Don't want to do stuff with null types. Maybe this will change later?
    if(a==b) return true;
    
    if(a->text==b->text&&a->type==b->type) return true;
    
    return false;
}

AST_node* symbol_table::get_type(const AST_node* const typeMe)const {
    
    if(typeMe->text == "("){
        if(!is_closed_parenthesis(typeMe)) return nullptr;
        if(typeMe->children.size()<=1) return nullptr;
        assert(typeMe->children.size()==2);
        return get_type(typeMe->children[0]);
    }
    
    if(is_type_declaration(typeMe,this)){
        return new AST_node {typeMe->text,0,node_t::type};
    }
    
    if(byte_exists(typeMe->text)) return new AST_node {"b8" ,0,node_t::type};
    if(dual_exists(typeMe->text)) return new AST_node {"b16",0,node_t::type};
    if(quad_exists(typeMe->text)) return new AST_node {"b32",0,node_t::type};
    if( oct_exists(typeMe->text)) return new AST_node {"b64",0,node_t::type};
    
    if(is_type_declaration(typeMe,this)){
        assert(typeMe->type==node_t::type);
        return new AST_node {typeMe->text,0,node_t::type};
    }
    
    // This is a really nasty dependency. This has to come later in the function so when is_operator_call calls this function, we don't recurse forever.
    if(is_operator_call(typeMe,this)){/*
        std::cout << "--------------------------------------------------------------------------------\n";
        typeMe->print_with_types();
        std::cout << "--------------------------------------------------------------------------------\n";//*/
        return new AST_node {"b8",0,node_t::type};
    }
    
    if(parent==nullptr)
        return nullptr;
    else
        return parent->get_type(typeMe);
}


















