#include "sset.h"

#include <string.h>

#define USE_KHASH

#ifdef USE_KHASH
#include "khash.h"

KHASH_SET_INIT_STR(STR_HASH);

typedef khash_t(STR_HASH) * T;

sset * create_sset(unsigned int minsize) {
    return (sset *) kh_init(STR_HASH);
}

int sset_insert(sset * ss, char * str) {
    int ret;
    kh_put(STR_HASH, (T) ss, str, &ret);
    return ret;
}

enum sset_haskey sset_search(sset * ss, const char * str) {
    T tss = (T) ss;
    khiter_t k;
    k = kh_get(STR_HASH, tss, str);
    if (k == kh_end(tss)) {
        return SS_HASNT_KEY;
    } else {
        return SS_HAS_KEY;
    }
}

enum sset_haskey sset_remove(sset * ss, const char * str) {
    T tss = (T) ss;
    khiter_t k;
    k = kh_get(STR_HASH, tss, str);
    if (k == kh_end(tss)) {
        return SS_HASNT_KEY;
    } else {
        free((void *) kh_key(tss, k));
        kh_del(STR_HASH, tss, k);
        return SS_HAS_KEY;
    }
}

void sset_destroy(sset * ss) {
    khiter_t k;
    for (k = kh_begin((T) ss); k != kh_end((T) ss); ++k) {
        if (kh_exist((T) ss, k)) {
            free((void *) kh_key((T) ss, k));
        }
    }
    kh_destroy(STR_HASH, (T) ss);
}

#else /* USE_KHASH */

#include "hashset.h"

static unsigned int hash(void * str);
static int streq(void * s1, void * s2);

sset * create_sset(unsigned int minsize) {
    return (sset *) create_hashset(minsize, hash, streq);
}

int sset_insert(sset * ss, char * str) {
    return hashset_insert((struct hashset *) ss, (void *) str);
}

enum sset_haskey sset_search(sset * ss, const char * str) {
    return hashset_search((struct hashset *) ss, (void *) str) == HAS_KEY ?
      SS_HAS_KEY : SS_HASNT_KEY; 
}

enum sset_haskey sset_remove(sset * ss, const char * str) {
    return hashset_remove((struct hashset *) ss, (void *) str) == HAS_KEY ?
      SS_HAS_KEY : SS_HASNT_KEY;
}

void sset_destroy(sset * ss) {
    hashset_destroy((struct hashset *) ss);
}

/**
 *  String hash function.
 *
 *  Taken from http://www.cse.yorku.ca/~oz/hash.html
 */
static unsigned int hash(void * v) {
    unsigned long hash = 0;
    int c;
    char * str = (char *) v;

    while ( (c = *str++) != '\0')
        hash = hash * 65599 + c;

    return hash;
}

/**
 *  Are two strings equal?
 */
static int streq(void * v1, void * v2) {
    char * s1 = (char *) v1;
    char * s2 = (char *) v2;
    return strcmp(s1, s2) == 0;
}

#endif /* USE_KHASH */
