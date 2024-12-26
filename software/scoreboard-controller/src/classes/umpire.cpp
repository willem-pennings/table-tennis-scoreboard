#include "Umpire.h"

// Constructor: initializes the Umpire with score and game counters for both players.
Umpire::Umpire(ScoreCounter* score_counter_1, ScoreCounter* score_counter_2, GameCounter* game_counter_1,
        GameCounter* game_counter_2, TimeoutIndicator* timeout_indicator_1, TimeoutIndicator* timeout_indicator_2,
        CRGB color, int brightness, int win_score, int win_difference, int games_to_win_match) :
        score_counter_1(score_counter_1), score_counter_2(score_counter_2), game_counter_1(game_counter_1),
        game_counter_2(game_counter_2), timeout_indicator_1(timeout_indicator_1),
        timeout_indicator_2(timeout_indicator_2), color(color), brightness(brightness), win_score(win_score),
        win_difference(win_difference), games_to_win_match(games_to_win_match) {
    // Initialize by resetting the entire match.
    ResetMatch();
}

// Increments a player's score and check for game or match win.
void Umpire::IncrementScore(uint8_t player) {
    if (player == 1) {
        score_counter_1->Increment();
    } else if (player == 2) {
        score_counter_2->Increment();
    } else {
        // Invalid player.
    }
    FastLED.show();

    // Check if the game has been won by either player.
    CheckForGameWin();
}

// Decrements a player's score.
void Umpire::DecrementScore(uint8_t player) {
    if (player == 1) {
        score_counter_1->Decrement();
    } else if (player == 2) {
        score_counter_2->Decrement();
    } else {
        // Invalid player.
    }
    FastLED.show();
}

// Increments a player's game count.
void Umpire::IncrementGames(uint8_t player) {
    if (player == 1) {
        game_counter_1->Increment();
    } else if (player == 2) {
        game_counter_2->Increment();
    } else {
        // Invalid player.
    }
    FastLED.show();

    // Check match win status.
    CheckForMatchWin();
}

// Decrements a player's game count.
void Umpire::DecrementGames(uint8_t player) {
    if (player == 1) {
        game_counter_1->Decrement();
    } else if (player == 2) {
        game_counter_2->Decrement();
    } else {
        // Invalid player.
    }
    FastLED.show();

    // Check match win status.
    CheckForMatchWin();
}

// Gets the current score count for the selected player.
int Umpire::GetScore(uint8_t player) {
    if (player == 1) {
        return score_counter_1->GetScore();
    } else if (player == 2) {
        return score_counter_2->GetScore();
    } else {
        // Invalid player.
        return 0;
    }
}

// Gets the current game count for the selected player.
int Umpire::GetGames(uint8_t player) {
    if (player == 1) {
        return game_counter_1->GetScore();
    } else if (player == 2) {
        return game_counter_2->GetScore();
    } else {
        // Invalid player.
        return 0;
    }
}

// Toggles the timeout status for a player.
void Umpire::ToggleTimeout(uint8_t player) {
    if (player == 1) {
        timeout_indicator_1->ToggleTimeout();
    } else if (player == 2) {
        timeout_indicator_2->ToggleTimeout();
    } else {
        // Invalid player.
    }
    FastLED.show();
}

// Checks if either player has won the game, based on the current scores.
void Umpire::CheckForGameWin() {
    // Access both players' scores.
    int score1 = score_counter_1->GetScore();
    int score2 = score_counter_2->GetScore();

    bool match_won = false;

    if (score1 >= win_score && (score1 - score2) >= win_difference) {
        // Player 1 has reached the winning score and is ahead by the game win difference.
        match_won = true;

        // Increment player 1's game counter.
        game_counter_1->Increment();
        
        // Flash player 1's score and game count to signal the game win.
        FlashWinner(1, 5);
    } else if (score2 >= win_score && (score2 - score1) >= win_difference) {
        // Player 2 has reached the winning score and is ahead by the game win difference.
        match_won = true;

        // Increment player 2's game counter.
        game_counter_2->Increment();
        
        // Flash player 1's score and game count to signal the game win.
        FlashWinner(2, 5);
    } else {
        // No game winner.
    }

    if (match_won) {
        // Check for a match winner as well.
        CheckForMatchWin();

        // Players will swap sides, so swap the scores to the opposite sides too.
        SwapSides();

        // Reset scores for the next game.
        ResetScores();
    }
}

