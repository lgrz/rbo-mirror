#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "strarr.h"

#define STRARR_INIT_BUF_SZ (1024)
#define STRARR_INIT_STR_SZ (128)

strarr_t * strarr_new(void) {
    strarr_t * sa;

    sa = malloc(sizeof(*sa));
    if (sa == NULL) {
        return NULL;
    }

    sa->str = malloc(sizeof(*sa->str) * STRARR_INIT_STR_SZ);
    if (sa->str == NULL) {
        free(sa);
        return NULL;
    }
    sa->str_num = 0;
    sa->str_sz = STRARR_INIT_STR_SZ;

    sa->buf = malloc(sizeof(*sa->buf) * STRARR_INIT_BUF_SZ);
    if (sa->buf == NULL) {
        free(sa->str);
        free(sa);
        return NULL;
    }
    sa->buf_used = 0;
    sa->buf_sz = STRARR_INIT_BUF_SZ;
    return sa;
}

int strarr_add(strarr_t * sa, const char * str) {
    unsigned int slen;

    slen = strlen(str);
    if (sa->str_num == sa->str_sz) {
        unsigned int new_sz;
        unsigned int * new_str;

        new_sz = sa->str_sz * 2;
        assert(new_sz > sa->str_sz); /* check wraparound */
        new_str = realloc(sa->str, sizeof(*sa->str) * new_sz);
        if (new_str == NULL) {
            return -1;
        }
        sa->str = new_str;
        sa->str_sz = new_sz;
    }
    assert(sa->str_num < sa->str_sz);

    while (sa->buf_used + slen + 1 > sa->buf_sz) {
        unsigned int new_sz;
        char * new_buf;

        new_sz = sa->buf_sz * 2;
        assert(new_sz > sa->buf_sz); /* check wraparound */
        new_buf = realloc(sa->buf, sizeof(*sa->buf) * new_sz);
        if (new_buf == NULL) {
            return -1;
        }
        sa->buf = new_buf;
        sa->buf_sz = new_sz;
    }
    assert(sa->buf_used + slen + 1 <= sa->buf_sz);

    strcpy(sa->buf + sa->buf_used, str);
    sa->str[sa->str_num] = sa->buf_used;
    sa->str_num++;
    sa->buf_used += slen + 1;
    return 0;
}

void strarr_delete(strarr_t ** sa_p) {
    strarr_t * sa = *sa_p;

    free(sa->str);
    free(sa->buf);
    free(sa);
    *sa_p = NULL;
}
