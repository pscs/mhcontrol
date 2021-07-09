#include "ksenergy_battery.h"

#include <Arduino.h>

#include "config.h"
#include "logger.h"
#include "bigendian.h"
#include "DS3231.h"

static const BLEUUID generalServiceUUID("fff0");
static const BLEUUID charUUID1("0000fff6-0000-1000-8000-00805f9b34fb");

const int NotifyBufferSize = 1000;

struct BatInfoStruct {
    uint8_t rs485addr;
    uint8_t cmd;
    uint8_t version;
    be_uint16_t dataLength;
    uint8_t date[7];
    be_uint16_t cellVolts[16];
    be_uint16_t status;
    be_uint16_t balanceState;
    uint8_t temperature[4];
    be_int16_t coulombmeterTemp;
    be_int32_t current;
    be_int32_t meanCurrent;
    be_uint16_t totalVoltage;
    be_uint16_t diffPressure;
    uint8_t batteriesInSeries;
    be_uint16_t cycles;
    uint8_t soc;
    be_uint32_t designCapacity;
    be_uint32_t remainingCapacity;
    be_uint32_t fullCapacity;
    be_uint16_t stateOfHealth;
    uint8_t socError;
    be_uint16_t flags;
    uint8_t learnedStatus;
    be_uint16_t serialNumber;
    be_uint32_t mfrDate;
    uint8_t mfrName[10];
    be_uint32_t systemRetention[3];
    uint8_t crc;
} __attribute__ ((packed));

static_assert(sizeof(BatInfoStruct) == 117, "Battery Info Structure size is not correct");

KSEnergyBattery ksEnergyBattery(KSENERGY_BATTERY_ADDRESS);

KSEnergyBattery::KSEnergyBattery(const char *address): MyBLEClient(address, false, 0)
{}

KSEnergyBattery::~KSEnergyBattery() {
    unregisterAll();
}

const char *KSEnergyBattery::getName() const {
    return "Battery";
}

bool KSEnergyBattery::connectIfNecessary() {
 
   if (!pNotifyBuffer) {
        pNotifyBuffer = reinterpret_cast<uint8_t *>(ps_malloc(NotifyBufferSize));
    }
    logger.send(LOG_BATTERY, LOG_DEBUG, "Battery - try connect\n");
    if (isConnected()) {
        logger.send(LOG_BATTERY, LOG_DEBUG, "Battery - already connected\n");
        return true;
    } else if (!isEnabled()) {
        logger.send(LOG_BATTERY, LOG_DEBUG, "Battery - disabled\n");
        return false;
    }

    logger.send(LOG_BATTERY, LOG_DEBUG, "Battery - start connect\n");

    if (pClient->connect(address)) {//, false)) {
        logger.printf(LOG_BATTERY, LOG_DEBUG, "%s - Connected to Server\r\n", getName());
    } else {
        logger.printf(LOG_BATTERY, LOG_DEBUG, "%s - Failed to connect to server\r\n", getName());
        return false;
    }
    pCharacteristic = nullptr;
    auto svcs = pClient->getServices(true);
    logger.send(LOG_BATTERY, LOG_DEBUG, "got svcs\n");
    for (const auto svc: *svcs) {
        printf(" - svc %s\n", svc->getUUID().toString().c_str());
        if (svc->getUUID().toString() == "0xfff0") {
            printf("get char\n");
            auto chars = svc->getCharacteristics(true);
            for (const auto chr: *chars) {
                printf(" - char %s\n", chr->getUUID().toString().c_str());
                if (chr->getUUID().toString() == "0xfff6") {
                    printf("got char");
                    pCharacteristic = chr;
                    break;
                }
            }
            if (pCharacteristic) {
                break;
            }
        }
    }

    if (!pCharacteristic) {
        printf(" - Battery failed to find characteristic\r\n");
        pClient->disconnect();
        return false;
    } else {
        printf(" - Battery found characteristic\r\n");
        if(pCharacteristic->canNotify()) {
            printf(" - Battery register for notifications\r\n");
            registerCharacteristics(pCharacteristic, this, 1);
            //characteristics[pChar1] = NotifyInfo(this, 1);
            pCharacteristic->subscribe(true, &notifyCallback);
            //pCharacteristic->registerForNotify(&notifyCallback);
        }
    }

    connected = true;
    return true;
}

void KSEnergyBattery::unregisterAll() {

    pCharacteristic->unsubscribe();
    unregisterCharacteristics(pCharacteristic, this);
    pCharacteristic = nullptr;
}

