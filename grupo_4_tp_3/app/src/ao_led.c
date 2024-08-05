/********************** inclusions *******************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"


#include "ao_led.h"

/********************** macros and definitions *******************************/
#define QUEUE_SIZE_              (10)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_led_message_t))

#define TASK_PERIOD_MS_          (500)

#define TIME_LED_ON_MS			 5000
/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
priority_queue_handle_t queue;

/********************** external data definition *****************************/

/********************** internal functions definition ************************/
static void turn_on_led(GPIO_TypeDef* port, uint16_t pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    vTaskDelay(TIME_LED_ON_MS);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

static void task_(void *argument) {
    ao_led_handle_t* hao_led = (ao_led_handle_t*)argument;
    ao_led_message_t msg;

    while (true) {
    	msg = (ao_led_message_t) priority_queue_dequeue(hao_led->hqueue);

		switch (msg) {
			case AO_LED_MESSAGE_RED_LED_ON:
				LOGGER_INFO("AO LED: red led on");
				turn_on_led(LED_RED_PORT, LED_RED_PIN);
				LOGGER_INFO("AO LED: red led off");
				break;

			case AO_LED_MESSAGE_GREEN_LED_ON:
				LOGGER_INFO("AO LED: green led on");
				turn_on_led(LED_GREEN_PORT, LED_GREEN_PIN);
				LOGGER_INFO("AO LED: green led off");
				break;

			case AO_LED_MESSAGE_BLUE_LED_ON:
				LOGGER_INFO("AO LED: blue led on");
				turn_on_led(LED_BLUE_PORT, LED_BLUE_PIN);
				LOGGER_INFO("AO LED: blue led off");
				break;

			default:
				break;
		}

		vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
    }

    priority_queue_delete(hao_led->hqueue);
}


/********************** external functions definition ************************/
void ao_led_send(ao_led_handle_t* hao, item_t item) {
	priority_queue_enqueue(hao->hqueue, item);
}

void ao_led_init(ao_led_handle_t* hao) {
	hao->hqueue = priority_queue_create(QUEUE_SIZE_);
	configASSERT(hao->hqueue);

    BaseType_t status;
    status = xTaskCreate(task_, "task_ao_led", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
    configASSERT(pdPASS == status);

    LOGGER_INFO("AO LED: init");
}

/********************** end of file ******************************************/

