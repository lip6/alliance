#cell1 nxr2_y idps_symb compose *
# 07-Oct-98 12:25 07-Oct-98 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 47 45
B 5 3 47 45
F F
C vdd 47 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 47 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
C f 44 45 metal2 2 5 * * N f
C f 44 3 metal2 2 6 * * S f
C i0 20 45 metal2 2 7 * * N i0
C i0 20 3 metal2 2 8 * * S i0
C i1 14 3 metal2 2 9 * * S i1
C i1 14 45 metal2 2 10 * * N i1
I cdp1 20 35 0 "cdp" ly * *
I cdp2 44 35 0 "cdp" ly * *
I cbn1 44 45 0 "cbn" ly * *
I cdn1 32 8 0 "cdn" ly * *
I cdn2 44 13 0 "cdn" ly * *
I cdp3 26 40 0 "cdp" ly * *
I cbn2 8 45 0 "cbn" ly * *
I cbn3 14 45 0 "cbn" ly * *
I cbn4 20 45 0 "cbn" ly * *
I cbp1 8 3 0 "cbp" ly * *
I cbp2 14 3 0 "cbp" ly * *
I cdp4 8 39 0 "cdp" ly * *
I cdp5 8 33 0 "cdp" ly * *
I cdp6 14 35 0 "cdp" ly * *
I cdp7 14 29 0 "cdp" ly * *
I cdp8 20 39 0 "cdp" ly * *
I cdp9 26 45 0 "cdp" ly * *
I cdp10 26 36 0 "cdp" ly * *
I cdp11 44 39 0 "cdp" ly * *
I cdp12 38 32 0 "cdp" ly * *
I cdn3 8 17 0 "cdn" ly * *
I cdn4 20 15 0 "cdn" ly * *
I cdn5 20 9 0 "cdn" ly * *
I cdn6 26 16 0 "cdn" ly * *
I cdn7 32 12 0 "cdn" ly * *
I cdn8 38 16 0 "cdn" ly * *
I cdn9 44 17 0 "cdn" ly * *
I cpf1 13 22 0 "cpf" ly * *
I cpf2 18 23 0 "cpf" ly * *
I cpf3 26 22 0 "cpf" ly * *
I cpf4 35 22 0 "cpf" ly * *
I via12x1 20 27 0 "via12" ly * *
I via12x2 44 30 0 "via12" ly * *
I via12x3 14 16 0 "via12" ly * *
I cbp3 20 3 0 "cbp" ly * *
I cpf5 40 23 0 "cpf" ly * *
P 40 23 * poly
P 41 23 * poly
W 39 22 42 24
S 1 H poly P 1 P 2
P 41 20 * poly
P 41 25 * poly
W 40 19 42 26
S 1 V poly P 3 P 4
P 8 17 * metal
P 8 26 * metal
W 7 16 9 27
S 1 V metal P 5 P 6
P 39 23 * metal
P 40 23 * metal
W 38 22 41 24
S 1 H metal P 7 P 8
P 39 23 * metal
P 39 26 * metal
W 38 22 40 27
S 1 V metal P 9 P 10
P 34 26 * metal
P 39 26 * metal
W 33 25 40 27
S 1 H metal P 11 P 12
P 34 26 * metal
P 34 31 * metal
W 33 25 35 32
S 1 V metal P 13 P 14
P 13 29 * metal
P 14 29 * metal
W 12 28 15 30
S 1 H metal P 15 P 16
P 13 26 * metal
P 13 29 * metal
W 12 25 14 30
S 1 V metal P 17 P 18
P 8 26 * metal
P 13 26 * metal
W 7 25 14 27
S 1 H metal P 19 P 20
P 18 23 * metal
P 18 24 * metal
W 17 22 19 25
S 1 V metal P 21 P 22
P 18 24 * metal
P 19 24 * metal
W 17 23 20 25
S 1 H metal P 23 P 24
P 19 24 * metal
P 19 26 * metal
W 18 23 20 27
S 1 V metal P 25 P 26
P 30 22 * metal
P 30 26 * metal
W 29 21 31 27
S 1 V metal P 27 P 28
P 30 22 * metal
P 35 22 * metal
W 29 21 36 23
S 1 H metal P 29 P 30
P 25 22 * metal
P 26 22 * metal
W 24 21 27 23
S 1 H metal P 31 P 32
P 25 20 * metal
P 25 22 * metal
W 24 19 26 23
S 1 V metal P 33 P 34
P 22 20 * metal
P 25 20 * metal
W 21 19 26 21
S 1 H metal P 35 P 36
P 22 19 * metal
P 22 20 * metal
W 21 18 23 21
S 1 V metal P 37 P 38
P 14 16 * metal
P 14 19 * metal
W 12 14 16 21
S 2 V metal P 39 P 40
P 13 19 * metal
P 22 19 * metal
W 12 18 23 20
S 1 H metal P 41 P 42
P 13 19 * metal
P 13 22 * metal
W 11 17 15 24
S 2 V metal P 43 P 44
P 32 5 * metal
P 32 12 * metal
W 30 3 34 14 vss
S 2 V metal P 45 P 46
P 5 5 * metal
P 47 5 * metal
W -3 -3 55 13 vss
S 8 H metal P 47 P 48
P 26 36 * metal
P 26 43 * metal
W 24 34 28 45 vdd
S 2 V metal P 49 P 50
P 38 31 * metal
P 38 32 * metal
W 37 30 39 33
S 1 V metal P 51 P 52
P 38 31 * metal
P 44 31 * metal
W 37 30 45 32
S 1 H metal P 53 P 54
P 17 23 * poly
P 18 23 * poly
W 16 22 19 24
S 1 H poly P 55 P 56
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 57 P 58
P 11 22 * poly
P 13 22 * poly
W 9 20 15 24
S 2 H poly P 59 P 60
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 61 P 62
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 63 P 64
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 65 P 66
P 26 3 * allowM2
P 26 45 * allowM2
W 24 1 28 47
S 2 V allowM2 P 67 P 68
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 69 P 70
P 35 20 * poly
P 35 25 * poly
W 34 19 36 26
S 1 V poly P 71 P 72
P 38 7 * ndif
P 38 18 * ndif
W 35 4 41 21
S 3 V ndif P 73 P 74
P 26 16 * metal
P 38 16 * metal
W 25 15 39 17
S 1 H metal P 75 P 76
P 26 7 * ndif
P 26 18 * ndif
W 23 4 29 21
S 3 V ndif P 77 P 78
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 79 P 80
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 81 P 82
P 26 22 * poly
P 29 22 * poly
W 23 19 32 25
S 3 H poly P 83 P 84
P 29 20 * poly
P 29 25 * poly
W 28 19 30 26
S 1 V poly P 85 P 86
P 8 3 * ptie
P 20 3 * ptie
W 6 1 22 5
S 2 H ptie P 87 P 88
P 8 45 * ntie
P 20 45 * ntie
W 6 43 22 47
S 2 H ntie P 89 P 90
P 44 7 * ndif
P 44 18 * ndif
W 41 4 47 21
S 3 V ndif P 91 P 92
P 32 7 * ndif
P 32 18 * ndif
W 29 4 35 21
S 3 V ndif P 93 P 94
P 20 8 * ndif
P 20 18 * ndif
W 17 5 23 21
S 3 V ndif P 95 P 96
P 14 8 * ndif
P 14 18 * ndif
W 11 5 17 21
S 3 V ndif P 97 P 98
P 8 8 * ndif
P 8 18 * ndif
W 5 5 11 21
S 3 V ndif P 99 P 100
P 44 27 * pdif
P 44 39 * pdif
W 41 24 47 42
S 3 V pdif P 101 P 102
P 38 27 * pdif
P 38 49 * pdif
W 35 24 41 52
S 3 V pdif P 103 P 104
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 105 P 106
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 107 P 108
P 20 27 * pdif
P 20 39 * pdif
W 17 24 23 42
S 3 V pdif P 109 P 110
P 14 27 * pdif
P 14 37 * pdif
W 11 24 17 40
S 3 V pdif P 111 P 112
P 8 27 * pdif
P 8 39 * pdif
W 5 24 11 42
S 3 V pdif P 113 P 114
P 44 3 * metal2
P 44 45 * metal2
W 42 1 46 47 f
S 2 V metal2 P 115 P 116
P 5 39 * nwell
P 47 39 * nwell
W -21 13 73 65
S 26 H nwell P 117 P 118
P 41 5 * ntrans
P 41 20 * ntrans
W 40 4 42 21
S 1 V ntrans P 119 P 120
P 35 5 * ntrans
P 35 20 * ntrans
W 34 4 36 21
S 1 V ntrans P 121 P 122
P 29 5 * ntrans
P 29 20 * ntrans
W 28 4 30 21
S 1 V ntrans P 123 P 124
P 17 6 * ntrans
P 17 20 * ntrans
W 16 5 18 21
S 1 V ntrans P 125 P 126
P 11 6 * ntrans
P 11 20 * ntrans
W 10 5 12 21
S 1 V ntrans P 127 P 128
P 41 25 * ptrans
P 41 40 * ptrans
W 40 24 42 41
S 1 V ptrans P 129 P 130
P 35 25 * ptrans
P 35 51 * ptrans
W 34 24 36 52
S 1 V ptrans P 131 P 132
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 133 P 134
P 17 25 * ptrans
P 17 39 * ptrans
W 16 24 18 40
S 1 V ptrans P 135 P 136
P 11 25 * ptrans
P 11 39 * ptrans
W 10 24 12 40
S 1 V ptrans P 137 P 138
P 44 17 * metal
P 44 31 * metal
W 43 16 45 32
S 1 V metal P 139 P 140
P 44 13 * metal
P 44 17 * metal
W 42 11 46 19
S 2 V metal P 141 P 142
P 19 26 * metal
P 30 26 * metal
W 18 25 31 27
S 1 H metal P 143 P 144
P 20 26 * metal
P 20 27 * metal
W 19 25 21 28
S 1 V metal P 145 P 146
P 14 29 * metal
P 14 35 * metal
W 12 27 16 37
S 2 V metal P 147 P 148
P 14 31 * metal
P 34 31 * metal
W 13 30 35 32
S 1 H metal P 149 P 150
P 20 35 * metal
P 20 39 * metal
W 18 33 22 41
S 2 V metal P 151 P 152
P 8 45 * metal
P 20 45 * metal
W 6 43 22 47 vdd
S 2 H metal P 153 P 154
P 8 33 * metal
P 8 45 * metal
W 6 31 10 47 vdd
S 2 V metal P 155 P 156
P 20 45 * metal
P 44 45 * metal
W 19 44 45 46
S 1 H metal P 157 P 158
P 20 3 * metal
P 20 15 * metal
W 18 1 22 17 vss
S 2 V metal P 159 P 160
P 8 3 * metal
P 20 3 * metal
W 6 1 22 5 vss
S 2 H metal P 161 P 162
P 5 43 * metal
P 47 43 * metal
W -3 35 55 51 vdd
S 8 H metal P 163 P 164
P 44 35 * metal
P 44 43 * metal
W 42 33 46 45 vdd
S 2 V metal P 165 P 166
E
