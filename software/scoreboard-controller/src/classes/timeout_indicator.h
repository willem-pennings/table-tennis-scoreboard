#ifndef timeout_indicator_h
#define timeout_indicator_h

#include <FastLED.h>
#include "../definitions/parameters.h"

class TimeoutIndicator {
    private:
        // Reference to the LED array.
        CRGB* led_array;
        // Current color for the timeout indicator display.
        CRGB color;
        // Start index of the first LED.
        uint8_t start_index;
        // Number of LEDs in the indicator.
        uint8_t number_of_leds;
        // Flag to track if a timeout was requested.
        bool timeout_requested;

    public:
        // Constructor to initialize the indicator with LED array, start index, and number of LEDs.
        TimeoutIndicator(CRGB* led_array, int start_index, int number_of_leds = LEDS_PER_TIMEOUT_INDICATOR);

        // Toggles the timeout status.
        void ToggleTimeout();

        // Sets a specific timeout status.
        void SetStatus(bool status);

        // Gets the current timeout status.
        bool GetStatus();

        // Resets the timeout (for a new match).
        void Reset();
        
        // Changes the color of the timeout indicator display.
        void SetColor(CRGB new_color);

        // Gets the current color of the timeout indicator.
        CRGB GetColor();

        // Turns the indicator on or off.
        void UpdateDisplay();
};

#endif
