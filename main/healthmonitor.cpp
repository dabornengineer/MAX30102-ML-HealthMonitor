/**
 * 
 */
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "main.h"

extern "C" void app_main()
{
    //init_heartrate();
    init_spo2();
    //xTaskCreate(heartrate_Task, "MAX30102 Task", 8192, NULL, 1, NULL);
    xTaskCreate(spo2_task, "Sp02_task", 8192, NULL, 3, NULL);
    while(true)
    {
        vTaskDelay(portMAX_DELAY);
    }
}