# FURY

The fury lanugage has several goals<br>
v-1.0 - produce binary code, for any subset of the language, for a platform, real or not.<br>
v0.0 - minimal C-like language, most of the core type system, minus the abstract stuff. multi file handling, compiling on a real platform.<br>
v1.0 - fully functioning compiler with all or most of the language shipped.

## Planned features:
### No abstract machine
All operations are just described in the fury language as operations on bytes, meaning there are almost 0 assumptions about physical hardware.<br>
the trade off here is that the ISA target has to be described in fury in the first place and that the compiler has to prove certain operations equivalent.<br>
I think this is ultimately a boon, since that means the compiler backend not only serves as a functional compiler, but also as documentation for the ISA.<br>
I'm not yet sure how to encode the binary for each instruction.<br>
good lord on second read of this, I am really tired.<br>
### Full custom operators
Beyond what you can do in other languages, syntax figured out, precedence still iffy.<br>
> operator byte myop my ( byte input ) op {/*implementation*/}  
> \<operator keyword\> \<optional return type\> \<all characters that will be present in final operator, in order\>  \< complicated(ish)\>  

Operators are just functions with special calling syntax.<br>

### Full custom syntax
Even harder than custom operators, this is not nearly fully thought out. String literals will not be part of the core language, they will be built ontop of it.<br>
this syntax is close to what I want, but not final, details like what the input and output types are, are not certain at all.<br>
It's like a function that operates during lexical analysis.<br>
> syntax byte* "" " ( \<wtf goes here, it's outside of the type system, it's just chars, bytes I guess?\> ) "  

### Built in types
byte ( 1 byte )<br>
dual ( 2 bytes )<br>
quad ( 4 bytes )<br>
oct  ( 8 bytes )<br>
hex  ( 16 bytes, only if necessary later )<br>
struct ( like a C struct )<br>
union ( like a C union, this one may not be included )<br>

notice there are no pointers or strings here, those will be somehow built out of bytes, and operations on bytes, that is all.

This read me file is far from finished, just needed to get something down before it slipped my mind again.
I'll start putting more of my thoughts on this project/approach in writing.
