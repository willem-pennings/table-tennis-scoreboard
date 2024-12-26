#include "counter_base.h"

// Protected constructor initializes score to 0 and color to the default color.
CounterBase::CounterBase() : score(0), color(DEFAULT_COLOR) {}

// Increments the score.
void CounterBase::Increment() {
    score++;
    UpdateDisplay();
}

// Decrements the score.
void CounterBase::Decrement() {
    if (score > 0) {
        score--;
        UpdateDisplay();
    }
}

// Sets the score to a specific value.
void CounterBase::SetScore(int value) {
    if (value >= 0) {
        score = value;
        UpdateDisplay();
    }
}

// Resets the score to zero.
void CounterBase::Reset() {
    score = 0;
    UpdateDisplay();
}

// Sets the color of the display.
void CounterBase::SetColor(CRGB new_color) {
    color = new_color;
    UpdateDisplay();
}

// Gets the current display color.
CRGB CounterBase::GetColor() {
    return color;
}

// Gets the current score.
int CounterBase::GetScore() {
    return score;
}
