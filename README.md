# ESP32 Linux CLI — Complete Installation Guide

## What is this project

This is a program that allows you to control ESP32 from the Linux terminal. Type commands like `pin 16 on` and the LED on your ESP32 lights up. Everything works in pure C++ without Arduino IDE.

---

## What you will need

- Computer with Linux (Arch, Ubuntu, Debian, Fedora)
- ESP32 board (any model, e.g. ESP32-WROOM-32)
- USB cable (must support data transfer, not just charging)
- Internet connection (to download tools, about 500 MB)

---

## Part 1. Install required software

### For Arch Linux

Open terminal (Ctrl+Alt+T) and run:

`sudo pacman -S --needed git cmake ninja gcc python-pip`

### For Ubuntu / Debian

`sudo apt update`

`sudo apt install git cmake ninja-build gcc python3-pip`

### For Fedora

`sudo dnf install git cmake ninja-build gcc python3-pip`

---

## Part 2. Install ESP-IDF (framework for ESP32)

### Step 1. Download ESP-IDF

`cd ~`

`git clone --recursive https://github.com/espressif/esp-idf.git`

This downloads the esp-idf folder to your home directory. Takes 2-3 minutes.

### Step 2. Run the installer

`cd esp-idf`

`./install.sh`

This installs compilers and debuggers. Takes 5-10 minutes. Wait for `Installation complete`.

### Step 3. Set up the environment

`echo "alias get_idf='. ~/esp-idf/export.sh'" >> ~/.bashrc`

`source ~/.bashrc`

This creates the `get_idf` command. Every time you open a terminal, just type `get_idf` to activate the ESP-IDF environment.

---

## Part 3. Download the project

### Step 1. Clone the repository

`cd ~`

`git clone https://github.com/dostievib-oss/esp32-linux-cli`

### Step 2. Enter the project folder

`cd esp32-linux-cli`

---

## Part 4. Build the firmware

### Step 1. Activate ESP-IDF environment

`get_idf`

### Step 2. Set the chip target

`idf.py set-target esp32`

This tells the compiler we are using ESP32.

### Step 3. Build the project

`idf.py build`

First build takes 2-3 minutes. You should see `Project build complete` at the end.

---

## Part 5. Connect ESP32 to your computer

### Step 1. Connect ESP32 via USB cable

### Step 2. Find which port appeared

`ls /dev/ttyUSB* /dev/ttyACM*`

You should see something like `/dev/ttyUSB0` or `/dev/ttyACM0`. Remember this name.

### Step 3. Give permission to access the port

`sudo chmod 666 /dev/ttyUSB0`

Replace `ttyUSB0` with your port. This allows your user to read and write to the port.

---

## Part 6. Flash the ESP32

### Step 1. Run the flash command

`idf.py -p /dev/ttyUSB0 flash`

Replace `ttyUSB0` with your port.

### Step 2. If flashing gets stuck (Connecting error)

ESP32 often does not enter flashing mode automatically. Do this:

1. Press and hold the **BOOT** button on the ESP32 board
2. Press and release the **EN** button (while still holding BOOT)
3. Release the **BOOT** button
4. Run the flash command again

### Step 3. Wait for completion

You will see `Hash of data verified`. This means the firmware was successfully uploaded.

---

## Part 7. Verify the firmware works

### Start the serial monitor

`idf.py -p /dev/ttyUSB0 monitor`

You should see the ASCII logo and the message `Ready to accept commands`.

**To exit the monitor,** press `Ctrl + ]` (Control and closing square bracket).

---

## Part 8. Build and run the Linux CLI tool

### Step 1. Go to the host folder

`cd ~/esp32-linux-cli/host`

### Step 2. Compile the tool

`g++ -o esp32-cli esp32-cli.cpp`

### Step 3. Run the tool

`./esp32-cli`

The tool automatically finds your ESP32 and shows a prompt for entering commands.

---

## Part 9. Commands to control ESP32

Inside the CLI tool (after running `./esp32-cli`), you will see the prompt `esp32$`. Enter the commands below.

### Command table

| Command | What it does | Example |
|---------|--------------|---------|
| `help` | Shows all commands | `help` |
| `pin <num> on` | Turns a pin ON (HIGH) | `pin 16 on` |
| `pin <num> off` | Turns a pin OFF (LOW) | `pin 16 off` |
| `pins <nums>=on` | Turns multiple pins ON at once | `pins 15,16,17=on` |
| `pins <nums>=off` | Turns multiple pins OFF at once | `pins 15,16,17=off` |
| `pwm <num> <value>` | Sets PWM duty cycle (0-1023) | `pwm 16 512` |
| `read <num>` | Shows pin state (HIGH or LOW) | `read 16` |
| `status` | Shows system info (free memory, version) | `status` |
| `exit` or `quit` | Exits the program | `exit` |

### Examples

After running `./esp32-cli`, you will see:

`┌─[esp32@linux]─[/dev/ttyUSB0]`
`└─$`

Now type commands:

- `pin 16 on` — LED on pin 16 turns on (if connected)
- `pin 16 off` — LED turns off
- `pins 15,16,17=on` — three LEDs turn on at the same time
- `pwm 16 512` — LED lights at half brightness
- `read 16` — shows `HIGH` or `LOW`
- `status` — shows free memory
- `exit` — exits the program

---

## Part 10. Troubleshooting

### Error: Permission denied when accessing port

**Solution:** `sudo chmod 666 /dev/ttyUSB0` (replace with your port)

### Error: get_idf: command not found

**Solution:** Run `source ~/.bashrc` or restart your terminal, then run `get_idf` again

### Error: idf.py: command not found

**Solution:** First run `get_idf`, then `idf.py build`

### Error: Unable to verify flash chip connection

**Solution:** Press BOOT, then EN, release BOOT, then run the flash command again

### Error: Cannot open port

**Solution:** Make sure ESP32 is connected. Check the port with `ls /dev/ttyUSB* /dev/ttyACM*`. If no port appears, try a different USB cable (some cables are charge-only).

### CLI tool cannot find ESP32

**Solution:** Run the tool with `sudo ./esp32-cli`, or set permanent port permissions with `sudo usermod -a -G uucp $USER` and reboot.

---

## Done!

After completing all steps, you can control your ESP32 from the Linux terminal. All commands work instantly with no delays, in pure C++.

**If you have any questions, open an Issue on GitHub.**