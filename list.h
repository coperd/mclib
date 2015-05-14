#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/*
 * simplified single linked list implementation
 */

/*
 * TODO: 
 * (1). further optimization the time complexity 
 * (2). handle generic data type
 */

struct node
{
    int data;
    struct node *next;
};

struct linked_list 
{
    struct node *head; /* head of the single linked list */
    struct node *tail; /* tail of the single linked list */
    int len; /* length of the single linked list */
};

void list_init(struct linked_list *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
}

/*
 * return true when list is empty and false otherwise
 */
bool list_is_empty(struct linked_list *list)
{
    return (NULL == list->head && NULL == list->tail);
}

/*
 * get number of nodes in the list
 */
int list_length(struct linked_list *list)
{
    assert(list->len >= 0);
    return list->len;
}

/*
 * insert an element to the head of the list
 * Time Complexity: O(1)
 */
void list_hadd(struct linked_list *list, struct node *p)
{
    if (NULL == p) return;

    /* when empty, should also handle the tail accordingly */
    if (list_is_empty(list)) {
        list->tail = p;
        list->tail->next = NULL;
    }

    p->next = list->head;
    list->head = p;

    list->len++;
}

/*
 * insert an element to the tail of the list
 * Time Complexity: O(1) [because the tail is automatically maintained by the 
 * linked_list structure]
 */
void list_tadd(struct linked_list *list, struct node *p)
{
    if (NULL == p) return;

    /* when empty, set the head accordingly */ 
    if (list_is_empty(list)) { 
        list->head = p;
        list->tail = p;
        list->tail->next = NULL;
    } else {
        list->tail->next = p;
        list->tail = p;
        list->tail->next = NULL;
    }

    list->len++;
}

/*
 * append a node to the tail of the list
 */
void list_append(struct linked_list *list, int value)
{
    struct node *p = (struct node *)malloc(sizeof(struct node));
    p->data = value;
    p->next = NULL;

    list_tadd(list, p);
}

/*
 * delete node p from the list. If p is not in the list, do nothing
 * Time Complexity: when p is the tail, it's O(n); otherwise O(1)
 */
void list_del(struct linked_list *list, struct node *p)
{
    if ((NULL == p) || list_is_empty(list)) return;
    else if (list->head == list->tail) { /* has only one element */
        if (list->head == p) list_init(list);
        else return;
    }

    /* if you are here, it demonstrates more than 2 nodes are in the list */
    struct node *q = list->head, *prev = NULL; 

    /* find the previous node of p first */
    while (q) {
        if (q == p) {
            break;
        }

        prev = q;
        q = q->next;
    }

     /* delete the node when p is indeed in the list */
    if (NULL != q) {
        if (NULL == prev) { /* p is the head */
            list->head = list->head->next;
        } else {
            prev->next = p->next;
        }
        list->len--;
        free(p);
    }
}

/*
 * get the number of occurrences of value in the list
 * Time Complexity: O(n)
 */
int list_count(struct linked_list *list, int value)
{
    int cnt = 0;
    struct node *p = NULL;

    for (p = list->head; p != NULL; p = p->next) {
        if (p->data == value) cnt++;
    }

    return cnt;
}

/* 
 * return the node indexed by "index", index range is range[0, list_length()-1]
 * Time Complexity: O(n)
 */
struct node * list_getitem(struct linked_list *list, int index)
{
    if ((index < 0) || (index >= list_length(list)))
        return NULL;

    struct node *p = NULL;
    int cnt = 0;
    for (p = list->head; p != NULL; p = p->next) {
        if (cnt == index) break;
        cnt++;
    }

    return p;
}

/* 
 * deallocate all of its memory and set it to the empty list
 */
void list_destroy(struct linked_list *list)
{
    if (NULL == list) return;

    struct node *p = list->head, *prev = NULL;
    while (p) {
        prev = p;
        p = p->next;
        free(prev);
    }
    (list)->len = 0;

    free(list);
}

