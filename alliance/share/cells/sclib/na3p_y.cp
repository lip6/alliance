#cell1 na3p_y idps_symb compose *
# 07-Oct-98 12:15 07-Oct-98 12:15 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 11 35 53
B 5 11 35 53
F F
C i0 26 11 metal2 2 1 * * S i0
C i1 20 11 metal2 2 2 * * S i1
C i2 14 11 metal2 2 3 * * S i2
C vss 35 13 metal 8 4 * * E vss
C vss 5 13 metal 8 5 * * W vss
C f 32 53 metal2 2 6 * * N f
C i0 26 53 metal2 2 7 * * N i0
C i1 20 53 metal2 2 8 * * N i1
C i2 14 53 metal2 2 9 * * N i2
C vdd 35 51 metal 8 10 * * E vdd
C vdd 5 51 metal 8 11 * * W vdd
C f 32 11 metal2 2 12 * * S f
I cbn1 8 49 0 "cbn" ly * *
I cbp1 8 19 0 "cbp" ly * *
I cbp2 8 23 0 "cbp" ly * *
I cdn1 14 22 0 "cdn" ly * *
I cdn2 14 18 0 "cdn" ly * *
I cdn3 32 21 0 "cdn" ly * *
I cdp1 14 49 0 "cdp" ly * *
I cdp2 14 41 0 "cdp" ly * *
I cdp3 20 36 0 "cdp" ly * *
I cdp4 20 42 0 "cdp" ly * *
I cdp5 26 41 0 "cdp" ly * *
I cdp6 32 42 0 "cdp" ly * *
I cdp7 32 36 0 "cdp" ly * *
I cpf1 15 30 0 "cpf" ly * *
I cpf2 21 30 0 "cpf" ly * *
I cpf3 27 30 0 "cpf" ly * *
I via12x1 20 25 0 "via12" ly * *
I via12x2 26 25 0 "via12" ly * *
I via12x3 32 30 0 "via12" ly * *
I via12x4 14 36 0 "via12" ly * *
I cbn2 8 53 0 "cbn" ly * *
I cbn3 8 41 0 "cbn" ly * *
I cbp3 8 11 0 "cbp" ly * *
I cdn4 14 10 0 "cdn" ly * *
I cdp8 14 53 0 "cdp" ly * *
I cdp9 26 53 0 "cdp" ly * *
I cdp10 14 45 0 "cdp" ly * *
I cdp11 26 45 0 "cdp" ly * *
I cdn5 32 25 0 "cdn" ly * *
I cdn6 14 14 0 "cdn" ly * *
I cbn4 8 45 0 "cbn" ly * *
I cbp4 8 15 0 "cbp" ly * *
I cdp12 26 49 0 "cdp" ly * *
P 14 4 * ndif
P 14 26 * ndif
W 11 1 17 29
S 3 V ndif P 1 P 2
P 14 30 * metal
P 14 36 * metal
W 13 29 15 37
S 1 V metal P 3 P 4
P 14 30 * metal
P 15 30 * metal
W 13 29 16 31
S 1 H metal P 5 P 6
P 26 30 * metal
P 27 30 * metal
W 25 29 28 31
S 1 H metal P 7 P 8
P 26 25 * metal
P 26 30 * metal
W 25 24 27 31
S 1 V metal P 9 P 10
P 20 30 * metal
P 21 30 * metal
W 19 29 22 31
S 1 H metal P 11 P 12
P 20 25 * metal
P 20 30 * metal
W 19 24 21 31
S 1 V metal P 13 P 14
P 26 41 * metal
P 26 53 * metal
W 24 39 28 55 vdd
S 2 V metal P 15 P 16
P 14 51 * metal
P 14 53 * metal
W 13 50 15 54 vdd
S 1 V metal P 17 P 18
P 14 41 * metal
P 14 51 * metal
W 12 39 16 53 vdd
S 2 V metal P 19 P 20
P 5 51 * metal
P 35 51 * metal
W -3 43 43 59 vdd
S 8 H metal P 21 P 22
P 8 41 * metal
P 8 51 * metal
W 6 39 10 53 vdd
S 2 V metal P 23 P 24
P 8 51 * metal
P 8 53 * metal
W 7 50 9 54 vdd
S 1 V metal P 25 P 26
P 8 11 * allowM2
P 8 53 * allowM2
W 6 9 10 55
S 2 V allowM2 P 27 P 28
P 8 11 * ptie
P 8 23 * ptie
W 5 8 11 26
S 3 V ptie P 29 P 30
P 8 41 * ntie
P 8 53 * ntie
W 5 38 11 56
S 3 V ntie P 31 P 32
P 14 13 * metal
P 14 22 * metal
W 12 11 16 24 vss
S 2 V metal P 33 P 34
P 14 10 * metal
P 14 13 * metal
W 13 9 15 14 vss
S 1 V metal P 35 P 36
P 8 13 * metal
P 8 23 * metal
W 6 11 10 25 vss
S 2 V metal P 37 P 38
P 8 11 * metal
P 8 13 * metal
W 7 10 9 14 vss
S 1 V metal P 39 P 40
P 5 13 * metal
P 35 13 * metal
W -3 5 43 21 vss
S 8 H metal P 41 P 42
P 20 36 * metal
P 20 42 * metal
W 18 34 22 44
S 2 V metal P 43 P 44
P 20 35 * pdif
P 20 57 * pdif
W 17 32 23 60
S 3 V pdif P 45 P 46
P 32 35 * pdif
P 32 57 * pdif
W 29 32 35 60
S 3 V pdif P 47 P 48
P 15 30 * poly
P 17 30 * poly
W 13 28 19 32
S 2 H poly P 49 P 50
P 17 28 * poly
P 17 33 * poly
W 16 27 18 34
S 1 V poly P 51 P 52
P 21 30 * poly
P 23 30 * poly
W 19 28 25 32
S 2 H poly P 53 P 54
P 23 28 * poly
P 23 33 * poly
W 22 27 24 34
S 1 V poly P 55 P 56
P 27 30 * poly
P 29 30 * poly
W 25 28 31 32
S 2 H poly P 57 P 58
P 29 28 * poly
P 29 33 * poly
W 28 27 30 34
S 1 V poly P 59 P 60
P 20 4 * ndif
P 20 26 * ndif
W 17 1 23 29
S 3 V ndif P 61 P 62
P 26 4 * ndif
P 26 26 * ndif
W 23 1 29 29
S 3 V ndif P 63 P 64
P 32 4 * ndif
P 32 26 * ndif
W 29 1 35 29
S 3 V ndif P 65 P 66
P 26 35 * pdif
P 26 57 * pdif
W 23 32 29 60
S 3 V pdif P 67 P 68
P 14 35 * pdif
P 14 57 * pdif
W 11 32 17 60
S 3 V pdif P 69 P 70
P 14 11 * metal2
P 14 53 * metal2
W 12 9 16 55
S 2 V metal2 P 71 P 72
P 20 11 * metal2
P 20 53 * metal2
W 18 9 22 55
S 2 V metal2 P 73 P 74
P 26 11 * metal2
P 26 53 * metal2
W 24 9 28 55
S 2 V metal2 P 75 P 76
P 29 2 * ntrans
P 29 28 * ntrans
W 28 1 30 29
S 1 V ntrans P 77 P 78
P 23 2 * ntrans
P 23 28 * ntrans
W 22 1 24 29
S 1 V ntrans P 79 P 80
P 17 2 * ntrans
P 17 28 * ntrans
W 16 1 18 29
S 1 V ntrans P 81 P 82
P 29 33 * ptrans
P 29 59 * ptrans
W 28 32 30 60
S 1 V ptrans P 83 P 84
P 23 33 * ptrans
P 23 59 * ptrans
W 22 32 24 60
S 1 V ptrans P 85 P 86
P 17 33 * ptrans
P 17 59 * ptrans
W 16 32 18 60
S 1 V ptrans P 87 P 88
P 5 47 * nwell
P 35 47 * nwell
W -21 21 61 73
S 26 H nwell P 89 P 90
P 32 11 * metal2
P 32 53 * metal2
W 30 9 34 55
S 2 V metal2 P 91 P 92
P 20 36 * metal
P 32 36 * metal
W 19 35 33 37
S 1 H metal P 93 P 94
P 32 21 * metal
P 32 42 * metal
W 30 19 34 44
S 2 V metal P 95 P 96
E
