#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "main.h"

static const char *TAG = "SPO2";

MAX30105 particleSensor;

/* ---------- INIT ---------- */
void init_spo2(void)
{
    Wire.begin(25, 26);          // SDA, SCL
    Wire.setClock(400000);

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
    {
        ESP_LOGE(TAG, "MAX30102 not found");
        vTaskDelay(portMAX_DELAY);
    }

    // SpO2-specific configuration
    byte ledBrightness = 60;
    byte sampleAverage = 4;
    byte ledMode = 2;            // RED + IR (REQUIRED)
    byte sampleRate = 100;
    int pulseWidth = 411;
    int adcRange = 4096;

    particleSensor.setup(
        ledBrightness,
        sampleAverage,
        ledMode,
        sampleRate,
        pulseWidth,
        adcRange
    );

    particleSensor.setPulseAmplitudeGreen(0); // disable green LED
}

/* ---------- TASK ---------- */
void spo2_task(void *pvParameters)
{
    static uint32_t redBuffer[100];
    static uint32_t irBuffer[100];

    int32_t spo2;
    int8_t validSPO2;
    int32_t heartRate;
    int8_t validHeartRate;

    const int bufferLength = 100;

    ESP_LOGI(TAG, "Collecting initial SpO2 samples...");

    /* Initial buffer fill */
    for (int i = 0; i < bufferLength; i++)
    {
        while (!particleSensor.available())
            particleSensor.check();

        redBuffer[i] = particleSensor.getRed();
        irBuffer[i]  = particleSensor.getIR();
        particleSensor.nextSample();

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    /* Main loop */
    while (1)
    {
        // Shift data
        memmove(redBuffer, redBuffer + 25, 75 * sizeof(uint32_t));
        memmove(irBuffer,  irBuffer  + 25, 75 * sizeof(uint32_t));

        // Read new samples
        for (int i = 75; i < 100; i++)
        {
            while (!particleSensor.available())
                particleSensor.check();

            redBuffer[i] = particleSensor.getRed();
            irBuffer[i]  = particleSensor.getIR();
            particleSensor.nextSample();
        }

        // Calculate SpO2
        maxim_heart_rate_and_oxygen_saturation(
            irBuffer,
            bufferLength,
            redBuffer,
            &spo2,
            &validSPO2,
            &heartRate,
            &validHeartRate
        );

        if (validSPO2)
        {
            ESP_LOGI(TAG, "SpO2 = %ld %%", spo2);
        }
        else
        {
            ESP_LOGW(TAG, "SpO2 invalid");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
