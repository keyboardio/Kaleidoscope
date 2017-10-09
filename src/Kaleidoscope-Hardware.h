#pragma once

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} cRGB;

typedef struct {
  uint8_t g;
  uint8_t r;
  uint8_t b;
} cGRB;

static inline cGRB asCGRB(const cRGB& crgb) {
  return (cGRB){crgb.g, crgb.r, crgb.b};
}

static inline cRGB asCRGB(const cGRB& cgrb) {
  return (cRGB){cgrb.r, cgrb.g, cgrb.b};
}

typedef struct {
  uint8_t r;
  uint8_t b;
  uint8_t g;
} cRBG;

static inline cRBG asCRBG(const cRGB& crgb) {
  return (cRBG){crgb.r, crgb.b, crgb.g};
}

static inline cRGB asCRGB(const cRBG& crbg) {
  return (cRGB){crbg.r, crbg.g, crbg.b};
}

typedef struct {
  uint8_t g;
  uint8_t b;
  uint8_t r;
} cGBR;

static inline cGBR asCGBR(const cRGB& crgb) {
  return (cGBR){crgb.g, crgb.b, crgb.r};
}

static inline cRGB asCRGB(const cGBR& cgbr) {
  return (cRGB){cgbr.r, cgbr.g, cgbr.b};
}

typedef struct {
  uint8_t b;
  uint8_t r;
  uint8_t g;
} cBRG;

static inline cBRG asCBRG(const cRGB& crgb) {
  return (cBRG){crgb.b, crgb.r, crgb.g};
}

static inline cRGB asCRGB(const cBRG& cbrg) {
  return (cRGB){cbrg.r, cbrg.g, cbrg.b};
}

typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} cBGR;

static inline cBGR asCBGR(const cRGB& crgb) {
  return (cBGR){crgb.b, crgb.g, crgb.r};
}

static inline cRGB asCRGB(const cBGR& cbgr) {
  return (cRGB){cbgr.r, cbgr.g, cbgr.b};
}

#define CRGB(r,g,b) (cRGB){r,g,b}
