
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
	priority_level_t highestPriority = NONE_PRIORITY;
    int index = 0;

    for (int i = 0; i < hqueue->size; i++) {
        if (highestPriority < hqueue->queue[i].priority) {
            highestPriority = hqueue->queue[i].priority;
            index = i;

            if(highestPriority == HIGH_PRIORITY) {
            	break;
            }
        }
    }

    return index;
}
/********************** external functions definition ************************/

priority_queue_handle_t * priority_queue_create() {
	priority_queue_handle_t * new_queue;

	new_queue = (priority_queue_handle_t *) pvPortMalloc(sizeof(priority_queue_handle_t));

	configASSERT(new_queue);

	new_queue->size = 0;
	new_queue->mutex_h = xSemaphoreCreateMutex();
	configASSERT(NULL != new_queue->mutex_h);

	memset(new_queue->queue, 0, QUEUE_SIZE * sizeof(item_t));

	return new_queue;
}

void priority_queue_delete(priority_queue_handle_t * hqueue) {
	configASSERT(hqueue);
	vPortFree(hqueue);
}

void priority_queue_enqueue(priority_queue_handle_t * hqueue, item_t new_item) {
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

