#cell1 tie_y idps_symb compose *
# 07-Oct-98 12:31 07-Oct-98 12:31 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 11 45
B 5 3 11 45
F F
C vdd 11 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 11 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
I cbp1 8 7 0 "cbp" ly * *
I cbn1 8 45 0 "cbn" ly * *
I cbn2 8 41 0 "cbn" ly * *
I cbn3 8 37 0 "cbn" ly * *
I cbn4 8 33 0 "cbn" ly * *
I cbn5 8 29 0 "cbn" ly * *
I cbp2 8 3 0 "cbp" ly * *
I cbp3 8 11 0 "cbp" ly * *
I cbp4 8 16 0 "cbp" ly * *
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 1 P 2
P 8 3 * ptie
P 8 16 * ptie
W 5 0 11 19
S 3 V ptie P 3 P 4
P 8 29 * ntie
P 8 45 * ntie
W 5 26 11 48
S 3 V ntie P 5 P 6
P 5 43 * metal
P 11 43 * metal
W -3 35 19 51 vdd
S 8 H metal P 7 P 8
P 5 5 * metal
P 11 5 * metal
W -3 -3 19 13 vss
S 8 H metal P 9 P 10
P 5 39 * nwell
P 11 39 * nwell
W -21 13 37 65
S 26 H nwell P 11 P 12
P 8 29 * metal
P 8 45 * metal
W 6 27 10 47 vdd
S 2 V metal P 13 P 14
P 8 3 * metal
P 8 16 * metal
W 6 1 10 18 vss
S 2 V metal P 15 P 16
E
