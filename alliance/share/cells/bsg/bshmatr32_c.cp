#cell1 bshmatr32_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A 14 0 28 120
B 14 0 28 120
F F
C comr0 14 43 metal2 2 1 * * W comr0
C comr0 25 0 metal 1 2 * * S comr0
C nshr 28 48 metal2 2 3 * * E nshr
C nshr 14 48 metal2 2 4 * * W nshr
C vss1 28 120 metal2 8 5 * * E vss1
C vss 14 0 metal2 8 6 * * W vss
C vss1 14 120 metal2 8 7 * * W vss1
C vdd 14 60 metal2 6 8 * * W vdd
C vdd 28 60 metal2 6 9 * * E vdd
C vss 28 0 metal2 8 10 * * E vss
I via1 25 43 0 "cvia" ly * *
I cbn1 16 114 0 "cbn" ly * *
I cbn2 16 108 0 "cbn" ly * *
I cbn3 16 102 0 "cbn" ly * *
I cbn4 16 96 0 "cbn" ly * *
I cbn5 16 90 0 "cbn" ly * *
I cbn6 16 84 0 "cbn" ly * *
I cbn7 16 78 0 "cbn" ly * *
I cbn8 16 72 0 "cbn" ly * *
I cbn9 16 66 0 "cbn" ly * *
I cbn10 16 54 0 "cbn" ly * *
I via2 16 62 0 "cvia" ly * *
P 14 120 * metal2
P 28 120 * metal2
W 6 112 36 128 vss1
S 8 H metal2 P 1 P 2
P 14 60 * metal2
P 28 60 * metal2
W 8 54 34 66 vdd
S 6 H metal2 P 3 P 4
P 14 0 * metal2
P 28 0 * metal2
W 6 -8 36 8 vss
S 8 H metal2 P 5 P 6
P 13 83 * nwell
P 29 83 * nwell
W -55 15 97 151
S 68 H nwell P 7 P 8
P 14 48 * metal2
P 28 48 * metal2
W 12 46 30 50 nshr
S 2 H metal2 P 9 P 10
P 16 54 * metal
P 16 114 * metal
W 14 52 18 116
S 2 V metal P 11 P 12
P 25 0 * metal
P 25 43 * metal
W 24 -1 26 44
S 1 V metal P 13 P 14
P 14 68 * metal2
P 28 68 * metal2
W -6 48 48 88
S 20 H metal2 P 15 P 16
P 14 8 * metal2
P 28 8 * metal2
W -8 -14 50 30
S 22 H metal2 P 17 P 18
P 14 43 * metal2
P 25 43 * metal2
W 12 41 27 45 comr0
S 2 H metal2 P 19 P 20
P 14 23 * allowM2
P 28 23 * allowM2
W 12 21 30 25
S 2 H allowM2 P 21 P 22
P 14 28 * allowM2
P 28 28 * allowM2
W 12 26 30 30
S 2 H allowM2 P 23 P 24
P 14 33 * allowM2
P 28 33 * allowM2
W 12 31 30 35
S 2 H allowM2 P 25 P 26
P 14 38 * allowM2
P 28 38 * allowM2
W 12 36 30 40
S 2 H allowM2 P 27 P 28
P 14 82 * allowM2
P 28 82 * allowM2
W 12 80 30 84
S 2 H allowM2 P 29 P 30
P 14 87 * allowM2
P 28 87 * allowM2
W 12 85 30 89
S 2 H allowM2 P 31 P 32
P 14 92 * allowM2
P 28 92 * allowM2
W 12 90 30 94
S 2 H allowM2 P 33 P 34
P 14 97 * allowM2
P 28 97 * allowM2
W 12 95 30 99
S 2 H allowM2 P 35 P 36
P 14 102 * allowM2
P 28 102 * allowM2
W 12 100 30 104
S 2 H allowM2 P 37 P 38
P 14 107 * allowM2
P 28 107 * allowM2
W 12 105 30 109
S 2 H allowM2 P 39 P 40
E
