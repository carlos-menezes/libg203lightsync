#ifndef LIBLIGHTSYNCMOUSE_LIBRARY_H
#define LIBLIGHTSYNCMOUSE_LIBRARY_H

#include <array>

#include "libusb.h"

#define G203_LIGHTSYNC_VENDOR_ID 0x046D
#define G203_LIGHTSYNC_PRODUCT_ID 0xC09D

enum direction { left = 0x01, right = 0x06 };

class g203_lightsync {
    libusb_device_handle *device_handle;
    int libusb_call_status;
    unsigned int libusb_timeout;

    void send_command(const std::array<const uint8_t, 20> &data, bool disable_onboard_memory = true);
    static uint16_t validate_rate(const uint16_t &rate);
    static uint8_t validate_brightness(const uint8_t &brightness);

public:
    explicit g203_lightsync(unsigned int libusb_timeout = 2000);

    void set_solid_color(const std::array<const uint8_t, 3> &color);
    void set_breathe(const std::array<const uint8_t, 3> &color, uint16_t rate, uint8_t brightness);
    void set_cycle(uint16_t rate, uint8_t brightness);
    void set_triple(const std::array<const std::array<const uint8_t, 3>, 3> &colors);
    void set_wave(uint16_t rate, uint8_t brightness, const direction &direction);
    void set_blend(uint16_t rate, uint8_t brightness);

    ~g203_lightsync();
};

#endif // LIBLIGHTSYNCMOUSE_LIBRARY_H
