#pragma once

typedef enum {
    MACRO_ACTION_END,

    MACRO_ACTION_STEP_INTERVAL,
    MACRO_ACTION_STEP_WAIT,
    MACRO_ACTION_STEP_KEYDOWN,
    MACRO_ACTION_STEP_KEYUP,
} MacroActionStepType;

typedef uint8_t macro_t;

#define MACRO_NONE 0
#define MACRO(...) ({static const macro_t __m[] PROGMEM = { __VA_ARGS__ }; &__m[0]; })

#define I(n) MACRO_ACTION_STEP_INTERVAL, n
#define W(n) MACRO_ACTION_STEP_WAIT, n
#define D(k) MACRO_ACTION_STEP_KEYDOWN, (Key_ ## k).flags, (Key_ ## k).rawKey
#define U(k) MACRO_ACTION_STEP_KEYUP, (Key_ ## k).flags, (Key_ ## k).rawKey
#define T(k) D(k), U(k)
#define END  MACRO_ACTION_END
