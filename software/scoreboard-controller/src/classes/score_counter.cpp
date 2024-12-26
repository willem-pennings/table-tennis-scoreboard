#include "score_counter.h"

// Constructor to initialize the score counter with two 7-segment displays.
ScoreCounter::ScoreCounter(SevenSegmentDisplay* display_tens, SevenSegmentDisplay* display_ones, Side side) 
        : display_tens(display_tens), display_ones(display_ones), side(side) {
    // Initialize the display with the starting score.
    UpdateDisplay();
}

// Increments the score.
void ScoreCounter::Increment() {
    if (score < 99) {
        // Ensure the score doesn't go above 99.
        score++;
        UpdateDisplay();
    }
}

// Sets the score to a specific value.
void ScoreCounter::SetScore(int value) {
    if (value >= 0 && value <= 99) {
        score = value;
        UpdateDisplay();
    }
}

// Updates the displays with the current score and color.
void ScoreCounter::UpdateDisplay() {
    // Separate the digits.
    int tens = score / 10;
    int ones = score % 10;

    if (tens == 0) {
        // Do not display a leading zero. Also, we want to display single-digit numbers on the outsides of the screen,
        // so on the left, we display them in the "tens" place while on the right we display them in the "ones" place.
        if (side == Side::LEFT) {
            // On the left side, display the "ones" digit in the tens digit and disable the ones digit.
            display_tens->SetDigit(ones, color);
            display_ones->Clear();
        } else if (side == Side::RIGHT) {
            // On the right side, display the "ones" digit in its original place and disable the tens digit.
            display_tens->Clear();
            display_ones->SetDigit(ones, color);
        } else {
            // Invalid side, do nothing.
        }
    } else {
        // Number to display requires two digits.
        display_tens->SetDigit(tens, color);
        display_ones->SetDigit(ones, color);
    }
}

// Turns on a specific segment in either digit of the score counter using the currently active color.
void ScoreCounter::SetSegment(uint8_t digit, uint8_t segment) {
    if (digit == 0) {
        display_tens->SetSegment(segment, color);
    } else if (digit == 1) {
        display_ones->SetSegment(segment, color);
    } else {
        // Invalid digit.
    }
}

// Turns on a specific segment in the game counter using a manually specified color.
void ScoreCounter::SetSegment(uint8_t digit, uint8_t segment, CRGB manual_color) {
    if (digit == 0) {
        display_tens->SetSegment(segment, manual_color);
    } else if (digit == 1) {
        display_ones->SetSegment(segment, manual_color);
    } else {
        // Invalid digit.
    }
}
