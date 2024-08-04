/*
 * ao_ui.c
 *
 *  Created on: Jun 30, 2024
 *      Author: lpmeoli
 */



/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "ao_ui.h"
#include "ao_led.h"

/********************** macros and definitions *******************************/

#define QUEUE_LENGTH_            (10)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_ui_message_t))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/
extern ao_led_handle_t hao_led;

/********************** internal functions definition ************************/

static void task_(void *argument) {
    ao_ui_handle_t* hao = (ao_ui_handle_t*)argument;

    while (true) {
        ao_ui_message_t msg;
        if (pdPASS == xQueueReceive(hao->hqueue, &msg, portMAX_DELAY)) {
            switch (msg) {
              case AO_UI_MESSAGE_BUTTON_NONE:
                  break;

              case AO_UI_MESSAGE_BUTTON_PULSE:
                  ao_led_send(&hao_led, AO_LED_MESSAGE_RED_LED_ON);
                  break;

              case AO_UI_MESSAGE_BUTTON_SHORT:
                  ao_led_send(&hao_led, AO_LED_MESSAGE_GREEN_LED_ON);
                  break;

              case AO_UI_MESSAGE_BUTTON_LONG:
                  ao_led_send(&hao_led, AO_LED_MESSAGE_BLUE_LED_ON);
                  break;

              default:
                  LOGGER_INFO("AO UI: error message");
                  break;
            }
        }
    }
}

/********************** external functions definition ************************/

bool ao_ui_send(ao_ui_handle_t* hao, ao_ui_message_t msg) {
    return (pdPASS == xQueueSend(hao->hqueue, (void*)&msg, 0));
}

bool ao_ui_init(ao_ui_handle_t* hao) {
    hao->hqueue = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
    if (NULL == hao->hqueue) {
    	LOGGER_INFO("AO UI: error, queue creation");
        return false;
    }

    BaseType_t status;
    status = xTaskCreate(task_, "task_ao_ui", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
    if (pdPASS != status) {
    	LOGGER_INFO("AO UI: error, task creation");
    	return false;
    }

    LOGGER_INFO("AO UI: init");
    return true;
}

/********************** end of file ******************************************/
