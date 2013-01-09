/*
 * $Id: nim.c,v 1.1 2013/01/09 10:43:59 urs Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static void usage(const char *name)
{
    fprintf(stderr, "Usage: %s [a b c]\n", name);
}

static int nim(int *heap, int depth);

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

static int nim(int *heap, int depth)
{
    int nr, count;
    int v, max = -10;

    assert(heap[0] >= 0 && heap[1] >= 0 && heap[2] >= 0);

    printf("%*snim: %d %d %d\n", depth * 2, "", heap[0], heap[1], heap[2]);

    if (heap[0] + heap[1] + heap[2] == 1) {
	max = -1;
	goto ret;
    }

    /* Generate all possible moves. */
    for (nr = 0, count = 1; nr < 3;
	 count < heap[nr] ? count++ : (nr++, count = 1)) {

	/* Take 'count' items from heap 'nr', if there are at least 'count'
	 * items left, but don't take the last item.
	 */
	if (heap[nr] < count)
	    continue;
	if (heap[0] + heap[1] + heap[2] == count)
	    continue;

	printf("%*s%d %d %d: %d %d\n", depth * 2, "",
	       heap[0], heap[1], heap[2], nr, count);
	heap[nr] -= count;
	v = -nim(heap, depth + 1);
	heap[nr] += count;
	if (v > max)
	    max = v;
	printf("%*sv = %+d, max = %+d\n", depth * 2, "", v, max);
    }

    assert(max == -1 || max == 1);

 ret:
    printf("%*sret: %+d\n", depth * 2, "", max);

    return max;
}
