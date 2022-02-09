#include "log.h"
#include <stdarg.h>

static FILE * error_fp = NULL;
static FILE * message_fp = NULL;
static int streams_inited = 0;

static int warning_output_on = 1;

static void init_streams(void) {
    if (streams_inited == 0) {
	error_fp = stderr;
	message_fp = stdout;
	streams_inited = 1;
    }
}

void log_error(char * fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    init_streams();
    if (error_fp != NULL) {
        fprintf(error_fp, "ERROR: ");
        vfprintf(error_fp, fmt, ap);
    }
    va_end(ap);
}

void log_warning(char * fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    init_streams();
    if (warning_output_on && error_fp != NULL) {
        fprintf(error_fp, "WARNING: ");
        vfprintf(error_fp, fmt, ap);
    }
    va_end(ap);
}

void log_message(char * fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    init_streams();
    if (message_fp != NULL) {
        vfprintf(message_fp, fmt, ap);
    }
    va_end(ap);
}

void log_set_error_stream(FILE * fp) {
    init_streams();
    error_fp = fp;
}

void log_set_message_stream(FILE * fp) {
    init_streams();
    message_fp = fp;
}

void log_set_warning_output_on(int on) {
    warning_output_on = on;
}
