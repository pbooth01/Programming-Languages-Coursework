/*
 * The [[main]] function becomes initialization followed
 * by a loop. The loop runs until [[roots.sources]] is
 * empty. [[roots.stack]] is initialized in [[eval]].
 * <scheme-stack.c>=
 */
#include "all.h"

int main(int argc, char *argv[]) {
    Env env = NULL;
    int doprompt = (argc <= 1) || (strcmp(argv[1], "-q") != 0);

    initvalue();
    initallocate();
    /*
     * We have many printers.
     * <install printers>=
     */
    installprinter('c', printclosure);
    installprinter('d', printdecimal);
    installprinter('e', printexp);
    installprinter('E', printexplist);
    installprinter('\\', printlambda);
    installprinter('n', printname);
    installprinter('N', printnamelist);
    installprinter('p', printpar);
    installprinter('P', printparlist);
    installprinter('r', printenv);
    installprinter('s', printstring);
    installprinter('t', printdef);
    installprinter('v', printvalue);
    installprinter('V', printvaluelist);
    installprinter('%', printpercent);
    /*
     * <install printers>=
     */
    installprinter('S', printstack);
    installprinter('F', printoneframe);
    installprinter('R', printnoenv);
    roots.globals = &env;   
    roots.stack   = emptystack();
    addprimitives(&env);
    /*
     * Standard input should be read after the initial
     * basis, but because reading is done on a stack, we
     * push it on before the basis.
     * <initialize [[roots.sources]] to read first the initial basis, then
                                                                     [[stdin]]>=
     */
    roots.sources =
      mkSL(mkSource(xdefreader(filereader("standard input", stdin), doprompt), 
                    stdin, ECHOING),
           NULL);
    {   /*
         * <C representation of initial basis for {\uscheme}>=
         */
        const char *basis=
          "(define caar (xs) (car (car xs)))\n"
          "(define cadr (xs) (car (cdr xs)))\n"
          "(define cdar (xs) (cdr (car xs)))\n"
          "(define list1 (x)     (cons x '()))\n"
          "(define list2 (x y)   (cons x (list1 y)))\n"
          "(define list3 (x y z) (cons x (list2 y z)))\n"
          "(define length (xs)\n"
          "  (if (null? xs) 0\n"
          "    (+ 1 (length (cdr xs)))))\n"
          "(define and (b c) (if b  c  b))\n"
          "(define or  (b c) (if b  b  c))\n"
          "(define not (b)   (if b #f #t))\n"

"(define atom? (x) (or (number? x) (or (symbol? x) (or (boolean? x) (null? x)))))\n"
          "(define equal? (s1 s2)\n"
          "  (if (or (atom? s1) (atom? s2))\n"
          "    (= s1 s2)\n"
          "    (and (equal? (car s1) (car s2)) (equal? (cdr s1) (cdr s2)))))\n"
          "(define append (xs ys)\n"
          "  (if (null? xs)\n"
          "     ys\n"
          "     (cons (car xs) (append (cdr xs) ys))))\n"
          "(define revapp (xs ys)\n"
          "  (if (null? xs)\n"
          "     ys\n"
          "     (revapp (cdr xs) (cons (car xs) ys))))\n"
          "(define reverse (xs) (revapp xs '()))\n"
          "(define mk-alist-pair (k a) (list2 k a))\n"
          "(define alist-pair-key        (pair)  (car  pair))\n"
          "(define alist-pair-attribute  (pair)  (cadr pair))\n"

   "(define alist-first-key       (alist) (alist-pair-key       (car alist)))\n"

   "(define alist-first-attribute (alist) (alist-pair-attribute (car alist)))\n"
          "(define bind (k a alist)\n"
          "  (if (null? alist)\n"
          "    (list1 (mk-alist-pair k a))\n"
          "    (if (equal? k (alist-first-key alist))\n"
          "      (cons (mk-alist-pair k a) (cdr alist))\n"
          "      (cons (car alist) (bind k a (cdr alist))))))\n"
          "(define find (k alist)\n"
          "  (if (null? alist) '()\n"
          "    (if (equal? k (alist-first-key alist))\n"
          "      (alist-first-attribute alist)\n"
          "      (find k (cdr alist)))))\n"
          "(define o (f g) (lambda (x) (f (g x))))\n"
          "(define curry   (f) (lambda (x) (lambda (y) (f x y))))\n"
          "(define uncurry (f) (lambda (x y) ((f x) y)))\n"
          "(define filter (p? xs)\n"
          "  (if (null? xs)\n"
          "    '()\n"
          "    (if (p? (car xs))\n"
          "      (cons (car xs) (filter p? (cdr xs)))\n"
          "      (filter p? (cdr xs)))))\n"
          "(define map (f xs)\n"
          "  (if (null? xs)\n"
          "    '()\n"
          "    (cons (f (car xs)) (map f (cdr xs)))))\n"
          "(define exists? (p? xs)\n"
          "  (if (null? xs)\n"
          "    #f\n"
          "    (if (p? (car xs)) \n"
          "      #t\n"
          "      (exists? p? (cdr xs)))))\n"
          "(define all? (p? xs)\n"
          "  (if (null? xs)\n"
          "    #t\n"
          "    (if (p? (car xs))\n"
          "      (all? p? (cdr xs))\n"
          "      #f)))\n"
          "(define foldr (op zero xs)\n"
          "  (if (null? xs)\n"
          "    zero\n"
          "    (op (car xs) (foldr op zero (cdr xs)))))\n"
          "(define foldl (op zero xs)\n"
          "  (if (null? xs)\n"
          "    zero\n"
          "    (foldl op (op (car xs) zero) (cdr xs))))\n"
          "(define <= (x y) (not (> x y)))\n"
          "(define >= (x y) (not (< x y)))\n"
          "(define != (x y) (not (= x y)))\n"
          "(define max (x y) (if (> x y) x y))\n"
          "(define min (x y) (if (< x y) x y))\n"
          "(define mod (m n) (- m (* n (/ m n))))\n"
          "(define gcd (m n) (if (= n 0) m (gcd n (mod m n))))\n"
          "(define lcm (m n) (if (= m 0) 0 (* m (/ n (gcd m n)))))\n"
          "(define caar  (sx) (car (car  sx)))\n"
          "(define cdar  (sx) (cdr (car  sx)))\n"
          "(define cadr  (sx) (car (cdr  sx)))\n"
          "(define cddr  (sx) (cdr (cdr  sx)))\n"
          "(define caaar (sx) (car (caar sx)))\n"
          "(define cdaar (sx) (cdr (caar sx)))\n"
          "(define caadr (sx) (car (cadr sx)))\n"
          "(define cdadr (sx) (cdr (cadr sx)))\n"
          "(define cadar (sx) (car (cdar sx)))\n"
          "(define cddar (sx) (cdr (cdar sx)))\n"
          "(define caddr (sx) (car (cddr sx)))\n"
          "(define cdddr (sx) (cdr (cddr sx)))\n"
          "(define list1 (x)               (cons x '()))\n"
          "(define list2 (x y)             (cons x (list1 y)))\n"
          "(define list3 (x y z)           (cons x (list2 y z)))\n"
          "(define list4 (x y z a)         (cons x (list3 y z a)))\n"
          "(define list5 (x y z a b)       (cons x (list4 y z a b)))\n"
          "(define list6 (x y z a b c)     (cons x (list5 y z a b c)))\n"
          "(define list7 (x y z a b c d)   (cons x (list6 y z a b c d)))\n"
          "(define list8 (x y z a b c d e) (cons x (list7 y z a b c d e)))\n";
        roots.sources =
           mkSL(mkSource(xdefreader(stringreader("initial basis", basis), 0),
                                                                  NULL, SILENT),
                roots.sources);
    }

    /*
     * The loop looks a bit like the body of the old
     * [[readevalprint]]. This version is more resilient to
     * errors that occur when a file is read.
     * <read definitions until [[roots.sources]] is exhausted>=
     */
    while (roots.sources != NULL) {
        XDef d;
        Source *cursource = &roots.sources->hd;
        Source newsource;   /* initialized when we hit USE */

        while (setjmp(errorjmp))
            ;
        d = readxdef(cursource->xdefs);
        if (d == NULL) {
            if (cursource->sourcefile != NULL)
                fclose(cursource->sourcefile);
            /*
             * <using error mode [[TESTING]], run unit tests from
                                         [[cursource->tests]], last test first>=
             */
            set_error_mode(TESTING);
            {   int npassed = tests_passed(cursource->tests, *roots.globals);
                int ntests  = lengthUL(cursource->tests);
                report_test_results(npassed, ntests);
            }
            set_error_mode(NORMAL);
            roots.sources = popSL(roots.sources);
        } else switch (d->alt) {
            case USE:
                /*
                 * File [[fin]] is closed above, after [[readxdef
                 * (cursource->xdefs)]] returns [[NULL]].
                 * <set [[newsource]] to a definition reader for [[d->u.use]]>=
                 */
                {
                    const char *filename = nametostr(d->u.use);
                    FILE *fin = fopen(filename, "r");

                    if (fin == NULL)
                        error("cannot open file \"%s\"", filename);
                    newsource = mkSource(xdefreader(filereader(filename, fin), 0
                                                               ), fin, ECHOING);
                }
                roots.sources = mkSL(newsource, roots.sources);
                break;
            case TEST:
                cursource->tests = mkUL(d->u.test, cursource->tests);
                break;
            case DEF:
                env = evaldef(d->u.def, env, cursource->echo);
                break;
            default:
                assert(0);
        }
    }
    return 0;
}
/*
 * Finally, here's how to make a [[Source]].
 * <scheme-stack.c>=
 */
struct Source mkSource(XDefreader xdefs, FILE *f, Echo echo) {
    Source s;
    s.xdefs      = xdefs;
    s.sourcefile = f;
    s.echo       = echo;
    s.tests      = NULL;
    return s;
}
