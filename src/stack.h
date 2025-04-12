#ifndef STACK_H
#define STACK_H

#include "node.h"
#include "program.h"

#include <vector>

struct stackObject{
    AST_node* object;
    int idx;
    uint64_t variable;// can either be a value, a pointer, or anything depending on the object type. byte/dual/quad/oct are as you'd expect. structs are pointers to objects that I haven't defined yet. Probably some sort of array/map type? -- structs are pointers to arrays of pointers/uint64_t's for now. Obviously not optimal, but it'll work for now lol.
};

// will eventually contain tons of helper functions, like unwind_function & what not.
struct stack{
    
    std::vector< stackObject > theStack;
    std::vector< std::string > scopes;
    
    program* executing;
    
    void step();
    stackObject& active();
    void pop();
    void push(stackObject pushMe);
    void print();
};

#endif//STACK_H
