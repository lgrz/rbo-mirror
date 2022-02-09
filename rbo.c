/**
 *  The RBO indefinite rank similarity metric.
 *
 *  This code implements the RBO metric, as described in:
 *
 *  @article{wmz10:acmtois,
 *      author = "Webber, William and Moffat, Alistair and Zobel, Justin",
 *      title = "A similarity measure for indefinite rankings",
 *      journal = "ACM Transactions on Information Systems",
 *      year = {2010},
 *      note = "to appear",
 *  }
 *
 *  In particular, the code implements extrapolated RBO, Equation 32
 *  in the above paper, supporting uneven lists, but not supporting 
 *  ties (for lack of a simple input format).
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "rbo.h"

/**
 *  The fundamental step in the working of RBO is the calculation
 *  of overlap $X_d$, or size of intersection, between the two rankings 
 *  at each depth.  The key insight is that: 
 *
 *     $X_{d+1} = X_{d} + I(S_{d+1} \in T_{1:{d+1}}) 
 *                      + I(T_{d+1} \in S_{1:{d+1}})
 *
 *  where $S$ and $T$ are the two lists, and $I$ is the indicator function,
 *  return $1$ if the enclosed statement is true, $0$ otherwise.
 *  That is, the overlap at the next depth is the overlap at the current
 *  depth, plus one each if the next element in one list is found by
 *  the next depth in the other list.  To implement this efficiently,
 *  we keep a lookup set of the elements encountered in each list to date.
 *  Note that we do not require separate lookup sets for each list: we 
 *  only record elements if they've only been encountered once.
 */

#define MIN_SSET_SIZE 4096


/**
 *  Initialize the RBO state.
 */
void rbo_state_init(rbo_state_t * st, double p) {
    st->depth = 0;
    st->overlap = 0;
    st->wgt = (1 - p) / p;
    st->rbo = 0.0;
    st->p = p;
    st->seen = create_sset(MIN_SSET_SIZE);
    st->short_depth = -1;
    st->short_overlap = -1;
}

/**
 *  Update the RBO state with two new strings.
 */
void rbo_state_update(rbo_state_t * st, const char *e1, const char *e2) {
    /* Make sure that rbo_mark_end_short has not been called. */
    assert(st->short_depth == -1);
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
    st->wgt *= st->p;
    st->rbo += ((double) st->overlap / st->depth) * st->wgt;
}

/**
 *  Mark a state to show that we have reached the end of the shorter list.
 */
void rbo_mark_end_short(rbo_state_t * st) {
    st->short_depth = st->depth;
    st->short_overlap = st->overlap;
}

/**
 *  Update the RBO state with a single string.
 *
 *  This indicates that we have uneven lists, and the shorter list has
 *  finished.
 */
void rbo_state_update_uneven(rbo_state_t * st, const char * s) {
    /* rbo_mark_end_short must have been called. */
    assert(st->short_depth != -1);
    if (sset_remove(st->seen, s) == SS_HAS_KEY) {
        st->overlap += 1;
    }
    st->depth += 1;
    st->wgt *= st->p;
    /* Contribution from overlap */
    st->rbo += ((double) st->overlap / st->depth) * st->wgt;
    /* Extrapolation of overlap at end of short list */
    st->rbo += ((st->short_overlap * ((double) st->depth - st->short_depth)) /
        (st->depth * st->short_depth)) * st->wgt;
}

/**
 *  Calculate final, extrapolated RBO.
 */
double rbo_calc_extrapolated(rbo_state_t * st) {
    double p_l = pow(st->p, st->depth);
    assert(fabs((st->wgt * st->p) / (1 - st->p) - p_l) < 0.00001);
    if  (st->short_depth == -1) {
        rbo_mark_end_short(st);
    }
    return st->rbo + ((st->overlap - st->short_overlap) / (double) st->depth
      + (st->short_overlap / (double) st->short_depth)) * p_l;
}

/**
 *  Clear an rbo state.
 */
void rbo_state_clear(rbo_state_t * st) {
    sset_destroy(st->seen);
    st->seen = NULL;
}
