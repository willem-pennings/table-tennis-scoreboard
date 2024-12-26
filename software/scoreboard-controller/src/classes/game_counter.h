#ifndef game_counter_h
#define game_counter_h

#include "counter_base.h"

class GameCounter : public CounterBase {
    private:
        // Single 7-segment display for the game counter.
        SevenSegmentDisplay *display;

    public:
        // Constructor to initialize the game counter with one 7-segment display.
        GameCounter(SevenSegmentDisplay* display);

        // Increments the score.
        void Increment() override;

        // Sets the score to a specific value.
        void SetScore(int value) override;

        // Updates the displays with the current score and color.
        void UpdateDisplay() override;

        // Sets the color of a specific segment.
        void SetSegment(uint8_t segment);
        void SetSegment(uint8_t segment, CRGB manual_color);
};

#endif
