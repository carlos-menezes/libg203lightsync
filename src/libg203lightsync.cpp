#include "libg203lightsync.h"

#undef max
#undef min

#include <format>
#include <stdexcept>
#include <array>

#include "libusb/libusb.h"


g203_lightsync::g203_lightsync(const unsigned int libusb_timeout) {
    this->libusb_timeout = libusb_timeout;
    this->libusb_call_status = 0;

    this->libusb_call_status = libusb_init_context(nullptr, nullptr, 0);
    if (this->libusb_call_status < 0) {
        throw std::runtime_error(std::string("could not call libusb_init_context: ") +
                                 libusb_error_name(this->libusb_call_status));
    }

    this->device_handle = libusb_open_device_with_vid_pid(nullptr, G203_LIGHTSYNC_VENDOR_ID, G203_LIGHTSYNC_PRODUCT_ID);
    if (this->device_handle == nullptr) {
        throw std::runtime_error("could not call libusb_open_device_with_vid_pid");
    }
}

void g203_lightsync::set_solid_color(const std::array<const uint8_t, 3> &color) {
    const std::array<const uint8_t, 20> buffer = {0x11,     0xff,     0x0e, 0x1b, 0x00, 0x01, color[0],
                                                  color[1], color[2], 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00,     0x00,     0x01, 0x00, 0x00, 0x00};
    return this->send_command(buffer, true);
}

void g203_lightsync::set_breathe(const std::array<const uint8_t, 3> &color, const uint16_t rate,
                                 const uint8_t brightness) {
    const auto validated_rate = validate_rate(rate);
    const uint8_t rate_low = validated_rate & 0xFF;
    const uint8_t rate_high = validated_rate >> 0x8;
    const auto validated_brightness = validate_brightness(brightness);
    const std::array<const uint8_t, 20> buffer = {
            0x11,
            0xff,
            0x0e,
            0x1b,
            0x00,
            0x04,
            color[0],
            color[1],
            color[2],
            rate_high,
            rate_low,
            0x00,
            validated_brightness,
            0x00,
            0x00,
            0x00,
            0x01,
            0x00,
            0x00,
            0x00,
    };

    return this->send_command(buffer, true);
}

void g203_lightsync::set_cycle(const uint16_t rate, const uint8_t brightness) {
    const auto validated_rate = validate_rate(rate);
    const uint8_t rate_low = validated_rate & 0xFF;
    const uint8_t rate_high = validated_rate >> 0x8;
    const auto validated_brightness = validate_brightness(brightness);
    const std::array<const uint8_t, 20> buffer = {
            0x11, 0xff, 0x0e, 0x1b, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, rate_high, rate_low, validated_brightness,
            0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    };
    return this->send_command(buffer, true);
}

void g203_lightsync::set_triple(const std::array<const std::array<const uint8_t, 3>, 3> &colors) {
    const std::array<const uint8_t, 20> buffer = {
            0x11,         0xff,         0x12,         0x1b,         0x01,         colors[0][0], colors[0][1],
            colors[0][2], 0x02,         colors[1][0], colors[1][1], colors[1][2], 0x03,         colors[2][0],
            colors[2][1], colors[2][2], 0x00,         0x00,         0x00,         0x00,
    };
    return this->send_command(buffer, false);
}

void g203_lightsync::set_wave(const uint16_t rate, const uint8_t brightness, const direction &direction) {
    const auto validated_rate = validate_rate(rate);
    const uint8_t rate_low = validated_rate & 0xFF;
    const uint8_t rate_high = validated_rate >> 0x8;
    const auto validated_brightness = validate_brightness(brightness);
    const std::array<const uint8_t, 20> buffer = {
            0x11,
            0xff,
            0x0e,
            0x1b,
            0x00,
            0x03,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            rate_high,
            static_cast<uint8_t>(direction),
            validated_brightness,
            rate_low,
            0x01,
            0x00,
            0x00,
            0x00,
    };
    return this->send_command(buffer, true);
}

void g203_lightsync::set_blend(const uint16_t rate, const uint8_t brightness) {
    const auto validated_rate = validate_rate(rate);
    const uint8_t rate_low = validated_rate & 0xFF;
    const uint8_t rate_high = validated_rate >> 0x8;
    const auto validated_brightness = validate_brightness(brightness);
    const std::array<const uint8_t, 20> buffer = {
            0x11, 0xff, 0x0e,      0x1b,     0x00,
            0x06, 0x00, 0x00,      0x00,     0x00,
            0x00, 0x00, rate_high, rate_low, validated_brightness,
            0x00, 0x01, 0x00,      0x00,     0x00,
    };
    return this->send_command(buffer, true);
}