// Checks if either player has won the match.
void Umpire::CheckForMatchWin() {
    // Access both players' game counts.
    int games1 = game_counter_1->GetScore();
    int games2 = game_counter_2->GetScore();

    // Check if either player has won enough games to win the match.
    if (games1 >= games_to_win_match && games2 >= games_to_win_match) {
        // If both players have somehow reached the winning count, don't declare a winner.
    } else if (games1 >= games_to_win_match) {       
        // Player 1 wins the match.
        score_counter_1->SetColor(WIN_EFFECT_COLOR);
        game_counter_1->SetColor(WIN_EFFECT_COLOR);
        FastLED.show();
    } else if (games2 >= games_to_win_match) {
        // Player 2 wins the match.
        score_counter_2->SetColor(WIN_EFFECT_COLOR);
        game_counter_2->SetColor(WIN_EFFECT_COLOR);
        FastLED.show();
    } else {
        // No match winner.
    }

    // Reset counter colors, this is useful for when the user manually alters the game count (decrement).
    if (games1 < games_to_win_match) {
        score_counter_1->SetColor(color);
        game_counter_1->SetColor(color);
        FastLED.show();
    }
    
    if (games2 < games_to_win_match) {
        score_counter_2->SetColor(color);
        game_counter_2->SetColor(color);
        FastLED.show();
    }
}

// Flashes the winning player's score and game count when a player wins a game.
void Umpire::FlashWinner(uint8_t player, uint8_t number_of_flashes) {
    CRGB color_score, color_game;
    
    // Get the current color of the score and game count for the selected player.
    if (player == 1) {
        color_score = score_counter_1->GetColor();
        color_game = game_counter_1->GetColor();
    } else if (player == 2) {
        color_score = score_counter_2->GetColor();
        color_game = game_counter_2->GetColor();
    } else {
        // Invalid player.
        return;
    }
    
    for (int i = 0; i < number_of_flashes; ++i) {
        // Turn off the LEDs briefly.
        if (player == 1) {
            score_counter_1->SetColor(CRGB::Black);
            game_counter_1->SetColor(CRGB::Black);
        } else if (player == 2) {
            score_counter_2->SetColor(CRGB::Black);
            game_counter_2->SetColor(CRGB::Black);
        } else {
            // Invalid player.
        }
        FastLED.show();
        delay(250);
        
        // Turn them on again in the "win" color.
        if (player == 1) {
            score_counter_1->SetColor(WIN_EFFECT_COLOR);
            game_counter_1->SetColor(WIN_EFFECT_COLOR);
        } else if (player == 2) {
            score_counter_2->SetColor(WIN_EFFECT_COLOR);
            game_counter_2->SetColor(WIN_EFFECT_COLOR);
        } else {
            // Invalid player.
        }
        FastLED.show();
        delay(250);
    }
    
    // Reset the color of the counter to its original value.
    if (player == 1) {
        score_counter_1->SetColor(color_score);
        game_counter_1->SetColor(color_game);
    } else if (player == 2) {
        score_counter_2->SetColor(color_score);
        game_counter_2->SetColor(color_game);
    } else {
        // Invalid player.
    }
    FastLED.show();
}

// Moves the scores and games on the left side to the right and vice versa.
void Umpire::SwapSides() {
    // Get the current score and game counts, timeout statuses, and colors for both sides.
    int score_1 = GetScore(1);
    int score_2 = GetScore(2);
    int games_1 = GetGames(1);
    int games_2 = GetGames(2);
    bool timeout_1 = timeout_indicator_1->GetStatus();
    bool timeout_2 = timeout_indicator_2->GetStatus();

    CRGB color_score_1 = score_counter_1->GetColor();
    CRGB color_score_2 = score_counter_2->GetColor();
    CRGB color_games_1 = game_counter_1->GetColor();
    CRGB color_games_2 = game_counter_2->GetColor();
    CRGB color_timeout_1 = timeout_indicator_1->GetColor();
    CRGB color_timeout_2 = timeout_indicator_2->GetColor();

    // Apply the current values to the opposite sides.
    score_counter_1->SetScore(score_2);
    score_counter_2->SetScore(score_1);
    game_counter_1->SetScore(games_2);
    game_counter_2->SetScore(games_1);
    timeout_indicator_1->SetStatus(timeout_2);
    timeout_indicator_2->SetStatus(timeout_1);

    score_counter_1->SetColor(color_score_2);
    score_counter_2->SetColor(color_score_1);
    game_counter_1->SetColor(color_games_2);
    game_counter_2->SetColor(color_games_1);
    timeout_indicator_1->SetColor(color_timeout_2);
    timeout_indicator_2->SetColor(color_timeout_1);

    FastLED.show();
}

