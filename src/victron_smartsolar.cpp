#include "victron_smartsolar.h"

#include "config.h"

/*
During initialisation app sends read requests 05 00 81 19 for:
0109 - resp 48c2450c4e2438e600
010a - short-resp 01 (serial no)
ec0f - short-resp 01
ec0e - short-resp 01
010c - short-resp 01 (installation description 1)
(050081189005) - (0900189001)
ec3f - resp 4101
ec12 - resp 42ffff

(01) 0100 - resp 440000fefe (product ID)
(01) 0100 - resp 440000fefe
(01) 0100 - resp 440000fefe
(01) 0109 - short-resp 01
(01) 010a - short-resp 01
(03) 0109 - resp 48c2450c4e2438e600
(03) 010a - resp 4b4851323130345043395455
(03) 0102 - resp 4400560100 (firmware version)
(03) ec0f - short-resp 01
(03 from now on)
ec0e - short-resp 01
0110 - resp 44fffffffe (UDF version)
edbc - resp 4400000000 (panel power = 0W)
(050381181805) - resp (0903181801)
0140 - resp 4475de0d0a (capabilities - 0000 1010 0000 1101 1101 1110 0111 0101 - load output,history,adaptive,manual equ,auto equ,solar timing,alt VE.direct.tx,
                    user def load,load in text,BMS supp,ext ctrl supp,parallel charge,alt ve.direct.rx,virt load output,load auto selector,paygo supp)
0201 - resp 4105 (state = 5 = Float)
edda - resp 4100 (error code = 0)
edbb - resp 42bb06 (panel voltage = 17.23V)
eccb - short-resp 01
ecdb - short-resp 01
eceb - short-resp 01
ecfb - short-resp 01
edbd - short-resp 01 (panel current)
eccd - short-resp 01
ecdd - short-resp 01
eced - short-resp 01
ecfd - short-resp 01

eccc - short-resp 01
ecdc - short-resp 01
ecec - short-resp 01
ecfc - short-resp 01
ed8d - resp 425305 (v1 = 13.63V)
ed8f - resp 420000 (i1 = 0A)
edec - resp 42ffff (batt temp)
eda8 - resp 4101 (load output status = on)
edad - resp 420100 (load output current  - 0.1A)
eda9 - resp 425205 (load output voltage = 13.62)
edac - resp 4100 (load output offset voltage = 0)
2211 - resp 422003
(ed8d - 425205 - not requested) (v1 = 13.62V)
2212 - resp 42cc06
(edbc - 440d000000 - not requested) (input power = 0.13W)
edb8 - resp 424c1d (panel max voltage = 75V)
034e - short-resp 01 (relay control)
(eda9 - 425205 - not requested) (load output voltage = 13.62V)
2027 - resp 44ffffffff
(edbc - 4400000000 - not requested)
(ed8d - 425305 - not requested)
0207 - resp 4400000000
0205 - short-resp 01
0244 - short-resp 01
0120 - resp 44e4820000 (uptime)
0200 - resp 4101 (device mode =  charger only)
0202 - resp 4402000000 (remote control used = on/off control)
ede8 - resp 4100 (battery BMS present = absent)
(05038104) (resp 09030403)
edce - resp 420c18
eddf - resp 429600 (charger max current = 15A)
edef - resp 410c (battery voltage selection = 12V)

edf1 - resp 4101 (battery type = 01)
edf0 - resp 429600 (battery max current = 15A)
edf7 - resp 42a005 (battery absorption voltage level = 14.40V)
edf6 - resp 426405 (battery storage voltage level = 13.80V)
edf4 - resp 425406 (battery equalisation voltage level = 16.20V)
edfb - resp 425802 (battery absorption time limit = 6 hrs)
edfd - resp 4100 (battery auto equalisation mode = off)
edf2 - resp 42acf9 (battery temperature compensation setting = -16.20mV/C)
ede6 - resp 42ffff (low temp charge current)
ede0 - resp 42f401 (batt low temp level = 5C)
ede2 - short-resp 01
ed2e - resp 422800 (rebulk voltage offset = 0.40V)
ede3 - resp 426400 (equ duration = 1hr)
ede4 - resp 4108 (equ current level =  8%)
ede5 - resp 4101 (auto equ stop on voltage = yes)
edfe - resp 4101 (battery adaptive mode = on)
ede7 - resp 420a00 (tail current = 1)
edca - short-resp 01 (voltage compensation)
d0c0 - short-resp 01
ed2f - short-resp 01
edab - resp 4101 (load output control mode = automatic)
ed9c - resp 42e803 (load switch low level = 10V)
ed9d - resp 427805 (load switch high level = 14V)
ed90 - resp 421e00 (load AES timer = 30min)
ed9e - resp 4100 (VE.Direct TX port operation mode)
ed98 - resp 4101 (VE.Direct RX port operation mode)

edd9 - short-resp 01 (charger relay mode)
100a - short-resp 01 (relay minimal enabled)
0350 - short-resp 01 (low voltage relay set)
0351 - short-resp 01 (low voltage relay clear)
0352 - short-resp 01 (high voltage relay set)
0353 - short-resp 01 (high voltage relay clear)
edba - short-resp 01 (input voltage maxi,um set)
edb9 - short-resp 01 (input voltage maximum clear)
eda0 - resp 4400000000 (timer event 0)
eda1 - resp 4400000000 (timer event 1)
eda2 - resp 4400000000 (timer event 2)
eda3 - resp 4400000000 (timer event 3)
eda4 - resp 4400000000 (timer event 4)
eda5 - resp 4400000000 (timer event 5)
ed9a - resp 420000 (panel voltage night = 0V)
ed96 - resp 420000 (sunset delay = 0min)
ed99 - resp 420000 (panel voltage day = 0V)
ed97 - resp 420000 (sunrise delay = 0min)
ed9b - resp 4100 (gradual dim speed = 0s)
eda7 - resp 420000 (mid-point shift = 0min)
2031 - resp 42ffff (time of day)
0400 - short-resp 01 (display backlight mode)
0404 - short-resp 01 (display temperature unit)
2004 - short-resp 01 (remote command)
1030 - short-resp 03 (clear history)
eddc (charger user yield)

eddd - resp 4400000000  (charger system yield = 0kWh)
(below are history)
104f - resp 58220100000000000000000000000000d2069f05020000ffffffffffffffffffffffffff
1050 - resp 58220000000000010000009f0568000000000000000001002d02000000000000bd060200
1051 - resp 58220000000000000000009f0500000000000000000003000001070000000a00bd060100
1052 - resp 58220000000000000000009f0500000000000000000003000201020000000300d2060000
1053 - short-resp 02
1054 - short-resp 02
1055 - short-resp 02
1056 - short-resp 02
1057 - short-resp 02
1058 - short-resp 02
1059 - short-resp 02
105a - short-resp 02
105b - short-resp 02
105c - short-resp 02
105d - short-resp 02
105e - short-resp 02
105f - short-resp 02
1060 - short-resp 02
1061 - short-resp 02
1062 - short-resp 02
1063 - short-resp 02
1064 - short-resp 02
1065 - short-resp 02
1066 - short-resp 02
1067 - short-resp 02
1068 - short-resp 02
1069 - short-resp 02
106a - short-resp 02
106b - short-resp 02
106c - short-resp 02
106d - short-resp 02
106e - short-resp 02
10a0 - short-resp 01
10a1 - short-resp 01
10a2 - short-resp 01
10a3 - short-resp 01
10a4 - short-resp 01
10a5 - short-resp 01
10a6 - short-resp 01
10a7 - short-resp 01
10a8 - short-resp 01
10a9 - short-resp 01
10aa - short-resp 01
10ab - short-resp 01
10ac - short-resp 01
10ad - short-resp 01
10ae - short-resp 01
10af - short-resp 01
10b0 - short-resp 01
10b1 - short-resp 01
10b2 - short-resp 01

10b3 - short-resp 01
10b4 - short-resp 01
10b5 - short-resp 01
10b6 - short-resp 01
10b7 - short-resp 01
10b8 - short-resp 01
10b9 - short-resp 01
10ba - short-resp 01
10bb - short-resp 01
10bc - short-resp 01
10bd - short-resp 01
10be - short-resp 01
(0150 - 4401000000 - not requested?)
(0503811890) (resp 080318904101)
(0503811891) (resp 0903189101)
edd4 - resp 4100 - (charger additional state information) 
ec3f - short-resp 01
ec12
(00) ec13 - resp 50ffffffffffffffffffffffffffffffffffff
(00) ec14 - short-resp 01
(00) ec15 - short-resp 01
(00) ec16 - resp 4100
(00) ec30 - resp 4101
(01) 0102 - short-resp 01
(01) 0142 - resp 4401000000
(0501811890) (resp 0901189001)
(01) ec3f - short-resp 01

(01 from now on)
ec12 - short-resp 01
0102 - short-resp 01
ec0f - short-resp 01
ec0e - short-resp 01
010c - resp 4c6e524635782d554152543000
0110 - short-resp 01
(060382192031426b05) (resp 090319ec1201) - 2031 -> 426b05 (time of day 

*/

VictronSmartSolar victronSmartSolar(VICTRON_SOLAR_ADDRESS, VICTRON_SOLAR_PASSCODE);

VictronSmartSolar::VictronSmartSolar(const char *addr, uint32_t passcode): Victron(addr, passcode) {
}

const char *VictronSmartSolar::getName() const {
    return "SmartSolar";
}

void VictronSmartSolar::askForInitialValues() {
    //ask for status, v1, i1, temp, panel power, panel voltage
    const uint8_t v[] = {0x05, 0x03, 0x81, 0x19, 0xed, 0x8d, //v1
        0x05, 0x03, 0x81, 0x19, 0xed, 0x8f, //i1
        0x05, 0x03, 0x81, 0x19, 0x02, 0x01, //status
        0x05, 0x03, 0x81, 0x19, 0xed, 0xbc, //panel power
        0x05, 0x03, 0x81, 0x19, 0xed, 0xbb, //panel voltage
        0x05, 0x03, 0x81, 0x19, 0xed, 0xdb}; //temp
    pChar3->writeValue((uint8_t*)v, sizeof(v), false);
}