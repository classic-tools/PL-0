/*
    pl0.c

    PL/0 compiler with code generation
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h> 

static jmp_buf environment;

static void throw(char *s) {
    fprintf(stdout, s);
    longjmp(environment, 1);
} /*throw*/

typedef int set;

static set cset(int n, ...) {
    int i;
    va_list ptr;
    set s = 0;

    va_start(ptr, n);
    for (i = 0; i < n; i++) {
        s |= (1 << (va_arg(ptr, int) - 1));
    }
    va_end(ptr);
    return s;
}

static int in(int e, set s) {
    return ((1 << (e - 1)) & s) != 0;
}

#define norw 11         /*no. of reserved words*/
#define txmax 100       /*length of identifier table*/
#define al  10          /*length of identifiers*/
#define cxmax 200       /*size of code array*/
#define stacksize 500

static const int nmax = 9;      /*max. no. of digits in numbers*/
static const int amax = 2047;   /*maximum address*/
static const int levmax = 3;    /*maximum depth of block nesting*/

typedef enum {nul, ident, number, plus, minus, times, slash, oddsym,
    eql, neq, lss, leq, gtr, geq, lparen, rparen, comma, semicolon,
    period, becomes, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym,
    readsym, writesym} symbol;
typedef char alfa[al + 2];
typedef enum {constant, variable, prozedure} object;
typedef set symset;
typedef enum {lit, opr, lod, sto, cal, int0, jmp, jpc} fct;  /*functions*/
typedef struct  {
                  fct f;    /*function code*/
                  int l;    /*level: 0..levmax*/
                  int a;    /*displacement address: 0..amax*/
                } instruction;
typedef int datastore[stacksize + 1];

typedef char alfa1[5 + 2];
typedef union   {
                    int val;
                    struct {
                        int level, adr;
                    } s;
                } variant;
typedef struct  {
                    alfa name;
                    object kind;
                    variant v;
                } entry;

/*
  LIT 0, a  :  load constant a
  OPR 0, a  :  execute operation a
  LOD l, a  :  load variable l, a
  STO l, a  :  store variable l, a
  CAL l, a  :  call void a at level l
  INT 0, a  :  increment t-register by a
  JMP 0, a  :  jump to a
  JPC 0, a  :  jump conditional to a
*/

static char ch;         /*last character read*/
static symbol sym;      /*last symbol read*/
static alfa id;         /*last identifier read*/
static int num;         /*last number read*/
static int cc;          /*character count*/
static int ll;          /*line length*/
static int  kk, err;
static int  cx;         /*code allocation index*/
static char line[81 + 2];
static alfa a;
static instruction code[cxmax + 1];
static alfa word[norw + 1];
static symbol wsym[norw + 1];
static symbol ssym[127 + 1];
static alfa1 mnemonic[7 + 1];
static symset declbegsys, statbegsys, facbegsys;
static entry table[txmax + 1];
static int outflag;

static void error(int n) {
    int i;

    fprintf(stdout, " ****");
    for (i = 0; i < cc; i++) {
        fprintf(stdout, " ");
    }
    fprintf(stdout, "^%2d\n", n); err = err + 1;
} /*error*/

static void getch(void) {
    int i;

    if (cc == ll) {
        ch = fgetc(stdin);
        if (feof(stdin)) {
            throw(" PROGRAM INCOMPLETE");
        }
        ll = 0;
        cc = 0;
        fprintf(stdout, "%5d ", cx);
        while (ch != '\n')
        {
            ll = ll + 1;
            line[ll] = ch;
            fprintf(stdout, "%c", ch);
            ch = fgetc(stdin);
        }
        fprintf(stdout, "\n");
        ll = ll + 1;
        line[ll] = ' ';
      }
      cc = cc + 1;
      ch = line[cc];
} /*getch*/

static void comment(void)
{ 
    getch();
    do {
        while (ch != '*') {
            getch();
        }
        getch();
    } while (ch != ')');
    getch();
} /*comment*/

static char upcase(char ch)
{
    return ch >= 'a' && ch <='z' ? ch - 'a' + 'A' : ch; 
} /*upcase*/

