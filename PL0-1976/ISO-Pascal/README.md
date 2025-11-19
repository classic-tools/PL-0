# ISO Pascal PL/0 Compiler-Interpreter system
**PL/0** compiler and **PL/0 machine** interpreter system written in **ISO Pascal**. The **CDC Pascal 3.4** system source code was hand-converted to **ISO Pascal**. 

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

**Free Pascal compilation**
```
fpc -Miso pl0.pas
```

**Execution**
```
pl0 < program-file
```

|File         | Description                                                       |
|-------------|-------------------------------------------------------------------|
|pl0.pas      |**PL/0** Compiler-Interpreter system                               |
|             |&uparrow; replaced with ^                                          |
|             |&equiv; replaced with \'                                           |
|             |(\* replaced with {                                                |            
|             |\*) replaced with }                                                |            
|             |Constant *nmax* changed from 14 to 9                               |
|             |*ssym* array initialization changed from CDC character set to ASCII|
|             |Unneeded page(output) deleted                                      |
|             |                                                                   |
|wirth1976.pl0|*Wirth*'s multiply-divide-gcd example                              |
