#cell1 na2x_fp cmos schematic 13312 v7r5.6
# 2-Oct-93 16:44 2-Oct-93 16:44 lotfi * .icn pChannelTransistor .sc
# pChannelTransistor .icn nChannelTransistor .sc nChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 "na2x_dp"; $B "Asheet" 1100 850; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$P 4; P 1 "nchwidth" "STRING" "4"; P 2 "nchlength" "STRING" "2"; P
3 "pchwidth" "STRING" "6"; P 4 "pchlength" "STRING" "2"; $N 7 "I0"
"I1" "O" "VDD" "VSS" "BULK" ""; $C 5; C 1 1 1; C 2 1 2; C 3 1 3;
C 4 1 4; C 5 1 5; $J 4; J 1 "u6" 3 3 1 3 1 1 1 2 1 7 2 1 0 "12" 2 0
"1"; J 2 "u5" 3 1 1 1 2 1 4 3 1 3 2 1 0 "12" 2 0 "1"; J 1 "u7" 3 2 1
5 3 1 7 1 1 2 2 1 0 "12" 2 0 "1"; J 2 "u4" 3 3 1 3 2 1 4 1 1 2 2 1 0
"12" 2 0 "1"; $I 4; I 1 "u6" "@" 640 400 0 2 2 1 0 "12" 2 0 "1"; I
2 "u5" "@" 640 540 0 2 2 1 0 "12" 2 0 "1"; I 1 "u7" "@" 640 310 0 2 2
1 0 "12" 2 0 "1"; I 2 "u4" "@" 500 540 0 2 2 1 0 "12" 2 0 "1"; $E 25
; E 200002 420 400 + 420 405 "i0" 1 CB H 0 + 420 385 "" 1 LB H 0 1 0;
E 400003 670 420 1 1 3; E 400003 640 400 1 1 1; E 400003 670 290 1 3
2; E 20200002 670 630 + 670 635 "vdd" 1 LB H 0 + 670 615 "" 1 LB H 0
4 0; E 3 600 400 0; E 400003 530 520 1 4 3; E 3 460 310 0; E 3 530
470 0; E 3 670 590 0; E 400003 670 330 1 3 3; E 400003 640 310 1 3
1; E 400003 670 380 1 1 2; E 3 600 540 0; E 400003 640 540 1 2 1;
E 400003 670 560 1 2 2; E 400003 670 520 1 2 3; E 3 530 590 0; E
400003 530 560 1 4 2; E 400003 500 540 1 4 1; E 3 460 540 0; E
20200002 670 250 + 670 255 "vss" 1 LB H 0 + 670 235 "" 1 LB H 0 5 0;
E 20200002 710 470 + 710 475 "o" 1 LB H 0 + 710 455 "" 1 LB H 0 3 0;
E 3 670 470 0; E 200002 420 310 + 420 315 "i1" 1 LB H 0 + 420 295 ""
1 LB H 0 2 0; $S 19; S 8 21 200002; S 22 4 2; S 10 5 2; S 16 10 2
; S 21 20 2; S 11 13 2; S 6 3 2; S 1 6 2; S 19 18 2; S 6 14 2; S
14 15 2; S 18 10 2; S 8 12 2; S 24 23 2; S 24 17 2; S 2 24 2; S
9 24 400002; S 9 7 2; S 25 8 2; $Z;
