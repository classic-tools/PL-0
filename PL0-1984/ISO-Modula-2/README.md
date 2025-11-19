# PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **ISO Modula-2**. The system is based on the **Lilith Modula-2** program published in the book *Compilerbau* by *Niklaus Wirth* (1986).

**PL/0 EBNF Grammar:**
```
program    = block "." .
block      = [ "CONST" ident "=" number { "," ident "=" number } ";" ]
             [ "VAR" ident { "," ident } ";" ]
             { "PROCEDURE" ident ";" block ";" } statement .
statement  = [ ident ":=" expression | "CALL" ident |
             "?" ident | "!" expression |
             "BEGIN" statement { ";" statement } "END" |
             "IF" condition "THEN" statement |
             "WHILE" condition "DO" statement ] .
condition  = "ODD" expression |
             expression ( "=" | "#" | "<" | "<=" | ">" | ">=" ) expression .
expression = [ "+" | "-" ] term { ( "+" | "-" ) term } .
term       = factor { ( "*" | "/" ) factor } .
factor     = ident | number | "(" expression ")" .
``` 

**GNU Modula-2 compilation**
```
gm2 -fiso Heap.mod -c
gm2 -fiso Terminal.mod -c
gm2 -fiso FileSystem.mod -c
gm2 -fiso TextWindows.mod -c
gm2 -fiso PL0Scanner.mod -c
gm2 -fiso PL0Interpreter.mod -c
gm2 -fiso PL0Generator.mod -c
gm2 -fiso PL0Parser.mod -c
gm2 -fiso PL0.mod Heap.o Terminal.o FileSystem.o TextWindows.o PL0Scanner.o PL0Interpreter.o PL0Generator.o PL0Parser.o -o pl0
```

**Execution**
```
pl0
```

|File              |Description                                                                  |
|------------------|-----------------------------------------------------------------------------|
|PL0Scanner.def    |PL/0 Scanner module                                                          |
|PL0Scanner.mod    |INTEGER cast changed to ORD function (Diff procedure)                        |
|                  |CARDINAL cast changed to ORD function (Number procedure)                     |
|                  |Keyword string size corrected (EnterKW procedure)                            |
|                  |                                                                             |
|PL0Interpreter.def|PL/0 machine Interpreter module                                              |
|PL0Interpreter.mod|Integer print format size changed from 7 to 11 (Interpreter procedure)       |
|                  |INTEGER cast deleted (Interpreter procedure)                                 |
|                  |                                                                             |
|PL0Generator.def  |PL/0 machine Code Generation module                                          |
|PL0Generator.mod  |Unneeded semicolons deleted                                                  |
|                  |                                                                             |
|PL0Parser.def     |PL/0 Parser module                                                           |
|PL0Parser.mod     |Unneeded semicolons deleted                                                  |
|                  |                                                                             |
|PL0.mod           |PL/0 Compiler-Interpreter main module                                        |
|                  |                                                                             |
|Heap.def          |Minimal reimplementation of Lilith library modules, using ISO library modules|
|Heap.mod          |                                                                             |
|                  |                                                                             |
|Terminal.def      |                                                                             |
|Terminal.mod      |                                                                             |
|                  |                                                                             |
|FileSystem.def    |                                                                             |
|FileSystem.mod    |                                                                             |
|                  |                                                                             |
|TextWindows.def   |                                                                             |
|TextWindows.mod   |                                                                             |
|                  |                                                                             |
|wirth1984a.pl0    |*Wirth*'s multiply-divide-gcd-fact example                                   |
|wirth1984b.pl0    |*Wirth*'s gcd example                                                        |
|wirth1984c.pl0    |*Wirth*'s syntactic errors example                                           |
