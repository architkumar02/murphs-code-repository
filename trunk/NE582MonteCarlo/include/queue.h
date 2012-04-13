/**
 * @file queue.h
 * @brief Function declarations of a queue implemented by a Linked List.
 *
 * @author Matthew J. Urffer
 */

#ifndef queue_h
#define queue_h 1


typedef struct NODE {
	void *value;			/* Value of the node */
	struct NODE *next;		/* Next node */
} node_t;

typedef struct QUEUE {
	node_t *head;					/* Head node of the List */
} queue_t;

/**
 * @brief Inserts a given node in the list 
 * 
 * @param queue Queue to add the value to
 * @param value Memory address to store value in
 * @return true or false if instert was sucesful
 */
int push(queue_t *queue, void* value);

/**
 * @brief Returns a given node from the list
 *
 * @param Queue to return the value
 * @return the first node in the list, or NULL if the queue is empty
 */
void *pop(queue_t *queue);

/**
 * @brief Checks if the queue is empty or not
 *
 * @param Queue in question
 * @return True if empty, false if not
 */
int isEmpty(queue_t *queue);

/**
 * @brief Initilizes a Queue
 */
int initilizeQueue(queue_t *queue);

#endif