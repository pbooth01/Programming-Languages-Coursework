/* prim.h 150d */
xx("+", PLUS,  arith)
xx("-", MINUS, arith)
xx("*", TIMES, arith)
xx("/", DIV,   arith)
xx("<", LT,    arith)
xx(">", GT,    arith)
/* prim.h 152b */
xx("cons", CONS, binary)
xx("=",    EQ,   binary)
/* prim.h 154a */
xx("boolean?",   BOOLEANP,   unary)
xx("null?",      NULLP,      unary)
xx("number?",    NUMBERP,    unary)
xx("pair?",      PAIRP,      unary)
xx("procedure?", PROCEDUREP, unary)
xx("symbol?",    SYMBOLP,    unary)
xx("car",        CAR,        unary)
xx("cdr",        CDR,        unary)
xx("print",      PRINT,      unary)
xx("error",      ERROR,      unary)
