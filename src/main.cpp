#include <iostream>

#include "token.h"

int main(int argc, char**argv){

    auto tokens = tokenize("./file.txt");

    for( auto token : tokens )
        std::cout << token.line_no << ": " << token.type << " : " << token.text  << "\n";
}
