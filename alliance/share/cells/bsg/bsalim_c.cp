#cell1 bsalim_c idps_symb compose *
# 16-Aug-93 17:09 16-Aug-93 17:09 mbk305 * .cp
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
I cbp1 17 96 0 "cbp" ly * *
I cbp2 17 102 0 "cbp" ly * *
I cbp3 17 114 0 "cbp" ly * *
I cbp4 17 108 0 "cbp" ly * *
I cbp5 17 24 0 "cbp" ly * *
I cbp6 17 18 0 "cbp" ly * *
I cbp7 17 12 0 "cbp" ly * *
I cbp8 17 6 0 "cbp" ly * *
I cbn1 7 54 0 "cbn" ly * *
I cbn2 7 48 0 "cbn" ly * *
I cbn3 7 42 0 "cbn" ly * *
I cbn4 7 36 0 "cbn" ly * *
I cbn5 7 84 0 "cbn" ly * *
I cbn6 7 78 0 "cbn" ly * *
I cbn7 7 72 0 "cbn" ly * *
I cbn8 7 66 0 "cbn" ly * *
I via1 19 118 0 "cvia" ly * *
I via2 15 118 0 "cvia" ly * *
I via3 9 62 0 "cvia" ly * *
I via4 5 62 0 "cvia" ly * *
I via5 9 58 0 "cvia" ly * *
I via6 5 58 0 "cvia" ly * *
I via7 15 2 0 "cvia" ly * *
I via8 19 2 0 "cvia" ly * *
I nwell_2 23 48 0 "ref_ref" ly * *
I nwell_1 23 72 0 "ref_ref" ly * *
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
P 1 60 * nwell
P 24 60 * nwell
W -51 8 76 112
S 52 H nwell P 17 P 18
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
E
