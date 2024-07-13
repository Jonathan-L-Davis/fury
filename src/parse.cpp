#include "parse.h"
#include <assert.h>
#include <iostream>



AST_node parse_if(const std::vector<token> &tokens, int &start_pos);
AST_node parse_while(const std::vector<token> &tokens, int &start_pos);
AST_node parse_for(const std::vector<token> &tokens, int &start_pos);
AST_node parse_function(const std::vector<token> &tokens, int &start_pos);
AST_node parse_return(const std::vector<token> &tokens, int &start_pos);
AST_node parse_typed_declaration(const std::vector<token> &tokens, int &start_pos);
AST_node parse_struct_or_union(const std::vector<token> &tokens, int &start_pos);
AST_node parse_import(const std::vector<token> &tokens, int &start_pos);
AST_node parse_export(const std::vector<token> &tokens, int &start_pos);


AST_node parse(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    
    retMe.tok.text = "";
    
    while( (unsigned) start_pos < tokens.size() )
        retMe.children.push_back( parse_expression( tokens, start_pos ) );
    
    return retMe;
}

AST_node parse_expression(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    
    switch(tokens[start_pos].type){
        case epsilon:{
            assert(false); //shouldn't be here;
        }break;
        case keyword:{
            if( tokens[start_pos].text == "byte" ){
                retMe = parse_typed_declaration(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "dual" ){
                retMe = parse_typed_declaration(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "else" ){
                assert(false);// handled by if, never occurs in isolation
            }else
            if( tokens[start_pos].text == "export" ){
                parse_export(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "for" ){
                retMe = parse_for(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "function" ){
                retMe = parse_function(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "goto" ){
                //skipping for v0.0
            }else
            if( tokens[start_pos].text == "if" ){
                retMe = parse_if(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "import" ){
                parse_import(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "oct" ){
                retMe = parse_typed_declaration(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "quad" ){
                retMe = parse_typed_declaration(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "return" ){
                retMe = parse_return(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "struct" ){
                parse_struct_or_union(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "union" ){
                parse_struct_or_union(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "while" ){
                retMe = parse_while(tokens,start_pos);
            }
        }break;
        case Operator:{
            if( tokens[start_pos].text == ";" ){
            
            retMe.tok = tokens[start_pos];
            start_pos++;
            }else assert(false);
        }break;
        case binary_operator:{// all binary operators handled the same,
            retMe.tok = tokens[start_pos];// take current token,
            start_pos++;
            AST_node exp = parse_expression(tokens,start_pos);
            retMe.children.push_back(exp);// first operand will be pushed later after return of this call
        }break;
        case type:{
            retMe = parse_typed_declaration(tokens,start_pos);
        }break;
        case identifier:{
            AST_node id;
            id.tok = tokens[start_pos];
            start_pos++;
            
            if( tokens[start_pos].text == ";" ){
                retMe = id;
                AST_node terminal_op;
                terminal_op.tok = tokens[start_pos];
                retMe.children.push_back(terminal_op);
                start_pos++;
            }else
            if( tokens[start_pos].type == binary_operator ){//binary operators should be handled this way, will add them one at a time
                retMe = parse_expression(tokens,start_pos);
                retMe.children.insert( retMe.children.begin(), id);
            }else
            if( tokens[start_pos].text == ")" ){
                retMe = id;
            }else if( tokens[start_pos].text == "(" ){//should only be function arguments for now.
                retMe = id;
                retMe.children.push_back({tokens[start_pos]});
                start_pos++;
                while( (unsigned) start_pos < tokens.size() && tokens[start_pos].text != ")" ){
                    AST_node exp = parse_expression(tokens,start_pos);
                    retMe.children[0].children.push_back(exp);
                }
                
                
                if((unsigned) start_pos < tokens.size() && tokens[start_pos].text == ")"){
                    retMe.children[0].children.push_back({tokens[start_pos]});
                    start_pos++;
                }else assert(false);
                if((unsigned) start_pos < tokens.size() && tokens[start_pos].text == ";"){
                    retMe.children.push_back({tokens[start_pos]});
                    start_pos++;
                }
            }else assert(false);
        }break;
        case parser:{
            assert(false); //shouldn't be here;
        }break;
        case scoping:{
            if( tokens[start_pos].text == "{" ){
                retMe.tok = tokens[start_pos];
                start_pos++;
                while( (unsigned) start_pos < tokens.size() && tokens[start_pos].text != "}" ){
                    AST_node exp = parse_expression(tokens,start_pos);
                    retMe.children.push_back(exp);
                }
                AST_node end_bracket;// we are not error checking like we should be here
                end_bracket.tok = tokens[start_pos];//may be past end of array
                start_pos++;
                retMe.children.push_back(end_bracket);
            }else assert(false);
        }break;
        case literal:{
            retMe.tok = tokens[start_pos];
            start_pos++;
        }break;
        case error:{
            //not used for now, but soon
        }break;
        case root:{
            assert(false); //shouldn't be here;
        }break;
        case paren:{
            if( tokens[start_pos].text == ")" ){
                retMe.tok = tokens[start_pos];
                start_pos++;
            }else assert(false);
        }break;
        default:
            std::cout << tokens[start_pos].line_no << " " << tokens[start_pos].text << "\n";
            assert(false);
    }
    
    return retMe;
}


AST_node parse_if(const std::vector<token> &tokens, int &start_pos){
    
    AST_node retMe;
    AST_node* active = &retMe;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "if" ){
        retMe.tok = tokens[start_pos];
        start_pos++;
    }else assert(false);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "(" ){
        AST_node temp = {tokens[start_pos]};
        retMe.children.push_back( temp );
        active = &retMe.children[0];
        start_pos++;
    }else assert(false);
    
    AST_node boolean_expression = parse_expression(tokens,start_pos);//multiple expressions handled by { ... } scoping expression
    
    active->children.push_back(boolean_expression);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == ")" ){
        active->children.push_back( {tokens[start_pos]} );
        active = &(active->children[active->children.size()-1]);
        start_pos++;
    }else assert(false);
    
    AST_node true_case, false_case;
    
    true_case = parse_expression(tokens,start_pos);
    active = &retMe;
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "else" ){// if else statement
        AST_node else_clause;
        else_clause.tok = tokens[start_pos]; start_pos++;
        false_case = parse_expression(tokens,start_pos);
        
        active->children.push_back( true_case );
        else_clause.children.push_back( false_case );
        
        active->children.push_back(else_clause);
    }else{// if statement
        active->children.push_back(true_case);
    }
    
    
    return retMe;
}

AST_node parse_while(const std::vector<token> &tokens, int &start_pos){
    
    AST_node retMe;
    AST_node* active = &retMe;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "while" ){
        retMe.tok = tokens[start_pos];
        start_pos++;
    }else assert(false);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "(" ){
        AST_node temp = {tokens[start_pos]};
        retMe.children.push_back( temp );
        active = &retMe.children[0];
        start_pos++;
    }else assert(false);
    
    AST_node boolean_expression = parse_expression(tokens,start_pos);
    
    active->children.push_back(boolean_expression);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == ")" ){
        active->children.push_back( {tokens[start_pos]} );
        active = &(active->children[active->children.size()-1]);
        start_pos++;
    }else assert(false);
    
    AST_node while_body;
    
    while_body = parse_expression(tokens,start_pos);
    
    retMe.children.push_back(while_body);
    
    if((unsigned) start_pos < tokens.size() && tokens[start_pos].text == ";"){//terminate function
        retMe.children.push_back({tokens[start_pos]});
        start_pos++;
    }
    
    return retMe;
}

AST_node parse_for(const std::vector<token> &tokens, int &start_pos){
    
    AST_node retMe;
    AST_node* active = &retMe;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "for" ){
        retMe.tok = tokens[start_pos];
        start_pos++;
    }else assert(false);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "(" ){
        AST_node temp = {tokens[start_pos]};
        retMe.children.push_back( temp );
        active = &retMe.children[0];
        start_pos++;
    }else assert(false);
    
    AST_node init_expression = parse_expression(tokens,start_pos);
    AST_node boolean_expression = parse_expression(tokens,start_pos);
    AST_node increment_expression = parse_expression(tokens,start_pos);
    
    active->children.push_back(init_expression);
    active->children.push_back(boolean_expression);
    active->children.push_back(increment_expression);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == ")" ){
        active->children.push_back( {tokens[start_pos]} );
        active = &(active->children[active->children.size()-1]);
        start_pos++;
    }else assert(false);
    
    AST_node for_body;
    
    for_body = parse_expression(tokens,start_pos);
    
    retMe.children.push_back(for_body);
    
    return retMe;
}

AST_node parse_function(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    AST_node* active = &retMe;
    AST_node* function_id;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "function" ){
        retMe.tok = tokens[start_pos];
        start_pos++;
    }else assert(false);
    
    // parse return type and ID
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == type ){//built in return type
        active->children.push_back( {tokens[start_pos]} );
        start_pos++;
        
        if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ) {//function name
            active->children.push_back( {tokens[start_pos]} );
            active = &(active->children[1]);
            start_pos++;
        }else assert(false);
    }else if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ) {//could be void return type or custom return type
        active->children.push_back( {tokens[start_pos]} );
        start_pos++;
        // may not happen, void functions have no return type
        if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ) {//id if custom type, doesn't appear if it's a void function
            active->children.push_back( {tokens[start_pos]} );
            active = &(active->children[1]);
            start_pos++;
        }else{
            active = &(active->children[0]);// set function id as active for void functions
        }
    }else assert(false);
    
    function_id = active;// non conditional because id is always active at this point;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "(" ){
        active->children.push_back( {tokens[start_pos]} );
        active = &(active->children[0]);
        start_pos++;
    }else assert(false);
    
    //handle case where function has no paramters
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == ")" ){
        active->children.push_back( {tokens[start_pos]} );
        //active = &(active->children[ active->children.size()-1 ]);
        //start_pos++;
    }
    
    while( (unsigned) start_pos < tokens.size() && tokens[start_pos].text != ")" ){
        AST_node type_name, param_name, comma;
        if( (unsigned) start_pos < tokens.size() &&
            ( tokens[start_pos].type == identifier || tokens[start_pos].type == type ) ){//type of param for function declaration
            type_name = {tokens[start_pos]};
            start_pos++;
        }else assert(false);
        if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ){//parameter name
            type_name.children.push_back({tokens[start_pos]});
            start_pos++;
        }else assert(false);
        
        active->children.push_back( type_name );
        
        if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "," ){//comma or end of param pack
            start_pos++;//skip comma
        }else
        if ( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == ")" ){
            active->children.push_back( {tokens[start_pos]} );
        }else assert(false);
    }
    start_pos++;
    
    function_id->children.push_back( parse_expression(tokens,start_pos) );//function body
    
    if((unsigned) start_pos < tokens.size() && tokens[start_pos].text == ";"){//terminate function
        retMe.children.push_back({tokens[start_pos]});
        start_pos++;
    }
    
    return retMe;
}