static void getsym(void)
{
    int i, j, k;

    while (ch ==' ' || ch == '\t') {
        getch();
    }
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
        /*identifier or reserved word*/
        k = 0; a[0] = ' '; a[11] = '\0';
        do {
            if (k < al) {
                k = k + 1;
                a[k] = upcase(ch);
            };
            getch();
        } while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'));
        if (k >= kk) {
            kk = k;
        } else {
            do {
                a[kk] = ' ';
                kk = kk - 1;
            } while (kk != k);
        }
        strcpy(id, a);
        i = 1;
        j = norw;
        do {
            k = (i + j) / 2;
            if (strcmp(id, word[k]) <= 0) {
                j = k - 1;
            }
            if (strcmp(id, word[k]) >= 0) {
                i = k + 1;
            }
        } while (i <= j);
        if (i - 1 > j) {
            sym = wsym[k];
        } else {
            sym = ident;
        }
    } else if (ch >= '0' && ch <= '9') {
        /*number*/
        k = 0;
        num = 0;
        sym = number;
        do {
            num = 10 * num + (ch - '0');
            k = k + 1;
            getch();
        } while (ch >= '0' && ch <= '9');
        if (k > nmax) {
            error(30);
        }
    } else if (ch == ':') {
        getch();
        if (ch == '=') {
            sym = becomes;
            getch();
        } else {
            sym = nul;
        }
    } else if (ch == '<') {
        getch();
        if (ch == '=') {
            sym = leq;
            getch();
        } else {
            sym = lss;
        }
    } else if (ch == '>') {
        getch();
        if (ch == '=') {
            sym = geq;
            getch();
        } else {
            sym = gtr;
        }
    } else if (ch == '(') {
        getch();
        if (ch == '*') {
            comment();
            getsym();
        } else {
            sym = lparen;
        }
    } else {
        sym = ssym[ch];
        getch();
    }
} /*getsym*/;

