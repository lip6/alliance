#cell1 grmrck_c CMOS schematic 31744 v7r5.6
# 5-Mar-93 15:48 5-Mar-93 15:48 dea9221 * .icn pChannelTransistor .sc
# pChannelTransistor .icn nChannelTransistor .sc nChannelTransistor .
V 4
 $H 2 10000 "Asheet" 1 ""; $B "Asheet" 1100 800; $D 2; D
"nChannelTransistor" "nChannelTransistor" 3 "gate" 0 0 1 "source" 30
-20 2 "drain" 30 20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
D "pChannelTransistor" "pChannelTransistor" 3 "gate" 0 0 1 "source" 30
20 2 "drain" 30 -20 3 3 "chwidth" 1 "chlength" 2 "gateCapacitance" 3;
$N 13 "VSS1" "VSS2" "VSS0" "CKP" "VDD" "CK_02" "CK_03" "CK" "VSS"
"BULK" "" "" ""; $C 8; C 13 1 1; C 2 1 2; C 1 1 3; C 3 1 4; C 14
1 5; C 9 1 6; C 12 1 7; C 8 1 8; $J 15; J 1 "u26" 3 2 1 3 1 1 13
3 1 4 2 1 0 "28" 2 0 "1"; J 2 "u25" 3 3 1 13 1 1 12 2 1 5 2 1 0 "14"
2 0 "1"; J 2 "u27" 3 3 1 4 2 1 5 1 1 13 2 1 0 "54" 2 0 "1"; J 1
"u24" 3 3 1 13 2 1 3 1 1 12 2 1 0 "14" 2 0 "1"; J 2 "u15" 3 2 1 5 1 1
6 3 1 7 2 1 0 "36" 2 0 "1"; J 1 "u14" 3 3 1 7 2 1 2 1 1 6 2 1 0 "18"
2 0 "1"; J 2 "u22" 3 1 1 7 2 1 5 3 1 12 2 1 0 "11" 2 0 "1"; J 1
"u20" 3 3 1 12 2 1 2 1 1 7 2 1 0 "3" 2 0 "1"; J 2 "u9" 3 1 1 11 2 1 5
3 1 6 2 1 0 "10" 2 0 "1"; J 2 "u10" 3 1 1 11 2 1 5 3 1 6 2 1 0 "7" 2
0 "1"; J 2 "u11" 3 1 1 11 2 1 5 3 1 6 2 1 0 "10" 2 0 "1"; J 1 "u13"
3 1 1 11 3 1 6 2 1 2 2 1 0 "7" 2 0 "1"; J 1 "u12" 3 2 1 2 3 1 6 1 1
11 2 1 0 "7" 2 0 "1"; J 1 "u16" 3 1 1 8 2 1 2 3 1 11 2 1 0 "7" 2 0
"1"; J 2 "u17" 3 3 1 11 2 1 5 1 1 8 2 2 0 "1" 1 0 "14"; $I 15; I 1
"u26" "@" 850 360 0 20 2 1 0 "28" 2 0 "1"; I 2 "u25" "@" 750 460 0 20
2 1 0 "14" 2 0 "1"; I 2 "u27" "@" 850 460 0 20 2 1 0 "54" 2 0 "1"; I
1 "u24" "@" 750 360 0 20 2 1 0 "14" 2 0 "1"; I 2 "u15" "@" 550 460 0
20 2 1 0 "36" 2 0 "1"; I 1 "u14" "@" 550 360 0 20 2 1 0 "18" 2 0 "1"
; I 2 "u22" "@" 650 460 0 20 2 1 0 "11" 2 0 "1"; I 1 "u20" "@" 650
360 0 20 2 1 0 "3" 2 0 "1"; I 2 "u9" "@" 330 450 0 20 2 1 0 "10" 2 0
"1"; I 2 "u10" "@" 380 450 0 20 2 1 0 "7" 2 0 "1"; I 2 "u11" "@" 430
450 0 20 2 1 0 "10" 2 0 "1"; I 1 "u13" "@" 410 370 0 20 2 1 0 "7" 2 0
"1"; I 1 "u12" "@" 360 370 0 20 2 1 0 "7" 2 0 "1"; I 1 "u16" "@" 230
370 0 20 2 1 0 "7" 2 0 "1"; I 2 "u17" "@" 230 460 0 20 2 2 0 "1" 1 0
"14"; $E 103; E 20200002 700 250 + 700 255 "vss1" 1 LB H 0 + 700 235
"" 1 LB H 0 13 0; E 20200002 600 250 + 600 255 "vss2" 1 LB H 0 + 600
235 "" 1 LB H 0 2 0; E 20200002 800 250 + 800 255 "vss0" 1 LB H 0 +
800 235 "" 1 LB H 0 1 0; E 20000002 880 510 0; E 20000002 460 410 0
; E 20400002 390 350 1 13 2; E 20000002 320 370 0; E 20000002 500
410 0; E 20400002 410 370 1 12 1; E 20000002 390 310 0; E 20000002
360 410 0; E 20000002 390 410 0; E 20000002 440 410 0; E 20400002
390 390 1 13 3; E 20400002 360 370 1 13 1; E 20000002 440 310 0; E
20400002 440 390 1 12 3; E 20000002 410 410 0; E 20400002 440 350 1
12 2; E 20000002 320 450 0; E 20400002 330 450 1 9 1; E 20400002
380 450 1 10 1; E 20400002 430 450 1 11 1; E 20400002 460 470 1 11 2
; E 20400002 460 430 1 11 3; E 20400002 410 470 1 10 2; E 20400002
410 430 1 10 3; E 20400002 360 470 1 9 2; E 20400002 360 430 1 9 3;
E 20000002 780 510 0; E 20000002 680 310 0; E 20000002 530 460 0; E
20000002 630 460 0; E 20400002 650 460 1 7 1; E 20000002 610 410 0;
E 20400002 580 480 1 5 2; E 20400002 680 480 1 7 2; E 20400002 550
460 1 5 1; E 20000002 600 310 0; E 20400002 680 380 1 8 3; E
20400002 680 340 1 8 2; E 20400002 650 360 1 8 1; E 20400002 680 440
1 7 3; E 20000002 630 360 0; E 20000002 530 410 0; E 20000002 580
410 0; E 20000002 580 310 0; E 20400002 580 380 1 6 3; E 20400002
580 340 1 6 2; E 20400002 550 360 1 6 1; E 20400002 580 440 1 5 3;
E 20000002 530 360 0; E 20000002 630 410 0; E 20000002 680 410 0; E
20400002 880 440 1 3 3; E 20400002 780 380 1 4 3; E 20400002 880 340
1 1 2; E 20400002 850 360 1 1 1; E 20000002 730 360 0; E 20000002
830 410 0; E 20000002 880 310 0; E 20200002 900 410 + 900 415 "ckp"
1 LB H 0 + 900 395 "" 1 LB H 0 3 0; E 20000002 880 410 0; E 20400002
880 380 1 1 3; E 20000002 730 460 0; E 20400002 780 440 1 2 3; E
20000002 780 310 0; E 20000002 780 410 0; E 20000002 730 410 0; E
20000002 830 360 0; E 20000002 830 460 0; E 20400002 750 460 1 2 1;
E 20400002 780 340 1 4 2; E 20400002 880 480 1 3 2; E 20400002 850
460 1 3 1; E 20400002 750 360 1 4 1; E 20400002 780 480 1 2 2; E
20000002 800 310 0; E 20000002 410 510 0; E 20000002 470 510 0; E
20200002 470 580 + 470 585 "vdd" 1 LB H 0 + 470 565 "" 1 LB H 0 14 0;
E 20000002 460 510 0; E 20000002 500 560 0; E 20000002 360 510 0; E
20000002 680 510 0; E 20000002 610 530 0; E 20000002 580 510 0; E
20200002 900 560 + 900 565 "ck_02" 1 LB H 0 + 900 545 "" 1 LB H 0 9 0
; E 20200002 900 530 + 900 535 "ck_03" 1 LB H 0 + 900 515 "" 1 LB H 0
12 0; E 20200002 130 410 + 130 415 "ck" 1 LB H 0 + 130 395 "" 1 LB H
0 8 0; E 20400002 230 370 1 14 1; E 20400002 260 350 1 14 2; E
20400002 260 390 1 14 3; E 20400002 260 440 1 15 3; E 20400002 260
480 1 15 2; E 20400002 230 460 1 15 1; E 20000002 260 310 0; E
20000002 260 510 0; E 20000002 200 460 0; E 20000002 200 370 0; E
20000002 200 410 0; E 20000002 260 410 0; E 20000002 320 410 0; $S
92; S 2 39 2; S 3 78 2; S 30 4 2; S 5 8 2; S 10 6 2; S 10 16 2;
S 16 19 2; S 16 47 2; S 15 9 2; S 7 15 2; S 17 13 2; S 13 5 2; S
18 13 2; S 12 18 2; S 11 12 2; S 14 12 2; S 20 21 2; S 11 29 2;
S 21 22 2; S 22 23 2; S 24 82 2; S 5 25 2; S 26 79 2; S 18 27 2;
S 28 84 2; S 87 85 2; S 83 88 2; S 37 85 2; S 46 35 2; S 8 45 2;
S 35 53 2; S 35 86 2; S 33 34 2; S 36 87 2; S 8 83 2; S 39 31 2;
S 47 39 2; S 54 43 2; S 40 54 2; S 44 42 2; S 31 41 2; S 45 32 2
; S 52 45 2; S 46 51 2; S 48 46 2; S 52 50 2; S 47 49 2; S 54 69
2; S 53 33 2; S 44 53 2; S 32 38 2; S 70 60 2; S 63 55 2; S 68
60 2; S 67 73 2; S 56 68 2; S 59 69 2; S 71 75 2; S 60 71 2; S
61 57 2; S 69 65 2; S 65 72 2; S 74 4 2; S 70 58 2; S 77 30 2; S
68 66 2; S 59 76 2; S 78 61 2; S 67 78 2; S 64 63 2; S 63 62 2;
S 79 82 2; S 80 81 2; S 82 80 2; S 84 79 2; S 80 87 2; S 86 89 2
; S 85 30 2; S 101 99 2; S 90 101 2; S 97 92 2; S 97 10 2; S 95
98 2; S 98 84 2; S 99 96 2; S 100 91 2; S 100 101 2; S 93 102 2;
S 102 94 2; S 7 103 2; S 103 20 2; S 102 103 2; $T 1; T + 600 140
"cell : grmrck_c" 1 LB H 0; $Z;