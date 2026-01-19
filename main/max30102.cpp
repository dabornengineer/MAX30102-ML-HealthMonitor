/**
 * 
 */

#include <Wire.h>
#include "MAX30105.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "heartRate.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
void max30102_Task(void *pvParameters);

void init_MAX30102(void)
{
  //Serial.begin(115200);
  //Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    ESP_LOGI("MAX30102", "MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
    //ESP_LOGI("MAX30102", "Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  //particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  //particleSensor.setPulseAmplitudeRed(0x2F);
  //particleSensor.setPulseAmplitudeIR(0x2F);
  //xTaskCreate(max30102_Task, "max30102 task", 2048, NULL, 3, NULL);
}

void max30102_Task(void *pvParameters)
{
    bool fingerPresent = false;
    bool startupMessagePrinted = false;

    while (1)
    {
        long irValue = particleSensor.getIR();

        // ---------- FINGER PRESENT ----------
        if (irValue > 50000)
        {
            // Finger just placed
            if (!fingerPresent)
            {
                ESP_LOGI("MAX30102", "Finger detected. Reading heart rate...");
                fingerPresent = true;

                // Reset HR calculation
                rateSpot = 0;
                beatAvg = 0;
                lastBeat = millis();
            }

            // BPM calculation
            if (checkForBeat(irValue) == true)
            {
                long delta = millis() - lastBeat;
                lastBeat = millis();

                beatsPerMinute = 60.0 / (delta / 1000.0);

                if (beatsPerMinute > 20 && beatsPerMinute < 255)
                {
                    rates[rateSpot++] = (byte)beatsPerMinute;
                    rateSpot %= RATE_SIZE;

                    beatAvg = 0;
                    for (byte i = 0; i < RATE_SIZE; i++)
                        beatAvg += rates[i];
                    beatAvg /= RATE_SIZE;

                    ESP_LOGI("MAX30102",
                             "IR=%ld  BPM=%.1f  Avg BPM=%d",
                             irValue, beatsPerMinute, beatAvg);
                }
            }
        }
        // ---------- NO FINGER ----------
        else
        {
            // Startup message (printed once)
            if (!fingerPresent && !startupMessagePrinted)
            {
                ESP_LOGI("MAX30102", "No finger detected. Please place your finger.");
                startupMessagePrinted = true;
            }
            // Finger just removed
            else if (fingerPresent)
            {
                ESP_LOGI("MAX30102", "Finger removed. Please place your finger.");
                fingerPresent = false;
            }
        }

        //vTaskDelay(pdMS_TO_TICKS(1));
    }
}
