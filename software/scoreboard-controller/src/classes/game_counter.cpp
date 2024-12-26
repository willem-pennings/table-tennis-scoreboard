#include "game_counter.h"

// Constructor to initialize the game counter with one 7-segment display.
GameCounter::GameCounter(SevenSegmentDisplay* display) : display(display) {
    // Initialize the display with the starting score.
    UpdateDisplay();
}

// Increments the score.
void GameCounter::Increment() {
    if (score < 9) {
        // Ensure the score doesn't go above 9.
        score++;
        UpdateDisplay();
    }
}

// Sets the score to a specific value.
void GameCounter::SetScore(int value) {
    if (value >= 0 && value <= 9) {
        score = value;
        UpdateDisplay();
    }
}

// Updates the displays with the current score and color.
void GameCounter::UpdateDisplay() {
    display->SetDigit(score, color);
}

// Turns on a specific segment in the game counter using the currently active color.
void GameCounter::SetSegment(uint8_t segment) {
    display->SetSegment(segment, color);
}

// Turns on a specific segment in the game counter using a manually specified color.
void GameCounter::SetSegment(uint8_t segment, CRGB manual_color) {
    display->SetSegment(segment, manual_color);
}
