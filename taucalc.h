#ifndef TAUCALC_H
#define TAUCALC_H

#include <stdio.h>

#define TC_OUT_OF_MEM -2.0
#define TC_UNEVEN_LISTS -3.0

/**
 *  Calculate Kendall's $\tau$ from two files.
 */
double taucalc_from_files(FILE * fp1, FILE * fp2);

#endif /* TAUCALC_H */
