#include "telnet_terminal.h"

#include "logger.h"
#include "config.h"

TelnetTerminal telnetTerminal;

TelnetTerminal::TelnetTerminal(): server(TELNET_PORT), password(TELNET_PASSWORD) {
}

void TelnetTerminal::initialise() {
    server.begin();
    server.setNoDelay(true);
    Terminal::initialise();
}

void TelnetTerminal::internalSend(const char *str) {
    if (client) {
        client.print(str);
    }
}

void TelnetTerminal::processData() {
    if (client) {
        size_t avail = client.available();

        if (needsAuthentication) {
            while(avail--) {
                char ch = client.read();
                if (ch == '\n') {
                    print("\r\n");
                    buffer[bufferUsed] = 0;
                    if ((!password.empty()) && (password == buffer)) {
                        needsAuthentication = false;
                        loginAttempts = 0;
                        print("Login successful\r\n\r\n");
                        print(getPrompt());
                        auditLogger.print(LOG_TERMINAL, LOG_SECURITY, "Successful telnet login from %s\n",
                            client.remoteIP().toString().c_str());
                        logger.print(LOG_TERMINAL, LOG_SECURITY, "Successful telnet login from %s\n",
                            client.remoteIP().toString().c_str());
                    } else {
                        switch (++loginAttempts) {
                            case 1: 
                                connectionBlock = 15;
                                break;
                            case 2: 
                                connectionBlock = 30;
                                break;
                            case 3: 
                            case 4:
                            case 5:
                            case 6:
                            case 7:
                                connectionBlock = 60 * (loginAttempts - 2);
                                break;
                            default:
                                connectionBlock = 300;
                                break;
                        }
                        printf("Login failed - wait %d seconds before reconnecting\r\n", connectionBlock);
                        logger.print(LOG_TERMINAL, LOG_SECURITY, "Failed telnet login from %s\n",
                            client.remoteIP().toString().c_str());
                        auditLogger.print(LOG_TERMINAL, LOG_SECURITY, "Failed telnet login from %s\n",
                            client.remoteIP().toString().c_str());
                        client.stop();
                        lastConnection = millis();
                    }
                    bufferUsed = 0;
                } else if (ch == 8) {
                    if (bufferUsed > 0) {
                        --bufferUsed;
                    }
                } else if (ch >= 32) {
                    if (bufferUsed < bufferSize - 1) {
                        buffer[bufferUsed++] = ch;
                    }
                    print("\x08*");
                }
            }

        } else {
            while(avail--) {
                char ch = client.read();
                if (ch == 8) {
                    print(" \x08");
                }
                processCharacter(ch);
            }
        }

    } else if (TELNET_PORT != 0) {
        if (isConnected) {
            logger.print(LOG_TERMINAL, LOG_INFO, "Telnet client disconnected\n");
            isConnected = false;
        }
        if (server.hasClient()) {

            if (millis() - lastConnection < connectionBlock * 1000) {
                client = server.available();
                print("Access blocked\r\n");
                client.stop();
            } else {
                client = server.available();
                client.setNoDelay(true);
                client.flush();
                logger.print(LOG_TERMINAL, LOG_INFO, "Telnet client connected\n");
                isConnected = true;
                needsAuthentication = true;
                bufferUsed = 0;

                client.print("Password: ");
            }
        }
    }
}

char TelnetTerminal::getType() const {
    return 'T';
}

void TelnetTerminal::doQuit() {
    client.stop();
}
