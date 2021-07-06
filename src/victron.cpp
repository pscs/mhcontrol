#include "victron.h"

#include <Arduino.h>
#include "ble.h"

#include "logger.h"
#include "DS3231.h"

static const BLEUUID generalServiceUUID("306b0001-b081-4037-83dc-e59fcc3cdfd0");
static const BLEUUID charUUID1("306b0002-b081-4037-83dc-e59fcc3cdfd0");
static const BLEUUID charUUID2("306b0003-b081-4037-83dc-e59fcc3cdfd0");
static const BLEUUID charUUID3("306b0004-b081-4037-83dc-e59fcc3cdfd0");

const int NotifyBufferSize = 100;


Victron::Victron(const char *addr, uint32_t passcode): MyBLEClient(addr, true, passcode) {
    pClientCallback->setPasscode(passcode);
}

Victron::~Victron() {
    logger.send(LOG_VICTRON, LOG_DEBUG, "Victron - destructor\n");
    unregisterAll();
}

bool Victron::connectIfNecessary()
{
    if (!pNotifyBuffer) {
        pNotifyBuffer = reinterpret_cast<uint8_t *>(ps_malloc(NotifyBufferSize));
    }
    logger.send(LOG_VICTRON, LOG_INFO, "Victron - try connect\n");
    if (isConnected() || demoMode) {
        logger.send(LOG_VICTRON, LOG_VERBOSE, "Victron - already connected\n");
        return true;
    } else if (!isEnabled()) {
        logger.send(LOG_VICTRON, LOG_VERBOSE, "Victron - disabled\n");
        return false;
    }

    //bleManager.setPasscode(passcode);

//    pClient = BLEDevice::createClient();
//    pClient->setClientCallbacks(m_ClientCallback);
//    pClient->setClientCallbacks(new MPPTClientCallback());

    logger.printf(LOG_VICTRON, LOG_INFO, "Victron - start connect - %s\n", address.toString().c_str());

    if (pClient->connect(address, false)) {
        logger.printf(LOG_VICTRON, LOG_INFO, "%s - Connected to Server\r\n", getName());
    } else {
        logger.printf(LOG_VICTRON, LOG_WARNING, "%s - Failed to connect to server\r\n", getName());
        return false;
    }

    pClient->secureConnection();
    
    BLERemoteService* pRemoteService = pClient->getService(getServiceUUID());
    if (pRemoteService == nullptr) {
        logger.printf(LOG_VICTRON, LOG_WARNING, "%s Failed to find our service UUID: %s\r\n", getName(), getServiceUUID().toString().c_str());
        pClient->disconnect();
        return false;
    } else {
        logger.printf(LOG_VICTRON, LOG_VERBOSE, " - %s Found our service\r\n", getName());

        pChar1 = pRemoteService->getCharacteristic(charUUID1);
        if (pChar1 == nullptr) {
            pClient->disconnect();
            return false;
        } else {
    
            if(pChar1->canNotify()) {
                registerCharacteristics(pChar1, this, 1);
                //characteristics[pChar1] = NotifyInfo(this, 1);
                pChar1->subscribe(true, &notifyCallback);
//                pChar1->registerForNotify(&notifyCallback);
            }
        }

        pChar2 = pRemoteService->getCharacteristic(charUUID2);
        if (pChar2) {
            if (pChar2->canNotify()) {
                registerCharacteristics(pChar2, this, 2);
                pChar2->subscribe(true, &notifyCallback);
//                pChar2->registerForNotify(&notifyCallback);
            }
        }
        pChar3 = pRemoteService->getCharacteristic(charUUID3);
        if (pChar3) {
            if(pChar3->canNotify()){
                registerCharacteristics(pChar3, this, 3);
                pChar3->subscribe(true, &notifyCallback);
//                pChar3->registerForNotify(&notifyCallback);
            }
//        const uint8_t v[] = {0x1,0x0};

          //pMPPTCharacteristic3->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)v, 2, false);


            const uint8_t v1[] = {0xfa, 0x80, 0xff};
            pChar1->writeValue((uint8_t*)v1, 3, false);
            const uint8_t v2[] = {0xf9, 0x80};
            pChar1->writeValue((uint8_t*)v2, 2, false);
            const uint8_t v3[] = {0x01};
            pChar2->writeValue((uint8_t*)v3, 1, false);
            const uint8_t v4[] = {0x03, 0x00};
            pChar2->writeValue((uint8_t*)v4, 2, false);
            const uint8_t v5[] = {0x06, 0x00, 0x82, 0x18, 0x93, 0x42, 0x10, 0x27, 0x03, 0x01, 0x03, 0x03};
            pChar2->writeValue((uint8_t*)v5, sizeof(v5), false);
            //from Internet - const uint8_t v6[] = {0x05, 0x00, 0x81, 0x19, 0xec, 0x0f, 0x05, 0x00, 0x81, 0x19, 0xec, 0x0e, 0x05, 0x00, 0x81, 0x19, 0x01, 0x0c, 0x05, 0x00};
            
            //Smartsolar packet capture
            const uint8_t v6[] = {0x05, 0x00, 0x81, 0x19, 0x01, 0x09, 0x05, 0x00, 0x81, 0x19, 0x01, 0x0a, 0x05, 0x00, 0x81, 0x19, 0xec, 0x0f, 0x05, 0x00};
            
            pChar3->writeValue((uint8_t*)v6, sizeof(v6), false);
            //from Internet const uint8_t v7[] = {0x81, 0x18, 0x90, 0x05, 0x00, 0x81, 0x19, 0xec, 0x3f, 0x05, 0x00, 0x81, 0x19, 0xec, 0x12};

            //Smartsolar packet capture
            const uint8_t v7[] = {0x81, 0x19, 0xec, 0x0e, 0x05, 0x00, 0x81, 0x19, 0x01, 0x0c, 0x05, 0x00, 0x81, 0x18, 0x90, 0x05, 0x00, 0x81, 0x19, 0xec};
            pChar3->writeValue((uint8_t*)v7, sizeof(v7), false);

            //Smartsolar packet capture
            const uint8_t v8[] = {0x3f, 0x05, 0x00, 0x81, 0x19, 0xec, 0x12};
            pChar2->writeValue((uint8_t*)v7, sizeof(v7), false);

            const uint8_t v9[] = {0x05, 0x03, 0x81, 0x19, 0x02, 0x01, 0x05, 0x03, 0x81, 0x19, 0xed, 0xdb};
            pChar3->writeValue((uint8_t*)v9, sizeof(v9), false);

        }
    }

    connected = true;
    return true;
}

