#ifndef RNKHASH_H
#define RNKHASH_H

/**
 *  Map from an element (string) to its rank (integer).
 */

typedef void * rnkhash_t;

/**
 *  Create a new rnkhash.
 */
rnkhash_t * create_rnkhash(void);

/**
 *  Insert an element and its rank into the rank hash.
 */
int rnkhash_insert(rnkhash_t * rh, const char * key, int rnk);

/**
 *  Get the rank that an element occurs at.
 *
 *  Returns -1 if the element is not found.
 */
int rnkhash_lookup(rnkhash_t * rh, const char * key);

/**
 *  Destroy a rank hash.  Note: does NOT destroy memory associated with keys.
 */
void rnkhash_destroy(rnkhash_t * rh);

#endif /* RNKHASH_H */
