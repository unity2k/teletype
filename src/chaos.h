#ifndef CHAOS_H
#define CHOAS_H
#include <stdint.h>

typedef struct {
    float value;
    float r;
} chaos_state_t;

void chaos_set_val(int16_t);
int16_t chaos_get_val(void);
void chaos_set_r(int16_t);
int16_t chaos_get_r(void);

#endif
