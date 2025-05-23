#!/usr/bin/bash

Source_Files=( "main.cpp" "symbol_table.cpp" "parse.cpp" "interpret.cpp" "program.cpp" "node.cpp" "stack.cpp" "parse_util.cpp" "lex.cpp" "grammer.cpp" "type_util.cpp" )
Object_Files=()

mkdir -p obj
for file in "${Source_Files[@]}" ; do
    
    if [[ $file =~ [[:space:]]+ ]] ; then
        echo "File path: \"$file\" Contains a space and should not. This is wrong. Fix yourself."
        exit 1
    fi
    
    object_file="obj/src/$(echo $file | sed -e 's/\.cpp/\.o/')"
    file="src/$file"
    
    path="$(dirname $file)"
    #echo "$file ---> $object_file"
    Object_Files+=( "${object_file}" )
    mkdir -p "obj/$path" # otherwise clang++/g++ complain about non-existing directory
    zig c++ "$file" -g -o "${object_file}" -std=c++20 -O0 -c
done

for object in "${Object_Files[@]}" ; do
    file_glob+=" $object"
done

zig c++ $file_glob -o "fury"
