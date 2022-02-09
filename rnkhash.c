#include <assert.h>
#include "rnkhash.h"
#include "khash.h"

KHASH_MAP_INIT_STR(RNK_HASH, int);

typedef khash_t(RNK_HASH) * T;

rnkhash_t * create_rnkhash(void) {
    return (rnkhash_t *) kh_init(RNK_HASH);
}

int rnkhash_insert(rnkhash_t * rh, const char * key, int rnk) {
    T trh = (T) rh;
    int ret;
    khiter_t k;

    assert(rnk >= 0);
    k = kh_put(RNK_HASH, trh, key, &ret);
    if (!ret) {
        return ret;
    }
    kh_value(trh, k) = rnk;
    return ret;
}

int rnkhash_lookup(rnkhash_t * rh, const char * key) {
    T trh = (T) rh;
    khiter_t k;

    k = kh_get(RNK_HASH, trh, key);
    if (k == kh_end(trh)) {
        return -1;
    } else {
        return kh_value(trh, k);
    }
}

void rnkhash_destroy(rnkhash_t * rh) {
    kh_destroy(RNK_HASH, (T) rh);
}
