#include <stdlib.h>
#include "taucalc.h"
#include "tau.h"
#include "strarr.h"
#include "util.h"

double taucalc_from_files(FILE * fp1, FILE * fp2) {
    strarr_t * sa1 = NULL, * sa2 = NULL;
    double tau;
    struct tau_lists tl = {.r1 = NULL, .r2 = NULL, .n = 0};
    int i;

    if ( (sa1 = read_lines(fp1)) == NULL) {
        goto MEM_ERROR;
    }
    if ( (sa2 = read_lines(fp2)) == NULL) {
        goto MEM_ERROR;
    }

    if (sa1->str_num != sa2->str_num) {
        tau = TC_UNEVEN_LISTS;
        goto END;
    }

    if ( (tl.r1 = malloc(sizeof(*tl.r1) * sa1->str_num)) == NULL) {
        goto MEM_ERROR;
    }
    if ( (tl.r2 = malloc(sizeof(*tl.r2) * sa2->str_num)) == NULL) {
        goto MEM_ERROR;
    }
    tl.n = sa1->str_num;

    for (i = 0; i < tl.n; i++) {
        tl.r1[i] = STRARR_GET(sa1, i);
        tl.r2[i] = STRARR_GET(sa2, i);
    }
    tau = tau_calc(&tl);

    goto END;

MEM_ERROR:
    tau = TC_OUT_OF_MEM;
    goto END;

END:
    if (sa1 != NULL) {
        strarr_delete(&sa1);
    }
    if (sa2 != NULL) {
        strarr_delete(&sa2);
    }
    if (tl.r1 != NULL) {
        free(tl.r1);
    }
    if (tl.r2 != NULL) {
        free(tl.r2);
    }
    return tau;
}
