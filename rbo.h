#ifndef RBO_H
#define RBO_H

#include "sset.h"

/**
 *  State of an RBO calculation.
 *
 *  @param depth is the current depth, counting from 1.
 *  @param overlap the current overlap.
 *  @param geom the current (geometrically decreasing) weight.
 *  @param rbo is the cumulative RBO score to date.
 *  @param p the p value being used.
 *  @param seen the strings seen to date.
 */
typedef struct rbo_state {
    /* General fields */
    unsigned depth;
    unsigned overlap;
    double wgt;
    double rbo;
    double p;
    sset * seen;

    /* Fields for handling uneven lists. */
    unsigned short_depth;
    unsigned short_overlap;
} rbo_state_t;

/**
 *  Initialize the RBO state.
 */
void rbo_state_init(rbo_state_t * st, double p);

/**
 *  Update the RBO state with two new elements.
 *
 *  Both strings must be non-empty and non-null.  See rbo_mark_end_short
 *  for dealing with the end of one of the lists.
 */
void rbo_state_update(rbo_state_t * st, const char *e1, const char *e2);

/**
 *  Inform a state to that we have reached the end of the shorter list.
 *
 *  Either list can be the short list; RBO doesn't actually care.
 */
void rbo_mark_end_short(rbo_state_t * st);

/**
 *  Update the RBO state with a single string.
 *
 *  This occurs after a single list has finished.  rbo_mark_end_short
 *  must be called first.
 */
void rbo_state_update_uneven(rbo_state_t * st, const char * s);

/**
 *  Calculate final, extrapolated RBO.
 */
double rbo_calc_extrapolated(rbo_state_t * st);

/**
 *  Clear an rbo state.
 */
void rbo_state_clear(rbo_state_t * st);

#endif /* RBO_H */