void g203_lightsync::send_command(const std::array<const uint8_t, 20> &data, const boolean disable_onboard_memory) {
    // PROLOGUE
    this->libusb_call_status = libusb_claim_interface(this->device_handle, 0x01);
    if (this->libusb_call_status < 0) {
        throw std::runtime_error(std::string("unable to claim interface") + std::format("{:x}", 0x01));
    }

    unsigned char interrupt_buffer[20] = {};

    if (disable_onboard_memory) {
        const std::array<uint8_t, 7> buffer = {0x10, 0xff, 0x0e, 0x5b, 0x01, 0x03, 0x05};
        this->libusb_call_status = libusb_control_transfer(this->device_handle, 0x21, 0x09, 0x210, 0x01,
                                                           const_cast<uint8_t *>(buffer.data()), buffer.size(), 2000);
        if (this->libusb_call_status < 0) {
            throw std::runtime_error(std::string("unable to call libusb_control_transfer: ") +
                                     libusb_error_name(this->libusb_call_status));
        }

        this->libusb_call_status = libusb_interrupt_transfer(this->device_handle, 0x82, interrupt_buffer,
                                                             sizeof(interrupt_buffer), nullptr, 2000);
        if (this->libusb_call_status < 0) {
            throw std::runtime_error(std::string("unable to call libusb_interrupt_transfer: ") +
                                     libusb_error_name(this->libusb_call_status));
        }
    }

    this->libusb_call_status = libusb_control_transfer(this->device_handle, 0x21, 0x09, 0x0211, 0x0001,
                                                       const_cast<unsigned char *>(data.data()), data.size(), 2000);
    if (this->libusb_call_status < 0) {
        throw std::runtime_error(std::string("unable to call libusb_control_transfer: ") +
                                 libusb_error_name(this->libusb_call_status));
    }


    this->libusb_call_status = libusb_interrupt_transfer(this->device_handle, 0x82, interrupt_buffer,
                                                         sizeof(interrupt_buffer), nullptr, 2000);
    if (this->libusb_call_status < 0) {
        throw std::runtime_error(std::string("unable to call libusb_interrupt_transfer: ") +
                                 libusb_error_name(this->libusb_call_status));
    }

    // Checks if the first 4 bytes match the first 4 bytes for the `triple` command
    if (data[0] + data[1] + data[2] + data[3] == 0x11 + 0xff + 0x12 + 0x1b) {

        std::array<const uint8_t, 20> buffer = {0x11, 0xff, 0x12, 0x7b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        this->libusb_call_status = libusb_control_transfer(this->device_handle, 0x21, 0x09, 0x210, 0x01,
                                                           const_cast<uint8_t *>(buffer.data()), buffer.size(), 2000);
        if (this->libusb_call_status < 0) {
            throw std::runtime_error(std::string("unable to call libusb_control_transfer: ") +
                                     libusb_error_name(this->libusb_call_status));
        }

        this->libusb_call_status = libusb_interrupt_transfer(this->device_handle, 0x82, interrupt_buffer,
                                                             sizeof(interrupt_buffer), nullptr, 2000);
        if (this->libusb_call_status < 0) {
            throw std::runtime_error(std::string("unable to call libusb_interrupt_transfer: ") +
                                     libusb_error_name(this->libusb_call_status));
        }
    }

    // EPILOGUE
    this->libusb_call_status = libusb_release_interface(this->device_handle, 0x01);
    if (this->libusb_call_status < 0) {
        throw std::runtime_error(std::string("unable to release interface") + std::format("{:x}", 0x01));
    }
}

uint16_t g203_lightsync::validate_rate(const uint16_t &rate) {
    //return rate;
    return std::max(static_cast<uint16_t>(1000), std::min(std::numeric_limits<uint16_t>::max(), rate));
}

uint8_t g203_lightsync::validate_brightness(const uint8_t &brightness) {
    //return brightness;
    return std::max(static_cast<uint8_t>(1), std::min(static_cast<uint8_t>(100), brightness));
}

g203_lightsync::~g203_lightsync() {
    libusb_close(this->device_handle);
    libusb_exit(nullptr);
}
