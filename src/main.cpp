#include <iostream>

#include "parse.h"
#include "symbol_table.h"
#include "program.h"
#include "interpret.h"

#include <vector>
#include <assert.h>



int main(int argc, char**argv){
    // currently leaks memory on exit. Not a big deal (yet). Just need to define destructors for AST_node's.
    program p1 = parse("function.fury");
    std::cout << "----------------------------------------------------------------------------------\n";
    p1.root.print_with_types();
    std::cout << "----------------------------------------------------------------------------------\n";
    p1.the_context.print();
    
    
    std::cout << "----------------------------------------------------------------------------------\n";
    std::cout << "Running program function.fury\n";
    std::cout << "----------------------------------------------------------------------------------\n";
    
    //interpret( p1 );
    
    return 0;
}
