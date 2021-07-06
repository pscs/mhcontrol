#pragma once

//#include "ESPTelnet.h"
#include "WiFi.h"
#include "terminal.h"

class TelnetTerminal: public Terminal {
public:
    TelnetTerminal();

    bool isConnected() const {
        return connected;
    }

    void initialise() override;

    void processData() override;
    void internalSend(const char *str) override;
    void setMonitoring(bool m) const override;
    bool getMonitoring() const override;;



    char getType() const override;

    void setPassword(const char *p) {
        password = p;
    }

    std::string getPassword() const {
        return password;
    }

    void doQuit() override;
    const char *getPrompt() const override {
        return "> \x1b[0K";
    }

private:
    void printPasswordPrompt(int len);

    //ESPTelnet telnet;
    WiFiServer server;
    WiFiClient client;
    bool connected = false;
    bool needsAuthentication = true;
    uint8_t loginAttempts = 0;
    std::string password;
    uint32_t lastConnection = 0;
    uint32_t connectionBlock = 0;
};

extern TelnetTerminal telnetTerminal;