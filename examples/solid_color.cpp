#include <array>
#include "../include/libg203lightsync.h"

int main() {
    g203_lightsync g203;

    constexpr std::array<const uint8_t, 3> red = {0xFF, 0x00, 0x00};
    constexpr std::array<const uint8_t, 3> green = {0x00, 0xFF, 0x00};
    constexpr std::array<const uint8_t, 3> blue = {0x00, 0x00, 0xFF};
    constexpr std::array<const std::array<const uint8_t, 3>, 3> colors = {red, green, blue};

    g203.set_triple(colors);

    return 0;
}
