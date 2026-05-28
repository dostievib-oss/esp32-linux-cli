# ESP32 Linux CLI

Control ESP32 from Linux terminal — pure C++ without Arduino IDE.

## What is this?

A bridge between your Linux terminal and ESP32 hardware. Write commands like "pin 16 on" in bash and see your LED light up. No Arduino IDE, no delay(), just clean C++ and ESP-IDF.

## Features

- pin <num> on/off — Set single GPIO
- pins <n>,<n>=on/off — Set multiple GPIOs at once
- pwm <num> <0-1023> — Set PWM duty cycle
- read <num> — Read GPIO state
- status — Show ESP32 info
- help — Show all commands

## Quick start

Clone the repository, then run idf.py flash and idf.py monitor.

Then try commands like pin 16 on, pin 16 off, pins 15,4=on, pwm 16 512, read 16, status.

## Requirements

- ESP32-WROOM-32 or any ESP32 board
- Linux (Arch, Ubuntu, Debian, Fedora)
- USB cable with data transfer support

## Project structure

The project has two main parts: firmware for ESP32 in main/main.cpp and Linux CLI tool in host/esp32-cli.cpp.

## Why this project?

Most Arduino tutorials teach bad habits. This project shows professional embedded development with ESP-IDF, pure C++17, non-blocking UART communication, and clean command parsing.

## License

MIT — free for personal and commercial use.

## Star this project

If this helped you, please star the repository on GitHub.