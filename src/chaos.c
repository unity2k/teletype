#include "chaos.h"

static chaos_state_t chaos_state = { 0.5, 3.75 };

void chaos_set_val(int16_t val) {
    chaos_state.value = (float)val / (10000.0);
}

int16_t chaos_get_val() {
    // Logistic map
    chaos_state.value =
        chaos_state.r * chaos_state.value * (1.0 - chaos_state.value);
    return (int16_t)(chaos_state.value * 10000);
}

void chaos_set_r(int16_t r) {
    chaos_state.r = (float)r / 100.0;
}

int16_t chaos_get_r() {
    return (int16_t)(chaos_state.r * 100);
}
