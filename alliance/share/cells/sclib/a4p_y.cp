#cell1 a4p_y idps_symb compose *
# 07-Oct-98 12:05 07-Oct-98 12:05 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 12 47 54
B 5 12 47 54
F F
C i3 44 12 metal2 2 1 * * S i3
C i3 44 54 metal2 2 2 * * N i3
C vss 47 14 metal 8 3 * * E vss
C t 8 12 metal2 2 4 * * S t
C t 8 54 metal2 2 5 * * N t
C i2 32 54 metal2 2 6 * * N i2
C i2 32 12 metal2 2 7 * * S i2
C i1 26 12 metal2 2 8 * * S i1
C i1 26 54 metal2 2 9 * * N i1
C i0 20 54 metal2 2 10 * * N i0
C i0 20 12 metal2 2 11 * * S i0
C vss 5 14 metal 8 12 * * W vss
C vdd 5 52 metal 8 13 * * W vdd
C vdd 47 52 metal 8 14 * * E vdd
I cbn1 44 41 0 "cbn" ly * *
I cbn2 44 47 0 "cbn" ly * *
I cbp1 44 19 0 "cbp" ly * *
I cbp2 44 26 0 "cbp" ly * *
I cbn3 38 53 0 "cbn" ly * *
I cbp3 44 12 0 "cbp" ly * *
I cbn4 44 53 0 "cbn" ly * *
I cdp1 14 54 0 "cdp" ly * *
I via12x1 8 31 0 "via12" ly * *
I cpf1 13 31 0 "cpf" ly * *
I cdp2 8 37 0 "cdp" ly * *
I cdp3 8 43 0 "cdp" ly * *
I cdn1 8 22 0 "cdn" ly * *
I cdn2 8 26 0 "cdn" ly * *
I cbn5 20 53 0 "cbn" ly * *
I cdn3 14 11 0 "cdn" ly * *
I via12x2 20 26 0 "via12" ly * *
I via12x3 44 31 0 "via12" ly * *
I via12x4 32 26 0 "via12" ly * *
I via12x5 26 26 0 "via12" ly * *
I cpf2 44 36 0 "cpf" ly * *
I cpf3 30 31 0 "cpf" ly * *
I cpf4 24 31 0 "cpf" ly * *
I cpf5 18 31 0 "cpf" ly * *
I cdp4 38 42 0 "cdp" ly * *
I cdp5 38 46 0 "cdp" ly * *
I cdp6 32 37 0 "cdp" ly * *
I cdp7 32 43 0 "cdp" ly * *
I cdp8 26 42 0 "cdp" ly * *
I cdp9 26 46 0 "cdp" ly * *
I cdp10 20 37 0 "cdp" ly * *
I cdp11 20 43 0 "cdp" ly * *
I cdp12 14 42 0 "cdp" ly * *
I cdp13 14 48 0 "cdp" ly * *
I cdn4 38 23 0 "cdn" ly * *
I cdn5 14 17 0 "cdn" ly * *
I cdn6 14 23 0 "cdn" ly * *
I cbn6 26 53 0 "cbn" ly * *
I cbn7 32 53 0 "cbn" ly * *
P 44 41 * metal
P 44 52 * metal
W 42 39 46 54 vdd
S 2 V metal P 1 P 2
P 5 52 * metal
P 47 52 * metal
W -3 44 55 60 vdd
S 8 H metal P 3 P 4
P 38 42 * metal
P 38 52 * metal
W 36 40 40 54 vdd
S 2 V metal P 5 P 6
P 17 34 * ptrans
P 17 49 * ptrans
W 16 33 18 50
S 1 V ptrans P 7 P 8
P 23 34 * ptrans
P 23 49 * ptrans
W 22 33 24 50
S 1 V ptrans P 9 P 10
P 29 34 * ptrans
P 29 49 * ptrans
W 28 33 30 50
S 1 V ptrans P 11 P 12
P 35 34 * ptrans
P 35 49 * ptrans
W 34 33 36 50
S 1 V ptrans P 13 P 14
P 17 2 * ntrans
P 17 29 * ntrans
W 16 1 18 30
S 1 V ntrans P 15 P 16
P 23 2 * ntrans
P 23 29 * ntrans
W 22 1 24 30
S 1 V ntrans P 17 P 18
P 29 2 * ntrans
P 29 29 * ntrans
W 28 1 30 30
S 1 V ntrans P 19 P 20
P 35 2 * ntrans
P 35 29 * ntrans
W 34 1 36 30
S 1 V ntrans P 21 P 22
P 5 48 * nwell
P 47 48 * nwell
W -21 22 73 74
S 26 H nwell P 23 P 24
P 20 12 * metal2
P 20 54 * metal2
W 18 10 22 56
S 2 V metal2 P 25 P 26
P 26 12 * metal2
P 26 54 * metal2
W 24 10 28 56
S 2 V metal2 P 27 P 28
P 32 12 * metal2
P 32 54 * metal2
W 30 10 34 56
S 2 V metal2 P 29 P 30
P 20 36 * pdif
P 20 47 * pdif
W 17 33 23 50
S 3 V pdif P 31 P 32
P 32 36 * pdif
P 32 47 * pdif
W 29 33 35 50
S 3 V pdif P 33 P 34
P 14 36 * pdif
P 14 58 * pdif
W 11 33 17 61
S 3 V pdif P 35 P 36
P 38 4 * ndif
P 38 27 * ndif
W 35 1 41 30
S 3 V ndif P 37 P 38
P 32 4 * ndif
P 32 27 * ndif
W 29 1 35 30
S 3 V ndif P 39 P 40
P 26 4 * ndif
P 26 27 * ndif
W 23 1 29 30
S 3 V ndif P 41 P 42
P 20 4 * ndif
P 20 27 * ndif
W 17 1 23 30
S 3 V ndif P 43 P 44
P 14 4 * ndif
P 14 27 * ndif
W 11 1 17 30
S 3 V ndif P 45 P 46
P 11 34 * ptrans
P 11 60 * ptrans
W 10 33 12 61
S 1 V ptrans P 47 P 48
P 11 14 * ntrans
P 11 29 * ntrans
W 10 13 12 30
S 1 V ntrans P 49 P 50
P 8 16 * ndif
P 8 27 * ndif
W 5 13 11 30
S 3 V ndif P 51 P 52
P 8 36 * pdif
P 8 58 * pdif
W 5 33 11 61
S 3 V pdif P 53 P 54
P 8 12 * metal2
P 8 54 * metal2
W 6 10 10 56
S 2 V metal2 P 55 P 56
P 35 29 * poly
P 35 34 * poly
W 34 28 36 35
S 1 V poly P 57 P 58
P 35 32 * poly
P 41 32 * poly
W 34 31 42 33
S 1 H poly P 59 P 60
P 41 36 * poly
P 44 36 * poly
W 38 33 47 39
S 3 H poly P 61 P 62
P 41 32 * poly
P 41 36 * poly
W 40 31 42 37
S 1 V poly P 63 P 64
P 11 29 * poly
P 11 34 * poly
W 10 28 12 35
S 1 V poly P 65 P 66
P 11 31 * poly
P 13 31 * poly
W 9 29 15 33
S 2 H poly P 67 P 68
P 25 26 * metal
P 26 26 * metal
W 24 25 27 27
S 1 H metal P 69 P 70
P 25 26 * metal
P 25 31 * metal
W 24 25 26 32
S 1 V metal P 71 P 72
P 24 31 * metal
P 25 31 * metal
W 23 30 26 32
S 1 H metal P 73 P 74
P 30 31 * metal
P 31 31 * metal
W 29 30 32 32
S 1 H metal P 75 P 76
P 31 26 * metal
P 31 31 * metal
W 30 25 32 32
S 1 V metal P 77 P 78
P 31 26 * metal
P 32 26 * metal
W 30 25 33 27
S 1 H metal P 79 P 80
P 26 36 * pdif
P 26 47 * pdif
W 23 33 29 50
S 3 V pdif P 81 P 82
P 38 36 * pdif
P 38 47 * pdif
W 35 33 41 50
S 3 V pdif P 83 P 84
P 14 12 * allowM2
P 14 54 * allowM2
W 12 10 16 56
S 2 V allowM2 P 85 P 86
P 38 12 * allowM2
P 38 54 * allowM2
W 36 10 40 56
S 2 V allowM2 P 87 P 88
P 44 12 * metal2
P 44 54 * metal2
W 42 10 46 56
S 2 V metal2 P 89 P 90
P 14 14 * metal
P 14 23 * metal
W 12 12 16 25 vss
S 2 V metal P 91 P 92
P 5 14 * metal
P 47 14 * metal
W -3 6 55 22 vss
S 8 H metal P 93 P 94
P 14 11 * metal
P 14 14 * metal
W 13 10 15 15 vss
S 1 V metal P 95 P 96
P 44 31 * metal
P 44 36 * metal
W 43 30 45 37
S 1 V metal P 97 P 98
P 13 31 * metal
P 14 31 * metal
W 12 30 15 32
S 1 H metal P 99 P 100
P 14 31 * metal
P 14 36 * metal
W 13 30 15 37
S 1 V metal P 101 P 102
P 14 36 * metal
P 38 36 * metal
W 13 35 39 37
S 1 H metal P 103 P 104
P 38 23 * metal
P 38 36 * metal
W 37 22 39 37
S 1 V metal P 105 P 106
P 32 36 * metal
P 32 43 * metal
W 30 34 34 45
S 2 V metal P 107 P 108
P 20 36 * metal
P 20 43 * metal
W 18 34 22 45
S 2 V metal P 109 P 110
P 18 31 * metal
P 19 31 * metal
W 17 30 20 32
S 1 H metal P 111 P 112
P 19 26 * metal
P 19 31 * metal
W 18 25 20 32
S 1 V metal P 113 P 114
P 19 26 * metal
P 20 26 * metal
W 18 25 21 27
S 1 H metal P 115 P 116
P 29 29 * poly
P 29 34 * poly
W 28 28 30 35
S 1 V poly P 117 P 118
P 29 31 * poly
P 30 31 * poly
W 28 30 31 32
S 1 H poly P 119 P 120
P 23 29 * poly
P 23 34 * poly
W 22 28 24 35
S 1 V poly P 121 P 122
P 23 31 * poly
P 24 31 * poly
W 22 30 25 32
S 1 H poly P 123 P 124
P 17 29 * poly
P 17 34 * poly
W 16 28 18 35
S 1 V poly P 125 P 126
P 17 31 * poly
P 18 31 * poly
W 16 30 19 32
S 1 H poly P 127 P 128
P 14 52 * metal
P 14 54 * metal
W 13 51 15 55 vdd
S 1 V metal P 129 P 130
P 14 42 * metal
P 14 52 * metal
W 12 40 16 54 vdd
S 2 V metal P 131 P 132
P 44 41 * ntie
P 44 54 * ntie
W 42 39 46 56
S 2 V ntie P 133 P 134
P 20 53 * ntie
P 44 53 * ntie
W 18 51 46 55
S 2 H ntie P 135 P 136
P 44 12 * ptie
P 44 26 * ptie
W 42 10 46 28
S 2 V ptie P 137 P 138
P 8 22 * metal
P 8 43 * metal
W 6 20 10 45
S 2 V metal P 139 P 140
P 20 53 * metal
P 44 53 * metal
W 18 51 46 55 vdd
S 2 H metal P 141 P 142
P 26 42 * metal
P 26 53 * metal
W 24 40 28 55 vdd
S 2 V metal P 143 P 144
P 44 12 * metal
P 44 26 * metal
W 42 10 46 28 vss
S 2 V metal P 145 P 146
E
