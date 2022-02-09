#ifndef TREC_H
#define TREC_H

#include <stdio.h>
#include "kvec.h"

/* Maximum number of topics for TREC run input. */
#define MAX_NUM_TREC_TOPICS 10000

/* Maximum TREC topic id length. */
#define MAX_TREC_TOPIC_ID_LEN 10

/* Maximum length of a TREC input line. */
#define MAX_TREC_LINE_LENGTH 2048

/* Struct holding score for TREC topic */
typedef struct trec_score {
    char topic_id[MAX_TREC_TOPIC_ID_LEN + 1];
    double score;
} trec_score_t;

/* Interesting information in a TREC line. */
typedef struct trec_entry {
    char * tpc;
    char * doc;
    double sim;
} trec_entry_t;

/* Entries for a given topic */
typedef struct trec_topic {
    char * tpc;
    kvec_t(trec_entry_t) entries;
} trec_topic_t;

/* Parser for TREC run file */
typedef struct trec_parser {
    FILE * fp;
    char buf[MAX_TREC_LINE_LENGTH];
    int buf_sz;
    int unparsed;
    trec_entry_t entry;
} trec_parser_t;

/* Initialize a TREC run parser. */
void trec_parser_init(trec_parser_t * parser, FILE * fp);

/* Parse the topic id and document id from the next TREC run entry. 

   The user must provide a working buffer, BUF.  The returned pointers
   point into this buffer.  
 
   Returns 1 on success, 0 on EOF, -1 on error. */
int trec_parse_entry(trec_parser_t * parser, trec_entry_t * entry);

/* Parse entries for next topic.  */
int trec_parse_tpc_entries(trec_parser_t * parser, trec_topic_t * tpc);

/* Unparse an entry; make it available for the next parse. */
int trec_parse_unparse(trec_parser_t * parser);

/* Initialize a TREC topic object. */
void trec_topic_init(trec_topic_t * topic);

/* Number of entries in TREC topic object. */
int trec_topic_len(trec_topic_t * topic);

/* Get a topic entry at a given index. */
trec_entry_t * trec_topic_entry(trec_topic_t * topic, int i);

/* Comparator between entries: by score first, then reverse alphabetic doc. */
int trec_entry_cmp(const void * v_a, const void * v_b);

/* Clear a TREC topic object. */
void trec_topic_clear(trec_topic_t * topic);

#endif /* TREC_H */
