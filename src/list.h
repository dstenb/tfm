#ifndef _SINGLY_LINKED_LIST_H_
#define _SINGLY_LINKED_LIST_H_

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

struct list {
	struct list *next;
	void *data;
};

struct list *list_append(struct list *, void *);
void list_foreach(struct list *, void (*)(void *));
void list_free(struct list *);
void list_free_full(struct list *, void (*)(void *));
struct list *list_insert(struct list *, void *, int);
struct list *list_insert_sorted(struct list *, void *, int (*)(void *, void *));
int list_length(struct list *);
struct list *list_new(void *);
struct list *list_prepend(struct list *, void *);

#endif
