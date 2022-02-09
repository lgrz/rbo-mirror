#include <stdlib.h>

#include "args.h"
#include "log.h"
#include "rbocalc.h"
#include "aocalc.h"
#include "taucalc.h"
#include "util.h"
#include "trec.h"

/**
 *  Main function.
 *
 *  Usage: rbo [-t] [-m <metric>] [-p <p-value>] {<input> | <input1> <input2>}
 *
 *    <input>: the input rankings, as two columns, separated by a comma.
 *        One column may run out before the other, in which case the
 *        column should be left empty (but the comma should not be omitted).
 *        Spaces will be stripped.
 *    <input1>, <input2>: files holding individual rankings, one entry
 *        per line.
 *    -m: the rank similarity metric to use.  Defaults to "rbo".
 *    -p: the persistence parameter (see wmz10:acmtois).  A value
 *        in the range $0 <= p < 1$.  Lower values are less top-weighted.
 *        Default values is $p = 0.98$.  Only for the RBO metric.
 *    -t: the pair of input files are TREC run files
 *
 *  Exits with 1 if help asked for; 0 if processing is successful; < 0
 *  on error.
 */
int main(int argc, const char * argv[]) {
    args_t args;
    args_stat_t ret;
    FILE * rnk_fp1, * rnk_fp2;

    args_init(&args);
    if ( (ret = args_parse(&args, argc, argv)) == ARGS_HELP) {
        return 1;  /* Help asked for and provided; return with success */
    } else if (ret == ARGS_ERROR) {
        return ret;
    }

    // XXX add support for other metrics.
    // probably, withdraw support for CSV-format file.
/*    if (args.metric != METRIC_RBO) {
        log_error("Only RBO is currently supported\n");
        return -1;
    } */

    if (ret == ARGS_ONE_FILE) {
        /* FILE * rnk_fp;

        rnk_fp = open_rd_or_exit(args.rnkfile);
        rbo = rbocalc_from_file(rnk_fp, args.p);
        fclose(rnk_fp); */
        log_error("Single-file input is not currently supported\n");
        return -1;
    }

    rnk_fp1 = open_rd_or_exit(args.rnkfile1);
    rnk_fp2 = open_rd_or_exit(args.rnkfile2);

    if (args.trec) {
        trec_score_t * scores;
        int s, num_scores;
        
        scores = malloc(sizeof(*scores) * MAX_NUM_TREC_TOPICS);
        if (scores == NULL) {
            log_error("Out of memory\n");
            return -1;
        }
        switch (args.metric) {
        case METRIC_RBO:
            num_scores = rbo_calc_from_trec_runs(rnk_fp1, rnk_fp2, args.p,
              &scores);
            break;
        default:
            log_error("TREC input only implemented for RBO\n");
            return -1;
        }

        if (num_scores < 0) {
            log_error("Error processing TREC runs\n");
            free(scores);
            return -1;
        }
        
        for (s = 0; s < num_scores; s++) {
            fprintf(stdout, "%s %lf\n", scores[s].topic_id, scores[s].score);
        }

        free(scores);
    } else {
        double score;

        switch (args.metric) {
        case METRIC_RBO:
            score = rbocalc_from_files(rnk_fp1, rnk_fp2, args.p);
            if (score < 0.0) {
                return -1;
            }
            break;
        case METRIC_AO:
            score = aocalc_from_files(rnk_fp1, rnk_fp2);
            if (score < 0.0) {
                return -1;
            }
            break;
        case METRIC_TAU:
            score = taucalc_from_files(rnk_fp1, rnk_fp2);
            if (score < -1.0) {
                if (score == TC_UNEVEN_LISTS) {
                    log_error("Lists are not of the same length\n");
                }
                return -1;
            }
            break;
        case METRIC_UNKNOWN:
            return -1;
            break;
        }
        fprintf(stdout, "%lf\n", score);
    }

    fclose(rnk_fp1);
    fclose(rnk_fp2);
    return 0;
}

