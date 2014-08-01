//#define DEBUG_SERIAL false

#define EEPROM_KEYMAP_LOCATION 0

#define MODEL00piersjesse true

#ifdef MODEL00
#define COLS 14
#define ROWS 5
static const byte colPins[COLS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A0 };
static const byte rowPins[ROWS] = { A5, A4, A3, A2, A1 };

#endif

#ifdef MODEL00bis
#define COLS 14
#define ROWS 5
static const byte colPins[COLS] = { 0,1, 2, 3, 4, 5,6,7, 8, 9, 10,11,12, A0};

static const byte rowPins[ROWS] = { A1,A2,A3,A4,A5};

#endif

#ifdef MODEL00piersjesse
#define COLS 16
#define ROWS 5

static const byte colPins[COLS] = { 

   5,
   4,
   2,
   3,
   1,
   9,
   0,
    MISO,
    SCK,
    MOSI,
   6,
   7,
    8,
    A6,
    A5,
    A4,

};
static const byte rowPins[ROWS] = { A0, A1,A2,A3 };


#endif
#ifdef MODEL00piers
#define COLS 16
#define ROWS 5

static const byte colPins[COLS] = { 

    3,
    A4,
    A5,
    MISO,
    SCK,
    1,
    MOSI,
    2,
    4,
    5,
    7,
    8,
    9,
    10,
    0,
    12,
};
static const byte rowPins[ROWS] = { A0, A1,A2,A3 };


#endif
//#static const byte colPins[COLS] = { 16, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
//#static const byte rowPins[ROWS] = { A2, A3, A4, A5, 15 };


// if we're sticking to boot protocol, these could all be 6 + mods
// but *mumble*

#define KEYS_HELD_BUFFER 12
