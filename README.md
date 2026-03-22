# Smart Waste Water Treatment Plant — Arduino & ESP8266 IoT Monitoring and Control System

---

## Project Description

The **Smart Waste Water Treatment Plant (SWWTP)** is an embedded IoT system that automates a multi-tank wastewater treatment process using an **Arduino Mega** for plant operations and an **ESP8266** for cloud connectivity via the **Arduino IoT Cloud**.

The Arduino Mega orchestrates the full treatment pipeline across four tanks — controlling 8 relay-driven actuators, reading a pH sensor, turbidity sensor, DS18B20 temperature sensor, and a metal detection sensor, and displaying live status on a 16x2 I2C LCD. Operators interact with the system through a 4x4 keypad and a physical toggle switch that switches the system between **Manual** and **Automatic** modes.

The ESP8266 receives sensor readings from the Arduino Mega over SoftwareSerial and pushes them to the **Arduino IoT Cloud dashboard** in real time — streaming metal detection status, pH level, turbidity, and temperature.

Water quality is automatically classified into four grades (A, B, C, D) based on pH and turbidity thresholds, and a built-in **Neutralization** routine automatically adjusts pH by triggering acid (MA) or base (MB) chemical dosing relays.

---

## How to Install and Run the Project

### Prerequisites

| Requirement | Details |
|---|---|
| Arduino IDE | 2.x or later |
| Arduino Mega 2560 | Main controller for plant operations |
| ESP8266 module | Handles Wi-Fi and Arduino IoT Cloud |
| Arduino IoT Cloud account | For dashboard and data logging |
| USB cable | For flashing both boards |

### Required Libraries

Install the following via **Arduino IDE → Tools → Manage Libraries**:

- `ArduinoIoTCloud` — Arduino IoT Cloud connectivity (ESP8266 sketch)
- `Arduino_ConnectionHandler` — Wi-Fi connection handler (ESP8266 sketch)
- `LiquidCrystal_I2C` — 16x2 I2C LCD display
- `Keypad` — 4x4 matrix keypad
- `OneWire` — OneWire bus for DS18B20
- `DallasTemperature` — DS18B20 temperature sensor
- `SoftwareSerial` — Serial communication between Mega and ESP8266

### Step 1 — Clone the Repository

```bash
git clone https://github.com/AbhinavKotagi/Smart-Waste-Water-Treatment-Plant.git
cd Smart-Waste-Water-Treatment-Plant
```

### Step 2 — Configure Wi-Fi and Cloud Credentials

Open `arduino_secrets.h` and fill in your credentials:

```cpp
#define SECRET_SSID "Your_WiFi_Name"
#define SECRET_PASS "Your_WiFi_Password"
#define SECRET_DEVICE_KEY "Your_Arduino_IoT_Device_Key"
```

Also update `thingProperties.h` with your **Device Login Name** from your Arduino IoT Cloud Thing page.

> ⚠️ Never commit `arduino_secrets.h` with real credentials to a public repository.

### Step 3 — Flash the Arduino Mega (Plant Controller)

1. Open the `SWWTP_ManAuto/` folder in Arduino IDE.
2. Go to **Tools → Board** and select `Arduino Mega 2560`.
3. Select the correct COM port under **Tools → Port**.
4. Click **Upload**.

### Step 4 — Flash the ESP8266 (IoT Cloud Module)

1. Open the `SWWTP_espCode/` folder in Arduino IDE.
2. Go to **Tools → Board** and select your ESP8266 board (e.g., `NodeMCU 1.0`).
3. Select the correct COM port under **Tools → Port**.
4. Click **Upload**.

### Step 5 — Wire Up the Hardware

Connect components to the Arduino Mega as defined in the source code:

| Component | Pin |
|---|---|
| Turbidity Sensor | A4 |
| pH Sensor | A2 |
| DS18B20 Temperature Sensor | A6 |
| Metal Detection Sensor | D49 |
| Float Switch 1 | D53 |
| Float Switch 2 | D51 |
| Toggle Switch — Auto | D3 |
| Toggle Switch — Manual | D2 |
| Relay 1–8 | D37, D35, D33, D31, D29, D27, D25, D23 |
| Keypad Rows | D36, D34, D32, D30 |
| Keypad Columns | D28, D26, D24, D22 |
| LCD (I2C) | SDA/SCL (I2C address: 0x25) |
| Warmup LED | D12 |
| ESP8266 SoftwareSerial RX/TX | D4, D5 |

