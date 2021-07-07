#include "victron_B2B.h"

#include "config.h"

/*
During initialisation app sends read requests 05 00 81 19 for:
ec3f - resp 4101 
ec12 - short-resp 01
010a - resp 4b485132313130484a5a5034 (serial number)
2211 - resp 42e803 (adjustable voltage minimum = 10.00V)
2212 - resp 42dc05 (adjustable voltage maximum = 15.00V)
ed8d - resp 42ff7f (v1 = -max)
2222 - resp 422003
2223 - resp 42a406
edbb - resp 424205 (input voltage = 13.46V)
0201 - resp 4100 (state = off)
edda - resp 4100 (error code = 0)
0207 - resp 4408000000 (device off reason = 8? Remote control off?)
0205 - short-resp 01 (old device off reason = )

edf1 - resp 41ff (battery type = user def)
edf7 - resp 42a005 (battery absorption voltage level = 14.40V)
edf6 - resp 424605 (battery float voltage level = 13.50V)
edfc - resp 42e803 (battery bulk time limit = 10hrs)
edfe - resp 4100 (battery adaptive mode = off)
edfb - resp 426400 (battery absorption time limit = 1hr)
edef - resp 410c (battery voltage selection = 12V)
ed2e - resp 420a00 (rebulk voltage offset = 0.10V)
ede9 - resp 42b004 (battery power supply voltage = 12.00V)
ee14 - resp 42e803 ()
ee15 - resp 42dc05
0320 - resp 42e204 (low voltage alarm set = 12.50V)
0321 - resp 420005 (low voltage alarm clear = 12.80V)

0206 - resp 4100
ee36 - resp 422003
ee37 - resp 422003
ee38 - resp 422003
ee39 - resp 4400000000
(05008104) (09000402)
0200 - resp 4101 (device mode = charger only)
0150 - short-resp 01
(050811890) (0900189001)
(050811891) (0900189101)


(not requested)
edbb - 424305 (input voltage = 13.47V)
0120 - 44d6f50400 (uptime)
edbb - 424205
0120 - 44d7f50400
edbb - 424105
0120 - 44d8f50400

0120 - 44d9f50400
0120 - 44daf50400
*/

VictronB2B victronB2B(VICTRON_B2B_ADDRESS, VICTRON_B2B_PASSCODE);

VictronB2B::VictronB2B(const char *addr, uint32_t passcode): Victron(addr, passcode) {
}

const char *VictronB2B::getName() const {
    return "B2B";
}

void VictronB2B::askForInitialValues() {
    //ask for status, v1, input voltage, temp
    const uint8_t v[] = {0x05, 0x00, 0x81, 0x19, 0xed, 0x8d, //v1
        0x05, 0x00, 0x81, 0x19, 0xed, 0xbb, //input voltage
        0x05, 0x00, 0x81, 0x19, 0x02, 0x01, //status
        0x05, 0x00, 0x81, 0x19, 0xed, 0xdb}; //temp
    pChar3->writeValue((uint8_t*)v, sizeof(v), false);
}