#cell1 mx2p_y idps_symb compose *
# 07-Oct-98 12:10 07-Oct-98 12:10 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 53 45
B 5 3 53 45
F F
C t 44 45 metal2 2 1 * * N t
C l0 32 45 metal2 2 2 * * N l0
C i0 26 45 metal2 2 3 * * N i0
C i1 14 45 metal2 2 4 * * N i1
C l1 8 45 metal2 2 5 * * N l1
C t 44 3 metal2 2 6 * * S t
C l0 32 3 metal2 2 7 * * S l0
C i0 26 3 metal2 2 8 * * S i0
C i1 14 3 metal2 2 9 * * S i1
C l1 8 3 metal2 2 10 * * S l1
C vdd 53 43 metal 8 11 * * E vdd
C vdd 5 43 metal 8 12 * * W vdd
C vss 5 5 metal 8 13 * * W vss
C vss 53 5 metal 8 14 * * E vss
I cbp1 50 3 0 "cbp" ly * *
I cbp2 44 3 0 "cbp" ly * *
I cbp3 38 3 0 "cbp" ly * *
I cbp4 32 3 0 "cbp" ly * *
I cbp5 26 3 0 "cbp" ly * *
I cbp6 20 3 0 "cbp" ly * *
I cbp7 14 3 0 "cbp" ly * *
I cbp8 8 3 0 "cbp" ly * *
I cdn1 8 12 0 "cdn" ly * *
I cdn2 20 15 0 "cdn" ly * *
I cdn3 32 12 0 "cdn" ly * *
I cdn4 38 15 0 "cdn" ly * *
I cdn5 44 16 0 "cdn" ly * *
I cdp1 8 28 0 "cdp" ly * *
I cdp2 8 35 0 "cdp" ly * *
I cdp3 14 34 0 "cdp" ly * *
I cdp4 14 42 0 "cdp" ly * *
I cdp5 20 31 0 "cdp" ly * *
I cdp6 20 35 0 "cdp" ly * *
I cdp7 32 30 0 "cdp" ly * *
I cdp8 32 34 0 "cdp" ly * *
I cdp9 38 30 0 "cdp" ly * *
I cdp10 38 34 0 "cdp" ly * *
I cdp11 38 39 0 "cdp" ly * *
I cdp12 44 34 0 "cdp" ly * *
I cdp13 44 28 0 "cdp" ly * *
I cpf1 9 22 0 "cpf" ly * *
I cpf2 15 22 0 "cpf" ly * *
I cpf3 25 22 0 "cpf" ly * *
I cpf4 31 22 0 "cpf" ly * *
I cpf5 39 24 0 "cpf" ly * *
I via12x1 44 22 0 "via12" ly * *
I via12x2 32 17 0 "via12" ly * *
I via12x3 26 17 0 "via12" ly * *
I via12x4 14 17 0 "via12" ly * *
I via12x5 8 17 0 "via12" ly * *
I cbn1 38 45 0 "cbn" ly * *
I cdp14 50 39 0 "cdp" ly * *
I cdp15 50 30 0 "cdp" ly * *
I cdn6 50 15 0 "cdn" ly * *
I cbn2 50 45 0 "cbn" ly * *
I cbn3 44 45 0 "cbn" ly * *
I cdp16 50 34 0 "cdp" ly * *
I cdp17 14 38 0 "cdp" ly * *
I cdp18 26 31 0 "cdp" ly * *
P 14 17 * metal
P 14 22 * metal
W 13 16 15 23
S 1 V metal P 1 P 2
P 32 17 * metal
P 32 22 * metal
W 31 16 33 23
S 1 V metal P 3 P 4
P 31 22 * metal
P 32 22 * metal
W 30 21 33 23
S 1 H metal P 5 P 6
P 26 17 * metal
P 26 22 * metal
W 25 16 27 23
S 1 V metal P 7 P 8
P 25 22 * metal
P 26 22 * metal
W 24 21 27 23
S 1 H metal P 9 P 10
P 20 15 * metal
P 20 26 * metal
W 19 14 21 27
S 1 V metal P 11 P 12
P 20 26 * metal
P 39 26 * metal
W 19 25 40 27
S 1 H metal P 13 P 14
P 26 26 * metal
P 26 31 * metal
W 24 24 28 33
S 2 V metal P 15 P 16
P 39 24 * metal
P 39 26 * metal
W 37 22 41 28
S 2 V metal P 17 P 18
P 32 5 * metal
P 32 12 * metal
W 30 3 34 14 vss
S 2 V metal P 19 P 20
P 50 30 * metal
P 50 43 * metal
W 48 28 52 45 vdd
S 2 V metal P 21 P 22
P 5 43 * metal
P 53 43 * metal
W -3 35 61 51 vdd
S 8 H metal P 23 P 24
P 44 16 * metal
P 44 34 * metal
W 42 14 46 36
S 2 V metal P 25 P 26
P 20 30 * metal
P 20 35 * metal
W 18 28 22 37
S 2 V metal P 27 P 28
P 20 35 * metal
P 32 35 * metal
W 19 34 33 36
S 1 H metal P 29 P 30
P 8 30 * metal
P 20 30 * metal
W 7 29 21 31
S 1 H metal P 31 P 32
P 8 28 * metal
P 8 35 * metal
W 6 26 10 37
S 2 V metal P 33 P 34
P 32 34 * metal
P 32 35 * metal
W 31 33 33 36
S 1 V metal P 35 P 36
P 32 30 * metal
P 32 34 * metal
W 30 28 34 36
S 2 V metal P 37 P 38
P 50 3 * allowM2
P 50 45 * allowM2
W 48 1 52 47
S 2 V allowM2 P 39 P 40
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 41 P 42
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 43 P 44
P 8 17 * metal
P 8 22 * metal
W 7 16 9 23
S 1 V metal P 45 P 46
P 8 22 * metal
P 9 22 * metal
W 7 21 10 23
S 1 H metal P 47 P 48
P 14 22 * metal
P 15 22 * metal
W 13 21 16 23
S 1 H metal P 49 P 50
P 38 27 * pdif
P 38 39 * pdif
W 35 24 41 42
S 3 V pdif P 51 P 52
P 50 27 * pdif
P 50 39 * pdif
W 47 24 53 42
S 3 V pdif P 53 P 54
P 44 27 * pdif
P 44 38 * pdif
W 41 24 47 41
S 3 V pdif P 55 P 56
P 5 39 * nwell
P 53 39 * nwell
W -21 13 79 65
S 26 H nwell P 57 P 58
P 38 13 * ndif
P 38 18 * ndif
W 35 10 41 21
S 3 V ndif P 59 P 60
P 44 13 * ndif
P 44 18 * ndif
W 41 10 47 21
S 3 V ndif P 61 P 62
P 50 13 * ndif
P 50 18 * ndif
W 47 10 53 21
S 3 V ndif P 63 P 64
P 8 11 * ndif
P 8 18 * ndif
W 5 8 11 21
S 3 V ndif P 65 P 66
P 32 11 * ndif
P 32 18 * ndif
W 29 8 35 21
S 3 V ndif P 67 P 68
P 23 22 * poly
P 25 22 * poly
W 21 20 27 24
S 2 H poly P 69 P 70
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 71 P 72
P 39 24 * poly
P 41 24 * poly
W 37 22 43 26
S 2 H poly P 73 P 74
P 41 20 * poly
P 41 25 * poly
W 40 19 42 26
S 1 V poly P 75 P 76
P 41 25 * poly
P 47 25 * poly
W 40 24 48 26
S 1 H poly P 77 P 78
P 47 20 * poly
P 47 25 * poly
W 46 19 48 26
S 1 V poly P 79 P 80
P 29 22 * poly
P 31 22 * poly
W 27 20 33 24
S 2 H poly P 81 P 82
P 29 20 * poly
P 29 25 * poly
W 28 19 30 26
S 1 V poly P 83 P 84
P 15 22 * poly
P 17 22 * poly
W 13 20 19 24
S 2 H poly P 85 P 86
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 87 P 88
P 9 22 * poly
P 11 22 * poly
W 7 20 13 24
S 2 H poly P 89 P 90
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 91 P 92
P 8 27 * pdif
P 8 42 * pdif
W 5 24 11 45
S 3 V pdif P 93 P 94
P 20 27 * pdif
P 20 42 * pdif
W 17 24 23 45
S 3 V pdif P 95 P 96
P 14 27 * pdif
P 14 42 * pdif
W 11 24 17 45
S 3 V pdif P 97 P 98
P 38 45 * ntie
P 50 45 * ntie
W 36 43 52 47
S 2 H ntie P 99 P 100
P 47 11 * ntrans
P 47 20 * ntrans
W 46 10 48 21
S 1 V ntrans P 101 P 102
P 47 25 * ptrans
P 47 40 * ptrans
W 46 24 48 41
S 1 V ptrans P 103 P 104
P 26 11 * ndif
P 26 18 * ndif
W 23 8 29 21
S 3 V ndif P 105 P 106
P 20 11 * ndif
P 20 18 * ndif
W 17 8 23 21
S 3 V ndif P 107 P 108
P 14 11 * ndif
P 14 18 * ndif
W 11 8 17 21
S 3 V ndif P 109 P 110
P 32 27 * pdif
P 32 42 * pdif
W 29 24 35 45
S 3 V pdif P 111 P 112
P 26 27 * pdif
P 26 42 * pdif
W 23 24 29 45
S 3 V pdif P 113 P 114
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 l1
S 2 V metal2 P 115 P 116
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 i1
S 2 V metal2 P 117 P 118
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 i0
S 2 V metal2 P 119 P 120
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47 l0
S 2 V metal2 P 121 P 122
P 44 3 * metal2
P 44 45 * metal2
W 42 1 46 47 t
S 2 V metal2 P 123 P 124
P 41 11 * ntrans
P 41 20 * ntrans
W 40 10 42 21
S 1 V ntrans P 125 P 126
P 29 9 * ntrans
P 29 20 * ntrans
W 28 8 30 21
S 1 V ntrans P 127 P 128
P 23 9 * ntrans
P 23 20 * ntrans
W 22 8 24 21
S 1 V ntrans P 129 P 130
P 17 9 * ntrans
P 17 20 * ntrans
W 16 8 18 21
S 1 V ntrans P 131 P 132
P 11 9 * ntrans
P 11 20 * ntrans
W 10 8 12 21
S 1 V ntrans P 133 P 134
P 41 25 * ptrans
P 41 40 * ptrans
W 40 24 42 41
S 1 V ptrans P 135 P 136
P 29 25 * ptrans
P 29 44 * ptrans
W 28 24 30 45
S 1 V ptrans P 137 P 138
P 23 25 * ptrans
P 23 44 * ptrans
W 22 24 24 45
S 1 V ptrans P 139 P 140
P 17 25 * ptrans
P 17 44 * ptrans
W 16 24 18 45
S 1 V ptrans P 141 P 142
P 11 25 * ptrans
P 11 44 * ptrans
W 10 24 12 45
S 1 V ptrans P 143 P 144
P 8 3 * ptie
P 50 3 * ptie
W 6 1 52 5
S 2 H ptie P 145 P 146
P 50 5 * metal
P 50 15 * metal
W 48 3 52 17 vss
S 2 V metal P 147 P 148
P 5 5 * metal
P 53 5 * metal
W -3 -3 61 13 vss
S 8 H metal P 149 P 150
P 38 5 * metal
P 38 15 * metal
W 36 3 40 17 vss
S 2 V metal P 151 P 152
P 14 34 * metal
P 14 43 * metal
W 12 32 16 45 vdd
S 2 V metal P 153 P 154
P 38 30 * metal
P 38 45 * metal
W 36 28 40 47 vdd
S 2 V metal P 155 P 156
P 38 45 * metal
P 50 45 * metal
W 36 43 52 47 vdd
S 2 H metal P 157 P 158
P 8 3 * metal
P 8 12 * metal
W 6 1 10 14 vss
S 2 V metal P 159 P 160
P 8 3 * metal
P 50 3 * metal
W 6 1 52 5 vss
S 2 H metal P 161 P 162
E
