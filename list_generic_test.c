#include <stdio.h>
#include <stdlib.h>

#include "/Users/coperd/git/mclib/list_generic.h"

struct mylist
{
	int a;
	struct list_head list;
};

int main()
{
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
	
	list_add(&(first1.list), &mylist1);
	list_add(&(second1.list), &mylist1);

	struct mylist *pos1;
	printf("list1: ");
	list_for_each_entry(pos1, &mylist1, list) {
		printf("%d, ", pos1->a);
	}
	printf("\n");
	
	struct mylist rp = {
		.a = 88,
		.list = LIST_HEAD_INIT(rp.list)
	};
	list_replace(&second1.list, &rp.list);
	
	printf("list1: ");
	list_for_each_entry(pos1, &mylist1, list) {
		printf("%d, ", pos1->a);
	}
	printf("\n");
	
    // second list
	struct mylist first2 = {
		.a = 21,
		.list = LIST_HEAD_INIT(first2.list)
	};

	struct mylist second2;
	second2.a = 22;
	INIT_LIST_HEAD(&second2.list);
	list_add(&(first2.list), &mylist2);
	list_add(&second2.list, &mylist2);
	
	struct mylist *pos2;
	printf("list2: ");
	list_for_each_entry(pos2, &mylist2, list) {
		printf("%d, ", pos2->a);
	}
	printf("\n");

	list_splice_tail(&mylist1, &mylist2);
	
	
	printf("list2: ");
	list_for_each_entry(pos2, &mylist2, list) {
		printf("%d, ", pos2->a);
	}
	printf("\n");

	return 0;
}
