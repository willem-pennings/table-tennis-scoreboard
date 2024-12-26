#ifndef segment_h
#define segment_h

#include <FastLED.h>

class SevenSegmentDisplay {
    private:
        // Reference to the FastLED LED array.
        CRGB* led_array;
        // Starting index for this 7-segment display in the LED array.
        uint8_t start_index;
        // Number of LEDs per segment.
        uint8_t leds_per_segment;
        // Array to hold the LED start indices for each segment.
        uint8_t segments[7];

        // Mapping of segments for digits 0-9.
        const bool digit_map[10][7] = {
            // A, B, C, D, E, F, G
            {1, 1, 1, 1, 1, 1, 0}, // 0
            {0, 1, 1, 0, 0, 0, 0}, // 1
            {1, 1, 0, 1, 1, 0, 1}, // 2
            {1, 1, 1, 1, 0, 0, 1}, // 3
            {0, 1, 1, 0, 0, 1, 1}, // 4
            {1, 0, 1, 1, 0, 1, 1}, // 5
            {1, 0, 1, 1, 1, 1, 1}, // 6
            {1, 1, 1, 0, 0, 0, 0}, // 7
            {1, 1, 1, 1, 1, 1, 1}, // 8
            {1, 1, 1, 1, 0, 1, 1}  // 9
        };

    public:
        // Constructor to initialize the display.
        SevenSegmentDisplay(CRGB* led_array, uint8_t start_index, uint8_t leds_per_segment);

        // Sets a digit (0-9) and update the LEDs.
        void SetDigit(uint8_t digit, CRGB color);

        // Manually sets a specific segment to a specific color (or black (off)).
        void SetSegment(uint8_t segment, CRGB Color);

        // Clears the entire digit by setting the display color to black.
        void Clear();
};

#endif