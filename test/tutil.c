#include "tutil.h"
#include <stdlib.h>

void mk_rnk_f(FILE * fp, char * rnk[], int len) {
    int l;

    for (l = 0; l < len; l++) {
        fputs(rnk[l], fp);
        putc('\n', fp);
    }
    fseek(fp, 0, SEEK_SET);
}

void gen_str(char * buf, unsigned int len, const char * pattern, 
  unsigned int patlen) {
    unsigned int c;
    for (c = 0; c < len; c++) {
        buf[c] = pattern[c % patlen];
    }
    buf[c] = '\0';
}

FILE * gen_str_to_file(unsigned int num_str, unsigned int max_len,
  const char * pattern, unsigned int patlen, unsigned int * slens) {
    unsigned int i;
    char * buf;
    FILE * fp;

    fp = tmpfile();

    buf = malloc(max_len + 2);
    if (buf == NULL) {
        return NULL;
    }

    for (i = 0; i < num_str; i++) {
        int len;

        if (i % 7 == 0) {
            len = 0;  /* make sure there are some zero-length strings. */
        } else {
            len = rand() % max_len;
        }
        gen_str(buf, len, pattern, patlen);
        slens[i] = len;
        fprintf(fp, "%s\n", buf);
    }
    rewind(fp);
    free(buf);
    return fp;
}
