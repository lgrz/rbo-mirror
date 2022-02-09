#include "trec.h"

#include <string.h>
#include <stdlib.h>

void trec_parser_init(trec_parser_t * parser, FILE * fp) {
    parser->fp = fp;
    parser->buf[0] = '\0';
    parser->buf_sz = MAX_TREC_LINE_LENGTH;
    parser->unparsed = 0;
}

int trec_parse_entry(trec_parser_t * parser, trec_entry_t * entry) {
    char * tok_sptr, * sim_s, * eptr;

    if (parser->unparsed) {
        parser->unparsed = 0;
    } else {
        if (fgets(parser->buf, parser->buf_sz, parser->fp) == NULL) {
            return 0;
        }

        /* XXX feature test for strtok_r */
        parser->entry.tpc = strtok_r(parser->buf, " \t", &tok_sptr);
        if (parser->entry.tpc == NULL) {
            return -1;
        }
        if (strtok_r(NULL, " \t", &tok_sptr) == NULL) {
            return -1;
        }
        parser->entry.doc = strtok_r(NULL, " \t", &tok_sptr);
        if (parser->entry.doc == NULL) {
            return -1;
        }
        if (strtok_r(NULL, " \t", &tok_sptr) == NULL) {
            return -1;
        }
        sim_s = strtok_r(NULL, " \t", &tok_sptr);
        if (sim_s == NULL) {
            return -1;
        }
        parser->entry.sim = strtod(sim_s, &eptr);
        if (*eptr != '\0') {
            return -1;
        }
    }
    *entry = parser->entry;
    return 1;
}

int trec_parse_tpc_entries(trec_parser_t * parser, trec_topic_t * entries) {
    trec_entry_t entry;
    int ret;
    char * tpc = NULL;

    while ( (ret = trec_parse_entry(parser, &entry)) > 0) {
        if (tpc == NULL || strcmp(tpc, entry.tpc) == 0) {
            if (tpc == NULL) {
                tpc = strdup(entry.tpc);
                if (tpc == NULL) {
                    return -1;
                }
                entries->tpc = tpc;
            }
            entry.tpc = tpc;
            entry.doc = strdup(entry.doc);
            if (entry.doc== NULL) {
                return -1;
            }
            kv_push(trec_entry_t, entries->entries, entry);
        } else {
            trec_parse_unparse(parser);
            break;
        }
    }
    if (tpc == NULL) {
        return 0;
    } else {
        qsort(entries->entries.a, kv_size(entries->entries), 
          sizeof(trec_entry_t), trec_entry_cmp);
        return 1;
    }
}

int trec_parse_unparse(trec_parser_t * parser) {
    if (parser->unparsed) {
        return -1;
    }
    parser->unparsed = 1;
    return 1;
}

void trec_topic_init(trec_topic_t * topic) {
    topic->tpc = NULL;
    kv_init(topic->entries);
}

int trec_topic_len(trec_topic_t * topic) {
    return kv_size(topic->entries);
}

trec_entry_t * trec_topic_entry(trec_topic_t * topic, int i) {
    return &kv_A(topic->entries, i);
}

int trec_entry_cmp(const void * v_a, const void * v_b) {
    const trec_entry_t * e_a = (const trec_entry_t *) v_a;
    const trec_entry_t * e_b = (const trec_entry_t *) v_b;

    if (e_a->sim < e_b->sim) {
        return 1;
    } else if (e_a->sim > e_b->sim) {
        return -1;
    } else {
        /* following trec_eval */
        return -strcmp(e_a->doc, e_b->doc);
    }
}

void trec_topic_clear(trec_topic_t * topic) {
    int i;
    free(topic->tpc);
    for (i = 0; i < kv_size(topic->entries); i++) {
        free(kv_A(topic->entries, i).doc);
    }
    kv_destroy(topic->entries);
    topic->tpc = NULL;
}
