#include <iostream>

#include "parse.h"
#include "symbol_table.h"
#include "program.h"
#include "interpret.h"

#include <vector>
#include <assert.h>

/** General notes/complaints.
 * I dislike how I've been allocating memory for type info. I should be passing existing nodes back out instead, but that's more work and I don't know how exactly that needs to look, so it works fine for now to manually allocate and delete the memory.
 * Scoping only works for ops right now. I need to make it work for {}, and then just have functions suck those down.
**/

int main(int argc, char**argv){
    // currently leaks memory on exit. Not a big deal (yet). Just need to define destructors for AST_node's.
    std::vector<program> prog_frags;
    
    bool print = false;
    for( int i = 1; i < argc; i++ ){
        if(std::string(argv[i])=="-p"){
            print = true;
            continue;
        }else
        prog_frags.push_back( parse( argv[i] ) );
    }
    
    for(auto prog_frag:prog_frags){
        std::cout << prog_frag.file_name << "\n";
        prog_frag.context.print();
        prog_frag.root.print_with_types(&prog_frag.context);
    }
    
    assert( prog_frags.size()==1 && "OOPS! You didn't have only 1 file." );
    //interpret(prog_frags[0]);
    
    return 0;
}

