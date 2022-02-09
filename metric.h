#ifndef METRICS_H
#define METRICS_H

/**
 *  Enumeration of supported metrics.
 */
typedef enum {
    METRIC_RBO = 0,
    METRIC_AO = 1,
    METRIC_TAU = 2,
    METRIC_UNKNOWN = -1
} metric_enum_t;

/* Maximum number of alternate names a metric could have */
#define MAX_NUM_METRIC_NAMES 15

/**
 *  Information about a metric.
 */
typedef struct {
    metric_enum_t metric;
    const char * name;
    const char * abbrev;
} metric_t;

/**
 *  List of metrics.
 */
extern metric_t metrics[];

extern int metrics_num;

/**
 *  Decode a metric name into a metric enum.
 */
metric_enum_t metric_from_name(const char * name);

/**
 *  Get the metric info for a given enum.
 */
metric_t * metric_from_id(metric_enum_t id);

#endif /* METRICS_H */
