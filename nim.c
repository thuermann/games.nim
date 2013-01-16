/*
 * $Id: nim.c,v 1.3 2013/01/16 07:04:01 urs Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static void usage(const char *name)
{
    fprintf(stderr, "Usage: %s [a b c]\n", name);
}

struct node {
    struct node *left, *right;
    int heap[3];
    int val;
};

static int nim(int *heap, int depth);
static struct node *lookup(struct node *root, int *heap);
static void insert(struct node **rootp, int *heap, int val);
static int heap_cmp(const int *a, const int *b);
static int int_cmp(const void *a, const void *b);

int main(int argc, char **argv)
{
    int heap[3] = { 3, 4, 5 };
    int i;

    if (argc == 4) {
	for (i = 0; i < 3; i++)
	    heap[i] = atoi(*++argv);
    } else if (argc != 1) {
	usage(argv[0]);
	exit(1);
    }

    printf("%+d\n", nim(heap, 0));

    return 0;
}

static struct node *root;

static int nim(int *heap, int depth)
{
    int nr, count;
    int v, max = -10;
    int myheap[3];
    struct node *n;

    assert(heap[0] >= 0 && heap[1] >= 0 && heap[2] >= 0);

    printf("%*snim: %d %d %d\n", depth * 2, "", heap[0], heap[1], heap[2]);

    if (n = lookup(root, heap)) {
	max = n->val;
	goto ret;
    }

    if (heap[0] + heap[1] + heap[2] == 0) {
	max = 1;
	goto ret;
    }

    /* Generate all possible moves. */
    for (nr = 0, count = 1; nr < 3;
	 count < heap[nr] ? count++ : (nr++, count = 1)) {

	/* Take 'count' items from heap 'nr', if there are at least 'count'
	 * items left.
	 */
	if (heap[nr] < count)
	    continue;

	printf("%*s%d %d %d: %d %d\n", depth * 2, "",
	       heap[0], heap[1], heap[2], nr, count);
	memcpy(myheap, heap, sizeof(myheap));
	myheap[nr] -= count;
	qsort(myheap, 3, sizeof(int), int_cmp);
	v = -nim(myheap, depth + 1);
	if (v > max)
	    max = v;
	printf("%*sv = %+d, max = %+d\n", depth * 2, "", v, max);
    }

    assert(max == -1 || max == 1);

    insert(&root, heap, max);

 ret:
    printf("%*sret: %+d\n", depth * 2, "", max);

    return max;
}

static struct node *lookup(struct node *root, int *heap)
{
    int cmp;

    while (root && (cmp = heap_cmp(root->heap, heap)) != 0) {
	if (cmp < 0)
	    root = root->left;
	else
	    root = root->right;
    }

    return root;
}

static void insert(struct node **rootp, int *heap, int val)
{
    struct node *n;
    int cmp;

    while ((n = *rootp) && (cmp = heap_cmp(n->heap, heap)) != 0) {
	if (cmp < 0)
	    rootp = &n->left;
	else
	    rootp = &n->right;
    }

    n = *rootp = malloc(sizeof(struct node));
    n->left = n->right = NULL;
    n->val  = val;
    memcpy(n->heap, heap, 3 * sizeof(int));
}

static int heap_cmp(const int *a, const int *b)
{
    if (a[0] != b[0])
	return a[0] - b[0];
    if (a[1] != b[1])
	return a[1] - b[1];
    return a[2] - b[2];
}

static int int_cmp(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}
