#cell2 * n1_y sc * 2 CMOS 10240 v8r4.4
# "21-Nov-94 GMT" "16:53:17 GMT" "21-Nov-94 GMT" "16:53:17 GMT" scr * .icn nChannelTransistor .sc nChannelTransistor .icn pChannelTransistor .sc pChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 "n1_y"; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 5 "I" "F" "VSS" "VDD" "BULK"; $C 4; C 1 1 1; C 2 1 2; C 4 1 3;
C 3 1 4; $J 2; J 2 "u3" 3 1 1 1 3 1 2 2 1 4 2 1 0 "12" 2 0 "1"; J 1
"u2" 3 3 1 2 1 1 1 2 1 3 2 1 0 "6" 2 0 "1"; $I 2; I 2 "u3" "@" 590
430 0 22 2 1 0 "12" 2 0 "1"; I 1 "u2" "@" 590 340 0 22 2 1 0 "6" 2 0
"1"; $E 14; E 20200002 500 370 + 490 370 "i" 1 RC H 0 + 500 355 "" 1
LB H 0 1 0; E 20000002 550 370 0; E 20000002 550 340 0; E 20200002
690 370 + 700 370 "f" 2 LC H 0 + 690 355 "" 1 LB H 0 2 0; E 20400002
590 430 1 1 1; E 20000002 550 430 0; E 20400002 620 410 1 1 3; E
20000002 620 370 0; E 20400002 620 360 1 2 3; E 20400002 590 340 1 2
1; E 20400002 620 320 1 2 2; E 20200002 620 260 + 620 265 "vss" 1 LB
H 0 + 620 245 "" 1 LB H 0 4 0; E 20400002 620 450 1 1 2; E 20200002
620 500 + 620 505 "vdd" 1 LB H 0 + 620 485 "" 1 LB H 0 3 0; $S 10; S
1 2 2; S 3 2 2; S 6 5 2; S 2 6 2; S 8 7 2; S 9 8 2; S 3 10 2; S
12 11 2; S 8 4 2; S 13 14 2; $T 1; T + 480 140 "n1_y " 1 LB H 0;
$Z;
