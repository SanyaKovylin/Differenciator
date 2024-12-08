#include <stdlib.h>
#include <stdio.h>

#include "expr_tree.h"
// #include "akakiy.h"
#include "ebnf.h"
#include "lexsint.h"

int main(void){
    e_tree TTT = {};
    TreeCtor(&TTT, NULL, "MY TREE");

    FParseInf("expr.txt", &TTT);
    // ParseExpressionFromFile(&TTT, "input.txt");
    PrintTree(&TTT);
    // printf("passed\n");
    TTT.curr_node = &TTT.head;
    // printptr(&TTT);
    ETreeSimplifier(&TTT);
    // printptr(&TTT);
    // printf("simp\n");

    TTT.curr_node = &TTT.head;
    PrintTree(&TTT);

    e_tree NewT = {};
    TreeCtor(&NewT, NULL, "Der");

    ETreeDerivate(&TTT, &NewT);

    // printf("\n");

    PrintTree(&NewT);

    ETreeSimplifier(&NewT);

    PrintTree(&NewT);

    // BnfRunBuild("Source/Rules.txt", "BNFParse/Source/Parser.cpp", "Include/operators.h");

    // LexParse("expr.txt", &n);
    // SintParse(&TTT, n);
//
//     PrintTree(&TTT);
}
