# ESP32 Linux CLI

Control your ESP32 from Linux terminal like a pro! Pure C++ • No Arduino IDE • No delay()

## Requirements

- ESP32-WROOM-32 (or any ESP32 board)
- Linux (Arch, Ubuntu, Debian, Fedora)
- USB cable with data transfer support
- Internet connection (to download ESP-IDF ~500MB)

## Step-by-Step Installation

### Step 1: Clone the repository

Open terminal (Ctrl+Alt+T) and run:

git clone https://github.com/dostievib-oss/esp32-linux-cli
cd esp32-linux-cli

### Step 2: Install dependencies

For Arch Linux:

sudo pacman -S --needed git cmake ninja gcc python-pip

For Ubuntu/Debian:

sudo apt update
sudo apt install git cmake ninja-build gcc python3-pip

For Fedora:

sudo dnf install git cmake ninja-build gcc python3-pip

### Step 3: Install ESP-IDF

cd ~
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh

Wait 5-10 minutes for installation to complete.

### Step 4: Set up environment

echo "alias get_idf='. ~/esp-idf/export.sh'" >> ~/.bashrc
source ~/.bashrc

### Step 5: Build the project

cd ~/esp32-linux-cli
get_idf
idf.py set-target esp32
idf.py build

### Step 6: Connect ESP32

1. Connect ESP32 via USB
2. Find the port:

ls /dev/ttyUSB* /dev/ttyACM*

3. Give permission:

sudo chmod 666 /dev/ttyUSB0

### Step 7: Flash ESP32

idf.py -p /dev/ttyUSB0 flash

If you see "Unable to verify flash chip connection":
- Hold BOOT button on ESP32
- Press and release EN (while holding BOOT)
- Release BOOT
- Run flash command again

### Step 8: Test the firmware

idf.py -p /dev/ttyUSB0 monitor

You should see the ASCII logo. Exit with Ctrl + ]

### Step 9: Compile CLI tool

cd ~/esp32-linux-cli/host
g++ -o esp32-cli esp32-cli.cpp
./esp32-cli

## Commands

In the CLI tool, try:

help          - Show all commands
pin 16 on     - Turn ON GPIO16
pin 16 off    - Turn OFF GPIO16
read 16       - Read GPIO16 state
status        - Show ESP32 info

## Troubleshooting

Permission denied:

sudo chmod 666 /dev/ttyUSB0

get_idf: command not found:

. ~/esp-idf/export.sh

Port not found:

ls /dev/ttyUSB* /dev/ttyACM*

Try different USB cable or port.

## 📹 Demo

https://github.com/user-attachments/assets/demo.mp4

## Project Structure

esp32-linux-cli/
├── main/
│   ├── main.cpp          # ESP32 firmware
│   └── CMakeLists.txt
├── host/
│   └── esp32-cli.cpp     # Linux CLI tool
├── CMakeLists.txt
└── README.md

## License

MIT License — free for personal and commercial use.

## Star This Project

If this helped you, please star the repository on GitHub!