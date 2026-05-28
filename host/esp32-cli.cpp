#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <vector>

using namespace std;

const string LOGO = 
"\n"
"  _____ ___________   _____           _ \n"
" |  ___/  ___| ___ \\ |_   _|         | |\n"
" | |__ \\ `--.| |_/ /   | | ___   ___ | |\n"
" |  __| `--. \\  __/    | |/ _ \\ / _ \\| |\n"
" | |___/\\__/ / |       | | (_) | (_) | |\n"
" \\____/\\____/\\_|       \\_/\\___/ \\___/|_|\n"
"\n"
"  ========== LINUX BRIDGE v1.1 ==========\n";

string find_esp32() {
    for (int i = 0; i < 10; i++) {
        string port = "/dev/ttyUSB" + to_string(i);
        if (access(port.c_str(), F_OK) == 0) return port;
        port = "/dev/ttyACM" + to_string(i);
        if (access(port.c_str(), F_OK) == 0) return port;
    }
    return "";
}

string send_command(const string& port, const string& cmd) {
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd == -1) return "ERROR: Cannot open port " + port;
    
    struct termios tty;
    tcgetattr(fd, &tty);
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(INLCR | ICRNL | IGNCR);
    tty.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &tty);
    
    string command = cmd + "\n";
    write(fd, command.c_str(), command.length());
    usleep(200000);
    
    string response;
    char buf[256];
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        response = buf;
    }
    close(fd);
    return response.empty() ? "OK (no response)" : response;
}

void print_help() {
    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║                 ESP32 LINUX CLI COMMANDS                  ║\n";
    cout << "╠════════════════════════════════════════════════════════════╣\n";
    cout << "║  pin <num> on/off      ── Set single GPIO pin            ║\n";
    cout << "║  pins <n>,<n>=on/off   ── Set multiple GPIO pins         ║\n";
    cout << "║  pwm <num> <0-1023>    ── Set PWM duty cycle             ║\n";
    cout << "║  read <num>            ── Read GPIO state                ║\n";
    cout << "║  status                ─── Show ESP32 info               ║\n";
    cout << "║  help                  ─── Show this help                ║\n";
    cout << "║  exit / quit           ─── Exit program                  ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
}

int main() {
    cout << LOGO << endl;
    
    string port = find_esp32();
    if (port.empty()) {
        cout << "❌ ESP32 NOT FOUND!\n";
        cout << "Please connect ESP32 via USB and try again.\n";
        return 1;
    }
    
    cout << "✅ ESP32 found on " << port << endl;
    cout << "💡 Type 'help' to see available commands\n";
    
    string input;
    while (true) {
        cout << "\n┌─[esp32@linux]─[" << port << "]\n";
        cout << "└─$ ";
        getline(cin, input);
        
        if (input == "exit" || input == "quit") {
            cout << "👋 Goodbye!\n";
            break;
        } else if (input == "help") {
            print_help();
        } else if (input.empty()) {
            continue;
        } else {
            string response = send_command(port, input);
            cout << response;
        }
    }
    return 0;
}