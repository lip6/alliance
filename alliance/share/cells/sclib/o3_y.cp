#cell1 o3_y idps_symb compose *
# 07-Oct-98 12:26 07-Oct-98 12:26 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 35 45
B 5 3 35 45
F F
C vdd 35 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 35 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
C t 8 45 metal2 2 5 * * N t
C t 8 3 metal2 2 6 * * S t
C i0 14 45 metal2 2 7 * * N i0
C i1 20 45 metal2 2 8 * * N i1
C i2 26 45 metal2 2 9 * * N i2
C i0 14 3 metal2 2 10 * * S i0
C i1 20 3 metal2 2 11 * * S i1
C i2 26 3 metal2 2 12 * * S i2
I cbp1 26 3 0 "cbp" ly * *
I cbp2 14 3 0 "cbp" ly * *
I cbp3 8 3 0 "cbp" ly * *
I cdn1 14 16 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdn3 26 13 0 "cdn" ly * *
I cdn4 32 13 0 "cdn" ly * *
I cdp1 14 41 0 "cdp" ly * *
I cdp2 14 37 0 "cdp" ly * *
I cdp3 14 33 0 "cdp" ly * *
I cdp4 32 35 0 "cdp" ly * *
I via12x1 14 28 0 "via12" ly * *
I cdn5 14 12 0 "cdn" ly * *
I cdn6 8 14 0 "cdn" ly * *
I cdp5 8 28 0 "cdp" ly * *
I via12x2 8 19 0 "via12" ly * *
I cpf1 13 21 0 "cpf" ly * *
I cbn1 8 45 0 "cbn" ly * *
I cbp4 32 3 0 "cbp" ly * *
I cdp6 8 34 0 "cdp" ly * *
I cbp5 20 3 0 "cbp" ly * *
I cpf2 18 24 0 "cpf" ly * *
I cpf3 23 24 0 "cpf" ly * *
I cpf4 29 24 0 "cpf" ly * *
I via12x3 20 34 0 "via12" ly * *
I via12x4 26 30 0 "via12" ly * *
I cdp7 32 31 0 "cdp" ly * *
P 20 33 * metal
P 20 34 * metal
W 19 32 21 35
S 1 V metal P 1 P 2
P 20 33 * metal
P 22 33 * metal
W 19 32 23 34
S 1 H metal P 3 P 4
P 18 24 * metal
P 18 28 * metal
W 16 22 20 30
S 2 V metal P 5 P 6
P 14 28 * metal
P 18 28 * metal
W 12 26 20 30
S 2 H metal P 7 P 8
P 32 13 * metal
P 33 13 * metal
W 31 12 34 14
S 1 H metal P 9 P 10
P 33 13 * metal
P 33 31 * metal
W 32 12 34 32
S 1 V metal P 11 P 12
P 13 20 * metal
P 33 20 * metal
W 12 19 34 21
S 1 H metal P 13 P 14
P 32 31 * metal
P 33 31 * metal
W 31 30 34 32
S 1 H metal P 15 P 16
P 32 31 * metal
P 32 35 * metal
W 30 29 34 37
S 2 V metal P 17 P 18
P 26 5 * metal
P 26 13 * metal
W 24 3 28 15 vss
S 2 V metal P 19 P 20
P 5 5 * metal
P 35 5 * metal
W -3 -3 43 13 vss
S 8 H metal P 21 P 22
P 8 43 * metal
P 8 45 * metal
W 6 41 10 47 vdd
S 2 V metal P 23 P 24
P 5 43 * metal
P 35 43 * metal
W -3 35 43 51 vdd
S 8 H metal P 25 P 26
P 14 33 * metal
P 14 43 * metal
W 12 31 16 45 vdd
S 2 V metal P 27 P 28
P 8 3 * metal
P 8 5 * metal
W 6 1 10 7 vss
S 2 V metal P 29 P 30
P 14 5 * metal
P 14 16 * metal
W 12 3 16 18 vss
S 2 V metal P 31 P 32
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 33 P 34
P 13 20 * metal
P 13 21 * metal
W 11 18 15 23
S 2 V metal P 35 P 36
P 19 13 * metal
P 19 20 * metal
W 18 12 20 21
S 1 V metal P 37 P 38
P 19 13 * metal
P 20 13 * metal
W 18 12 21 14
S 1 H metal P 39 P 40
P 28 24 * metal
P 29 24 * metal
W 27 23 30 25
S 1 H metal P 41 P 42
P 28 24 * metal
P 28 29 * metal
W 27 23 29 30
S 1 V metal P 43 P 44
P 26 29 * metal
P 28 29 * metal
W 25 28 29 30
S 1 H metal P 45 P 46
P 26 29 * metal
P 26 30 * metal
W 25 28 27 31
S 1 V metal P 47 P 48
P 22 24 * metal
P 23 24 * metal
W 21 23 24 25
S 1 H metal P 49 P 50
P 22 24 * metal
P 22 33 * metal
W 21 23 23 34
S 1 V metal P 51 P 52
P 17 10 * ntrans
P 17 17 * ntrans
W 16 9 18 18
S 1 V ntrans P 53 P 54
P 23 10 * ntrans
P 23 17 * ntrans
W 22 9 24 18
S 1 V ntrans P 55 P 56
P 29 10 * ntrans
P 29 17 * ntrans
W 28 9 30 18
S 1 V ntrans P 57 P 58
P 17 25 * ptrans
P 17 44 * ptrans
W 16 24 18 45
S 1 V ptrans P 59 P 60
P 17 24 * poly
P 18 24 * poly
W 15 22 20 26
S 2 H poly P 61 P 62
P 17 17 * poly
P 17 25 * poly
W 16 16 18 26
S 1 V poly P 63 P 64
P 11 21 * poly
P 13 21 * poly
W 9 19 15 23
S 2 H poly P 65 P 66
P 11 19 * poly
P 11 25 * poly
W 10 18 12 26
S 1 V poly P 67 P 68
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 69 P 70
P 26 12 * ndif
P 26 15 * ndif
W 23 9 29 18
S 3 V ndif P 71 P 72
P 8 12 * ndif
P 8 17 * ndif
W 5 9 11 20
S 3 V ndif P 73 P 74
P 8 14 * metal
P 8 34 * metal
W 6 12 10 36
S 2 V metal P 75 P 76
P 32 12 * ndif
P 32 15 * ndif
W 30 10 34 17
S 2 V ndif P 77 P 78
P 20 12 * ndif
P 20 15 * ndif
W 17 9 23 18
S 3 V ndif P 79 P 80
P 5 39 * nwell
P 35 39 * nwell
W -21 13 61 65
S 26 H nwell P 81 P 82
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 83 P 84
P 14 27 * pdif
P 14 42 * pdif
W 11 24 17 45
S 3 V pdif P 85 P 86
P 14 11 * ndif
P 14 16 * ndif
W 12 9 16 18
S 2 V ndif P 87 P 88
P 11 10 * ntrans
P 11 19 * ntrans
W 10 9 12 20
S 1 V ntrans P 89 P 90
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 91 P 92
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 93 P 94
P 23 17 * poly
P 23 25 * poly
W 22 16 24 26
S 1 V poly P 95 P 96
P 29 17 * poly
P 29 25 * poly
W 28 16 30 26
S 1 V poly P 97 P 98
P 20 27 * pdif
P 20 42 * pdif
W 17 24 23 45
S 3 V pdif P 99 P 100
P 26 27 * pdif
P 26 42 * pdif
W 23 24 29 45
S 3 V pdif P 101 P 102
P 32 27 * pdif
P 32 42 * pdif
W 29 24 35 45
S 3 V pdif P 103 P 104
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 105 P 106
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 107 P 108
P 29 25 * ptrans
P 29 44 * ptrans
W 28 24 30 45
S 1 V ptrans P 109 P 110
P 23 25 * ptrans
P 23 44 * ptrans
W 22 24 24 45
S 1 V ptrans P 111 P 112
P 8 3 * ptie
P 32 3 * ptie
W 6 1 34 5
S 2 H ptie P 113 P 114
P 8 3 * metal
P 32 3 * metal
W 7 2 33 4
S 1 H metal P 115 P 116
E
