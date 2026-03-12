# 📡 CAN Bus — Simulation of CAN Protocol

![Platform](https://img.shields.io/badge/Platform-Arduino%20UNO-00979D?logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%20%2F%20C%2B%2B-00599C?logo=c&logoColor=white)
![Protocol](https://img.shields.io/badge/Protocol-CAN%202.0A%2F2.0B-1A3A5C)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen)

A hardware-level simulation of the **Controller Area Network (CAN)** protocol — the backbone communication standard used in virtually every modern vehicle's ECU network.

---

## 🔍 Overview

This project implements full CAN bus communication between **two Arduino UNO boards** using **MCP2515 CAN controllers** connected over SPI. The transmitter node reads real-time temperature and humidity data from a DHT22 sensor and sends it over the CAN bus. The receiver node decodes and displays the incoming frames.

> CAN bus is used in every modern vehicle — from engine management and ABS to airbag deployment and infotainment. This project simulates that ECU-level communication at the hardware layer.

---

## ⚙️ Hardware Used

| Component | Quantity | Role |
|---|---|---|
| Arduino UNO R3 | 2 | Node A (TX) + Node B (RX) |
| MCP2515 CAN Module (TJA1050) | 2 | CAN controller + transceiver |
| DHT22 Sensor | 1 | Temperature & humidity source |
| 120 Ω Resistor | 2 | CAN bus termination |
| Jumper Wires + Breadboard | — | Prototyping |

---

## 🗂️ System Architecture
```
[Node A — Transmitter]                [Node B — Receiver]
┌──────────────────────┐              ┌──────────────────────┐
│   Arduino UNO        │   CAN_H ──── │   Arduino UNO        │
│   + MCP2515          │   CAN_L ──── │   + MCP2515          │
│   + DHT22 Sensor     │              │   → Serial Monitor   │
└──────────────────────┘              └──────────────────────┘
       (SPI)                                   (SPI)
  120Ω ─┤                                         ├─ 120Ω
```

- **Interface:** SPI at 10 MHz
- **Bus Speed:** 500 kbps
- **Frame Type:** Standard CAN 2.0A (11-bit ID)
- **Oscillator:** 8 MHz crystal on MCP2515

---

## 📨 CAN Message Layout

| Signal | CAN ID | DLC | Encoding |
|---|---|---|---|
| Temperature | `0x100` | 4 bytes | IEEE 754 float |
| Humidity | `0x101` | 4 bytes | IEEE 754 float |

Lower CAN ID = higher bus priority. Temperature wins arbitration over humidity.

---

## 🔌 Pin Connections (Arduino UNO ↔ MCP2515)

| Signal | Arduino Pin | MCP2515 Pin |
|---|---|---|
| CS (Chip Select) | D10 | CS |
| SCK | D13 | SCK |
| MOSI | D11 | SI |
| MISO | D12 | SO |
| INT | D2 | INT |
| VCC | 5V | VCC |
| GND | GND | GND |

---

## 📁 Repository Structure
```
can-bus-simulation/
├── node_a_transmitter/
│   └── node_a_transmitter.ino    # TX — reads DHT22, sends CAN frames
├── node_b_receiver/
│   └── node_b_receiver.ino       # RX — receives and decodes CAN frames
├── docs/
│   └── CAN_Bus_Documentation.docx
├── circuit/
│   └── wiring_diagram.png
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

Install these libraries via Arduino Library Manager:

- [`mcp_can`](https://github.com/coryjfowler/MCP_CAN_lib) — Cory J. Fowler
- `DHT sensor library` — Adafruit
- `SPI` — Arduino built-in

### Flash & Run

1. Open `node_a_transmitter.ino` → upload to Arduino A
2. Open `node_b_receiver.ino` → upload to Arduino B
3. Wire both boards as per the pin table above
4. Add 120 Ω resistors between CAN_H/GND and CAN_L/GND at each end
5. Open Serial Monitor on both boards at **115200 baud**

---

## 📊 Sample Output

**Node A (TX):**
```
CAN BUS Initialized — Node A (Transmitter)
TX Temp: 24.50
TX Humidity: 58.30
```

**Node B (RX):**
```
CAN BUS Initialized — Node B (Receiver)
RX Temperature : 24.50 C
RX Humidity    : 58.30 %
```

---

## 🧠 Concepts Demonstrated

- **CAN frame structure** — SOF, CAN ID, DLC, data, CRC, ACK
- **Non-destructive bitwise arbitration** — lower ID wins bus access
- **SPI communication** — Arduino ↔ MCP2515 register control
- **Interrupt-driven reception** — INT pin polling on Node B
- **Differential signaling** — CAN_H / CAN_L via TJA1050 transceiver
- **Multi-node bus topology** — scalable to N nodes

---

## 🔮 Future Work

- [ ] Add a third node to demonstrate multi-node arbitration
- [ ] Implement CAN FD for higher data-rate payloads
- [ ] Build a `.dbc` (CAN database) file for signal decoding
- [ ] Decode frames with a USB-CAN adapter (PEAK / Kvaser)
- [ ] Add OLED display on receiver node

---

## 📚 References

- [Microchip MCP2515 Datasheet — DS20001801H](https://www.microchip.com/en-us/product/MCP2515)
- [ISO 11898-1:2015 — CAN Protocol Specification](https://www.iso.org/standard/63648.html)
- [mcp_can Library — coryjfowler](https://github.com/coryjfowler/MCP_CAN_lib)
- [CAN Bus Explained — CSS Electronics](https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial)

---

## 🏷️ Tags

`embedded-systems` `can-bus` `arduino` `mcp2515` `automotive` `ecu` `spi` `c` `iot` `real-time`
