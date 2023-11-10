#include "parse.h"
#include <assert.h>
#include <iostream>


/** AHHHHH AGONY**/

AST_node parse_if(const std::vector<token> &tokens, int &start_pos);
AST_node parse_while(const std::vector<token> &tokens, int &start_pos);
AST_node parse_function(const std::vector<token> &tokens, int &start_pos);
AST_node parse_return(const std::vector<token> &tokens, int &start_pos);

AST_node epsilon_node = {{"",0,epsilon},{}};

AST_node parse_expression(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;

    switch(tokens[start_pos].type){
        case epsilon:{
            assert(false); //shouldn't be here;
        }break;
        case keyword:{
            if( tokens[start_pos].text == "byte" ){

            }else
            if( tokens[start_pos].text == "dual" ){

            }else
            if( tokens[start_pos].text == "else" ){

            }else
            if( tokens[start_pos].text == "export" ){

            }else
            if( tokens[start_pos].text == "for" ){

            }else
            if( tokens[start_pos].text == "function" ){
                retMe = parse_function(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "goto" ){

            }else
            if( tokens[start_pos].text == "if" ){
                retMe = parse_if(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "import" ){

            }else
            if( tokens[start_pos].text == "oct" ){

            }else
            if( tokens[start_pos].text == "quad" ){

            }else
            if( tokens[start_pos].text == "return" ){
                retMe = parse_return(tokens,start_pos);
            }else
            if( tokens[start_pos].text == "struct" ){

            }else
            if( tokens[start_pos].text == "union" ){

            }else
            if( tokens[start_pos].text == "while" ){
                retMe = parse_while(tokens,start_pos);
            }
        }break;
        case Operator:{
            if( tokens[start_pos].text == "=" ){

            retMe.tok = tokens[start_pos];
            start_pos++;
            AST_node exp = parse_expression(tokens,start_pos);
            retMe.children.push_back(exp);
            }else
            if( tokens[start_pos].text == ";" ){

            retMe.tok = tokens[start_pos];
            start_pos++;
            }else
            std::cout << tokens[start_pos].text << "\n",
            assert(false);
        }break;
        case type:{
            assert(false); //shouldn't be here;
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
            if( tokens[start_pos].text == "=" ){//binary operators should be handled this way, will add them one at a time
                retMe = parse_expression(tokens,start_pos);
                retMe.children.insert( retMe.children.begin(), id);
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
        default:
            std::cout << tokens[start_pos].text << "\n";
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
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "else" ){// if else statement
        AST_node else_clause;
        else_clause.tok = tokens[start_pos]; start_pos++;
        false_case = parse_expression(tokens,start_pos);

        else_clause.children.push_back( true_case );
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

    active->children.push_back(while_body);

    return retMe;
}

AST_node parse_function(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;
    AST_node* active = &retMe;

    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "function" ){
        retMe.tok = tokens[start_pos];
        start_pos++;
    }else assert(false);

    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == type ){//built in return type
        active->children.push_back( {tokens[start_pos]} );
        active = &(active->children[0]);
        start_pos++;

        if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ) {//function name
            active->children.push_back( {tokens[start_pos]} );
            active = &(active->children[0]);
            start_pos++;
        }else assert(false);
    }else if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ) {//could be void return type or custom return type
        active->children.push_back( {tokens[start_pos]} );
        active = &(active->children[0]);
        start_pos++;

        if( (unsigned) start_pos < tokens.size() && tokens[start_pos].type == identifier ) {//id if custom type, doesn't appear if it's a void function
            active->children.push_back( {tokens[start_pos]} );
            active = &(active->children[0]);
            start_pos++;
        }// may not happen, void functions have no return type
    }else assert(false);

    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "(" ){
        active->children.push_back( {tokens[start_pos]} );
        active = &(active->children[0]);
        start_pos++;
    }else assert(false);

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
            active = &(active->children[ active->children.size()-1 ]);
            //start_pos++;//don't do until after loop, since it is the break condition
            //active->children.push_back( type_name );
        }else assert(false);
    }
    start_pos++;


    //active = &(active->children[ active->children.size()-1 ]);


    active->children.push_back( parse_expression(tokens,start_pos) );//function body

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

std::string indent = "";
void AST_node::print(){
    std::cout << indent << tok.text << "\n";
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ )
        children[i].print();
    indent.resize(indent.size()-4);
}


