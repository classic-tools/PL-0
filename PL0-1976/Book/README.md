# PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system. The system was published in the book *Algorithms + Data Structures = Programs* by *Niklaus Wirth* (1976). The **PL/0** example and the system source code were scanned and OCR'd from the book.

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

|File        | Description                         |
|------------|-------------------------------------|
|pl0.md      |**PL/0** Compiler-Interpreter system |
|wirth1976.md|*Wirth*'s multiply-divide-gcd example|
