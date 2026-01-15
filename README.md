# MAX30102 ML Health Monitor

ESP32 project using the **MAX30102** sensor and **machine learning** to monitor **heart rate** and **SpO₂**. The system collects sensor data, processes it, and uses ML algorithms to detect anomalies or trends in real-time, enabling smart health monitoring on embedded devices.

## Features
- Real-time heart rate and SpO₂ monitoring
- Data collection from MAX30102 (IR and RED LEDs)
- Machine learning-based anomaly detection
- Runs entirely on ESP32 (no internet needed)
- Optional data logging for offline analysis

## Hardware Components
- ESP32 development board
- MAX30102 pulse oximeter / heart rate sensor
- Jumper wires
- Breadboard (optional)

### Pin Connections (typical)
| MAX30102 Pin | ESP32 Pin |
|--------------|-----------|
| VIN / VCC    | 3.3V      |
| GND          | GND       |
| SDA          | GPIO21    |
| SCL          | GPIO22    |
| INT          | Optional GPIO for interrupts |

## Software & Tools
- ESP-IDF
- FreeRTOS
- I2C communication
- Machine Learning (on-device inference)
- Python (optional for training ML models)

## How It Works
1. ESP32 reads IR and RED values from MAX30102 via I2C.  
2. Preprocess the signals (DC removal, smoothing, normalization).  
3. ML model runs inference to detect anomalies or predict heart health metrics.  
4. Output displayed on serial monitor, OLED, or web interface (optional).

## Future Improvements
- Real-time alerts for abnormal readings
- Integration with ESP32 SoftAP web dashboard
- On-device model training and adaptive learning
- Data logging to SD card or cloud

## Author
**DAborn Engineer**

