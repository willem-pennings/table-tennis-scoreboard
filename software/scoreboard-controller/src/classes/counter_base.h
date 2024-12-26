#ifndef counter_base_h
#define counter_base_h

#include <FastLED.h>
#include "../definitions/parameters.h"
#include "segment.h"

class CounterBase {
    protected:
        // Current score (0-99 or 0-9 depending on the number of digits available).
        int score;
        // Current color for the counter display.
        CRGB color;

        // Protected constructor to be called by subclasses.
        CounterBase();

    public:
        // Increments the score (to be overridden by subclasses if needed).
        virtual void Increment();

        // Decrements the score (to be overridden by subclasses if needed).
        virtual void Decrement();

        // Sets the score to a specific value (to be overridden by subclasses if needed).
        virtual void SetScore(int value);

        // Resets the score.
        virtual void Reset();

        // Sets the color of the counter display.
        virtual void SetColor(CRGB new_color);
        
        // Gets the current display color.
        virtual CRGB GetColor();
        
        // Gets the current score.
        virtual int GetScore();

        // Pure virtual method to update the display, to be implemented in subclasses.
        virtual void UpdateDisplay() = 0;
        
};

#endif
