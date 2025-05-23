#include "interpret.h"
#include "stack.h"
#include "type_util.h"

#include <cassert>

void exec(stack &prog_stack, program &prog);

/**  ASSUMES runMe is a completely valid program.  **/
void interpret(program runMe){
    
    stack theStack;
    theStack.theStack.reserve(100);
    
    stackObject a;
    a.object = &runMe.root;
    a.idx = 0;
    
    theStack.theStack.push_back(a);
    
    while( theStack.theStack.size() > 0 ){
        
        exec(theStack,runMe);
        
    }
    
    std::cout << "Program Halted. Exiting with success.\n";
    
}

bool is_function_definition(AST_node const*);

void exec(stack &prog_stack, program &prog){
    if( prog_stack.theStack.size() == 0 ) return;
    
    prog_stack.print();
    
    stackObject &obj = prog_stack.active();
    if( obj.idx >= obj.object->children.size() ) {
        
        while( &obj != &prog_stack.theStack[ prog_stack.theStack.size()-1 ] ) prog_stack.pop();
        prog_stack.pop();
        
        
        return;
    }
    
    if( obj.object->children[obj.idx]->text == "function" ){
        obj.idx++;
        return;
    }
    
    if( obj.object->children[obj.idx]->text == "byte" ){
        
        stackObject a;
        
        a.object = prog.context.get_byte(obj.object->children[obj.idx]->children[0]->text).value;
        a.idx = 0;
        prog_stack.push(a);
        
        obj.idx++;
        return;
    }
    
    if( obj.object->children[obj.idx]->text == "dual" ){
        
        stackObject a;
        a.object = prog.context.get_dual(obj.object->children[obj.idx]->children[0]->text).value;
        a.idx = 0;
        prog_stack.push(a);
        
        
        obj.idx++;
        return;
    }
    
    if( obj.object->children[obj.idx]->text == "quad" ){
        
        stackObject a;
        
        a.object = prog.context.get_quad(obj.object->children[obj.idx]->children[0]->text).value;
        a.idx = 0;
        prog_stack.push(a);
        
        
        obj.idx++;
        return;
    }
    
    if( obj.object->children[obj.idx]->text == "oct" ){
        
        stackObject a;
        
        a.object = prog.context.get_oct(obj.object->children[obj.idx]->children[0]->text).value;
        a.idx = 0;
        prog_stack.push(a);
        
        
        obj.idx++;
        return;
    }
    
    if( prog.context.function_exists(obj.object->children[obj.idx]->text,get_function_param_types(obj.object)) ){
        
        stackObject a;
        a.object = prog.context.get_function(obj.object->children[obj.idx]->text,get_function_param_types(obj.object)).value;
        a.idx = 0;
        
        
        
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
        prog_stack.pop();
        obj = prog_stack.active();
        obj.idx++;
        return;
    }
    
    std::cout << "   " << obj.object->text << "\n";
    prog_stack.print();
    assert(false);// should not be encountering objects where we don't know how to execute them.
}
