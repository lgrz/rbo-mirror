/**
 *  Test the AO calculator.
 */
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "tutil.h"
#include "../log.h"
#include "../aocalc.h"

#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define MAX_RNKLEN 1024

struct aocalc_tc {
    char * r1 [MAX_RNKLEN];
    char * r2 [MAX_RNKLEN];
    int len1;
    int len2;
    double ao;
};

static struct aocalc_tc testcases[] = {
    {
        .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", NULL },
        .r2 = { "h", "g", "f", "e", "d", "c", "b", "a", NULL },
        .len1 = 8,
        .len2 = 8,
        .ao = 0.3654762
    },
    {
        .r1 = { "g", "a", "f", "c", "z", NULL },
        .r2 = { "a", "b", "c", "d", NULL },
        .len1 = 5,
        .len2 = 4,
        .ao = 0.3333333
    },
    { 
        .r1 = {"a", "b", "c", "d", "e", "f", "g", "h", NULL },
        .r2 = {"b", "a", "d", "f", "c", "h", NULL },
        .len1 = 8,
        .len2 = 6,
        .ao = 0.675
    },
    {
        .r1 = {"a", "b", "c", "d", "e", NULL },
        .r2 = {"b", "a", "g", "h", "e", "k", "l", "c", NULL },
        .len1 = 5,
        .len2 = 8,
        .ao = 0.5533333
    },
    {
        .r1 = {NULL},
        .r2 = {NULL},
        .len1 = 0,
        .len2 = 0,
        .ao = 0.0
    },
    {
        .r1 = {"a", "b", "c"},
        .r2 = {"d", "e", "f"},
        .len1 = 0,
        .len2 = 0,
        .ao = 0.0
    },
};

static int do_test_case(struct aocalc_tc * tc, int tc_num) {
    FILE * fp1 = tmpfile();
    FILE * fp2 = tmpfile();
    double ao;

    mk_rnk_f(fp1, tc->r1, tc->len1);
    mk_rnk_f(fp2, tc->r2, tc->len2);
    ao = aocalc_from_files(fp1, fp2);
    fclose(fp1);
    fclose(fp2);

    if (fabs(ao - tc->ao) > 0.00001) {
        fprintf(stderr, "Test case %d failed: expected AO %lf, got %lf\n",
          tc_num, tc->ao, ao);
        return 1;
    }
    return 0;
}

int main(void) {
    int t;
    int num_testcases = sizeof(testcases) / sizeof(testcases[0]);
    int error = 0;

    /* turn off warnings, because uneven lists create them */
    log_set_warning_output_on(0);

    for (t = 0; t < num_testcases; t++) {
        error += do_test_case(&testcases[t], t);
    }

    return 0;
}
