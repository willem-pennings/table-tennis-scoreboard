#ifndef parameters_h
#define parameters_h

// Game rules (in case of GAMES_TO_WIN_MATCH, this is only the default value).
const uint8_t WIN_SCORE = 11;
const uint8_t WIN_DIFFERENCE = 2;
const uint8_t GAMES_TO_WIN_MATCH = 3;

// Appearance.
const CRGB DEFAULT_COLOR = CRGB::White;
const CRGB WIN_EFFECT_COLOR = CRGB::Green;

// LED matrix.
const uint8_t NUM_LEDS = 150;
const uint8_t BRIGHTNESS = 128; // Do not go above 128 because the power supply can not handle that.

// LED string layout.
const uint8_t LEDS_PER_SEGMENT_SCORE = 4;
const uint8_t LEDS_PER_SEGMENT_GAME = 2;
const uint8_t LEDS_PER_TIMEOUT_INDICATOR = 5;
const uint8_t START_P1_TENS = 0;
const uint8_t START_P1_ONES = 28;
const uint8_t START_P1_GAMES = 56;
const uint8_t START_P2_TENS = 94;
const uint8_t START_P2_ONES = 122;
const uint8_t START_P2_GAMES = 80;
const uint8_t START_P1_TIMEOUT = 70;
const uint8_t START_P2_TIMEOUT = 75;

#endif
