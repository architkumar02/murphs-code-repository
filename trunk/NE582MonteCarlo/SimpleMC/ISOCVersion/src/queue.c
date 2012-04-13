/**
 * @file queue.h
 * @brief Function declarations of a queue implemented by a Linked List.
 *
 * @author Matthew J. Urffer
 */


#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"


/**
 * @brief Inserts a given node in the list 
 * 
 * @param queue Queue to add the value to
 * @param value Memory address to store value in
 * @return true or false if instert was sucesful
 */
int push(queue_t *queue, void* value){
	// Allocating Memory for a new node
	node_t* node = (node_t*) Malloc(sizeof(node_t));
	
	// Assinging Contents
	node->value = value;
	
	// Adding node to the front
	node->next = queue->head;
	queue->head = node;
	return 0;
}

/**
 * @brief Returns a given node from the list
 *
 * @param Queue to return the value
 * @return the first node in the list, or NULL if the queue is empty
 */
node_t *node;		// Node of the queue
void *value;		// Value to return
void *pop(queue_t *queue){
	// Checking for an empty list
	if ( isEmpty(queue))
		return NULL;
	
	// Getting node to return, then reseting head
	node = queue->head;
	queue->head = node->next;
	
	// Getting the value it stores, then freeing the node
	value = node->value;
	free(node);
	
	// Returning the value
	return value;
}

/**
 * @brief Checks if the queue is empty or not
 *
 * @param Queue in question
 * @return True if empty, false if not
 */
int isEmpty(queue_t *queue){
	// List is empty if head node is null
	return (queue->head == NULL);
			
}

/**
 * @brief Initilizes a Queue
 */
int initilizeQueue(queue_t *queue){
	queue->head = NULL;
	return 0;
}