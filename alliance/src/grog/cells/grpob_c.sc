#cell1 grpob_c CMOS schematic 10240 v7r5.6
# 11-Mar-93 14:48 11-Mar-93 14:48 dea9221 * .icn nChannelTransistor .sc
# nChannelTransistor .icn pChannelTransistor .sc pChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 5 "F" "I" "VSS" "VDD" "BULK"; $C 4; C 2 1 1; C 1 1 2; C 4 1 3;
C 3 1 4; $J 2; J 1 "u2" 3 1 1 2 2 1 4 3 1 1 2 1 0 "50" 2 0 "1"; J 2
"u3" 3 3 1 1 2 1 3 1 1 2 2 1 0 "23" 2 0 "1"; $I 2; I 1 "u2" "@" 500
660 0 22 2 1 0 "50" 2 0 "1"; I 2 "u3" "@" 500 560 0 22 2 1 0 "23" 2 0
"1"; $E 14; E 20400002 500 660 1 1 1; E 20400002 530 680 1 1 2; E
20400002 530 640 1 1 3; E 20400002 530 580 1 2 3; E 20400002 530 540
1 2 2; E 20400002 500 560 1 2 1; E 20000002 480 660 0; E 20000002
480 560 0; E 20000002 530 610 0; E 20200002 570 610 + 570 615 "f" 1
LB H 0 + 570 595 "" 1 LB H 0 2 0; E 20000002 480 610 0; E 20200002
440 610 + 440 615 "i" 1 LB H 0 + 440 595 "" 1 LB H 0 1 0; E 20200002
530 500 + 530 505 "vss" 1 LB H 0 + 530 485 "" 1 LB H 0 4 0; E
20200002 530 720 + 530 725 "vdd" 1 LB H 0 + 530 705 "" 1 LB H 0 3 0;
$S 10; S 8 6 2; S 7 1 2; S 9 10 2; S 4 9 2; S 9 3 2; S 8 11 2;
S 11 7 2; S 12 11 2; S 13 5 2; S 2 14 2; $T 1; T + 470 450
"cell : grpob_c" 1 LB H 0; $Z;
