/*
 * priority_queue.h
 *
 *  Created on: Aug 4, 2024
 *      Author: lpmeoli
 */

#ifndef INC_PRIORITY_QUEUE_H_
#define INC_PRIORITY_QUEUE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/********************** macros ***********************************************/

/********************** typedef **********************************************/
typedef enum {
	NONE_PRIORITY = 0,
	LOW_PRIORITY = 1,
	MEDIUM_PRIORITY = 2,
	HIGH_PRIORITY = 3,
	PRIORITY_LEVEL_COUNT,
} priority_level_t;

typedef struct {
    int16_t value;
    priority_level_t priority;
} item_t;

typedef struct {
	item_t * queue;
	UBaseType_t max_size;
	int16_t size;
	SemaphoreHandle_t mutex_h;
} priority_queue_handle_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
/**
 * Creates a new priority queue and initializes its components.
 *
 * @return A pointer to the newly created priority queue handle.
 */
priority_queue_handle_t * priority_queue_create(UBaseType_t queue_size);

/**
 * Deletes the priority queue and frees associated memory.
 *
 * @param hqueue The handle to the priority queue to delete.
 */
void priority_queue_delete(priority_queue_handle_t * hqueue);

/**
 * Enqueues a new item into the priority queue.
 *
 * @param hqueue The handle to the priority queue.
 * @param new_item The item to enqueue in the priority queue.
 */
void priority_queue_enqueue(priority_queue_handle_t * hqueue, item_t new_item);

/**
 * Dequeues the highest priority item from the priority queue and returns its value.
 *
 * @param hqueue The handle to the priority queue.
 * @return The value of the dequeued item, or -1 if the queue is empty.
 */
int16_t priority_queue_dequeue(priority_queue_handle_t * hqueue);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif


#endif /* INC_PRIORITY_QUEUE_H_ */

/********************** end of file ******************************************/
