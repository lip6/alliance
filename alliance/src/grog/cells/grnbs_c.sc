#cell1 grnbs_c CMOS schematic 17408 v7r5.6
# 9-Mar-93 12:45 9-Mar-93 12:45 dea9221 * .icn nChannelTransistor .sc
# nChannelTransistor .icn pChannelTransistor .sc pChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 14 "VSS" "CK_11" "CK" "VDD0" "E7" "E6" "E4" "E5" "E8" "E2" "E1"
"E3" "VDD" "BULK"; $C 12; C 2 1 1; C 19 1 2; C 18 1 3; C 1 1 4;
C 16 1 5; C 15 1 6; C 7 1 7; C 8 1 8; C 9 1 9; C 5 1 10; C 17 1
11; C 6 1 12; $J 2; J 1 "u2" 3 1 1 3 2 1 4 3 1 2 2 1 0 "14" 2 0 "1"
; J 2 "u3" 3 3 1 2 1 1 3 2 1 1 2 1 0 "8" 2 0 "1"; $I 2; I 1 "u2" "@"
510 580 0 22 2 1 0 "14" 2 0 "1"; I 2 "u3" "@" 510 480 0 22 2 1 0 "8"
2 0 "1"; $E 22; E 20400002 510 580 1 1 1; E 20400002 540 600 1 1 2
; E 20400002 540 500 1 2 3; E 20000002 480 530 0; E 20200002 540 410
+ 540 415 "VSS" 1 LB H 0 + 540 395 "" 1 LB H 0 2 0; E 20200002 580
530 + 580 535 "ck_11" 1 LB H 0 + 580 515 "" 1 LB H 0 19 0; E 20400002
510 480 1 2 1; E 20000002 540 530 0; E 20200002 430 530 + 430 535
"ck" 1 LB H 0 + 430 515 "" 1 LB H 0 18 0; E 20400002 540 460 1 2 2;
E 20400002 540 560 1 1 3; E 20000002 480 580 0; E 20200002 540 640 +
540 645 "VDD0" 1 LB H 0 + 540 625 "" 1 LB H 0 1 0; E 20000002 480 480
0; E 20200002 430 300 + 430 305 "e7" 1 LB H 0 + 430 285 "" 1 LB H 0
16 0; E 20200002 430 320 + 430 325 "e6" 1 LB H 0 + 430 305 "" 1 LB H
0 15 0; E 20200002 430 380 + 430 385 "e4" 1 LB H 0 + 430 365 "" 1 LB
H 0 7 0; E 20200002 430 350 + 430 355 "e5" 1 LB H 0 + 430 335 "" 1 LB
H 0 8 0; E 20200002 430 270 + 430 275 "e8" 1 LB H 0 + 430 255 "" 1 LB
H 0 9 0; E 20200002 430 440 + 430 445 "e2" 1 LB H 0 + 430 425 "" 1 LB
H 0 5 0; E 20200002 430 490 + 430 495 "e1" 1 LB H 0 + 430 475 "" 1 LB
H 0 17 0; E 20200002 430 410 + 430 415 "e3" 1 LB H 0 + 430 395 "" 1
LB H 0 6 0; $S 10; S 9 4 2; S 5 10 2; S 2 13 2; S 12 1 2; S 8 11
2; S 8 6 2; S 4 12 2; S 3 8 2; S 14 4 2; S 14 7 2; $T 1; T +
480 260 "cell : grnbs_c" 1 LB H 0; $Z;
