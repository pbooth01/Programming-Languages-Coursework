/* print.c 684a */
#include "all.h"

static Printer *printertab[256];
/* print.c 684b */
void installprinter(unsigned char c, Printer *printer) {
    printertab[c] = printer;
}
/* print.c 684c */
void fprint(FILE *output, const char *fmt, ...) {
    va_list_box box;

    assert(fmt);
    va_start(box.ap, fmt);
    vprint(output, fmt, &box);
    va_end(box.ap);
    fflush(output);
}
/* print.c 684d */
void print(const char *fmt, ...) {
    va_list_box box;

    assert(fmt);
    va_start(box.ap, fmt);
    vprint(stdout, fmt, &box);
    va_end(box.ap);
    fflush(stdout);
}
/* print.c 684e */
void vprint(FILE *output, const char *fmt, va_list_box *box) {
    unsigned char *p;
    int broken = 0;
    for (p = (unsigned char*)fmt; *p; p++) {
        if (*p != '%') {
            putc(*p, output);
            continue;
        }
        if (!broken && printertab[*++p])
            printertab[*p](output, box);
        else {
            broken = 1;  /* box is not consumed */
            fprintf(output, "%%%c", *p);
        }
    }
}
/* print.c 685a */
void printpercent(FILE *output, va_list_box *box) {
    (void)box;
    putc('%', output);
}
/* print.c 685b */
void printstring(FILE *output, va_list_box *box) {
    const char *s = va_arg(box->ap, char*);
    fputs(s == NULL ? "<null>" : s, output);
}
/* print.c 685c */
void printdecimal(FILE *output, va_list_box *box) {
    fprintf(output, "%d", va_arg(box->ap, int));
}
