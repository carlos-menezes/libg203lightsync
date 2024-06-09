#include "../include/libg203lightsync.h"
#include <thread>

using namespace std::chrono_literals;

int main() {
    g203_lightsync mouse;
    mouse.set_solid_color({0xFF, 0x00, 0x00});
    std::this_thread::sleep_for(1000ms);
    mouse.set_solid_color({0x00, 0xFF, 0x00});
    std::this_thread::sleep_for(1000ms);
    mouse.set_solid_color({0x00, 0x00, 0xFF});
    return 0;
}