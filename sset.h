#ifndef SSET_H
#define SSET_H

/**
 *  Simple string set interface.
 */

#include "hashset.h"

enum sset_haskey {
    SS_HAS_KEY = 1,
    SS_HASNT_KEY = 0
};

//typedef struct hashset sset;
typedef void * sset;

/**
 *  Create a new string set.
 */
sset * create_sset(unsigned int minsize);

/**
 *  Insert a string into a sset.
 *
 *  @param sset the stringset to insert into.
 *  @param str the string to insert.  The sset take memory ownership.
 *  @return non-zero for successful insertion.
 */
int
sset_insert(sset * ss, char *str);

/**
 *  Search for a string in a sset.
 *
 *  @param sset the stringset to search in.
 *  @param str the string to search for.
 *  @return an sset_haskey, with HAS_KEY if found, HASNT_KEY if not.
 */
enum sset_haskey
sset_search(sset *ss, const char *str);

/** 
 *  Delete a string.
 *
 *  @param sset the sset to delete the string from.
 *  @param str the string to delete.  Memory is also free.
 *  @return an sset_haskey, with HAS_KEY if found, HASNT_KEY if not.
 */
enum sset_haskey
sset_remove(sset *ss, const char *str);

/**
 *  Destroy a sset.
 *
 *  Also free all memory associated with keys.
 */
void sset_destroy(sset *ss);

#endif /* SSET_H */
