#cell1 amalimpout_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 3 5 26 65
B 3 5 26 65
F F
C vdd 20 5 metal 1 1 * * S vdd
C vss 9 65 metal 1 2 * * N vss
C vss 3 5 metal2 2 3 * * W vss
C vdd 3 65 metal2 2 4 * * W vdd
C vdd 26 65 metal2 2 5 * * E vdd
C vss 26 5 metal2 2 6 * * E vss
I cbn1 22 50 0 "cbn" ly * *
I cbn2 18 50 0 "cbn" ly * *
I cbn3 22 46 0 "cbn" ly * *
I cbp1 11 22 0 "cbp" ly * *
I cbp2 7 22 0 "cbp" ly * *
I cbp3 11 14 0 "cbp" ly * *
I cbp4 7 14 0 "cbp" ly * *
I cbn4 18 42 0 "cbn" ly * *
I cbn5 22 42 0 "cbn" ly * *
I cbn6 18 46 0 "cbn" ly * *
I cbp5 11 18 0 "cbp" ly * *
I cbp6 7 18 0 "cbp" ly * *
I via1 23 65 0 "cvia" ly * *
I via2 17 65 0 "cvia" ly * *
I via3 12 5 0 "cvia" ly * *
I via4 6 5 0 "cvia" ly * *
P 9 13 * ptie
P 9 23 * ptie
W 1 5 17 31
S 8 V ptie P 1 P 2
P 20 41 * ntie
P 20 51 * ntie
W 12 33 28 59
S 8 V ntie P 3 P 4
P 20 5 * metal
P 20 65 * metal
W 12 -3 28 73
S 8 V metal P 5 P 6
P 9 5 * metal
P 9 65 * metal
W 1 -3 17 73
S 8 V metal P 7 P 8
P 3 52 * nwell
P 26 52 * nwell
W -23 26 52 78
S 26 H nwell P 9 P 10
P 3 65 * metal2
P 26 65 * metal2
W -3 59 32 71
S 6 H metal2 P 11 P 12
P 3 5 * metal2
P 26 5 * metal2
W -5 -3 34 13
S 8 H metal2 P 13 P 14
E
