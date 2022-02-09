/**
 *  The average overlap metric.
 *
 *  This metric is called the intersection metric by Fagin et. al, 2003,
 *  and average accuracy by Wu and Crestani, 2003.
 *
 *  As described by the above authors (and by us in TOIS, 2010),
 *  average overlap handles neither ties nor uneven rankings.
 *  In the latter case, we calculate the metric to the length of
 *  the shorter ranking.  The former will be dealt with when we
 *  add support for ties.
 */
#include <stdlib.h>
#include <string.h>
#include "ao.h"

#define MIN_SSET_SIZE 4096

void ao_state_init(ao_state_t * st) {
    st->depth = 0;
    st->overlap = 0;
    st->sa = 0.0;
    st->seen = create_sset(MIN_SSET_SIZE);
}

void ao_state_update(ao_state_t * st, const char * e1, const char * e2) {
    if (strcmp(e1, e2) == 0) {
        st->overlap += 1;
    } else {
        if (sset_remove(st->seen, e1) == SS_HAS_KEY) {
            st->overlap += 1;
        } else {
            sset_insert(st->seen, strdup(e1));
        }
        if (sset_remove(st->seen, e2) == SS_HAS_KEY) {
            st->overlap += 1;
        } else {
            sset_insert(st->seen, strdup(e2));
        }
    }
    st->depth += 1;
    st->sa += (double) st->overlap / (double) st->depth;
}

double ao_calc(ao_state_t * st) {
    return st->sa / (double) st->depth;
}

void ao_state_clear(ao_state_t * st) {
    sset_destroy(st->seen);
    st->seen = NULL;
}
