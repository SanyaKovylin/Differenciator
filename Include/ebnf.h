#ifndef EBNF_H_INCLUDED
#define EBNF_H_INCLUDED

#include "expr_tree.h"

struct Oper {
    e_oper num;
    const char* name;
    const char* altname;
};

typedef struct Function {
    const char* name;
} bnf_func;

const int DefaultFuncNum = 4;

typedef enum Errors {
    ALL_RIGHT = 0,
    EMPTY_POINTER = 1,
    BAD_WORD = 2,
} bnf_err;

typedef enum Indicator {
    GOT_IT = 0,
    NETY = 1,
} bnf_ind;

bnf_err BnfRunBuild(const char *srcfile, const char* outputfile, const char* database);
bnf_err BnfInit(const char *srcfile, const char* outf, Buffer *src, bnf_func **funcs);

bnf_err BnfParse(Buffer *src, bnf_func *funcs);
bnf_err BnfParseLine(Buffer *src, bnf_func *funcs);
bnf_err BnfParseWord(Buffer *src, char *name);
bnf_err BnfParsePhrase(Buffer *src, bnf_func *funcs, char* flagname);
bnf_err BnfParseCmd(char *comd, char flagname);



bnf_err repeat(bnf_err err);
#define Check ERR = ALL_RIGHT; (ERR =

template <typename T>
inline bnf_err val (T value) {return repeat(value);}

#define val(value) ( value )
#define verified ) || val(ERR)

#endif //EBNF_H_INCLUDED
