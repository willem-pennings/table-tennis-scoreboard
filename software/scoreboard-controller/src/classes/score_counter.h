#ifndef score_counter_h
#define score_counter_h

#include "counter_base.h"

class ScoreCounter : public CounterBase {
    public:
        // Side enumerator
        enum Side {
            LEFT,
            RIGHT
        };

        // Constructor to initialize the score counter with two 7-segment displays.
        ScoreCounter(SevenSegmentDisplay* display_tens, SevenSegmentDisplay* display_ones, Side side);

        // Increments the score.
        void Increment() override;

        // Sets the score to a specific value.
        void SetScore(int value) override;

        // Updates the displays with the current score and color.
        void UpdateDisplay() override;

        // Sets the color of a specific segment in a specific digit.
        void SetSegment(uint8_t digit, uint8_t segment);
        void SetSegment(uint8_t digit, uint8_t segment, CRGB manual_color);

    private:
        // Display for the tens place.
        SevenSegmentDisplay *display_tens;
        // Display for the ones place.
        SevenSegmentDisplay *display_ones;
        // Side of the screen on which the score counter is displayed.
        Side side;
};

#endif
