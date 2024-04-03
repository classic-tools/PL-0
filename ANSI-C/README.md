# PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **ANSI C** (**C89**). The system was hand-translated from the **ISO Pascal** program.

|File |Description                              |
|-----|-----------------------------------------|
|pl0.c|PL/0 Compiler-Interpreter system (ANSI C)|

**GCC compilation**
```
gcc -std=c89 pl0.c -o pl0
```

**Execution**
```
pl0 < program-file
```
The execution data must follow the PL/0 program source code.

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
