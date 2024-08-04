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

#define QUEUE_LENGTH_            (10)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_led_message_t))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/
static void turn_on_led(GPIO_TypeDef* port, uint16_t pin, uint16_t time) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    vTaskDelay(time);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

static void task_(void *argument) {
    ao_led_handle_t* hao_led = (ao_led_handle_t*)argument;
    ao_led_message_t msg;
    uint16_t time_on = hao_led->time_on;

    while (true) {
    	if (pdPASS == xQueueReceive(hao_led->hqueue, &msg, portMAX_DELAY)) {
			switch (msg) {
				case AO_LED_MESSAGE_RED_LED_ON:
				    LOGGER_INFO("AO LED: red led on");
					turn_on_led(LED_RED_PORT, LED_RED_PIN, time_on);
				    LOGGER_INFO("AO LED: red led off");
					break;

				case AO_LED_MESSAGE_GREEN_LED_ON:
					LOGGER_INFO("AO LED: green led on");
					turn_on_led(LED_GREEN_PORT, LED_GREEN_PIN, time_on);
				    LOGGER_INFO("AO LED: green led off");
					break;

				case AO_LED_MESSAGE_BLUE_LED_ON:
					LOGGER_INFO("AO LED: blue led on");
					turn_on_led(LED_BLUE_PORT, LED_BLUE_PIN, time_on);
				    LOGGER_INFO("AO LED: blue led off");
					break;

				default:
					LOGGER_INFO("AO LED: error message");
					break;
			}
    	}
    }
}


/********************** external functions definition ************************/

bool ao_led_send(ao_led_handle_t* hao, ao_led_message_t msg) {
    return (pdPASS == xQueueSend(hao->hqueue, (void*)&msg, 0));
}

bool ao_led_init(ao_led_handle_t* hao) {
    hao->hqueue = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
    if (NULL == hao->hqueue) {
      LOGGER_INFO("AO LED: error, queue creation");
      return false;
    }

    BaseType_t status;
    status = xTaskCreate(task_, "task_ao_led", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
    if (pdPASS != status) {
    	LOGGER_INFO("AO LED: error, task creation");
        return false;
    }

    LOGGER_INFO("AO LED: init");
    return true;
}

/********************** end of file ******************************************/

