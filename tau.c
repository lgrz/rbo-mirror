#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tau.h"
#include "rnkhash.h"
#include "log.h"

/**
 *  Implementing Kendall's $\tau$.
 *
 *  We implement the method based upon a merge sort described by:
 *
 *  @article{knight66:jasa,
 *    title = "A Computer Method for Calculating Kendall's Tau with Ungrouped Data",
 *    author = "Knight, William R.",
 *    journal = jasa,
 *    volume = 61,
 *    number = 314,
 *    pages = {436--439},
 *    year = {1966}
 *  }
 *
 *  This method is also described on the Wikipedia page for
 *  "Kendall's tau".  (There is another method proposed by
 *  Christensen in 2005, but since this involves first a sort,
 *  then an insertion into a binary tree, it appears no more efficient
 *  that Knight's method.)
 *
 *  Given two rankings, S and T, which are permutations over a domain
 *  M, we find for each m \in M its rank in S.  We then replace
 *  each m \in T with m's rank in S.  Finally, we perform a merge
 *  sort on the integer values in T, and sum the number of positions
 *  during the merge that each element is moved forward.
 *
 *  Unfortunately, Knight's method requires reading the
 *  entirety of the both lists into memory, to determine the rank
 *  positions.  There is, therefore, nothing to be gained by an
 *  incremental reading of paired values.
 */

/**
 *  Sum the promotions of elements during a merge sort.
 *
 *  @param from the array of values being merged from
 *  @param to the array of values being merged to
 *  @param begin the beginning of the sequence to merge
 *  @param end one past the end of the sequence to merge
 */
static int sum_merge_promotions(int * from, int * to, int begin, int end);

double tau_calc(struct tau_lists * l) {
    rnkhash_t * rh = NULL;
    int i;
    int * ord = NULL;
    int * space = NULL;
    double tau;
    int swaps;

    if (l->n == 0) {
        log_warning("Zero-length list; returning tau of 0.0\n");
        tau = 0.0;
        goto END;
    }
   
    /* Record element positions in the first ranking */
    rh = create_rnkhash();
    if (rh == NULL) {
        goto MEM_ERROR;
    }
    for (i = 0; i < l->n; i++) {
        rnkhash_insert(rh, l->r1[i], i);
    }

    /* Replace second ranking elements with their rank in the first ranking */
    ord = malloc(sizeof(*ord) * l->n);
    if (ord == NULL) {
        goto MEM_ERROR;
    }

    for (i = 0; i < l->n; i++) {
        const char * el;
        int rnk;

        el = l->r2[i];
        rnk = rnkhash_lookup(rh, el);
        if (rnk < 0) {
            log_error("Element '%s', at rank %d of second ranking, not "
              "contained in first ranking\n", el, i);
            goto NONCONJOINT_ERROR;
        }
        ord[i] = rnk;
    }

    space = malloc(sizeof(*space) * l->n);
    if (space == NULL) {
        goto MEM_ERROR;
    }
    memcpy(space, ord, sizeof(*ord) * l->n);

    swaps = sum_merge_promotions(space, ord, 0, l->n);

#ifndef N_DEBUG
    for (i = 0; i < l->n - 1; i++) {
        assert(space[i] < space[i + 1]);
    }
#endif /* N_DEBUG */

    tau = 1 - (4 * (double) swaps) / (l->n * (l->n - 1));

    // XXX not finished!
    goto END;

MEM_ERROR:
    tau = TAU_MEM_ERROR;
    goto END;

NONCONJOINT_ERROR:
    tau = TAU_NONCONJOINT_ERROR;
    goto END;
    
END:
    if (ord != NULL) {
        free(ord);
    }
    if (space != NULL) {
        free(space);
    }
    if (rh != NULL) {
        rnkhash_destroy(rh);
    }
    return tau;
}

static int sum_merge_promotions(int * from, int * to, int begin, int end) {
#ifndef N_DEBUG
    if (begin >= end) {
        fprintf(stderr, "End is %d, begin is %d\n", end, begin);
        assert(0);
    }
#endif /* N_DEBUG */
    /* Base case of recursion */
    if (end == begin + 1) {
        to[begin] = from[begin];
        return 0;
    } else {
        /* Recursive case */
        int left_prom, right_prom, merge_prom, mid, l, r, i;

        mid = (begin + end) / 2;

        left_prom = sum_merge_promotions(to, from, begin, mid);
        right_prom = sum_merge_promotions(to, from, mid, end);

        /* The left and right partitions are sorted, in the $to$ space.  
         * Now merge them from $to$ back to $from$, counting promotions. */
        merge_prom = 0;
        i = begin;
        l = begin;
        r = mid;
        while (l < mid && r < end) {
            if (to[r] < to[l]) {
                merge_prom += r - i;
                from[i] = to[r];
                r++;
            } else {
                from[i] = to[l];
                l++;
            }
            i++;
        }
        /* only one of the next two will be executed */
        while (l < mid) {
            from[i++] = to[l++];
        }
        while (r < end) {
            merge_prom += r - i;
            from[i++] = to[r++];
        }
        return left_prom + right_prom + merge_prom;
    }
}
