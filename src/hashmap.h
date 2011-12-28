#ifndef _HASHMAS_H_
#define _HASHMAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/*
 *
 * hm.h
 * ----
 * simple hash map implementation with strings as keys.
 * to keep the keys persistent, the given keys are copied.
 *
 */

struct hm_node {
	char *key;
	void *data;
	struct hm_node *next;
};

struct hm {
	unsigned int size;
	struct hm_node **buckets;
};

/* put data with the given key, returns existing data if found, else NULL */
void *hm_put(struct hm *, const char *, void *);

/* executes function on all elements in table */
void hm_foreach(struct hm *, void (*func) (void *));

/* free hashmap */
void hm_free(struct hm *);

/* get value from hashmap, returns NULL if not found */
void *hm_get(const struct hm *, const char *);

/* check if data with given key exists */
int hm_has(const struct hm *, const char *);

/* initialize hashmap with given hashsize */
struct hm *hm_init(unsigned int);

#endif
