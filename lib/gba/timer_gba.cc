#include "timer_gba.h"

void Timer::tick(uint cycles) {
    u8 new_divider = static_cast<u8>(divider.value() + cycles);
    divider.set(new_divider);
}

u8 Timer::get_divider() const {
    return divider.value();
}

u8 Timer::get_timer() const {
    return timer_counter.value();
}

u8 Timer::get_timer_modulo() const {
    return timer_modulo.value();
}

u8 Timer::get_timer_control() const {
    return timer_control.value();
}

void Timer::reset_divider() {
    divider.set(0x0);
}

void Timer::set_timer_modulo(u8 value) {
}

void Timer::set_timer_control(u8 value) {
}
