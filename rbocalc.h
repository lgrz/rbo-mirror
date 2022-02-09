#ifndef RBOCALC_H
#define RBOCALC_H

#include <stdio.h>

#include "trec.h"

/**
 *  Calculate RBO from an input file.
 */
double rbocalc_from_file(FILE * fp, double p);

/**
 *  Calculate RBO from two input files.
 */
double rbocalc_from_files(FILE * fp1, FILE * fp2, double p);

/**
 *  Calculate RBO between two TREC runs for each topic.
 *
 *  We assume that the same topics appear, in the same order, in
 *  both runs.
 */
int rbo_calc_from_trec_runs(FILE * fp1, FILE * fp2, double p,
  trec_score_t ** scores);

#endif /* RBOCALC_H */
