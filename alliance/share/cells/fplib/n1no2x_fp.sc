#cell1 n1no2x_fp cmos schematic 11264 v7r5.6
# 2-Oct-93 16:29 2-Oct-93 16:29 lotfi * .icn nChannelTransistor .sc
# nChannelTransistor .icn pChannelTransistor .sc pChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 "n1no2x_fp"; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$P 4; P 1 "nchwidth" "STRING" "4"; P 2 "nchlength" "STRING" "2"; P
3 "pchwidth" "STRING" "12"; P 4 "pchlength" "STRING" "2"; $N 6 "I0"
"I1" "O" "VDD" "VSS" "BULK"; $C 5; C 1 1 1; C 2 1 2; C 3 1 3; C 4
1 4; C 5 1 5; $J 2; J 2 "u3" 3 2 1 4 3 1 3 1 1 1 3 1 0 "12" 2 0 "1"
3 0 "0"; J 1 "u2" 3 2 1 5 3 1 3 1 1 1 3 1 0 "6" 2 0 "1" 3 0 "0"; $I
2; I 2 "u3" "@" 560 500 0 2 3 1 0 "12" 2 0 "1" 3 0 "0"; I 1 "u2" "@"
560 360 0 2 3 1 0 "6" 2 0 "1" 3 0 "0"; $E 16; E 400003 590 340 1 2 2
; E 400003 590 380 1 2 3; E 400003 590 520 1 1 2; E 400003 590 480 1
1 3; E 400003 560 360 1 2 1; E 400003 560 500 1 1 1; E 3 540 500 0
; E 3 540 360 0; E 20000002 540 430 0; E 20200002 710 430 + 710 435
"o" 1 LB H 0 + 710 415 "" 1 LB H 0 3 0; E 20200002 420 430 + 420 435
"i0" 1 LB H 0 + 420 415 "" 1 LB H 0 1 0; E 20000002 590 430 0; E
20200002 590 550 + 590 555 "vdd" 1 LB H 0 + 590 535 "" 1 LB H 0 4 0;
E 20200002 590 310 + 590 315 "vss" 1 LB H 0 + 590 295 "" 1 LB H 0 5 0
; E 20200002 470 310 + 470 315 "i1" 1 LB H 0 + 470 295 "" 1 LB H 0 2 0
; E 20000002 470 550 0; $S 11; S 8 5 2; S 7 6 2; S 2 12 2; S 11 9
2; S 3 13 2; S 9 7 0; S 14 1 2; S 12 4 2; S 12 10 2; S 8 9 0; S
15 16 2; $Z;
