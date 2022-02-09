#ifndef TUTIL_H
#define TUTIL_H

#include <stdio.h>

/**
 *  Make a rank file from an array of elements.
 */
void mk_rnk_f(FILE * fp, char * rnk[], int len);

/**
 *  Generate a string of a given length, by repeating a pattern.
 */
void gen_str(char * buf, unsigned int len, const char * pattern, 
  unsigned int patlen);

/**
 *  Generate a series of strings and write them to a file.
 *
 *  The returned file pointer will be readable, and will have been
 *  rewound to the beginning.
 *
 *  The length of each string is written to slens, which must be at
 *  least of size num_str.
 */
FILE * gen_str_to_file(unsigned int num_str, unsigned int max_len,
  const char * pattern, unsigned int patlen, unsigned int * slens);

#endif /* TUTIL_H */
