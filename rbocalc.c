#include "rbocalc.h"
#include "rnkf.h"
#include "rnkf_pr.h"
#include "rbo.h"
#include "log.h"
#include "kvec.h"
#include <assert.h>
#include <string.h>

double rbocalc_from_file(FILE * fp, double p) {
    rnkf_t rnkf;
    rbo_state_t rbo_st;
    int ret;
    double rbo;
    int e1_fin = 0, e2_fin = 0;
    char * e1, * e2;

    rnkf_init(&rnkf, fp);
    rbo_state_init(&rbo_st, p);

    while ( (ret = rnkf_next(&rnkf, &e1, &e2)) == 1) {
        if (e1_fin && e1[0] != '\0') {
            log_error("First ranking has restarted on line %d\n",
              rnkf_get_lineno(&rnkf));
            return -1.0;
        }
        if (e2_fin && e2[0] != '\0') {
            log_error("Second ranking has restarted on line %d\n",
              rnkf_get_lineno(&rnkf));
            return -1.0;
        }
        if (e1[0] == '\0' && e2[0] == '\0') {
            log_error("Both rankings are empty on line %d\n",
              rnkf_get_lineno(&rnkf));
            return -1.0;
        }
        if (!e1_fin && e1[0] == '\0') {
            rbo_mark_end_short(&rbo_st);
            e1_fin = 1;
        } else if (!e2_fin && e2[0] == '\0') {
            rbo_mark_end_short(&rbo_st);
            e2_fin = 1;
        }
        if (e1_fin) {
            rbo_state_update_uneven(&rbo_st, e2);
        } else if (e2_fin) {
            rbo_state_update_uneven(&rbo_st, e1);
        } else {
            rbo_state_update(&rbo_st, e1, e2);
        }
    }
    if (ret < 0) {
        log_error("Format error on line %d of rankings\n",
              rnkf_get_lineno(&rnkf));
        return -1.0;
    }
    rbo = rbo_calc_extrapolated(&rbo_st);

    rbo_state_clear(&rbo_st);
    rnkf_destroy(&rnkf, 0);  /* '1' mean "close the file" */
    return rbo;
}

double rbocalc_from_files(FILE * fp1, FILE * fp2, double p) {
    rnkf_pr_t rfp;
    rnkf_pr_stat_t stat;
    rbo_state_t rbo_st;
    char * e1, * e2, * entry;
    int rnk = 0;
    double rbo;

    rnkf_pr_init(&rfp, fp1, fp2);
    rbo_state_init(&rbo_st, p);

    while ( (stat = rnkf_pr_next(&rfp, &e1, &e2)) == RNKF_PR_SUCCESS) {
        rnk++;
        rbo_state_update(&rbo_st, e1, e2);
    }

    if (stat < 0) {
        goto ERROR;
    }

    if (stat == RNKF_PR_RNK1_FINISHED || stat == RNKF_PR_RNK2_FINISHED) {
        rbo_mark_end_short(&rbo_st);
        while ( (stat = rnkf_pr_cont_next(&rfp, &entry)) == RNKF_PR_SUCCESS) {
            rnk++;
            rbo_state_update_uneven(&rbo_st, entry);
        }
        if (stat < 0) {
            goto ERROR;
        }
    }
    rbo = rbo_calc_extrapolated(&rbo_st);

    goto END;

ERROR:
    if (stat == RNKF_PR_FORMAT_ERROR) {
        log_error("Format error at rank %d; line too long?\n", rnk);
        rbo = -1.0;
    } else if (stat == RNKF_PR_IO_ERROR) {
        log_error("Read error at rank %d\n", rnk);
        rbo = -1.0;
    }

END:
    rbo_state_clear(&rbo_st);
    rnkf_pr_destroy(&rfp, 0);
    return rbo;
}

int rbo_calc_from_trec_runs(FILE * fp1, FILE * fp2, double p,
  trec_score_t ** scores) {
    trec_parser_t parser1, parser2;
    kvec_t(trec_score_t) sc;

    kv_init(sc);
    trec_parser_init(&parser1, fp1);
    trec_parser_init(&parser2, fp2);

    while (1) {
        int ret, i;
        trec_topic_t topic1, topic2;
        trec_score_t score;
        rbo_state_t rbo_state;
        int len1, len2;

        trec_topic_init(&topic1);
        trec_topic_init(&topic2);

        ret = trec_parse_tpc_entries(&parser1, &topic1);
        if (ret < 0) {
            log_error("Input error\n");
            return -1;
        } else if (ret == 0) {
            break;
        }

        ret = trec_parse_tpc_entries(&parser2, &topic2);
        if (ret <= 0) {
            log_error("Second run file has more topics than first\n");
            return -1;
        }

        if (strcmp(topic1.tpc, topic2.tpc) != 0) {
            log_error("Run files differ in topics\n");
            return -1;
        }

        len1 = trec_topic_len(&topic1);
        len2 = trec_topic_len(&topic2);

        rbo_state_init(&rbo_state, p);

        for (i = 0; i < (len1 < len2 ? len1 : len2); i++) {
            trec_entry_t * entry1 = trec_topic_entry(&topic1, i);
            trec_entry_t * entry2 = trec_topic_entry(&topic2, i);
            rbo_state_update(&rbo_state, entry1->doc, entry2->doc);
        }

        rbo_mark_end_short(&rbo_state);
        for ( ; i < len1; i++) {
            trec_entry_t * entry = trec_topic_entry(&topic1, i);
            rbo_state_update_uneven(&rbo_state, entry->doc);
        }
        for ( ; i < len2; i++) {
            trec_entry_t * entry = trec_topic_entry(&topic2, i);
            rbo_state_update_uneven(&rbo_state, entry->doc);
        }

        score.score = rbo_calc_extrapolated(&rbo_state);
        strncpy(score.topic_id, topic1.tpc, MAX_TREC_TOPIC_ID_LEN);
        kv_push(trec_score_t, sc, score);

        rbo_state_clear(&rbo_state);

        trec_topic_clear(&topic1);
        trec_topic_clear(&topic2);
    }

    *scores = sc.a;

    return kv_size(sc);
}
