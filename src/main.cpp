#include <iostream>

#include "token.h"

int main(int argc, char**argv){

    char c = '0';
    std::cout << (c=='0'?0:c=='1'?1:c=='Z'?2:3);

    if( cond ) a; else b;
    cond ? a : b;

    return 0;

    auto tokens = tokenize("./file.txt");

    for( auto token : tokens )
        std::cout << token.line_no << ": " << token.type << " : " << token.text  << "\n";
}
