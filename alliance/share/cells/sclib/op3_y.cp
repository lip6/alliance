#cell1 op3_y idps_symb compose *
# 07-Oct-98 12:29 07-Oct-98 12:29 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 41 45
B 5 3 41 45
F F
C vdd 41 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 41 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
C t 14 45 metal2 2 5 * * N t
C t 14 3 metal2 2 6 * * S t
C i0 20 45 metal2 2 7 * * N i0
C i1 26 45 metal2 2 8 * * N i1
C i2 32 45 metal2 2 9 * * N i2
C i0 20 3 metal2 2 10 * * S i0
C i1 26 3 metal2 2 11 * * S i1
C i2 32 3 metal2 2 12 * * S i2
I cbp1 32 3 0 "cbp" ly * *
I cbp2 8 16 0 "cbp" ly * *
I cdn1 32 9 0 "cdn" ly * *
I cdn2 38 14 0 "cdn" ly * *
I cdp1 20 41 0 "cdp" ly * *
I cdp2 20 37 0 "cdp" ly * *
I cdp3 20 33 0 "cdp" ly * *
I cdp4 20 45 0 "cdp" ly * *
I cdp5 38 35 0 "cdp" ly * *
I cdp6 38 29 0 "cdp" ly * *
I cpf1 23 24 0 "cpf" ly * *
I cpf2 29 24 0 "cpf" ly * *
I cpf3 34 24 0 "cpf" ly * *
I via12x1 20 28 0 "via12" ly * *
I via12x2 26 29 0 "via12" ly * *
I via12x3 32 29 0 "via12" ly * *
I cdn3 20 7 0 "cdn" ly * *
I cdn4 14 13 0 "cdn" ly * *
I cdp7 14 31 0 "cdp" ly * *
I via12x4 14 16 0 "via12" ly * *
I cbn1 8 28 0 "cbn" ly * *
I cbp3 38 3 0 "cbp" ly * *
I cdp8 14 35 0 "cdp" ly * *
I cbp4 26 3 0 "cbp" ly * *
I cbn2 8 33 0 "cbn" ly * *
I cbn3 8 37 0 "cbn" ly * *
I cbn4 8 41 0 "cbn" ly * *
I cbn5 8 45 0 "cbn" ly * *
I cbp5 8 12 0 "cbp" ly * *
I cbp6 8 7 0 "cbp" ly * *
I cbp7 8 3 0 "cbp" ly * *
I cpf4 18 21 0 "cpf" ly * *
I cdn5 26 14 0 "cdn" ly * *
I cdn6 20 13 0 "cdn" ly * *
I cdn7 32 13 0 "cdn" ly * *
P 20 5 * metal
P 20 13 * metal
W 18 3 22 15 vss
S 2 V metal P 1 P 2
P 17 21 * poly
P 18 21 * poly
W 16 20 19 22
S 1 H poly P 3 P 4
P 17 19 * poly
P 17 25 * poly
W 16 18 18 26
S 1 V poly P 5 P 6
P 14 6 * ndif
P 14 17 * ndif
W 11 3 17 20
S 3 V ndif P 7 P 8
P 38 29 * metal
P 38 35 * metal
W 36 27 40 37
S 2 V metal P 9 P 10
P 18 20 * metal
P 18 21 * metal
W 17 19 19 22
S 1 V metal P 11 P 12
P 20 33 * metal
P 20 45 * metal
W 18 31 22 47 vdd
S 2 V metal P 13 P 14
P 5 43 * metal
P 41 43 * metal
W -3 35 49 51 vdd
S 8 H metal P 15 P 16
P 13 31 * metal
P 14 31 * metal
W 11 29 16 33
S 2 H metal P 17 P 18
P 14 31 * metal
P 14 35 * metal
W 12 29 16 37
S 2 V metal P 19 P 20
P 13 16 * metal
P 13 31 * metal
W 12 15 14 32
S 1 V metal P 21 P 22
P 13 16 * metal
P 14 16 * metal
W 11 14 16 18
S 2 H metal P 23 P 24
P 14 13 * metal
P 14 16 * metal
W 12 11 16 18
S 2 V metal P 25 P 26
P 20 6 * ndif
P 20 17 * ndif
W 17 3 23 20
S 3 V ndif P 27 P 28
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 29 P 30
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 31 P 32
P 32 29 * metal
P 33 29 * metal
W 31 28 34 30
S 1 H metal P 33 P 34
P 33 24 * metal
P 34 24 * metal
W 32 23 35 25
S 1 H metal P 35 P 36
P 33 24 * metal
P 33 29 * metal
W 32 23 34 30
S 1 V metal P 37 P 38
P 20 28 * metal
P 22 28 * metal
W 18 26 24 30
S 2 H metal P 39 P 40
P 22 24 * metal
P 22 28 * metal
W 21 23 23 29
S 1 V metal P 41 P 42
P 22 24 * metal
P 23 24 * metal
W 21 23 24 25
S 1 H metal P 43 P 44
P 34 24 * poly
P 35 24 * poly
W 33 23 36 25
S 1 H poly P 45 P 46
P 35 19 * poly
P 35 25 * poly
W 34 18 36 26
S 1 V poly P 47 P 48
P 38 12 * ndif
P 38 17 * ndif
W 35 9 41 20
S 3 V ndif P 49 P 50
P 8 3 * ptie
P 8 16 * ptie
W 5 0 11 19
S 3 V ptie P 51 P 52
P 28 24 * metal
P 29 24 * metal
W 27 23 30 25
S 1 H metal P 53 P 54
P 26 29 * metal
P 28 29 * metal
W 24 27 30 31
S 2 H metal P 55 P 56
P 28 24 * metal
P 28 29 * metal
W 27 23 29 30
S 1 V metal P 57 P 58
P 8 28 * ntie
P 8 45 * ntie
W 5 25 11 48
S 3 V ntie P 59 P 60
P 5 39 * nwell
P 41 39 * nwell
W -21 13 67 65
S 26 H nwell P 61 P 62
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 63 P 64
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 65 P 66
P 17 4 * ntrans
P 17 19 * ntrans
W 16 3 18 20
S 1 V ntrans P 67 P 68
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 69 P 70
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 71 P 72
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 73 P 74
P 23 19 * poly
P 23 25 * poly
W 22 18 24 26
S 1 V poly P 75 P 76
P 29 19 * poly
P 29 25 * poly
W 28 18 30 26
S 1 V poly P 77 P 78
P 32 9 * ndif
P 32 17 * ndif
W 29 6 35 20
S 3 V ndif P 79 P 80
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 81 P 82
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 83 P 84
P 38 27 * pdif
P 38 49 * pdif
W 35 24 41 52
S 3 V pdif P 85 P 86
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 87 P 88
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47
S 2 V metal2 P 89 P 90
P 35 10 * ntrans
P 35 19 * ntrans
W 34 9 36 20
S 1 V ntrans P 91 P 92
P 29 10 * ntrans
P 29 19 * ntrans
W 28 9 30 20
S 1 V ntrans P 93 P 94
P 23 10 * ntrans
P 23 19 * ntrans
W 22 9 24 20
S 1 V ntrans P 95 P 96
P 35 25 * ptrans
P 35 51 * ptrans
W 34 24 36 52
S 1 V ptrans P 97 P 98
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 99 P 100
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 101 P 102
P 25 3 * ptie
P 38 3 * ptie
W 23 1 40 5
S 2 H ptie P 103 P 104
P 38 14 * metal
P 38 29 * metal
W 37 13 39 30
S 1 V metal P 105 P 106
P 26 12 * ndif
P 26 17 * ndif
W 23 9 29 20
S 3 V ndif P 107 P 108
P 18 20 * metal
P 38 20 * metal
W 17 19 39 21
S 1 H metal P 109 P 110
P 26 14 * metal
P 26 20 * metal
W 25 13 27 21
S 1 V metal P 111 P 112
P 8 28 * metal
P 8 45 * metal
W 6 26 10 47 vdd
S 2 V metal P 113 P 114
P 8 3 * metal
P 8 16 * metal
W 6 1 10 18 vss
S 2 V metal P 115 P 116
P 26 3 * metal
P 38 3 * metal
W 24 1 40 5 vss
S 2 H metal P 117 P 118
P 26 3 * metal
P 26 5 * metal
W 25 2 27 6 vss
S 1 V metal P 119 P 120
P 5 5 * metal
P 41 5 * metal
W -3 -3 49 13 vss
S 8 H metal P 121 P 122
P 32 5 * metal
P 32 13 * metal
W 30 3 34 15 vss
S 2 V metal P 123 P 124
E
