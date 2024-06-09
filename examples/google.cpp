#include "../include/libg203lightsync.h"
#include <thread>

using namespace std::chrono_literals;

int main() {
    g203_lightsync mouse;

    const std::array<const uint8_t, 3> left = {0x33, 0x69, 0xe8};
    const std::array<const uint8_t, 3> center = {0xee, 0xb2, 0x11};
    const std::array<const uint8_t, 3> right = {0xd5, 0x0f, 0x25};

    for (uint8_t i = 0; i < 0xFF; i++) {
        mouse.set_triple({left, center, right});
        std::this_thread::sleep_for(100ms);
    }
    return 0;
}