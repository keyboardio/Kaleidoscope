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
#define MACRODOWN(...) (key_toggled_on(keyState) ? MACRO(__VA_ARGS__) : MACRO_NONE)

#define I(n)  MACRO_ACTION_STEP_INTERVAL, n
#define W(n)  MACRO_ACTION_STEP_WAIT, n
#define Dr(k) MACRO_ACTION_STEP_KEYDOWN, (k).flags, (k).keyCode
#define D(k)  Dr(Key_ ## k)
#define Ur(k) MACRO_ACTION_STEP_KEYUP, (k).flags, (k).keyCode
#define U(k)  Ur(Key_ ## k)
#define Tr(k) Dr(k), Ur(k)
#define T(k)  D(k), U(k)
#define END   MACRO_ACTION_END
