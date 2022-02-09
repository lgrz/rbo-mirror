/**
 *  Test the rnkfile pair reader
 */
#include "../rnkf_pr.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 *  XXX largely cut-and-paste from test_rnkf.c.
 */

#define MAX_RNKLEN 128

struct rnkf_pr_tc {
    char * r1 [MAX_RNKLEN];
    char * r2 [MAX_RNKLEN];
    int len1;
    int len2;
};

static struct rnkf_pr_tc testcases[] = {
    {
        { "a", "b", "c", "d", "e", "f", "g", "h", NULL },
        { "h", "g", "f", "e", "d", "c", "b", "a", NULL },
        8,
        8
    },
    {
        { "foo", "bar", "baz", "wumpus", "wazoo", NULL },
        { "a", "b", "c", "d", NULL },
        5,
        4
    },
    {
        { "a", "c", "e", "f", "h", NULL},
        { "a", "b", "c", "d", "e", "f", "g", "h", "i", NULL },
        5,
        9
    },
};

static int do_test_case(struct rnkf_pr_tc * tc, int tc_num) {
    FILE * tmpf1 = tmpfile();
    FILE * tmpf2 = tmpfile();
    rnkf_pr_t rfp;
    char * e1, * e2, * entry;
    int i;
    rnkf_pr_stat_t stat;
    
    for (i = 0; i < tc->len1; i++) {
        fputs(tc->r1[i], tmpf1);
        putc('\n', tmpf1);
    }
    for (i = 0; i < tc->len2; i++) {
        fputs(tc->r2[i], tmpf2);
        putc('\n', tmpf2);
    }
    fseek(tmpf1, 0, SEEK_SET);
    fseek(tmpf2, 0, SEEK_SET);
    rnkf_pr_init(&rfp, tmpf1, tmpf2);

    for (i = 0; i < tc->len1 && i < tc->len2; i++) {
        if ( (stat = rnkf_pr_next(&rfp, &e1, &e2)) != RNKF_PR_SUCCESS) {
            fprintf(stderr, "Error reading rank %d of rnkfiles; "
              "return code %d\n", i, stat);
            rnkf_pr_destroy(&rfp, 0);
            return 1;
        }
        if (strcmp(e1, tc->r1[i]) != 0) {
            fprintf(stderr, "Rank %d, first ranking, expected '%s', "
              "got '%s'\n", i, tc->r1[i], e1);
            rnkf_pr_destroy(&rfp, 0);
            return 1;
        }
        if (strcmp(e2, tc->r2[i]) != 0) {
            fprintf(stderr, "Rank %d, first ranking, expected '%s', "
              "got '%s'\n", i, tc->r2[i], e2);
            rnkf_pr_destroy(&rfp, 0);
            return 1;
        }
    }
    /* one or both lists should now finish */
    stat = rnkf_pr_next(&rfp, &e1, &e2);
    if (stat < 0) {
        fprintf(stderr, "Error reading %d of rnkfiles; return code %d\n",
          i, stat);
        return 1;
    } else if (stat == RNKF_PR_BOTH_FINISHED) {
        if (tc->len1 != tc->len2) {
            fprintf(stderr, "Uneven rankings of depth %d and %d, but "
              "both finished at rank %d\n", tc->len1, tc->len2, i);
            return 1;
        } else {
            assert(i == tc->len1 && i == tc->len2);
            return 0;
        }
    } else {
        assert(stat == RNKF_PR_RNK1_FINISHED || stat == RNKF_PR_RNK2_FINISHED);
        if (stat == RNKF_PR_RNK1_FINISHED) {
            if (tc->len1 > tc->len2) {
                fprintf(stderr, "Rank 2, of depth %d, should have finished "
                  "first, but Rank 1, of depth %d, did so\n", tc->len2,
                  tc->len1);
                return 1;
            } 
        } else {
            assert(stat == RNKF_PR_RNK2_FINISHED);
            if (tc->len1 < tc->len2) {
                fprintf(stderr, "Rank 1, of depth %d, should have finished "
                  "first, but Rank 2, of depth %d, did so\n", tc->len1,
                  tc->len2);
                return 1;
            }
        }
        for (; i < tc->len1 || i < tc->len2; i++) {
            if ( (stat = rnkf_pr_cont_next(&rfp, &entry)) != RNKF_PR_SUCCESS) {
                fprintf(stderr, "Test case %d: error reading continued "
                  "rank %d; return code %d\n", tc_num, i, stat);
                return 1;
            }
            if (tc->len1 > tc->len2) {
                if (strcmp(entry, tc->r1[i]) != 0) {
                    fprintf(stderr, "Continued rank %d, first ranking, "
                      "expected '%s', got '%s'\n", i, tc->r1[i], entry);
                    return 1;
                }
            } else {
                if (strcmp(entry, tc->r2[i]) != 0) {
                    fprintf(stderr, "Continued rank %d, second ranking, "
                      "expected '%s', got '%s'\n", i, tc->r2[i], entry);
                    return 1;
                }
            }
        }
        stat = rnkf_pr_cont_next(&rfp, &entry);
        if (stat != RNKF_PR_BOTH_FINISHED) {
            fprintf(stderr, "Test case %d: expected end of both rankings "
              "at %d, but received status code %d\n", tc_num, i, stat);
            return 1;
        }
    }
    return 0;
}

int main(void) {
    int num_testcases = sizeof(testcases) / sizeof(testcases[0]);
    int t;
    int error = 0;

    for (t = 0; t < num_testcases; t++) {
        error += do_test_case(&testcases[t], t);
    }
    return error;
}