// Resets both players' scores to 0.
void Umpire::ResetScores() {
    score_counter_1->Reset();
    score_counter_2->Reset();
    FastLED.show();
}

// Resets both players' timeouts.
void Umpire::ResetTimeouts() {
    timeout_indicator_1->Reset();
    timeout_indicator_2->Reset();
    FastLED.show();
}

// Resets both players' game counts.
void Umpire::ResetGames() {
    game_counter_1->Reset();
    game_counter_2->Reset();
    FastLED.show();
}

// Resets the entire match (games, scores and timeouts).
void Umpire::ResetMatch() {
    // Reset all counters/states.
    ResetScores();
    ResetTimeouts();
    ResetGames();

    // Reset colors of all counters.
    score_counter_1->SetColor(color);
    score_counter_2->SetColor(color);
    game_counter_1->SetColor(color);
    game_counter_2->SetColor(color);
    FastLED.show();
}

// Sets the brightness of the screen.
void Umpire::SetBrightness(int new_brightness) {
    brightness = new_brightness;
    FastLED.setBrightness(brightness);
}

// Sets the color of the screen.
void Umpire::SetColor(CRGB new_color) {
    color = new_color;
    score_counter_1->SetColor(color);
    score_counter_2->SetColor(color);
    game_counter_1->SetColor(color);
    game_counter_2->SetColor(color);
    timeout_indicator_1->SetColor(color);
    timeout_indicator_2->SetColor(color);
    FastLED.show();
}

// Sets the number of games to win the match.
void Umpire::SetGamesToWin(int games) {
    games_to_win_match = games;
}

// Gets the current brightness of the screen.
int Umpire::GetBrightness() {
    return brightness;
}

// Gets the current number of games to win the match.
int Umpire::GetGamesToWin() {
    return games_to_win_match;
}

// Gets the current color of the screen.
CRGB Umpire::GetColor() {
    return color;
}

// Updates the display to display the current state of the game (if something else was previously displayed).
void Umpire::UpdateDisplay() {
    score_counter_1->UpdateDisplay();
    score_counter_2->UpdateDisplay();
    game_counter_1->UpdateDisplay();
    game_counter_2->UpdateDisplay();
    timeout_indicator_1->UpdateDisplay();
    timeout_indicator_2->UpdateDisplay();
    FastLED.show();
}

// Displays a nice farewell greeting.
void Umpire::Houdoe(CRGB effect_color) {
    // Clear the screen before displaying the text.
    FastLED.clear();

    // H
    score_counter_1->SetSegment(0, 1, effect_color);
    score_counter_1->SetSegment(0, 2, effect_color);
    score_counter_1->SetSegment(0, 4, effect_color);
    score_counter_1->SetSegment(0, 5, effect_color);
    score_counter_1->SetSegment(0, 6, effect_color);

    // O
    score_counter_1->SetSegment(1, 0, effect_color);
    score_counter_1->SetSegment(1, 1, effect_color);
    score_counter_1->SetSegment(1, 2, effect_color);
    score_counter_1->SetSegment(1, 3, effect_color);
    score_counter_1->SetSegment(1, 4, effect_color);
    score_counter_1->SetSegment(1, 5, effect_color);

    // U
    game_counter_1->SetSegment(1, effect_color);
    game_counter_1->SetSegment(2, effect_color);
    game_counter_1->SetSegment(3, effect_color);
    game_counter_1->SetSegment(4, effect_color);
    game_counter_1->SetSegment(5, effect_color);

    // D
    game_counter_2->SetSegment(1, effect_color);
    game_counter_2->SetSegment(2, effect_color);
    game_counter_2->SetSegment(3, effect_color);
    game_counter_2->SetSegment(4, effect_color);
    game_counter_2->SetSegment(6, effect_color);

    // O
    score_counter_2->SetSegment(0, 0, effect_color);
    score_counter_2->SetSegment(0, 1, effect_color);
    score_counter_2->SetSegment(0, 2, effect_color);
    score_counter_2->SetSegment(0, 3, effect_color);
    score_counter_2->SetSegment(0, 4, effect_color);
    score_counter_2->SetSegment(0, 5, effect_color);

    // E
    score_counter_2->SetSegment(1, 0, effect_color);
    score_counter_2->SetSegment(1, 3, effect_color);
    score_counter_2->SetSegment(1, 4, effect_color);
    score_counter_2->SetSegment(1, 5, effect_color);
    score_counter_2->SetSegment(1, 6, effect_color);

    FastLED.show();
}
