#ifndef scoreboard_server_h
#define scoreboard_server_h

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

#include "umpire.h"

class ScoreboardServer {
    private:
        // Wi-Fi SSID.
        const char* ssid;
        // Wi-Fi password.
        const char* password;
        // Umpire to keep track of the game.
        Umpire* umpire;
        // Web server to allow user interaction.
        AsyncWebServer server;

    public:
        // Constructor that takes an Umpire object.
        ScoreboardServer(const char* ssid, const char* password, Umpire* umpire);

        // Starts the WiFi AP and the web server.
        void Begin();

        // Handlers for web interactions.
        void HandleRoot(AsyncWebServerRequest *request);
        void HandleIncrementScore(AsyncWebServerRequest *request, uint8_t player);
        void HandleDecrementScore(AsyncWebServerRequest *request, uint8_t player);
        void HandleIncrementGames(AsyncWebServerRequest *request, uint8_t player);
        void HandleDecrementGames(AsyncWebServerRequest *request, uint8_t player);
        void HandleTimeout(AsyncWebServerRequest *request, uint8_t player);
        void HandleSwap(AsyncWebServerRequest *request);
        void HandleResetMatch(AsyncWebServerRequest *request);
        void HandleFirmwareUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
        void HandleSettings(AsyncWebServerRequest *request);
        void HandleRefresh(AsyncWebServerRequest *request);

        // Easter egg handlers.
        void HandleHoudoe(AsyncWebServerRequest *request);
};

#endif
