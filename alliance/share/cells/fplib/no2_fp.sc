#cell1 no2_fp cmos schematic 14336 v7r5.6
# 30-Oct-91 12:43 30-Oct-91 12:43 dea9247 * .icn pChannelTransistor .sc
# pChannelTransistor .icn nChannelTransistor .sc nChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 "no2_dp"; $B "Asheet" 1100 850; $D 2; D
"pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$P 4; P 1 "nchwidth" "STRING" "3"; P 2 "nchlength" "STRING" "2"; P
3 "pchwidth" "STRING" "18"; P 4 "pchlength" "STRING" "2"; $N 7 "I0"
"I1" "O" "VDD" "VSS" "BULK" ""; $C 5; C 1 1 1; C 2 1 2; C 3 1 3;
C 4 1 4; C 5 1 5; $J 4; J 2 "u3" 3 1 1 1 2 1 5 3 1 3 2 1 0 "6" 2 0
"1"; J 1 "u4" 3 1 1 2 2 1 7 3 1 3 2 1 0 "21" 2 0 "1"; J 2 "u5" 3 1 1
2 2 1 5 3 1 3 2 1 0 "6" 2 0 "1"; J 1 "u6" 3 2 1 4 1 1 1 3 1 7 2 1 0
"21" 2 0 "1"; $I 4; I 2 "u3" "@" 480 310 0 2 2 1 0 "6" 2 0 "1"; I 1
"u4" "@" 640 450 0 2 2 1 0 "21" 2 0 "1"; I 2 "u5" "@" 640 310 0 2 2 1
0 "6" 2 0 "1"; I 1 "u6" "@" 640 540 0 2 2 1 0 "21" 2 0 "1"; $E 25;
E 3 670 380 0; E 200003 350 540 + 350 545 "i0" 1 CB H 0 + 350 525 ""
1 LB H 0 1 0; E 3 590 450 0; E 400003 670 560 1 4 2; E 400003 640
540 1 4 1; E 3 510 380 0; E 3 430 310 0; E 3 430 540 0; E 200003
350 450 + 350 455 "i1" 1 CB H 0 + 350 435 "" 1 LB H 0 2 0; E 200003
740 380 + 740 385 "o" 1 LB H 0 + 740 365 "" 1 LB H 0 3 0; E 3 510 250
0; E 400003 480 310 1 1 1; E 400003 510 290 1 1 2; E 400003 510 330
1 1 3; E 400003 640 450 1 2 1; E 400003 670 470 1 2 2; E 400003 670
430 1 2 3; E 400003 640 310 1 3 1; E 400003 670 290 1 3 2; E 400003
670 330 1 3 3; E 3 590 310 0; E 3 670 250 0; E 400003 670 520 1 4 3
; E 20200002 670 600 + 670 605 "VDD" 1 LB H 0 + 670 585 "" 1 LB H 0 4
0; E 20200002 670 200 + 670 205 "VSS" 1 LB H 0 + 670 185 "" 1 LB H 0
5 0; $S 19; S 22 19 2; S 6 1 400002; S 7 8 200002; S 9 3 400002;
S 1 10 400002; S 21 3 2; S 21 18 2; S 20 1 2; S 11 22 2; S 7 12 2
; S 3 15 2; S 1 17 2; S 8 5 2; S 16 23 2; S 2 8 400002; S 14 6 2
; S 11 13 2; S 4 24 2; S 25 22 2; $T 1; T + 460 140
"no2_dp  23/10/91" 1 LB H 0; $Z;
