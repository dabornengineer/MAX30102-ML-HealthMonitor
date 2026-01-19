/**
 * 
 */
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "main.h"

extern "C" void app_main()
{
    init_MAX30102();
    xTaskCreate(max30102_Task, "MAX30102 Task", 2048, NULL, 3, NULL);
}