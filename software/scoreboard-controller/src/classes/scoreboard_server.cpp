#include "scoreboard_server.h"

// Constructor.
ScoreboardServer::ScoreboardServer(const char* ssid, const char* password, Umpire* umpire) : ssid(ssid),
    password(password), umpire(umpire), server(80) {}

// Starts the Wi-Fi AP and web server.
void ScoreboardServer::Begin() {
    // Setup the access point.
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password, 1, false, 4);

    // Define endpoints for player interactions.
    server.on("/", HTTP_GET, std::bind(&ScoreboardServer::HandleRoot, this, std::placeholders::_1));
    server.on("/incrementscore1", HTTP_GET, std::bind(&ScoreboardServer::HandleIncrementScore, this, std::placeholders::_1, 1));
    server.on("/incrementscore2", HTTP_GET, std::bind(&ScoreboardServer::HandleIncrementScore, this, std::placeholders::_1, 2));
    server.on("/decrementscore1", HTTP_GET, std::bind(&ScoreboardServer::HandleDecrementScore, this, std::placeholders::_1, 1));
    server.on("/decrementscore2", HTTP_GET, std::bind(&ScoreboardServer::HandleDecrementScore, this, std::placeholders::_1, 2));
    server.on("/timeout1", HTTP_GET, std::bind(&ScoreboardServer::HandleTimeout, this, std::placeholders::_1, 1));
    server.on("/timeout2", HTTP_GET, std::bind(&ScoreboardServer::HandleTimeout, this, std::placeholders::_1, 2));
    server.on("/incrementgames1", HTTP_GET, std::bind(&ScoreboardServer::HandleIncrementGames, this, std::placeholders::_1, 1));
    server.on("/incrementgames2", HTTP_GET, std::bind(&ScoreboardServer::HandleIncrementGames, this, std::placeholders::_1, 2));
    server.on("/decrementgames1", HTTP_GET, std::bind(&ScoreboardServer::HandleDecrementGames, this, std::placeholders::_1, 1));
    server.on("/decrementgames2", HTTP_GET, std::bind(&ScoreboardServer::HandleDecrementGames, this, std::placeholders::_1, 2));
    server.on("/swap", HTTP_GET, std::bind(&ScoreboardServer::HandleSwap, this, std::placeholders::_1));
    server.on("/reset", HTTP_GET, std::bind(&ScoreboardServer::HandleResetMatch, this, std::placeholders::_1));
    server.on("/settings", HTTP_POST, std::bind(&ScoreboardServer::HandleSettings, this, std::placeholders::_1));
    server.on("/refresh", HTTP_GET, std::bind(&ScoreboardServer::HandleRefresh, this, std::placeholders::_1));
    server.on("/houdoe", HTTP_GET, std::bind(&ScoreboardServer::HandleHoudoe, this, std::placeholders::_1));


    // Define the OTA update handler.
    server.on(
        "/update", HTTP_POST, [](AsyncWebServerRequest *request) {},
        [this](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
            this->HandleFirmwareUpdate(request, filename, index, data, len, final);
        }
    );

    // Start the web server.
    server.begin();
}

