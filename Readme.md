This project implements a compiler and IDE for the Luon programming language.

Luon is a high-level programming language with a syntax similar to Oberon+, Oberon-07 and Oberon-2, aspiring to run on the LuaJIT VM and to be compatible with the Lua programming language. Luon can be regarded as a statically typed version of Lua. The name is thus a combination of "Lua" and "Oberon". 

The project is the result of the author's experience with the [Smalltalk-80](https://github.com/rochus-keller/Smalltalk/) and [SOM VM](https://github.com/rochus-keller/Som/) implementation, and the prospect to build a [LuaJIT based Interlisp VM](https://github.com/rochus-keller/Gingko/). 

In contrast to Oberon+, Luon doesn't have pointers, but instead all structured datatypes have reference semantics and are dynamically created. In addition to the ARRAY type, there is also a HASHMAP type for compatibility with Lua. There is also a STRING data type which - like Lua - is immutable and follows a value semantics. 

In particular, Luon solves the following shortcomings recognized during the aforementioned projects:

- conditional compilation to enable or disable statements only used for debugging and to avoid wasting calculation time 
- constants not requiring local slots or hashed element access
- explicit inline declaration, so the code can be better structured without additional context 
  switches and slot consumption
- no implicit global declarations, and other means to detect as many errors as possible during compile time
- locals can no longer be used before declaration
- switch/case control statement to avoid writing the relation expressionall over again

Here is a screenshot of the IDE:

![IDE Screenshot 1](http://software.rochus-keller.ch/luon-ide-screenshot-0.7.0-1.png)

And here is the source-level debugger of the IDE:

![IDE Screenshot 2](http://software.rochus-keller.ch/luon-ide-screenshot-0.7.0-2.png)


NOTE that this project is work-in-progress.

#### Planned features

- [x] Implement lexer with directive support
- [x] Implement parser and AST
- [x] Implement semantic validator 
- [x] Implement a LuaJIT backend based on LjTools
- [x] Implement an IDE similar to the Oberon+ IDE
- [x] Document the language (specification is available)
- [ ] Migrate the Smalltalk-80 VM to Luon as a proof-of-concept and to optimize the language (WIP)
- [ ] Complete the language implementation according to the specification

#### Status on October 14, 2024

Derived the syntax from Oberon+ and Micron and generated a parser using [the grammar](https://github.com/micron-language/specification/blob/master/Micron_Programming_Language.ebnf) and adopted the lexer from [the Micron project](https://github.com/rochus-keller/Micron). The parser is able to successfully read the ongoing Luon migration of the 
[Are-we-fast-yet benchmark suite](https://github.com/rochus-keller/Are-we-fast-yet/tree/main/Luon) (not yet commited).

#### Status on October 15, 2024

Adopted the AST infrastructure from Micron and extended the AST for full program coverage.
Implemented a recursive descent parser (modified the one generated  by [EbnfStudio](https://github.com/rochus-keller/EbnfStudio)) which generates the AST.

#### Status on October 16, 2024

Improved language: constructors generate object, extended for arrays and hashmaps; extra set constructor no longer needed. Can generate AST for ongoing AWFY with no memory leaks.

#### Status on October 25, 2024

The semantic validator is complete and tested. I developed it in parallel with [the Luon version of the Are-we-fast-yet benchmark suite](https://github.com/rochus-keller/Are-we-fast-yet/tree/main/Luon). Some refactorings and language changes were necessary. As in Oberon-07, there is now only one INTEGER and REAL type, the former with 53 bits of precision. The validator was feature complete on October 22, so development took six, and testing and debugging three calendar days.

#### Status on October 27, 2024

The IDE is ready for testing and debugging. I derived it from the LuaJIT version of the Oberon IDE, but many changes were necessary because the AST and code model are completely different. This included an optional cross-referencing infrastructure integrated with the validator. Also the import logic had to be extended for compatibility with the project file concept, which required refactoring of AST, parser and validator.

#### Status on November 05, 2024

The LuaJIT bytecode generator including most built-in procedures is complete and ready for testing and debugging. All functions of the LuonFfi.c file 
were migrated to LUON.lua to avoid C dependencies. Added Oakwood implementations in Lua (Files is no longer Oakwood compatible).

#### Status on November 09, 2024

After a lot of testing, debugging and fixes the generated code seems to be sufficiently correct for the moment to continue with other stuff. The testcases
in the all.lnpro project, which I migrated from Oberon+ ("features" subfolder), work, which covers most aspect of the language. I also 
implemented delegates and the copy() builtin, and made some other language changes (e.g. := no longer copies char arrays by value). Next goal is
to run the Luon Awfy suite.

#### Status on November 13, 2024

The Luon Are-we-fast-yet suite works up to Json; a significant refactoring of the code generator was necessary for this to properly intitialize all
class objects up-front; the performance is currently slightly (1%) better than Lua, and there is room for improvement. Now after a calendar month 
in the project, the compiler and IDE seem ready for implementing a Smalltalk VM, so let's go for it.

#### Status on November 26, 2024

All benchmarks of the Luon Are-we-fast-yet suite now work and show a performance equal to the Lua on LuaJIT version of the benchmark. The debugger has been significantly improved. The BYTE type and TOSTRING builtin have been added to the language.
The Project Oberon System has been migrated to Luon; the code works so far, but for correct display output many changes would be required (maybe in future).
The Smalltalk VM is work in progress and expected to be complete in a week. Both compiler and IDE are used to implement the Smalltalk VM and have proved to be useful so far.

#### Status on December 1. 2024

The migration of the Smalltalk VM (C++ to Luon) is complete. In contrast to the C++ version which uses Qt, this version uses SDL2 via LuaJIT FFI. It is also an example how the EXTERN keyword works, i.e. how the external implementations are provided. Now the debugging starts; wish me luck ;-)

Meanwhile also the Luon specification is available, though not all features are yet implemented.


#### Precompiled versions

Not available at this time.

#### How to build

Qt Creator 3.x is currently used for development. There are *.pro files which are compatible with qmake.
