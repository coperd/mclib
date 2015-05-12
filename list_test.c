#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h> 
#include "list.h"

/* ansi color code */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define NCASES 100
#define N      5

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
 
 
void current_utc_time(struct timespec *ts) {
 
#ifdef __MACH__ /* OS X does not have clock_gettime, use clock_get_time */
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
 
}

/* generate a list with <n> random elements ranging from min to max */
void make_list(struct linked_list *list, int a[], int n)
{
    if (n < 0 || NULL == list) 
        return;

    int i;
    list_init(list);

    for (i = 0; i < n; i++) {
        struct node *p = (struct node *)malloc(sizeof(struct node));
        p->data = a[i];
        p->next = NULL;
        list_tadd(list, p);
    }
}

void make_array(int **a, int n)
{
    int i;
    *a = (int *)malloc(sizeof(int)*n);

    srand((unsigned)time(0));
    for (i = 0; i < n; i++) {
        (*a)[i] = rand() % (8*n);
    }
}

void TEST_DESTROY()
{
    int *a = NULL;
    struct linked_list *list = 
        (struct linked_list *)malloc(sizeof(struct linked_list));

    list_init(list);

    make_array(&a, 10);

    make_list(list, a, sizeof(a)/sizeof(a[0]));

    list_destroy(list);
}

void compare_sort(int n, int nt)
{
    int i;
    int *a = NULL;

    make_array(&a, n);


    struct timespec tm1, tm2, tq1, tq2;

    printf("%-10s%-6s%-15s%-15s%-15s\n", "Size", "Idx", "MS Time", "QS Time", "Ratio(MS/QS)");
    for (i = 0; i < nt; i++) {
        struct linked_list *list1 =
            (struct linked_list *)malloc(sizeof(struct linked_list));
        struct linked_list *list2 =
            (struct linked_list *)malloc(sizeof(struct linked_list));

        list_init(list1);
        list_init(list2);

        make_list(list1, a, n);
        make_list(list2, a, n);

        memset(&tm1, 0, sizeof(struct timespec));
        memset(&tm2, 0, sizeof(struct timespec));
        memset(&tq1, 0, sizeof(struct timespec));
        memset(&tq2, 0, sizeof(struct timespec));

        current_utc_time(&tq1);
        list_quicksort(list2);
        current_utc_time(&tq2);

        current_utc_time(&tm1);
        list_mergesort(list1);
        current_utc_time(&tm2);

        list_destroy(list1);
        list_destroy(list2);

        double ttm = tm2.tv_sec - tm1.tv_sec + (tm2.tv_nsec - tm1.tv_nsec) / 1000000000.0;
        double ttq = tq2.tv_sec - tq1.tv_sec + (tq2.tv_nsec - tq1.tv_nsec) / 1000000000.0;

        printf("%-10d%-6d%-15.10f%-15.10f%-15.10f\n", n, i+1, ttm, ttq, ttm*1./ttq);
    }
}

void TEST_INSERTIONSORT()
{
    srand((unsigned int)time(0));
    int ncases = NCASES;
    int len = N;
    struct linked_list *list = 
        (struct linked_list *)malloc(sizeof(struct linked_list));

    int i = 0, j = 0;
    int nsuccess = 0;
    for (i = 0; i < ncases; i++) {
        list_init(list);
        for (j = 0; j < len; j++) {
            struct node *p = (struct node *)malloc(sizeof(struct node));
            int x = rand() % 10; /* x ~ [0, 100] */
            p->data = x;
            p->next = NULL;
            list_tadd(list, p);
        }

        printf("Before: ");
        list_disp(list);

        list_insertionsort(list);

        printf("After :");
        struct node *p = list->head;
        bool is_sorted = true;
        while (p) {
            printf("%-4d", p->data);
            if (NULL != p->next) {
                printf("->");
                if (p->next->data < p->data)
                    is_sorted = false;
            }

            p = p->next;
        }

        if (is_sorted) {
            printf(KGRN "\tPASSED\n" RESET);
            nsuccess++;
        } else printf(KRED "\tFAILED\n" RESET);
    }
}

/* test list_push() & list_pop() */
void TEST_STACK()
{
    struct linked_list *list = 
        (struct linked_list *)malloc(sizeof(struct linked_list));
    struct linked_list *list2 = 
        (struct linked_list *)malloc(sizeof(struct linked_list));

    //make_list(list1, list2, list3, 10, 5, 20);
    //make_list(list1, list2, list3, 0, 5, 20);

    list_pop(list);
    list_push(list, 4);
}

void TEST_QUICKSORT()
{
    srand((unsigned int)time(0));
    int ncases = NCASES;
    int len = N;
    struct linked_list *list = 
        (struct linked_list *)malloc(sizeof(struct linked_list));

    int i = 0, j = 0;
    int nsuccess = 0;
    for (i = 0; i < ncases; i++) {
        list_init(list);
        for (j = 0; j < len; j++) {
            struct node *p = (struct node *)malloc(sizeof(struct node));
            int x = rand() % 10; /* x ~ [0, 100] */
            p->data = x;
            p->next = NULL;
            list_tadd(list, p);
        }

        printf("Before: ");
        list_disp(list);

        list_quicksort(list);

        printf("After :");
        struct node *p = list->head;
        bool is_sorted = true;
        while (p) {
            printf("%-4d", p->data);
            if (NULL != p->next) {
                printf("->");
                if (p->next->data < p->data)
                    is_sorted = false;
            }

            p = p->next;
        }

        if (is_sorted) {
            printf(KGRN "\tPASSED\n" RESET);
            nsuccess++;
        } else printf(KRED "\tFAILED\n" RESET);

    }

    printf("%-10s%4d\n", "total", ncases);
    printf("%-10s%4d\n", "succeeded", nsuccess);
    printf("%-10s%4d\n", "failed", ncases-nsuccess);
}

int main(int argc, char **argv)
{
    //TEST_QUICKSORT();
    //TEST_STACK();
    //TEST_INSERTIONSORT();
    if (argc != 3) {
        fprintf(stderr, "usage: %s <num> <ntimes>\n", argv[0]);
        exit(1);
    }

    //printf("%lu, %lu\n", sizeof(void *), sizeof(struct node));
    compare_sort(atoi(argv[1]), atoi(argv[2]));

    return 0;
}
