#cell1 grpli_c CMOS schematic 10240 v7r5.6
# 20-Mar-93 15:55 20-Mar-93 15:55 dea9221 * .icn pChannelTransistor .sc
# pChannelTransistor .icn nChannelTransistor .sc nChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 5 "I" "F" "VSS" "VDD" "BULK"; $C 4; C 3 1 1; C 2 1 2; C 4 1 3;
C 1 1 4; $J 2; J 1 "u2" 3 1 1 1 2 1 3 3 1 2 2 1 0 "15" 2 0 "1"; J 2
"u3" 3 1 1 1 2 1 4 3 1 2 2 1 0 "30 " 2 0 "1"; $I 2; I 1 "u2" "@" 620
420 0 22 2 1 0 "15" 2 0 "1"; I 2 "u3" "@" 620 530 0 22 2 1 0 "30 " 2
0 "1"; $E 14; E 20400002 620 420 1 1 1; E 20400002 650 400 1 1 2;
E 20400002 650 440 1 1 3; E 20400002 620 530 1 2 1; E 20400002 650
550 1 2 2; E 20400002 650 510 1 2 3; E 20000002 560 530 0; E
20000002 560 420 0; E 20000002 560 480 0; E 20200002 510 480 + 510
485 "i" 1 LB H 0 + 510 465 "" 1 LB H 0 3 0; E 20000002 650 480 0; E
20200002 720 480 + 720 485 "f" 1 LB H 0 + 720 465 "" 1 LB H 0 2 0; E
20200002 650 380 + 650 385 "VSS" 1 LB H 0 + 650 365 "" 1 LB H 0 4 0;
E 20200002 650 590 + 650 595 "VDD" 1 LB H 0 + 650 575 "" 1 LB H 0 1 0
; $S 10; S 7 4 2; S 8 1 2; S 8 9 2; S 9 7 2; S 10 9 2; S 3 11 2
; S 11 6 2; S 11 12 2; S 13 2 2; S 5 14 2; $Z;
