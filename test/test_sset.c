/**
 *  Test the stringset implementation.
 */
#include "../sset.h"
#include <assert.h>

#include <string.h>

#define MIN_SIZE 5

char * inserts[] = { "the", "cat", "sat", "matterly", "while", "we", "begat",
  "gnatterly" };
char * noninserts[] = { "my", "brown", "dog" };

int main(void) {
    sset * ss;
    int i;
    int num_inserts = sizeof(inserts) / sizeof(inserts[0]);
    int num_noninserts = sizeof(noninserts) / sizeof(noninserts[0]);

    ss = create_sset(MIN_SIZE);
    for (i = 0; i < num_inserts; i++) {
        assert(sset_search(ss, inserts[i]) == SS_HASNT_KEY);
        assert(sset_remove(ss, inserts[i]) == SS_HASNT_KEY);
        assert(sset_insert(ss, strdup(inserts[i])) != 0);
    }
    for (i = 0; i < num_noninserts; i++) {
        assert(sset_search(ss, noninserts[i]) == SS_HASNT_KEY);
        assert(sset_remove(ss, noninserts[i]) == SS_HASNT_KEY);
    }
    for (i = 0; i < num_inserts; i++) {
        assert(sset_search(ss, inserts[i]) == SS_HAS_KEY);
        assert(sset_remove(ss, inserts[i]) == SS_HAS_KEY);
        assert(sset_search(ss, inserts[i]) == SS_HASNT_KEY);
    }
    sset_destroy(ss);
    return 0;
}
