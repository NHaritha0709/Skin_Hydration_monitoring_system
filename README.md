# Skin Hydration Monitoring Using IR Spectroscopy

An ESP32-based IoT prototype for monitoring skin-surface optical reflectance and moisture readings, displaying sensor values locally, and transmitting measurements to cloud platforms for remote observation.

## Project Overview

Skin hydration is an important factor in maintaining the skin's protective barrier. This project explores a low-cost, non-invasive approach to estimating skin-surface hydration-related readings using an infrared (IR) reflective sensor. A moisture sensor provides an additional measurement for comparison.

An ESP32 microcontroller acquires the sensor readings, converts them into scaled values, and presents the information through a 16×2 I2C LCD. The prototype also connects to the Blynk IoT platform for remote monitoring and notifications and sends measurements to ThingSpeak for cloud-based data logging.

> **Important:** This repository contains the current ESP32 firmware implementation. The uploaded project report also discusses DHT22 environmental sensing, machine-learning classification and a web dashboard. Those features are not implemented in the current `AI_Hydration_Monitor.ino` file and should not be described as completed features until their hardware, software and results are added and verified.

## Key Features

- Reads analog values from an IR reflective sensor.
- Reads analog values from a moisture sensor.
- Scales the IR reading and maps the moisture reading to a percentage-like value.
- Displays readings and status messages on a 16×2 I2C LCD.
- Sends IR and moisture measurements to ThingSpeak using an HTTP request.
- Connects to Blynk for remote monitoring and event notifications.
- Uses configurable threshold conditions in the firmware to display hydration-related status messages.

## System Architecture

```text
       IR Reflective Sensor
                |
                v
         ESP32 ADC Input <----- Moisture Sensor
                |
                v
       Reading Conversion
         and Thresholding
                |
        +-------+--------+
        |                |
        v                v
    16x2 I2C LCD     Wi-Fi Connection
                         |
                  +------+------+
                  |             |
                  v             v
                Blynk        ThingSpeak
           Monitoring and   Cloud Data
            Notifications     Logging
```

## Hardware Requirements

| Component | Purpose |
|---|---|
| ESP32 development board | Reads sensor inputs and manages Wi-Fi communication |
| TCRT5000 IR reflective sensor module | Provides an optical reflectance-related analog reading |
| Analog moisture sensor | Provides an additional moisture-related analog reading |
| 16×2 I2C LCD | Displays readings and status messages |
| Buck converter (as required by the power design) | Provides the required regulated supply |
| Jumper wires and breadboard/PCB | Hardware connections |
| USB cable and suitable power source | Programming and powering the prototype |

**Note:** The report mentions a DHT22 temperature and humidity sensor. It is not read by the firmware currently present in this repository. Add and test its wiring and code before listing it as an operational component.

## Software and Platforms

- **Arduino IDE** – firmware development and uploading.
- **ESP32 Arduino Core** – ESP32 board support.
- **Blynk IoT** – remote monitoring and event notifications.
- **ThingSpeak** – cloud logging of sensor readings.
- **Required Arduino libraries:**
  - `WiFi.h` – Wi-Fi connectivity (provided with the ESP32 Arduino Core).
  - `BlynkSimpleEsp32.h` – Blynk connectivity.
  - `LCD_I2C.h` – I2C LCD control.
  - `HTTPClient.h` – HTTP communication (provided with the ESP32 Arduino Core).

Install the ESP32 board package and the required third-party libraries through the Arduino IDE Library Manager or their official installation instructions.

## Pin Connections

The pin assignments below are taken from the current firmware.

| Module | ESP32 connection |
|---|---|
| IR sensor analog output | GPIO 33 |
| Moisture sensor analog output | GPIO 32 |
| I2C LCD SDA | ESP32 default I2C SDA pin for the selected board |
| I2C LCD SCL | ESP32 default I2C SCL pin for the selected board |
| LCD I2C address | `0x27` |

Connect sensor power and ground according to each module's specifications. Ensure that the voltage applied to ESP32 GPIO pins does not exceed the board's permitted input voltage. Confirm the I2C pins for your particular ESP32 board before wiring.

## Firmware Configuration

