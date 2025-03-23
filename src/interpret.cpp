#include "interpret.h"
#include <cassert>

stackObject& stack::active(){
    assert(theStack.size()>0);
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

void exec(stack &prog_stack, program &prog);

/**  ASSUMES runMe is a completely valid program.  **/
void interpret(program runMe){
    
    stack theStack;
    theStack.theStack.reserve(10);
    
    stackObject a;
    a.object = &runMe.root;
    a.idx = 0;
    
    theStack.theStack.push_back(a);
    
    while( theStack.theStack.size() > 0 ){
        
        exec(theStack,runMe);
        
    }
    
}

bool is_function_definition(AST_node const*);

void exec(stack &prog_stack, program &prog){
    std::cout << "exec\n";
    if( prog_stack.theStack.size() == 0 ) return;
    
    stackObject &obj = prog_stack.active();
    
    if( obj.object->children[obj.idx]->text == "function" ){
        std::cout << "    function\n";
        obj.idx++;
        return;
    }
    
    if( prog.the_context.function_exists(obj.object->children[obj.idx]->text) ){
        
        stackObject a;
        a.object = prog.the_context.get_function(obj.object->children[obj.idx]->text).value;
        a.idx = 0;
        
        
        std::cout << "    " << a.object->children[0]->text << "\n";
        
        if( !is_function_definition(a.object) ){
            a.object->print();
            std::cout << "Function '" << a.object->children[0]->text << "' not defined. Exiting now.\n";
            std::exit(-1);
        }
        
        a.object = a.object->children[1];
        prog_stack.push(a);
        
        obj.idx++;
        return;
    }
    
    if( obj.object->children[obj.idx]->text == "}" ){
        std::cout << "    }\n";
        prog_stack.pop();
        obj = prog_stack.active();
        obj.idx++;
        return;
    }
    
    std::cout << obj.object->text << "\n";
    
    /** TODO ADD { / } **/
}
