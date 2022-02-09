#ifndef RNKF_PR_H
#define RNKF_PR_H

#include <stdio.h>

/**
 *  Read rank entries from two separate files.
 */

#define RNKF_PR_BUF_SIZE 1024 /* Maximum length of an input value */

/**
 *  Return values for rankfile pair functions.
 */
typedef enum {
    RNKF_PR_SUCCESS = 0,
    RNKF_PR_RNK1_FINISHED = 1,
    RNKF_PR_RNK2_FINISHED = 2,
    RNKF_PR_BOTH_FINISHED = 3,
    RNKF_PR_FORMAT_ERROR = -1,
    RNKF_PR_IO_ERROR = -2
} rnkf_pr_stat_t;

/**
 *  Rankfile pair state
 */
typedef struct rnkf_pr {
    FILE * fp1, *fp2;
    char buf1[RNKF_PR_BUF_SIZE], buf2[RNKF_PR_BUF_SIZE];
    int buflen;
    rnkf_pr_stat_t stat;  /* SUCCESS if neither finished */

    /* Internal fields. */
    int first_cont;
} rnkf_pr_t;

/**
 *  Initialize a new rankfile pair.
 */
int rnkf_pr_init(rnkf_pr_t * rfp, FILE * fp1, FILE *fp2);

/**
 *  Read the next entry pair from the rankfile pair.  
 *
 *  This should only be called while both rankings are ongoing.
 *  Returns $RNKF_PR_SUCCESS$ if both rankings continue, and places
 *  the values into $e1$, $e2$ below.  If one or other ranking
 *  finishes early, returns $RNKF_PR_RNKX_FINISHED$.  In this
 *  case, the contents of $e1$ and $e2$ are undefined.  The
 *  user should then switch to calling $rnkf_pr_cont_next$.
 *
 *  @param e1 the entry in the first ranking.
 *  @param e2 the entry in the second ranking.
 */
rnkf_pr_stat_t rnkf_pr_next(rnkf_pr_t * rfp, char ** e1, char ** e2);

/**
 *  Read the next entry from the continuing ranking.
 *
 *  This should only be called after $rnkf_pr_next$ has returned
 *  either $RNKF_PR_RNK1_FINISHED$ or $RNKF_PR_RNK2_FINISHED$.  The
 *  first call will return the first element in the longer list
 *  after the end of the shorter list.  When the end of the 
 *  continuing ranking is reached, this function will return 
 *  $RNKF_PR_BOTH_FINISHED$.
 *
 *  @param entry the entry in the continuing ranking.
 */
rnkf_pr_stat_t rnkf_pr_cont_next(rnkf_pr_t * rfp, char ** entry);

/**
 *  Close a rankfile pair and free resources.
 *
 *  @param close_fp 1 to close the internal FPs, 0 to leave as is.
 */
void rnkf_pr_destroy(rnkf_pr_t * rfp, int close_fp);

#endif /* RNKF_PR_H */
