/* Copyright (C) 2002 Christopher Clark <firstname.lastname@cl.cam.ac.uk> */

/**
 *  NOTE for inclusion in RBO sources.  The original implementation
 *  was for a hash table.  This has been modified to work with a hash
 *  set (mostly by removing unwanted functionality) by William Webber
 *  (wew@csse.unimelb.edu.au).
 *
 *  Sun Aug 29 12:38:28 EST 2010
 */

#ifndef __HASHSET_H__
#define __HASHSET_H__

struct hashset;

enum hashset_haskey {
    HAS_KEY = 1,
    HASNT_KEY = 0
};

/* Example of use:
 *
 *      struct hashset  *h;
 *      struct some_key   *k;
 *      struct some_value *v;
 *
 *      static unsigned int         hash_from_key_fn( void *k );
 *      static int                  keys_equal_fn ( void *key1, void *key2 );
 *
 *      h = create_hashset(16, hash_from_key_fn, keys_equal_fn);
 *      k = (struct some_key *)     malloc(sizeof(struct some_key));
 *      v = (struct some_value *)   malloc(sizeof(struct some_value));
 *
 *      (initialise k and v to suitable values)
 * 
 *      if (! hashset_insert(h,k,v) )
 *      {     exit(-1);               }
 *
 *      if (NULL == (found = hashset_search(h,k) ))
 *      {    printf("not found!");                  }
 *
 *      if (NULL == (found = hashset_remove(h,k) ))
 *      {    printf("Not found\n");                 }
 *
 */

/* Macros may be used to define type-safe(r) hashset access functions, with
 * methods specialized to take known key and value types as parameters.
 * 
 * Example:
 *
 * Insert this at the start of your file:
 *
 * DEFINE_HASHSET_INSERT(insert_some, struct some_key);
 * DEFINE_HASHSET_SEARCH(search_some, struct some_key);
 * DEFINE_HASHSET_REMOVE(remove_some, struct some_key);
 *
 * This defines the functions 'insert_some', 'search_some' and 'remove_some'.
 * These operate just like hashset_insert etc., with the same parameters,
 * but their function signatures have 'struct some_key *' rather than
 * 'void *', and hence can generate compile time errors if your program is
 * supplying incorrect data as a key (and similarly for value).
 *
 * Note that the hash and key equality functions passed to create_hashset
 * still take 'void *' parameters instead of 'some key *'. This shouldn't be
 * a difficult issue as they're only defined and passed once, and the other
 * functions will ensure that only valid keys are supplied to them.
 *
 * The cost for this checking is increased code size and runtime overhead
 * - if performance is important, it may be worth switching back to the
 * unsafe methods once your program has been debugged with the safe methods.
 * This just requires switching to some simple alternative defines - eg:
 * #define insert_some hashset_insert
 *
 */

/*****************************************************************************
 * create_hashset
   
 * @name                    create_hashset
 * @param   minsize         minimum initial size of hashset
 * @param   hashfunction    function for hashing keys
 * @param   key_eq_fn       function for determining key equality
 * @return                  newly created hashset or NULL on failure
 */

struct hashset *
create_hashset(unsigned int minsize,
                 unsigned int (*hashfunction) (void*),
                 int (*key_eq_fn) (void*,void*));

/*****************************************************************************
 * hashset_insert
   
 * @name        hashset_insert
 * @param   h   the hashset to insert into
 * @param   k   the key - hashset claims ownership and will free on removal
 * @param   v   the value - does not claim ownership
 * @return      non-zero for successful insertion
 *
 * This function will cause the table to expand if the insertion would take
 * the ratio of entries to table size over the maximum load factor.
 *
 * This function does not check for repeated insertions with a duplicate key.
 * The value returned when using a duplicate key is undefined -- when
 * the hashset changes size, the order of retrieval of duplicate key
 * entries is reversed.
 * If in doubt, remove before insert.
 */

int 
hashset_insert(struct hashset *h, void *k);

#define DEFINE_HASHSET_INSERT(fnname, keytype) \
int fnname (struct hashset *h, keytype *k) \
{ \
    return hashset_insert(h,k); \
}

/*****************************************************************************
 * hashset_search
   
 * @name        hashset_search
 * @param   h   the hashset to search
 * @param   k   the key to search for  - does not claim ownership
 * @return      the value associated with the key, or NULL if none found
 */

enum hashset_haskey
hashset_search(struct hashset *h, void *k);

#define DEFINE_HASHSET_SEARCH(fnname, keytype) \
enum hashset_haskey fnname (struct hashset *h, keytype *k) \
{ \
    return (hashset_search(h,k)); \
}

/*****************************************************************************
 * hashset_remove
   
 * @name        hashset_remove
 * @param   h   the hashset to remove the item from
 * @param   k   the key to search for  - does not claim ownership
 * @return      the value associated with the key, or NULL if none found
 */

enum hashset_haskey
hashset_remove(struct hashset *h, void *k);

#define DEFINE_HASHSET_REMOVE(fnname, keytype) \
enum hashset_haskey fnname (struct hashset *h, keytype *k) \
{ \
    return (hashset_remove(h,k)); \
}


/*****************************************************************************
 * hashset_count
   
 * @name        hashset_count
 * @param   h   the hashset
 * @return      the number of items stored in the hashset
 */
unsigned int
hashset_count(struct hashset *h);


/*****************************************************************************
 * hashset_destroy
   
 * @name        hashset_destroy
 * @param   h   the hashset
 */

void
hashset_destroy(struct hashset *h);

#endif /* __HASHSET_H__ */

/*
 * Copyright (c) 2002, Christopher Clark
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * * Neither the name of the original author; nor the names of any contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 * 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
