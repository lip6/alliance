#cell1 bshrmx32_c CMOS schematic 12288 v7r5.6
# 27-May-92 15:30 27-May-92 15:30 dea9106 * .icn nChannelTransistor .sc
# nChannelTransistor .icn pChannelTransistor .sc pChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 9 "VSS1" "VSS" "SHR" "VDD" "VSS" "NSHR" "VDD" "VSS1" "BULK"; $C 8
; C 6 1 1; C 30 1 2; C 38 1 3; C 15 1 4; C 31 1 2; C 33 1 6; C
16 1 4; C 5 1 1; $J 2; J 2 "u4" 3 2 1 4 1 1 6 3 1 3 2 1 0 "54" 2 0
"1"; J 1 "u2" 3 2 1 2 1 1 6 3 1 3 2 1 0 "27" 2 0 "1"; $I 2; I 2
"u4" "@" 550 300 0 22 2 1 0 "54" 2 0 "1"; I 1 "u2" "@" 550 260 0 22 2
1 0 "27" 2 0 "1"; $E 18; E 20200002 520 560 + 520 565 "vss1" 1 LB H
0 + 520 545 "" 1 LB H 0 6 0; E 20000002 550 280 0; E 20000002 580
400 0; E 20000002 540 280 0; E 20400002 550 260 1 2 1; E 20200002
520 240 + 520 245 "vss" 1 LB H 0 + 520 225 "" 1 LB H 0 30 0; E
20400002 580 240 1 2 2; E 20200002 700 560 + 700 565 "vss1" 1 LB H 0
+ 700 545 "" 1 LB H 0 5 0; E 20200002 600 220 + 600 225 "shr" 1 LB H
0 + 600 205 "" 1 LB H 0 38 0; E 20000002 540 420 0; E 20400002 550
300 1 1 1; E 20000002 600 280 0; E 20400002 580 280 2 1 3 2 3; E
20400002 580 320 1 1 2; E 20200002 520 400 + 520 405 "vdd" 1 LB H 0 +
520 385 "" 1 LB H 0 15 0; E 20200002 700 240 + 700 245 "vss" 1 LB H 0
+ 700 225 "" 1 LB H 0 31 0; E 20200002 520 420 + 520 425 "nshr" 1 LB
H 0 + 520 405 "" 1 LB H 0 33 0; E 20200002 700 400 + 700 405 "vdd" 1
LB H 0 + 700 385 "" 1 LB H 0 16 0; $S 13; S 17 10 2; S 14 3 2; S 9
12 2; S 7 16 2; S 4 2 2; S 3 18 2; S 6 7 2; S 13 12 2; S 5 2 2;
S 2 11 2; S 1 8 2; S 4 10 2; S 15 3 2; $Z;
