#cell1 na2p_y idps_symb compose *
# 07-Oct-98 12:13 07-Oct-98 12:13 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 11 29 53
B 5 11 29 53
F F
C i0 26 53 metal2 2 1 * * N i0
C f 20 53 metal2 2 2 * * N f
C i1 14 53 metal2 2 3 * * N i1
C i0 26 11 metal2 2 4 * * S i0
C f 20 11 metal2 2 5 * * S f
C i1 14 11 metal2 2 6 * * S i1
C vdd 29 51 metal 8 7 * * E vdd
C vdd 5 51 metal 8 8 * * W vdd
C vss 5 13 metal 8 9 * * W vss
C vss 29 13 metal 8 10 * * E vss
I cbp1 8 20 0 "cbp" ly * *
I cdp1 14 47 0 "cdp" ly * *
I cbn1 8 48 0 "cbn" ly * *
I cbp2 8 15 0 "cbp" ly * *
I cdn1 14 24 0 "cdn" ly * *
I cdn2 14 18 0 "cdn" ly * *
I cdn3 26 22 0 "cdn" ly * *
I cdp2 14 41 0 "cdp" ly * *
I cdp3 20 42 0 "cdp" ly * *
I cdp4 20 36 0 "cdp" ly * *
I cdp5 26 41 0 "cdp" ly * *
I cpf1 15 30 0 "cpf" ly * *
I cpf2 25 30 0 "cpf" ly * *
I via12x1 14 36 0 "via12" ly * *
I via12x2 26 36 0 "via12" ly * *
I via12x3 20 30 0 "via12" ly * *
I cbn2 8 42 0 "cbn" ly * *
I cbn3 8 36 0 "cbn" ly * *
I cbp3 8 24 0 "cbp" ly * *
I cdp6 26 47 0 "cdp" ly * *
I cdp7 26 53 0 "cdp" ly * *
I cdp8 14 53 0 "cdp" ly * *
I cdn4 14 12 0 "cdn" ly * *
I cbp4 8 11 0 "cbp" ly * *
I cbn4 8 53 0 "cbn" ly * *
P 14 12 * metal
P 14 24 * metal
W 13 11 15 25 vss
S 1 V metal P 1 P 2
P 5 13 * metal
P 29 13 * metal
W -3 5 37 21 vss
S 8 H metal P 3 P 4
P 8 13 * metal
P 8 24 * metal
W 6 11 10 26 vss
S 2 V metal P 5 P 6
P 8 36 * metal
P 8 51 * metal
W 6 34 10 53 vdd
S 2 V metal P 7 P 8
P 8 51 * metal
P 8 53 * metal
W 7 50 9 54 vdd
S 1 V metal P 9 P 10
P 5 51 * metal
P 29 51 * metal
W -3 43 37 59 vdd
S 8 H metal P 11 P 12
P 14 41 * metal
P 14 53 * metal
W 13 40 15 54 vdd
S 1 V metal P 13 P 14
P 26 41 * metal
P 26 53 * metal
W 25 40 27 54 vdd
S 1 V metal P 15 P 16
P 14 35 * pdif
P 14 57 * pdif
W 11 32 17 60
S 3 V pdif P 17 P 18
P 26 35 * pdif
P 26 57 * pdif
W 23 32 29 60
S 3 V pdif P 19 P 20
P 25 30 * metal
P 26 30 * metal
W 24 29 27 31
S 1 H metal P 21 P 22
P 26 30 * metal
P 26 36 * metal
W 25 29 27 37
S 1 V metal P 23 P 24
P 8 11 * allowM2
P 8 53 * allowM2
W 6 9 10 55
S 2 V allowM2 P 25 P 26
P 14 30 * metal
P 15 30 * metal
W 13 29 16 31
S 1 H metal P 27 P 28
P 14 30 * metal
P 14 36 * metal
W 13 29 15 37
S 1 V metal P 29 P 30
P 26 22 * metal
P 26 25 * metal
W 24 20 28 27
S 2 V metal P 31 P 32
P 20 25 * metal
P 20 42 * metal
W 19 24 21 43
S 1 V metal P 33 P 34
P 20 25 * metal
P 26 25 * metal
W 19 24 27 26
S 1 H metal P 35 P 36
P 8 12 * ptie
P 8 24 * ptie
W 5 9 11 27
S 3 V ptie P 37 P 38
P 8 36 * ntie
P 8 51 * ntie
W 5 33 11 54
S 3 V ntie P 39 P 40
P 20 35 * pdif
P 20 57 * pdif
W 17 32 23 60
S 3 V pdif P 41 P 42
P 15 30 * poly
P 17 30 * poly
W 13 28 19 32
S 2 H poly P 43 P 44
P 17 28 * poly
P 17 33 * poly
W 16 27 18 34
S 1 V poly P 45 P 46
P 23 30 * poly
P 25 30 * poly
W 21 28 27 32
S 2 H poly P 47 P 48
P 23 28 * poly
P 23 33 * poly
W 22 27 24 34
S 1 V poly P 49 P 50
P 14 4 * ndif
P 14 26 * ndif
W 11 1 17 29
S 3 V ndif P 51 P 52
P 26 4 * ndif
P 26 26 * ndif
W 23 1 29 29
S 3 V ndif P 53 P 54
P 20 4 * ndif
P 20 26 * ndif
W 17 1 23 29
S 3 V ndif P 55 P 56
P 5 47 * nwell
P 29 47 * nwell
W -21 21 55 73
S 26 H nwell P 57 P 58
P 14 11 * metal2
P 14 53 * metal2
W 12 9 16 55 i1
S 2 V metal2 P 59 P 60
P 20 11 * metal2
P 20 53 * metal2
W 18 9 22 55 f
S 2 V metal2 P 61 P 62
P 26 11 * metal2
P 26 53 * metal2
W 24 9 28 55 i0
S 2 V metal2 P 63 P 64
P 23 2 * ntrans
P 23 28 * ntrans
W 22 1 24 29
S 1 V ntrans P 65 P 66
P 17 2 * ntrans
P 17 28 * ntrans
W 16 1 18 29
S 1 V ntrans P 67 P 68
P 23 33 * ptrans
P 23 59 * ptrans
W 22 32 24 60
S 1 V ptrans P 69 P 70
P 17 33 * ptrans
P 17 59 * ptrans
W 16 32 18 60
S 1 V ptrans P 71 P 72
P 8 11 * metal
P 8 13 * metal
W 7 10 9 14 vss
S 1 V metal P 73 P 74
E
