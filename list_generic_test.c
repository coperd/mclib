#include <stdio.h>
#include <stdlib.h>

#include "list_generic.h"

/* list structure to store integer */
struct mylist
{
	int a;
	struct list_head list;
};

void list_disp(struct list_head *head)
{
    struct mylist *pos;
    printf("head[%p] -> ", head);
    list_for_each_entry(pos, head, list) {
        printf("%d -> ", pos->a);
    }
    printf("head[%p]", head);
    printf("\n");
}

int main()
{
    struct mylist *pos;

    /* initialize the first list "mylist1" and the second one "mylist2" */
    LIST_HEAD(mylist1);
    struct list_head mylist2;
    INIT_LIST_HEAD(&mylist2);

    struct mylist first1 = {
        .a = 11,
        .list = LIST_HEAD_INIT(first1.list)
    };

    struct mylist second1;
    second1.a = 12;
    INIT_LIST_HEAD(&second1.list);

    printf("=======testing list_add()=======\n");
    /* build the first list "mylist1" */
    list_add(&(first1.list), &mylist1);
    list_add(&(second1.list), &mylist1);

    printf("list1 after list_add() of [%d, %d]: ", first1.a, second1.a);
    list_disp(&mylist1);

    /* updating node */
    struct mylist rp = {
        .a = 88,
        .list = LIST_HEAD_INIT(rp.list)
    };

    /* the second list's nodes */
    struct mylist first2 = {
        .a = 21,
        .list = LIST_HEAD_INIT(first2.list)
    };

    struct mylist second2;
    second2.a = 22;
    INIT_LIST_HEAD(&second2.list);

    /* build the second list */
    list_add_tail(&(first2.list), &mylist2);
    list_add_tail(&second2.list, &mylist2);

    printf("list2 after list_add_tail() of [%d, %d]: ", first2.a, second2.a);
    list_disp(&mylist2);

    printf("\n\n=======testing list_replace()=======\n");
    /* update the value of second1.a */
    list_replace(&second1.list, &rp.list);

    printf("list1 after list_replace(%d): ", second1.a);
    list_disp(&mylist1);

    printf("\n\n=======testing list_splice_tail()=======\n");
    /* splice the two lists */
    list_splice_tail(&mylist1, &mylist2);
    /* list_splice(&mylist1, &mylist2); */

    printf("list2 after list_splice(): ");
    list_disp(&mylist2);

    printf("\n\n========testing list_cut_position()========\n");
    /* cut mylist2 into two lists */
    struct list_head new_list;
    list_cut_position(&new_list, &mylist2, &first2.list);

    printf("list2 after list_cut_position(first2 = %d): ", first2.a);
    list_disp(&mylist2);

    printf("new list generated is: ");
    list_disp(&new_list);

    printf("\n\n=======testing list_move()=======\n");
    struct mylist *tmp = list_entry(new_list.next, struct mylist, list);   
    list_move(new_list.next, &mylist2);
    printf("list2 after list_move(new_list=%d): ", tmp->a);
    list_disp(&mylist2);

    printf("\n\n=======testing list_del()========\n");
    printf("list2 after list_del(%d): ", second2.a);
    list_del(&second2.list, &mylist2);
    list_disp(&mylist2);

    return 0;
}
