# PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **ISO Pascal**. The system is based on the **CDC Pascal** program published in the book *Algorithms + Data Structures = Programs* by *Niklaus Wirth* (1976).

|File   |Description                                  |
|-------|---------------------------------------------|
|pl0.pas|PL/0 Compiler-Interpreter system (ISO Pascal)|

**Free Pascal compilation**
```
fpc -Miso pl0.pas
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

|PL/0 language changes|Changes from CDC Pascal to ISO Pascal                              |
|---------------------|-------------------------------------------------------------------|
|&ne; replaced with # |&not; replaced with **not**                                        |
|&le; replaced with <=|&uparrow; replaced with ^                                          |
|&ge; replaced with >=|&ne; replaced with <>                                              |
|                     |&le; replaced with <=                                              |
|                     |&ge; replaced with >=                                              |
|                     |Constant *nmax* changed from 14 to 9                               |
|                     |"procedure" enumeration item was renamed to "prozedure"            |
|                     |Unneeded semicolons deleted                                        |
|                     |tab character allowed in source code (ignored)                     |
|                     |Lowercase letters allowed in source code (converted to uppercase)  |
|                     |Code to recognize <, <=, >, and >= added                           |
|                     |Unneeded begin-end pair deleted                                    |
|                     |*ssym* array initialization changed from CDC character set to ASCII|
|                     |*ssym*['&ne;'] replaced with *ssym*['#']                           |
|                     |*ssym*['<'] and *ssym*['>'] initializations removed                |
|                     |*ssym*['&le;'] and *ssym*['&ge;'] initializations removed          |
|                     |Unneeded page(output) deleted                                      |
|                     |Missing semicolon added                                            |
|                     |Code to recognize/interpret the input operator (?) added           |
|                     |Code to recognize/interpret the output operator (!) added          |
|                     |Code to recognize comments (*...*) added                           |
