#cell1 bno2x_fp CMOS schematic 12288 v7r5.6
# 8-Sep-93 11:16 8-Sep-93 11:16 dea9243 * .icn pChannelTransistor .sc
# pChannelTransistor .icn nChannelTransistor .sc nChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 7 "VDD" "I" "O" "VSS" "VSS1" "BULK" ""; $C 5; C 3 1 1; C 1 1 2;
C 2 1 3; C 4 1 4; C 5 1 5; $J 4; J 2 "u4" 3 2 1 4 3 1 7 1 1 2 2 1
0 "6" 2 0 "1"; J 1 "u2" 3 1 1 2 3 1 7 2 1 1 2 1 0 "12" 2 0 "1"; J 1
"u3" 3 1 1 7 3 1 3 2 1 1 2 1 0 "54" 2 0 "1"; J 2 "u5" 3 2 1 4 1 1 7 3
1 3 2 1 0 "27" 2 0 "1"; $I 4; I 2 "u4" "@" 420 460 0 22 2 1 0 "6" 2
0 "1"; I 1 "u2" "@" 420 630 0 22 2 1 0 "12" 2 0 "1"; I 1 "u3" "@"
530 630 0 22 2 1 0 "54" 2 0 "1"; I 2 "u5" "@" 530 460 0 22 2 1 0 "27"
2 0 "1"; $E 22; E 20000002 450 540 0; E 20400002 420 630 1 2 1; E
20000002 420 540 0; E 20400002 450 440 1 1 2; E 20200002 450 670 +
450 675 "vdd" 1 LB H 0 + 450 655 "vdd" 1 LB H 0 3 0; E 20400002 450
610 1 2 3; E 20400002 450 480 1 1 3; E 20200002 370 540 + 370 545
"i" 1 LB H 0 + 370 525 "i" 1 LB H 0 1 0; E 20400002 420 460 1 1 1; E
20200002 600 540 + 600 545 "s" 1 LB H 0 + 600 525 "s" 1 LB H 0 2 0; E
20200002 450 390 + 450 395 "vss" 1 LB H 0 + 450 375 "vss" 1 LB H 0 4 0
; E 20400002 450 650 1 2 2; E 20000002 530 540 0; E 20400002 530 630
1 3 1; E 20400002 560 440 1 4 2; E 20400002 530 460 1 4 1; E
20400002 560 480 1 4 3; E 20000002 560 540 0; E 20400002 560 610 1 3
3; E 20400002 560 650 1 3 2; E 20000002 600 450 0; E 20200002 600
390 + 600 395 "vss1" 1 LB H 0 + 600 375 "vss1" 1 LB H 0 5 0; $S 16;
S 11 4 2; S 9 3 2; S 12 5 2; S 1 6 2; S 18 10 2; S 7 1 2; S 3 2
2; S 13 14 2; S 1 13 2; S 4 15 2; S 16 13 2; S 17 18 2; S 8 3 2
; S 18 19 2; S 12 20 2; S 22 21 2; $Z;
