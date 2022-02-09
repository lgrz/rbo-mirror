#ifndef UTIL_H
#define UTIL_H

#include "strarr.h"
#include <stdio.h>

#define MAX_LINE_LEN (10 * 1024)

/**
 *  Open a file in read mode, or exit with error on failure.
 */
FILE * open_rd_or_exit(const char * fname);

/**
 *  Read the lines of a file into an array of strings.
 *
 *  Newlines are stripped, but not other whitespace.
 *
 *  Lines must not be longer than MAX_LINE_LEN.
 */
strarr_t * read_lines(FILE * fp);

#endif /* UTIL_H */