void Victron::unregisterAll() {
    
    if (pChar1) {
        pChar1->unsubscribe();
        unregisterCharacteristics(pChar1, this);
        pChar1 = nullptr;
    }
    if (pChar2) {
        pChar2->unsubscribe();
        unregisterCharacteristics(pChar2, this);
        pChar2 = nullptr;
    }
    if (pChar3) {
        pChar3->unsubscribe();
        unregisterCharacteristics(pChar3, this);
        pChar3 = nullptr;
    }
}

BLEUUID Victron::getServiceUUID() const {
    return generalServiceUUID;
}

void Victron::keepAlive() {
    if (isConnected() && isEnabled()) {
          const uint8_t v[] = {0xf9, 0x41};
          pChar1->writeValue((uint8_t*)v, sizeof(v), false);

          const uint8_t v2[] = {0x06, 0x00, 0x82, 0x18, 0x93, 0x42, 0x10, 0x27};
          pChar2->writeValue((uint8_t*)v2, sizeof(v2), false);

        const uint8_t v3[] = {0x05, 0x03, 0x81, 0x19, 0x02, 0x01};
        //pChar3->writeValue((uint8_t*)v3, sizeof(v3), false);
        const uint8_t v4[] = {0x05, 0x03, 0x81, 0x19, 0xed, 0xdb};
        //pChar2->writeValue((uint8_t*)v4, sizeof(v4), false);

          //printf("%s - Victron Send Keepalive\n", getName());
    }
}

