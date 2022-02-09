#ifndef RNKF_H
#define RNKF_H

#include <stdio.h>

// XXX change interface so that there are distinct read states
// for when both lists are present and for when only one list
// is being continue.  See a[] b{} c[] d(e)!

/**
 *  Read rank entries from a two-column rank file.
 *
 *  The format of a rank file is two entries per line, separated
 *  by a comma.  We XXX currently don't support quoting or escaping,
 *  so the entries themselves must not contain commas.  An entry
 *  may, however, be empty.
 */

#define RNKF_BUF_SIZE 1024  /* Maximum length of an input line */
#define RNKF_SEP ','        /* Separator between columns */

/**
 *  Rankfile state
 */
typedef struct rnkf {
    FILE * fp;
    char buf[RNKF_BUF_SIZE];
    int buflen;
    int lineno;
} rnkf_t;

/**
 *  Initialize a new rankfile.
 */
int rnkf_init(rnkf_t * rf, FILE * fp);

/**
 *  Read the next entry from a rankfile.
 *
 *  Returns 1 for success, 0 if at the end of file, -1 for format error.
 *
 *  @param e1 the first entry on the line.  "" if empty.
 *  @param e2 the second entry on the line. "" if empty.
 */
int rnkf_next(rnkf_t * rf, char ** e1, char ** e2);

/**
 *  Get the current line number of the input.
 */
int rnkf_get_lineno(rnkf_t * rf);

/**
 *  Close a rankfile and free resources.
 *
 *  @param close_fp 1 to close the internal FP, 0 to leave as is.
 */
void rnkf_destroy(rnkf_t * rf, int close_fp);

#endif /* RNKF_H */
