#include <array>
#include <thread>
#include <conio.h>

#include "../include/libg203lightsync.h"

int main() {
    g203_lightsync g203;

    constexpr std::array<const uint8_t, 3> red = {0xFF, 0x00, 0x00};
    constexpr std::array<const uint8_t, 3> blue = {0x00, 0x00, 0xFF};

    std::atomic running(true);

    std::thread lighting_thread([&] {
        while (running) {
            g203.set_solid_color(red);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            g203.set_solid_color(blue);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Thread to monitor keyboard input
    std::thread input_thread([&] {
        while (running) {
            if (kbhit()) {
                if (const char ch = static_cast<char>(getch()); ch == 's' || ch == 'S') {
                    running = false;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    lighting_thread.join();
    input_thread.join();


    return 0;
}
