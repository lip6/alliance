#cell1 grpbs_c CMOS schematic 13312 v7r5.6
# 20-Mar-93 15:25 20-Mar-93 15:25 dea9221 * .icn pChannelTransistor .sc
# pChannelTransistor .icn nChannelTransistor .sc nChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 14 "E1" "CK" "E6" "E2" "E3" "E8" "E5" "E4" "E7" "CK_11" "CK" "VDD"
"VSS" "BULK"; $C 13; C 1 1 1; C 2 1 2; C 24 1 3; C 10 1 4; C 12
1 5; C 14 1 6; C 16 1 7; C 18 1 8; C 22 1 9; C 25 1 10; C 26 1 2
; C 27 1 12; C 28 1 13; $J 2; J 1 "u2" 3 1 1 2 2 1 10 3 1 13 2 1 0
"8" 2 0 "1"; J 2 "u3" 3 1 1 2 2 1 12 3 1 10 2 1 0 "14" 2 0 "1"; $I 2
; I 1 "u2" "@" 620 550 0 22 2 1 0 "8" 2 0 "1"; I 2 "u3" "@" 470 550 0
22 2 1 0 "14" 2 0 "1"; $E 22; E 200000 0 720 + 0 720 "e7" 1 LB H 0 +
0 720 "e7" 1 LB H 0 22 0; E 200000 0 80 + 0 80 "e1" 1 LB H 0 + 0 80
"e1" 1 LB H 0 1 0; E 200000 0 480 + 0 480 "e8" 1 LB H 0 + 0 480 "e8"
1 LB H 0 14 0; E 200000 0 160 + 0 160 "ck" 1 LB H 0 + 0 160 "ck" 1 LB
H 0 2 0; E 200000 0 360 + 0 360 "e3" 1 LB H 0 + 0 360 "e3" 1 LB H 0
12 0; E 200000 0 200 + 0 200 "e6" 1 LB H 0 + 0 200 "e6" 1 LB H 0 24 0
; E 200000 0 640 + 0 640 "e4" 1 LB H 0 + 0 640 "e4" 1 LB H 0 18 0; E
200000 0 280 + 0 280 "e2" 1 LB H 0 + 0 280 "e2" 1 LB H 0 10 0; E
200000 0 560 + 0 560 "e5" 1 LB H 0 + 0 560 "e5" 1 LB H 0 16 0; E
20400002 620 550 1 1 1; E 20400002 650 530 1 1 2; E 20400002 650 570
1 1 3; E 20400002 470 550 1 2 1; E 20400002 500 570 1 2 2; E
20400002 500 530 1 2 3; E 20200002 440 550 + 440 555 "ck" 1 LB H 0 +
440 535 "" 1 LB H 0 26 0; E 20200002 500 610 + 500 615 "VDD" 1 LB H 0
+ 500 595 "" 1 LB H 0 27 0; E 20200002 650 610 + 650 615 "VSS" 1 LB H
0 + 650 595 "" 1 LB H 0 28 0; E 20000002 650 480 0; E 20000002 500
480 0; E 20000002 570 480 0; E 20200002 570 430 + 570 435 "ck_11" 1
LB H 0 + 570 415 "" 1 LB H 0 25 0; $S 9; S 13 10 2; S 16 13 2; S
14 17 2; S 12 18 2; S 19 11 2; S 20 15 2; S 20 21 2; S 21 19 2;
S 22 21 2; $Z;
