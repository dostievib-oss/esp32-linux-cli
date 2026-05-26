# ESP32 Linux CLI

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-ESP32--WROOM--32-blue)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![Linux](https://img.shields.io/badge/Linux-Arch%20%7C%20Ubuntu%20%7C%20Debian-orange)
![License](https://img.shields.io/badge/License-MIT-green)

**Control your ESP32 from Linux terminal like a pro!**  
*Pure C++ • No Arduino IDE • No delay()*
[![Stars](https://img.shields.io/github/stars/dostievib-oss/esp32-linux-cli?style=social)](https://github.com/dostievib-oss/esp32-linux-cli)

</div>

---

## 🎯 What is this?
A professional bridge between your Linux terminal and ESP32 hardware. Write commands like `pin 16 on` in bash and watch your LED light up in real-time.

Built with **ESP-IDF** and pure **C++17** — no Arduino legacy code, no blocking delays.

---

## ✨ Features
| Feature | Status | Description |
|---------|--------|-------------|
| 🎨 ASCII Art Logo | ✅ | Cyberpunk-style banner on startup |
| 🔌 GPIO Control | ✅ | `pin <num> on/off` commands |
| 📊 Read Pins | ✅ | `read <num>` to get state |
| 📟 Serial Monitor | ✅ | Built-in `idf.py monitor` |
| 🐧 Linux CLI Tool | ✅ | Interactive terminal interface |
| 💾 Free Heap | ✅ | `status` command shows memory |

---

## 🚀 Quick Start (30 seconds)

```bash
# Clone the repository
git clone https://github.com/dostievib-oss/esp32-linux-cli
cd esp32-linux-cli

# Flash your ESP32
idf.py flash

# Open the CLI tool
cd host && ./esp32-cli
