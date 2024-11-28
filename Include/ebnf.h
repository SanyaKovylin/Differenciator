#ifndef EBNF_H_INCLUDED
#define EBNF_H_INCLUDED

#include "expr_tree.h"

struct Oper {
    e_oper num;
    const char* name;
    const char* altname;
};

#define OPERATOR(name, number, str, nargs, ispref, isfunc, prior, diff_action, action, simplifier, alterstr) \
    {name, str, alterstr}

Oper* Operators = {
    #include "operators.h"
};

#undef OPERATOR

#endif //EBNF_H_INCLUDED
