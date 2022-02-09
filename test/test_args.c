/**
 *  Test the args implementation.
 */

#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "../metric.h"
#include "../args.h"
#include "../log.h"

#define MAX_TEST_ARGS 10

// XXX test using -p option on non-RBO metric

struct args_test {
    int argc;
    const char * argv[MAX_TEST_ARGS];
    args_stat_t status;
    float p; 
    char * rnkfile, * rnkfile1, * rnkfile2;
    const char * expl;
    metric_enum_t metric;
};

static void cat_file(FILE * fp, FILE * out) {
#define CAT_BF_SZ 1024
    char buf[CAT_BF_SZ];
    size_t nread;
    if (fseek(fp, 0, SEEK_SET) < 0) {
        fprintf(stderr, "Can't rewind file!\n");
        return;
    }
    nread = fread(buf, 1, CAT_BF_SZ,  fp);
    fwrite(buf, 1, nread, out);
}

static void print_args(FILE * stream, int argc, const char * argv[]) {
    int i;
    for (i = 0; i < argc; i++) {
        fputs(argv[i], stream);
        if (i < argc - 1)
            fputc(' ', stream);
    }
}

static void failed(struct args_test * test, char * fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "TEST FAILED: %s: '", test->expl);
    print_args(stderr, test->argc, test->argv);
    fprintf(stderr, "': ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

static int do_test(struct args_test * test) {
    struct args args;
    args_stat_t status;
    int errors = 0;
    FILE * errf = tmpfile();
    FILE * msgf = tmpfile();

    args_init(&args);
    log_set_error_stream(errf);
    log_set_message_stream(msgf);
    status = args_parse(&args, test->argc, test->argv);
    if (test->status == ARGS_ERROR) {
        if (status != ARGS_ERROR) {
            failed(test, "non-negative status: '%d'", status);
            errors++;
        }
        if (ftell(errf) == 0) {
            failed(test, "no error message");
            errors++;
        }
        if (ftell(msgf) != 0) {
            failed(test, "message output");
            errors++;
        }
    } else if (test->status == ARGS_HELP) {
        if (status != ARGS_HELP) {
            failed(test, "status != %d: '%d'", ARGS_HELP, status);
            errors++;
        }
        if (ftell(msgf) == 0) {
            failed(test, "no help message");
            errors++;
        }
        if (ftell(errf) != 0) {
            failed(test, "error output");
            cat_file(errf, stderr);
            errors++;
        }
    } else {
        assert(test->status == ARGS_ONE_FILE 
          || test->status == ARGS_TWO_FILES);
        if (status != test->status) {
            failed(test, "status != %d: '%d'", test->status, status);
            errors++;
        }
        if (ftell(msgf) != 0) {
            failed(test, "message output");
            errors++;
        }
        if (ftell(errf) != 0) {
            failed(test, "error output");
            cat_file(errf, stderr);
            errors++;
        }
        if (args.p != test->p) {
            failed(test, "expected p value of '%f'; got '%f'", test->p,
              args.p);
            errors++;
        }
        if (args.metric != test->metric) {
            failed(test, "expected metric of '%d'; got '%d'", test->metric,
              args.metric);
            errors++;
        }
        if (test->status == ARGS_ONE_FILE) {
            if (args.rnkfile == NULL 
              || strcmp(args.rnkfile, test->rnkfile) != 0) {
                failed(test, "expected rnkfile of '%s'; got '%s'", 
                  test->rnkfile, args.rnkfile);
                errors++;
            }
        } else { 
            assert(test->status == ARGS_TWO_FILES);
            if (args.rnkfile1 == NULL 
              || strcmp(args.rnkfile1, test->rnkfile1) != 0) {
                failed(test, "expected rnkfile one of '%s'; got '%s'", 
                  test->rnkfile1, args.rnkfile1);
                errors++;
            }
            if (args.rnkfile2 == NULL 
              || strcmp(args.rnkfile2, test->rnkfile2) != 0) {
                failed(test, "expected rnkfile two of '%s'; got '%s'", 
                  test->rnkfile2, args.rnkfile2);
                errors++;
            }
        }
    }
    fclose(errf);
    fclose(msgf);
    return errors;
}

int main(void) {
    struct args_test test_cases[] = {
	/* Correct cases */
/*	{ 
            .expl = "correct, one file, no p",
	    .argv = {"test_args", "foo"}, .argc = 2,
	    .status = ARGS_ONE_FILE, .p = DEFAULT_P, .rnkfile = "foo",
            .metric = METRIC_RBO
	},
	{
            .expl = "correct, one file, with p",
	    .argv = {"test_args", "-p", "0.8", "bar"}, .argc = 4,
	    .status = ARGS_ONE_FILE, .p = 0.8, .rnkfile = "bar",
            .metric = METRIC_RBO
	}, */
	{ 
            .expl = "correct, two files, no p",
	    .argv = {"test_args", "foo", "bar"}, .argc = 3,
	    .status = ARGS_TWO_FILES, .p = DEFAULT_P, .rnkfile1 = "foo",
            .rnkfile2 = "bar",
            .metric = METRIC_RBO
	},
	{
            .expl = "correct, two files, with p",
	    .argv = {"test_args", "-p", "0.8", "bar", "baz"}, .argc = 5,
	    .status = ARGS_TWO_FILES, .p = 0.8, .rnkfile1 = "bar",
            .rnkfile2 = "baz",
            .metric = METRIC_RBO
	},
	{
            .expl = "correct",
	    .argv = {"test_args", "-h"}, .argc = 2,
	    .status = ARGS_HELP, .p = DEFAULT_P, .rnkfile = NULL,
            .metric = METRIC_RBO
	},
	{ 
            .expl = "correct, two files, AO",
	    .argv = {"test_args", "-m", "ao", "foo", "bar"}, .argc = 5,
	    .status = ARGS_TWO_FILES, .p = DEFAULT_P, .rnkfile1 = "foo",
            .rnkfile2 = "bar",
            .metric = METRIC_AO
	},
        /* Incorrect cases */
	{
            .expl = "no rank file",
	    .argv = {"test_args"}, .argc = 1,
	    .status = ARGS_ERROR
	},
        {
            .expl = "no argument to p",
	    .argv = {"test_args", "-p"}, .argc = 2,
	    .status = ARGS_ERROR
	},
	{
            .expl = "malformed option",
	    .argv = {"test_args", "-", "baz"}, .argc = 3,
	    .status = ARGS_ERROR
	},
	{
	    .expl = "no proper argument to p",
	    .argv = {"test_args", "-p", "baz"}, .argc = 3,
	    .status = ARGS_ERROR
	},
        {
            .expl = "argument to p >= 1.0",
	    .argv = {"test_args", "-p", "1.1", "foo"}, .argc = 4,
	    .status = ARGS_ERROR
        },
        {
	    .expl = "argument to p < 0.0",
	    .argv = {"test_args", "-p", "-0.1", "foo"}, .argc = 4,
	    .status = ARGS_ERROR
        },
        {
            .expl = "-p used with non-rbo metric",
	    .argv = {"test_args", "-m", "ao", "-p", "0.8", "foo", "bar"}, 
            .argc = 7,
            .status = ARGS_ERROR
        }
    };
    int ntests = sizeof(test_cases) / sizeof(test_cases[0]);
    int t;

    for (t = 0; t < ntests; t++) {
        do_test(&test_cases[t]);
    }
    return 0;
}
