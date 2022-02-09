#ifndef ARGS_H
#define ARGS_H

#include "metric.h"

/**
 *  Process command-line options and arguments.
 */

#define USAGE_STRING "USAGE: %s [-m metric] [-p <p-value>] <rnk1> <rnk2>\n"

#define DEFAULT_P        0.98
#define ARGS_ERR_BUF_SZ  1024

typedef struct args {
    float p;               /* Persistence parameter */
    const char * rnkfile;  /* The CSV file to read rankings from */
    const char * rnkfile1, * rnkfile2; /* The two rank files to read from */
    metric_enum_t metric;  /* The metric chosen */
    int trec;              /* Input files are TREC run files */
} args_t;

/**
 *  Argument parse status.
 */
typedef enum args_stat {
    ARGS_HELP = 0,         /* User has asked for help */
    ARGS_ERROR = -1,       /* Arguments incorrectly specified */
    ARGS_ONE_FILE = 1,     /* User has specified a single rank file */
    ARGS_TWO_FILES = 2,    /* User has specified two rank files */
    ARGS_UNSET
} args_stat_t;

/**
 *  Initialize an arg structure, and set default values.
 */
void args_init(args_t * args);

/**
 *  Process command-line arguments.
 *
 *  Returns 0 on success, a negative number on failure, 1 if help
 *  was requested and a help message printed.
 */
args_stat_t args_parse(args_t * args, int argc, const char * argv[]);

/**
 *  Print the program usage message.
 *
 *  Does _not_ exit from the program.
 */
void args_usage(const char * progname, int is_error);

#endif /* ARGS_H */
