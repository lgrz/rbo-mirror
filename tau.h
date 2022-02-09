#ifndef TAU_H
#define TAU_H

/**
 *  Kendall's $\tau$ rank correlation coefficient.
 */

/* Error cases */
#define TAU_MEM_ERROR -2.0
#define TAU_NONCONJOINT_ERROR -3.0

/**
 *  Rankings as processed by Kendall's $\tau$.
 *
 *  Note that the entirety of both lists must be in memory.
 */
struct tau_lists {
    const char ** r1;
    const char ** r2;
    int n;
};

/**
 *  Calculate tau between two rankings.
 */
double tau_calc(struct tau_lists * l);

#endif /* TAU_H */
