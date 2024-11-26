**program** PL0(input, output);\
{PL/0 compiler with code generation}\
**label** 99;\
**const** norw = 11;&ensp;&ensp;{no. of reserved words}\
&ensp;&ensp;txmax = 100;&ensp;&ensp;&ensp;&ensp;{length of identifier table}\
&ensp;&ensp;nmax = 14;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{max. no. of digits in numbers}\
&ensp;&ensp;al = 10;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{length of identifiers}\
&ensp;&ensp;amax = 2047;&ensp;&ensp;&ensp;&ensp;{maximum address}\
&ensp;&ensp;levmax = 3;&ensp;&ensp;&ensp;&ensp;&ensp;{maximum depth of block nesting}\
&ensp;&ensp;cxmax = 200;&ensp;&ensp;&ensp;&ensp;{size of code array}\
**type** symbol =\
&ensp;&ensp;(nul, ident, number, plus, minus, times, slash, oddsym,\
&ensp;&ensp;&ensp;eql, neq, lss, leq, gtr, geq, lparen, rparen, comma, semicolon,\
&ensp;&ensp;&ensp;period, becomes, beginsym, endsym, ifsym, thensym,\
&ensp;&ensp;&ensp;whilesym, dosym, callsym, constsym, varsym, procsym);\
&ensp;&ensp;alfa = **packed** **array** [1..al] **of** char;\
&ensp;&ensp;object = (constant, variable, procedure);\
&ensp;&ensp;symset = **set** **of** symbol;\
&ensp;&ensp;fct = (lit, opr, lod, sto, cal, int, jmp, jpc);&ensp;&ensp;{functions}\
&ensp;&ensp;instruction = **packed** **record**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;f: fct;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{function code}\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;l: 0..levmax;&ensp;&ensp;{level}\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;a: 0..amax;&ensp;&ensp;&ensp;&ensp;{displacement address}\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
{ LIT 0, a&ensp;&ensp;:&ensp;&ensp;load constant a\
&ensp;&ensp;OPR 0, a&ensp;&ensp;:&ensp;&ensp;execute operation a\
&ensp;&ensp;LOD l, a&ensp;&ensp;:&ensp;&ensp;load variable l, a\
&ensp;&ensp;STO l, a&ensp;&ensp;:&ensp;&ensp;store variable l, a\
&ensp;&ensp;CAL l, a&ensp;&ensp;:&ensp;&ensp;call procedure a at level l\
&ensp;&ensp;INT 0, a&ensp;&ensp;:&ensp;&ensp;increment t-register by a\
&ensp;&ensp;JMP 0, a&ensp;&ensp;:&ensp;&ensp;jump to a\
&ensp;&ensp;JPC 0, a&ensp;&ensp;:&ensp;&ensp;jump conditional to a&ensp;&ensp;}\
**var** ch: char;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{last character read}\
&ensp;&ensp;sym: symbol;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{last symbol read}\
&ensp;&ensp;id: alfa;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{last identifier read}\
&ensp;&ensp;num: integer;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{last number read}\
&ensp;&ensp;cc: integer;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{character count}\
&ensp;&ensp;ll: integer;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{line length}\
&ensp;&ensp;kk, err: integer;\
&ensp;&ensp;cx: integer;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;{code allocation index}\
&ensp;&ensp;line: **array** [1..81] **of** char;\
&ensp;&ensp;a: alfa;\
&ensp;&ensp;code: **array** [0..cxmax] **of** instruction;\
&ensp;&ensp;word: **array** [1..norw] **of** alfa;\
&ensp;&ensp;wsym: **array** [1..norw] **of** symbol;\
&ensp;&ensp;ssym: **array** [char] **of** symbol;\
&ensp;&ensp;mnemonic: **array** [fct] **of**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**packed** **array** [1..5] **of** char;\
&ensp;&ensp;declbegsys, statbegsys, facbegsys: symset;\
&ensp;&ensp;table: **array** [0..txmax] **of**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**record** name: alfa;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**case** kind: object **of**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;constant: (val: integer);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;variable, procedure: (level, adr: integer)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
**procedure** error(n: integer);\
**begin** writeln(' \*\*\*\*', ' ': cc - 1, '&uparrow;', n: 2); err := err + 1\
**end** {error};\
**procedure** getsym;\
&ensp;&ensp;**var** i, j, k: integer;\
&ensp;&ensp;**procedure** getch;\
&ensp;&ensp;**begin** **if** cc = ll **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** **if** eof(input) **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** write(' PROGRAM INCOMPLETE'); **goto** 99\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ll := 0; cc := 0; write(cx: 5, ' ');\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** &not;eoln(input) **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** ll := ll + 1; read(ch); write(ch); line[ll] := ch\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;writeln; ll := ll + 1; read(line[ll])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;cc := cc + 1; ch := line[cc]\
&ensp;&ensp;**end** {getch};\
**begin** {getsym}\
&ensp;&ensp;**while** ch = ' ' **do** getch;\
&ensp;&ensp;**if** ch **in** ['A'..'Z'] **then**\
&ensp;&ensp;**begin** {identifier or reserved word} k := 0;\
&ensp;&ensp;&ensp;&ensp;**repeat** **if** k < al **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** k := k + 1; a[k] := ch\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;getch\
&ensp;&ensp;&ensp;&ensp;**until** &not;(ch **in** ['A'..'Z', '0'..'9']);\
&ensp;&ensp;&ensp;&ensp;**if** k &ge; kk **then** kk := k **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**repeat** a[kk] := ' '; kk := kk - 1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**until** kk = k;\
&ensp;&ensp;&ensp;&ensp;id := a; i := 1; j := norw;\
&ensp;&ensp;&ensp;&ensp;**repeat** k := (i + j) div 2;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** id &le; word[k] **then** j := k - 1;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** id &ge; word[k] **then** i := k + 1\
&ensp;&ensp;&ensp;&ensp;**until** i > j;\
&ensp;&ensp;&ensp;&ensp;**if** i - 1 > j **then** sym := wsym[k] **else** sym := ident\
&ensp;&ensp;**end** **else**\
&ensp;&ensp;**if** ch **in** ['0'..'9'] **then**\
&ensp;&ensp;**begin** {number} k := 0; num := 0; sym := number;\
&ensp;&ensp;&ensp;&ensp;**repeat** num := 10 \* num + (ord(ch) - ord('0'));\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;k := k + 1; getch\
&ensp;&ensp;&ensp;&ensp;**until** &not;(ch **in** ['0'..'9']);\
&ensp;&ensp;&ensp;&ensp;**if** k > nmax **then** error(30)\
&ensp;&ensp;**end** **else**\
&ensp;&ensp;**if** ch = ':' **then**\
&ensp;&ensp;**begin** getch;\
&ensp;&ensp;&ensp;&ensp;**if** ch = '=' **then**\
&ensp;&ensp;&ensp;&ensp;**begin** sym := becomes; getch\
&ensp;&ensp;&ensp;&ensp;**end** **else** sym := nul;\
&ensp;&ensp;**end** **else**\
&ensp;&ensp;**begin** sym := ssym[ch]; getch\
&ensp;&ensp;**end**\
**end** {getsym};\
**procedure** gen(x: fct; y, z: integer);\
**begin** **if** cx > cxmax **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** write(' PROGRAM TOO LONG'); **goto** 99\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;**with** code[cx] **do**\
&ensp;&ensp;**begin** f := x; l := y; a := z\
&ensp;&ensp;**end**;\
&ensp;&ensp;cx := cx + 1\
**end** {gen};\
**procedure** test(s1, s2: symset; n: integer);\
**begin** **if** &not;(sym **in** s1) **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** error(n); s1 := s1 + s2;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** &not;(sym **in** s1) **do** getsym\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
**end** {test};\
**procedure** block(lev, tx: integer; fsys: symset);\
&ensp;&ensp;**var** dx: integer;&ensp;&ensp;{data allocation index}\
&ensp;&ensp;&ensp;&ensp;tx0: integer;&ensp;&ensp;&ensp;{initial table index}\
&ensp;&ensp;&ensp;&ensp;cx0: integer;&ensp;&ensp;&ensp;{initial code index}\
&ensp;&ensp;**procedure** enter(k: object);\
&ensp;&ensp;**begin** {enter object into table}\
&ensp;&ensp;&ensp;&ensp;tx := tx + 1;\
&ensp;&ensp;&ensp;&ensp;**with** table[tx] **do**\
&ensp;&ensp;&ensp;&ensp;**begin** name := id; kind := k;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**case** k **of**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;constant: **begin** **if** num > amax **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** error(30); num := 0 **end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;val := num\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;variable: **begin** level := lev; adr := dx; dx := dx + 1;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;procedure: level := lev\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;**end** {enter};\
&ensp;&ensp;**function** position(id: alfa): integer;\
&ensp;&ensp;&ensp;&ensp;**var** i: integer;\
&ensp;&ensp;**begin** {find identifier id in table}\
&ensp;&ensp;&ensp;&ensp;table[0].name := id; i := tx;\
&ensp;&ensp;&ensp;&ensp;**while** table[i].name &ne; id **do** i := i - 1;\
&ensp;&ensp;&ensp;&ensp;position := i\
&ensp;&ensp;**end** {position};\
&ensp;&ensp;**procedure** constdeclaration;\
&ensp;&ensp;**begin** **if** sym = ident **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** getsym;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym **in** [eql, becomes] **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** **if** sym = becomes **then** error(1);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;getsym;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = number **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** enter(constant); getsym\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**else** error(2)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** **else** error(3)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** **else** error(4)\
&ensp;&ensp;**end** {constdeclaration};\
&ensp;&ensp;**procedure** vardeclaration;\
&ensp;&ensp;**begin** **if** sym = ident **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** enter(variable); getsym\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** **else** error(4)\
&ensp;&ensp;**end** {vardeclaration};\
&ensp;&ensp;**procedure** listcode;\
&ensp;&ensp;&ensp;&ensp;**var** i: integer;\
&ensp;&ensp;**begin** {list code generated for this block}\
&ensp;&ensp;&ensp;&ensp;**for** i := cx0 **to** cx - 1 **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**with** code[i] **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;writeln(i, mnemonic[f]: 5, l: 3, a: 5)\
&ensp;&ensp;**end** {listcode};\
&ensp;&ensp;**procedure** statement(fsys: symset);\
&ensp;&ensp;&ensp;&ensp;**var** i, cx1, cx2: integer;\
&ensp;&ensp;&ensp;&ensp;**procedure** expression(fsys: symset);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**var** addop: symbol;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**procedure** term(fsys: symset);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**var** mulop: symbol;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**procedure** factor (fsys: symset);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**var** i: integer;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** test(facbegsys, fsys, 24);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** sym **in** facbegsys **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = ident **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** i := position(id);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** i = 0 **then** error(11) **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**with** table[i] **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**case** kind **of**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;constant: gen(lit, 0, val);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;variable: gen(lod, lev - level, adr);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;procedure: error(21)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;getsym\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = number **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** **if** num > amax **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** error(30); num := 0\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;gen(lit, 0, num); getsym\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = lparen **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** getsym; expression([rparen] + fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = rparen **then** getsym **else** error(22)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;test(fsys, [lparen], 23)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** {factor};\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** {term} factor(fsys + [times, slash]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** sym **in** [times, slash] **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** mulop := sym; getsym; factor(fsys + [times, slash]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** mulop = times **then** gen(opr, 0, 4) **else** gen(opr, 0, 5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** {term};\
&ensp;&ensp;&ensp;&ensp;**begin** {expression}\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym **in** [plus, minus] **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** addop := sym; getsym; term(fsys + [plus, minus]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** addop = minus **then** gen(opr, 0, 1)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** **else** term(fsys + [plus, minus]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** sym **in** [plus, minus] **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** addop := sym; getsym; term(fsys + [plus, minus]);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** addop = plus **then** gen(opr, 0, 2) **else** gen(opr, 0, 3)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;**end** {expression};\
&ensp;&ensp;&ensp;&ensp;**procedure** condition(fsys: symset);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**var** relop: symbol;\
&ensp;&ensp;&ensp;&ensp;**begin**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = oddsym **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** getsym; expression(fsys); gen(opr, 0, 6)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end** **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** expression([eql, neq, lss, gtr, leq, geq] + fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** &not;(sym **in** [eql, neq, lss, leq, gtr, geq]) **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;error(20) **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** relop := sym; getsym; expression(fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**case** relop **of**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;eql: gen(opr, 0, 8);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;neq: gen(opr, 0, 9);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;lss: gen(opr, 0, 10);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;geq: gen(opr, 0, 11);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;gtr: gen(opr, 0, 12);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;leq: gen(opr, 0, 13);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;**end** {condition};\
&ensp;&ensp;**begin** {statement}\
&ensp;&ensp;&ensp;&ensp;**if** sym = ident **then**\
&ensp;&ensp;&ensp;&ensp;**begin** i := position(id);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** i = 0 **then** error(11) **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** table[i].kind &ne; variable **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** {assignment to non-variable} error(12); i := 0\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;getsym; **if** sym = becomes **then** getsym **else** error(13);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;expression(fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** i &ne; 0 **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**with** table[i] **do** gen(sto, lev - level, adr)\
&ensp;&ensp;&ensp;&ensp;**end** **else**\
&ensp;&ensp;&ensp;&ensp;**if** sym = callsym **then**\
&ensp;&ensp;&ensp;&ensp;**begin** getsym;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym &ne; ident **then** error(14) **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** i := position(id);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** i = 0 **then** error(11) **else**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**with** table[i] **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** kind = procedure **then** gen(cal, lev - level, adr)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**else** error(15);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;getsym\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;**end** **else**\
&ensp;&ensp;&ensp;&ensp;**if** sym = ifsym **then**\
&ensp;&ensp;&ensp;&ensp;**begin** getsym; condition([thensym, dosym] + fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = thensym **then** getsym **else** error(16);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;cx1 := cx; gen(jpc, 0, 0);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;statement(fsys); code[cx1].a := cx\
&ensp;&ensp;&ensp;&ensp;**end** **else**\
&ensp;&ensp;&ensp;&ensp;**if** sym = beginsym **then**\
&ensp;&ensp;&ensp;&ensp;**begin** getsym; statement([semicolon, endsym] + fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** sym **in** [semicolon] + statbegsys **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = semicolon **then** getsym **else** error(10);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;statement([semicolon, endsym] + fsys)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = endsym **then** getsym **else** error(17)\
&ensp;&ensp;&ensp;&ensp;**end** **else**\
&ensp;&ensp;&ensp;&ensp;**if** sym = whilesym **then**\
&ensp;&ensp;&ensp;&ensp;**begin** cx1 := cx; getsym; condition([dosym] + fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;cx2 := cx; gen(jpc, 0, 0);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = dosym **then** getsym **else** error(18);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;statement(fsys); gen(jmp, 0, cx1); code[cx2].a := cx\
&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;test(fsys, [], 19)\
&ensp;&ensp;**end** {statement};\
**begin** {block} dx := 3; tx0 := tx; table[tx].adr := cx; gen(jmp, 0, 0);\
&ensp;&ensp;**if** lev > levmax **then** error(32);\
&ensp;&ensp;**repeat**\
&ensp;&ensp;&ensp;&ensp;**if** sym = constsym **then**\
&ensp;&ensp;&ensp;&ensp;**begin** getsym;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**repeat** constdeclaration;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** sym = comma **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** getsym; constdeclaration\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = semicolon **then** getsym **else** error(5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**until** sym &ne; ident\
&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;**if** sym = varsym **then**\
&ensp;&ensp;&ensp;&ensp;**begin** getsym;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**repeat** vardeclaration;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**while** sym = comma **do**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** getsym; vardeclaration\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = semicolon **then** getsym **else** error(5)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**until** sym &ne; ident;\
&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;**while** sym = procsym **do**\
&ensp;&ensp;&ensp;&ensp;**begin** getsym;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = ident **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** enter(procedure); getsym\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**else** error(4);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = semicolon **then** getsym **else** error(5);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;block(lev + 1, tx, [semicolon] + fsys);\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**if** sym = semicolon **then**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**begin** getsym; test(statbegsys + [ident, procsym], fsys, 6)\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**else** error(5)\
&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;test(statbegsys + [ident], declbegsys, 7)\
&ensp;&ensp;**until** &not;(sym **in** declbegsys);\
&ensp;&ensp;code[table[tx0].adr].a := cx;\
&ensp;&ensp;**with** table[tx0] **do**\
&ensp;&ensp;**begin** adr := cx; {start adr of code}\
&ensp;&ensp;**end**;\
&ensp;&ensp;cx0 := cx; gen(int, 0, dx);\
&ensp;&ensp;statement([semicolon, endsym] + fsys);\
&ensp;&ensp;gen(opr, 0, 0); {return}\
&ensp;&ensp;test(fsys, [], 8);\
&ensp;&ensp;listcode;\
**end** {block};\
**procedure** interpret;\
&ensp;&ensp;**const** stacksize = 500;\
&ensp;&ensp;**var** p, b, t: integer;&ensp;&ensp;{program-, base-, topstack-registers}\
&ensp;&ensp;&ensp;&ensp;i: instruction; {instruction register}\
&ensp;&ensp;&ensp;&ensp;s: **array** [1..stacksize] **of** integer;&ensp;&ensp;{datastore}\
&ensp;&ensp;**function** base(l: integer): integer;\
&ensp;&ensp;&ensp;&ensp;**var** b1: integer;\
&ensp;&ensp;**begin** b1 := b; {find base l levels down}\
&ensp;&ensp;&ensp;&ensp;**while** l > 0 **do**\
&ensp;&ensp;&ensp;&ensp;**begin** b1 := s[b1]; l := l - 1\
&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;base := b1\
&ensp;&ensp;**end** {base};\
**begin** writeln(' START PL/0');\
&ensp;&ensp;t := 0; b := 1; p := 0;\
&ensp;&ensp;s[1] := 0; s[2] := 0; s[3] := 0;\
&ensp;&ensp;**repeat** i := code[p]; p := p + 1;\
&ensp;&ensp;&ensp;&ensp;**with** i **do**\
&ensp;&ensp;&ensp;&ensp;**case** f **of**\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;lit: **begin** t := t + 1; s[t] := a\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;opr: **case** a **of**&ensp;&ensp;{operator}\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;0: **begin**&ensp;&ensp;{return}\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;t := b - 1; p := s[t + 3]; b := s[t + 2];\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;1: s[t] := -s[t];\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;2: **begin** t := t - 1; s[t] := s[t] + s[t + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;3: **begin** t := t - 1; s[t] := s[t] - s[t + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;4: **begin** t := t - 1; s[t] := s[t] \* s[t + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;5: **begin** t := t - 1; s[t] := s[t] div s[t + 1]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;6: s[t] := ord(odd(s[t]));\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;8: **begin** t := t - 1; s[t] := ord(s[t] = s[t + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;9: **begin** t := t - 1; s[t] := ord(s[t] &ne; s[t + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;10: **begin** t := t - 1; s[t] := ord(s[t] < s[t + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;11: **begin** t := t - 1; s[t] := ord(s[t] &ge; s[t + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;12: **begin** t := t - 1; s[t] := ord(s[t] > s[t + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;13: **begin** t := t - 1; s[t] := ord(s[t] &le; s[t + 1])\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;lod: **begin** t := t + 1; s[t] := s[base(l) + a]\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;sto: **begin** s[base(l) + a] := s[t]; writeln(s[t]); t := t - 1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;cal: **begin** {generate new block mark}\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;s[t + 1] := base(l); s[t + 2] := b; s[t + 3] := p;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;b := t + 1; p := a\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;int: t := t + a;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;jmp: p := a;\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;jpc: **begin** **if** s[t] = 0 **then** p := a; t := t - 1\
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;**end**\
&ensp;&ensp;&ensp;&ensp;**end** {with, case}\
&ensp;&ensp;**until** p = 0;\
&ensp;&ensp;write(' END PL/0');\
**end** {interpret};\
**begin** {main program}\
&ensp;&ensp;**for** ch := 'A' **to** ';' **do** ssym[ch] := nul;\
&ensp;&ensp;word[ 1] := 'BEGIN&ensp;&ensp;&ensp;&ensp;&ensp;';&ensp;&ensp;&ensp;&ensp;word[ 2] := 'CALL&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;';\
&ensp;&ensp;word[ 3] := 'CONST&ensp;&ensp;&ensp;&ensp;&ensp;';&ensp;&ensp;&ensp;&ensp;word[ 4] := 'DO&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;';\
&ensp;&ensp;word[ 5] := 'END&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;';&ensp;&ensp;&ensp;&ensp;word[ 6] := 'IF&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;';\
&ensp;&ensp;word[ 7] := 'ODD&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;';&ensp;&ensp;&ensp;&ensp;word[ 8] := 'PROCEDURE ';\
&ensp;&ensp;word[ 9] := 'THEN&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;';&ensp;&ensp;&ensp;&ensp;word[10] := 'VAR&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;';\
&ensp;&ensp;word[11] := 'WHILE&ensp;&ensp;&ensp;&ensp;&ensp;';\
&ensp;&ensp;wsym[ 1] := beginsym;&ensp;&ensp;&ensp;wsym[ 2] := callsym;\
&ensp;&ensp;wsym[ 3] := constsym;&ensp;&ensp;&ensp;wsym[ 4] := dosym;\
&ensp;&ensp;wsym[ 5] := endsym;&ensp;&ensp;&ensp;&ensp;&ensp;wsym[ 6] := ifsym;\
&ensp;&ensp;wsym[ 7] := oddsym;&ensp;&ensp;&ensp;&ensp;&ensp;wsym[ 8] := procsym;\
&ensp;&ensp;wsym[ 9] := thensym;&ensp;&ensp;&ensp;&ensp;wsym[10] := varsym;\
&ensp;&ensp;wsym[11] := whilesym;\
&ensp;&ensp;ssym['+'] := plus;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ssym['-'] := minus;\
&ensp;&ensp;ssym['\*'] := times;&ensp;&ensp;&ensp;&ensp;&ensp;ssym['/'] := slash;\
&ensp;&ensp;ssym['('] := lparen;&ensp;&ensp;&ensp;&ensp;ssym[')'] := rparen;\
&ensp;&ensp;ssym['='] := eql;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ssym[','] := comma;\
&ensp;&ensp;ssym['.'] := period;&ensp;&ensp;&ensp;&ensp;ssym['&ne;'] := neq;\
&ensp;&ensp;ssym['<'] := lss;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ssym['>'] := gtr;\
&ensp;&ensp;ssym['&le;'] := leq;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;ssym['&ge;'] := geq;\
&ensp;&ensp;ssym[';'] := semicolon;\
&ensp;&ensp;mnemonic[lit] := ' LIT '; mnemonic[opr] := ' OPR ';\
&ensp;&ensp;mnemonic[lod] := ' LOD '; mnemonic[sto] := ' STO ';\
&ensp;&ensp;mnemonic[cal] := ' CAL '; mnemonic[int] := ' INT ';\
&ensp;&ensp;mnemonic[jmp] := ' JMP '; mnemonic[jpc] := ' JPC ';\
&ensp;&ensp;declbegsys := [constsym, varsym, procsym];\
&ensp;&ensp;statbegsys := [beginsym, callsym, ifsym, whilesym];\
&ensp;&ensp;facbegsys := [ident, number, lparen];\
&ensp;&ensp;page(output); err := 0\
&ensp;&ensp;cc := 0; cx := 0; ll := 0; ch := ' '; kk := al; getsym;\
&ensp;&ensp;block(0, 0, [period] + declbegsys + statbegsys);\
&ensp;&ensp;**if** sym &ne; period **then** error(9);\
&ensp;&ensp;**if** err = 0 **then** interpret **else** write(' ERRORS IN PL/0 PROGRAM');\
99: writeln\
**end**.
