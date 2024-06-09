#include "../include/libg203lightsync.h"

int main() {
    g203_lightsync mouse;

    const std::array<const uint8_t, 3> left = {0x33, 0x69, 0xe8};
    const std::array<const uint8_t, 3> center = {0xee, 0xb2, 0x11};
    const std::array<const uint8_t, 3> right = {0xd5, 0x0f, 0x25};

    mouse.set_triple({left, center, right});
    return 0;
}
