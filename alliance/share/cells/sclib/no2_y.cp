#cell1 no2_y idps_symb compose *
# 07-Oct-98 12:19 07-Oct-98 12:19 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 29 45
B 5 3 29 45
F F
C vdd 5 43 metal 8 1 * * W vdd
C vss 5 5 metal 8 2 * * W vss
C f 26 3 metal2 2 3 * * S f
C i0 20 3 metal2 2 4 * * S i0
C i1 14 3 metal2 2 5 * * S i1
C f 26 45 metal2 2 6 * * N f
C i0 20 45 metal2 2 7 * * N i0
C i1 14 45 metal2 2 8 * * N i1
C vdd 29 43 metal 8 9 * * E vdd
C vss 29 5 metal 8 10 * * E vss
I cbp1 26 3 0 "cbp" ly * *
I cbp2 20 3 0 "cbp" ly * *
I cbp3 14 3 0 "cbp" ly * *
I cbp4 8 7 0 "cbp" ly * *
I cbn1 8 33 0 "cbn" ly * *
I cbn2 8 37 0 "cbn" ly * *
I cbn3 8 28 0 "cbn" ly * *
I cbn4 8 41 0 "cbn" ly * *
I cbn5 8 45 0 "cbn" ly * *
I cbp5 8 17 0 "cbp" ly * *
I cbp6 8 13 0 "cbp" ly * *
I cbp7 8 3 0 "cbp" ly * *
I cdn1 14 17 0 "cdn" ly * *
I cdn2 14 11 0 "cdn" ly * *
I cdn3 26 11 0 "cdn" ly * *
I cdn4 20 17 0 "cdn" ly * *
I cdp1 14 33 0 "cdp" ly * *
I cdp2 14 39 0 "cdp" ly * *
I cdp3 14 45 0 "cdp" ly * *
I cdp4 26 34 0 "cdp" ly * *
I cdp5 26 28 0 "cdp" ly * *
I cpf1 14 23 0 "cpf" ly * *
I cpf2 21 23 0 "cpf" ly * *
I via12x1 14 28 0 "via12" ly * *
I via12x2 20 28 0 "via12" ly * *
I via12x3 26 22 0 "via12" ly * *
P 20 17 * metal
P 26 17 * metal
W 18 15 28 19
S 2 H metal P 1 P 2
P 26 17 * metal
P 26 34 * metal
W 24 15 28 36
S 2 V metal P 3 P 4
P 14 23 * poly
P 17 23 * poly
W 11 20 20 26
S 3 H poly P 5 P 6
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 7 P 8
P 21 23 * poly
P 23 23 * poly
W 19 21 25 25
S 2 H poly P 9 P 10
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 11 P 12
P 26 5 * metal
P 26 11 * metal
W 24 3 28 13 vss
S 2 V metal P 13 P 14
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 15 P 16
P 14 5 * metal
P 14 17 * metal
W 12 3 16 19 vss
S 2 V metal P 17 P 18
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 19 P 20
P 14 33 * metal
P 14 45 * metal
W 12 31 16 47 vdd
S 2 V metal P 21 P 22
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 23 P 24
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 25 P 26
P 20 23 * metal
P 20 28 * metal
W 18 21 22 30 i0
S 2 V metal P 27 P 28
P 20 23 * metal
P 21 23 * metal
W 18 21 23 25 i0
S 2 H metal P 29 P 30
P 14 23 * metal
P 14 28 * metal
W 12 21 16 30
S 2 V metal P 31 P 32
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 i1
S 2 V metal2 P 33 P 34
P 8 28 * ntie
P 8 43 * ntie
W 5 25 11 46
S 3 V ntie P 35 P 36
P 8 5 * ptie
P 8 17 * ptie
W 5 2 11 20
S 3 V ptie P 37 P 38
P 20 13 * ndif
P 20 18 * ndif
W 17 10 23 21
S 3 V ndif P 39 P 40
P 14 11 * ndif
P 14 18 * ndif
W 11 8 17 21
S 3 V ndif P 41 P 42
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 f
S 2 V metal2 P 43 P 44
P 26 11 * ndif
P 26 18 * ndif
W 23 8 29 21
S 3 V ndif P 45 P 46
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 47 P 48
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 49 P 50
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 51 P 52
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 53 P 54
P 23 11 * ntrans
P 23 20 * ntrans
W 22 10 24 21
S 1 V ntrans P 55 P 56
P 17 11 * ntrans
P 17 20 * ntrans
W 16 10 18 21
S 1 V ntrans P 57 P 58
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 59 P 60
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 61 P 62
P 8 3 * ptie
P 26 3 * ptie
W 6 1 28 5
S 2 H ptie P 63 P 64
P 8 28 * metal
P 8 45 * metal
W 6 26 10 47 vdd
S 2 V metal P 65 P 66
P 8 3 * metal
P 26 3 * metal
W 6 1 28 5 vss
S 2 H metal P 67 P 68
P 8 3 * metal
P 8 17 * metal
W 6 1 10 19 vss
S 2 V metal P 69 P 70
E
