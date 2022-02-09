/**
 *  Test the tau calculator.
 */
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "../taucalc.h"
#include "tutil.h"

#define MAX_RNKLEN 128

struct test_case {
    char * r1 [MAX_RNKLEN];
    char * r2 [MAX_RNKLEN];
    int n;
    double tau;
};

static int do_test_case(struct test_case * tc, int tc_num) {
    FILE * fp1, * fp2;
    double tau;

    fp1 = tmpfile();
    fp2 = tmpfile();

    mk_rnk_f(fp1, tc->r1, tc->n);
    mk_rnk_f(fp2, tc->r2, tc->n);

    tau = taucalc_from_files(fp1, fp2);

    fclose(fp1);
    fclose(fp2);

    if (fabs(tau - tc->tau) > 0.00001) {
        fprintf(stderr, "Test case %d fail: expected tau %lf, "
          "got %lf\n", tc_num, tc->tau, tau);
        return 1;
    }
    return 0;
}

int main(void) {
    int t;
    int errors = 0;

    struct test_case tc[] = {
        {
            .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" },
            .r2 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" },
            .n = 10,
            .tau = 1.0
        },
        {
            .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", 
                "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", 
                "w", "x", "y", "z"},
            .r2 = { "g", "n", "u", "q", "c", "y", "m", "d", "k", "p", "w", 
                "l", "j", "x", "f", "r", "h", "a", "b", "z", "e", "t", "v", 
                "i", "o", "s"},
            .n = 26,
            .tau = 0.015384
        }
    };
    int num_tc = sizeof(tc) / sizeof(tc[0]);
    
    for (t = 0; t < num_tc; t++) {
        errors += do_test_case(&tc[t], t);
    }
    return errors;
}
