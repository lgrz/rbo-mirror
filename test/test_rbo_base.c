/**
 *  Test the base RBO calculation.
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../rbo.h"

struct step {
    const char * e1;
    const char * e2;
    unsigned overlap;
    double rbo;
};

int main(void) {
    struct rbo_state st;
    double p = 0.95;
    int i;

    struct step dt1[] = { 
        {"a", "b", 0, 0},
        {"b", "c", 1, (1 - p) * (0.5 * pow(p, 1))},
        {"e", "e", 2, (1 - p) * (0.5 *  pow(p, 1) 
          + (double) 2/3 * pow(p, 2)) },
        {"f", "g", 2, (1 - p) * (0.5 * pow(p , 1)
          + (double) 2/3 * pow(p, 2) 
          + (double) 2/4 * pow(p, 3))},
    };

    rbo_state_init(&st, p);
    for (i = 0; i < sizeof(dt1) / sizeof(dt1[0]); i++) {
        rbo_state_update(&st, dt1[i].e1, dt1[i].e2);
        assert(st.overlap == dt1[i].overlap);
        assert(fabs(st.rbo - dt1[i].rbo) < 0.0001);
    }
    rbo_state_clear(&st);
    return 0;
}
