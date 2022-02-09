/**
 *  Test the calculation of Kendall's $tau$.
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "../log.h"
#include "../tau.h"
#include "tc_tau.h"

static int do_test_case(struct tc_tau * tc, int tc_num) {
    struct tau_lists tl;
    double tau;

    tl.r1 = tc->r1;
    tl.r2 = tc->r2;
    tl.n = tc->n;

    if (tc->tau < -1.0) {  /* ERROR */
        log_set_error_stream(NULL);
    }
    tau = tau_calc(&tl);
    if (fabs(tau - tc->tau) > 0.0001) {
        fprintf(stderr, "Test case %d, tau should be %lf, is %lf\n",
          tc_num, tc->tau, tau);
        return 1;
    }
    log_set_error_stream(stderr);
    return 0;
}

int main(void) {
    int t;
    int errors = 0;

    int num_tc = sizeof(tc_tau) / sizeof(tc_tau[0]);
    
    for (t = 0; t < num_tc; t++) {
        errors += do_test_case(&tc_tau[t], t);
    }
    return errors;
}
