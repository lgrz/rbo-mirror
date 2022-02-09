#include "args.h"
#include "log.h"
#include <stdlib.h>

void args_init(args_t * args) {
    args->p = DEFAULT_P;
    args->rnkfile = NULL;
    args->rnkfile1 = NULL;
    args->rnkfile2 = NULL;
    args->metric = METRIC_RBO;
    args->trec = 0;
}

int args_parse(args_t * args, int argc, const char * argv[]) {
    int a;
    int p_set = 0;
    args_stat_t status = ARGS_UNSET;

    for (a = 1; a < argc && argv[a][0] == '-'; a++) {
	char * endp;
	switch (argv[a][1]) {
	case 'p':
	    if (a == argc - 1) {
		log_error("Option '-p' requires an argument\n");
		status = ARGS_ERROR;
		break;
	    }
            a++;
            p_set = 1;
	    args->p = strtod(argv[a], &endp);
	    if (*endp != '\0') {
		log_error("Invalid float value '%s' as argument to '-p'\n",
		  argv[a]);
		status = ARGS_ERROR;
		break;
	    }
	    if (args->p < 0.0 || args->p >= 1.0) {
		log_error("The '-p' parameter must be 0.0 <= p < 1.0; given "
		  "value is '%f'\n", args->p);
		status = ARGS_ERROR;
		break;
	    }
	    break;
        case 'm':
            if (a == argc - 1) {
                log_error("Option '-m' requires an argument\n");
                status = ARGS_ERROR;
                break;
            }
            a++;
            if ( (args->metric = metric_from_name(argv[a])) 
              == METRIC_UNKNOWN) {
                log_error("Unknown metric '%s'\n", argv[a]);
                status = ARGS_ERROR;
                break;
            }
            break;
        case 't':
            args->trec = 1;
            break;
	case 'h':
	    args_usage(argv[0], 0);
	    return ARGS_HELP;
	default:
	    log_error("Unknown option '%s'\n", argv[a]);
	    status = ARGS_ERROR;
	    break;
	}
    }
    if (a == argc - 2) {
        /* Two rank files specified */
        args->rnkfile1 = argv[a];
        args->rnkfile2 = argv[a + 1];
        if (status != ARGS_ERROR) {
            status = ARGS_TWO_FILES;
        }
    } else if (a == argc - 1) {
        if (args->trec) {
            log_error("TREC input must be in two run files\n");
        } else {
            /* A single, CSV file specified */
            log_error("CSV support is currently disabled\n");
        }
        status = ARGS_ERROR; 
        /* args->rnkfile = argv[a];
        if (status != ARGS_ERROR) {
            status = ARGS_ONE_FILE;
        } */
    } else {
	log_error("Requires either single ranking file or ranking file pair\n");
	status = ARGS_ERROR;
    } 

    if (p_set && args->metric != METRIC_RBO) {
        log_error("Argument -p can only be used with RBO metric\n");
        status = ARGS_ERROR;
    }

    if (args->trec && args->metric != METRIC_RBO) {
        log_error("Only RBO currently supported for TREC input (-t)\n");
        status = ARGS_ERROR;
    }

    if (status == ARGS_ERROR) {
	args_usage(argv[0], 1);
    }
    return status;
}

void args_usage(const char * progname, int is_error) {
    if (is_error) {
	log_error(USAGE_STRING, progname);
    } else {
	log_message(USAGE_STRING, progname);
    }
}
