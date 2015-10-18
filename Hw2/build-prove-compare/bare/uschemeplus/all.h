/* {\Tt all.h} for \uschemeplus 220a */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <ctype.h>
/* shared type definitions 32a */
typedef struct Name *Name;
typedef struct Namelist *Namelist;   /* list of Name */
/* shared type definitions 34c */
typedef struct Reader *Reader;
/* shared type definitions 34f */
typedef struct XDefreader *XDefreader;
/* shared type definitions 35b */
typedef enum Prompts { NO_PROMPTS, STD_PROMPTS } Prompts;
/* shared type definitions 35e */
typedef enum Echo { SILENT, ECHOING } Echo;
/* shared type definitions 38a */
/* definition of [[va_list_box]] 39a */
typedef struct va_list_box {
  va_list ap;
} va_list_box;
typedef void Printer(FILE *output, va_list_box*);
/* shared type definitions 683d */
typedef struct Parlist *Parlist; /* list of Par */
/* shared type definitions (generated by a script) */
typedef struct Par *Par;
typedef enum { ATOM, LIST } Paralt; 
/* type definitions for \uschemeplus (generated by a script) */
typedef struct Lambda Lambda; 
typedef struct Value Value;
typedef enum { NIL, BOOL, NUM, SYM, PAIR, CLOSURE, PRIMITIVE } Valuealt;

/* type definitions for \uschemeplus (generated by a script) */
typedef struct Def *Def;
typedef enum { VAL, EXP, DEFINE } Defalt; 
typedef struct XDef *XDef;
typedef enum { DEF, USE, TEST } XDefalt; 
typedef struct UnitTest *UnitTest;
typedef enum { CHECK_EXPECT, CHECK_ERROR } UnitTestalt;

typedef struct Exp *Exp;
typedef enum {
    LITERAL, VAR, SET, IFX, WHILEX, BEGIN, LETX, LAMBDAX, APPLY, BREAKX,
    CONTINUEX, RETURNX, THROW, TRY_CATCH, HOLE, WHILE_RUNNING_BODY,
    CALLENV, LETXENV
} Expalt;

/* type definitions for \uschemeplus 220b */
typedef struct Stack *Stack;
typedef struct Frame Frame;
/* type definitions for \uscheme 128b */
typedef enum Letkeyword { LET, LETSTAR, LETREC } Letkeyword;
typedef struct UnitTestlist  *UnitTestlist;  /* list of UnitTest  */
typedef struct Explist  *Explist;            /* list of Exp  */
/* type definitions for \uscheme 128d */
typedef struct Valuelist *Valuelist;     /* list of Value */
typedef Value (Primitive)(Exp e, int tag, Valuelist vl);
/* type definitions for \uscheme 140a */
typedef struct Env *Env;
/* shared structure definitions 686e */
struct Reader {
    char *buf;               /* holds the last line read */
    int nbuf;                /* size of buf */
    int line;                /* current line number */
    const char *readername;  /* identifies this reader */

    FILE *fin;               /* filereader */
    const char *s;           /* stringreader */
};
/* shared structure definitions (generated by a script) */
struct Par { Paralt alt; union { Name atom; Parlist list; } u; }; 
/* structure definitions for \uschemeplus (generated by a script) */
struct Lambda { Namelist formals; Exp body; }; 
struct Value {
    Valuealt alt;
    union {
        int bool;
        int num;
        Name sym;
        struct { Value *car; Value *cdr; } pair;
        struct { Lambda lambda; Env env; } closure;
        struct { int tag; Primitive *function; } primitive;
    } u;
};

/* structure definitions for \uschemeplus (generated by a script) */
struct Def {
    Defalt alt;
    union {
        struct { Name name; Exp exp; } val;
        Exp exp;
        struct { Name name; Lambda lambda; } define;
    } u;
};

struct XDef {
    XDefalt alt; union { Def def; Name use; UnitTest test; } u;
};

struct UnitTest {
    UnitTestalt alt;
    union {
        struct { Exp check; Exp expect; } check_expect; Exp check_error;
    } u;
};

struct Exp {
    Expalt alt;
    union {
        Value literal;
        Name var;
        struct { Name name; Exp exp; } set;
        struct { Exp cond; Exp true; Exp false; } ifx;
        struct { Exp cond; Exp body; } whilex;
        Explist begin;
        struct { Letkeyword let; Namelist nl; Explist el; Exp body; } letx;
        Lambda lambdax;
        struct { Exp fn; Explist actuals; } apply;
        Exp returnx;
        Exp throw;
        struct { Exp body; Exp handler; } try_catch;
        Env callenv;
        Env letxenv;
    } u;
};

/* structure definitions for \uschemeplus (generated by a script) */
struct Explist {
	Exp hd;
	Explist tl;
};

struct UnitTestlist {
	UnitTest hd;
	UnitTestlist tl;
};

struct Parlist {
	Par hd;
	Parlist tl;
};

struct Valuelist {
	Value hd;
	Valuelist tl;
};

struct Namelist {
	Name hd;
	Namelist tl;
};

