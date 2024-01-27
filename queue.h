#ifndef _QUEUE_H_
#define _QUEUE_H_

// Include necessary standard C libraries
#include <stdlib.h>
#include <string.h>

//Set Queue elements type to char*
typedef char* Element;

// Node structure representing elements in the queue
typedef struct _Node {
    Element element;       // Data stored in the node
    struct _Node* next;    // Pointer to the next node in the queue
} Node;

// Queue structure representing a queue (First In First Out)
typedef struct {
    Node* first;    // Pointer to the first node in the queue
    Node* last;     // Pointer to the last node in the queue
} Queue;

// Function prototypes

/**
 * @brief Initializes a new queue.
 * 
 * @return The initialized queue.
 */
Queue QUEUE_init();

/**
 * @brief Adds an element to the end of the queue.
 * 
 * @param queue Pointer to the queue.
 * @param element The element to be added.
 */
void QUEUE_add(Queue* queue, Element element);

/**
 * @brief Gets and removes the element at the front of the queue.
 * 
 * @param queue Pointer to the queue.
 * @return The element at the front of the queue.
 */
Element QUEUE_get(Queue* queue);

/**
 * @brief Checks if the queue is empty.
 * 
 * @param queue The queue to be checked.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int QUEUE_isEmpty(Queue queue);

/**
 * @brief Destroys the queue and frees the memory.
 * 
 * @param queue Pointer to the queue.
 */
void QUEUE_destroy(Queue* queue);

#endif // _QUEUE_H_
