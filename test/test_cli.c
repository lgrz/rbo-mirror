/**
 *  Test the command-line version of the program.
 *
 *  This test requires the functions fork(),
 *  exec(), and pipe().
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "tutil.h"
#include "../metric.h"
#include "../log.h"

#define MAX_METRIC_RESULTS 16
#define MAX_RNKLEN 256
#define EXEC_NAME "rbo"
#define MAX_ARGS 16

struct metric_result {
    metric_enum_t metric;
    double score;
    const char * args[MAX_ARGS];
    int nargs;
    int retval;
};

struct test_case {
    char * r1 [MAX_RNKLEN];
    char * r2 [MAX_RNKLEN];
    int n1;
    int n2;
    struct metric_result met[MAX_METRIC_RESULTS];
    int nmet;
};

static struct test_case test_cases[] = {
    {
        .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" },
        .r2 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" },
        .n1 = 10,
        .n2 = 10,
        .met = {
            {
                .metric = METRIC_TAU,
                .score = 1.0,
                .args = {},
                .nargs = 0,
                .retval = 0
            },
            {
                .metric = METRIC_RBO,
                .score = 1.0,
                .args = {},
                .nargs = 0,
                .retval = 0
            },
            {
                .metric = METRIC_AO,
                .score = 1.0,
                .args = {},
                .nargs = 0,
                .retval = 0
            }
        },
        .nmet = 3
    },
    {
        .r1 = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", 
            "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", 
            "w", "x", "y", "z"},
        .r2 = { "g", "n", "u", "q", "c", "y", "m", "d", "k", "p", "w", 
            "l", "j", "x", "f", "r", "h", "a", "b", "z", "e", "t", "v", 
            "i", "o", "s"},
        .n1 = 26,
        .n2 = 26,
        .met = {
            {
                .metric = METRIC_TAU,
                .score = 0.015384,
                .args = {},
                .nargs = 0,
                .retval = 0
            },
            {
                .metric = METRIC_RBO,
                .score = 0.138488,
                .args = {"-p", "0.8"},
                .nargs = 2,
                .retval = 0
            },
            {
                .metric = METRIC_AO,
                .score = 0.506653,
                .args = {},
                .nargs = 0,
                .retval = 0
            }
        },
        .nmet = 3
    }
};

static int do_metric(struct metric_result * met, char * fname1, char * fname2) {
    const char *argv[MAX_ARGS];
    int a;
    pid_t pid;
    int errors = 0;
    int pipefd[2];
#define PIPE_READ 0
#define PIPE_WRITE 1

    argv[0] = EXEC_NAME;
    /* XXX test where the metric type is not applied. */
    argv[1] = "-m";
    argv[2] = metric_from_id(met->metric)->abbrev;
    for (a = 0; a < met->nargs; a++) {
        argv[3 + a] = met->args[a];
    }
    argv[3 + a] = fname1;
    argv[4 + a] = fname2;
    argv[5 + a] = NULL;

    /* XXX open pipe */
    if (pipe(pipefd) < 0) {
        log_error("Failed to open pipe: %s\n", strerror(errno));
        return -1;
    }

    pid = fork();
    if (pid == 0) {
        /* child */
        close(pipefd[PIPE_READ]);
        if (dup2(pipefd[PIPE_WRITE], STDOUT_FILENO) != STDOUT_FILENO) {
            log_error("Error redirecting stdout in child program: %s\n",
              strerror(errno));
            return -1;
        }
        if (execv(EXEC_NAME, (char **const) argv) < 0) {
            log_error("Failed to execute sub-program: %s\n", strerror(errno));
            return -1;
        }
    } else {
        /* parent */
        int status;
        FILE * res_in;
        double res;

        close(pipefd[PIPE_WRITE]);
        wait(&status);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                /* no error */
            } else {
                log_error("Child exited with error\n");
                return -1;
                /* error */
            }
        } else {
            log_error("Child did not exit normally\n");
            return -1;
        }
        res_in = fdopen(pipefd[PIPE_READ], "r");
        if (res_in == NULL) {
            log_error("Unable to open pipe input as FILE: %s\n", 
              strerror(errno));
            return -1;
        }
        if (fscanf(res_in, "%lf\n", &res) != 1) {
            log_error("Couldn't parse output of child process\n");
            return -1;
        }
        if (fabs(met->score - res) > 0.0001) {
            fprintf(stderr, "Score should be %lf, is %lf\n", met->score,
              res);
        }
        errors += 1;
    }
    return errors;
}

static int do_test_case(struct test_case * tc, int tc_num) {
    int fd1, fd2;
    FILE * fp1, * fp2;
    char fname1[] = "/tmp/rnktstXXXXXX";
    char fname2[] = "/tmp/rnktstXXXXXX";
    int m;
    int errors = 0;

    fd1 = mkstemp(fname1);
    fd2 = mkstemp(fname2);

    if (fd1 < 0 || fd2 < 0) {
        log_error("Call to mkstemp() failed");
        return -1;
    }

    fp1 = fdopen(fd1, "w");
    fp2 = fdopen(fd2, "w");

    if (fp1 == NULL || fp2 == NULL) {
        log_error("Call to fdopen() failed");
        return -1;
    }

    mk_rnk_f(fp1, tc->r1, tc->n1);
    mk_rnk_f(fp2, tc->r2, tc->n2);

    fclose(fp1);
    fclose(fp2);

    for (m = 0; m < tc->nmet; m++) {
        errors += do_metric(&tc->met[m], fname1, fname2);
    }

    remove(fname1);
    remove(fname2);
    return errors;
}

int main(void) {
    int t;
    int num_tc = sizeof(test_cases) / sizeof(test_cases[0]);

    for (t = 0; t < num_tc; t++) {
        do_test_case(&test_cases[t], t);
    }
    return 0;
}
