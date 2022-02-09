#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "log.h"

FILE * open_rd_or_exit(const char * fname) {
    FILE * fp;

    if ( (fp = fopen(fname, "r")) == NULL) {
        log_error("Failed to open rank file '%s' for reading: %s\n",
          fname, strerror(errno));
        exit(-1);
    }
    return fp;
}

strarr_t * read_lines(FILE * fp) {
    strarr_t * sa;
    char buf[MAX_LINE_LEN];

    sa = strarr_new();
    if (sa == NULL) {
        return NULL;
    }
    while ( (fgets(buf, MAX_LINE_LEN, fp)) != NULL) {
        int i;

        for (i = 0; buf[i] != '\n' && buf[i] != '\0'; i++) 
            ;
        if (buf[i] != '\n') {
            log_error("Line too long: '%s'\n", buf);
            free(sa);
            return NULL;
        }
        buf[i] = '\0';
        if (strarr_add(sa, buf) < 0) {
            log_error("Out of memory adding line to string array\n");
        }
    }
    return sa;
}
