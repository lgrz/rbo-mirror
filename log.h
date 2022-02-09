#ifndef LOG_H
#define LOG_H

#include <stdio.h>

/**
 *  Output of error and other log messages.
 */

/**
 *  Output an error message.
 */
void log_error(char * fmt, ...);

/**
 *  Output a warning message.
 */
void log_warning(char * fmt, ...);

/**
 *  Output an actual to-the-user message.
 */
void log_message(char * fmt, ...);

/**
 *  Set the output stream for error messages.
 */
void log_set_error_stream(FILE * fp);

/**
 *  Set the output stream for plain messages.
 */
void log_set_message_stream(FILE * fp);

/**
 *  Set warning output on or off.
 */
void log_set_warning_output_on(int on);

#endif /* LOG_H */
