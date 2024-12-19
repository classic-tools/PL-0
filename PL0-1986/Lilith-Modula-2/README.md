# Extended PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **Lilith Modula-2**. The system was published in the book *Compilerbau* by *Niklaus Wirth* (1986). The **PL/0** examples and the system source code were scanned, and OCRed from the book.

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

|File              |Description                               |
|------------------|------------------------------------------|
|PL0.MOD           |**PL/0** Compiler-Interpreter main module |
|                  |                                          |
|PL0Generator.DEF  |**PL/0** machine code generation module   |
|PL0Generator.MOD  |                                          |
|                  |                                          |
|PL0Interpreter.DEF|**PL/0** machine Interpreter module       |
|PL0Interpreter.MOD|                                          |
|                  |                                          |
|PL0Parser.DEF     |**PL/0** Parser module                    |
|PL0Parser.MOD     |                                          |
|                  |                                          |
|PL0Scanner.DEF    |Scanner module                            |
|PL0Scanner.MOD    |                                          |
|                  |                                          |
|wirth1986a.pl0    |*Wirth*'s multiply-divide-gcd-fact example|
|wirth1986b.pl0    |*Wirth*'s gcd example                     |
|wirth1986c.pl0    |*Wirth*'s syntactic errors example        |
