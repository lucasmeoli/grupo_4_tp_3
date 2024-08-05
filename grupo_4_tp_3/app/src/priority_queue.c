
/********************** inclusions *******************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "logger.h"

#include "cmsis_os.h"
#include "priority_queue.h"

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/
/**
 * Retrieves the index of the item with the highest priority in the queue.
 *
 * @param hqueue The handle to the priority queue.
 * @return The index of the item with the highest priority in the queue.
 */
static int peek(priority_queue_handle_t * hqueue) {
	priority_level_t highest_priority = NONE_PRIORITY;
    int index = 0;

    for (int i = 0; i < hqueue->size; i++) {
        if (highest_priority < hqueue->queue[i].priority) {
        	highest_priority = hqueue->queue[i].priority;
            index = i;

            if(highest_priority == HIGH_PRIORITY) {
            	break;
            }
        }
    }

    return index;
}
/********************** external functions definition ************************/

priority_queue_handle_t * priority_queue_create(UBaseType_t queue_size) {
	priority_queue_handle_t * hqueue;

	hqueue = (priority_queue_handle_t *) pvPortMalloc(sizeof(priority_queue_handle_t));
	configASSERT(hqueue);

	hqueue->queue = (item_t *) pvPortMalloc(sizeof(item_t) * queue_size);
	configASSERT(hqueue->queue);

	hqueue->size = 0;
	hqueue->mutex_h = xSemaphoreCreateMutex();
	configASSERT(NULL != hqueue->mutex_h);

	memset(hqueue->queue, 0, queue_size * sizeof(item_t));

	return hqueue;
}

void priority_queue_delete(priority_queue_handle_t * hqueue) {
	configASSERT(hqueue);
	configASSERT(hqueue->queue);
	vPortFree(hqueue->queue);
	vPortFree(hqueue);
}

void priority_queue_enqueue(priority_queue_handle_t * hqueue, item_t new_item) {
	configASSERT(hqueue);
	xSemaphoreTake(hqueue->mutex_h, portMAX_DELAY);
	{
	    // Insert new element
	    hqueue->queue[hqueue->size].value = new_item.value;
	    hqueue->queue[hqueue->size].priority = new_item.priority;

	    hqueue->size++;
	    xSemaphoreGive(hqueue->mutex_h);
	}
}

int16_t priority_queue_dequeue(priority_queue_handle_t * hqueue) {
	int16_t ret = -1;

	configASSERT(hqueue);
	xSemaphoreTake(hqueue->mutex_h, portMAX_DELAY);
	{
		if (hqueue->size != 0) {
			int index = peek(hqueue);

			ret = hqueue->queue[index].value;

			for (int i = index; i < hqueue->size-1; i++) {
				hqueue->queue[i] = hqueue->queue[i + 1];
			}

			hqueue->size--;
		}

		xSemaphoreGive(hqueue->mutex_h);
	}

    return ret;
}

/********************** end of file ******************************************/

