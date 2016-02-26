# mclib 
## **Get hands dirty and roll on my own implementation.**##



###list.h: implementation of single linked list and basic operations on it###
* **fucntions**

	* `list_hadd()` and `list_tadd()`, two insertion methods
	* `list_del()`, deletion of nodes
	* `list_push()` and `list_pop()`, linked list based stack construction
	* `list_mergesort()` and `list_quicksort()`, sorting with linked list
	* `detect_loop()`, cycle detection in the list
	
###list_generic.h: generic version of singlely circular linked list implementation###

* _**list node structure**_
		
		struct list_head
		{
			struct list_head *next;
		};
	
* _**Sample usage**_
	
		/* define your own data type and embed list_head structure in it */
		struct CONTAINER_STRUCTURE 
		{
			DATA_TYPE data; 		 /* whatever kind of data you need to process */
			struct list_head list;   /* the embedded list */
		};
		
		/* list initialization (two ways) */
		(1).
		struct CONTAINER_STRUCTURE mylist;
		INIT_LIST_HEAD(mylist);
		
		(2).
		LIST_HEAD(mylist);
		
		/* do you own stuff here to manipulate the list */
		
* **Useful Macros to traverse the list**
	* `list_for_each`
	* `list_for_each_entry`
		
		
* **functions**		
    * `list_add()` and `list_add_tail()`, two insertion methods
    * `list_replace()`
    * `list_del()`
    * `list_move()` and `list_move_tail()`
    * `list_splice()`
    * `list_cut_position`

## FreeBSD queue.h ##

  * Tail Queue (double/single linked)
  * Lists (double/single linked)
