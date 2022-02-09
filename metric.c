#include <string.h>
#include "metric.h"

metric_t metrics[] = {
    {
        .metric = METRIC_RBO,
        .name = "rank-biased overlap",
        .abbrev = "rbo",
    },
    {
        .metric = METRIC_AO,
        .name = "average overlap",
        .abbrev = "ao"
    },
    {
        .metric = METRIC_TAU,
        .name = "kendall's tau",
        .abbrev = "tau"
    }
};

int metrics_num = sizeof(metrics) / sizeof(metrics[0]);

metric_enum_t metric_from_name(const char * name) {
    int m;

    for (m = 0; m < metrics_num; m++) {
        if (strcasecmp(metrics[m].abbrev, name) == 0) {
            return metrics[m].metric;
        }
    }
    return METRIC_UNKNOWN;
}

metric_t * metric_from_id(metric_enum_t id) {
    int m;

    for (m = 0; m < metrics_num; m++) {
        if (metrics[m].metric == id) {
            return &metrics[m];
        }
    }
    return NULL;
}
