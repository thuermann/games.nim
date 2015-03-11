/*
 * $Id: nim.c,v 1.9 2015/03/11 10:25:49 urs Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static void usage(const char *name)
{
    fprintf(stderr, "Usage: %s num ...\n", name);
}

struct map {
    int **offset;
    char *result;
};

static int nim(const struct map *map, const int *heap, int n, int depth);
static int lookup_init(struct map *map, const int *max, int n);
static char *lookup(const struct map *map, const int *heap, int n);
static int int_cmp(const void *a, const void *b);

int main(int argc, char **argv)
{
    struct map map;
    int n, i;

    if (argc == 1) {
	usage(argv[0]);
	exit(1);
    }

    n = argc - 1;
    int heap[n];
    for (i = 0; i < n; i++)
	if ((heap[i] = atoi(*++argv)) < 0) {
	    fprintf(stderr, "Invalid heap size: %d\n", heap[i]);
	    exit(1);
	}

    if (!lookup_init(&map, heap, n)) {
	perror("init");
	exit(1);
    }

    printf("%+d\n", nim(&map, heap, n, 0));

    return 0;
}

static int nim(const struct map *map, const int *heap, int n, int depth)
{
    int nr, count;
    int s, v, max = -10;
    int myheap[n];
    char *res;
    int i;

    for (i = 0; i < n; i++)
	assert(heap[i] >= 0);

    printf("%*snim:", depth * 2, "");
    for (i = 0; i < n; i++)
	printf(" %d", heap[i]);
    putchar('\n');

    res = lookup(map, heap, n);
    if (*res != 0) {
	max = *res;
	goto ret;
    }

    for (s = 0, i = 0; i < n; i++)
	s += heap[i];
    if (s == 0) {
	max = 1;
	goto ret;
    }

    /* Generate all possible moves. */
    for (nr = 0, count = 1; nr < n;
	 count < heap[nr] ? count++ : (nr++, count = 1)) {

	/* Take 'count' items from heap 'nr', if there are at least 'count'
	 * items left.
	 */
	if (heap[nr] < count)
	    continue;

	printf("%*s", depth * 2, "");
	for (i = 0; i < n; i++)
	    printf(i == 0 ? "%d" : " %d", heap[i]);
	printf(": %d %d\n", nr, count);
	memcpy(myheap, heap, sizeof(myheap));
	myheap[nr] -= count;
	qsort(myheap, n, sizeof(int), int_cmp);
	v = -nim(map, myheap, n, depth + 1);
	if (v > max)
	    max = v;
	printf("%*sv = %+d, max = %+d\n", depth * 2, "", v, max);
    }

    assert(max == -1 || max == 1);

    *res = max;

 ret:
    printf("%*sret: %+d\n", depth * 2, "", max);

    return max;
}

static int lookup_init(struct map *map, const int *max, int n)
{
    int i, j, k;

    if (!(map->offset = malloc(n * sizeof(int *))))
	return 0;

    for (i = 0; i < n; i++)
	if (!(map->offset[i] = malloc((max[i] + 2) * sizeof(int))))
	    return 0;

    for (i = 0; i <= max[0] + 1; i++)
	map->offset[0][i] = i;

    for (k = 1; k < n; k++)
	for (i = 0; i <= max[k] + 1; i++) {
	    int s = 0;
	    for (j = 0; j <= i; j++)
		if (j <= max[k - 1])
		    s += map->offset[k - 1][j];
		else
		    s += map->offset[k - 1][max[k - 1] + 1];
	    map->offset[k][i] = s;
	}

    if (!(map->result = calloc(map->offset[n - 1][max[n - 1] + 1], 1)))
	return 0;

    return 1;
}

static char *lookup(const struct map *map, const int *heap, int n)
{
    int idx, i;

    idx = heap[0];
    for (i = 1; i < n; i++)
	idx += map->offset[i][heap[i]];
    return &map->result[idx];
}

static int int_cmp(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}
