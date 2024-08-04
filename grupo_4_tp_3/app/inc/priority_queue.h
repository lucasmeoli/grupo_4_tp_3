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

#define QUEUE_SIZE 		30

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
	item_t queue[QUEUE_SIZE];
	int16_t size;
	SemaphoreHandle_t mutex_h;
} priority_queue_handle_t;


/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
void priority_queue_create(priority_queue_handle_t * hqueue);
void priority_queue_enqueue(priority_queue_handle_t * hqueue, item_t new_item);
int16_t priority_queue_dequeue(priority_queue_handle_t * hqueue);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif


#endif /* INC_PRIORITY_QUEUE_H_ */

/********************** end of file ******************************************/
