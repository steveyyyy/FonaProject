# 📞 FonaProject – Retro Rotary Phone Meets 3G
> *"The past called. So we gave it 3G."*

Bring a classic rotary dial phone into the 21st century! **FonaProject** is a PlatformIO-powered embedded project that transforms a vintage telephone into a functional mobile phone using an Adafruit Fona 3G module and an STM32 Nucleo L432KC board.

## 🚀 Features

- ☎️ **Rotary dialing support** – because buttons are too mainstream.
- 🔐 **SIM unlock via PIN** – enter your SIM PIN through the rotary dial.
- 🇨🇭 **Automatic dialing for Swiss numbers** – auto-connect to preconfigured numbers.
- 🔁 **Modular hardware abstraction** – easily swap or extend components like the ringer, display, and dial.
- ⚙️ **Event-driven design** – using a custom reactive framework.
- 🧪 **Zephyr RTOS integration** – for timers, messaging, and async handling.

---

## 🔧 Hardware Used

- STM32 Nucleo L432KC
- Adafruit Fona 3G Cellular Module
- Vintage rotary telephone
- GPIO peripherals: ringer, buttons, rotary pulse detector, LEDs

---

## 🛠 Getting Started

1. **Install [PlatformIO](https://platformio.org/)** (VSCode recommended)
2. Clone this repository
3. Flash to your STM32 board via PlatformIO
4. Connect hardware according to the pinout (see below)
5. Power up the system and dial like it’s 1965 🚀

---

## 🔌 Wiring Overview

| Component     | Pin/Port       | Notes                       |
|---------------|----------------|-----------------------------|
| Rotary Dial   | D11 / GPIOB.5  | Wind-up detection (Green)   |
| Rotary Dial   | D12 / GPIOB.4  | Pulse detection (Brown)     |
| Switchhook    | D2  / GPIOA.11 | Detects handset lifted      |
| Ringer        | D7  / GPIOA.7  | Controls analog ringer      |
| Fona Module   | UART_1         | Handles AT command exchange |

(Full hardware diagrams and planning notes can be found in the `informations/` folder.)

---

## ⚙️ How It Works

FonaProject is built around an **event-driven architecture**. The main loop (`main.cpp`) runs a minimal state machine that:

- Initializes hardware via a central `Factory`
- Reacts to user input via events (e.g. rotary pulses, switchhook)
- Sends AT commands to the Fona 3G module
- Handles UART responses via an observer pattern

Each hardware component (e.g. dial, ringer, uart) is encapsulated in its own module under `src/hw/`.

---

## 🧠 Dev Notes

- The rotary dial logic uses **timing-based pulse detection** to decode numbers.
- Fona integration is done over UART, with **non-blocking message queues**.
- The code runs on **Zephyr RTOS**, which powers its timers and async behavior.
- Diagrams and planning notes are in `informations/` for those curious about the logic.

---

## ☑️ Roadmap Ideas

- 📱 Add very basic SMS sending support
- 🔋 Add battery voltage monitoring
- 📷 Upload a photo of the actual build

---

## 📜 License

This project is licensed under the **GNU General Public License v3.0**.
You are free to **use**, **modify**, and **distribute** this code — even commercially — **under the following conditions**:

- You must provide access to the **source code** when distributing the software.
- You must **include a copy of this license**.
- You may **not impose additional restrictions** on the rights granted by this license.

For full details, see the [**GNU GPL v3.0 License**](./LICENSE).

---

Happy hacking, and long live rotary phones! ☎️👨‍🔧✨