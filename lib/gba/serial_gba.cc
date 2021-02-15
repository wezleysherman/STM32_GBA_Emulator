#include "serial_gba.h"

#include "util/bitwise.h"
#include "util/log.h"

#include <cstdio>

u8 Serial_gba::read() const {
    return data;
}

void Serial_gba::write(const u8 byte) {
    data = byte;
}

void Serial_gba::write_control(const u8 byte) {
    if (bitwise::check_bit(byte, 7) && options.print_serial_gba) {
        printf("%c", data);
        fflush(stdout);
    }
}
