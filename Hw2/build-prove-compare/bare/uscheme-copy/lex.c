/* lex.c 690b */
#include "all.h"
#include <ctype.h>
/* lex.c 690c */
static Name strntoname(char *s, int n) {
    char *t = malloc(n + 1);
    assert(t != NULL);
    strncpy(t, s, n);
    t[n] = '\0';
    return strtoname(t);
}
/* lex.c 690d */
static int isdelim(char c, int quote) {
    if (quote && c == '\'')
        return 1;

    return c == '\0' || c == '(' || c == ')' || c == ';' || isspace(c);
}
/* lex.c 691a */
static Name readname(char **ps, int quote) {
    char *p, *q;

    p = *ps;
    for (q = p; !isdelim(*q, quote); q++)
        ;
    *ps = q;
    return strntoname(p, q - p);
}
/* lex.c 691b */
static void skipspace(char **ps) {
    while (isspace((unsigned char)**ps))
        (*ps)++;
}
/* lex.c 691c */
enum {
    More       = 1 << 0,
    Quote      = 1 << 1,
    Rightparen = 1 << 2
};
static Par readpar(char **ps, Reader r, int flag, const char *moreprompt) {
    if (*ps == NULL)
        return NULL;

    skipspace(ps);

    switch (**ps) {
    case '\0':
    case ';':
        /* readpar end of line and return 692a */
        if ((flag & More) && (*ps = readline(r, moreprompt)) != NULL)
            return readpar(ps, r, flag, moreprompt);
        *ps = NULL;
        return NULL;
    case ')':
        /* readpar right parenthesis and return 692b */
        if ((flag & Rightparen) == 0)
            error("unexpected right parenthesis in %s, line %d", r->readername,
                                                                       r->line);
        return NULL;
    case '(':
        /* readpar left parenthesis and return 692c */
        {
            Par p = mkList(NULL);
            Parlist *ppl =&p->u.list;
            Par q;  /* next par read in, to be accumulated into p */

            (*ps)++;
            while ((q = readpar(ps, r, flag | More | Rightparen, moreprompt))) {
                *ppl = mkPL(q, NULL);
                ppl  = &(*ppl)->tl;
            }
            if (*ps == NULL)
                error("premature end of file reading list (missing right paren)"
                                                                              );

            assert(**ps == ')');
            (*ps)++;    /* past right parenthesis */

            return p;
        }
    default:
        if ((flag & Quote) && **ps == '\'') {
            /* readpar quoted expression and return 692d */
            {
                Par p;

                (*ps)++;
                p = readpar(ps, r, More|Quote, moreprompt);
                return mkList(mkPL(mkAtom(strtoname("quote")), mkPL(p, NULL)));
            }
        } else {
            /* readpar name and return 692e */
            return mkAtom(readname(ps, flag & Quote));
        }
    }   
}
/* lex.c 693a */
Parlist readparlist(Reader r, int doquote, Prompts prompts) {
    char *s;
    Par p;
    Parlist pl = NULL;
    Parlist *ppl = &pl;
    const char *prompt1 = prompts == STD_PROMPTS ? "-> " : "";
    const char *prompt2 = prompts == STD_PROMPTS ? "   " : "";

    while (pl == NULL) {
        if ((s = readline(r, prompt1)) == NULL)
            return NULL;

        while ((p = readpar(&s, r, doquote ? Quote : 0, prompt2))) {
            *ppl = mkPL(p, NULL);
            ppl  = &(*ppl)->tl;
        }
    }
    return pl;
}
/* lex.c 693b */
void printpar(FILE *output, va_list_box *box) {
    Par p = va_arg(box->ap, Par);
    if (p == NULL) {
        fprint(output, "<null>");
        return;
    }

    switch (p->alt){
    case ATOM:
        fprint(output, "%n", p->u.atom);
        break;
    case LIST:
        fprint(output, "(%P)", p->u.list);
        break;
    }
}
