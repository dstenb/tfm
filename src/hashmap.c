#include "hashmap.h"

#define DEFAULT_SIZE 7

static unsigned int hashfunc(const struct hm *, const char *);
static struct hm_node *hm_node_create(const char *, void *, struct hm_node *);

unsigned int hashfunc(const struct hm * hm, const char *key)
{
	unsigned int h = 0;

	while (*key != '\0')
		h = 31 * h + *key++;

	return h % hm->size;
}

struct hm_node *hm_node_create(const char *key, void *data, struct hm_node * next)
{
	struct hm_node *node;
	
	if (!(node = malloc(sizeof(struct hm_node))))
		oom();
	if (!(node->key = strdup(key)))
		oom();

	node->data = data;
	node->next = next;
	return node;
}

void *hm_put(struct hm * hm, const char *key, void *data)
{
	int hash = hashfunc(hm, key);
	struct hm_node *node;
	void *tmp;

	for (node = hm->buckets[hash]; node; node = node->next) {
		if (strcmp(node->key, key) == 0) {
			tmp = node->data;
			node->data = data;
			return tmp;
		}
	}
	hm->buckets[hash] = hm_node_create(key, data, hm->buckets[hash]);
	return NULL;
}

void hm_foreach(struct hm * hm, void (*func) (void *arg))
{
	unsigned int i;
	struct hm_node *node;

	for (i = 0; i < hm->size; i++) {
		for (node = hm->buckets[i]; node; node = node->next) {
			func(node->data);
		}
	}
}

void hm_free(struct hm * hm)
{
	unsigned int i;
	struct hm_node *node;
	struct hm_node *tmp;

	for (i = 0; i < hm->size; i++) {
		while ((node = hm->buckets[i])) {
			tmp = node;
			hm->buckets[i] = tmp->next;
			free(tmp->key);
			free(tmp);
		}
	}

	free(hm->buckets);
	free(hm);
}

void *hm_get(const struct hm * hm, const char *key)
{
	struct hm_node *node;

	for (node = hm->buckets[hashfunc(hm, key)]; node; node = node->next) {
		if (strcmp(node->key, key) == 0)
			return node->data;
	}

	return NULL;
}

int hm_has(const struct hm * hm, const char *key)
{
	struct hm_node *node;

	for (node = hm->buckets[hashfunc(hm, key)]; node; node = node->next) {
		if (strcmp(node->key, key) == 0)
			return 1;
	}

	return 0;
}

struct hm *hm_init(unsigned int size)
{
	unsigned int i;
	struct hm *hm;

	if (!(hm = malloc(sizeof(struct hm))))
		oom();

	hm->size = size != 0 ? size : DEFAULT_SIZE;

	if (!(hm->buckets = malloc(sizeof(struct hm_node *) * hm->size)))
		oom();

	for (i = 0; i < hm->size; i++)
		hm->buckets[i] = NULL;

	return hm;
}
