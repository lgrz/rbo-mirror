#include "rnkf_pr.h"
#include <assert.h>
#include <errno.h>
#include <string.h>

/**
 *  Read an entry from a file into a buffer.
 *
 *  Returns RNKF_PR_BOTH_FINISHED if the file is at EOF,
 *  RNKF_PR_IO_ERROR if there is an error on the file pointer,
 *  RNKF_PR_FORMAT_ERROR if the line is too big for the buffer,
 *  and RNKF_PR_SUCCESS on success.
 */
static rnkf_pr_stat_t read_entry(FILE * fp, char * buf, int buflen);

int rnkf_pr_init(rnkf_pr_t * rfp, FILE * fp1, FILE *fp2) {
    assert(rfp != NULL);
    assert(fp1 != NULL);
    assert(fp2 != NULL);
    assert(fp1 != fp2);

    rfp->fp1 = fp1;
    rfp->fp2 = fp2;
    rfp->buflen = RNKF_PR_BUF_SIZE;
    rfp->stat = RNKF_PR_SUCCESS;

    rfp->first_cont = 0;

    return 0;
}

rnkf_pr_stat_t rnkf_pr_next(rnkf_pr_t * rfp, char ** e1, char ** e2) {
    rnkf_pr_stat_t st1, st2;

    assert(rfp != NULL);
    assert(e1 != NULL);
    assert(e2 != NULL);
    assert(rfp->stat == RNKF_PR_SUCCESS);
    assert(rfp->first_cont == 0);

    /* Fields are only defined if both ranks are present. */
    *e1 = NULL;
    *e2 = NULL;

    st1 = read_entry(rfp->fp1, rfp->buf1, rfp->buflen);
    st2 = read_entry(rfp->fp2, rfp->buf2, rfp->buflen);

    /* Implementation note: when we come across the end of one list, but
     * not the other, we note the fact in $first_cont$, and record the
     * next element of the continued list in $buf1$.  Then, when the
     * first call to $rnkf_pr_cont$ is made, we serve the next element
     * from $buf1$, and only after that do we continue reading from
     * the files.
     */
    if (st1 < 0) {
        rfp->stat = st1;
    } else if (st2 < 0) {
        rfp->stat = st2;
    } else if (st1 == RNKF_PR_BOTH_FINISHED) {
        if (st2 == RNKF_PR_BOTH_FINISHED) {
            rfp->stat = RNKF_PR_BOTH_FINISHED;
        } else {
            /* continued ranking served out of bufffer 1 */
            strcpy(rfp->buf1, rfp->buf2);
            rfp->first_cont = 1;
            rfp->stat = RNKF_PR_RNK1_FINISHED;
        }
    } else if (st2 == RNKF_PR_BOTH_FINISHED) {
        rfp->first_cont = 1;
        rfp->stat = RNKF_PR_RNK2_FINISHED;
    } else {
        *e1 = rfp->buf1;
        *e2 = rfp->buf2;
        rfp->stat = RNKF_PR_SUCCESS;
    }
    return rfp->stat;
}

rnkf_pr_stat_t rnkf_pr_cont_next(rnkf_pr_t * rfp, char ** entry) {
    rnkf_pr_stat_t stat;

    assert(rfp != NULL);
    assert(entry != NULL);
    assert(rfp->stat == RNKF_PR_RNK1_FINISHED 
      || rfp->stat == RNKF_PR_RNK2_FINISHED);

    if (rfp->first_cont == 1) {
        *entry = rfp->buf1;
        rfp->first_cont = 0;
        return RNKF_PR_SUCCESS;
    }

    if (rfp->stat == RNKF_PR_RNK2_FINISHED) {
        stat = read_entry(rfp->fp1, rfp->buf1, rfp->buflen);
    } else {
        /* also read into buf1 */
        stat = read_entry(rfp->fp2, rfp->buf1, rfp->buflen);
    }
    if (stat < 0) {
        rfp->stat = stat;
    } else if (stat == RNKF_PR_BOTH_FINISHED) {
        rfp->stat = RNKF_PR_BOTH_FINISHED;
    } else {
        assert(stat == RNKF_PR_SUCCESS);
        *entry = rfp->buf1;
    }
    return stat;
}

void rnkf_pr_destroy(rnkf_pr_t * rfp, int close_fp) {
    assert(rfp != NULL);
    assert(rfp->fp1 != NULL);
    assert(rfp->fp2 != NULL);
    
    if (close_fp) {
        fclose(rfp->fp1);
        fclose(rfp->fp2);
    }
    rfp->fp1 = NULL;
    rfp->fp2 = NULL;
}

static rnkf_pr_stat_t read_entry(FILE * fp, char * buf, int buflen) {
    char * cp;

    errno = 0;
    if (fgets(buf, buflen, fp) == NULL) {
        if (errno != 0) {
            return RNKF_PR_IO_ERROR;
        } else {
            return RNKF_PR_BOTH_FINISHED;
        }
    }

    for (cp = buf; *cp != '\0' && *cp != '\n'; cp++)
        ;
    if (*cp != '\n') {
        return RNKF_PR_FORMAT_ERROR;
    } else {
        *cp = '\0';
        return RNKF_PR_SUCCESS;
    }
}
