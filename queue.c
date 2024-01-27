#include "queue.h"

/**
 * @brief Initializes a new queue.
 * 
 * @return The initialized queue.
 */
Queue QUEUE_init() {
    Queue newQueue;
    newQueue.first = NULL;
    newQueue.last = NULL;
    return newQueue;
}

/**
 * @brief Adds an element to the end of the queue.
 * 
 * @param queue Pointer to the queue.
 * @param element The element to be added.
 */
void QUEUE_add(Queue* queue, Element element) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->element = element;
    newNode->next = NULL;

    if (QUEUE_isEmpty(*queue)) {
        queue->first = newNode;
    } else {
        queue->last->next = newNode;
    }
	queue->last = newNode;

}

/**
 * @brief Gets and removes the element at the front of the queue.
 * 
 * @param queue Pointer to the queue.
 * @return The element at the front of the queue.
 */
Element QUEUE_get(Queue* queue) {
    Element element;
    if (!QUEUE_isEmpty(*queue)) {
        Node* firstNode = queue->first;
        element = strdup(firstNode->element);

        queue->first = firstNode->next;
        free(firstNode->element);
        free(firstNode);

        // If the queue is empty after removing the element, update the 'last' pointer as well.
        if (queue->first == NULL) {
            queue->last = NULL;
        }
    }
    return element;
}

/**
 * @brief Checks if the queue is empty.
 * 
 * @param queue The queue to be checked.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int QUEUE_isEmpty(Queue queue) {
    return queue.first == NULL;
}

/**
 * @brief Destroys the queue and frees the memory.
 * 
 * @param queue Pointer to the queue.
 */
void QUEUE_destroy(Queue* queue) {
    while (!QUEUE_isEmpty(*queue)) {
        Element element = QUEUE_get(queue);
        free(element);
    }
}
