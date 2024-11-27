PROGRAM PL0(INPUT, OUTPUT);\
(\*PL/0 COMPILER WITH CODE GENERATION\*)\
LABEL 99;\
CONST NORW = 11;&ensp;&ensp;(\*NO. OF RESERVED WORDS\*)\
&ensp;&ensp;TXMAX = 100;&ensp;&ensp;&ensp;&ensp;(\*LENGTH OF IDENTIFIER TABLE\*)\
&ensp;&ensp;NMAX = 14;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*MAX NO. OF DIGITS IN NUMBERS\*)\
&ensp;&ensp;AL = 10;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*LENGTH OF IDENTIFIERS\*)\
&ensp;&ensp;AMAX = 2047;&ensp;&ensp;&ensp;&ensp;(\*MAXIMUM ADDRESS\*)\
&ensp;&ensp;LEVMAX = 3;&ensp;&ensp;&ensp;&ensp;&ensp;(\*MAXIMUM DEPTH OF BLOCK NESTING\*)\
&ensp;&ensp;CXMAX = 200;&ensp;&ensp;&ensp;&ensp;(\*SIZE OF CODE ARRAY\*)\
TYPE SYMBOL =\
&ensp;&ensp;(NUL, IDENT, NUMBER, PLUS, MINUS, TIMES, SLASH, ODDSYM,\
&ensp;&ensp;&ensp;EQL, NEQ, LSS, LEQ, GTR, GEQ, LPAREN, RPAREN, COMMA, SEMICOLON,\
&ensp;&ensp;&ensp;PERIOD, BECOMES, BEGINSYM, ENDSYM, IFSYM, THENSYM,\
&ensp;&ensp;&ensp;WHILESYM, DOSYM, CALLSYM, CONSTSYM, VARSYM, PROCSYM);\
&ensp;&ensp;ALFA = PACKED ARRAY [1..AL] OF CHAR;\
&ensp;&ensp;OBJECT = (CONSTANT, VARIABLE, PROZEDURE);\
&ensp;&ensp;SYMSET = SET OF SYMBOL;\
&ensp;&ensp;FCT = (LIT, OPR, LOD, STO, CAL, INT, JMP, JPC);&ensp;&ensp;(\*FUNCTIONS\*)\
&ensp;&ensp;INSTRUCTION = PACKED RECORD\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;F: FCT;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*FUNCTION CODE\*)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;L: 0..LEVMAX;&ensp;&ensp;(\*LEVEL\*)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;A: 0..AMAX&ensp;&ensp;&ensp;&ensp;(\*DISPLACEMENT ADDRESS\*)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
(\* LIT 0, A&ensp;&ensp;:&ensp;&ensp;LOAD CONSTANT A\
&ensp;&ensp;OPR 0, A&ensp;&ensp;:&ensp;&ensp;EXECUTE OPERATION A\
&ensp;&ensp;LOD L, A&ensp;&ensp;:&ensp;&ensp;LOAD VARIABLE L, A\
&ensp;&ensp;STO L, A&ensp;&ensp;:&ensp;&ensp;STORE VARIABLE L, A\
&ensp;&ensp;CAL L, A&ensp;&ensp;:&ensp;&ensp;CALL PROCEDURE A AT LEVEL L\
&ensp;&ensp;INT 0, A&ensp;&ensp;:&ensp;&ensp;INCREMENT T-REGISTER BY A\
&ensp;&ensp;JMP 0, A&ensp;&ensp;:&ensp;&ensp;JUMP TO A\
&ensp;&ensp;JPC 0, A&ensp;&ensp;:&ensp;&ensp;JUMP CONDITIONAL TO A&ensp;&ensp;\*)\
VAR CH: CHAR;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*LAST CHARACTER READ\*)\
&ensp;&ensp;SYM: SYMBOL;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*LAST SYMBOL READ\*)\
&ensp;&ensp;ID: ALFA;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*LAST IDENTIFIER READ\*)\
&ensp;&ensp;NUM: INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*LAST NUMBER READ\*)\
&ensp;&ensp;CC: INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*CHARACTER COUNT\*)\
&ensp;&ensp;LL: INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*LINE LENGTH\*)\
&ensp;&ensp;KK, ERR: INTEGER;\
&ensp;&ensp;CX: INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;(\*CODE ALLOCATION INDEX\*)\
&ensp;&ensp;LINE: ARRAY [1..81] OF CHAR;\
&ensp;&ensp;A: ALFA;\
&ensp;&ensp;CODE: ARRAY [0..CXMAX] OF INSTRUCTION;\
&ensp;&ensp;WORD: ARRAY [1..NORW] OF ALFA;\
&ensp;&ensp;WSYM: ARRAY [1..NORW] OF SYMBOL;\
&ensp;&ensp;SSYM: ARRAY [CHAR] OF SYMBOL;\
&ensp;&ensp;MNEMONIC: ARRAY [FCT] OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PACKED ARRAY [1..5] OF CHAR;\
&ensp;&ensp;DECLBEGSYS, STATBEGSYS, FACBEGSYS: SYMSET;\
&ensp;&ensp;TABLE: ARRAY [0..TXMAX] OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;RECORD NAME: ALFA;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE KIND: OBJECT OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CONSTANT: (VAL: INTEGER);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VARIABLE, PROZEDURE: (LEVEL, ADR: INTEGER)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
PROCEDURE ERROR(N: INTEGER);\
BEGIN WRITELN(&equiv; \*\*\*\*&equiv;, &equiv;&ensp;&equiv;: CC - 1, &equiv;&uparrow;&equiv;, N: 2); ERR := ERR + 1\
END (\*ERROR\*);\
PROCEDURE GETSYM;\
&ensp;&ensp;VAR I, J, K: INTEGER;\
&ensp;&ensp;PROCEDURE GETCH;\
&ensp;&ensp;BEGIN IF CC = LL THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN IF EOF(INPUT) THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN WRITE(&equiv; PROGRAM INCOMPLETE&equiv;); GOTO 99\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LL := 0; CC := 0; WRITE(CX: 5, &equiv;&ensp;&equiv;);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE NOT EOLN(INPUT) DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN LL := LL + 1; READ(CH); WRITE(CH); LINE[LL] := CH\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WRITELN; LL := LL + 1; READ(LINE[LL])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;CC := CC + 1; CH := LINE[CC]\
&ensp;&ensp;END (\*GETCH\*);\
BEGIN (\*GETSYM\*)\
&ensp;&ensp;WHILE CH = &equiv;&ensp;&equiv; DO GETCH;\
&ensp;&ensp;IF CH IN [&equiv;A&equiv;..&equiv;Z&equiv;] THEN\
&ensp;&ensp;BEGIN (\*IDENTIFIER OR RESERVED WORD\*) K := 0;\
&ensp;&ensp;&ensp;&ensp;REPEAT IF K < AL THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN K := K + 1; A[K] := CH\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETCH\
&ensp;&ensp;&ensp;&ensp;UNTIL NOT (CH IN [&equiv;A&equiv;..&equiv;Z&equiv;, &equiv;0&equiv;..&equiv;9&equiv;]);\
&ensp;&ensp;&ensp;&ensp;IF K >= KK THEN KK := K ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;REPEAT A[KK] := &equiv;&ensp;&equiv;; KK := KK - 1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;UNTIL KK = K;\
&ensp;&ensp;&ensp;&ensp;ID := A; I := 1; J := NORW;\
&ensp;&ensp;&ensp;&ensp;REPEAT K := (I + J) DIV 2;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF ID <= WORD[K] THEN J := K - 1;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF ID >= WORD[K] THEN I := K + 1\
&ensp;&ensp;&ensp;&ensp;UNTIL I > J;\
&ensp;&ensp;&ensp;&ensp;IF I - 1 > J THEN SYM := WSYM[K] ELSE SYM := IDENT\
&ensp;&ensp;END ELSE\
&ensp;&ensp;IF CH IN [&equiv;0&equiv;..&equiv;9&equiv;] THEN\
&ensp;&ensp;BEGIN (\*NUMBER\*) K := 0; NUM := 0; SYM := NUMBER;\
&ensp;&ensp;&ensp;&ensp;REPEAT NUM := 10 \* NUM + (ORD(CH) - ORD(&equiv;0&equiv;));\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;K := K + 1; GETCH\
&ensp;&ensp;&ensp;&ensp;UNTIL NOT (CH IN [&equiv;0&equiv;..&equiv;9&equiv;]);\
&ensp;&ensp;&ensp;&ensp;IF K > NMAX THEN ERROR(30)\
&ensp;&ensp;END ELSE\
&ensp;&ensp;IF CH = &equiv;:&equiv; THEN\
&ensp;&ensp;BEGIN GETCH;\
&ensp;&ensp;&ensp;&ensp;IF CH = &equiv;=&equiv; THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN SYM := BECOMES; GETCH\
&ensp;&ensp;&ensp;&ensp;END ELSE SYM := NUL\
&ensp;&ensp;END ELSE\
(\*EXTRA STUFF ADDED TO SUPPORT <= AND >=\*)\
&ensp;&ensp;IF CH = &equiv;<&equiv; THEN BEGIN GETCH;\
&ensp;&ensp;&ensp;&ensp;IF CH = &equiv;=&equiv; THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN SYM := LEQ; GETCH\
&ensp;&ensp;&ensp;&ensp;END ELSE SYM := LSS\
&ensp;&ensp;END ELSE\
&ensp;&ensp;IF CH = &equiv;>&equiv; THEN\
&ensp;&ensp;BEGIN GETCH;\
&ensp;&ensp;&ensp;&ensp;IF CH = &equiv;=&equiv; THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN SYM := GEQ; GETCH\
&ensp;&ensp;&ensp;&ensp;END ELSE SYM := GTR\
&ensp;&ensp;END ELSE\
(\*END OF EXTRA STUFF\*)\
&ensp;&ensp;BEGIN SYM := SSYM[CH]; GETCH\
&ensp;&ensp;END\
END (\*GETSYM\*);\
PROCEDURE GEN(X: FCT; Y, Z: INTEGER);\
BEGIN IF CX > CXMAX THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN WRITE(&equiv; PROGRAM TOO LONG&equiv;); GOTO 99\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;WITH CODE[CX] DO\
&ensp;&ensp;BEGIN F := X; L := Y; A := Z\
&ensp;&ensp;END;\
&ensp;&ensp;CX := CX + 1\
END (\*GEN\*);\
PROCEDURE TEST(S1, S2: SYMSET; N: INTEGER);\
BEGIN IF NOT (SYM IN S1) THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ERROR(N); S1 := S1 + S2;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE NOT (SYM IN S1) DO GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
END (\*TEST\*);\
PROCEDURE BLOCK(LEV, TX: INTEGER; FSYS: SYMSET);\
&ensp;&ensp;VAR DX: INTEGER;&ensp;&ensp;(\*DATA ALLOCATION INDEX\*)\
&ensp;&ensp;&ensp;&ensp;TX0: INTEGER;&ensp;&ensp;&ensp;(\*INITIAL TABLE INDEX\*)\
&ensp;&ensp;&ensp;&ensp;CX0: INTEGER;&ensp;&ensp;&ensp;(\*INITIAL CODE INDEX\*)\
&ensp;&ensp;PROCEDURE ENTER(K: OBJECT);\
&ensp;&ensp;BEGIN (\*ENTER OBJECT INTO TABLE\*)\
&ensp;&ensp;&ensp;&ensp;TX := TX + 1;\
&ensp;&ensp;&ensp;&ensp;WITH TABLE[TX] DO\
&ensp;&ensp;&ensp;&ensp;BEGIN NAME := ID; KIND := K;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE K OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CONSTANT: BEGIN IF NUM > AMAX THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ERROR(30); NUM := 0 END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAL := NUM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VARIABLE: BEGIN LEVEL := LEV; ADR := DX; DX := DX + 1;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROZEDURE: LEVEL := LEV\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;END (\*ENTER\*);\
&ensp;&ensp;FUNCTION POSITION(ID: ALFA): INTEGER;\
&ensp;&ensp;&ensp;&ensp;VAR I: INTEGER;\
&ensp;&ensp;BEGIN (\*FIND IDENTIFIER ID IN TABLE\*)\
&ensp;&ensp;&ensp;&ensp;TABLE[0].NAME := ID; I := TX;\
&ensp;&ensp;&ensp;&ensp;WHILE TABLE[I].NAME <> ID DO I := I - 1;\
&ensp;&ensp;&ensp;&ensp;POSITION := I\
&ensp;&ensp;END (\*POSITION\*);\
&ensp;&ensp;PROCEDURE CONSTDECLARATION;\
&ensp;&ensp;BEGIN IF SYM = IDENT THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM IN [EQL, BECOMES] THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN IF SYM = BECOMES THEN ERROR(1);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = NUMBER THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ENTER(CONSTANT); GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE ERROR(2)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE ERROR(3)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE ERROR(4)\
&ensp;&ensp;END (\*CONSTDECLARATION\*);\
&ensp;&ensp;PROCEDURE VARDECLARATION;\
&ensp;&ensp;BEGIN IF SYM = IDENT THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ENTER(VARIABLE); GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE ERROR(4)\
&ensp;&ensp;END (\*VARDECLARATION\*);\
&ensp;&ensp;PROCEDURE LISTCODE;\
&ensp;&ensp;&ensp;&ensp;VAR I: INTEGER;\
&ensp;&ensp;BEGIN (\*LIST CODE GENERATED FOR THIS BLOCK\*)\
&ensp;&ensp;&ensp;&ensp;FOR I := CX0 TO CX - 1 DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH CODE[I] DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WRITELN(I, MNEMONIC[F]: 5, L: 3, A: 5)\
&ensp;&ensp;END (\*LISTCODE\*);\
&ensp;&ensp;PROCEDURE STATEMENT(FSYS: SYMSET);\
&ensp;&ensp;&ensp;&ensp;VAR I, CX1, CX2: INTEGER;\
&ensp;&ensp;&ensp;&ensp;PROCEDURE EXPRESSION(FSYS: SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR ADDOP: SYMBOL;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROCEDURE TERM(FSYS: SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR MULOP: SYMBOL;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROCEDURE FACTOR (FSYS: SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR I: INTEGER;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN TEST(FACBEGSYS, FSYS, 24);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE SYM IN FACBEGSYS DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = IDENT THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN I := POSITION(ID);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF I = 0 THEN ERROR(11) ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH TABLE[I] DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE KIND OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CONSTANT: GEN(LIT, 0, VAL);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VARIABLE: GEN(LOD, LEV - LEVEL, ADR);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROZEDURE: ERROR(21)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = NUMBER THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN IF NUM > AMAX THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ERROR(30); NUM := 0\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GEN(LIT, 0, NUM); GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = LPAREN THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM; EXPRESSION([RPAREN] + FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = RPAREN THEN GETSYM ELSE ERROR(22)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;TEST(FSYS, [LPAREN], 23)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END (\*FACTOR\*);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN (\*TERM\*) FACTOR(FSYS + [TIMES, SLASH]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE SYM IN [TIMES, SLASH] DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN MULOP := SYM; GETSYM; FACTOR(FSYS + [TIMES, SLASH]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF MULOP = TIMES THEN GEN(OPR, 0, 4) ELSE GEN(OPR, 0, 5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END (\*TERM\*);\
&ensp;&ensp;&ensp;&ensp;BEGIN (\*EXPRESSION\*)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM IN [PLUS, MINUS] THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ADDOP := SYM; GETSYM; TERM(FSYS + [PLUS, MINUS]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF ADDOP = MINUS THEN GEN(OPR, 0, 1)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE TERM(FSYS + [PLUS, MINUS]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE SYM IN [PLUS, MINUS] DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ADDOP := SYM; GETSYM; TERM(FSYS + [PLUS, MINUS]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF ADDOP = PLUS THEN GEN(OPR, 0, 2) ELSE GEN(OPR, 0, 3)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END (\*EXPRESSION\*);\
&ensp;&ensp;&ensp;&ensp;PROCEDURE CONDITION(FSYS: SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR RELOP: SYMBOL;\
&ensp;&ensp;&ensp;&ensp;BEGIN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = ODDSYM THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM; EXPRESSION(FSYS); GEN(OPR, 0, 6)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN EXPRESSION([EQL, NEQ, LSS, GTR, LEQ, GEQ] + FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF NOT (SYM IN [EQL, NEQ, LSS, LEQ, GTR, GEQ]) THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ERROR(20) ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN RELOP := SYM; GETSYM; EXPRESSION(FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE RELOP OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;EQL: GEN(OPR, 0, 8);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;NEQ: GEN(OPR, 0, 9);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LSS: GEN(OPR, 0, 10);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GEQ: GEN(OPR, 0, 11);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GTR: GEN(OPR, 0, 12);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LEQ: GEN(OPR, 0, 13)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END (\*CONDITION\*);\
&ensp;&ensp;BEGIN (\*STATEMENT\*)\
&ensp;&ensp;&ensp;&ensp;IF SYM = IDENT THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN I := POSITION(ID);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF I = 0 THEN ERROR(11) ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF TABLE[I].KIND <> VARIABLE THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN (\*ASSIGNMENT TO NON-VARIABLE\*) ERROR(12); I := 0\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM; IF SYM = BECOMES THEN GETSYM ELSE ERROR(13);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;EXPRESSION(FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF I <> 0 THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH TABLE[I] DO GEN(STO, LEV - LEVEL, ADR)\
&ensp;&ensp;&ensp;&ensp;END ELSE\
&ensp;&ensp;&ensp;&ensp;IF SYM = CALLSYM THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM <> IDENT THEN ERROR(14) ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN I := POSITION(ID);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF I = 0 THEN ERROR(11) ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH TABLE[I] DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF KIND = PROZEDURE THEN GEN(CAL, LEV - LEVEL, ADR)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ELSE ERROR(15);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END ELSE\
&ensp;&ensp;&ensp;&ensp;IF SYM = IFSYM THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM; CONDITION([THENSYM, DOSYM] + FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = THENSYM THEN GETSYM ELSE ERROR(16);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CX1 := CX; GEN(JPC, 0, 0);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STATEMENT(FSYS); CODE[CX1].A := CX\
&ensp;&ensp;&ensp;&ensp;END ELSE\
&ensp;&ensp;&ensp;&ensp;IF SYM = BEGINSYM THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM; STATEMENT([SEMICOLON, ENDSYM] + FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE SYM IN [SEMICOLON] + STATBEGSYS DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = SEMICOLON THEN GETSYM ELSE ERROR(10);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STATEMENT([SEMICOLON, ENDSYM] + FSYS)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = ENDSYM THEN GETSYM ELSE ERROR(17)\
&ensp;&ensp;&ensp;&ensp;END ELSE\
&ensp;&ensp;&ensp;&ensp;IF SYM = WHILESYM THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN CX1 := CX; GETSYM; CONDITION([DOSYM] + FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CX2 := CX; GEN(JPC, 0, 0);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = DOSYM THEN GETSYM ELSE ERROR(18);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STATEMENT(FSYS); GEN(JMP, 0, CX1); CODE[CX2].A := CX\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;TEST(FSYS, [], 19)\
&ensp;&ensp;END (\*STATEMENT\*);\
BEGIN (\*BLOCK\*) DX := 3; TX0 := TX; TABLE[TX].ADR := CX; GEN(JMP, 0, 0);\
&ensp;&ensp;IF LEV > LEVMAX THEN ERROR(32);\
&ensp;&ensp;REPEAT\
&ensp;&ensp;&ensp;&ensp;IF SYM = CONSTSYM THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;REPEAT CONSTDECLARATION;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE SYM = COMMA DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM; CONSTDECLARATION\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = SEMICOLON THEN GETSYM ELSE ERROR(5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;UNTIL SYM <> IDENT\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;IF SYM = VARSYM THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;REPEAT VARDECLARATION;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE SYM = COMMA DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM; VARDECLARATION\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = SEMICOLON THEN GETSYM ELSE ERROR(5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;UNTIL SYM <> IDENT\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;WHILE SYM = PROCSYM DO\
&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = IDENT THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN ENTER(PROZEDURE); GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE ERROR(4);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = SEMICOLON THEN GETSYM ELSE ERROR(5);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BLOCK(LEV + 1, TX, [SEMICOLON] + FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF SYM = SEMICOLON THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN GETSYM; TEST(STATBEGSYS + [IDENT, PROCSYM], FSYS, 6)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END ELSE ERROR(5)\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;TEST(STATBEGSYS + [IDENT], DECLBEGSYS, 7)\
&ensp;&ensp;UNTIL NOT (SYM IN DECLBEGSYS);\
&ensp;&ensp;CODE[TABLE[TX0].ADR].A := CX;\
&ensp;&ensp;WITH TABLE[TX0] DO\
&ensp;&ensp;ADR := CX; (\*START ADR OF CODE\*)\
&ensp;&ensp;CX0 := CX; GEN(INT, 0, DX);\
&ensp;&ensp;STATEMENT([SEMICOLON, ENDSYM] + FSYS);\
&ensp;&ensp;GEN(OPR, 0, 0); (\*RETURN\*)\
&ensp;&ensp;TEST(FSYS, [], 8);\
&ensp;&ensp;LISTCODE\
END (\*BLOCK\*);\
PROCEDURE INTERPRET;\
&ensp;&ensp;CONST STACKSIZE = 500;\
&ensp;&ensp;VAR P, B, T: INTEGER;&ensp;&ensp;(\*PROGRAM-, BASE-, TOPSTACK- REGISTERS\*)\
&ensp;&ensp;&ensp;&ensp;I: INSTRUCTION; (\*INSTRUCTION REGISTER\*)\
&ensp;&ensp;&ensp;&ensp;S: ARRAY [1..STACKSIZE] OF INTEGER;&ensp;&ensp;(\*DATASTORE\*)\
&ensp;&ensp;FUNCTION BASE(L: INTEGER): INTEGER;\
&ensp;&ensp;&ensp;&ensp;VAR B1: INTEGER;\
&ensp;&ensp;BEGIN B1 := B; (\*FIND BASE L LEVELS DOWN\*)\
&ensp;&ensp;&ensp;&ensp;WHILE L > 0 DO\
&ensp;&ensp;&ensp;&ensp;BEGIN B1 := S[B1]; L := L - 1\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;BASE := B1\
&ensp;&ensp;END (\*BASE\*);\
BEGIN WRITELN(&equiv; START PL/0&equiv;);\
&ensp;&ensp;T := 0; B := 1; P := 0;\
&ensp;&ensp;S[1] := 0; S[2] := 0; S[3] := 0;\
&ensp;&ensp;REPEAT I := CODE[P]; P := P + 1;\
&ensp;&ensp;&ensp;&ensp;WITH I DO\
&ensp;&ensp;&ensp;&ensp;CASE F OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LIT: BEGIN T := T + 1; S[T] := A\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;OPR: CASE A OF&ensp;&ensp;(\*OPERATOR\*)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;0: BEGIN&ensp;&ensp;(\*RETURN\*)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;T := B - 1; P := S[T + 3]; B := S[T + 2];\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;1: S[T] := -S[T];\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;2: BEGIN T := T - 1; S[T] := S[T] + S[T + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;3: BEGIN T := T - 1; S[T] := S[T] - S[T + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;4: BEGIN T := T - 1; S[T] := S[T] \* S[T + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;5: BEGIN T := T - 1; S[T] := S[T] DIV S[T + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;6: S[T] := ORD(ODD(S[T]));\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;8: BEGIN T := T - 1; S[T] := ORD(S[T] = S[T + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;9: BEGIN T := T - 1; S[T] := ORD(S[T] <> S[T + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;10: BEGIN T := T - 1; S[T] := ORD(S[T] < S[T + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;11: BEGIN T := T - 1; S[T] := ORD(S[T] >= S[T + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;12: BEGIN T := T - 1; S[T] := ORD(S[T] > S[T + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;13: BEGIN T := T - 1; S[T] := ORD(S[T] <= S[T + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LOD: BEGIN T := T + 1; S[T] := S[BASE(L) + A]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STO: BEGIN S[BASE(L) + A] := S[T]; WRITELN(S[T]); T := T - 1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CAL: BEGIN (\*GENERATE NEW BLOCK MARK\*)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;S[T + 1] := BASE(L); S[T + 2] := B; S[T + 3] := P;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;B := T + 1; P := A\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;INT: T := T + A;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;JMP: P := A;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;JPC: BEGIN IF S[T] = 0 THEN P := A; T := T - 1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END (\*WITH, CASE\*)\
&ensp;&ensp;UNTIL P = 0;\
&ensp;&ensp;WRITE(&equiv; END PL/0&equiv;)\
END (\*INTERPRET\*);\
BEGIN (\*MAIN PROGRAM\*)\
&ensp;&ensp;FOR CH := &equiv;A&equiv; TO &equiv;;&equiv; DO SSYM[CH] := NUL;\
&ensp;&ensp;WORD[ 1] := &equiv;BEGIN&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[ 2] := &equiv;CALL&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[ 3] := &equiv;CONST&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[ 4] := &equiv;DO&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[ 5] := &equiv;END&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[ 6] := &equiv;IF&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[ 7] := &equiv;ODD&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[ 8] := &equiv;PROCEDURE &equiv;;\
&ensp;&ensp;WORD[ 9] := &equiv;THEN&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[10] := &equiv;VAR&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[11] := &equiv;WHILE&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WSYM[ 1] := BEGINSYM;&ensp;&ensp;&ensp;WSYM[ 2] := CALLSYM;\
&ensp;&ensp;WSYM[ 3] := CONSTSYM;&ensp;&ensp;&ensp;WSYM[ 4] := DOSYM;\
&ensp;&ensp;WSYM[ 5] := ENDSYM;&ensp;&ensp;&ensp;&ensp;&ensp;WSYM[ 6] := IFSYM;\
&ensp;&ensp;WSYM[ 7] := ODDSYM;&ensp;&ensp;&ensp;&ensp;&ensp;WSYM[ 8] := PROCSYM;\
&ensp;&ensp;WSYM[ 9] := THENSYM;&ensp;&ensp;&ensp;&ensp;WSYM[10] := VARSYM;\
&ensp;&ensp;WSYM[11] := WHILESYM;\
&ensp;&ensp;SSYM[&equiv;+&equiv;] := PLUS;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;-&equiv;] := MINUS;\
&ensp;&ensp;SSYM[&equiv;\*&equiv;] := TIMES;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;/&equiv;] := SLASH;\
&ensp;&ensp;SSYM[&equiv;(&equiv;] := LPAREN;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;)&equiv;] := RPAREN;\
&ensp;&ensp;SSYM[&equiv;=&equiv;] := EQL;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;,&equiv;] := COMMA;\
&ensp;&ensp;SSYM[&equiv;.&equiv;] := PERIOD;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;#&equiv;] := NEQ;\
&ensp;&ensp;SSYM[&equiv;;&equiv;] := SEMICOLON;\
&ensp;&ensp;MNEMONIC[LIT] := &equiv; LIT &equiv;; MNEMONIC[OPR] := &equiv; OPR &equiv;;\
&ensp;&ensp;MNEMONIC[LOD] := &equiv; LOD &equiv;; MNEMONIC[STO] := &equiv; STO &equiv;;\
&ensp;&ensp;MNEMONIC[CAL] := &equiv; CAL &equiv;; MNEMONIC[INT] := &equiv; INT &equiv;;\
&ensp;&ensp;MNEMONIC[JMP] := &equiv; JMP &equiv;; MNEMONIC[JPC] := &equiv; JPC &equiv;;\
&ensp;&ensp;DECLBEGSYS := [CONSTSYM, VARSYM, PROCSYM];\
&ensp;&ensp;STATBEGSYS := [BEGINSYM, CALLSYM, IFSYM, WHILESYM];\
&ensp;&ensp;FACBEGSYS := [IDENT, NUMBER, LPAREN];\
&ensp;&ensp;PAGE(OUTPUT); ERR := 0;\
&ensp;&ensp;CC := 0; CX := 0; LL := 0; CH := &equiv;&ensp;&equiv;; KK := AL; GETSYM;\
&ensp;&ensp;BLOCK(0, 0, [PERIOD] + DECLBEGSYS + STATBEGSYS);\
&ensp;&ensp;IF SYM <> PERIOD THEN ERROR(9);\
&ensp;&ensp;IF ERR = 0 THEN INTERPRET ELSE WRITE(&equiv; ERRORS IN PL/0 PROGRAM&equiv;);\
99: WRITELN\
END.
