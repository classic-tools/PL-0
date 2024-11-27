# Revised CDC Pascal PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **Revised CDC Pascal**. The **CDC Pascal** system source code, was hand-converted to **Revised CDC Pascal**. 

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

|File         |Charracter set      | Description                                             |
|-------------|--------------------|---------------------------------------------------------|
|pl0.md       |CDC 6 bits          |**PL/0** Compiler-Interpreter system                     |
|             |                    |&#8625; replaced with (\*                                |            
|             |                    |&downarrow; replaced with \*)                            |            
|             |                    |&not; replaced with **not**                              |
|             |                    |&ne; replaced with <>                                    |
|             |                    |&le; replaced with <=                                    |
|             |                    |&ge; replaced with >=                                    |
|             |                    |*ssym*['&ne;'] replaced with *ssym*['#']                 |
|             |                    |*ssym*['<'] and *ssym*['>'] initializations removed      |
|             |                    |*ssym*['&le;'] and *ssym*['&ge;'] initializations removed|
|             |                    |Code to recognize <= and >= added                        |
|             |                    |                                                         |
|pl0.pdf      |CDC 6 bits          |**PL/0** Compiler-Interpreter system listing             |
|             |                    |                                                         |
|wirth1976.pl0|CDC 6 bits/CDC ASCII|*Wirth's* multiply-divide-gcd **PL/0** example           |
|             |                    |&le; replaced with <=                                    |
|             |                    |&ne; replaced with #                                     |
|             |                    |                                                         |
|pl0.pas      |CDC ASCII           |**PL/0** Compiler-Interpreter system                     |
|             |                    |&uparrow; replaced with ^                                |
|             |                    |&equiv; replaced with \'                                 |
