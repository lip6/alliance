#cell2 * np1_y sc * 2 CMOS 9216 v8r4.4
# "21-Nov-94 GMT" "16:58:31 GMT" "21-Nov-94 GMT" "16:58:31 GMT" scr * .icn nChannelTransistor .sc nChannelTransistor .icn pChannelTransistor .sc pChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 "np1_y"; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 5 "VDD" "F" "VSS" "I" "BULK"; $C 4; C 3 1 1; C 2 1 2; C 4 1 3;
C 1 1 4; $J 2; J 2 "u5" 3 3 1 2 2 1 1 1 1 4 2 1 0 "23" 2 0 "1"; J 1
"u4" 3 3 1 2 1 1 4 2 1 3 2 1 0 "12" 2 0 "1"; $I 2; I 2 "u5" "@" 510
470 0 22 2 1 0 "23" 2 0 "1"; I 1 "u4" "@" 510 390 0 22 2 1 0 "12" 2 0
"1"; $E 12; E 20000002 540 420 0; E 20400002 540 450 1 1 3; E
20200002 540 520 + 540 525 "vdd" 1 LB H 0 + 540 505 "" 1 LB H 0 3 0;
E 20200002 600 420 + 600 425 "f" 1 LB H 0 + 600 405 "" 1 LB H 0 2 0;
E 20200002 540 350 + 540 355 "vss" 1 LB H 0 + 540 335 "" 1 LB H 0 4 0
; E 20400002 540 410 1 2 3; E 20400002 540 490 1 1 2; E 20400002 510
470 1 1 1; E 20400002 510 390 1 2 1; E 20400002 540 370 1 2 2; E
20200002 380 420 + 380 425 "i" 1 LB H 0 + 380 405 "" 1 LB H 0 1 0; E
20000002 510 420 0; $S 8; S 1 2 2; S 7 3 2; S 5 10 2; S 6 1 2; S
1 4 2; S 9 12 2; S 12 8 2; S 11 12 2; $T 1; T + 480 240 "np1_y" 1
LB H 0; $Z;
