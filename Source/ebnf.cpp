#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "ebnf.h"

bnf_err ERR = ALL_RIGHT;
bnf_err repeat(bnf_err err){return err;}

#define OPERATOR(name, number, str, nargs, ispref, isfunc, prior, diff_action, action, simplifier, alterstr) \
    {name, str, alterstr},

Oper Operators[] = {
    #include "operators.h"
};

#undef OPERATOR

static FILE *writef = NULL;

bnf_err BnfRunBuild(const char *srcfile, const char* outputfile, const char* database){

    Buffer src = {};
    bnf_func *functions = NULL;

    Check BnfInit(srcfile, outputfile, &src, &functions) verified;

    Check BnfParse(&src, functions) verified;

    return ALL_RIGHT;
}

bnf_err BnfInit(const char *srcfile, const char* outf, Buffer *src, bnf_func **funcs){

    char *buf = NULL;
    size_t lenbuf = Read(srcfile, &buf);

    *src = {
        .buffer = buf,
        .readptr = 0,
        .len = lenbuf,
    };

    writef = fopen(outf, "w");

    bnf_func *functions = (bnf_func*) calloc (DefaultFuncNum, sizeof(bnf_func));

    if (functions == NULL) return EMPTY_POINTER;

    *funcs = functions;

    return ALL_RIGHT;
}

#define readval src->buffer[src->readptr]
#define readpt src->buffer + src->readptr

bnf_err BnfParse(Buffer *src, bnf_func *funcs){

    assert(   src != NULL);
    assert( funcs != NULL);

    fprintf(writef,"#include \"parsing.h\"\n");

    while (src->readptr < src->len - 2) {
        Check BnfParseLine(src, funcs) verified;
        src->readptr++;
    }

    return ALL_RIGHT;
}

const char *obraces = "\"({<[$";
const char *cbraces = "\")}>]$";

const int MxWrdLth = 20;

#define STRIP while (isspace(readval)) src->readptr++;
#define SKIP(chr) STRIP; if(readval == chr) src->readptr++; STRIP;
#define SKIP_BRACES(dst, str) char *dst = str + 1; dst[strlen(str) - 2] = '\0';

bnf_err BnfParseLine(Buffer *src, bnf_func *funcs){

    char name[MxWrdLth] = {};
    printf("newline\n\n");
    Check BnfParseWord(src, name) verified;
    SKIP_BRACES(func, name);

    fprintf(writef , "ind %s (void);\n"
    "ind %s(void){\n"
    "\tind ans = NOT_FOUND;\n", func, func);

    SKIP('=');

    char start = 'A';

    while (readval != '.'){
        fprintf(writef, "if (true){\n"
        "\tind %c = NOT_FOUND;\n", start);

        Check BnfParsePhrase(src, funcs, &start) verified;

        fprintf(writef, "\tans || %c;\n", start);

        start++;

        fprintf(writef, "}\n");

        SKIP('|');
    }

    fprintf(writef, "\treturn ans;\n""}\n");

    return ALL_RIGHT;
}

bnf_err BnfParsePhrase(Buffer *src, bnf_func *funcs, char* flagname){

    char comd[MxWrdLth] = {};
    Check BnfParseWord(src, comd) verified;

    switch (comd[0]){

        case '{':
            (*flagname)++;
            fprintf(writef, "\tind %c = OK;\n"
            "\twhile (%c == OK){\n", *flagname, *flagname);

            src->readptr -= strlen(comd) - 1;
            while (readval != ']'){
                BnfParsePhrase(src, funcs, flagname);
                SKIP('|');
            }
            (*flagname)--;
            break;

        case '[':

            fprintf(writef, "\t%s();\n", comd);
            break;

        default: BnfParseCmd(comd, *flagname);
    }
    return ALL_RIGHT;
}

bnf_err BnfParseCmd(char *comd, char flagname){

    const char brace = comd[0];
    SKIP_BRACES(cmd, comd);

    printf("Command: %s\n", cmd);

    char *back = NULL;
    char slash[] = "\\";
    char empty[] = "";

    switch (brace){

        case '<':
            fprintf(writef, "\t ans = ans && %s();\n", cmd);
            break;



        case '$':

            #define OPERATOR(name, number, str, nargs, ispref, isfunc, prior, diff_action, action, simplifier, alterstr)\
                if (stricmp(#name, cmd) == 0){\
                    if (str[0] == '\\') back = slash; else back = empty;\
                    if (strcmp(alterstr, ""))   fprintf(writef, "\t%c = %c && get(\"%s%s\") || get(\"%s\");\n",flagname, flagname, back, str, alterstr);\
                    else fprintf(writef, "\t%c = %c && get(\"%s%s\");\n",flagname, flagname, back, str);} else

            #include "operators.h"

            //else
            {fprintf(stderr, "found exception!!!\n");}

            break;

        case '\"':
            fprintf(writef, "\t%c = %c && get(\"%s\");\n",flagname, flagname, cmd);
            break;

        default: printf("found strange brace: .%c.", brace);
    }


    return ALL_RIGHT;
}

// bnf_err CondPhrase(){
//
// }

bnf_err BnfParseWord(Buffer *src, char *name){

    assert( src != NULL);
    assert(name != NULL);

    STRIP;

    char format[MxWrdLth] = {};
    sprintf(format, "%%[^%s]", cbraces);

    char open = readval;
    int len = 1;
    char *place = NULL;

    if (!(place = strchr(obraces, open))){

        // src->readptr++;
        return BAD_WORD;
    }

    char close = cbraces[place - obraces];
    int cnt = 1;

    while (cnt > 0){

        cnt += (*(readpt + len) == open) * (open != close);
        cnt -= (*(readpt + len) == close);
        len++;
    }

    memcpy(name, readpt, len);
    name[len + 1] = '\0';
    src->readptr += len;

    printf("Got name: %s of lenght %d\n", name, len);
    STRIP;
    if (name[0] != name[len - 1]) return BAD_WORD;
    return ALL_RIGHT;
}