AST_node parse_return(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "return" ){
        retMe = {tokens[start_pos]};
        start_pos++;
    }else assert(false);
    
    retMe.children.push_back( parse_expression( tokens, start_pos ) );
    
    return retMe;
}

AST_node parse_typed_declaration(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    
    if((unsigned) start_pos < tokens.size() && (//pull typename which may be an identifier for a custom type
       tokens[start_pos].text == "byte" ||
       tokens[start_pos].text == "dual" ||
       tokens[start_pos].text == "quad" ||
       tokens[start_pos].text == "oct" ||
       tokens[start_pos].type == identifier) ){
        retMe = {tokens[start_pos]};
        start_pos++;
    }else assert(false);
    
    if((unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ){//pull variable name
        retMe.children.push_back( {tokens[start_pos]} );
        start_pos++;
    }else assert(false);
    //AST_node* decl_end = &retMe.children[0];
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "=" ){//with default value
        //retMe.children[0].children.push_back( {tokens[start_pos]} );
        start_pos++;
        
        retMe.children.push_back( parse_expression( tokens, start_pos ) );
    }
    //*
    if((unsigned) start_pos < tokens.size() && tokens[start_pos].text == ";"){//terminate function
        retMe.children.push_back({tokens[start_pos]});
        start_pos++;
    }else assert(false);//*/
    
    return retMe;
}

AST_node parse_struct_or_union(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    
    if( (unsigned) start_pos < tokens.size() && (
        tokens[start_pos].text == "struct" ||
        tokens[start_pos].text == "union"
        ) ){
        retMe = {tokens[start_pos]};
        start_pos++;
    }else assert(false);
    
    retMe.children.push_back( parse_expression( tokens, start_pos ) );
    
    return retMe;
}

//import and export will get way more complicated, but this should work for now
AST_node parse_import(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "import" ){
        retMe = {tokens[start_pos]};
        start_pos++;
    }else assert(false);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ){
        retMe.children.push_back( {tokens[start_pos]} );
        start_pos++;
    }else assert(false);
    
    return retMe;
}

AST_node parse_export(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "export" ){
        retMe = {tokens[start_pos]};
        start_pos++;
    }else assert(false);
    
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ){
        retMe.children.push_back( {tokens[start_pos]} );
        start_pos++;
    }else assert(false);
    
    return retMe;
}

std::string indent = "";
void AST_node::print(){
    std::cout << indent << tok.text << "\n";
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ )
        children[i].print();
    indent.resize(indent.size()-4);
}


