#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* 
 * This file is modified from Linux Kernel (include/linux/list.h) 
 * Here by copyright, credits attributed to wherever they belong.
 *
 * Li, Huaicheng (huaicheng@cs.uchicago.edu)
 * 
 * Thanks to Kulesh Shanmugasundaram (kulesh [squiggly] isis.poly.edu)
 * for providing the initial user-space *doubly* linked list version.
 */

/*
 * Simple singlely circular linked list implementation.
 *
 * _Generic Data_ handling by integrating list node into user's own data 
 * structure
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head
{
    struct list_head *next;
};

#define LIST_HEAD_INIT(name) { &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name);

void INIT_LIST_HEAD(struct list_head *head)
{
    head->next = head;
}

#define offsetof(type, member) (unsigned long)&(((type *)0)->member)

#define container_of(ptr, type, member) ({ \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)(__mptr) - offsetof(type, member)); })

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)    

/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define list_first_entry_or_null(ptr, type, member) \
	(!list_empty(ptr) ? list_first_entry(ptr, type, member) : NULL)

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); \
            pos = pos->next)

/**
 * list_get_tail - get the tail entry of the list
 * @head: list head to get tail from
 *
 * head entry is returned when list is empty
 */
struct list_head * list_get_tail(const struct list_head *head)
{
    struct list_head *pos, *tail = (struct list_head *)head;
    list_for_each(pos, head) {
        tail = pos;
    }

    return tail;
}

struct list_head * list_get_prev(struct list_head *entry)
{
    struct list_head *pos;
    for (pos = (entry)->next; pos != entry; pos = pos->next) {
        if (pos->next == entry)
            break;
    }

    return pos;
}

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member) \
    for (pos = list_entry((head)->next, typeof(*pos), member); \
            &pos->member != (head); \
            pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
bool list_empty(const struct list_head *head)
{
    return head->next == head;
}

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
void __list_add(struct list_head *new, struct list_head *head, struct list_head *next)
{
    head->next = new;
    new->next = next;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void list_add_tail(struct list_head *new, struct list_head *head)
{
    struct list_head *tail = list_get_tail(head); 

    __list_add(new, tail, tail->next);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
void __list_del(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 */
void list_del(struct list_head *entry, struct list_head *head)
{
	if (list_empty(head)) return;
	
    struct list_head *prev;
    for (prev = head->next; prev->next != entry; prev = prev->next);
    if (prev->next != entry) return; /* entry not in the list */

    __list_del(prev, entry->next);

    INIT_LIST_HEAD(entry);
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
void list_replace(struct list_head *old,
				struct list_head *new)
{
	new->next = old->next;
    struct list_head *prev = list_get_prev(old);
    prev->next = new;
}

void list_replace_init(struct list_head *old,
					struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
void list_move(struct list_head *list, struct list_head *head)
{
    struct list_head *prev;
    for (prev = list->next; prev->next != list; prev = prev->next);
    __list_del(prev, list->next);
    list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
void list_move_tail(struct list_head *list, struct list_head *head)
{
    struct list_head *prev;
    for (prev = list->next; prev->next != list; prev = prev->next);
    __list_del(prev, list->next);
    list_add_tail(list, head);
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
bool list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}

/**
 * list_rotate_left - rotate the list to the left
 * @head: the head of the list
 */
static inline void list_rotate_left(struct list_head *head)
{
	struct list_head *first;

	if (!list_empty(head)) {
		first = head->next;
		list_move_tail(first, head);
	}
}

static inline void __list_splice(const struct list_head *list,
				 struct list_head *prev,
				 struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list_get_tail(list);

	prev->next = first;
	last->next = next;
}

/**
 * list_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(const struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}

/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice_tail(struct list_head *list,
				struct list_head *head)
{
    struct list_head *tail = list_get_tail(head);
	if (!list_empty(list))
		__list_splice(list, tail, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}

/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void list_splice_tail_init(struct list_head *list,
					 struct list_head *head)
{
    struct list_head *tail = list_get_tail(head);
	if (!list_empty(list)) {
		__list_splice(list, tail, head);
		INIT_LIST_HEAD(list);
	}
}

/**
 * list_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int list_is_singular(const struct list_head *head)
{
	return !list_empty(head) && (head->next == list_get_tail(head));
}

#endif
