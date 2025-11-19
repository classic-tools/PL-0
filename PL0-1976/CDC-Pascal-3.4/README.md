# CDC Pascal PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **CDC Pascal**. The **CDC Pascal 1.2** system source code was hand-converted to **CDC Pascal 3.4**. 

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
             expression ( "=" | "#" | "<" | "<=" | ">" | ">=" ) expression .
expression = [ "+" | "-" ] term { ( "+" | "-" ) term } .
term       = factor { ( "*" | "/" ) factor } .
factor     = ident | number | "(" expression ")" .
```

|File        | Description                                             |
|------------|---------------------------------------------------------|
|pl0.md      |**PL/0** Compiler-Interpreter system                     |
|            |&#8625; replaced with (\*                                |            
|            |&downarrow; replaced with \*)                            |            
|            |&not; replaced with **NOT**                              |
|            |&ne; replaced with <>                                    |
|            |&le; replaced with <=                                    |
|            |&ge; replaced with >=                                    |
|            |*SSYM*['&ne;'] replaced with *SSYM*['#']                 |
|            |*SSYM*['<'] and *SSYM*['>'] initializations removed      |
|            |*SSYM*['&le;'] and *SSYM*['&ge;'] initializations removed|
|            |Code to recognize <=, and >= added                       |
|            |                                                         |
|wirth1976.md|*Wirth*'s multiply-divide-gcd example                    |
|            |&le; replaced with <=                                    |
|            |&ne; replaced with #                                     |
