#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <vector>

using namespace std;

// ASCII Art логотип
const string LOGO = 
"\n"
"  _____ ___________   _____           _ \n"
" |  ___/  ___| ___ \\ |_   _|         | |\n"
" | |__ \\ `--.| |_/ /   | | ___   ___ | |\n"
" |  __| `--. \\  __/    | |/ _ \\ / _ \\| |\n"
" | |___/\\__/ / |       | | (_) | (_) | |\n"
" \\____/\\____/\\_|       \\_/\\___/ \\___/|_|\n"
"\n"
"  ========== LINUX BRIDGE ==========\n";

// Поиск ESP32 по USB-портам
string find_esp32() {
    // Проверяем /dev/ttyUSB0-9
    for (int i = 0; i < 10; i++) {
        string port = "/dev/ttyUSB" + to_string(i);
        if (access(port.c_str(), F_OK) == 0) {
            return port;
        }
    }
    // Проверяем /dev/ttyACM0-9
    for (int i = 0; i < 10; i++) {
        string port = "/dev/ttyACM" + to_string(i);
        if (access(port.c_str(), F_OK) == 0) {
            return port;
        }
    }
    return "";
}

// Отправка команды на ESP32 и получение ответа
string send_command(const string& port, const string& cmd) {
    // Открываем порт
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (fd == -1) {
        return "ERROR: Cannot open port " + port + ". Try: sudo chmod 666 " + port;
    }
    
    // Настраиваем порт
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
    
    // Отправляем команду
    string command = cmd + "\n";
    write(fd, command.c_str(), command.length());
    
    // Ждём ответ
    usleep(200000); // 200ms
    
    // Читаем ответ
    string response;
    char buf[256];
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        response = buf;
    }
    
    close(fd);
    
    if (response.empty()) {
        return "OK (no response)";
    }
    return response;
}

void print_help() {
    cout << "\n╔════════════════════════════════════════════╗\n";
    cout << "║           ESP32 CLI COMMANDS               ║\n";
    cout << "╠════════════════════════════════════════════╣\n";
    cout << "║  pin <num> on/off  ── Set GPIO HIGH/LOW   ║\n";
    cout << "║  read <num>        ── Read GPIO state     ║\n";
    cout << "║  status            ── ESP32 info          ║\n";
    cout << "║  help              ── This help           ║\n";
    cout << "║  exit / quit       ── Exit program        ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
}

int main() {
    cout << LOGO << endl;
    
    // Ищем ESP32
    cout << "🔍 Looking for ESP32 on USB ports..." << endl;
    string port = find_esp32();
    
    if (port.empty()) {
        cout << "\n❌ ESP32 NOT FOUND!\n";
        cout << "Please:\n";
        cout << "  1. Connect ESP32 via USB cable\n";
        cout << "  2. Check cable supports data transfer\n";
        cout << "  3. Run: sudo chmod 666 /dev/ttyUSB0\n";
        return 1;
    }
    
    cout << "✅ ESP32 found on " << port << endl;
    cout << "\n💡 Type 'help' to see available commands\n";
    
    // Проверяем соединение
    string test = send_command(port, "help");
    if (test.find("ERROR") != string::npos) {
        cout << "\n⚠️  Connection issue. Try:\n";
        cout << "   sudo chmod 666 " << port << "\n";
    }
    
    // Интерактивный режим
    string input;
    while (true) {
        cout << "\n┌─[esp32@linux]─[" << port << "]\n";
        cout << "└─$ ";
        getline(cin, input);
        
        if (input == "exit" || input == "quit") {
            cout << "\n👋 Goodbye! Exiting ESP32 CLI...\n";
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