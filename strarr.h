#ifndef STRARR_H
#define STRARR_H

/**
 *  A data structure holding an array of strings.
 *
 *  The only modification operation supported is adding a string to 
 *  the end of the array.  All memory is managed internally.
 */

typedef struct strarr {
    unsigned * str;
    unsigned int str_num;
    unsigned int str_sz;
    char * buf;
    unsigned int buf_sz;
    unsigned int buf_used;
} strarr_t;

/**
 *  Create a new string array.
 *
 *  Returns NULL if out of memory.
 */
strarr_t * strarr_new(void);

/**
 *  Add a string to a string array.
 *
 *  Returns -1 if out of memory, 0 otherwise.
 */
int strarr_add(strarr_t * sa, const char * str);

/**
 *  Get the string at a certain offset.
 */
#define STRARR_GET(sa, i)  ((sa)->buf + (sa)->str[i])

/**
 *  Destroy a string array.
 */
void strarr_delete(strarr_t ** sa_p);

#endif /* STRARR_H */
