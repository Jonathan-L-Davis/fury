#include <iostream>

#include "parse.h"
#include "symbol_table.h"
#include "program.h"
#include <vector>
#include <assert.h>

program parse2(std::string file_name);

int main(int argc, char**argv){
    
    program p1 = parse2("function.fury");
    
    p1.root.print();
    std::cout << "----------------------------------------------------------------------------------\n";
    p1.root.print_with_types();
    
    return 0;
}
