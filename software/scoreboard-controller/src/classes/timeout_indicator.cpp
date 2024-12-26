#include "timeout_indicator.h"

// Constructor to initialize the timeout indicator.
TimeoutIndicator::TimeoutIndicator(CRGB* led_array, int start_index, int number_of_leds) : led_array(led_array), 
        start_index(start_index), number_of_leds(number_of_leds), timeout_requested(false), color(DEFAULT_COLOR) {
    // Initialize the indicator with all LEDs off.
    UpdateDisplay();
}

// Toggles the timeout status.
void TimeoutIndicator::ToggleTimeout() {
    // Toggle the flag and update the corresponding LEDs.
    timeout_requested = !timeout_requested;
    UpdateDisplay();
}

// Sets a specific timeout status.
void TimeoutIndicator::SetStatus(bool status) {
    timeout_requested = status;
}

// Checks whether a timeout is requested.
bool TimeoutIndicator::GetStatus() {
    return timeout_requested;
}

// Resets the timeout for a new game.
void TimeoutIndicator::Reset() {
    // Reset the flag and turn off the LEDs.
    timeout_requested = false;
    UpdateDisplay();
}

// Sets the color of the timeout indicator display.
void TimeoutIndicator::SetColor(CRGB new_color) {
    color = new_color;
    UpdateDisplay();
}

// Gets the current display color.
CRGB TimeoutIndicator::GetColor() {
    return color;
}

// Updates the display: turn on or off all LEDs.
void TimeoutIndicator::UpdateDisplay() {
    // Timeout indicator color.
    if (timeout_requested) {
        for (uint8_t i = 0; i < number_of_leds; i++) {
            // Turn on the indicator in the active colour.
            led_array[start_index + i] = color;
        }
    } else {
        for (uint8_t i = 0; i < number_of_leds; i++) {
            // Turn off the indicator.
            led_array[start_index + i] = CRGB::Black;
        }
    }
}
