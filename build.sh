#!/usr/bin/bash

Source_Files=( "main.cpp" "lex.cpp" "parse.cpp" "something_new.cpp" "symbol_table.cpp" )
Object_Files=()

mkdir -p obj
for file in "${Source_Files[@]}" ; do
    
    if [[ $file =~ [[:space:]]+ ]] ; then
        echo "File path: \"$file\" Contains a space and should not. This is naughty. Fix yourself."
        exit 1
    fi
    
    object_file="obj/src/$(echo $file | sed -e 's/\.cpp/\.o/')"
    file="src/$file"
    
    path="$(dirname $file)"
    #echo "$file ---> $object_file"
    Object_Files+=( "${object_file}" )
    mkdir -p "obj/$path" # otherwise clang++/g++ complain about non-existing directory
    clang++ "$file" -c -o "${object_file}" -std=c++17
done

for object in "${Object_Files[@]}" ; do
    file_glob+=" $object"
done

clang++ $file_glob -o "fury"
