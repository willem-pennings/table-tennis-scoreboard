#ifndef umpire_h
#define umpire_h

#include "score_counter.h"
#include "game_counter.h"
#include "timeout_indicator.h"
#include "../definitions/parameters.h"

#include "Arduino.h"

class Umpire {
    private:
        // Score counter for player 1.
        ScoreCounter* score_counter_1;
        // Score counter for player 2.
        ScoreCounter* score_counter_2;
        // Game counter for player 1.
        GameCounter* game_counter_1;
        // Game counter for player 2.
        GameCounter* game_counter_2;
        // Timeout indicator for player 1.
        TimeoutIndicator* timeout_indicator_1;
        // Timeout indicator for player 2.
        TimeoutIndicator* timeout_indicator_2;

        // Screen color setting.
        CRGB color;
        // Screen brightness setting.
        int brightness;
        // Score required to win a game (e.g., 11).
        int win_score;
        // Minimum score difference to win a game (e.g., 2).
        int win_difference;
        // Number of games needed to win the match (e.g., 2 out of 3 or 3 out of 5).
        int games_to_win_match;

        // Checks if a player has won a game and reset the scores.
        void CheckForGameWin();
        // Checks if a player has won the match.
        void CheckForMatchWin();
        // Flashes the winning player's score and game count.
        void FlashWinner(uint8_t player, uint8_t number_of_flashes);

    public:
        // Constructor to initialize umpire with two score counters, two game counters, and two timeout indicators.
        Umpire(ScoreCounter* score_counter_1, ScoreCounter* score_counter_2, GameCounter* game_counter_1, 
            GameCounter* game_counter_2, TimeoutIndicator* timeout_indicator_1, TimeoutIndicator* timeout_indicator_2,
            CRGB color = DEFAULT_COLOR, int brightness = BRIGHTNESS, int win_score = WIN_SCORE,
            int win_difference = WIN_DIFFERENCE, int games_to_win_match = GAMES_TO_WIN_MATCH);

        // Increments or decrement player 1 or player 2's score or game count.
        void IncrementScore(uint8_t player);
        void DecrementScore(uint8_t player);
        void IncrementGames(uint8_t player);
        void DecrementGames(uint8_t player);
        
        // Gets player 1 or player 2's score or game count.
        int GetScore(uint8_t player);
        int GetGames(uint8_t player);
        
        // Toggles the timeout status for player 1 or player 2.
        void ToggleTimeout(uint8_t player);

        // Swaps the score and game counts (scores and games on left move to right and vice versa).
        void SwapSides();

        // Resets both players' scores, game counts or timeout status.
        void ResetScores();
        void ResetTimeouts();
        void ResetGames();
        
        // Resets the entire match (games, scores and timeouts).
        void ResetMatch();

        // Updates the display to display the current state of the game (if something else was previously displayed).
        void UpdateDisplay();

        // Sets user-adjustable values.
        void SetBrightness(int brightness);
        void SetColor(CRGB new_color);
        void SetGamesToWin(int games);

        // Gets user-adjustable values.
        int GetBrightness();
        int GetGamesToWin();
        CRGB GetColor();

        // Easter eggs.
        void Houdoe(CRGB effect_color);
};

#endif