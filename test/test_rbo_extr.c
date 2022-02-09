/**
 *  Test the extrapolated RBO calculation.
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>

#define MAX_TESTCASE_LEN 128

#include "../rbo.h"

struct testcase {
    const char * r1[MAX_TESTCASE_LEN];
    const char * r2[MAX_TESTCASE_LEN];
    double p;
    double rbo;
};

int main(void) {
    struct rbo_state st;
    int t, i;

    struct testcase tc [] = {
        { 
            {"a", "b", "e", "f", NULL },
            {"b", "c", "f", "g", NULL },
            0.98, 
            0.4867987
        },
        { 
            {"a", "b", "e", "f", NULL },
            {"g", NULL },
            0.95, 
            0.0
        },
        { 
            {"a", "b", "e", "f", NULL },
            {"e", NULL },
            0.95, 
            0.2293854
        },
        { 
            {"a", "b", "c", "d", "e", "f", "g", "h", NULL },
            {"b", "a", "d", "f", "c", "h", NULL },
            0.8, 
            0.6869504
        },
    };
    int error = 0;
    for (t = 0; t < sizeof(tc) / sizeof(tc[0]); t++) {
        double extr_rbo;
        rbo_state_init(&st, tc[t].p);
        for (i = 0; tc[t].r1[i] != NULL && tc[t].r2[i] != NULL; i++) {
            rbo_state_update(&st, tc[t].r1[i], tc[t].r2[i]);
        }
        rbo_mark_end_short(&st);
        /* No more than one of the following will be executed. */
        for ( ; tc[t].r1[i] != NULL; i++) {
            rbo_state_update_uneven(&st, tc[t].r1[i]);
        }
        for ( ; tc[t].r2[i] != NULL; i++) {
            rbo_state_update_uneven(&st, tc[t].r2[i]);
        }
        extr_rbo = rbo_calc_extrapolated(&st);
        if (fabs(extr_rbo - tc[t].rbo) > 0.00001) {
            fprintf(stderr, "Gap in extrapolated RBO: should be %.5lf, is "
              "%.5lf, for %dth test case\n", tc[t].rbo, extr_rbo, t);
            error++;
        }
        rbo_state_clear(&st);
    }
    return error;
}
