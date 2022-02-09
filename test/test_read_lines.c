#include "../util.h"
#include "../strarr.h"
#include "../log.h"
#include "tutil.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct rl_tc {
    unsigned int num_str;
    unsigned int max_len;
    const char * pattern;
    unsigned int patlen;
};

/**
 *  Execute a test case.
 *
 *  Returns the number of errors found, 0 if none.
 */
static int do_test_case(struct rl_tc * tc);

int main(void) {
    struct rl_tc testcases [] = {
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

static int do_test_case(struct rl_tc * tc) {
    FILE * fp;
    unsigned int * slens;
    int errors;
    strarr_t * sa;
    int i;
    char * buf;

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

    sa = read_lines(fp);
    if (sa == NULL) {
        free(slens);
        return -1;
    }

    assert(sa->str_num == tc->num_str);

    buf = malloc(tc->max_len + 2);
    if (buf == NULL) {
        free(slens);
        strarr_delete(&sa);
        return -1;
    }

    errors = 0;
    for (i = 0; i < sa->str_num; i++) {
        gen_str(buf, slens[i], tc->pattern, tc->patlen);
        if (strcmp(STRARR_GET(sa, i), buf) != 0) {
            errors += 1;
        }
    }
    free(buf);
    free(slens);
    strarr_delete(&sa);
    return errors;
}
