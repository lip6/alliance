#cell1 xr2_y idps_symb compose *
# 07-Oct-98 12:34 07-Oct-98 12:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 53 45
B 5 3 53 45
F F
C vss 5 5 metal 8 1 * * W vss
C vss 53 5 metal 8 2 * * E vss
C vdd 53 43 metal 8 3 * * E vdd
C vdd 5 43 metal 8 4 * * W vdd
C t 50 45 metal2 2 5 * * N t
C t 50 3 metal2 2 6 * * S t
C i0 38 45 metal2 2 7 * * N i0
C i0 38 3 metal2 2 8 * * S i0
C i1 20 45 metal2 2 9 * * N i1
C i1 20 3 metal2 2 10 * * S i1
I cbp1 20 3 0 "cbp" ly * *
I cbp2 14 3 0 "cbp" ly * *
I cbp3 8 3 0 "cbp" ly * *
I cbp4 50 3 0 "cbp" ly * *
I cbp5 26 13 0 "cbp" ly * *
I cbn1 26 40 0 "cbn" ly * *
I cbn2 26 35 0 "cbn" ly * *
I cbn3 26 45 0 "cbn" ly * *
I cbp6 26 17 0 "cbp" ly * *
I cbp7 26 8 0 "cbp" ly * *
I cbp8 26 3 0 "cbp" ly * *
I cdn1 8 12 0 "cdn" ly * *
I cdn2 20 12 0 "cdn" ly * *
I cdn3 32 15 0 "cdn" ly * *
I cdn4 32 9 0 "cdn" ly * *
I cdn5 44 15 0 "cdn" ly * *
I cdn6 50 12 0 "cdn" ly * *
I cdp1 8 34 0 "cdp" ly * *
I cdp2 20 42 0 "cdp" ly * *
I cdp3 20 36 0 "cdp" ly * *
I cdp4 32 34 0 "cdp" ly * *
I cdp5 38 38 0 "cdp" ly * *
I cdp6 44 34 0 "cdp" ly * *
I cdp7 50 34 0 "cdp" ly * *
I cdp8 50 28 0 "cdp" ly * *
I cdn7 14 16 0 "cdn" ly * *
I cpf1 13 22 0 "cpf" ly * *
I cpf2 20 22 0 "cpf" ly * *
I cpf3 32 22 0 "cpf" ly * *
I cpf4 39 22 0 "cpf" ly * *
I cpf5 45 22 0 "cpf" ly * *
I via12x1 20 17 0 "via12" ly * *
I via12x2 38 16 0 "via12" ly * *
I via12x3 50 22 0 "via12" ly * *
I cdp9 38 44 0 "cdp" ly * *
I cdp10 8 28 0 "cdp" ly * *
P 32 7 * ndif
P 32 18 * ndif
W 29 4 35 21
S 3 V ndif P 1 P 2
P 32 5 * metal
P 32 15 * metal
W 30 3 34 17 vss
S 2 V metal P 3 P 4
P 20 5 * metal
P 20 12 * metal
W 18 3 22 14 vss
S 2 V metal P 5 P 6
P 26 5 * metal
P 26 17 * metal
W 24 3 28 19 vss
S 2 V metal P 7 P 8
P 5 5 * metal
P 53 5 * metal
W -3 -3 61 13 vss
S 8 H metal P 9 P 10
P 8 16 * metal
P 14 16 * metal
W 7 15 15 17
S 1 H metal P 11 P 12
P 8 16 * metal
P 8 28 * metal
W 7 15 9 29
S 1 V metal P 13 P 14
P 8 28 * metal
P 8 34 * metal
W 6 26 10 36
S 2 V metal P 15 P 16
P 8 30 * metal
P 44 30 * metal
W 7 29 45 31
S 1 H metal P 17 P 18
P 44 22 * metal
P 44 30 * metal
W 43 21 45 31
S 1 V metal P 19 P 20
P 44 22 * metal
P 45 22 * metal
W 42 20 47 24
S 2 H metal P 21 P 22
P 13 22 * metal
P 13 26 * metal
W 11 20 15 28
S 2 V metal P 23 P 24
P 13 26 * metal
P 38 26 * metal
W 12 25 39 27
S 1 H metal P 25 P 26
P 38 22 * metal
P 38 26 * metal
W 37 21 39 27
S 1 V metal P 27 P 28
P 38 22 * metal
P 39 22 * metal
W 36 20 41 24
S 2 H metal P 29 P 30
P 38 16 * metal
P 38 22 * metal
W 36 14 40 24
S 2 V metal P 31 P 32
P 11 22 * poly
P 13 22 * poly
W 9 20 15 24
S 2 H poly P 33 P 34
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 35 P 36
P 17 22 * poly
P 20 22 * poly
W 14 19 23 25
S 3 H poly P 37 P 38
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 39 P 40
P 32 22 * poly
P 35 22 * poly
W 29 19 38 25
S 3 H poly P 41 P 42
P 35 20 * poly
P 35 25 * poly
W 34 19 36 26
S 1 V poly P 43 P 44
P 39 22 * poly
P 41 22 * poly
W 37 20 43 24
S 2 H poly P 45 P 46
P 41 20 * poly
P 41 25 * poly
W 40 19 42 26
S 1 V poly P 47 P 48
P 45 22 * poly
P 47 22 * poly
W 43 20 49 24
S 2 H poly P 49 P 50
P 47 20 * poly
P 47 25 * poly
W 46 19 48 26
S 1 V poly P 51 P 52
P 50 17 * metal
P 50 34 * metal
W 48 15 52 36
S 2 V metal P 53 P 54
P 44 17 * metal
P 50 17 * metal
W 43 16 51 18
S 1 H metal P 55 P 56
P 44 15 * metal
P 44 17 * metal
W 42 13 46 19
S 2 V metal P 57 P 58
P 20 17 * metal
P 20 22 * metal
W 18 15 22 24
S 2 V metal P 59 P 60
P 20 22 * metal
P 32 22 * metal
W 19 21 33 23
S 1 H metal P 61 P 62
P 38 38 * metal
P 38 44 * metal
W 37 37 39 45 vdd
S 1 V metal P 63 P 64
P 20 36 * metal
P 20 43 * metal
W 18 34 22 45 vdd
S 2 V metal P 65 P 66
P 44 3 * allowM2
P 44 45 * allowM2
W 42 1 46 47
S 2 V allowM2 P 67 P 68
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 69 P 70
P 26 3 * allowM2
P 26 45 * allowM2
W 24 1 28 47
S 2 V allowM2 P 71 P 72
P 14 3 * allowM2
P 14 45 * allowM2
W 12 1 16 47
S 2 V allowM2 P 73 P 74
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 75 P 76
P 32 34 * metal
P 44 34 * metal
W 31 33 45 35
S 1 H metal P 77 P 78
P 38 3 * metal2
P 38 45 * metal2
W 36 1 40 47 i0
S 2 V metal2 P 79 P 80
P 26 35 * ntie
P 26 43 * ntie
W 23 32 29 46
S 3 V ntie P 81 P 82
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 83 P 84
P 26 5 * ptie
P 26 17 * ptie
W 23 2 29 20
S 3 V ptie P 85 P 86
P 44 7 * ndif
P 44 18 * ndif
W 41 4 47 21
S 3 V ndif P 87 P 88
P 14 13 * ndif
P 14 18 * ndif
W 11 10 17 21
S 3 V ndif P 89 P 90
P 38 7 * ndif
P 38 18 * ndif
W 35 4 41 21
S 3 V ndif P 91 P 92
P 50 3 * metal2
P 50 45 * metal2
W 48 1 52 47
S 2 V metal2 P 93 P 94
P 50 12 * ndif
P 50 18 * ndif
W 47 9 53 21
S 3 V ndif P 95 P 96
P 20 12 * ndif
P 20 18 * ndif
W 17 9 23 21
S 3 V ndif P 97 P 98
P 8 12 * ndif
P 8 18 * ndif
W 5 9 11 21
S 3 V ndif P 99 P 100
P 50 27 * pdif
P 50 49 * pdif
W 47 24 53 52
S 3 V pdif P 101 P 102
P 44 27 * pdif
P 44 49 * pdif
W 41 24 47 52
S 3 V pdif P 103 P 104
P 38 27 * pdif
P 38 49 * pdif
W 35 24 41 52
S 3 V pdif P 105 P 106
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 107 P 108
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 109 P 110
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 111 P 112
P 8 27 * pdif
P 8 49 * pdif
W 5 24 11 52
S 3 V pdif P 113 P 114
P 5 39 * nwell
P 53 39 * nwell
W -21 13 79 65
S 26 H nwell P 115 P 116
P 47 11 * ntrans
P 47 20 * ntrans
W 46 10 48 21
S 1 V ntrans P 117 P 118
P 41 5 * ntrans
P 41 20 * ntrans
W 40 4 42 21
S 1 V ntrans P 119 P 120
P 35 5 * ntrans
P 35 20 * ntrans
W 34 4 36 21
S 1 V ntrans P 121 P 122
P 17 11 * ntrans
P 17 20 * ntrans
W 16 10 18 21
S 1 V ntrans P 123 P 124
P 11 11 * ntrans
P 11 20 * ntrans
W 10 10 12 21
S 1 V ntrans P 125 P 126
P 47 25 * ptrans
P 47 51 * ptrans
W 46 24 48 52
S 1 V ptrans P 127 P 128
P 41 25 * ptrans
P 41 51 * ptrans
W 40 24 42 52
S 1 V ptrans P 129 P 130
P 35 25 * ptrans
P 35 51 * ptrans
W 34 24 36 52
S 1 V ptrans P 131 P 132
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 133 P 134
P 11 25 * ptrans
P 11 51 * ptrans
W 10 24 12 52
S 1 V ptrans P 135 P 136
P 5 43 * metal
P 53 43 * metal
W -3 35 61 51 vdd
S 8 H metal P 137 P 138
P 8 3 * ptie
P 26 3 * ptie
W 6 1 28 5
S 2 H ptie P 139 P 140
P 26 35 * metal
P 26 45 * metal
W 24 33 28 47 vdd
S 2 V metal P 141 P 142
P 8 3 * metal
P 8 12 * metal
W 6 1 10 14 vss
S 2 V metal P 143 P 144
P 8 3 * metal
P 26 3 * metal
W 6 1 28 5 vss
S 2 H metal P 145 P 146
P 50 3 * metal
P 50 12 * metal
W 48 1 52 14 vss
S 2 V metal P 147 P 148
E
