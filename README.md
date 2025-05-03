# FURY

There is some redundant info here, & lots missing. Work in Progress.<br>

Fury is a low-level language, that aims to be expressive enough to run on very unique hardware devices.
One of my major reasons for developing the language is I want to write hardware with lots of control over low level details & C doesn't handle all of those well.
For example I want to make fpga designs that play with the memory model compared to "normal" & I want to make custom IEEE-754 extensions, and C wouldn't abstract either of those well.
For the memory model stuff, I want the ability to have multiple pointer types recognized at both the language & hardware level instead of memory mapped IO.
And I want enough abstraction to implement something like the floating point extensions in a way that feels like normal float syntax without being builtin to the compiler.

The fury lanugage has several compiler goals<br>
v(not doing version control seriously at this stage) - produce binary code, for any subset of the language, for a platform, real or not.<br>
v0.0 - minimal C-like language, most of the core type system, minus the abstract stuff. multi file handling, compiling on a real platform.<br>
v1.0 - fully functioning compiler with all or most of the language shipped.

## Planned features:

### The compiler will be part of the standard library.
This is how builds will be done (eventually), just load files and pass them into a standard library "compile" function.<br>
I think this'll blur the line between whether fury is a compiled or scripting language.<br>

### Two "standard" libaries
Since the pure language is extremely minimal, I plan to have 2 standard libraries<br>
One called "core" to include what you'd actually expect out of a low level language, like floats & pointers.<br>
One called "std" to include what you'd expect from a typical standard library.<br>

### Direct access to interpreter/compiler internals
This is how things like operator precedence will be controlled, for both interpreted & compiled code<br>
Basically there will be a std library module that returns you a reference to the interpreter context.<br>
Metaprogramming can be done with either the AST, returned from the standard libraries parsing function,<br>
or it can be done by grabbing the interpreter context & modifying it as it's running.<br>

### Keywords
byte<br>
dual<br>
quad<br>
oct<br>
struct<br>
function<br>
operator<br>
syntax<br>
label<br>
type<br>
import<br>
export<br>
namespace<br>
if<br>
else<br>
for<br>
while<br>
goto<br>
return<br>

#### Primitive types
    byte
    dual
    quad
    oct
These are 1 byte, 2 byte, 4 byte, & 8 byte types respectively. Just unsigned bytes w/ the obvious alignments.<br>

#### Structs
    struct myStructName{ byte a; byte b; };
Effectively C structs.<br>

#### "Functionals" (not the math kind)
    function
    operator
    syntax
These are constructs that operate like functions in the language. Not linear functionals.<br>
<br>
Some examples:<br>
Functions are what you'd expect.
```
function byte myFunc( byte a ){
    return a;
}
```

Operators are just functions with special calling syntax.<br>
I need to write much better/more for operators & syntaxes. Here are some examples, but it's pretty lacking given everything. Will implement them & then do some more examples, once I have some practical ones.<br>
An example of implementing string concatenation
```
operator string + (string a) + (string b){
    string retMe = a;
    for( int i = 0; i < b.size(); b++ )
        retMe += b[i];
    return retMe;
}
```

In general you can have n-ary operators as such<br>
```
          Continuous non-ws token      You can place () for arguments between each separate token
                        |                              |
                        V                              V
operator retType     op...op                    () op ( ) ... () op () {
    # do stuff
}
```


Syntaxes are comptime/interpret time executed code that trigger once a substring has been parsed.<br>
They can have any return type, but can only take in one byte at a time.<br>
There's some funky stuff with "return type abuse".<br>
Return type abuse lets us decide when to stop sucking in individual bytes.<br>
<br>
The following example lets us define a comment syntax which finishes executing when a newline is read in.<br>
Returning itself is how it continues to run until the end of the line. Otherwise it would only read in one byte.<br>
```
# this is pseudo-code, there wouldn't be a character literal in actual fury.
syntax # (byte b){
    if(b=='\n') return;
    return #;
}


```
#### type
type is basically a generic type. It's a set of types.<br>
```
type myFavoriteType = dual;
type primitiveByteTypes = byte|dual|quad|oct;
```
There's some other operators, but it's getting close to my bed time.<br>

#### label
Goto labels. Declared as:
```
label myLabel;
goto myLabel;
```
Trying to add explicitly to the type system.<br>
Only thing I'm stuck on right now is I might want to change the syntax to allow forward declaration of labels. Otherwise it get's really annoying. Can maybe work around with import & export, but god that's ugly.<br>

#### import & export, what you'd expect, syntax not final yet
yes
#### control flow
what you'd expect.
