#pragma once

#include "definitions.h"
#include "options.h"

class Serial_gba {
public:
    Serial_gba(Options& inOptions) : options(inOptions) {}

    u8 read() const;
    void write(u8 byte);
    void write_control(u8 byte);

private:
    Options& options;

    u8 data;
};
