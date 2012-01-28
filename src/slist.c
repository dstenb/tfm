#include "slist.h"

static struct list *create(struct list *, void *);

struct list *create(struct list *n, void *d)
{
	struct list *l;

	if (!(l = malloc(sizeof(struct list))))
		oom();

	l->data = d;
	l->next = n;

	return l;
}

struct list *list_append(struct list *head, void *d)
{
	struct list *l;

	if (head) {
		for (l = head; l->next; l = l->next);
		l->next = create(NULL, d);
	} else {
		head = create(NULL, d);
	}

	return head;
}

void list_foreach(struct list *head, void (*func)(void *))
{
	if (!func)
		return;

	for ( ; head; head = head->next)
		func(head->data);
}

void list_free(struct list *head)
{
	struct list *tmp;

	while (head) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

void list_free_full(struct list *head, void (*func)(void *))
{
	struct list *tmp;

	while (head) {
		tmp = head;
		head = head->next;
		if (tmp->data && func)
			func(tmp->data);
		free(tmp);
	}
}

int list_length(struct list *head)
{
	int i;

	for (i = 0; head; head = head->next, i++);

	return i;
}

struct list *list_insert(struct list *head, void *d, int p)
{
	struct list *l;

	if (head && p != 0) {
		for (l = head ; l->next && p != 0; l = l->next, p--);
		l->next = create(NULL, d);
	} else {
		head = create(head, d);
	}

	return head;
}

struct list *list_insert_sorted(struct list *head, void *d,
		int (*cmp)(void *, void *))
{
	struct list *l;

	if (!head || cmp(d, head->data) < 0)
		return create(head, d);

	for (l = head ; l->next && cmp(d, l->next->data) > 0; l = l->next);

	l->next = create(l->next, d);

	return head;
}

struct list *list_new(void *d)
{
	return create(NULL, d);
}

struct list *list_prepend(struct list *l, void *d)
{
	return create(l, d);
}
