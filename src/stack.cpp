#include "stack.h"
#include <assert.h>



stackObject& stack::active(){
    assert(theStack.size()>0);// need to return either root or highest function.
    
    for( int i = theStack.size()-1; i >= 0; i-- ){
        
        if( theStack[i].object->text == "{" || theStack[i].object->type == node_t::root ){
            
            return theStack[i];
        }
    }
    
    return theStack[theStack.size()-1];
}

void stack::pop(){
    theStack.erase( theStack.end()-1, theStack.end() );
}

void stack::push(stackObject pushMe){
    
    
    if( theStack.size() == theStack.capacity() ){
        
        std::cout << "Stack Overflow. Program exiting.\n";
        
        std::exit(-1);
    }
    
    theStack.push_back(pushMe);
}

void stack::print(){
    for( int i = 0; i < theStack.size(); i++ ){
        std::cout << theStack[i].object->text << "\n";
        if( theStack[i].object->text == "byte" ){
            std::cout << "    " << theStack[i].object->children[0]->text << "\n";
            std::cout << "    " << (theStack[i].variable&0xFF) << "\n";
        }
        if( theStack[i].object->text == "dual" ){
            std::cout << "    " << theStack[i].object->children[0]->text << "\n";
            std::cout << "    " << (theStack[i].variable&0xFF) << "\n";
            std::cout << "    " << (theStack[i].variable&0xFFFF) << "\n";
        }
        if( theStack[i].object->text == "quad" ){
            std::cout << "    " << theStack[i].object->children[0]->text << "\n";
            std::cout << "    " << (theStack[i].variable&0xFF) << "\n";
            std::cout << "    " << (theStack[i].variable&0xFFFF) << "\n";
            std::cout << "    " << (theStack[i].variable&0xFFFF'FFFF) << "\n";
        }
        if( theStack[i].object->text == "oct" ){
            std::cout << "    " << theStack[i].object->children[0]->text << "\n";
            std::cout << "    " << (theStack[i].variable&0xFF) << "\n";
            std::cout << "    " << (theStack[i].variable&0xFFFF) << "\n";
            std::cout << "    " << (theStack[i].variable&0xFFFF'FFFF) << "\n";
            std::cout << "    " << (theStack[i].variable&0xFFFF'FFFF'FFFF'FFFF) << "\n";
        }
    }
}
