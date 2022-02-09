/**
 *  Test the RBO calculator.
 */
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "../rbocalc.h"
#include "tutil.h"

#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define MAX_RNKLEN 1024

struct rbocalc_tc {
    char * r1 [MAX_RNKLEN];
    char * r2 [MAX_RNKLEN];
    int len1;
    int len2;
    double p;
    double rbo;
};

/* XXX also do some malformed test cases */

static struct rbocalc_tc testcases[] = {
    {
        .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", NULL },
        .r2 = { "h", "g", "f", "e", "d", "c", "b", "a", NULL },
        .len1 = 8,
        .len2 = 8,
        .p = 0.95,
        .rbo = 0.771924
    },
    {
        .r1 = { "g", "a", "f", "c", "z", NULL },
        .r2 = { "a", "b", "c", "d", NULL },
        .len1 = 5,
        .len2 = 4,
        .p = 0.8,
        .rbo = 0.3786667
    },
    { 
        .r1 = {"a", "b", "c", "d", "e", "f", "g", "h", NULL },
        .r2 = {"b", "a", "d", "f", "c", "h", NULL },
        .len1 = 8,
        .len2 = 6,
        .p = 0.8, 
        .rbo = 0.6869504
    },
    {
        .r1 = {"a", "b", "c", "d", "e", NULL },
        .r2 = {"b", "a", "g", "h", "e", "k", "l", "c", NULL },
        .len1 = 5,
        .len2 = 8,
        .p = 0.9,
        .rbo = 0.6338971
    },
};

static void mk_rnk_csv_f(FILE * fp, struct rbocalc_tc * tc) {
    int l;

    for (l = 0; l < MAX(tc->len1, tc->len2); l++) {
        if (l < tc->len1) {
            fputs(tc->r1[l], fp);
        }
        putc(',', fp);
        if (l < tc->len2) {
            fputs(tc->r2[l], fp);
        }
        putc('\n', fp);
    }
    fseek(fp, 0, SEEK_SET);
}

static void mk_rnk_pr_f(FILE * fp1, FILE * fp2, struct rbocalc_tc * tc) {
    mk_rnk_f(fp1, tc->r1, tc->len1);
    mk_rnk_f(fp2, tc->r2, tc->len2);
}

static int do_onefile_test(struct rbocalc_tc * tc, int tc_num) {
    FILE * fp = tmpfile();
    double rbo;

    mk_rnk_csv_f(fp, tc);
    rbo = rbocalc_from_file(fp, tc->p);
    fclose(fp);
    if (fabs(rbo - tc->rbo) > 0.00001) {
        fprintf(stderr, "Test case %d fail (one file): expected RBO %lf, "
          "got %lf\n", tc_num, tc->rbo, rbo);
        return 1;
    }
    return 0;
}

static int do_twofile_test(struct rbocalc_tc * tc, int tc_num) {
    FILE * fp1 = tmpfile();
    FILE * fp2 = tmpfile();
    double rbo;

    mk_rnk_pr_f(fp1, fp2, tc);
    rbo = rbocalc_from_files(fp1, fp2, tc->p);
    fclose(fp1);
    fclose(fp2);
    if (fabs(rbo - tc->rbo) > 0.00001) {
        fprintf(stderr, "Test case %d fail (two file): expected RBO %lf, "
          "got %lf\n", tc_num, tc->rbo, rbo);
        return 1;
    }
    return 0;
}

static int do_test_case(struct rbocalc_tc * tc, int tc_num) {
    return do_onefile_test(tc, tc_num) + do_twofile_test(tc, tc_num);
}

int main(void) {
    int t;
    int num_testcases = sizeof(testcases) / sizeof(testcases[0]);
    int error = 0;

    for (t = 0; t < num_testcases; t++) {
        error += do_test_case(&testcases[t], t);
    }
    return error;
}