---

## How to Use the Project

### Startup

On power-up, the Arduino Mega runs a warmup LED blink sequence, initializes all relays to OFF (HIGH), and displays a welcome message on the LCD:

```
WELCOME TO
SWWTP
```

### Selecting an Operating Mode

Use the **physical toggle switch** to select a mode:

| Toggle Position | Mode |
|---|---|
| Toggle 1 (D3) LOW | **Automatic Mode** |
| Toggle 2 (D2) LOW | **Manual Mode** |

---

### Automatic Mode — Keypad Controls

In **Auto Mode**, press these keys on the 4x4 keypad:

| Key | Action |
|---|---|
| `3` | Run full treatment pipeline: Tank 2 → Tank 3 → Tank 4 |
| `6` | Send stored sensor data to ESP8266 and push to Arduino IoT Cloud |
| `#` | Enter Lab Mode — classify water quality (A / B / C / D) |
| `B` | Display real-time sensor readings on LCD |
| `C` | Display last stored sensor readings on LCD |

---

### Manual Mode — Keypad Controls

In **Manual Mode**, each tank process can be triggered individually:

| Key | Action |
|---|---|
| `1` | Tank 1 — prompt to pour water in |
| `4` | Tank 2 — aeration process (Relay 1, 60s) |
| `7` | Tank 3 — filtration + pH neutralization |
| `*` | Tank 4 — final settling process (Relay 8) |
| `6` | Send data to cloud |
| `#` | Enter Lab Mode |
| `B` | Real-time sensor data page |
| `C` | Stored sensor data page |

---

### Treatment Pipeline (Auto Mode — Key `3`)

The full automated pipeline runs across three stages:

**Tank 2 — Aeration**
Relay 1 activates for 60 seconds to aerate the wastewater.

**Tank 3 — Filtration & pH Neutralization**
- Relay 2 runs for 60 seconds (primary filtration).
- Relays 3 & 4 activate sequentially for chemical dosing.
- pH is read and the **Neutralization** routine runs 2 cycles:
  - If pH > 8 → **MAprocess** (acid dosing via Relay 6)
  - If pH < 6.5 → **MBprocess** (base dosing via Relay 7)
- Target neutral range: **pH 6.5 – 8.0**

**Tank 4 — Final Settling**
Relay 8 runs in timed pulses (30s ON → 10s OFF → 2s ON) for final solids settling.

---

### Lab Mode & Water Classification (Key `#`)

Pressing `#` triggers Lab Mode, which reads all sensors and classifies the treated water:

| Class | pH Range | Turbidity (raw analog) |
|---|---|---|
| A | 7.0 – 7.5 | 1 – 50 |
| B | 6.5 – 8.0 | 51 – 200 |
| C | 5.5 – 8.0 | 201 – 500 |
| D | Outside above ranges | — |

The classification result is shown on the LCD.

---

### Arduino IoT Cloud Dashboard

The ESP8266 receives a comma-separated data string from the Arduino Mega over SoftwareSerial in this format:

```
<MetalDetect>,<pH>,<Turbidity>,<Temperature>
```

The following variables are streamed live to the **Arduino IoT Cloud** dashboard:

| Cloud Variable | Sensor |
|---|---|
| `metalDetect` | Metal detection sensor (D49) |
| `pH` | pH sensor (A2) |
| `tURBIDITY` | Turbidity sensor (A4) |
| `tEMP` | DS18B20 temperature sensor (A6) |

Log into [create.arduino.cc/iot](https://create.arduino.cc/iot) to view your live dashboard and historical data.

---

## Project Structure

```
Smart-Waste-Water-Treatment-Plant/
│
├── SWWTP_espCode/
│   ├── SWWTP_espCode.ino       # ESP8266 firmware — receives data from Mega,
│   │                           # pushes to Arduino IoT Cloud
│   ├── thingProperties.h       # Auto-generated IoT Cloud properties & variables
│   └── arduino_secrets.h       # Wi-Fi SSID, password, and device secret key
│
└── SWWTP_ManAuto/
    ├── SWWTP_ManAuto.ino       # Arduino Mega main sketch — setup, loop,
    │                           # mode switching, keypad handling
    └── 02_SWWTP_MA.ino         # Tank process functions, neutralization logic,
                                # lab testing, classification, LCD pages, SendData
```
