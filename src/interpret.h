#include "program.h"

void interpret(program runMe);




struct stackObject{
    AST_node* object;
    int idx;
};

// will eventually contain tons of helper functions, like unwind_function & what not.
struct stack{
    
    std::vector< stackObject > theStack;
    
    void step();
    stackObject& active();
    void pop();
    void push(stackObject pushMe);
};
