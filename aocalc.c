#include "aocalc.h"
#include "rnkf_pr.h"
#include "ao.h"
#include "log.h"

double aocalc_from_files(FILE * fp1, FILE * fp2) {
    rnkf_pr_t rfp;
    rnkf_pr_stat_t stat;
    ao_state_t ao_st;
    char * e1, * e2;
    int rnk = 0;
    double ao;

    rnkf_pr_init(&rfp, fp1, fp2);
    ao_state_init(&ao_st);

    while ( (stat = rnkf_pr_next(&rfp, &e1, &e2)) == RNKF_PR_SUCCESS) {
        rnk++;
        ao_state_update(&ao_st, e1, e2);
    }

    if (stat == RNKF_PR_FORMAT_ERROR) {
        log_error("Format error at rank %d; line too long?\n", rnk);
        ao = -1.0;
    } else if (stat == RNKF_PR_IO_ERROR) {
        log_error("Read error at rank %d\n", rnk);
        ao = -1.0;
    } else {
        if (stat == RNKF_PR_RNK1_FINISHED || stat == RNKF_PR_RNK2_FINISHED) {
            log_warning("Uneven rankings not supported by Average Overlap; "
              "truncating at length of shorter ranking");
        }
        ao = ao_calc(&ao_st);
    }

    ao_state_clear(&ao_st);
    rnkf_pr_destroy(&rfp, 0);
    return ao;
}