static void gen(fct x, int y, int z)
{
    if (cx > cxmax) {
        throw(" PROGRAM TOO LONG");
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx = cx + 1;
} /*gen*/;

static void test(symset s1, symset s2, int n) {
    if (!in(sym, s1)) {
        error(n);
        s1 = s1 | s2;
        while (!in(sym, s1)) { 
            getsym();
        }
    }
} /*test*/

static void enter(object k, int lev, int *tx, int *dx) {
    /*enter object into table*/
    *tx = *tx + 1;
    strcpy(table[*tx].name, id);
    table[*tx].kind = k;
    switch (k) {
        case constant: 
            if (num > amax){
                error(30);
                num = 0;
            };
            table[*tx].v.val = num;
            break;
        case variable:
            table[*tx].v.s.level = lev;
            table[*tx].v.s.adr = *dx;
            *dx = *dx + 1;
            break;
        case prozedure:
            table[*tx].v.s.level = lev;
            break;
    }  
} /*enter*/

static int position(alfa id, int tx) {
    /*find identifier id in table*/
    int i;

    strcpy(table[0].name, id);
    i = tx;
    while (strcmp(table[i].name, id) != 0) {
        i = i - 1;
    }
    return i;
} /*position*/

static void constdeclaration(int lev, int *tx, int *dx) {
    if (sym == ident) {
        getsym();
        if (sym == eql || sym == becomes) {
            if (sym == becomes) {
                error(1);
            }
            getsym();
            if (sym == number) {
                enter(constant, lev, tx, dx);
                getsym();
            } else {
                error(2);
            }
        } else {
            error(3);
        }
    } else {
        error(4);
    }
} /*constdeclaration*/

static void vardeclaration(int lev, int *tx, int *dx) {
    if (sym == ident) {
        enter(variable, lev, tx, dx);
        getsym();
    } else {
        error(4);
    }
} /*vardeclaration*/

static void listcode(int cx0) {
    /*list code generated for this block*/
    int i;

    for (i = cx0; i < cx; i++) {
        fprintf(stdout, "%11d%5s%3d%5d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
    }
} /*listcode*/

static void expression(symset, int, int);

static void factor (symset fsys, int lev, int tx) {
    int i;

    test(facbegsys, fsys, 24);
    while (in(sym, facbegsys)) {
        if (sym == ident) {
            i = position(id, tx);
            if (i == 0) {
                error(11);
            } else {
                switch (table[i].kind) {
                    case constant:
                        gen(lit, 0, table[i].v.val);
                        break;
                    case variable:
                        gen(lod, lev - table[i].v.s.level, table[i].v.s.adr);
                        break;
                    case prozedure:
                        error(21);
                        break;
                }
            }
            getsym();
        } else if (sym == number) {
            if (num > amax) {
                error(30);
                num = 0;
            };
            gen(lit, 0, num);
            getsym();
        } else if (sym == lparen) {
            getsym();
            expression(cset(1, rparen) | fsys, lev, tx);
            if (sym == rparen) {
                getsym();
            } else {
                error(22);
            }
        };
        test(fsys, cset(1, lparen), 23);
    }
} /*factor*/

static void term(symset fsys, int lev, int tx) {
    symbol mulop;

    factor(fsys | cset(2, times, slash), lev, tx);
    while (sym == times || sym == slash) {
        mulop = sym;
        getsym();
        factor(fsys | cset(times, slash), lev, tx);
        if (mulop == times) {
            gen(opr, 0, 4);
        } else {
            gen(opr, 0, 5);
        }
  }
} /*term*/

static void expression(symset fsys, int lev, int tx) {
    symbol addop;
    
    if (sym == plus || sym == minus) {
        addop = sym;
        getsym();
        term(fsys | cset(2, plus, minus), lev, tx);
        if (addop == minus) {
            gen(opr, 0, 1);
        }
    } else {
        term(fsys | cset(2, plus, minus), lev, tx);
    }
    while (sym == plus || sym == minus) {
        addop = sym;
        getsym();
        term(fsys | cset(2, plus, minus), lev, tx);
        if (addop == plus) {
            gen(opr, 0, 2);
        } else {
            gen(opr, 0, 3);
        }
  }
} /*expression*/

static void condition(symset fsys, int lev, int tx) {
    symbol relop;

    if (sym == oddsym) {
        getsym();
        expression(fsys, lev, tx);
        gen(opr, 0, 6);
    } else {
        expression(cset(6, eql, neq, lss, gtr, leq, geq) | fsys, lev, tx);
        if (!in(sym, cset(6, eql, neq, lss, leq, gtr, geq))) {
            error(20);
        } else {
            relop = sym;
            getsym();
            expression(fsys, lev, tx);
            switch (relop) {
                case eql:
                    gen(opr, 0, 8);
                    break;
                case neq:
                    gen(opr, 0, 9);
                    break;
                case lss:
                    gen(opr, 0, 10);
                    break;
                case geq:
                    gen(opr, 0, 11);
                    break;
                case gtr:
                    gen(opr, 0, 12);
                    break;
                case leq:
                    gen(opr, 0, 13);
                    break;
            }
        }
    }
} /*condition*/

static void statement(symset fsys, int lev, int tx) {
    int i, cx1, cx2;

    if (sym == ident) {
        i = position(id, tx);
        if (i == 0) {
            error(11);
        } else if (table[i].kind != variable) {
            /*assignment to non-variable*/
            error(12);
            i = 0;
        }
        getsym();
        if (sym == becomes) {
            getsym();
        } else {
            error(13);
        }
        expression(fsys, lev, tx);
        if (i != 0) {
            gen(sto, lev - table[i].v.s.level, table[i].v.s.adr);
        }
    } else if (sym == callsym) {
        getsym();
        if (sym != ident) {
            error(14);
        } else {
            i = position(id, tx);
            if (i == 0) {
                error(11);
            } else if (table[i].kind == prozedure) {
                gen(cal, lev - table[i].v.s.level, table[i].v.s.adr);
            } else {
                error(15);
            }
            getsym();
        }
    } else if (sym == ifsym) {
        getsym();
        condition(cset(2, thensym, dosym) | fsys, lev, tx);
        if (sym == thensym) {
            getsym();
        } else {
            error(16);
        }
        cx1 = cx;
        gen(jpc, 0, 0);
        statement(fsys, lev, tx);
        code[cx1].a = cx;
    } else if (sym == beginsym) {
        getsym();
        statement(cset(2, semicolon, beginsym) | fsys, lev, tx);
        while (in(sym, cset(1, semicolon) | statbegsys)) {
            if (sym == semicolon) {
                getsym();
            } else {
                error(10);
            }
            statement(cset(2, semicolon, endsym) | fsys, lev, tx);
        }
        if (sym == endsym) {
            getsym();
        } else {
            error(17);
        }
    } else if (sym == whilesym) {
        cx1 = cx;
        getsym();
        condition(cset(1, dosym) | fsys, lev, tx);
        cx2 = cx;
        gen(jpc, 0, 0);
        if (sym =dosym) {
            getsym();
        } else {
            error(18);
        }
        statement(fsys, lev, tx);
        gen(jmp, 0, cx1);
        code[cx2].a = cx;
    } else if (sym == readsym) {
        getsym();
        if (sym != ident) {
            error(14);
        } else {
            i = position(id, tx);
            if (i == 0) {
                error(11);
            } else if (table[i].kind != variable) {
                /*read to non-variable*/
                error(12);
                i = 0;
            };
            expression(fsys, lev, tx);
            if (i != 0) {
                gen(opr, 0, 14);
                gen(sto, lev - table[i].v.s.level, table[i].v.s.adr);
            }
        }
    } else if (sym == writesym) {
        getsym();
        expression(fsys, lev, tx);
        gen(opr, 0, 15);
        outflag = outflag + 1;
    }
    test(fsys, cset(0), 19);
} /*statement*/

static void block(int lev, int tx, symset fsys) {
    int dx;     /*data allocation index*/
    int tx0;    /*initial table index*/
    int cx0;    /*initial code index*/

    dx = 3;
    tx0 = tx;
    table[tx].v.s.adr = cx;
    gen(jmp, 0, 0);
    if (lev > levmax) {
        error(32);
    }
    do {
        if (sym == constsym) {
            getsym();
            do {
                constdeclaration(lev, &tx, &dx);
                while (sym == comma) {
                    getsym();
                    constdeclaration(lev, &tx, &dx);
                }
                if (sym == semicolon) {
                    getsym();
                } else {
                    error(5);
                }
            } while (sym == ident);
        }
        if (sym == varsym) {
            getsym();
            do {
                vardeclaration(lev, &tx, &dx);
                while (sym == comma) {
                    getsym();
                    vardeclaration(lev, &tx, &dx);
                }
                if (sym == semicolon) {
                    getsym();
                } else {
                    error(5);
                }
            } while (sym == ident);
        }
        while (sym == procsym) {
            getsym();
            if (sym == ident) {
                enter(prozedure, lev, &tx, &dx);
                getsym();
            } else {
                error(4);
            }
            if (sym == semicolon) {
                getsym();
            } else {
                error(5);
            }
            block(lev + 1, tx, cset(1, semicolon) | fsys);
            if (sym == semicolon)
            {
                getsym();
                test(statbegsys | cset(2, ident, procsym), fsys, 6);
            } else {
                error(5);
            }
        };
        test(statbegsys | cset(1, ident), declbegsys, 7);
    } while (in(sym, declbegsys));
    code[table[tx0].v.s.adr].a = cx;
    table[tx0].v.s.adr = cx; /*start adr of code*/
    cx0 = cx;
    gen(int0, 0, dx);
    statement(cset(2, semicolon, endsym) | fsys, lev, tx);
    gen(opr, 0, 0); /*return*/
    test(fsys, cset(0), 8);
    listcode(cx0);
} /*block*/

static int base(int l, int b, datastore s)
{
    int b1;
    
    b1 = b; /*find base l levels down*/
    while (l > 0) {
        b1 = s[b1];
        l = l - 1;
    }
    return b1;
} /*base*/

static void interpreter(void)
{
    int p, b, t;    /*program-, base-, topstack-registers*/
    instruction i;  /*instruction register*/
    datastore s;
    
    fprintf(stdout, " START PL/0\n");
    t = 0;
    b = 1;
    p = 0;
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;
    do {
        i = code[p];
        p = p + 1;
        switch (i.f) {
            case lit:
                t = t + 1;
                s[t] = i.a;
                break;
            case opr:
                switch (i.a) {  /*operator*/
                    case 0:  /*return*/
                        t = b - 1;
                        p = s[t + 3];
                        b = s[t + 2];
                        break;
                    case 1:
                        s[t] = -s[t];
                        break;
                    case 2:
                        t = t - 1;
                        s[t] = s[t] + s[t + 1];
                        break;
                    case 3:
                        t = t - 1;
                        s[t] = s[t] - s[t + 1];
                        break;
                    case 4:
                        t = t - 1;
                        s[t] = s[t] * s[t + 1];
                        break;
                    case 5:
                        t = t - 1;
                        s[t] = s[t] / s[t + 1];
                        break;
                    case 6:
                        s[t] = s[t] % 2 == 1;
                        break;
                    case 8:
                        t = t - 1;
                        s[t] = s[t] == s[t + 1];
                        break;
                    case 9:
                        t = t - 1;
                        s[t] = s[t] != s[t + 1];
                        break;
                    case 10:
                        t = t - 1;
                        s[t] = s[t] < s[t + 1];
                        break;
                    case 11:
                        t = t - 1;
                        s[t] = s[t] >= s[t + 1];
                        break;
                    case 12:
                        t = t - 1;
                        s[t] = s[t] > s[t + 1];
                        break;
                    case 13:
                        t = t - 1;
                        s[t] = s[t] <= s[t + 1];
                        break;
                    case 14:
                        t = t + 1;
                        fscanf(stdin, "%d", &(s[t]));
                        break;
                    case 15:
                        fprintf(stdout, "%d\n", s[t]);
                        t = t - 1;
                        break;
                };
                break;
            case lod:
                t = t + 1; s[t] = s[base(i.l, b, s) + i.a];
                break;
            case sto:
                s[base(i.l, b, s) + i.a] = s[t];
                if (outflag == 0) {
                    fprintf(stdout, "%11d\n", s[t]);
                }
                t = t - 1;
                break;
            case cal: /*generate new block mark*/
                s[t + 1] = base(i.l, b, s);
                s[t + 2] = b;
                s[t + 3] = p;
                b = t + 1;
                p = i.a;
                break;
            case int0:
                t = t + i.a;
                break;
            case jmp:
                p = i.a;
                break;
            case jpc:
                if (s[t] == 0) {
                    p = i.a;
                }
                t = t - 1;
                break;
        }; /*switch*/
    } while (p != 0);
    fprintf(stdout, " END PL/0");
} /*interpreter*/

int main(int argc, char*argv[])
{
    int exception = setjmp(environment);  /* Setup jump position using environment */

    if (!exception) {
        int i;

        for (i = 0; i < 128; i++) {
            ssym[i] = nul;
        }
        strcpy(word[ 1], " BEGIN     ");
        strcpy(word[ 2], " CALL      ");
        strcpy(word[ 3], " CONST     ");
        strcpy(word[ 4], " DO        ");
        strcpy(word[ 5], " END       ");
        strcpy(word[ 6], " IF        ");
        strcpy(word[ 7], " ODD       ");
        strcpy(word[ 8], " PROCEDURE ");
        strcpy(word[ 9], " THEN      ");
        strcpy(word[10], " VAR       ");
        strcpy(word[11], " WHILE     ");
        wsym[ 1] = beginsym;
        wsym[ 2] = callsym;
        wsym[ 3] = constsym;
        wsym[ 4] = dosym;
        wsym[ 5] = endsym;
        wsym[ 6] = ifsym;
        wsym[ 7] = oddsym;
        wsym[ 8] = procsym;
        wsym[ 9] = thensym;
        wsym[10] = varsym;
        wsym[11] = whilesym;
        ssym['?'] = readsym;
        ssym['!'] = writesym;
        ssym['+'] = plus;
        ssym['-'] = minus;
        ssym['*'] = times;
        ssym['/'] = slash;
        ssym['='] = eql;
        ssym[','] = comma;
        ssym['.'] = period;
        ssym['#'] = neq;
        ssym[';'] = semicolon;
        ssym[')'] = rparen;
        strcpy(mnemonic[lit], "  LIT "); strcpy(mnemonic[opr], "  OPR ");
        strcpy(mnemonic[lod], "  LOD "); strcpy(mnemonic[sto], "  STO ");
        strcpy(mnemonic[cal], "  CAL "); strcpy(mnemonic[int0], "  INT ");
        strcpy(mnemonic[jmp], "  JMP "); strcpy(mnemonic[jpc], "  JPC ");
        declbegsys = cset(3, constsym, varsym, procsym);
        statbegsys = cset(4, beginsym, callsym, ifsym, whilesym);
        facbegsys = cset(3, ident, number, lparen);
        err = 0;
        outflag = 0;
        cc = 0;
        cx = 0;
        ll = 0;
        ch = ' ';
        line[0] = ' ';
        kk = al;
        getsym();
        block(0, 0, cset(1, period) | declbegsys | statbegsys);
        if (sym != period) {
            error(9);
        }
        if (err == 0) {
            interpreter();
        } else {
            fprintf(stdout, " ERRORS IN PL/0 PROGRAM");
        }
    }
    fprintf(stdout, "\n");
    return EXIT_SUCCESS;
}  
