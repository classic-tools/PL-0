PROGRAM&ensp;PL0(INPUT,&ensp;OUTPUT);\
&#8625;PL/0&ensp;COMPILER&ensp;WITH&ensp;CODE&ensp;GENERATION&downarrow;\
LABEL&ensp;99;\
CONST&ensp;NORW&ensp;=&ensp;11;&ensp;&ensp;&#8625;NO.&ensp;OF&ensp;RESERVED&ensp;WORDS&downarrow;\
&ensp;&ensp;TXMAX&ensp;=&ensp;100;&ensp;&ensp;&ensp;&ensp;&#8625;LENGTH&ensp;OF&ensp;IDENTIFIER&ensp;TABLE&downarrow;\
&ensp;&ensp;NMAX&ensp;=&ensp;14;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;MAX&ensp;NO.&ensp;OF&ensp;DIGITS&ensp;IN&ensp;NUMBERS&downarrow;\
&ensp;&ensp;AL&ensp;=&ensp;10;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;LENGTH&ensp;OF&ensp;IDENTIFIERS&downarrow;\
&ensp;&ensp;LXMAX&ensp;=&ensp;81;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;LENGTH&ensp;OF&ensp;SOURCE&ensp;LINE&downarrow;\
&ensp;&ensp;AMAX&ensp;=&ensp;2047;&ensp;&ensp;&ensp;&ensp;&#8625;MAXIMUM&ensp;ADDRESS&downarrow;\
&ensp;&ensp;LEVMAX&ensp;=&ensp;3;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;MAXIMUM&ensp;DEPTH&ensp;OF&ensp;BLOCK&ensp;NESTING&downarrow;\
&ensp;&ensp;CXMAX&ensp;=&ensp;200;&ensp;&ensp;&ensp;&ensp;&#8625;SIZE&ensp;OF&ensp;CODE&ensp;ARRAY&downarrow;\
TYPE&ensp;SYMBOL&ensp;=\
&ensp;&ensp;(NUL,&ensp;IDENT,&ensp;NUMBER,&ensp;PLUS,&ensp;MINUS,&ensp;TIMES,&ensp;SLASH,&ensp;ODDSYM,\
&ensp;&ensp;&ensp;EQL,&ensp;NEQ,&ensp;LSS,&ensp;LEQ,&ensp;GTR,&ensp;GEQ,&ensp;LPAREN,&ensp;RPAREN,&ensp;COMMA,&ensp;SEMICOLON,\
&ensp;&ensp;&ensp;PERIOD,&ensp;BECOMES,&ensp;BEGINSYM,&ensp;ENDSYM,&ensp;IFSYM,&ensp;THENSYM,\
&ensp;&ensp;&ensp;WHILESYM,&ensp;DOSYM,&ensp;CALLSYM,&ensp;CONSTSYM,&ensp;VARSYM,&ensp;PROCSYM);\
&ensp;&ensp;ALFA&ensp;=&ensp;PACKED&ensp;ARRAY&ensp;[1..AL]&ensp;OF&ensp;CHAR;\
&ensp;&ensp;OBJECT&ensp;=&ensp;(CONSTANT,&ensp;VARIABLE,&ensp;PROZEDURE);\
&ensp;&ensp;SYMSET&ensp;=&ensp;SET&ensp;OF&ensp;SYMBOL;\
&ensp;&ensp;FCT&ensp;=&ensp;(LIT,&ensp;OPR,&ensp;LOD,&ensp;STO,&ensp;CAL,&ensp;INT,&ensp;JMP,&ensp;JPC);&ensp;&ensp;&#8625;FUNCTIONS&downarrow;\
&ensp;&ensp;INSTRUCTION&ensp;=&ensp;PACKED&ensp;RECORD\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;F:&ensp;FCT;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;FUNCTION&ensp;CODE&downarrow;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;L:&ensp;0..LEVMAX;&ensp;&ensp;&#8625;LEVEL&downarrow;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;A:&ensp;0..AMAX&ensp;&ensp;&ensp;&ensp;&#8625;DISPLACEMENT&ensp;ADDRESS&downarrow;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&#8625;&ensp;LIT&ensp;0,&ensp;A&ensp;&ensp;:&ensp;&ensp;LOAD&ensp;CONSTANT&ensp;A\
&ensp;&ensp;OPR&ensp;0,&ensp;A&ensp;&ensp;:&ensp;&ensp;EXECUTE&ensp;OPERATION&ensp;A\
&ensp;&ensp;LOD&ensp;L,&ensp;A&ensp;&ensp;:&ensp;&ensp;LOAD&ensp;VARIABLE&ensp;L,&ensp;A\
&ensp;&ensp;STO&ensp;L,&ensp;A&ensp;&ensp;:&ensp;&ensp;STORE&ensp;VARIABLE&ensp;L,&ensp;A\
&ensp;&ensp;CAL&ensp;L,&ensp;A&ensp;&ensp;:&ensp;&ensp;CALL&ensp;PROCEDURE&ensp;A&ensp;AT&ensp;LEVEL&ensp;L\
&ensp;&ensp;INT&ensp;0,&ensp;A&ensp;&ensp;:&ensp;&ensp;INCREMENT&ensp;T-REGISTER&ensp;BY&ensp;A\
&ensp;&ensp;JMP&ensp;0,&ensp;A&ensp;&ensp;:&ensp;&ensp;JUMP&ensp;TO&ensp;A\
&ensp;&ensp;JPC&ensp;0,&ensp;A&ensp;&ensp;:&ensp;&ensp;JUMP&ensp;CONDITIONAL&ensp;TO&ensp;A&ensp;&ensp;&downarrow;\
VAR&ensp;CH:&ensp;CHAR;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;LAST&ensp;CHARACTER&ensp;READ&downarrow;\
&ensp;&ensp;SYM:&ensp;SYMBOL;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;LAST&ensp;SYMBOL&ensp;READ&downarrow;\
&ensp;&ensp;ID:&ensp;ALFA;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;LAST&ensp;IDENTIFIER&ensp;READ&downarrow;\
&ensp;&ensp;NUM:&ensp;INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;LAST&ensp;NUMBER&ensp;READ&downarrow;\
&ensp;&ensp;CC:&ensp;INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;CHARACTER&ensp;COUNT&downarrow;\
&ensp;&ensp;LL:&ensp;INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;LINE&ensp;LENGTH&downarrow;\
&ensp;&ensp;KK,&ensp;ERR:&ensp;INTEGER;\
&ensp;&ensp;CX:&ensp;INTEGER;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&#8625;CODE&ensp;ALLOCATION&ensp;INDEX&downarrow;\
&ensp;&ensp;LINE:&ensp;ARRAY&ensp;[1..LXMAX]&ensp;OF&ensp;CHAR;\
&ensp;&ensp;A:&ensp;ALFA;\
&ensp;&ensp;CODE:&ensp;ARRAY&ensp;[0..CXMAX]&ensp;OF&ensp;INSTRUCTION;\
&ensp;&ensp;WORD:&ensp;ARRAY&ensp;[1..NORW]&ensp;OF&ensp;ALFA;\
&ensp;&ensp;WSYM:&ensp;ARRAY&ensp;[1..NORW]&ensp;OF&ensp;SYMBOL;\
&ensp;&ensp;SSYM:&ensp;ARRAY&ensp;[CHAR]&ensp;OF&ensp;SYMBOL;\
&ensp;&ensp;MNEMONIC:&ensp;ARRAY&ensp;[FCT]&ensp;OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PACKED&ensp;ARRAY&ensp;[1..5]&ensp;OF&ensp;CHAR;\
&ensp;&ensp;DECLBEGSYS,&ensp;STATBEGSYS,&ensp;FACBEGSYS:&ensp;SYMSET;\
&ensp;&ensp;TABLE:&ensp;ARRAY&ensp;[0..TXMAX]&ensp;OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;RECORD&ensp;NAME:&ensp;ALFA;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE&ensp;KIND:&ensp;OBJECT&ensp;OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CONSTANT:&ensp;(VAL:&ensp;INTEGER);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VARIABLE,&ensp;PROZEDURE:&ensp;(LEVEL,&ensp;ADR:&ensp;INTEGER)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
PROCEDURE&ensp;ERROR(N:&ensp;INTEGER);\
BEGIN&ensp;WRITELN(&equiv;&ensp;\*\*\*\*&equiv;,&ensp;&equiv;&ensp;&equiv;:&ensp;CC&ensp;-&ensp;1,&ensp;&equiv;&uparrow;&equiv;,&ensp;N:&ensp;2);&ensp;ERR&ensp;:=&ensp;ERR&ensp;+&ensp;1\
END&ensp;&#8625;ERROR&downarrow;;\
PROCEDURE&ensp;GETSYM;\
&ensp;&ensp;VAR&ensp;I,&ensp;J,&ensp;K:&ensp;INTEGER;\
&ensp;&ensp;PROCEDURE&ensp;GETCH;\
&ensp;&ensp;BEGIN&ensp;IF&ensp;CC&ensp;=&ensp;LL&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;IF&ensp;EOF(INPUT)&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;WRITE(&equiv;&ensp;PROGRAM&ensp;INCOMPLETE&equiv;);&ensp;GOTO&ensp;99\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LL&ensp;:=&ensp;0;&ensp;CC&ensp;:=&ensp;0;&ensp;WRITE(CX:&ensp;5,&ensp;&equiv;&ensp;&equiv;);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;&not;EOLN(INPUT)&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;LL&ensp;:=&ensp;LL&ensp;+&ensp;1;&ensp;READ(CH);&ensp;WRITE(CH);&ensp;LINE[LL]&ensp;:=&ensp;CH\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WRITELN;&ensp;LL&ensp;:=&ensp;LL&ensp;+&ensp;1;&ensp;READ(LINE[LL])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;CC&ensp;:=&ensp;CC&ensp;+&ensp;1;&ensp;CH&ensp;:=&ensp;LINE[CC]\
&ensp;&ensp;END&ensp;&#8625;GETCH&downarrow;;\
BEGIN&ensp;&#8625;GETSYM&downarrow;\
&ensp;&ensp;WHILE&ensp;CH&ensp;=&ensp;&equiv;&ensp;&equiv;&ensp;DO&ensp;GETCH;\
&ensp;&ensp;IF&ensp;CH&ensp;IN&ensp;[&equiv;A&equiv;..&equiv;Z&equiv;]&ensp;THEN\
&ensp;&ensp;BEGIN&ensp;&#8625;IDENTIFIER&ensp;OR&ensp;RESERVED&ensp;WORD&downarrow;&ensp;K&ensp;:=&ensp;0;\
&ensp;&ensp;&ensp;&ensp;REPEAT&ensp;IF&ensp;K&ensp;<&ensp;AL&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;K&ensp;:=&ensp;K&ensp;+&ensp;1;&ensp;A[K]&ensp;:=&ensp;CH\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETCH\
&ensp;&ensp;&ensp;&ensp;UNTIL&ensp;&not;(CH&ensp;IN&ensp;[&equiv;A&equiv;..&equiv;Z&equiv;,&ensp;&equiv;0&equiv;..&equiv;9&equiv;]);\
&ensp;&ensp;&ensp;&ensp;IF&ensp;K&ensp;&ge;&ensp;KK&ensp;THEN&ensp;KK&ensp;:=&ensp;K&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;REPEAT&ensp;A[KK]&ensp;:=&ensp;&equiv;&ensp;&equiv;;&ensp;KK&ensp;:=&ensp;KK&ensp;-&ensp;1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;UNTIL&ensp;KK&ensp;=&ensp;K;\
&ensp;&ensp;&ensp;&ensp;ID&ensp;:=&ensp;A;&ensp;I&ensp;:=&ensp;1;&ensp;J&ensp;:=&ensp;NORW;\
&ensp;&ensp;&ensp;&ensp;REPEAT&ensp;K&ensp;:=&ensp;(I&ensp;+&ensp;J)&ensp;DIV&ensp;2;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;ID&ensp;&le;&ensp;WORD[K]&ensp;THEN&ensp;J&ensp;:=&ensp;K&ensp;-&ensp;1;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;ID&ensp;&ge;&ensp;WORD[K]&ensp;THEN&ensp;I&ensp;:=&ensp;K&ensp;+&ensp;1\
&ensp;&ensp;&ensp;&ensp;UNTIL&ensp;I&ensp;>&ensp;J;\
&ensp;&ensp;&ensp;&ensp;IF&ensp;I&ensp;-&ensp;1&ensp;>&ensp;J&ensp;THEN&ensp;SYM&ensp;:=&ensp;WSYM[K]&ensp;ELSE&ensp;SYM&ensp;:=&ensp;IDENT\
&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;IF&ensp;CH&ensp;IN&ensp;[&equiv;0&equiv;..&equiv;9&equiv;]&ensp;THEN\
&ensp;&ensp;BEGIN&ensp;&#8625;NUMBER&downarrow;&ensp;K&ensp;:=&ensp;0;&ensp;NUM&ensp;:=&ensp;0;&ensp;SYM&ensp;:=&ensp;NUMBER;\
&ensp;&ensp;&ensp;&ensp;REPEAT&ensp;NUM&ensp;:=&ensp;10&ensp;\*&ensp;NUM&ensp;+&ensp;(ORD(CH)&ensp;-&ensp;ORD(&equiv;0&equiv;));\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;K&ensp;:=&ensp;K&ensp;+&ensp;1;&ensp;GETCH\
&ensp;&ensp;&ensp;&ensp;UNTIL&ensp;&not;(CH&ensp;IN&ensp;[&equiv;0&equiv;..&equiv;9&equiv;]);\
&ensp;&ensp;&ensp;&ensp;IF&ensp;K&ensp;>&ensp;NMAX&ensp;THEN&ensp;ERROR(30)\
&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;IF&ensp;CH&ensp;=&ensp;&equiv;:&equiv;&ensp;THEN\
&ensp;&ensp;BEGIN&ensp;GETCH;\
&ensp;&ensp;&ensp;&ensp;IF&ensp;CH&ensp;=&ensp;&equiv;=&equiv;&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;SYM&ensp;:=&ensp;BECOMES;&ensp;GETCH\
&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;SYM&ensp;:=&ensp;NUL\
&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;BEGIN&ensp;SYM&ensp;:=&ensp;SSYM[CH];&ensp;GETCH\
&ensp;&ensp;END\
END&ensp;&#8625;GETSYM&downarrow;;\
PROCEDURE&ensp;GEN(X:&ensp;FCT;&ensp;Y,&ensp;Z:&ensp;INTEGER);\
BEGIN&ensp;IF&ensp;CX&ensp;>&ensp;CXMAX&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;WRITE(&equiv;&ensp;PROGRAM&ensp;TOO&ensp;LONG&equiv;);&ensp;GOTO&ensp;99\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;WITH&ensp;CODE[CX]&ensp;DO\
&ensp;&ensp;BEGIN&ensp;F&ensp;:=&ensp;X;&ensp;L&ensp;:=&ensp;Y;&ensp;A&ensp;:=&ensp;Z\
&ensp;&ensp;END;\
&ensp;&ensp;CX&ensp;:=&ensp;CX&ensp;+&ensp;1\
END&ensp;&#8625;GEN&downarrow;;\
PROCEDURE&ensp;TEST(S1,&ensp;S2:&ensp;SYMSET;&ensp;N:&ensp;INTEGER);\
BEGIN&ensp;IF&ensp;&not;(SYM&ensp;IN&ensp;S1)&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ERROR(N);&ensp;S1&ensp;:=&ensp;S1&ensp;+&ensp;S2;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;&not;(SYM&ensp;IN&ensp;S1)&ensp;DO&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
END&ensp;&#8625;TEST&downarrow;;\
PROCEDURE&ensp;BLOCK(LEV,&ensp;TX:&ensp;INTEGER;&ensp;FSYS:&ensp;SYMSET);\
&ensp;&ensp;VAR&ensp;DX:&ensp;INTEGER;&ensp;&ensp;&#8625;DATA&ensp;ALLOCATION&ensp;INDEX&downarrow;\
&ensp;&ensp;&ensp;&ensp;TX0:&ensp;INTEGER;&ensp;&ensp;&ensp;&#8625;INITIAL&ensp;TABLE&ensp;INDEX&downarrow;\
&ensp;&ensp;&ensp;&ensp;CX0:&ensp;INTEGER;&ensp;&ensp;&ensp;&#8625;INITIAL&ensp;CODE&ensp;INDEX&downarrow;\
&ensp;&ensp;PROCEDURE&ensp;ENTER(K:&ensp;OBJECT);\
&ensp;&ensp;BEGIN&ensp;&#8625;ENTER&ensp;OBJECT&ensp;INTO&ensp;TABLE&downarrow;\
&ensp;&ensp;&ensp;&ensp;TX&ensp;:=&ensp;TX&ensp;+&ensp;1;\
&ensp;&ensp;&ensp;&ensp;WITH&ensp;TABLE[TX]&ensp;DO\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;NAME&ensp;:=&ensp;ID;&ensp;KIND&ensp;:=&ensp;K;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE&ensp;K&ensp;OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CONSTANT:&ensp;BEGIN&ensp;IF&ensp;NUM&ensp;>&ensp;AMAX&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ERROR(30);&ensp;NUM&ensp;:=&ensp;0&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAL&ensp;:=&ensp;NUM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VARIABLE:&ensp;BEGIN&ensp;LEVEL&ensp;:=&ensp;LEV;&ensp;ADR&ensp;:=&ensp;DX;&ensp;DX&ensp;:=&ensp;DX&ensp;+&ensp;1;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROZEDURE:&ensp;LEVEL&ensp;:=&ensp;LEV\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;END&ensp;&#8625;ENTER&downarrow;;\
&ensp;&ensp;FUNCTION&ensp;POSITION(ID:&ensp;ALFA):&ensp;INTEGER;\
&ensp;&ensp;&ensp;&ensp;VAR&ensp;I:&ensp;INTEGER;\
&ensp;&ensp;BEGIN&ensp;&#8625;FIND&ensp;IDENTIFIER&ensp;ID&ensp;IN&ensp;TABLE&downarrow;\
&ensp;&ensp;&ensp;&ensp;TABLE[0].NAME&ensp;:=&ensp;ID;&ensp;I&ensp;:=&ensp;TX;\
&ensp;&ensp;&ensp;&ensp;WHILE&ensp;TABLE[I].NAME&ensp;&ne;&ensp;ID&ensp;DO&ensp;I&ensp;:=&ensp;I&ensp;-&ensp;1;\
&ensp;&ensp;&ensp;&ensp;POSITION&ensp;:=&ensp;I\
&ensp;&ensp;END&ensp;&#8625;POSITION&downarrow;;\
&ensp;&ensp;PROCEDURE&ensp;CONSTDECLARATION;\
&ensp;&ensp;BEGIN&ensp;IF&ensp;SYM&ensp;=&ensp;IDENT&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;IN&ensp;[EQL,&ensp;BECOMES]&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;IF&ensp;SYM&ensp;=&ensp;BECOMES&ensp;THEN&ensp;ERROR(1);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;NUMBER&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ENTER(CONSTANT);&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;ERROR(2)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;ERROR(3)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;ERROR(4)\
&ensp;&ensp;END&ensp;&#8625;CONSTDECLARATION&downarrow;;\
&ensp;&ensp;PROCEDURE&ensp;VARDECLARATION;\
&ensp;&ensp;BEGIN&ensp;IF&ensp;SYM&ensp;=&ensp;IDENT&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ENTER(VARIABLE);&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;ERROR(4)\
&ensp;&ensp;END&ensp;&#8625;VARDECLARATION&downarrow;;\
&ensp;&ensp;PROCEDURE&ensp;LISTCODE;\
&ensp;&ensp;&ensp;&ensp;VAR&ensp;I:&ensp;INTEGER;\
&ensp;&ensp;BEGIN&ensp;&#8625;LIST&ensp;CODE&ensp;GENERATED&ensp;FOR&ensp;THIS&ensp;BLOCK&downarrow;\
&ensp;&ensp;&ensp;&ensp;FOR&ensp;I&ensp;:=&ensp;CX0&ensp;TO&ensp;CX&ensp;-&ensp;1&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH&ensp;CODE[I]&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WRITELN(I,&ensp;MNEMONIC[F]:&ensp;5,&ensp;L:&ensp;3,&ensp;A:&ensp;5)\
&ensp;&ensp;END&ensp;&#8625;LISTCODE&downarrow;;\
&ensp;&ensp;PROCEDURE&ensp;STATEMENT(FSYS:&ensp;SYMSET);\
&ensp;&ensp;&ensp;&ensp;VAR&ensp;I,&ensp;CX1,&ensp;CX2:&ensp;INTEGER;\
&ensp;&ensp;&ensp;&ensp;PROCEDURE&ensp;EXPRESSION(FSYS:&ensp;SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR&ensp;ADDOP:&ensp;SYMBOL;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROCEDURE&ensp;TERM(FSYS:&ensp;SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR&ensp;MULOP:&ensp;SYMBOL;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROCEDURE&ensp;FACTOR&ensp;(FSYS:&ensp;SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR&ensp;I:&ensp;INTEGER;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;TEST(FACBEGSYS,&ensp;FSYS,&ensp;24);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;SYM&ensp;IN&ensp;FACBEGSYS&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;IDENT&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;I&ensp;:=&ensp;POSITION(ID);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;I&ensp;=&ensp;0&ensp;THEN&ensp;ERROR(11)&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH&ensp;TABLE[I]&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE&ensp;KIND&ensp;OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CONSTANT:&ensp;GEN(LIT,&ensp;0,&ensp;VAL);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VARIABLE:&ensp;GEN(LOD,&ensp;LEV&ensp;-&ensp;LEVEL,&ensp;ADR);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;PROZEDURE:&ensp;ERROR(21)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;NUMBER&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;IF&ensp;NUM&ensp;>&ensp;AMAX&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ERROR(30);&ensp;NUM&ensp;:=&ensp;0\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GEN(LIT,&ensp;0,&ensp;NUM);&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;LPAREN&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;&ensp;EXPRESSION([RPAREN]&ensp;+&ensp;FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;RPAREN&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(22)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;TEST(FSYS,&ensp;[LPAREN],&ensp;23)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;&#8625;FACTOR&downarrow;;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;&#8625;TERM&downarrow;&ensp;FACTOR(FSYS&ensp;+&ensp;[TIMES,&ensp;SLASH]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;SYM&ensp;IN&ensp;[TIMES,&ensp;SLASH]&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;MULOP&ensp;:=&ensp;SYM;&ensp;GETSYM;&ensp;FACTOR(FSYS&ensp;+&ensp;[TIMES,&ensp;SLASH]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;MULOP&ensp;=&ensp;TIMES&ensp;THEN&ensp;GEN(OPR,&ensp;0,&ensp;4)&ensp;ELSE&ensp;GEN(OPR,&ensp;0,&ensp;5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;&#8625;TERM&downarrow;;\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;&#8625;EXPRESSION&downarrow;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;IN&ensp;[PLUS,&ensp;MINUS]&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ADDOP&ensp;:=&ensp;SYM;&ensp;GETSYM;&ensp;TERM(FSYS&ensp;+&ensp;[PLUS,&ensp;MINUS]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;ADDOP&ensp;=&ensp;MINUS&ensp;THEN&ensp;GEN(OPR,&ensp;0,&ensp;1)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;TERM(FSYS&ensp;+&ensp;[PLUS,&ensp;MINUS]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;SYM&ensp;IN&ensp;[PLUS,&ensp;MINUS]&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ADDOP&ensp;:=&ensp;SYM;&ensp;GETSYM;&ensp;TERM(FSYS&ensp;+&ensp;[PLUS,&ensp;MINUS]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;ADDOP&ensp;=&ensp;PLUS&ensp;THEN&ensp;GEN(OPR,&ensp;0,&ensp;2)&ensp;ELSE&ensp;GEN(OPR,&ensp;0,&ensp;3)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END&ensp;&#8625;EXPRESSION&downarrow;;\
&ensp;&ensp;&ensp;&ensp;PROCEDURE&ensp;CONDITION(FSYS:&ensp;SYMSET);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;VAR&ensp;RELOP:&ensp;SYMBOL;\
&ensp;&ensp;&ensp;&ensp;BEGIN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;ODDSYM&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;&ensp;EXPRESSION(FSYS);&ensp;GEN(OPR,&ensp;0,&ensp;6)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;EXPRESSION([EQL,&ensp;NEQ,&ensp;LSS,&ensp;GTR,&ensp;LEQ,&ensp;GEQ]&ensp;+&ensp;FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;&not;(SYM&ensp;IN&ensp;[EQL,&ensp;NEQ,&ensp;LSS,&ensp;LEQ,&ensp;GTR,&ensp;GEQ])&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ERROR(20)&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;RELOP&ensp;:=&ensp;SYM;&ensp;GETSYM;&ensp;EXPRESSION(FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CASE&ensp;RELOP&ensp;OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;EQL:&ensp;GEN(OPR,&ensp;0,&ensp;8);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;NEQ:&ensp;GEN(OPR,&ensp;0,&ensp;9);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LSS:&ensp;GEN(OPR,&ensp;0,&ensp;10);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GEQ:&ensp;GEN(OPR,&ensp;0,&ensp;11);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GTR:&ensp;GEN(OPR,&ensp;0,&ensp;12);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LEQ:&ensp;GEN(OPR,&ensp;0,&ensp;13)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END&ensp;&#8625;CONDITION&downarrow;;\
&ensp;&ensp;BEGIN&ensp;&#8625;STATEMENT&downarrow;\
&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;IDENT&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;I&ensp;:=&ensp;POSITION(ID);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;I&ensp;=&ensp;0&ensp;THEN&ensp;ERROR(11)&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;TABLE[I].KIND&ensp;&ne;&ensp;VARIABLE&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;&#8625;ASSIGNMENT&ensp;TO&ensp;NON-VARIABLE&downarrow;&ensp;ERROR(12);&ensp;I&ensp;:=&ensp;0\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM;&ensp;IF&ensp;SYM&ensp;=&ensp;BECOMES&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(13);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;EXPRESSION(FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;I&ensp;&ne;&ensp;0&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH&ensp;TABLE[I]&ensp;DO&ensp;GEN(STO,&ensp;LEV&ensp;-&ensp;LEVEL,&ensp;ADR)\
&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;CALLSYM&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;&ne;&ensp;IDENT&ensp;THEN&ensp;ERROR(14)&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;I&ensp;:=&ensp;POSITION(ID);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;I&ensp;=&ensp;0&ensp;THEN&ensp;ERROR(11)&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WITH&ensp;TABLE[I]&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;KIND&ensp;=&ensp;PROZEDURE&ensp;THEN&ensp;GEN(CAL,&ensp;LEV&ensp;-&ensp;LEVEL,&ensp;ADR)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ELSE&ensp;ERROR(15);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;IFSYM&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;&ensp;CONDITION([THENSYM,&ensp;DOSYM]&ensp;+&ensp;FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;THENSYM&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(16);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CX1&ensp;:=&ensp;CX;&ensp;GEN(JPC,&ensp;0,&ensp;0);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STATEMENT(FSYS);&ensp;CODE[CX1].A&ensp;:=&ensp;CX\
&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;BEGINSYM&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;&ensp;STATEMENT([SEMICOLON,&ensp;ENDSYM]&ensp;+&ensp;FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;SYM&ensp;IN&ensp;[SEMICOLON]&ensp;+&ensp;STATBEGSYS&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;SEMICOLON&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(10);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STATEMENT([SEMICOLON,&ensp;ENDSYM]&ensp;+&ensp;FSYS)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;ENDSYM&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(17)\
&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE\
&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;WHILESYM&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;CX1&ensp;:=&ensp;CX;&ensp;GETSYM;&ensp;CONDITION([DOSYM]&ensp;+&ensp;FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CX2&ensp;:=&ensp;CX;&ensp;GEN(JPC,&ensp;0,&ensp;0);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;DOSYM&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(18);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STATEMENT(FSYS);&ensp;GEN(JMP,&ensp;0,&ensp;CX1);&ensp;CODE[CX2].A&ensp;:=&ensp;CX\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;TEST(FSYS,&ensp;[],&ensp;19)\
&ensp;&ensp;END&ensp;&#8625;STATEMENT&downarrow;;\
BEGIN&ensp;&#8625;BLOCK&downarrow;&ensp;DX&ensp;:=&ensp;3;&ensp;TX0&ensp;:=&ensp;TX;&ensp;TABLE[TX].ADR&ensp;:=&ensp;CX;&ensp;GEN(JMP,&ensp;0,&ensp;0);\
&ensp;&ensp;IF&ensp;LEV&ensp;>&ensp;LEVMAX&ensp;THEN&ensp;ERROR(32);\
&ensp;&ensp;REPEAT\
&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;CONSTSYM&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;REPEAT&ensp;CONSTDECLARATION;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;SYM&ensp;=&ensp;COMMA&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;&ensp;CONSTDECLARATION\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;SEMICOLON&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;UNTIL&ensp;SYM&ensp;&ne;&ensp;IDENT\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;VARSYM&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;REPEAT&ensp;VARDECLARATION;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;WHILE&ensp;SYM&ensp;=&ensp;COMMA&ensp;DO\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;&ensp;VARDECLARATION\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;SEMICOLON&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;UNTIL&ensp;SYM&ensp;&ne;&ensp;IDENT\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;WHILE&ensp;SYM&ensp;=&ensp;PROCSYM&ensp;DO\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;IDENT&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;ENTER(PROZEDURE);&ensp;GETSYM\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;ERROR(4);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;SEMICOLON&ensp;THEN&ensp;GETSYM&ensp;ELSE&ensp;ERROR(5);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BLOCK(LEV&ensp;+&ensp;1,&ensp;TX,&ensp;[SEMICOLON]&ensp;+&ensp;FSYS);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;IF&ensp;SYM&ensp;=&ensp;SEMICOLON&ensp;THEN\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;GETSYM;&ensp;TEST(STATBEGSYS&ensp;+&ensp;[IDENT,&ensp;PROCSYM],&ensp;FSYS,&ensp;6)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END&ensp;ELSE&ensp;ERROR(5)\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;TEST(STATBEGSYS&ensp;+&ensp;[IDENT],&ensp;DECLBEGSYS,&ensp;7)\
&ensp;&ensp;UNTIL&ensp;&not;(SYM&ensp;IN&ensp;DECLBEGSYS);\
&ensp;&ensp;CODE[TABLE[TX0].ADR].A&ensp;:=&ensp;CX;\
&ensp;&ensp;WITH&ensp;TABLE[TX0]&ensp;DO\
&ensp;&ensp;ADR&ensp;:=&ensp;CX;&ensp;&#8625;START&ensp;ADR&ensp;OF&ensp;CODE&downarrow;\
&ensp;&ensp;CX0&ensp;:=&ensp;CX;&ensp;GEN(INT,&ensp;0,&ensp;DX);\
&ensp;&ensp;STATEMENT([SEMICOLON,&ensp;ENDSYM]&ensp;+&ensp;FSYS);\
&ensp;&ensp;GEN(OPR,&ensp;0,&ensp;0);&ensp;&#8625;RETURN&downarrow;\
&ensp;&ensp;TEST(FSYS,&ensp;[],&ensp;8);\
&ensp;&ensp;LISTCODE\
END&ensp;&#8625;BLOCK&downarrow;;\
PROCEDURE&ensp;INTERPRET;\
&ensp;&ensp;CONST&ensp;STACKSIZE&ensp;=&ensp;500;\
&ensp;&ensp;VAR&ensp;P,&ensp;B,&ensp;T:&ensp;INTEGER;&ensp;&ensp;&#8625;PROGRAM-,&ensp;BASE-,&ensp;TOPSTACK-REGISTERS&downarrow;\
&ensp;&ensp;&ensp;&ensp;I:&ensp;INSTRUCTION;&ensp;&#8625;INSTRUCTION&ensp;REGISTER&downarrow;\
&ensp;&ensp;&ensp;&ensp;S:&ensp;ARRAY&ensp;[1..STACKSIZE]&ensp;OF&ensp;INTEGER;&ensp;&ensp;&#8625;DATASTORE&downarrow;\
&ensp;&ensp;FUNCTION&ensp;BASE(L:&ensp;INTEGER):&ensp;INTEGER;\
&ensp;&ensp;&ensp;&ensp;VAR&ensp;B1:&ensp;INTEGER;\
&ensp;&ensp;BEGIN&ensp;B1&ensp;:=&ensp;B;&ensp;&#8625;FIND&ensp;BASE&ensp;L&ensp;LEVELS&ensp;DOWN&downarrow;\
&ensp;&ensp;&ensp;&ensp;WHILE&ensp;L&ensp;>&ensp;0&ensp;DO\
&ensp;&ensp;&ensp;&ensp;BEGIN&ensp;B1&ensp;:=&ensp;S[B1];&ensp;L&ensp;:=&ensp;L&ensp;-&ensp;1\
&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;BASE&ensp;:=&ensp;B1\
&ensp;&ensp;END&ensp;&#8625;BASE&downarrow;;\
BEGIN&ensp;WRITELN(&equiv;&ensp;START&ensp;PL/0&equiv;);\
&ensp;&ensp;T&ensp;:=&ensp;0;&ensp;B&ensp;:=&ensp;1;&ensp;P&ensp;:=&ensp;0;\
&ensp;&ensp;S[1]&ensp;:=&ensp;0;&ensp;S[2]&ensp;:=&ensp;0;&ensp;S[3]&ensp;:=&ensp;0;\
&ensp;&ensp;REPEAT&ensp;I&ensp;:=&ensp;CODE[P];&ensp;P&ensp;:=&ensp;P&ensp;+&ensp;1;\
&ensp;&ensp;&ensp;&ensp;WITH&ensp;I&ensp;DO\
&ensp;&ensp;&ensp;&ensp;CASE&ensp;F&ensp;OF\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LIT:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;+&ensp;1;&ensp;S[T]&ensp;:=&ensp;A\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;OPR:&ensp;CASE&ensp;A&ensp;OF&ensp;&ensp;&#8625;OPERATOR&downarrow;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;0:&ensp;BEGIN&ensp;&ensp;&#8625;RETURN&downarrow;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;T&ensp;:=&ensp;B&ensp;-&ensp;1;&ensp;P&ensp;:=&ensp;S[T&ensp;+&ensp;3];&ensp;B&ensp;:=&ensp;S[T&ensp;+&ensp;2];\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;1:&ensp;S[T]&ensp;:=&ensp;-S[T];\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;2:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;S[T]&ensp;+&ensp;S[T&ensp;+&ensp;1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;3:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;S[T]&ensp;-&ensp;S[T&ensp;+&ensp;1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;4:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;S[T]&ensp;\*&ensp;S[T&ensp;+&ensp;1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;5:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;S[T]&ensp;DIV&ensp;S[T&ensp;+&ensp;1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;6:&ensp;S[T]&ensp;:=&ensp;ORD(ODD(S[T]));\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;8:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;ORD(S[T]&ensp;=&ensp;S[T&ensp;+&ensp;1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;9:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;ORD(S[T]&ensp;&ne;&ensp;S[T&ensp;+&ensp;1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;10:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;ORD(S[T]&ensp;<&ensp;S[T&ensp;+&ensp;1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;11:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;ORD(S[T]&ensp;&ge;&ensp;S[T&ensp;+&ensp;1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;12:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;ORD(S[T]&ensp;>&ensp;S[T&ensp;+&ensp;1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;13:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1;&ensp;S[T]&ensp;:=&ensp;ORD(S[T]&ensp;&le;&ensp;S[T&ensp;+&ensp;1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;LOD:&ensp;BEGIN&ensp;T&ensp;:=&ensp;T&ensp;+&ensp;1;&ensp;S[T]&ensp;:=&ensp;S[BASE(L)&ensp;+&ensp;A]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;STO:&ensp;BEGIN&ensp;S[BASE(L)&ensp;+&ensp;A]&ensp;:=&ensp;S[T];&ensp;WRITELN(S[T]);&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;CAL:&ensp;BEGIN&ensp;&#8625;GENERATE&ensp;NEW&ensp;BLOCK&ensp;MARK&downarrow;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;S[T&ensp;+&ensp;1]&ensp;:=&ensp;BASE(L);&ensp;S[T&ensp;+&ensp;2]&ensp;:=&ensp;B;&ensp;S[T&ensp;+&ensp;3]&ensp;:=&ensp;P;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;B&ensp;:=&ensp;T&ensp;+&ensp;1;&ensp;P&ensp;:=&ensp;A\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;INT:&ensp;T&ensp;:=&ensp;T&ensp;+&ensp;A;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;JMP:&ensp;P&ensp;:=&ensp;A;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;JPC:&ensp;BEGIN&ensp;IF&ensp;S[T]&ensp;=&ensp;0&ensp;THEN&ensp;P&ensp;:=&ensp;A;&ensp;T&ensp;:=&ensp;T&ensp;-&ensp;1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;END\
&ensp;&ensp;&ensp;&ensp;END&ensp;&#8625;WITH,&ensp;CASE&downarrow;\
&ensp;&ensp;UNTIL&ensp;P&ensp;=&ensp;0;\
&ensp;&ensp;WRITE(&equiv;&ensp;END&ensp;PL/0&equiv;)\
END&ensp;&#8625;INTERPRET&downarrow;;\
BEGIN&ensp;&#8625;MAIN&ensp;PROGRAM&downarrow;\
&ensp;&ensp;FOR&ensp;CH&ensp;:=&ensp;&equiv;A&equiv;&ensp;TO&ensp;&equiv;;&equiv;&ensp;DO&ensp;SSYM[CH]&ensp;:=&ensp;NUL;\
&ensp;&ensp;WORD[&ensp;1]&ensp;:=&ensp;&equiv;BEGIN&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[&ensp;2]&ensp;:=&ensp;&equiv;CALL&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[&ensp;3]&ensp;:=&ensp;&equiv;CONST&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[&ensp;4]&ensp;:=&ensp;&equiv;DO&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[&ensp;5]&ensp;:=&ensp;&equiv;END&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[&ensp;6]&ensp;:=&ensp;&equiv;IF&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[&ensp;7]&ensp;:=&ensp;&equiv;ODD&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[&ensp;8]&ensp;:=&ensp;&equiv;PROCEDURE&ensp;&equiv;;\
&ensp;&ensp;WORD[&ensp;9]&ensp;:=&ensp;&equiv;THEN&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;&ensp;&ensp;&ensp;&ensp;WORD[10]&ensp;:=&ensp;&equiv;VAR&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WORD[11]&ensp;:=&ensp;&equiv;WHILE&ensp;&ensp;&ensp;&ensp;&ensp;&equiv;;\
&ensp;&ensp;WSYM[&ensp;1]&ensp;:=&ensp;BEGINSYM;&ensp;&ensp;&ensp;WSYM[&ensp;2]&ensp;:=&ensp;CALLSYM;\
&ensp;&ensp;WSYM[&ensp;3]&ensp;:=&ensp;CONSTSYM;&ensp;&ensp;&ensp;WSYM[&ensp;4]&ensp;:=&ensp;DOSYM;\
&ensp;&ensp;WSYM[&ensp;5]&ensp;:=&ensp;ENDSYM;&ensp;&ensp;&ensp;&ensp;&ensp;WSYM[&ensp;6]&ensp;:=&ensp;IFSYM;\
&ensp;&ensp;WSYM[&ensp;7]&ensp;:=&ensp;ODDSYM;&ensp;&ensp;&ensp;&ensp;&ensp;WSYM[&ensp;8]&ensp;:=&ensp;PROCSYM;\
&ensp;&ensp;WSYM[&ensp;9]&ensp;:=&ensp;THENSYM;&ensp;&ensp;&ensp;&ensp;WSYM[10]&ensp;:=&ensp;VARSYM;\
&ensp;&ensp;WSYM[11]&ensp;:=&ensp;WHILESYM;\
&ensp;&ensp;SSYM[&equiv;+&equiv;]&ensp;:=&ensp;PLUS;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;-&equiv;]&ensp;:=&ensp;MINUS;\
&ensp;&ensp;SSYM[&equiv;\*&equiv;]&ensp;:=&ensp;TIMES;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;/&equiv;]&ensp;:=&ensp;SLASH;\
&ensp;&ensp;SSYM[&equiv;(&equiv;]&ensp;:=&ensp;LPAREN;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;)&equiv;]&ensp;:=&ensp;RPAREN;\
&ensp;&ensp;SSYM[&equiv;=&equiv;]&ensp;:=&ensp;EQL;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;,&equiv;]&ensp;:=&ensp;COMMA;\
&ensp;&ensp;SSYM[&equiv;.&equiv;]&ensp;:=&ensp;PERIOD;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;&ne;&equiv;]&ensp;:=&ensp;NEQ;\
&ensp;&ensp;SSYM[&equiv;<&equiv;]&ensp;:=&ensp;LSS;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;>&equiv;]&ensp;:=&ensp;GTR;\
&ensp;&ensp;SSYM[&equiv;&le;&equiv;]&ensp;:=&ensp;LEQ;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;SSYM[&equiv;&ge;&equiv;]&ensp;:=&ensp;GEQ;\
&ensp;&ensp;SSYM[&equiv;;&equiv;]&ensp;:=&ensp;SEMICOLON;\
&ensp;&ensp;MNEMONIC[LIT]&ensp;:=&ensp;&equiv;&ensp;LIT&ensp;&equiv;;&ensp;MNEMONIC[OPR]&ensp;:=&ensp;&equiv;&ensp;OPR&ensp;&equiv;;\
&ensp;&ensp;MNEMONIC[LOD]&ensp;:=&ensp;&equiv;&ensp;LOD&ensp;&equiv;;&ensp;MNEMONIC[STO]&ensp;:=&ensp;&equiv;&ensp;STO&ensp;&equiv;;\
&ensp;&ensp;MNEMONIC[CAL]&ensp;:=&ensp;&equiv;&ensp;CAL&ensp;&equiv;;&ensp;MNEMONIC[INT]&ensp;:=&ensp;&equiv;&ensp;INT&ensp;&equiv;;\
&ensp;&ensp;MNEMONIC[JMP]&ensp;:=&ensp;&equiv;&ensp;JMP&ensp;&equiv;;&ensp;MNEMONIC[JPC]&ensp;:=&ensp;&equiv;&ensp;JPC&ensp;&equiv;;\
&ensp;&ensp;DECLBEGSYS&ensp;:=&ensp;[CONSTSYM,&ensp;VARSYM,&ensp;PROCSYM];\
&ensp;&ensp;STATBEGSYS&ensp;:=&ensp;[BEGINSYM,&ensp;CALLSYM,&ensp;IFSYM,&ensp;WHILESYM];\
&ensp;&ensp;FACBEGSYS&ensp;:=&ensp;[IDENT,&ensp;NUMBER,&ensp;LPAREN];\
&ensp;&ensp;PAGE(OUTPUT);&ensp;ERR&ensp;:=&ensp;0;\
&ensp;&ensp;CC&ensp;:=&ensp;0;&ensp;CX&ensp;:=&ensp;0;&ensp;LL&ensp;:=&ensp;0;&ensp;CH&ensp;:=&ensp;&equiv;&ensp;&equiv;;&ensp;KK&ensp;:=&ensp;AL;&ensp;GETSYM;\
&ensp;&ensp;BLOCK(0,&ensp;0,&ensp;[PERIOD]&ensp;+&ensp;DECLBEGSYS&ensp;+&ensp;STATBEGSYS);\
&ensp;&ensp;IF&ensp;SYM&ensp;&ne;&ensp;PERIOD&ensp;THEN&ensp;ERROR(9);\
&ensp;&ensp;IF&ensp;ERR&ensp;=&ensp;0&ensp;THEN&ensp;INTERPRET&ensp;ELSE&ensp;WRITE(&equiv;&ensp;ERRORS&ensp;IN&ensp;PL/0&ensp;PROGRAM&equiv;);\
99:&ensp;WRITELN\
END.
