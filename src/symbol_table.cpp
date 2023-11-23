#include "symbol_table.h"


void anal( AST_node& frisk_me, symbol_table& fill_me ){



    // fill symbol_table
    if( frisk_me.tok.type == type || frisk_me.tok.type == keyword ){
        fill_me.symbols.push_back({"type",frisk_me.tok.text,""});
    }

    if( frisk_me.tok.text == ":root" ){
        ;// ?
    }

    //*
    for( unsigned int i = 0; i < frisk_me.children.size(); i++ )
        anal( frisk_me.children[i], fill_me );//*/

    //do clean up

    return;
}