#cell1 nop3_y idps_symb compose *
# 07-Oct-98 12:23 07-Oct-98 12:23 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 47 45
B 5 3 47 45
F F
C vdd 5 43 metal 8 1 * * W vdd
C vss 5 5 metal 8 2 * * W vss
C f 8 3 metal2 2 3 * * S f
C f 8 45 metal2 2 4 * * N f
C i0 38 3 metal2 2 5 * * S i0
C i0 38 45 metal2 2 6 * * N i0
C i1 32 3 metal2 2 7 * * S i1
C i1 32 45 metal2 2 8 * * N i1
C i2 26 3 metal2 2 9 * * S i2
C i2 26 45 metal2 2 10 * * N i2
C vdd 47 43 metal 8 11 * * E vdd
C vss 47 5 metal 8 12 * * E vss
I cbn1 20 43 0 "cbn" ly * *
I cbp1 20 3 0 "cbp" ly * *
I cdn1 20 13 0 "cdn" ly * *
I cdn2 26 13 0 "cdn" ly * *
I cdn3 26 9 0 "cdn" ly * *
I cdn4 38 13 0 "cdn" ly * *
I cdn5 38 9 0 "cdn" ly * *
I cdn6 44 13 0 "cdn" ly * *
I cdp1 26 41 0 "cdp" ly * *
I cdp2 26 37 0 "cdp" ly * *
I cdp3 26 33 0 "cdp" ly * *
I cdp4 26 45 0 "cdp" ly * *
I cdp5 44 34 0 "cdp" ly * *
I cdp6 44 28 0 "cdp" ly * *
I cpf1 27 23 0 "cpf" ly * *
I cpf2 33 23 0 "cpf" ly * *
I cpf3 39 23 0 "cpf" ly * *
I via12x1 26 28 0 "via12" ly * *
I via12x2 32 28 0 "via12" ly * *
I via12x3 38 28 0 "via12" ly * *
I via12x4 8 20 0 "via12" ly * *
I cdp7 20 28 0 "cdp" ly * *
I cdp8 20 34 0 "cdp" ly * *
I cdp9 14 28 0 "cdp" ly * *
I cdp10 14 32 0 "cdp" ly * *
I cdp11 14 37 0 "cdp" ly * *
I cdp12 14 41 0 "cdp" ly * *
I cdp13 14 45 0 "cdp" ly * *
I cdp14 8 28 0 "cdp" ly * *
I cdp15 8 34 0 "cdp" ly * *
I cdn7 8 17 0 "cdn" ly * *
I cdn8 8 13 0 "cdn" ly * *
I cdn9 14 17 0 "cdn" ly * *
I cdn10 14 13 0 "cdn" ly * *
I cdn11 14 8 0 "cdn" ly * *
I cbp2 26 3 0 "cbp" ly * *
I cbp3 32 3 0 "cbp" ly * *
I cbp4 38 3 0 "cbp" ly * *
I cbp5 44 3 0 "cbp" ly * *
I cpf4 13 22 0 "cpf" ly * *
I cpf5 23 18 0 "cpf" ly * *
I cdn12 32 13 0 "cdn" ly * *
P 14 43 * metal
P 14 45 * metal
W 13 42 15 46 vdd
S 1 V metal P 1 P 2
P 14 28 * metal
P 14 43 * metal
W 12 26 16 45 vdd
S 2 V metal P 3 P 4
P 5 43 * metal
P 47 43 * metal
W -3 35 55 51 vdd
S 8 H metal P 5 P 6
P 38 5 * metal
P 38 13 * metal
W 36 3 40 15 vss
S 2 V metal P 7 P 8
P 5 5 * metal
P 47 5 * metal
W -3 -3 55 13 vss
S 8 H metal P 9 P 10
P 44 13 * metal
P 44 34 * metal
W 43 12 45 35
S 1 V metal P 11 P 12
P 32 13 * metal
P 32 18 * metal
W 31 12 33 19
S 1 V metal P 13 P 14
P 23 18 * metal
P 44 18 * metal
W 22 17 45 19
S 1 H metal P 15 P 16
P 8 7 * ndif
P 8 18 * ndif
W 5 4 11 21
S 3 V ndif P 17 P 18
P 26 33 * metal
P 26 45 * metal
W 24 31 28 47 vdd
S 2 V metal P 19 P 20
P 20 28 * metal
P 20 34 * metal
W 18 26 22 36
S 2 V metal P 21 P 22
P 19 28 * metal
P 20 28 * metal
W 18 27 21 29
S 1 H metal P 23 P 24
P 13 22 * metal
P 19 22 * metal
W 12 21 20 23
S 1 H metal P 25 P 26
P 19 13 * metal
P 19 28 * metal
W 18 12 20 29
S 1 V metal P 27 P 28
P 19 13 * metal
P 20 13 * metal
W 18 12 21 14
S 1 H metal P 29 P 30
P 38 23 * metal
P 39 23 * metal
W 37 22 40 24
S 1 H metal P 31 P 32
P 38 23 * metal
P 38 28 * metal
W 37 22 39 29
S 1 V metal P 33 P 34
P 32 23 * metal
P 33 23 * metal
W 31 22 34 24
S 1 H metal P 35 P 36
P 32 23 * metal
P 32 28 * metal
W 31 22 33 29
S 1 V metal P 37 P 38
P 26 23 * metal
P 27 23 * metal
W 25 22 28 24
S 1 H metal P 39 P 40
P 26 23 * metal
P 26 28 * metal
W 25 22 27 29
S 1 V metal P 41 P 42
P 44 3 * allowM2
P 44 45 * allowM2
W 42 1 46 47
S 2 V allowM2 P 43 P 44
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 45 P 46
P 14 3 * allowM2
P 14 45 * allowM2
W 12 1 16 47
S 2 V allowM2 P 47 P 48
P 20 27 * pdif
P 20 38 * pdif
W 17 24 23 41
S 3 V pdif P 49 P 50
P 11 22 * poly
P 13 22 * poly
W 9 20 15 24
S 2 H poly P 51 P 52
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 53 P 54
P 27 23 * poly
P 29 23 * poly
W 25 21 31 25
S 2 H poly P 55 P 56
P 29 16 * poly
P 29 25 * poly
W 28 15 30 26
S 1 V poly P 57 P 58
P 33 23 * poly
P 35 23 * poly
W 31 21 37 25
S 2 H poly P 59 P 60
P 35 16 * poly
P 35 25 * poly
W 34 15 36 26
S 1 V poly P 61 P 62
P 39 23 * poly
P 41 23 * poly
W 37 21 43 25
S 2 H poly P 63 P 64
P 41 16 * poly
P 41 25 * poly
W 40 15 42 26
S 1 V poly P 65 P 66
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 67 P 68
P 8 27 * pdif
P 8 49 * pdif
W 5 24 11 52
S 3 V pdif P 69 P 70
P 20 9 * ndif
P 20 14 * ndif
W 17 6 23 17
S 3 V ndif P 71 P 72
P 44 9 * ndif
P 44 14 * ndif
W 41 6 47 17
S 3 V ndif P 73 P 74
P 38 8 * ndif
P 38 14 * ndif
W 35 5 41 17
S 3 V ndif P 75 P 76
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 77 P 78
P 26 8 * ndif
P 26 14 * ndif
W 23 5 29 17
S 3 V ndif P 79 P 80
P 14 8 * metal
P 14 17 * metal
W 12 6 16 19
S 2 V metal P 81 P 82
P 23 16 * poly
P 23 25 * poly
W 22 15 24 26
S 1 V poly P 83 P 84
P 32 9 * ndif
P 32 14 * ndif
W 29 6 35 17
S 3 V ndif P 85 P 86
P 11 5 * ntrans
P 11 20 * ntrans
W 10 4 12 21
S 1 V ntrans P 87 P 88
P 23 7 * ntrans
P 23 16 * ntrans
W 22 6 24 17
S 1 V ntrans P 89 P 90
P 11 25 * ptrans
P 11 51 * ptrans
W 10 24 12 52
S 1 V ptrans P 91 P 92
P 23 25 * ptrans
P 23 40 * ptrans
W 22 24 24 41
S 1 V ptrans P 93 P 94
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 95 P 96
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 97 P 98
P 38 27 * pdif
P 38 49 * pdif
W 35 24 41 52
S 3 V pdif P 99 P 100
P 44 27 * pdif
P 44 49 * pdif
W 41 24 47 52
S 3 V pdif P 101 P 102
P 5 39 * nwell
P 47 39 * nwell
W -21 13 73 65
S 26 H nwell P 103 P 104
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 i2
S 2 V metal2 P 105 P 106
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47 i1
S 2 V metal2 P 107 P 108
P 38 3 * metal2
P 38 45 * metal2
W 36 1 40 47 i0
S 2 V metal2 P 109 P 110
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 f
S 2 V metal2 P 111 P 112
P 41 7 * ntrans
P 41 16 * ntrans
W 40 6 42 17
S 1 V ntrans P 113 P 114
P 35 7 * ntrans
P 35 16 * ntrans
W 34 6 36 17
S 1 V ntrans P 115 P 116
P 29 7 * ntrans
P 29 16 * ntrans
W 28 6 30 17
S 1 V ntrans P 117 P 118
P 41 25 * ptrans
P 41 51 * ptrans
W 40 24 42 52
S 1 V ptrans P 119 P 120
P 35 25 * ptrans
P 35 51 * ptrans
W 34 24 36 52
S 1 V ptrans P 121 P 122
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 123 P 124
P 20 3 * ptie
P 44 3 * ptie
W 18 1 46 5
S 2 H ptie P 125 P 126
P 26 3 * metal
P 26 13 * metal
W 24 1 28 15 vss
S 2 V metal P 127 P 128
P 20 3 * metal
P 44 3 * metal
W 18 1 46 5 vss
S 2 H metal P 129 P 130
P 8 13 * metal
P 8 34 * metal
W 6 11 10 36
S 2 V metal P 131 P 132
E