/* 
 * add a new head node, used to construct STACK
 * Time Complexity: O(1)
 */
void list_push(struct linked_list *list, int value)
{
    struct node *p = (struct node *)malloc(sizeof(struct node));
    p->data = value;
    p->next = NULL;
    list_hadd(list, p); 
}

/*
 * deletes the head node and return its data
 * Time Complexity: O(1)
 */
int list_pop(struct linked_list *list)
{
    assert(!list_is_empty(list));

    struct node *p = list->head;
    int ret = p->data;

    /* list has only one element, should also set the tail accordingly */
    if (list->head == list->tail) {
        list->tail = list->tail->next;
    }

    list->head = list->head->next;
    list->len--;
    free(p); 

    return ret;
}

/*
 * insert value to the index of idx of linked list 
 * do nothing when index is illeagal
 */
void list_insert_nth(struct linked_list *list, int idx, int value)
{
    if ((idx < 0) || (idx >= list_length(list))) return;

    struct node *p = (struct node *)malloc(sizeof(struct node));
    p->data = value;
    p->next = NULL;

    if (0 == idx) {
        list_hadd(list, p);
    } else if (list_length(list) - 1 == idx) {
        list_tadd(list, p);
    } else {

        int i;
        struct node *q = list->head, *prev = NULL;
        for (i = 0; i <= idx; i++) {
            prev = q;
            q = q->next; 
        }
        prev->next = p;
        p->next = q;
        list->len++;
    }
}

/*
 * insert a node to the already sorted linked list
 * Time Complexity: O(n)
 */
void list_sortedinsert(struct linked_list *list, struct node *p)
{
    if (NULL == list || NULL == p) return;
    if (list_is_empty(list)) {
        list->head = list->tail = p;
        list->len = 1;
    }

    struct node *q = list->head, *prev = NULL;
    while (q) {
        if (p->data <= q->data) {
            if (NULL == prev) {
                list_hadd(list, p);
                return;
            }
            prev->next = p;
            p->next = q;
            list->len++;
            return;
        }
        
        prev = q;
        q = q->next;
        if (q) assert(prev->data <= q->data);
    }
    
    if (NULL == q) list_tadd(list, p);
}

/*
 * sort the linked list using insertionsort() way
 */
void list_insertionsort(struct linked_list *list)
{
    if (NULL == list || list_is_empty(list)) return;

    /* only keep the first element in the list */
    struct node *q = list->head->next;
    list->tail = list->head;
    list->tail->next = NULL;
    list->len = 1;

    struct node *next = NULL;

    while (q) {
        /* after p inserted to the list, its next field changes */
        next = q->next; 
        list_sortedinsert(list, q);
        q = next;
    }
}

/* 
 * list_merge - merge two sorted linked lists
 * @a, @b: the two lists to merge
 *
 * Be aware that @a and @b are in unconsistant state after the call
 *
 * Time Complexity: O(a && b)
 * Space Complexity: O(1)
 */
struct linked_list * list_merge(struct linked_list *a, struct linked_list *b)
{
    struct linked_list *list = 
        (struct linked_list *)malloc(sizeof(struct linked_list));
    struct node *ap = a->head, *bp = b->head, *prev = NULL;

    list_init(list);

    while (ap && bp) {
        if (ap->data <= bp->data) {
            prev = ap;
            ap = ap->next;
        } else {
            prev = bp;
            bp = bp->next;
        }
        list_tadd(list, prev);
    }
   
    if (list->tail) {
        list->tail->next = ap?ap:bp;
        list->tail = ap?a->tail:b->tail;
    } else {
        list->head = ap?:bp;
        list->tail = ap?a->tail:b->tail;
    }
    list->len = a->len + b->len;

    list_init(a);
    list_init(b);

    return list;
}

/*
 * list_mergesort - list merge sort
 * @list: list to sort
 *
 * Time Complexity: O(nlgn)
 * Space Complexity: O(lgn)
 */
