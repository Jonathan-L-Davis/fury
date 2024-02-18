# FURY

The fury lanugage has several compiler goals<br>
v(not doing version control seriously at this stage) - produce binary code, for any subset of the language, for a platform, real or not.<br>
v0.0 - minimal C-like language, most of the core type system, minus the abstract stuff. multi file handling, compiling on a real platform.<br>
v1.0 - fully functioning compiler with all or most of the language shipped.

## Planned features:

### The compiler will be part of the standard library.
This is how builds will be done (eventually), just load files and pass them into a standard library "compile" function.<br>
I think this'll blur the line between whether fury is a compiled or scripting language.<br>

### No abstract machine
All operations are just described in the fury language as operations on bytes, meaning there are almost no assumptions about physical hardware.<br>
the trade off here is that the ISA target has to be described in fury in the first place and that the compiler has to prove certain operations equivalent.<br>
This description is intended to be part of a "backend library", obviously most people don't want to touch that to write a hello world.<br>
I think this is ultimately a boon, since that means the compiler backend not only serves as a functional compiler, but also as documentation for the ISA.<br>
I'm not yet sure how to encode the binary for each instruction, but this is a backend problem not a user one.<br>

Things like pointers are not built in very intentionally, even though this is a low level language.<br>
I want more direct control over hardware, in ways current hardware doesn't really support.<br>
Such as direct control over the cache or in processor memories.<br>
I think these kinds of hardware changes would dictate a new type of pointers for type of physical memory.<br>

### Custom operators
Beyond what you can do in other languages, syntax figured out, precedence still iffy.<br>
> operator byte myop my ( byte input ) op {/*implementation*/} 
as an example, the ternary would be written <br>
> operator byte ?: (byte a) ? (byte b) : (byte c) { if(a) return b; else return c; }

Operators are just functions with special calling syntax.<br>

### Built in types
byte ( 1 byte )<br>
dual ( 2 bytes )<br>
quad ( 4 bytes )<br>
oct  ( 8 bytes )<br>
struct ( like a C struct )<br>

notice there are no pointers or strings here, those will be somehow built out of bytes, and operations on bytes, that is all.<br>

This read me file is far from finished, just needed to get something down before it slipped my mind again.<br>
I'll start putting more of my thoughts on this project/approach in writing.<br>

Things I need to self host the compiler<br>
pointers (AST does not exist without them)<br>
syscalls (no file IO is possible without them)<br>
some syntax magic for string literals so that I can use strings in a sane way. (technically a comfort feature, but *needed*)
