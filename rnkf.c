#include <stdio.h>
#include <stdlib.h>
#include "rnkf.h"

int rnkf_init(rnkf_t * rf, FILE * fp) {
    rf->fp = fp;
    rf->buflen = RNKF_BUF_SIZE;
    rf->lineno = 0;
    return 0;
}

int rnkf_next(rnkf_t * rf, char ** e1, char ** e2) {
    char * cp;
    if (fgets(rf->buf, rf->buflen, rf->fp) == 0) {
        return 0;
    }
    *e1 = rf->buf;
    for (cp = rf->buf; *cp != ',' && *cp != '\n' && *cp != '\0'; cp++) 
        ;
    if (*cp != ',') {
        return -1;
    }
    *cp = '\0';
    cp++;
    *e2 = cp;
    for ( ; *cp != '\n' && *cp != '\0'; cp++)
        ;
    if (*cp == '\0') {
        return -1;  /* line longer than buffer */
    }
    *cp = '\0';
    rf->lineno++;
    return 1;
}

int rnkf_get_lineno(rnkf_t * rf) {
    return rf->lineno;
}

void rnkf_destroy(rnkf_t * rf, int close_fp) {
    if (close_fp) {
        fclose(rf->fp);
    }
    rf->fp = NULL;
}
