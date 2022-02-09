/**
 *  Test the rnkhash implementation.
 */
#include "../rnkhash.h"

#include <assert.h>

char * inserts[] = { "the", "cat", "sat", "matterly", "while", "we", "begat",
  "gnatterly" };
char * noninserts[] = { "my", "brown", "dog" };

int main(void) {
    rnkhash_t * rh;
    int i;
    int num_inserts = sizeof(inserts) / sizeof(inserts[0]);
    int num_noninserts = sizeof(noninserts) / sizeof(noninserts[0]);

    rh = create_rnkhash();
    for (i = 0; i < num_inserts; i++) {
        assert(rnkhash_lookup(rh, inserts[i]) < 0);
        assert(rnkhash_insert(rh, inserts[i], i) != 0);
    }
    for (i = 0; i < num_inserts; i++) {
        assert(rnkhash_lookup(rh, inserts[i]) == i);
    }
    for (i = 0; i < num_noninserts; i++) {
        assert(rnkhash_lookup(rh, noninserts[i]) < 0);
    }
    rnkhash_destroy(rh);
    return 0;
}