void Victron::onNotify(uint8_t index, uint8_t *pData, size_t length) {
    //printf("%s - Got Notify %d - %d (%d)\n", getName(), index, length, lenNotifyBuffer);
    if ((index == 2) || (index == 3)) {
        if (lenNotifyBuffer + length < NotifyBufferSize) {
            memcpy(pNotifyBuffer + lenNotifyBuffer, pData, length);
            lenNotifyBuffer += length;
        }
        while(lenNotifyBuffer >= 3) {
            //printf("%02x %02x %02x\n", pNotifyBuffer[0], pNotifyBuffer[1], pNotifyBuffer[2]);
            if ((pNotifyBuffer[0] == 0x08) && (pNotifyBuffer[2] == 0x18)) {//cmd?
                if (lenNotifyBuffer >= 5) {
                    uint8_t cmd = pNotifyBuffer[3];
                    int32_t value;
                    uint8_t valLength; 
                    if (getCBOR(4, valLength, value)) {
                        //printf("Cmd %04x %d\n", cmd, value);
                        //printf("Consume %d\n", 5 + valLength);
                        lenNotifyBuffer -= (5 + valLength);
                        memmove(pNotifyBuffer, pNotifyBuffer + 5 + valLength, lenNotifyBuffer);
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            else if ((pNotifyBuffer[0] == 0x08) && (pNotifyBuffer[2] == 0x19)) {//data?
                if (lenNotifyBuffer >= 6) {
                    uint16_t vreg = pNotifyBuffer[3] << 8 | pNotifyBuffer[4];
                    int32_t value;
                    uint8_t valLength; 
                    if (getCBOR(5, valLength, value)) {
                        //printf("Notify %04x %d\n", vreg, value);
                        lastUpdateTime = Clock.Get();
                        switch(vreg) {
                            case 0x0200:
                            case 0x0202:
                                break;

                            case 0x0201:
                                state = value;
                                break;

                            case 0xed8d:
                                voltage[1] = value;
                                break;
                            case 0xed8e:
                                power[1] = value;
                                break;
                            case 0xed8f:
                                current[1] = value;
                                break;

                            case 0xeda9: //load out voltage
                                break;

                            case 0xedbb:
                                voltage[0] = value;
                                break;
                            case 0xedbc:
                                power[0] = value;
                                break;
                            case 0xedbd:
                                current[0] = value;
                                break;

                            case 0xeddb:
                                temperature = value;
                                break;

                            case 0x0150:
                                break;

                            default:
                                logger.printf(LOG_VICTRON, LOG_INFO, "Notify %04x %d\n", vreg, value);
                        }
                        //printf("Consume %d\n", 6 + valLength);
                        if (6 + valLength > lenNotifyBuffer) {
                            logger.printf(LOG_VICTRON, LOG_WARNING, "Consume too much %d> %d\n", 6 + valLength, lenNotifyBuffer);
                        }
                        lenNotifyBuffer -= (6 + valLength);
                        memmove(pNotifyBuffer, pNotifyBuffer + 6 + valLength, lenNotifyBuffer);
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            } else {
                logger.printf(LOG_VICTRON, LOG_VERBOSE, "Discard 1 - %02x\n", pNotifyBuffer[0]);
                memmove(pNotifyBuffer, pNotifyBuffer + 1, --lenNotifyBuffer);
            }
        }
    }
}

bool Victron::getCBOR(uint8_t startPos, uint8_t &valLength_, int32_t &value) const {
    uint8_t valType = pNotifyBuffer[startPos] >> 5;
    uint8_t valLength = pNotifyBuffer[startPos] & 0x1f;
    //printf("CBOR1 - %d %d %d %d\n", startPos, pNotifyBuffer[startPos], valType, valLength);

    bool gotLength = false;
    uint8_t extraLength = 0;
    switch(valLength) {
        case 24:
            if (lenNotifyBuffer > startPos + 1) {
                gotLength = true;
                valLength = pNotifyBuffer[startPos + 1];
                extraLength = 1;
            }
            break;
        case 25:
            if (lenNotifyBuffer > startPos + 2) {
                gotLength = true;
                valLength = pNotifyBuffer[startPos + 1] << 8 | pNotifyBuffer[startPos + 2];
                extraLength = 2;
            }
            break;
        case 26:
            if (lenNotifyBuffer > startPos + 3) {
                gotLength = true;
                valLength = pNotifyBuffer[startPos + 1] << 16 | pNotifyBuffer[startPos + 2] << 8 | pNotifyBuffer[startPos + 3];
                extraLength = 3;
            }
            break;
        case 27:
            if (lenNotifyBuffer > startPos + 4) {
                gotLength = true;
                valLength = pNotifyBuffer[startPos + 1] << 24 | pNotifyBuffer[startPos + 2] << 16 | pNotifyBuffer[startPos + 3] << 8 | pNotifyBuffer[startPos + 4];
                extraLength = 4;
            }
            break;
        case 28:
        case 29:
        case 30:
        case 31:
            logger.printf(LOG_VICTRON, LOG_ERROR, "CBOR error (len=%d)\n", valLength);
            value = 0;
            valLength_ = 0;
            return true; //to get it to skip the data
        default:
            gotLength = true;
            break;
    }

    //printf("CBOR2 - %d %d\n", valType, valLength);

    if (lenNotifyBuffer >= startPos + 1 + valLength + extraLength) {
        int32_t val = 0;
        if (valLength <= 4) {
            uint8_t shift = 0;
            for (int i = 0; i < valLength; i++) {
                val |= pNotifyBuffer[startPos + 1 + i] << shift;
                shift += 8;
            }
        }
        valLength_ = valLength + extraLength;
        value = val;
        //printf("CBOR3 - %d %d\n", valLength_, value);
        return true;
    }
    return false;
}

int16_t Victron::getVoltage(int index) const {
    if ((index < 0) || (index >= 2)) {
        return 0;
    } else {
        return voltage[index];
    }
}

int16_t Victron::getCurrent(int index) const {
    if ((index < 0) || (index >= 2)) {
        return 0;
    } else {
        return current[index];
    }
}

uint32_t Victron::getPower(int index) const {
    if ((index < 0) || (index >= 2)) {
        return 0;
    } else {
        return power[index];
    }
}

void Victron::setDemoData(int16_t v1, int16_t v2, int16_t i1, int16_t i2, uint32_t p1, uint32_t p2, int16_t t, uint8_t s) {
    voltage[0] = v1;
    voltage[1] = v2;
    current[0] = i1;
    current[1] = i2;
    power[0] = p1;
    power[1] = p2;
    temperature = t;
    state = s;
}


#if 0

VictronClientCallback::VictronClientCallback(Victron *p): pVictron(p) {
}

void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    //printf("Notify callback for characteristic %s of data length %d\n",
    //    pBLERemoteCharacteristic->getUUID().toString().c_str(), length);
    //printf("data: ");
    //for (int i = 0; i < length; i++) {
    //  printf("%02x ", pData[i]);
    //}
    //printf("\n");

    auto it = characteristics.find(pBLERemoteCharacteristic);
    if ((it != characteristics.end()) && (it->second.pVictron)) {
        it->second.pVictron->onNotify(it->second.index, pData, length);
    }
}
/*
    if ((pBLERemoteCharacteristic->getUUID().equals(charUUID2)) || (pBLERemoteCharacteristic->getUUID().equals(charUUID3))) {
        if (len + length < NotifyBufferSize) {
            memcpy(pBuffer + len, pData, length);
            len += length;
        }
        while (len >= 6) {
            if ((pBuffer[0] == 0x08) && (pBuffer[2] == 0x19)) {
                uint8_t valLength = pBuffer[5] & 0x0f;

                if (len >= 6 + valLength) {
                    uint16_t vreg = pBuffer[3] << 8 | pBuffer[4];
                    uint8_t valType = pBuffer[5] & 0xf0;
                    if (valType == 0x40) {
                        uint32_t val = 0;
                        for (int i = 0; i < valLength; i++) {
                            val = val << 8 | pBuffer[6 + i];
                        }
                        printf("Notify %04x %d\n", vreg, val);
                    } else {
                        printf("Notify %04x - type %02x\n", vreg, valType);
                    }
                    printf("Consume %d\n", 6 + valLength);
                    len -= (6 + valLength);
                    memmove(pBuffer, pBuffer + 6 + valLength, len);
                }

            } else {
                printf("Discard 1\n");
                memmove(pBuffer, pBuffer + 1, --len);
            }
        }
    }
}*/

#endif
