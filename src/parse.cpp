#include "parse.h"
#include <assert.h>
#include <iostream>


/** AHHHHH AGONY**/

AST_node parse_if(const std::vector<token> &tokens, int &start_pos);
AST_node parse_until_end_paren(const std::vector<token> &tokens, int &start_pos);
AST_node parse_until_end_curly_brace(const std::vector<token> &tokens, int &start_pos);

AST_node null_node = {{"",0,null},{}};

AST_node parse_expression(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe;

    switch(tokens[start_pos].type){
        case null:{
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

            }else
            if( tokens[start_pos].text == "struct" ){

            }else
            if( tokens[start_pos].text == "union" ){

            }else
            if( tokens[start_pos].text == "while" ){

            }
        }break;
        case Operator:{
            std::cout << tokens[start_pos].text << "\n";
            assert(false);
        }break;
        case type:{
            assert(false); //shouldn't be here;
        }break;
        case identifier:{
            retMe.tok = tokens[start_pos];
            start_pos++;
        }break;
        case parser:{
            assert(false); //shouldn't be here;
        }break;
        case enclosing:{
            retMe.tok = tokens[start_pos];
            start_pos++;
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


    }

    return retMe;
}

AST_node parse(const std::vector<token> &tokens){// going to change drastically
    AST_node ret_me;

    ret_me.tok.line_no = 0;
    ret_me.tok.text = ":root";
    ret_me.tok.type = parser;

    return ret_me;
}

AST_node parse_until_end_paren(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe = {tokens[start_pos]};
    start_pos++;

    AST_node temp;
    while( temp.tok.text != ")" ){
        temp = parse_expression(tokens,start_pos);//this handles nested parens because it can call this function, 2 step recursion
        retMe.children.push_back( temp );
        //std::cout << temp.tok.text << "\n";
    }
    start_pos++;

    return retMe;
}

AST_node parse_until_end_curly_brace(const std::vector<token> &tokens, int &start_pos){
    AST_node retMe = {tokens[start_pos]};
    start_pos++;

    AST_node temp;
    while( temp.tok.text != "}" ){
        temp = parse_expression(tokens,start_pos);//this handles nested parens because it can call this function, 2 step recursion
        retMe.children.push_back( temp );
        //std::cout << temp.tok.text << "\n";
    }
    start_pos++;

    return retMe;
}

AST_node parse_if(const std::vector<token> &tokens, int &start_pos){

    AST_node retMe;

    // else clauses should error
    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "if" )
        retMe.tok = tokens[start_pos];
    start_pos++;

    if( (unsigned) start_pos < tokens.size() && tokens[start_pos].text == "(" ){
        AST_node temp = {tokens[start_pos]};
        retMe.children.push_back( temp );
        start_pos++;
    }else assert(false);

    parse_expression(tokens,start_pos);

    if(tokens[start_pos].text == ")" )
        retMe.children.push_back( {tokens[start_pos]} );
    start_pos++;



    if( tokens[start_pos].text == "{" ){
        //AST_node bracket_exp = parse_until_end_curly_brace(tokens,start_pos);
    }else{
        retMe.children.push_back( parse_expression(tokens,start_pos) );//single expression
    }

    //general parse
    //if {,
    // scan for n expressions,
    // scan for }
    //else
    // scan for single expression
    //scan for else




    return retMe;
}

std::string indent = "";
void AST_node::print(){
// TODO    VVV
//is depth first search not breadth. will fix later, but any output is better than none rn
    std::cout << indent << tok.text << "\n";
    indent += "    ";
    for( unsigned int i = 0; i < children.size(); i++ )
        children[i].print();
    indent.resize(indent.size()-4);
}


