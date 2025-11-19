# CDC Pascal PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **CDC Pascal**. The system source code from *Niklaus Wirth*'s book was hand-converted to **CDC Pascal 1.2**. 

**PL/0 EBNF Grammar:**
```
program    = block "." .
block      = [ "CONST" ident "=" number { "," ident "=" number } ";" ]
             [ "VAR" ident { "," ident } ";" ]
             { "PROCEDURE" ident ";" block ";" } statement .
statement  = [ ident ":=" expression | "CALL" ident |
             "BEGIN" statement { ";" statement } "END" |
             "IF" condition "THEN" statement |
             "WHILE" condition "DO" statement ] .
condition  = "ODD" expression |
             expression ( "=" | "≠" | "<" | "≤" | ">" | "≥" ) expression .
expression = [ "+" | "-" ] term { ( "+" | "-" ) term } .
term       = factor { ( "*" | "/" ) factor } .
factor     = ident | number | "(" expression ")" .
```

|File        |Description                                     |
|------------|------------------------------------------------|
|pl0.md      |**PL/0** Compiler-Interpreter system            |
|            |{ converted to &#8625;                          |
|            |} converted to &downarrow;                      |
|            |\' converted to &equiv;                         |
|            |Constant **LXMAX** (length of source line) added|
|            |Identifier PROCEDURE renamed to PROZEDURE       |
|            |Missing semicolon added                         |
|            |Unneeded semicolons removed                     |
|            |Unneeded begin-end pair removed                 |
|            |Small indentation changes                       |
|            |                                                | 
|wirth1976.md|*Wirth*'s multiply-divide-gcd example           |