/* structure definitions for \uschemeplus 220c */
struct Frame {
    struct Exp context;     /* mutated in place during evaluation */
    Exp syntax;
                           /* when not NULL, kept pristine for error messages */
};
/* shared function prototypes 32b */
Name strtoname(const char *s);
const char *nametostr(Name n);
/* shared function prototypes 32c */
Printer printname;
/* shared function prototypes 34d */
char *readline(Reader r, const char *prompt);
/* shared function prototypes 34e */
Reader stringreader(const char *stringname, const char *s);
Reader filereader  (const char *filename,   FILE *fin);
/* shared function prototypes 34g */
XDef readxdef(XDefreader r);
/* shared function prototypes 35a */
XDefreader xdefreader(Reader r, Prompts prompts);
/* shared function prototypes 37f */
void print (const char *fmt, ...);  /* print to standard output */
void fprint(FILE *output, const char *fmt, ...);  /* print to given file */
/* shared function prototypes 38b */
void installprinter(unsigned char c, Printer *fmt);
/* shared function prototypes 38c */
Printer printpercent, printstring, printdecimal;
/* shared function prototypes 38d */
void vprint(FILE *output, const char *fmt, va_list_box *box);
/* shared function prototypes 39b */
void error(const char *fmt, ...);
extern jmp_buf errorjmp;        /* longjmp here on error */
/* shared function prototypes 39c */
typedef enum ErrorMode { NORMAL, TESTING } ErrorMode;
void set_error_mode(ErrorMode mode);
extern jmp_buf testjmp;
/* shared function prototypes 39d */
void checkargc(Exp e, int expected, int actual);
/* shared function prototypes 39e */
Name duplicatename(Namelist names);
/* shared function prototypes 49d */
void report_test_results(int npassed, int ntests);
/* shared function prototypes 683e */
Printer printpar;
/* shared function prototypes 683f */
Parlist readparlist(Reader r, int doquote, Prompts prompts);
/* shared function prototypes 689f */
extern int checkoverflow(int limit);
/* shared function prototypes (generated by a script) */
Par mkAtom(Name atom);
Par mkList(Parlist list);
struct Par mkAtomStruct(Name atom);
struct Par mkListStruct(Parlist list);
/* function prototypes for \uscheme 140b */
Value *find(Name name, Env env);
/* function prototypes for \uscheme 140c */
Env bindalloc    (Name name,   Value v,      Env env);
Env bindalloclist(Namelist nl, Valuelist vl, Env env);
/* function prototypes for \uscheme 140d */
Value *allocate(Value v);
/* function prototypes for \uscheme 140e */
void initallocate(void);
/* function prototypes for \uscheme 141a */
Value truev, falsev;
/* function prototypes for \uscheme 141b */
void initvalue(void);
/* function prototypes for \uscheme 141c */
int istrue(Value v);
/* function prototypes for \uscheme 141d */
Value unspecified(void);
/* function prototypes for \uscheme 141e */
Value eval   (Exp e, Env rho);
Env   evaldef(Def d, Env rho, Echo echo);
/* function prototypes for \uscheme 141f */
void readevalprint(XDefreader r, Env *envp, Echo echo);
/* function prototypes for \uscheme 141g */
Env primenv(void);
/* function prototypes for \uscheme 142a */
void printenv    (FILE *output, va_list_box*);
void printvalue  (FILE *output, va_list_box*);
void printclosure(FILE *output, va_list_box*);
void printexp    (FILE *output, va_list_box*);
void printdef    (FILE *output, va_list_box*);
void printlambda (FILE *output, va_list_box*);
/* function prototypes for \uscheme 712c */
int tests_passed(UnitTestlist tests, Env rho);
/* function prototypes for \uscheme 714e */
int equalpairs(Value v, Value w);
/* function prototypes for \uschemeplus (generated by a script) */
Lambda mkLambda(Namelist formals, Exp body);
Value mkNil(void);
Value mkBool(int bool);
Value mkNum(int num);
Value mkSym(Name sym);
Value mkPair(Value *car, Value *cdr);
Value mkClosure(Lambda lambda, Env env);
Value mkPrimitive(int tag, Primitive *function);
/* function prototypes for \uschemeplus (generated by a script) */
Def mkVal(Name name, Exp exp);
Def mkExp(Exp exp);
Def mkDefine(Name name, Lambda lambda);
struct Def mkValStruct(Name name, Exp exp);
struct Def mkExpStruct(Exp exp);
struct Def mkDefineStruct(Name name, Lambda lambda);
XDef mkDef(Def def);
XDef mkUse(Name use);
XDef mkTest(UnitTest test);
struct XDef mkDefStruct(Def def);
struct XDef mkUseStruct(Name use);
struct XDef mkTestStruct(UnitTest test);
UnitTest mkCheckExpect(Exp check, Exp expect);
UnitTest mkCheckError(Exp check_error);
struct UnitTest mkCheckExpectStruct(Exp check, Exp expect);
struct UnitTest mkCheckErrorStruct(Exp check_error);
Exp mkLiteral(Value literal);
Exp mkVar(Name var);
Exp mkSet(Name name, Exp exp);
Exp mkIfx(Exp cond, Exp true, Exp false);
Exp mkWhilex(Exp cond, Exp body);
Exp mkBegin(Explist begin);
Exp mkLetx(Letkeyword let, Namelist nl, Explist el, Exp body);
Exp mkLambdax(Lambda lambdax);
Exp mkApply(Exp fn, Explist actuals);
Exp mkBreakx(void);
Exp mkContinuex(void);
Exp mkReturnx(Exp returnx);
Exp mkThrow(Exp throw);
Exp mkTryCatch(Exp body, Exp handler);
Exp mkHole(void);
Exp mkWhileRunningBody(void);
Exp mkCallenv(Env callenv);
Exp mkLetxenv(Env letxenv);
struct Exp mkLiteralStruct(Value literal);
struct Exp mkVarStruct(Name var);
struct Exp mkSetStruct(Name name, Exp exp);
struct Exp mkIfxStruct(Exp cond, Exp true, Exp false);
struct Exp mkWhilexStruct(Exp cond, Exp body);
struct Exp mkBeginStruct(Explist begin);
struct Exp mkLetxStruct(Letkeyword let, Namelist nl, Explist el, Exp body);
struct Exp mkLambdaxStruct(Lambda lambdax);
struct Exp mkApplyStruct(Exp fn, Explist actuals);
struct Exp mkBreakxStruct(void);
struct Exp mkContinuexStruct(void);
struct Exp mkReturnxStruct(Exp returnx);
struct Exp mkThrowStruct(Exp throw);
struct Exp mkTryCatchStruct(Exp body, Exp handler);
struct Exp mkHoleStruct(void);
struct Exp mkWhileRunningBodyStruct(void);
struct Exp mkCallenvStruct(Env callenv);
struct Exp mkLetxenvStruct(Env letxenv);
/* function prototypes for \uschemeplus (generated by a script) */
int     lengthEL (Explist l);
Exp     nthEL    (Explist l, unsigned n);
Explist mkEL     (Exp hd, Explist tl);
Explist popEL     (Explist l);
Printer printexplist;
/* function prototypes for \uschemeplus (generated by a script) */
int          lengthUL (UnitTestlist l);
UnitTest     nthUL    (UnitTestlist l, unsigned n);
UnitTestlist mkUL     (UnitTest hd, UnitTestlist tl);
UnitTestlist popUL     (UnitTestlist l);
Printer      printunittestlist;
/* function prototypes for \uschemeplus (generated by a script) */
int     lengthPL (Parlist l);
Par     nthPL    (Parlist l, unsigned n);
Parlist mkPL     (Par hd, Parlist tl);
Parlist popPL     (Parlist l);
Printer printparlist;
/* function prototypes for \uschemeplus (generated by a script) */
int       lengthVL (Valuelist l);
Value     nthVL    (Valuelist l, unsigned n);
Valuelist mkVL     (Value hd, Valuelist tl);
Valuelist popVL     (Valuelist l);
Printer   printvaluelist;
/* function prototypes for \uschemeplus (generated by a script) */
int      lengthNL (Namelist l);
Name     nthNL    (Namelist l, unsigned n);
Namelist mkNL     (Name hd, Namelist tl);
Namelist popNL     (Namelist l);
Printer  printnamelist;
/* function prototypes for \uschemeplus 220d */
Stack  emptystack  (void);
Exp    pushcontext (struct Exp e, Stack s);
void   popframe    (Stack s);
void   clearstack  (Stack s);
/* function prototypes for \uschemeplus 220e */
Frame *topframe (Stack s);  /* NULL if empty */
/* function prototypes for \uschemeplus 221a */
void   pushenv_opt (Env env, Expalt context, Stack s);  /* may optimize */
/* function prototypes for \uschemeplus 221d */
void stack_trace_init(int *countp);  /* how many steps to show */
void stack_trace_current_expression(Exp e,   Env rho, Stack s);
void stack_trace_current_value     (Value v, Env rho, Stack s);
/* function prototypes for \uschemeplus 221e */
Value getoption(Name name, Env env, Value defaultval);
/* function prototypes for \uschemeplus 221f */
Value validate(Value v);
/* function prototypes for \uschemeplus 228a */
Exp transition_explist(Explist es, Value v); /* pointer to static memory */
/* function prototypes for \uschemeplus 228b */
Exp head_replaced_with_hole(Explist es);
                                     /* shares memory with transition_explist */
/* function prototypes for \uschemeplus 228c */
Explist copyEL(Explist el);
void    freeEL(Explist el);
/* function prototypes for \uschemeplus 229a */
Valuelist asLiterals(Explist es);
Value     asLiteral (Exp e);
/* function prototypes for \uschemeplus 229b */
void freeVL(Valuelist vl);
/* function prototypes for \uschemeplus 730a */
void printstack   (FILE *, va_list_box*);
void printoneframe(FILE *, va_list_box*);
void printframe   (FILE *, Frame *fr);
void printnoenv   (FILE *, va_list_box*);
/* global variables for \uschemeplus 221b */
extern int high_stack_mark;
/* global variables for \uschemeplus 221c */
extern int optimize_tail_calls;
extern int show_high_stack_mark;