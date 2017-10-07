#pragma once

typedef enum {
  MACRO_ACTION_END,

  MACRO_ACTION_STEP_INTERVAL,
  MACRO_ACTION_STEP_WAIT,

  MACRO_ACTION_STEP_KEYDOWN,
  MACRO_ACTION_STEP_KEYUP,
  MACRO_ACTION_STEP_TAP,

  MACRO_ACTION_STEP_KEYCODEDOWN,
  MACRO_ACTION_STEP_KEYCODEUP,
  MACRO_ACTION_STEP_TAPCODE,
} MacroActionStepType;

typedef uint8_t macro_t;

#define MACRO_NONE 0
#define MACRO(...) ({static const macro_t __m[] PROGMEM = { __VA_ARGS__, MACRO_ACTION_END }; &__m[0]; })
#define MACRODOWN(...) (keyToggledOn(keyState) ? MACRO(__VA_ARGS__) : MACRO_NONE)

#define I(n)  MACRO_ACTION_STEP_INTERVAL, n
#define W(n)  MACRO_ACTION_STEP_WAIT, n

#define Dr(k) MACRO_ACTION_STEP_KEYDOWN, (k).flags, (k).keyCode
#define D(k)  Dr(Key_ ## k)
#define Ur(k) MACRO_ACTION_STEP_KEYUP, (k).flags, (k).keyCode
#define U(k)  Ur(Key_ ## k)
#define Tr(k) MACRO_ACTION_STEP_TAP, (k).flags, (k).keyCode
#define T(k)  Tr(Key_ ## k)

#define Dc(k) MACRO_ACTION_STEP_KEYCODEDOWN, (Key_ ## k).keyCode
#define Uc(k) MACRO_ACTION_STEP_KEYCODEUP, (Key_ ## k).keyCode
#define Tc(k) MACRO_ACTION_STEP_TAPCODE, (Key_ ## k).keyCode

__attribute__((deprecated("END is no longer required to end macros"))) const MacroActionStepType END = MACRO_ACTION_END;
