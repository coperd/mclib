#include "sys-queue.h"
#include <stdio.h>
#include <stdlib.h>

struct request
{
    int a;
    TAILQ_ENTRY(request) node;
    int b;
};

int main(int argc, char **argv)
{
    TAILQ_HEAD(head, request) mylist;


    struct head AB = TAILQ_HEAD_INITIALIZER(AB);

    TAILQ_INIT(&mylist);

    struct request elm1, elm2, elm3;

    TAILQ_INSERT_TAIL(&mylist, &elm1, node);
    TAILQ_INSERT_TAIL(&mylist, &elm2, node);
    TAILQ_INSERT_TAIL(&mylist, &elm3, node);

    TAILQ_REMOVE(&mylist, &elm2, node);

    if (!TAILQ_EMPTY(&mylist))
        printf("mylist is NOT empty!\n");

    printf("TAILQ_FIRST = [%p]\n", TAILQ_FIRST(&mylist));
    printf("TAILQ_NEXT(%p) = [%p]\n", TAILQ_FIRST(&mylist), TAILQ_NEXT(TAILQ_FIRST(&mylist), node));
    printf("TAILQ_LAST = [%p]\n", TAILQ_LAST(&mylist, head));

    struct request *elm;
    int cnt = 0;

    TAILQ_FOREACH(elm, &mylist, node) {
        cnt++;
        printf("%p\n", elm);
    }

    printf("In total, there are [%d] elements in the tail queue!\n", cnt);

    return 0;
}
