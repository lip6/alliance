#cell1 np1_y idps_symb compose *
# 07-Oct-98 12:24 07-Oct-98 12:24 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 23 45
B 5 3 23 45
F F
C vdd 23 43 metal 8 1 * * E vdd
C vss 23 5 metal 8 2 * * E vss
C f 8 45 metal2 2 3 * * N f
C i 14 45 metal2 2 4 * * N i
C f 8 3 metal2 2 5 * * S f
C i 14 3 metal2 2 6 * * S i
C vdd 5 43 metal 8 7 * * W vdd
C vss 5 5 metal 8 8 * * W vss
I cbn1 20 28 0 "cbn" ly * *
I cbp1 20 8 0 "cbp" ly * *
I cdn1 14 13 0 "cdn" ly * *
I cdn2 8 13 0 "cdn" ly * *
I cbn2 20 45 0 "cbn" ly * *
I cbn3 20 39 0 "cbn" ly * *
I cbn4 20 33 0 "cbn" ly * *
I cbp2 20 3 0 "cbp" ly * *
I cbp3 20 13 0 "cbp" ly * *
I cbp4 20 17 0 "cbp" ly * *
I cdn3 8 17 0 "cdn" ly * *
I cdn4 14 17 0 "cdn" ly * *
I cdp1 14 39 0 "cdp" ly * *
I cdp2 14 33 0 "cdp" ly * *
I cdp3 8 34 0 "cdp" ly * *
I cdp4 8 28 0 "cdp" ly * *
I cpf1 13 22 0 "cpf" ly * *
I via12x1 8 22 0 "via12" ly * *
I via12x2 14 28 0 "via12" ly * *
I cdp5 14 45 0 "cdp" ly * *
I cdn5 14 8 0 "cdn" ly * *
P 8 17 * metal
P 8 29 * metal
W 7 16 9 30
S 1 V metal P 1 P 2
P 8 29 * metal
P 8 34 * metal
W 6 27 10 36
S 2 V metal P 3 P 4
P 5 5 * metal
P 23 5 * metal
W -3 -3 31 13 vss
S 8 H metal P 5 P 6
P 14 33 * metal
P 14 45 * metal
W 12 31 16 47 vdd
S 2 V metal P 7 P 8
P 5 43 * metal
P 23 43 * metal
W -3 35 31 51 vdd
S 8 H metal P 9 P 10
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 11 P 12
P 13 22 * metal
P 14 22 * metal
W 12 21 15 23
S 1 H metal P 13 P 14
P 14 22 * metal
P 14 28 * metal
W 13 21 15 29
S 1 V metal P 15 P 16
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 i
S 2 V metal2 P 17 P 18
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 19 P 20
P 11 22 * poly
P 13 22 * poly
W 9 20 15 24
S 2 H poly P 21 P 22
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 23 P 24
P 8 27 * pdif
P 8 49 * pdif
W 5 24 11 52
S 3 V pdif P 25 P 26
P 20 5 * ptie
P 20 16 * ptie
W 17 2 23 19
S 3 V ptie P 27 P 28
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 f
S 2 V metal2 P 29 P 30
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 31 P 32
P 8 7 * ndif
P 8 18 * ndif
W 5 4 11 21
S 3 V ndif P 33 P 34
P 11 5 * ntrans
P 11 20 * ntrans
W 10 4 12 21
S 1 V ntrans P 35 P 36
P 11 25 * ptrans
P 11 51 * ptrans
W 10 24 12 52
S 1 V ptrans P 37 P 38
P 5 39 * nwell
P 23 39 * nwell
W -21 13 49 65
S 26 H nwell P 39 P 40
P 20 29 * ntie
P 20 45 * ntie
W 17 26 23 48
S 3 V ntie P 41 P 42
P 8 13 * metal
P 8 17 * metal
W 6 11 10 19
S 2 V metal P 43 P 44
P 14 5 * metal
P 14 17 * metal
W 12 3 16 19 vss
S 2 V metal P 45 P 46
P 20 28 * metal
P 20 45 * metal
W 18 26 22 47 vdd
S 2 V metal P 47 P 48
P 20 3 * metal
P 20 17 * metal
W 18 1 22 19 vss
S 2 V metal P 49 P 50
E
