/**
  Table tennis score board controller
  Copyright (c) by Willem Pennings, 2024
  For more information, visit https://willempennings.nl
**/

#include <FastLED.h>

// Controller library.
#include "src/scoreboard-controller.h"

// Create LED array.
CRGB leds[NUM_LEDS];

// Create the 7-segment displays to display the number of points and games for each player.
SevenSegmentDisplay player_1_tens(leds, START_P1_TENS, LEDS_PER_SEGMENT_SCORE);
SevenSegmentDisplay player_1_ones(leds, START_P1_ONES, LEDS_PER_SEGMENT_SCORE);
SevenSegmentDisplay player_1_games(leds, START_P1_GAMES, LEDS_PER_SEGMENT_GAME);
SevenSegmentDisplay player_2_tens(leds, START_P2_TENS, LEDS_PER_SEGMENT_SCORE);
SevenSegmentDisplay player_2_ones(leds, START_P2_ONES, LEDS_PER_SEGMENT_SCORE);
SevenSegmentDisplay player_2_games(leds, START_P2_GAMES, LEDS_PER_SEGMENT_GAME);

// Create the timeout indicators for each player.
TimeoutIndicator player_1_timeout(leds, START_P1_TIMEOUT);
TimeoutIndicator player_2_timeout(leds, START_P2_TIMEOUT);

// Create the score and game counters to keep track of the number of points and games for each player.
ScoreCounter score_counter_1(&player_1_tens, &player_1_ones, ScoreCounter::LEFT);
ScoreCounter score_counter_2(&player_2_tens, &player_2_ones, ScoreCounter::RIGHT);
GameCounter game_counter_1(&player_1_games);
GameCounter game_counter_2(&player_2_games);

// Create the umpire to manage the game.
Umpire umpire(&score_counter_1, &score_counter_2, &game_counter_1, &game_counter_2,
  &player_1_timeout, &player_2_timeout);

// Create the web server which the user uses to interact with the umpire.
ScoreboardServer server_manager(WIFI_SSID, WIFI_PASSWORD, &umpire);

void setup() {
  // Wait a little.
  delay(1000);

  // Initialize the LED strip.
  FastLED.addLeds<WS2812, PIN_DATA, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(0);

  // Start web server.
  server_manager.Begin();

  // Reset the match to begin.
  umpire.ResetMatch();
  FastLED.show();

  // Gradually ramp up the brightness to the set value.
  for (int i = 0; i < BRIGHTNESS; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(10);
  }
}

void loop() {
  // Web server requests are handled in the background.
}
