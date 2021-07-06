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
                        print("\x1b[mLogin successful\r\n\r\n");
                        print(getPrompt());
                        auditLogger.printf(LOG_TERMINAL, LOG_SECURITY, "Successful telnet login from %s\n",
                            client.remoteIP().toString().c_str());
                        logger.printf(LOG_TERMINAL, LOG_SECURITY, "Successful telnet login from %s\n",
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
                        printf("\x1b[1;31mLogin failed - wait %d seconds before reconnecting\x1b[m\r\n", connectionBlock);
                        logger.printf(LOG_TERMINAL, LOG_SECURITY, "Failed telnet login from %s\n",
                            client.remoteIP().toString().c_str());
                        auditLogger.printf(LOG_TERMINAL, LOG_SECURITY, "Failed telnet login from %s\n",
                            client.remoteIP().toString().c_str());
                        client.stop();
                        lastConnection = millis();
                    }
                    bufferUsed = 0;
                } else if (ch == 8) {
                    if (bufferUsed > 0) {
                        --bufferUsed;
                    }
                    printPasswordPrompt(bufferUsed);

                } else if (ch >= 32) {
                    if (bufferUsed < bufferSize - 1) {
                        buffer[bufferUsed++] = ch;
                    }
                    printPasswordPrompt(bufferUsed);
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
        if (connected) {
            logger.send(LOG_TERMINAL, LOG_INFO, "Telnet client disconnected\n");
            connected = false;
            logger.setTelnetOut(false);
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
                logger.send(LOG_TERMINAL, LOG_INFO, "Telnet client connected\n");
                connected = true;
                needsAuthentication = true;
                bufferUsed = 0;

                printPasswordPrompt(0);
            }
        }
    }
}

void TelnetTerminal::printPasswordPrompt(int len) {
    char buf[21];
    if (len > sizeof(buf) - 1) {
        len = sizeof(buf) - 1;
    }
    memset(buf, '*', len);
    buf[len] = 0;
    printf("\r\x1b[m\x1b[1;31mPassword: \x1b[m\x1b[0K%s\x1b[2;41;31m\x1b[1;%dH", buf, len + 11);
}

char TelnetTerminal::getType() const {
    return 'T';
}

void TelnetTerminal::doQuit() {
    client.stop();
}

void TelnetTerminal::setMonitoring(bool m) const {
    logger.setTelnetOut(m);
}

bool TelnetTerminal::getMonitoring() const {
    return logger.getTelnetOut();
}
