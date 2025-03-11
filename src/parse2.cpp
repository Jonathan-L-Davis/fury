#include <string>
#include <fstream>
#include <iostream>

#include "parse.h"
#include "symbol_table.h"
#include "program.h"
#include <vector>
#include <assert.h>

enum token_type:uint32_t{
    epsilon = 0,
    keyword = 1,
    Operator = 2,
    type = 3,
    identifier = 4,
    parser = 5,
    scoping = 6,
    literal = 7,
    error = 8,
    root = 9,
    sigma = 10,
    paren = 11,
    binary_operator = 12,// needs to be removed soon. As soon as new parser is done it will be gone.
};

program parse2(std::string file_name){
    program retMe;
    retMe.root = {{":--:",0,root},{}};// should be the only node/token with a line_no of 0.
    std::fstream file(file_name);// If I cared about performance I could at a minimum preallocate the number of bytes in the file.
    
    std::vector<uint8_t> file_buffer;
    std::vector<AST_node> nodePool = {};// for constucting nodes without forcing them into a tree immediately
    
    uint8_t q;
    while(file.read((char*)&q,1)){
        file_buffer.push_back(q);//should obviously preallocate the size in one go & 1 memcpy.
    }
    
    uint32_t line_no = 1;
    std::string completeToken;
    std::string currentToken;
    
    for(uint8_t byte : file_buffer){
        
        if( !( byte=='\n' || (byte>=' ' && byte<127) ) ){// fuck your \r -- but seriously, the 127 restriction is to allow for forward compatibility with utf-8. If you're reading this, please take a moment of silence fore the Unicode Consortium, our unsung heroes.
            std::cout << (int) byte << std::endl;
            assert(byte=='\n' || ( byte>=' ' && byte<127) );
        }
        
        
        
        if(byte == ' ' || byte == '\n'){
            std::cout << "ws\n";
            completeToken = currentToken;
            currentToken = "";
            
            if(byte == '\n') line_no++;
        }else if(byte=='('||byte==')'||byte=='{'||byte=='}'||byte==';'){// characters are forced to be single character tokens.
            std::cout << "special chars\n";
            if(currentToken!=""){
                nodePool.push_back({currentToken,line_no,node_t::id});// not a general solution. Need to fix.
                currentToken="";
            }
            
            nodePool.push_back({std::string(1,byte),line_no,node_t::paren});
            completeToken = "Sick hack, since this token won't be read, it doesn't matter what it is so long as it's not empty.";
            
        }else{
            std::cout << "normal chars\n";
            currentToken += byte;
        }
        
        // will get tricky when I add syntax here. Will probably parse '#' as comment syntax definition & implement a special function for it in the parser.
        
        if( completeToken == "" ) continue;
        // there's an obvious optimization we're skipping where we skip over if we don't add a new token after our last reduce step (from the perspective of per byte). -- completeToken being "" might be that check actually.
        
        
        
        // there is a better way to generalize for keywords, but I only need this one for right now. This is helping me work out the general shift reduce structure first. Then I can organize my thousand rules.
        if( completeToken == "function" ){
            
            AST_node node;
            
            node.line_no = line_no;
            node.text = completeToken;
            node.type = node_t::keyword;
            
            nodePool.push_back(node);
        }
        
        
        
        
        
        
        completeToken = "";// don't reparse a completed token, duh.
        
        for(auto nod:nodePool)
            nod.print();
        
        
        /// ----------------------------------------------------    Reduction Rules Start Here    ---------------------------------------------------------- ///
        
        for( int i = 0; i < nodePool.size(); i++ ){
            
            
            if( nodePool[i].text=="("){
                //need to handle commas as well for function declarations.
                // should everything in a () be fully reduced before this runs? That seems reasonable.
                if( i<nodePool.size()-1 && nodePool[i+1].text == ")" ){
                    nodePool[i].children.push_back(nodePool[i+1]);
                    nodePool.erase(nodePool.begin()+i+1);
                    continue;
                }//else assert(false); // guess you get to implement proper '(' reduction today. LMAO nerd.
            }
            
            if( nodePool[i].text=="function"){// totally ignores return types, oh well for now. Just need it to work.
                //need to handle commas as well for function declarations.
                // should everything in a () be fully reduced before this runs? That seems reasonable.
                if( i+2<nodePool.size() &&
                    
                    nodePool[i+1].type == node_t::id &&
                    
                    nodePool[i+2].text == "(" &&
                    nodePool[i+2].children.size()>0 && nodePool[i+2].children[nodePool[i+2].children.size()-1].text == ")" ){
                    //
                    
                    nodePool[i+1].children.push_back(nodePool[i+2]);
                    nodePool[i].children.push_back(nodePool[i+1]);
                    
                    nodePool.erase(nodePool.begin()+i+1,nodePool.begin()+i+3);// removes only 2 nodes [ i+1, i+3 ) o [i+1,i+2]
                    continue;
                }
            }
            
            
        }
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
    }
    
    return retMe;
}

