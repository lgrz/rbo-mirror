#ifndef AO_H
#define AO_H

#include "sset.h"

/**
 *  State of an Average Overlap (AO) calculation.
 *
 *  @param depth is the current depth, counting from 1.
 *  @param overlap is the current overlap.
 *  @param sa the sum of the agreements observed so far.
 *  @param seen is the set of strings seen to date.
 */
typedef struct ao_state {
    unsigned depth;
    unsigned overlap;
    double sa;  
    sset * seen;
} ao_state_t;

/**
 *  Initialize the AO state
 */
void ao_state_init(ao_state_t * st);

/**
 *  Update the AO state with two new elements.
 */
void ao_state_update(ao_state_t * st, const char * e1, const char * e2);

/**
 *  Calculate the final AO score.
 */
double ao_calc(ao_state_t * st);

/**
 *  Clear an AO state.
 */
void ao_state_clear(ao_state_t * st);

#endif /* AO_H */
