/**
 *  Test the strarr class.
 */
#include "../strarr.h"
#include "tutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct strarr_tc {
    unsigned int num_str;
    unsigned int max_len;
    const char * pattern;
    unsigned int patlen;
};

/**
 *  Check that the contents of a file are consistent with a test case.
 *
 *  Returns a count of the number of errors, 0 if there are none.
 */
static int test_file_against_tc(struct strarr_tc * tc, FILE * fp,
  unsigned int * slens);

/**
 *  Test a test case.
 */
static int do_test_case(struct strarr_tc * tc);

int main(void) {
    struct strarr_tc testcases [] = {
        {
        .num_str = 2000,
        .max_len = 10000,
        .pattern = "abcde",
        .patlen = 5
        }
    };
    int num_tc = sizeof(testcases) / sizeof(testcases[0]);
    int t;
    int total_errors = 0;

    for (t = 0; t < num_tc; t++) {
        int errors;

        errors = do_test_case(&testcases[t]);
        if (errors != 0) {
            fprintf(stderr, "%d errors on test case %d\n", errors, t);
        }
        if (errors < 0) {
            return errors;
        }
        total_errors += errors;
    }
    return total_errors;
}

static int test_file_against_tc(struct strarr_tc * tc, FILE * fp,
  unsigned int * slens) {
    char * str, * buf;
    int errors = 0;
    int i;
    strarr_t * sa;

    buf = malloc(tc->max_len + 2);
    if (buf == NULL) {
        return -1;
    }
    str = malloc(tc->max_len + 2);
    if (str == NULL) {
        return -1;
    }
    sa = strarr_new();

    for (i = 0; i  < tc->num_str; i++) {
        fgets(buf, tc->max_len + 2, fp);
        buf[strlen(buf) - 1] = '\0';
        strarr_add(sa, buf);
    }
    for (i = 0; i  < tc->num_str; i++) {
        gen_str(str, slens[i], tc->pattern, tc->patlen);
        if (strcmp(STRARR_GET(sa, i), str) != 0) {
            errors += 1;
        }
    }
    free(buf);
    free(str);
    return errors;
}

static int do_test_case(struct strarr_tc * tc) {
    FILE * fp;
    unsigned int * slens;
    int errors;

    slens = malloc(sizeof(*slens) * tc->num_str);
    if (slens == NULL) {
        return -1;
    }

    fp = gen_str_to_file(tc->num_str, tc->max_len, tc->pattern, tc->patlen,
      slens);
    if (fp == NULL) {
        free(slens);
        return -1;
    }

    errors = test_file_against_tc(tc, fp, slens);

    free(slens);
    fclose(fp);
    return errors;
}
