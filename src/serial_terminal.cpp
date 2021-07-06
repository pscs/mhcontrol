#include "serial_terminal.h"

#include <Arduino.h>

#include "logger.h"

SerialTerminal serialTerminal;

void SerialTerminal::processData() {
    size_t avail = Serial.available();

    while(avail--) {
        char ch = Serial.read();
        processCharacter(ch);
    }
}

void SerialTerminal::internalSend(const char *buffer) {
    Serial.print(buffer);
}

char SerialTerminal::getType() const {
    return 'S';
}

void SerialTerminal::setMonitoring(bool m) const {
    logger.setSerialOut(m);
}

bool SerialTerminal::getMonitoring() const {
    return logger.getSerialOut();
}