void list_mergesort(struct linked_list **list)
{
    struct linked_list *list_fh = NULL, *list_lh = NULL;
    struct node *p = (*list)->head, *prev = NULL;
    int len = list_length(*list);
    int cnt = 1;

    /* return when list is NULL or only has one element */
    if (NULL == *list || (*list)->head == (*list)->tail) return;

    /* find the mid node pointer */
    for (; cnt <= len/2; cnt++, prev = p, p = p->next);

    /* split the list into two consecutive ones */
    list_fh = (struct linked_list *)malloc(sizeof(struct linked_list));
    list_lh = (struct linked_list *)malloc(sizeof(struct linked_list));

    list_init(list_fh);
    list_init(list_lh);

    list_fh->head = (*list)->head;
    list_fh->tail = prev;
    prev->next = NULL;
    list_fh->len = cnt - 1;

    list_lh->head = p;
    list_lh->tail = (*list)->tail;
    list_lh->len = len - cnt + 1;

    (*list)->head = (*list)->tail = NULL;
    (*list)->len = 0;
    
    list_mergesort(&list_fh);
    list_mergesort(&list_lh);
    *list = list_merge(list_fh, list_lh);

    free(list_fh);
    free(list_lh);
}

/* list_mergesort2 - merge sort list
 * @list - list to merge sort
 *
 * Time Complexity: O(nlgn)
 * Space Complexity: O(1)
 *
 * Modified from kernel(lib/list_sort.c), 
 * This alternative implementation scales better, reaching ~3x performance gain 
 * as list length approaches the L2 cache size
 */
void list_mergesort2(struct linked_list **list)
{
    int MAX_LIST_LENGTH_BITS = 20;
    struct linked_list part[MAX_LIST_LENGTH_BITS+1];

    int lev; /* index to part[] */
    int max_lev = 0;
    struct node *p;

    if (list_is_empty(*list))
        return;

    for (lev = 0; lev <= MAX_LIST_LENGTH_BITS; lev++)
        list_init(&part[lev]);

    struct linked_list *cur = 
        (struct linked_list *)malloc(sizeof(struct linked_list));
    list_init(cur);
    p = (*list)->head;
    while (p) {
        cur->head = cur->tail = p;
        cur->len = 1; 
        p = p->next;

        (*list)->head = p;
        if (!p) (*list)->tail = NULL;
        (*list)->len--;

        cur->tail->next = NULL;

        for (lev = 0; !list_is_empty(&part[lev]); lev++) {
            cur = list_merge(&part[lev], cur);
            list_init(&part[lev]);
        }
        if (lev > max_lev) {
            if (lev >= MAX_LIST_LENGTH_BITS) {
                lev--;
            }
            max_lev = lev;
        }
        part[lev] = *cur;
    }

    list_init(*list);
    for (lev = 0; lev <= max_lev; lev++)
        if (!list_is_empty(&part[lev]))
            *list = list_merge(&part[lev], *list);
}

/*
 * swap the data of node p and q
 */
void swap(struct node *p, struct node *q)
{
    int tmp = p->data;
    p->data = q->data;
    q->data = tmp;
}

/*
 * take the head as pivot, partitioning the list
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */
struct node * list_partition(struct linked_list *list)
{
    if (NULL == list || list_is_empty(list)) return NULL;

    if (list->head == list->tail) return list->head;

    /* take the list head as the pivot node */
    struct node *h = list->head;
    int key = list->head->data;

    struct node *q = list->head, *r = q->next, *prev = NULL;

    while (r) {
        if (r->data < key) { /* find a small element */
            assert(q->data >= key);
            swap(q, r);
            prev = q;
            q = q->next;
        } 

        r = r->next;
    }

    if (q->data == key) return q;

    for (h = q; h->data != key; h = h->next);

    list_del(list, h);

    h = (struct node *)malloc(sizeof(struct node));
    h->data = key;
    h->next = NULL;
    if (prev) {
        prev->next = h;
        h->next = q;
        list->len++;
    } else {
        list_hadd(list, h);
    }