// Serves the main page to control the score board.
void ScoreboardServer::HandleRoot(AsyncWebServerRequest *request) {
    // Build the HTML page.
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html lang="en">
        <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Scoreboard</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 0;
                display: flex;
                flex-direction: column;
                align-items: center;
                justify-content: center;
                min-height: 100vh;
                background-color: #f4f4f9;
                box-sizing: border-box;
            }

            .container {
                display: flex;
                flex-direction: column;
                gap: 20px;
                width: 90%;
                max-width: 600px;
                padding: 10px;
                box-sizing: border-box;
            }

            .row {
                display: flex;
                justify-content: space-between;
                gap: 10px;
                max-width: 600px;
                width: 100%;
                margin: 0 auto;
            }

            .column {
                display: flex;
                flex-direction: column;
                align-items: center;
                flex: 1;
                padding: 10px;
                background-color: #fff;
                box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
                border-radius: 8px;
                position: relative;
            }

            .score-container {
                display: flex;
                justify-content: center;
                align-items: flex-end;
                gap: 10px;
                margin-top: 10px;
            }

            .score {
                font-size: 4rem;
                font-weight: bold;
                color: #333;
                line-height: 1;
            }

            .games {
                font-size: 2rem;
                font-weight: bold;
                color: #666;
                line-height: 1;
                align-self: flex-end;
                margin-top: 1em;
            }

            a.button {
                display: block;
                width: 100%;
                margin: 5px 0;
                font-size: 1.2rem;
                font-weight: bold;
                text-decoration: none;
                text-align: center;
                border-radius: 5px;
                transition: background-color 0.3s; transform 0.2s;
            }

            a.button:hover {
                transform: scale(1.02);
            }

            .button.increment-score {
                background-color: #28a745;
                color: white;
                padding: 50px 0px;
            }

            .button.increment-score:hover {
                background-color: #218838;
            }

            .button.increment-games {
                background-color: #28a745;
                color: white;
                padding: 10px 0px;
            }

            .button.increment-games:hover {
                background-color: #218838;
            }

            .button.decrement {
                background-color: #dc3545;
                color: white;
                padding: 10px 0px;
            }

            .button.decrement:hover {
                background-color: #c82333;
            }

            .button.timeout {
                background-color: #007bff;
                color: white;
                padding: 10px 0px;
            }

            .button.timeout:hover {
                background-color: #0056b3;
            }

            .reset {
                display: block;
                max-width: 100%;
                width: 100%;
                margin: 0;
                padding: 15px;
                font-size: 1.2rem;
                background-color: #ff595e;
                color: white;
                text-decoration: none;
                text-align: center;
                border-radius: 8px;
                cursor: pointer;
                box-sizing: border-box;
            }

            .reset:hover {
                background-color: #ff3b40;
            }

            .swap {
                display: block;
                max-width: 100%;
                width: 100%;
                margin: 0;
                padding: 15px;
                font-size: 1.2rem;
                background-color: #e26928;
                color: white;
                text-decoration: none;
                text-align: center;
                border-radius: 8px;
                cursor: pointer;
                box-sizing: border-box;
            }

            .swap:hover {
                background-color: #d84528;
            }

            .update {
                display: flex;
                flex-direction: column;
                align-items: center;
                margin-top: 20px auto 0;
                padding: 15px;
                background-color: #fff;
                box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
                border-radius: 8px;
                width: 100%;
                box-sizing: border-box;
            }

            .update input[type="file"] {
                margin: 10px 0;
            }

            .update input[type="submit"] {
                padding: 10px 20px;
                font-size: 1rem;
                font-weight: bold;
                color: white;
                background-color: #007bff;
                border: none;
                border-radius: 5px;
                cursor: pointer;
                width: auto;
            }

            .update input[type="submit"]:hover {
                background-color: #0056b3;
            }

            .settings {
                display: flex;
                flex-direction: column;
                align-items: center;
                margin: 20px auto;
                padding: 15px;
                background-color: #fff;
                box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
                border-radius: 8px;
                max-width: 600px;
                width: 100%;
                box-sizing: border-box;
            }

            .settings form {
                width: 100%;
                display: flex;
                flex-direction: column;
                gap: 15px;
            }

            .form-group {
                display: flex;
                flex-direction: column;
                width: 100%;
            }

            .form-group label {
                font-size: 1rem;
                font-weight: bold;
                margin-bottom: 5px;
            }

            .form-group select,
            .form-group input[type="range"],
            .form-group input[type="number"],
            .form-group input[type="submit"] {
                padding: 10px;
                font-size: 1rem;
                border: 1px solid #ccc;
                border-radius: 5px;
                width: 100%;
                box-sizing: border-box;
            }

            .form-group input[type="range"] {
                cursor: pointer;
            }

            .form-group input[type="submit"] {
                font-weight: bold;
                color: white;
                background-color: #007bff;
                border: none;
                cursor: pointer;
                transition: background-color 0.3s;
            }

            .form-group input[type="submit"]:hover {
                background-color: #0056b3;
            }

            @media (min-width: 667px) and (orientation: landscape) {
                .row {
                    flex-direction: row;
                    gap: 20px;
                }

                .reset, .update {
                    margin-top: 10px;
                }
            }
        </style>
        </head>
        <body>
            <div class="container">
                <div class="row">
                    <div class="column" id="player1">
                        <h2>Left</h2>
                        <div class="score-container">
                            <div class="score" id="score1">PLAYER_1_SCORE</div>
                            <div class="games" id="games1">PLAYER_1_GAMES</div>
                        </div>
                        <a href="/incrementscore1" class="button increment-score">Score +1</a>
                        <a href="/decrementscore1" class="button decrement">Score -1</a>
                        <a href="/timeout1" class="button timeout">Timeout</a>
                        <a href="/incrementgames1" class="button increment-games">Games +1</a>
                        <a href="/decrementgames1" class="button decrement">Games -1</a>
                    </div>
                    <div class="column" id="player2">
                        <h2>Right</h2>
                        <div class="score-container">
                            <div class="score" id="score2">PLAYER_2_SCORE</div>
                            <div class="games" id="games2">PLAYER_2_GAMES</div>
                        </div>
                        <a href="/incrementscore2" class="button increment-score">Score +1</a>
                        <a href="/decrementscore2" class="button decrement">Score -1</a>
                        <a href="/timeout2" class="button timeout">Timeout</a>
                        <a href="/incrementgames2" class="button increment-games">Games +1</a>
                        <a href="/decrementgames2" class="button decrement">Games -1</a>
                    </div>
                </div>
                <a href="/swap" class="swap">Swap sides</a>
                <a href="/reset" class="reset">Reset match</a>
                <div class="settings">
                    <form method="POST" action="/settings">
                        <div class="form-group">
                            <label for="color">Display color:</label>
                            <select id="color" name="color">
                                <option value="white">White</option>
                                <option value="red">Red</option>
                                <option value="green">Green</option>
                                <option value="blue">Blue</option>
                                <option value="yellow">Yellow</option>
                                <option value="chocolate">Chocolate</option>
                                <option value="orange">Orange</option>
                                <option value="gold">Gold</option>
                            </select>
                        </div>
                        <div class="form-group">
                            <label for="brightness">Brightness:</label>
                            <input type="range" id="brightness" name="brightness" min="20" max="128" value="DEFAULT_BRIGHTNESS">
                        </div>
                        <div class="form-group">
                            <label for="games-to-win">Games to win:</label>
                            <input type="number" id="games-to-win" name="games-to-win" min="1" max="9" value="GAMES_TO_WIN">
                        </div>
                        <div class="form-group">
                            <input type="submit" value="Apply settings">
                        </div>
                    </form>
                </div>
                <div class="update">
                    <form method="POST" action="/update" enctype="multipart/form-data">
                        <input type="file" name="update">
                        <input type="submit" value="Update">
                    </form>
                </div>
            </div>
        </body>
        </html>
    )rawliteral";

    // Replace the placeholders.
    html.replace("PLAYER_1_SCORE", String(umpire->GetScore(1)));
    html.replace("PLAYER_2_SCORE", String(umpire->GetScore(2)));
    html.replace("PLAYER_1_GAMES", String(umpire->GetGames(1)));
    html.replace("PLAYER_2_GAMES", String(umpire->GetGames(2)));
    html.replace("DEFAULT_BRIGHTNESS", String(umpire->GetBrightness()));
    html.replace("GAMES_TO_WIN", String(umpire->GetGamesToWin()));

    request->send(200, "text/html", html);
}

