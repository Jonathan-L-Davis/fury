#include <iostream>

#include "parse.h"
#include "symbol_table.h"
#include "program.h"
#include "interpret.h"

#include <vector>
#include <assert.h>



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
        prog_frag.root.print_with_types();
    }
        
    return 0;
}