    return h;
}

/*
 * quicksort the list
 * Time Complexity: O(n log n)
 * Space Complexity: O(log n)
 */
void list_quicksort(struct linked_list *list)
{
    struct node *p = list_partition(list);
    if (NULL == p) return;

    int len = list_length(list);
    int cnt = 0;
    struct node *q = NULL, *prev = NULL;

    for (q = list->head, cnt = 1; q != p; prev = q, q = q->next, cnt++);

    q = q->next; /* as the head of the latter half list */

    struct linked_list *list_fh = 
        (struct linked_list *)malloc(sizeof(struct linked_list));
    struct linked_list *list_lh = 
        (struct linked_list *)malloc(sizeof(struct linked_list));

    list_init(list_fh);
    list_init(list_lh);

    if (prev) {
        list_fh->head = list->head;
        list_fh->tail = prev;
        list_fh->tail->next = NULL;
        list_fh->len = cnt - 1;
    } 
    
    if (q) {
        list_lh->head = q;
        list_lh->tail = list->tail;
        list_lh->len = len - cnt;
    }

    list->head = list->tail = NULL;
    list->len = 0;

    list_quicksort(list_fh);
    list_quicksort(list_lh);

    if (list_fh->head) {
        list->head = list_fh->head;
        list_fh->tail->next = p;
    } else {
        list->head = p;
    }
    p->next = list_lh->head;

    if (list_lh->tail) {
        list->tail = list_lh->tail;
    } else {
        list->tail = p;
    }
    
    list->len = list_fh->len + list_lh->len + 1;
    
    free(list_fh);
    free(list_lh);
}


/*
 * sorting based method to remove duplicates in the list
 * Time Complexity: O(n log n)
 * Space Complexity: O(log n)
 */
void list_rmdup(struct linked_list *list)
{
    if (list_is_empty(list)) return;

    struct node *p = list->head->next, *q = list->head, *r = NULL;

    /* sort the list first */
    list_mergesort(&list);

    while (p) {
        if (p->data == q->data) {
            r = p;
            p = p->next;
            q->next = p;
            free(r);
        } else {
            q = p;
            p = p->next;
        }
    }
}

/*
 * hashmap based method to remove duplicates in the list
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 * Suppose the number in the list are smaller than a specific number
 */
void list_rmdup2(struct linked_list *list)
{
}

/*
 * takes two lists, removes the front node from the second list and pushes it
 * onto the front of the first.
 * list1 <--head insert-- list2
 */
void list_mvnode(struct linked_list *list1, struct linked_list *list2)
{
    struct node *p = list2->head, *q = NULL; 
    while (list2->head) {
        list2->head = p->next;
        list2->len--;

        list_hadd(list1, p);
    }

}

/*
 * reverse the linked list
 * Time Complexity: O(n)
 */
void list_reverse(struct linked_list *list)
{
    struct node *p = list->head;
    list->len = 0;
    list->head = NULL;
    list->tail = NULL;

    while (p) {
        list_hadd(list, p);
        p = p->next;
    }
}

/*
 * detect loop in the list
 * return the starting node of the loop on success
 */
struct node * detect_loop(struct node *head)
{
    /* slop -> tortoise, fastp -> hare */
    struct node *slowp = NULL, *fastp = NULL; 

    /* make sure they meet at some point */
    for (slowp = head->next, fastp = head->next->next; slowp != fastp; 
            slowp = slowp->next, fastp = fastp->next->next);
    
    /* move tortoise to the beginning */
    slowp = head;

    /* make them run again at the same speed */
    for (; slowp != fastp; slowp = slowp->next, fastp = fastp->next);

    /* now the meeting point is the start of the loop */
    return slowp;
}

void list_disp(struct linked_list *list)
{
    struct node *p = list->head;
    while (p) {
        printf("%d", p->data);
        if (NULL != p->next) {
            printf(" -> ");
        }

        p = p->next;
    }

    printf("\n");
}

#endif
