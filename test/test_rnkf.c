/**
 *  Test the rnkfile reader
 */
#include "../rnkf.h"
#include <stdio.h>
#include <string.h>

#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define MAX_RNKLEN 1024

struct rnkf_tc {
    char * r1 [MAX_RNKLEN];
    char * r2 [MAX_RNKLEN];
    int len1;
    int len2;
};

static struct rnkf_tc testcases[] = {
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
};

static int do_test_case(struct rnkf_tc * tc) {
    FILE * tmpf = tmpfile();
    int maxlen = MAX(tc->len1, tc->len2);
    rnkf_t rf;
    char * e1, * e2;
    int i;
    for (i = 0; i < maxlen; i++) {
        if (i < tc->len1) {
            fputs(tc->r1[i], tmpf);
        }
        putc(',', tmpf);
        if (i < tc->len2) {
            fputs(tc->r2[i], tmpf);
        }
        putc('\n', tmpf);
    }
    fseek(tmpf, 0, SEEK_SET);
    rnkf_init(&rf, tmpf);
    for (i = 0; i < maxlen; i++) {
        if (rnkf_next(&rf, &e1, &e2) <= 0) {
            fprintf(stderr, "Error reading line %d of rnkfile\n", rf.lineno);
            rnkf_destroy(&rf, 1);
            return 1;
        }
        if (i < tc->len1) {
            if (strcmp(e1, tc->r1[i]) != 0) {
                fprintf(stderr, "Line %d of rank file, first ranking," 
                  " expected '%s', got '%s'\n", rf.lineno, tc->r1[i], e1);
                rnkf_destroy(&rf, 1);
                return 1;
            }
        } else if (strcmp(e1, "") != 0) {
            fprintf(stderr, "Line %d of rank file, first ranking,"
              " should be empty, got '%s'\n", rf.lineno, e1);
            rnkf_destroy(&rf, 1);
            return 1;
        }
        if (i < tc->len2) {
            if (strcmp(e2, tc->r2[i]) != 0) {
                fprintf(stderr, "Line %d of rank file, second ranking," 
                  " expected '%s', got '%s'\n", rf.lineno, tc->r2[i], e2);
                rnkf_destroy(&rf, 1);
                return 1;
            }
        } else if (strcmp(e2, "") != 0) {
            fprintf(stderr, "Line %d of rank file, first ranking,"
              " should be empty, got '%s'\n", rf.lineno, e2);
            rnkf_destroy(&rf, 1);
            return 1;
        }
    }
    if (rnkf_next(&rf, &e1, &e2) != 0) {
        fprintf(stderr, "Rnkfile should be finished after line %d,"
          " but more lines\n", rf.lineno);
        rnkf_destroy(&rf, 1);
        return 1;
    }
    rnkf_destroy(&rf, 1);
    return 0;
}

int main(void) {
    int num_testcases = sizeof(testcases) / sizeof(testcases[0]);
    int t;
    int error = 0;

    for (t = 0; t < num_testcases; t++) {
        error += do_test_case(&testcases[t]);
    }
    return error;
}