// Handles increment score requests.
void ScoreboardServer::HandleIncrementScore(AsyncWebServerRequest *request, uint8_t player) {
    umpire->IncrementScore(player);
    request->redirect("/");
}

// Handles decrement score requests.
void ScoreboardServer::HandleDecrementScore(AsyncWebServerRequest *request, uint8_t player) {
    umpire->DecrementScore(player);
    request->redirect("/");
}

// Handles increment game requests.
void ScoreboardServer::HandleIncrementGames(AsyncWebServerRequest *request, uint8_t player) {
    umpire->IncrementGames(player);
    request->redirect("/");
}

// Handles decrement game requests.
void ScoreboardServer::HandleDecrementGames(AsyncWebServerRequest *request, uint8_t player) {
    umpire->DecrementGames(player);
    request->redirect("/");
}

// Handles timeout requests.
void ScoreboardServer::HandleTimeout(AsyncWebServerRequest *request, uint8_t player) {
    umpire->ToggleTimeout(player);
    request->redirect("/");
}

// Handles side swaps.
void ScoreboardServer::HandleSwap(AsyncWebServerRequest *request) {
    umpire->SwapSides();
    request->redirect("/");
}

// Handles reset match.
void ScoreboardServer::HandleResetMatch(AsyncWebServerRequest *request) {
    umpire->ResetMatch();
    request->redirect("/");
}

// Applies settings.
void ScoreboardServer::HandleSettings(AsyncWebServerRequest *request) {
    // Update screen brightness.
    if (request->hasParam("brightness", true)) {
        const AsyncWebParameter* p = request->getParam("brightness", true);
        umpire->SetBrightness(p->value().toInt());
    }

    // Update screen color.
    if (request->hasParam("color", true)) {
        const AsyncWebParameter* p = request->getParam("color", true);
        String color = p->value();
        if (color == "red") {
            umpire->SetColor(CRGB::Red);
        } else if (color == "green") {
            umpire->SetColor(CRGB::Green);
        } else if (color == "blue") {
            umpire->SetColor(CRGB::Blue);
        } else if (color == "yellow") {
            umpire->SetColor(CRGB::Yellow);
        } else if (color == "white") {
            umpire->SetColor(CRGB::White);
        } else if (color == "chocolate") {
            umpire->SetColor(CRGB::Chocolate);
        } else if (color == "gold") {
            umpire->SetColor(CRGB::Gold);
        } else if (color == "orange") {
            umpire->SetColor(CRGB::Orange);
        }
    }

    // Update the number of games to win a match.
    if (request->hasParam("games-to-win", true)) {
        const AsyncWebParameter* p = request->getParam("games-to-win", true);
        umpire->SetGamesToWin(p->value().toInt());
    }

    // Redirect back to the main page after applying settings.
    request->redirect("/");
}

// Handles OTA firmware updates.
void ScoreboardServer::HandleFirmwareUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) {
        if (!Update.begin(request->contentLength(), U_FLASH)) {
            // An error occurred.
        }
    }

    if (Update.write(data, len) != len) {
        // An error occurred.
    }

    if (final) {
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots...");
        response->addHeader("Refresh", "5");
        response->addHeader("Location", "/");
        request->send(response);
        if (Update.end(true)) {
            // Update successful.
            ESP.restart();
        }
    }
}

// Refreshes the screen to display the current game state.
void ScoreboardServer::HandleRefresh(AsyncWebServerRequest *request) {
    umpire->UpdateDisplay();
    request->redirect("/");
}

// Easter egg.
void ScoreboardServer::HandleHoudoe(AsyncWebServerRequest *request) {
    umpire->Houdoe(CRGB::White);
    request->redirect("/");
}
