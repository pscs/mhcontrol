#include "victron_mainscharger.h"

#include "config.h"

/*
During initialisation app sends read requests 05 00 81 19 for:
0109 - short-resp 01
010a - serial number - resp 4b4851323035303548315239 (KHQ20505H1R9)
ec0f - short-resp 01
ec0e - short-resp 01
0140 - resp 44fc00c140 (capabilities - 0100 0000 1100 0001 0000 0000 1111 1100 - history,batterysafe,adaptive,manual equ,auto equ,storage,parallel charging,
0143 - resp 4403000000
ed8d - resp 422b05 (v1 = 13.23V)
ed8f - resp 420000 (i1 = 0A)
0201 - resp 4106 (state = 6 = storage?)
edda - resp 4100 (error code = 0)
e000 - short-resp 01
0207 - resp 44000000 (device off reason = no input power)
0205 - short-resp 01
ee14 - resp 420005
ee15 - resp 42be05
034e - short-resp 01 (relay control)
edde - resp 4101 (number of outputs = 1)
2211 - resp 420005 (adjustable voltage minimum = 0.05V)
2212 - resp 42d606 (adjustable voltage maximum = 17.50V)
edfd - resp 422c01 (battery auto equalisation mode = on)
edc8 - resp 424b00
edcb - resp 429600
0211 - short-resp 01 (AC In Current Lim Min)
0212 - short-resp 01 (AC In Current Lim Max)
0234 - short-resp 01 ()
edf1 - resp 41ff (batt type = user defined)
e001 - resp 4100
edf7 - resp 429605 (batt absorption voltage level = 14.30V)
edf5 - resp 422805 (batt storage voltage level = 13.20V)
edfc - resp 426009 (batt bulk time limit = 24 hrs)
edfa - resp 422003 (batt float time limit = 8 hrs)
edfb - resp 42c800 (batt absorption time limit = 2hrs)
edf8 - resp 42bc02 (batt repeated absorption time interval = 7hrs)
edf9 - resp 426400 (batt repeated absorption time duration = 1hr)
edf2 - resp 420000 (batt temp compensation = 0V/deg)
ede2 - resp 421e05
ed2e - resp 420a00 (rebulk voltage offset = 0.10V)
ede1 - resp 421400 
ede3 - resp 42c800 (equalisation duration = 0.2hrs)
ede4 - resp 4108 (equalisation current = 8% of max current)
ede5 - resp 4100 (auto equalise stop on voltage = no)
edca - resp 420000 (voltage compensation = 0V)
edf0 - resp 422c01 (batt max current = 30A)
edf6 - resp 424605 (batt float voltage level = 13.50V)
edf4 - resp 420000 (batt equalisation voltage level = 0 (off?))
edfd - resp 4100 (batt auto equalisation mode = off)
edfe - resp 4101 (batt adaptive mode = on)
edff - resp 4100 (batt safe mode = off)
ede7 - resp 420000 (tail current = 0)
edef - resp 410c (batt voltage selection = 12V)
edea - short-resp 01 (battery voltage setting)
ede6 - resp 420000 (low temperature charge current = 0)

ede0 - resp 42f401 (battery low temperature level = 5.00C)
edce - short-resp 01 (voltage settings range)
ee17 - resp 4101
ede9 - resp 420005 (battery power supply voltage = 0.05V?)
1030 - short-resp 01 (clear history command)
106f - resp 4104
1099 - resp 44da9a
(below are history records?)
1070 - resp 5834007764190003000000d906000000000000c23600000f1c0300000000000800000000000000120000000200000096059805fff100
1071 - resp 583400da9a140005000000d806000000000000c2360000fe8b0400000000000500000000000000170000000200000099059405fff100
1072 - resp 583400b7380f0003000000080700000000000040380000d82205000000000007000000000000000f0000001000000096059905fff100
1073 - resp 583400944000001b000000080700000000000040380000c0b80e00020000000c00000000000000170000006d00000042059505fff100
1074 - resp 583400b7010000030000000807000000000000d23700000000000000000000040000000000000019000000000000008c059505fff100
1075 - short-resp 02

1076 - short-resp 02
1077 - short-resp 02
1078 - short-resp 02
1079 - short-resp 02
107a - short-resp 02
107b - short-resp 02
107c - short-resp 02
107d - short-resp 02
107e - short-resp 02
107f - short-resp 02
1080 - short-resp 02
1081 - short-resp 02
1082 - short-resp 02

1083 - short-resp 02
1084 - short-resp 02
1085 - short-resp 02
1086 - short-resp 02
1087 - short-resp 02
1088 - short-resp 02
1089 - short-resp 02
108a - short-resp 02
108b - short-resp 02
108c - short-resp 02
108d - short-resp 02
108e - short-resp 02
108f - short-resp 02

1090 - short-resp 02
1091 - short-resp 02
1092 - short-resp 02
1093 - short-resp 02
1094 - short-resp 02
1095 - short-resp 02
1096 - short-resp 02
1097 - short-resp 02
1098 - short-resp 02
1043 - resp 58190025be1c000f01000005000000050000000900000000000000
0206 - resp 4100
0200 - short-resp 01 (device mode)
ede8 - short-resp 01 (battery BMS present)

0213 - short-resp 01 (AC IN1 current limit internal)
0150 - resp 4405000000
(0500811890) - resp (080018904101) - (resp 90 = 01?)
(0500811891) - resp (08001891420000) - (resp 91 = 0000?)
(05008104) - resp (09000403)
0352 - short-resp 01 (high voltage relay set)
0353 - short-resp 01 (high voltage relay clear)
0350 - short-resp 01 (low voltage relay set)
0351 - short-resp 01 (low voltage relay clear)
100a - short-resp 01 (relay minimal enabled)
034f - short-resp 01 (relay mode)
100b - short-resp 01 (relay disable delay)
edec - resp 42ffff (batt temperature)
ee16 - resp 42ffff

ed6f - short-resp 01 (channel 3 current - for 3 output mains charger?)
ed6d - short-resp 01 (channel 3 voltage)
ed7f - short-resp 01 (channel 2 current)
ed7d - short-resp 01 (channel 2 voltage)
ec3f - resp 4101
ec12 - resp 42ffff

ec13 - resp 50ffffffffffffffffffffffffffffffff

ec14 - resp 40
ec15 - resp 4100
ec16 - resp 4100
ec30 - resp 4100
*/

VictronMainsCharger victronMainsCharger(VICTRON_MAINS_ADDRESS, VICTRON_MAINS_PASSCODE);

VictronMainsCharger::VictronMainsCharger(const char *addr, uint32_t passcode): Victron(addr, passcode) {
}

const char *VictronMainsCharger::getName() const {
    return "Mainscharger";
}

void VictronMainsCharger::askForInitialValues() {
    //ask for status, v1, i1, temp
    const uint8_t v[] = {0x05, 0x00, 0x81, 0x19, 0xed, 0x8d, //v1
        0x05, 0x00, 0x81, 0x19, 0xed, 0x8f, //i1
        0x05, 0x00, 0x81, 0x19, 0x02, 0x01, //status
        0x05, 0x00, 0x81, 0x19, 0xed, 0xdb}; //temp
    pChar3->writeValue((uint8_t*)v, sizeof(v), false);
}
