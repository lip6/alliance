#cell1 zero_y idps_symb compose *
# 07-Oct-98 12:36 07-Oct-98 12:36 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 17 45
B 5 3 17 45
F F
C f 14 3 metal2 2 1 * * S f
C f 14 45 metal2 2 2 * * N f
C vdd 5 43 metal 8 3 * * W vdd
C vss 17 5 metal 8 4 * * E vss
C vdd 17 43 metal 8 5 * * E vdd
C vss 5 5 metal 8 6 * * W vss
I cdn1 8 16 0 "cdn" ly * *
I cdn2 14 16 0 "cdn" ly * *
I via12x1 14 21 0 "via12" ly * *
I cpf1 9 23 0 "cpf" ly * *
I cdn3 8 8 0 "cdn" ly * *
I cbp1 8 2 0 "cbp" ly * *
I cbp2 14 2 0 "cbp" ly * *
I cbn1 11 39 0 "cbn" ly * *
I cbn2 11 34 0 "cbn" ly * *
I cbn3 11 29 0 "cbn" ly * *
I cbn4 11 45 0 "cbn" ly * *
P 11 29 * ntie
P 11 45 * ntie
W 8 26 14 48
S 3 V ntie P 1 P 2
P 11 5 * ntrans
P 11 20 * ntrans
W 10 4 12 21
S 1 V ntrans P 3 P 4
P 8 2 * ptie
P 14 2 * ptie
W 6 0 16 4
S 2 H ptie P 5 P 6
P 5 5 * metal
P 17 5 * metal
W -3 -3 25 13 vss
S 8 H metal P 7 P 8
P 8 3 * metal
P 8 5 * metal
W 7 2 9 6 vss
S 1 V metal P 9 P 10
P 8 5 * metal
P 8 17 * metal
W 6 3 10 19 vss
S 2 V metal P 11 P 12
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 f
S 2 V metal2 P 13 P 14
P 8 7 * ndif
P 8 18 * ndif
W 6 5 10 20
S 2 V ndif P 15 P 16
P 14 7 * ndif
P 14 18 * ndif
W 12 5 16 20
S 2 V ndif P 17 P 18
P 14 16 * metal
P 14 21 * metal
W 13 15 15 22
S 1 V metal P 19 P 20
P 5 39 * nwell
P 17 39 * nwell
W -21 13 43 65
S 26 H nwell P 21 P 22
P 8 2 * allowM2
P 8 45 * allowM2
W 6 0 10 47
S 2 V allowM2 P 23 P 24
P 11 29 * metal
P 11 45 * metal
W 9 27 13 47 vdd
S 2 V metal P 25 P 26
P 9 29 * metal
P 11 29 * metal
W 7 27 13 31 vdd
S 2 H metal P 27 P 28
P 9 23 * metal
P 9 29 * metal
W 8 22 10 30 vdd
S 1 V metal P 29 P 30
P 5 43 * metal
P 17 43 * metal
W -3 35 25 51 vdd
S 8 H metal P 31 P 32
P 11 20 * poly
P 11 23 * poly
W 10 19 12 24
S 1 V poly P 33 P 34
P 9 23 * poly
P 11 23 * poly
W 8 22 12 24
S 1 H poly P 35 P 36
E
