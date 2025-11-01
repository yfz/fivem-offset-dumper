#include "utils.hpp"
#include <windows.h>
#include <iostream>
#include <string>

namespace logger {
    void set_console_color(WORD color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    void log(const std::string& message, int type) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        WORD white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        WORD bright_white = white | FOREGROUND_INTENSITY;
        WORD magenta_bright = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        WORD green_bright = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        WORD red_bright = FOREGROUND_RED | FOREGROUND_INTENSITY;

        const int padding = 5;
        std::cout << std::string(padding, ' ');

        set_console_color(white);
        std::cout << "[";

        switch (type) {
        case info:
            set_console_color(magenta_bright);
            std::cout << ">";
            break;
        case success:
            set_console_color(green_bright);
            std::cout << "+";
            break;
        case fail:
            set_console_color(red_bright);
            std::cout << "!";
            break;
        default:
            set_console_color(bright_white);
            std::cout << "?";
            break;
        }

        set_console_color(white);
        std::cout << "] ";

        set_console_color(bright_white);
        std::cout << message << std::endl;

        set_console_color(white);
    }
}