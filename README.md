# ESP32-Timer-Configuration-with-Registers
This project demonstrates how to configure and use a hardware timer on the ESP32 directly via its timer registers, without relying on the higher-level ESP-IDF timer APIs. It toggles an LED connected to GPIO 14 at regular intervals.

## Key features include:

Direct Register Access: The project manipulates the ESP32’s Timer Group 0 registers (TIMG0_T0CONFIG, TIMG0_T0ALARMLO, etc.) to configure the timer’s behavior, alarm value, and auto-reload.

- Timer Setup: The timer is configured to count up, enable auto-reload, and generate an alarm after a specific interval.

- GPIO Control: GPIO 14 is initialized as an output to drive an LED. Each time the timer alarm triggers, the LED state is toggled.

- Polling-Based Timer Handling: The program continuously checks the timer alarm flag and toggles the LED when the alarm occurs.

- Watchdog Handling: The ESP32 task watchdog is disabled to prevent resets during the infinite loop.

This low-level approach gives insight into the inner workings of ESP32 timers and how to directly manipulate hardware registers for precise timing control.
