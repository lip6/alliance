#cell1 grrli_c CMOS schematic 10240 v7r5.6
# 2-Apr-92 16:05 2-Apr-92 16:05 fred * .icn pChannelTransistor .sc
# pChannelTransistor .icn nChannelTransistor .sc nChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 5 "F" "VSS" "I" "VDD" "BULK"; $C 4; C 3 1 1; C 2 1 2; C 4 1 3;
C 1 1 4; $J 2; J 1 "u2" 3 1 1 3 2 1 2 3 1 1 2 1 0 "22" 2 0 "1"; J 2
"u3" 3 1 1 3 2 1 4 3 1 1 2 1 0 "45" 2 0 "1"; $I 2; I 1 "u2" "@" 490
400 0 22 2 1 0 "22" 2 0 "1"; I 2 "u3" "@" 490 510 0 22 2 1 0 "45" 2 0
"1"; $E 14; E 20400002 490 400 1 1 1; E 20400002 520 380 1 1 2; E
20400002 520 420 1 1 3; E 20400002 490 510 1 2 1; E 20400002 520 530
1 2 2; E 20400002 520 490 1 2 3; E 20000002 520 450 0; E 20200002
590 450 + 590 455 "f" 1 LB H 0 + 590 435 "" 1 LB H 0 3 0; E 20000002
440 400 0; E 20200002 520 340 + 520 345 "vss" 1 LB H 0 + 520 325 "" 1
LB H 0 2 0; E 20200002 390 450 + 390 455 "i" 1 LB H 0 + 390 435 "" 1
LB H 0 4 0; E 20000002 440 510 0; E 20000002 440 450 0; E 20200002
520 570 + 520 575 "vdd" 1 LB H 0 + 520 555 "" 1 LB H 0 1 0; $S 10; S
9 1 2; S 10 2 2; S 12 4 2; S 11 13 2; S 9 13 2; S 7 8 2; S 3 7 2
; S 7 6 2; S 13 12 2; S 5 14 2; $T 1; T + 460 220 "cell : grrli_c" 1
LB H 0; $Z;