1. Install Arduino IDE and configure it for the ESP32 board.
2. Install the Blynk and LCD I2C libraries.
3. Open `AI_Hydration_Monitor.ino`.
4. Configure your own Wi-Fi credentials and Blynk details in the firmware.
5. Configure your ThingSpeak Write API Key.
6. Confirm the Blynk template, event and virtual-pin settings.
7. Select the correct ESP32 board and COM port, then compile and upload the sketch.

### Credentials and API keys

Do not publish Wi-Fi passwords, Blynk authentication tokens or ThingSpeak Write API Keys in a public repository. Store them in a separate, untracked configuration file or use another secure configuration method. If real credentials have already been committed or shared, regenerate/revoke the exposed tokens and passwords where applicable.

For a public GitHub repository, replace any credentials in the source code with placeholders before committing.

## Blynk Configuration

The current firmware uses the following Blynk resources:

| Resource | Use in the current firmware |
|---|---|
| Virtual Pin `V0` | Sends the scaled IR reading |
| Virtual Pin `V2` | Receives a button value through `BLYNK_WRITE(V2)` |
| Event name `msg` | Sends hydration-related event notifications |

Create the matching template, datastreams and event in your Blynk account. The event code in the firmware expects an event named `msg`. Configure the dashboard widgets to use the corresponding virtual pins.

## ThingSpeak Configuration

The firmware sends sensor values to ThingSpeak through its HTTP update endpoint.

| ThingSpeak field | Data sent |
|---|---|
| Field 1 | Scaled IR reading |
| Field 2 | Moisture percentage-like value |

Create a ThingSpeak channel with Field 1 and Field 2 enabled. Add your channel's Write API Key to the firmware configuration. Keep the key private.

## Working Principle

1. The ESP32 initializes the serial interface, LCD and sensor input pins.
2. The board connects to the configured Wi-Fi network and starts the Blynk connection.
3. The IR sensor is read through GPIO 33. The firmware maps the ADC reading to a scaled value.
4. The moisture sensor is read through GPIO 32. Its raw ADC value is mapped using the calibration endpoints currently specified in the sketch (4095 for dry and 1700 for wet), then constrained to the range 0–100.
5. The LCD displays the readings and status messages based on the threshold conditions in the firmware.
6. The firmware sends the scaled IR and moisture values to ThingSpeak and updates Blynk where configured.
7. Blynk event notifications are triggered when the programmed status conditions are met.

## Calibration and Interpretation

The current sketch uses fixed mapping endpoints and threshold ranges. These values are prototype settings and may need to be recalibrated for the specific sensor modules, mounting arrangement, skin contact conditions, lighting, power supply and test environment.

The displayed values are sensor-derived estimates. They should not be interpreted as clinically validated skin-hydration measurements or as a medical diagnosis. Validate the sensing method against an appropriate reference instrument and a controlled test procedure before making health-related claims.

## Repository Structure

```text
AI_SKIN_HYDRATION_MONITOR/
├── AI_Hydration_Monitor.ino
├── README.md
└── FINAL REPORT BHS (1).pdf
```

The report PDF is included in the supplied project archive. If you publish the source repository, check that you have permission to share the report and that it contains no personal or confidential information.

## Current Implementation and Future Enhancements

### Implemented in the uploaded firmware

- ESP32-based analog sensor acquisition.
- IR and moisture sensor reading conversion.
- 16×2 I2C LCD output.
- Blynk connectivity and event notifications.
- ThingSpeak HTTP data logging.

### Potential future enhancements

- Integrate the DHT22 sensor and use temperature/humidity readings for environmental compensation.
- Collect a labelled dataset under a controlled measurement protocol.
- Train and validate a machine-learning model, then integrate the model into the ESP32 workflow if resource constraints permit.
- Develop the web dashboard described in the report and connect it to verified cloud data.
- Add sensor fault detection, improved calibration routines and robust network error handling.
- Evaluate repeatability and compare estimates with a suitable reference measurement method.

## Limitations

- Sensor readings depend on calibration, placement and environmental conditions.
- The current firmware uses fixed thresholds rather than a trained machine-learning classifier.
- DHT22 integration and the report's web-dashboard/AI recommendation features are not present in the supplied sketch.
- Cloud transmission depends on Wi-Fi and the availability/configuration of Blynk and ThingSpeak services.
- This prototype is intended for educational and experimental use, not for medical diagnosis or treatment decisions.