void KSEnergyBattery::keepAlive() {
    if (isConnected() && isEnabled()) {
        printf("battery keepalive\n");
        const char *cmd = ":015150000EFE";
        pCharacteristic->writeValue(cmd, 13);
    }
    /* else {
   if (!pNotifyBuffer) {
        pNotifyBuffer = reinterpret_cast<uint8_t *>(ps_malloc(NotifyBufferSize));
    }

        strcpy((char *)pNotifyBuffer, ":01540100EC000102030405060D030D060D040CFC000000000000000000000000000000000000000000000000F00000003636363610D100000000000000003409000A0400044E0001D4C0000222800001D4C0000105F00006000100000000000000000000000000000000000000000000000000005C~");
        lenNotifyBuffer = strlen((char *)pNotifyBuffer);
        processData();
    }*/
}

void KSEnergyBattery::onNotify(uint8_t index, uint8_t *pData, size_t length) {
    //printf("Battery OnNotify\n");
    for (int i = 0; i < length; i++) {
        if (lenNotifyBuffer >= NotifyBufferSize) {
            lenNotifyBuffer = 0;
            logger.send(LOG_BATTERY, LOG_ERROR, "Battery notify Buffer overflow\n");
        }
        if (pData[i]) {
            //printf("%c", pData[i]);
            pNotifyBuffer[lenNotifyBuffer++] = pData[i];
            if (pData[i] == '~') {
                //printf("\nProcess Data\n");
                processData();
                lenNotifyBuffer = 0;
            }
        }
    }
    //printf("\n");
}

void KSEnergyBattery::processData() {
    uint8_t hexBuf[200];
    if (pNotifyBuffer[0] == ':') {
        int bufLen = 0;
        for (int i = 1; i < lenNotifyBuffer - 2; i += 2) {
            uint8_t by = charToHex(pNotifyBuffer[i]) << 4 | charToHex(pNotifyBuffer[i + 1]);
            hexBuf[bufLen++] = by;
        }
        if (bufLen < 6) {
            return;
        }
        uint8_t crc;
        for (int i = 1 ; i < bufLen - 1; i++) {
            crc += hexBuf[i];
        };
        crc ^= 0xff;
        if (crc != hexBuf[bufLen - 1]) {
            logger.printf(LOG_BLE, LOG_ERROR, "Battery CRC error %02x <> %02x\n", crc, hexBuf[bufLen - 1]);
            return;
        }

        if ((hexBuf[0] == 0x01) && (hexBuf[1] == 0x54) && (hexBuf[2] == 0x01)) {
            int16_t len = hexBuf[3] << 8 | hexBuf[4];
            if (len != lenNotifyBuffer) {
                logger.printf(LOG_BLE, LOG_ERROR, "Battery LEN error %02x <> %02x\n", len, lenNotifyBuffer);
            }
            else {
                BatInfoStruct *info = reinterpret_cast<BatInfoStruct *>(hexBuf);

                voltage = 0;
                for (int i = 0; i < 16; i++) {
                    voltage += info->cellVolts[i];
                }
                current = info->current;
                temperature = info->temperature[0];
                cycles = info->cycles;
                state = info->status;
                SOC = info->soc;
                SOCError = info->socError;
                SOH = info->stateOfHealth;
                learnState = info->learnedStatus;
                designCapacity = info->designCapacity;
                remainingCapacity = info->remainingCapacity;
                fullCapacity = info->fullCapacity;
                lastUpdateTime = Clock.Get();


                logger.printf(LOG_BLE, LOG_INFO, "Battery data checks passed\n");
            }
        } else {
            logger.printf(LOG_BLE, LOG_WARNING, "Battery unknown data block %02x %02x %02x\n", hexBuf[0], hexBuf[1], hexBuf[2]);
        }

    }
}

uint8_t KSEnergyBattery::charToHex(char ch) {
    uint8_t r = ch - '0';
    if (r > 9) {
        r = (r - 'A') + '9' + 1;
        if (r > 15) {
            r -= 32; //lower case
        }
    }
    return r;
}

std::string KSEnergyBattery::getStateText() const {
    switch(state) {
        case 0xf000:
            return "Standby";
        case 0xf001:
            return "Charging";
        case 0xf002:
            return "Discharging";
        case 0x0e80:
            return "Discharging (low temp)";
        case 0xe400:
            return "Discharging (high temp)";
        case 0xd200:
            return "Charging (low temp)";
        case 0xd100:
            return "Charging (high temp)";
        case 0xe080:
            return "Discharge overcurrent 1";
        case 0xe040:
            return "Discharge overcurrent 2";
        case 0xc020:
            return "Short-circuit protection";
        case 0xd010:
            return "Charge overcurrent";
        case 0xe008:
            return "Over-discharge protection";
        case 0xd004:
            return "Over-voltage protection";
        case 0xca00:
            return "Charge/discharge low temp protection";
        case 0xc500:
            return "Charge/discharge high temp protection";
        default:
            {
                char buf[20];
                snprintf(buf, sizeof(buf), "Unknown: %04x", state);
                return buf;
            }
    }
}

void KSEnergyBattery::setDemoData(int16_t v, int32_t i, uint16_t s, uint8_t soc) {
    voltage = v;
    current = i;
    state = s;
    SOC = soc;
}