#cell1 bshalim_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A 2 0 23 120
B 2 0 23 120
F F
C vss 23 120 metal2 8 1 * * E vss
C vss 2 120 metal2 8 2 * * W vss
C vdd 2 60 metal2 6 3 * * W vdd
C vdd 23 60 metal2 6 4 * * E vdd
C vss 2 0 metal2 8 5 * * W vss
C vss 23 0 metal2 8 6 * * E vss
C vdd 7 120 metal 6 7 * * N vdd
C vdd 7 0 metal 6 8 * * S vdd
C vss 17 0 metal 8 9 * * S vss
C vss 17 120 metal 8 10 * * N vss
I cbp1 17 36 0 "cbp" ly * *
I cbp2 17 30 0 "cbp" ly * *
I cbp3 17 24 0 "cbp" ly * *
I cbp4 17 18 0 "cbp" ly * *
I cbp5 17 12 0 "cbp" ly * *
I cbp6 17 6 0 "cbp" ly * *
I cbn1 7 54 0 "cbn" ly * *
I cbn2 7 114 0 "cbn" ly * *
I cbn3 7 108 0 "cbn" ly * *
I cbn4 7 102 0 "cbn" ly * *
I cbn5 7 96 0 "cbn" ly * *
I cbn6 7 90 0 "cbn" ly * *
I cbn7 7 84 0 "cbn" ly * *
I cbn8 7 78 0 "cbn" ly * *
I cbn9 7 72 0 "cbn" ly * *
I cbn10 7 66 0 "cbn" ly * *
I via1 19 118 0 "cvia" ly * *
I via2 15 118 0 "cvia" ly * *
I via3 9 62 0 "cvia" ly * *
I via4 5 62 0 "cvia" ly * *
I via5 9 58 0 "cvia" ly * *
I via6 5 58 0 "cvia" ly * *
I via7 15 2 0 "cvia" ly * *
I via8 19 2 0 "cvia" ly * *
I nwell 23 83 0 "ref_ref" ly * *
I fc[0] 2 0 0 "ref_ref" ly * *
P 5 58 * metal
P 9 58 * metal
W 4 57 10 59 vdd
S 1 H metal P 1 P 2
P 2 0 * metal2
P 23 0 * metal2
W -6 -8 31 8 vss
S 8 H metal2 P 3 P 4
P 2 60 * metal2
P 23 60 * metal2
W -4 54 29 66 vdd
S 6 H metal2 P 5 P 6
P 5 58 * metal2
P 9 58 * metal2
W 2 55 12 61
S 3 H metal2 P 7 P 8
P 7 0 * metal
P 7 120 * metal
W 1 -6 13 126 vdd
S 6 V metal P 9 P 10
P 5 62 * metal
P 9 62 * metal
W 3 60 11 64 vdd
S 2 H metal P 11 P 12
P 5 62 * metal2
P 9 62 * metal2
W 2 59 12 65
S 3 H metal2 P 13 P 14
P 2 120 * metal2
P 23 120 * metal2
W -6 112 31 128
S 8 H metal2 P 15 P 16
P 1 83 * nwell
P 24 83 * nwell
W -67 15 92 151
S 68 H nwell P 17 P 18
P 17 2 * metal
P 19 2 * metal
W 15 0 21 4 vss
S 2 H metal P 19 P 20
P 15 2 * metal
P 17 2 * metal
W 13 0 19 4 vss
S 2 H metal P 21 P 22
P 17 0 * metal
P 17 2 * metal
W 9 -8 25 10 vss
S 8 V metal P 23 P 24
P 17 2 * metal
P 17 118 * metal
W 9 -6 25 126 vss
S 8 V metal P 25 P 26
P 15 118 * metal
P 17 118 * metal
W 13 116 19 120 vss
S 2 H metal P 27 P 28
P 17 118 * metal
P 19 118 * metal
W 15 116 21 120 vss
S 2 H metal P 29 P 30
P 17 118 * metal
P 17 120 * metal
W 9 110 25 128 vss
S 8 V metal P 31 P 32
P 2 23 * allowM2
P 23 23 * allowM2
W 0 21 25 25
S 2 H allowM2 P 33 P 34
P 2 28 * allowM2
P 23 28 * allowM2
W 0 26 25 30
S 2 H allowM2 P 35 P 36
P 2 33 * allowM2
P 23 33 * allowM2
W 0 31 25 35
S 2 H allowM2 P 37 P 38
P 2 38 * allowM2
P 23 38 * allowM2
W 0 36 25 40
S 2 H allowM2 P 39 P 40
P 2 82 * allowM2
P 23 82 * allowM2
W 0 80 25 84
S 2 H allowM2 P 41 P 42
P 2 87 * allowM2
P 23 87 * allowM2
W 0 85 25 89
S 2 H allowM2 P 43 P 44
P 2 92 * allowM2
P 23 92 * allowM2
W 0 90 25 94
S 2 H allowM2 P 45 P 46
P 2 97 * allowM2
P 23 97 * allowM2
W 0 95 25 99
S 2 H allowM2 P 47 P 48
P 2 102 * allowM2
P 23 102 * allowM2
W 0 100 25 104
S 2 H allowM2 P 49 P 50
P 2 107 * allowM2
P 23 107 * allowM2
W 0 105 25 109
S 2 H allowM2 P 51 P 52
E
