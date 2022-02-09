/**
 *  Test the calculation of average overlap.
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../ao.h"

#define MAX_STEPS 100

struct step {
    const char * e1;
    const char * e2;
    unsigned overlap;
    double sa;
};

struct test_case {
    struct step steps[MAX_STEPS];
    int num_steps;
    double ao;
};

static void do_test_case(struct test_case * tc, int tc_num) {
    int i;
    ao_state_t st;

    ao_state_init(&st);
    for (i = 0; i < tc->num_steps; i++) {
        ao_state_update(&st, tc->steps[i].e1, tc->steps[i].e2);
        assert(st.overlap == tc->steps[i].overlap);
        assert(fabs(st.sa - tc->steps[i].sa) < 0.0001);
    }
    assert(ao_calc(&st) - tc->ao < 0.0001);
    ao_state_clear(&st);
}

int main(void) {
    int t;

    struct test_case tc[] = {
        {
            .steps = { 
                {"a", "b", 0, 0},
                {"b", "c", 1, 1.0 / 2 },
                {"e", "e", 2, 1.0 / 2 + 2.0 / 3 },
                {"f", "g", 2, 1.0 / 2 + 2.0 / 3 + 2.0 / 4 },
            },
            .num_steps = 4,
            .ao = 0.4166667
        },
    };
    int num_tc = sizeof(tc) / sizeof(tc[0]);

    for (t = 0; t < num_tc; t++) {
        do_test_case(&tc[t], t);
    }
    return 0;
}
