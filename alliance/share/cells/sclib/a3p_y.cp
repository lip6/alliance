#cell1 a3p_y idps_symb compose *
# 07-Oct-98 12:03 07-Oct-98 12:03 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 35 45
B 5 3 35 45
F F
C t 8 45 metal2 2 1 * * N t
C t 8 3 metal2 2 2 * * S t
C i2 26 3 metal2 2 3 * * S i2
C i2 26 45 metal2 2 4 * * N i2
C i1 20 45 metal2 2 5 * * N i1
C i1 20 3 metal2 2 6 * * S i1
C i0 14 45 metal2 2 7 * * N i0
C i0 14 3 metal2 2 8 * * S i0
C vss 35 5 metal 8 9 * * E vss
C vss 5 5 metal 8 10 * * W vss
C vdd 5 43 metal 8 11 * * W vdd
C vdd 35 43 metal 8 12 * * E vdd
I cbp1 8 2 0 "cbp" ly * *
I cdp1 8 35 0 "cdp" ly * *
I cdn1 14 12 0 "cdn" ly * *
I cdn2 8 17 0 "cdn" ly * *
I cpf1 23 23 0 "cpf" ly * *
I cpf2 12 23 0 "cpf" ly * *
I cdp2 14 45 0 "cdp" ly * *
I via12x1 14 17 0 "via12" ly * *
I via12x2 8 31 0 "via12" ly * *
I cdp3 8 28 0 "cdp" ly * *
I cdn3 8 13 0 "cdn" ly * *
I cbn1 20 45 0 "cbn" ly * *
I cbp2 14 2 0 "cbp" ly * *
I cbp3 32 2 0 "cbp" ly * *
I cbn2 32 45 0 "cbn" ly * *
I via12x3 26 13 0 "via12" ly * *
I via12x4 20 13 0 "via12" ly * *
I cpf3 28 23 0 "cpf" ly * *
I cpf4 18 23 0 "cpf" ly * *
I cdp4 32 28 0 "cdp" ly * *
I cdp5 32 34 0 "cdp" ly * *
I cdp6 26 33 0 "cdp" ly * *
I cdp7 26 39 0 "cdp" ly * *
I cdp8 20 34 0 "cdp" ly * *
I cdp9 20 28 0 "cdp" ly * *
I cdp10 14 33 0 "cdp" ly * *
I cdp11 14 39 0 "cdp" ly * *
I cdn4 32 13 0 "cdn" ly * *
I cdn5 14 8 0 "cdn" ly * *
I cbp4 26 2 0 "cbp" ly * *
I cbp5 20 2 0 "cbp" ly * *
I cbn3 26 45 0 "cbn" ly * *
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 1 P 2
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 3 P 4
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 5 P 6
P 11 5 * ntrans
P 11 20 * ntrans
W 10 4 12 21
S 1 V ntrans P 7 P 8
P 8 2 * ptie
P 32 2 * ptie
W 6 0 34 4
S 2 H ptie P 9 P 10
P 20 45 * ntie
P 32 45 * ntie
W 18 43 34 47
S 2 H ntie P 11 P 12
P 20 27 * pdif
P 20 38 * pdif
W 17 24 23 41
S 3 V pdif P 13 P 14
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 15 P 16
P 11 23 * poly
P 12 23 * poly
W 10 22 13 24
S 1 H poly P 17 P 18
P 8 7 * ndif
P 8 18 * ndif
W 5 4 11 21
S 3 V ndif P 19 P 20
P 11 25 * ptrans
P 11 51 * ptrans
W 10 24 12 52
S 1 V ptrans P 21 P 22
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 23 P 24
P 32 27 * pdif
P 32 38 * pdif
W 29 24 35 41
S 3 V pdif P 25 P 26
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 27 P 28
P 32 7 * ndif
P 32 18 * ndif
W 29 4 35 21
S 3 V ndif P 29 P 30
P 29 5 * ntrans
P 29 20 * ntrans
W 28 4 30 21
S 1 V ntrans P 31 P 32
P 23 5 * ntrans
P 23 20 * ntrans
W 22 4 24 21
S 1 V ntrans P 33 P 34
P 17 5 * ntrans
P 17 20 * ntrans
W 16 4 18 21
S 1 V ntrans P 35 P 36
P 29 25 * ptrans
P 29 40 * ptrans
W 28 24 30 41
S 1 V ptrans P 37 P 38
P 23 25 * ptrans
P 23 40 * ptrans
W 22 24 24 41
S 1 V ptrans P 39 P 40
P 17 25 * ptrans
P 17 40 * ptrans
W 16 24 18 41
S 1 V ptrans P 41 P 42
P 5 39 * nwell
P 35 39 * nwell
W -21 13 61 65
S 26 H nwell P 43 P 44
P 26 27 * pdif
P 26 39 * pdif
W 23 24 29 42
S 3 V pdif P 45 P 46
P 8 27 * pdif
P 8 49 * pdif
W 5 24 11 52
S 3 V pdif P 47 P 48
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 49 P 50
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 i0
S 2 V metal2 P 51 P 52
P 26 7 * ndif
P 26 18 * ndif
W 23 4 29 21
S 3 V ndif P 53 P 54
P 5 43 * metal
P 35 43 * metal
W -3 35 43 51 vdd
S 8 H metal P 55 P 56
P 5 5 * metal
P 35 5 * metal
W -3 -3 43 13 vss
S 8 H metal P 57 P 58
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 59 P 60
P 29 20 * poly
P 29 25 * poly
W 28 19 30 26
S 1 V poly P 61 P 62
P 28 23 * poly
P 29 23 * poly
W 27 22 30 24
S 1 H poly P 63 P 64
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 65 P 66
P 8 13 * metal
P 8 16 * metal
W 6 11 10 18
S 2 V metal P 67 P 68
P 8 16 * metal
P 8 29 * metal
W 7 15 9 30
S 1 V metal P 69 P 70
P 8 29 * metal
P 8 35 * metal
W 6 27 10 37
S 2 V metal P 71 P 72
P 17 23 * poly
P 18 23 * poly
W 16 22 19 24
S 1 H poly P 73 P 74
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 75 P 76
P 22 23 * metal
P 23 23 * metal
W 21 22 24 24
S 1 H metal P 77 P 78
P 22 13 * metal
P 22 23 * metal
W 21 12 23 24
S 1 V metal P 79 P 80
P 20 13 * metal
P 22 13 * metal
W 18 11 24 15
S 2 H metal P 81 P 82
P 32 13 * metal
P 32 28 * metal
W 31 12 33 29
S 1 V metal P 83 P 84
P 32 28 * metal
P 32 34 * metal
W 30 26 34 36
S 2 V metal P 85 P 86
P 13 28 * metal
P 32 28 * metal
W 12 27 33 29
S 1 H metal P 87 P 88
P 13 23 * metal
P 13 28 * metal
W 12 22 14 29
S 1 V metal P 89 P 90
P 12 23 * metal
P 13 23 * metal
W 11 22 14 24
S 1 H metal P 91 P 92
P 20 28 * metal
P 20 34 * metal
W 18 26 22 36
S 2 V metal P 93 P 94
P 15 17 * metal
P 15 18 * metal
W 14 16 16 19
S 1 V metal P 95 P 96
P 15 18 * metal
P 18 18 * metal
W 14 17 19 19
S 1 H metal P 97 P 98
P 18 18 * metal
P 18 23 * metal
W 17 17 19 24
S 1 V metal P 99 P 100
P 14 17 * metal
P 15 17 * metal
W 13 16 16 18
S 1 H metal P 101 P 102
P 26 13 * metal
P 28 13 * metal
W 24 11 30 15
S 2 H metal P 103 P 104
P 28 13 * metal
P 28 23 * metal
W 27 12 29 24
S 1 V metal P 105 P 106
P 14 33 * metal
P 14 45 * metal
W 12 31 16 47 vdd
S 2 V metal P 107 P 108
P 26 33 * metal
P 26 45 * metal
W 24 31 28 47 vdd
S 2 V metal P 109 P 110
P 20 45 * metal
P 32 45 * metal
W 18 43 34 47 vdd
S 2 H metal P 111 P 112
P 8 2 * metal
P 32 2 * metal
W 6 0 34 4 vss
S 2 H metal P 113 P 114
P 14 2 * metal
P 14 12 * metal
W 12 0 16 14 vss
S 2 V metal P 115 P 116
E
