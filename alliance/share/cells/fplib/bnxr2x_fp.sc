#cell1 bnxr2x_fp CMOS schematic 12288 v7r5.6
# 8-Sep-93 11:17 8-Sep-93 11:17 dea9243 * .icn nChannelTransistor .sc
# nChannelTransistor .icn pChannelTransistor .sc pChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 7 "VDD" "S" "I" "VSS" "VSS1" "BULK" ""; $C 5; C 3 1 1; C 2 1 2;
C 1 1 3; C 4 1 4; C 5 1 5; $J 4; J 1 "u2" 3 3 1 7 1 1 3 2 1 1 2 1
0 "12" 2 0 "1"; J 2 "u5" 3 3 1 2 2 1 4 1 1 7 2 1 0 "27" 2 0 "1"; J 1
"u3" 3 3 1 2 2 1 1 1 1 7 2 1 0 "54" 2 0 "1"; J 2 "u4" 3 1 1 3 3 1 7 2
1 4 2 1 0 "6" 2 0 "1"; $I 4; I 1 "u2" "@" 410 630 0 22 2 1 0 "12" 2
0 "1"; I 2 "u5" "@" 530 470 0 22 2 1 0 "27" 2 0 "1"; I 1 "u3" "@"
530 630 0 22 2 1 0 "54" 2 0 "1"; I 2 "u4" "@" 410 470 0 22 2 1 0 "6"
2 0 "1"; $E 22; E 20000002 440 540 0; E 20200002 440 680 + 440 685
"vdd" 1 LB H 0 + 440 665 "vdd" 1 LB H 0 3 0; E 20400002 560 490 1 2 3
; E 20000002 600 450 0; E 20200002 600 540 + 600 545 "s" 1 LB H 0 +
600 525 "s" 1 LB H 0 2 0; E 20400002 440 610 1 1 3; E 20400002 410
630 1 1 1; E 20200002 360 540 + 360 545 "i" 1 LB H 0 + 360 525 "i" 1
LB H 0 1 0; E 20000002 560 540 0; E 20400002 440 650 1 1 2; E
20200002 440 390 + 440 395 "vss" 1 LB H 0 + 440 375 "vss" 1 LB H 0 4 0
; E 20200002 600 390 + 600 395 "vss1" 1 LB H 0 + 600 375 "vss1" 1 LB H
0 5 0; E 20400002 560 610 1 3 3; E 20400002 410 470 1 4 1; E
20400002 440 490 1 4 3; E 20400002 440 450 1 4 2; E 20400002 560 450
1 2 2; E 20400002 560 650 1 3 2; E 20400002 530 470 1 2 1; E
20000002 530 540 0; E 20400002 530 630 1 3 1; E 20000002 410 540 0;
$S 16; S 12 4 2; S 9 13 2; S 9 5 2; S 1 6 2; S 10 2 2; S 1 20 2
; S 20 21 2; S 19 20 2; S 15 1 2; S 11 16 2; S 16 17 2; S 3 9 2;
S 10 18 2; S 8 22 2; S 14 22 2; S 22 7 2; $Z;
