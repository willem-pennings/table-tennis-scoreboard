#include "segment.h"

// Constructor to initialize the display.
SevenSegmentDisplay::SevenSegmentDisplay(CRGB* led_array, uint8_t start_index, uint8_t leds_per_segment)
        : led_array(led_array), start_index(start_index), leds_per_segment(leds_per_segment) {
    // Initialize the segment indices.
    for (uint8_t i = 0; i < 7; i++) {
        segments[i] = start_index + (i * leds_per_segment);
    }
}

// Sets a digit (0-9) and update the LEDs.
void SevenSegmentDisplay::SetDigit(uint8_t digit, CRGB color) {
    if (digit > 9) {
        // Invalid digit.
        return;
    }

    // Loop over all 7 segments
    for (uint8_t seg = 0; seg < 7; seg++) {
        // If the segment should be on for this digit.
        if (digit_map[digit][seg]) {
            // Set all LEDs in this segment to the given color.
            for (uint8_t led = 0; led < leds_per_segment; led++) {
                led_array[segments[seg] + led] = color;
            }
        } else {
            // Otherwise, turn off the LEDs in this segment.
            for (uint8_t led = 0; led < leds_per_segment; led++) {
                led_array[segments[seg] + led] = CRGB::Black;
            }
        }
    }
}

// Turns on a specific segment manually in a specific color. The segment can be disabled by setting the color to black.
void SevenSegmentDisplay::SetSegment(uint8_t segment, CRGB color) {
    if (segment > 6) {
        // Invalid segment identifier.
        return;
    }

    for (uint8_t led = 0; led < leds_per_segment; led++) {
        led_array[segments[segment] + led] = color;
    }
}

// Clears the entire digit by setting the display color to black.
void SevenSegmentDisplay::Clear() {
    for (uint8_t i = 0; i < 7; i++) {
        SetSegment(i, CRGB::Black);
    }
}
