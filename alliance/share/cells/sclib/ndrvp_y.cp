#cell1 ndrvp_y idps_symb compose *
# 07-Oct-98 15:47 07-Oct-98 15:47 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 47 45
B 5 3 47 45
F F
C vdd 5 43 metal 8 1 * * W vdd
C vss 5 5 metal 8 2 * * W vss
C vdd 47 43 metal 8 3 * * E vdd
C vss 47 5 metal 8 4 * * E vss
C i 26 45 metal2 2 5 * * N i
C i 26 3 metal2 2 6 * * S i
C f 14 45 metal2 2 7 * * N f
C f 14 3 metal2 2 8 * * S f
I cbp1 44 17 0 "cbp" ly * *
I cbp2 44 12 0 "cbp" ly * *
I cbp3 44 7 0 "cbp" ly * *
I cbp4 44 3 0 "cbp" ly * *
I cdn1 38 16 0 "cdn" ly * *
I cdn2 38 12 0 "cdn" ly * *
I cdn3 38 8 0 "cdn" ly * *
I cdn4 32 13 0 "cdn" ly * *
I cdn5 32 17 0 "cdn" ly * *
I cdp1 14 37 0 "cdp" ly * *
I cdp2 14 33 0 "cdp" ly * *
I cdp3 26 33 0 "cdp" ly * *
I cdp4 26 37 0 "cdp" ly * *
I cdp5 32 30 0 "cdp" ly * *
I cdp6 32 35 0 "cdp" ly * *
I cdp7 38 29 0 "cdp" ly * *
I cdp8 38 33 0 "cdp" ly * *
I cdp9 38 37 0 "cdp" ly * *
I cpf1 30 23 0 "cpf" ly * *
I cdp10 38 41 0 "cdp" ly * *
I cdp11 38 45 0 "cdp" ly * *
I cdp12 26 45 0 "cdp" ly * *
I cbn1 44 29 0 "cbn" ly * *
I cbn2 44 33 0 "cbn" ly * *
I cbn3 44 37 0 "cbn" ly * *
I cbn4 44 41 0 "cbn" ly * *
I cbn5 44 45 0 "cbn" ly * *
I cdp13 14 45 0 "cdp" ly * *
I cdn6 14 8 0 "cdn" ly * *
I cdn7 20 17 0 "cdn" ly * *
I cbp5 8 3 0 "cbp" ly * *
I cbn6 8 29 0 "cbn" ly * *
I cbn7 8 37 0 "cbn" ly * *
I cbn8 8 33 0 "cbn" ly * *
I cbn9 8 41 0 "cbn" ly * *
I cbn10 8 45 0 "cbn" ly * *
I cbp6 8 17 0 "cbp" ly * *
I cbp7 8 13 0 "cbp" ly * *
I cbp8 8 9 0 "cbp" ly * *
I cdn8 14 12 0 "cdn" ly * *
I cdn9 20 13 0 "cdn" ly * *
I cdn10 26 12 0 "cdn" ly * *
I cdn11 26 8 0 "cdn" ly * *
I cdp14 14 41 0 "cdp" ly * *
I cdp15 20 35 0 "cdp" ly * *
I cdp16 26 41 0 "cdp" ly * *
I via12x1 14 23 0 "via12" ly * *
I via12x2 26 23 0 "via12" ly * *
I cdp17 20 30 0 "cdp" ly * *
I cpf2 22 23 0 "cpf" ly * *
P 14 5 * metal
P 14 12 * metal
W 12 3 16 14 vss
S 2 V metal P 1 P 2
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 3 P 4
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 5 P 6
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 f
S 2 V metal2 P 7 P 8
P 8 3 * ptie
P 8 17 * ptie
W 5 0 11 20
S 3 V ptie P 9 P 10
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 11 P 12
P 26 7 * ndif
P 26 18 * ndif
W 23 4 29 21
S 3 V ndif P 13 P 14
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 15 P 16
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 17 P 18
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 19 P 20
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 21 P 22
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 23 P 24
P 5 39 * nwell
P 47 39 * nwell
W -21 13 73 65
S 26 H nwell P 25 P 26
P 17 23 * poly
P 23 23 * poly
W 16 22 24 24
S 1 H poly P 27 P 28
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 29 P 30
P 23 5 * ntrans
P 23 20 * ntrans
W 22 4 24 21
S 1 V ntrans P 31 P 32
P 17 5 * ntrans
P 17 20 * ntrans
W 16 4 18 21
S 1 V ntrans P 33 P 34
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 35 P 36
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 37 P 38
P 8 45 * metal
P 14 45 * metal
W 6 43 16 47 vdd
S 2 H metal P 39 P 40
P 8 29 * metal
P 8 45 * metal
W 6 27 10 47 vdd
S 2 V metal P 41 P 42
P 8 3 * metal
P 8 17 * metal
W 6 1 10 19 vss
S 2 V metal P 43 P 44
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 45 P 46
P 35 25 * ptrans
P 35 51 * ptrans
W 34 24 36 52
S 1 V ptrans P 47 P 48
P 32 27 * pdif
P 32 49 * pdif
W 30 25 34 51
S 2 V pdif P 49 P 50
P 38 27 * pdif
P 38 49 * pdif
W 35 24 41 52
S 3 V pdif P 51 P 52
P 44 27 * ntie
P 44 49 * ntie
W 41 24 47 52
S 3 V ntie P 53 P 54
P 8 27 * ntie
P 8 49 * ntie
W 5 24 11 52
S 3 V ntie P 55 P 56
P 22 23 * metal
P 30 23 * metal
W 20 21 32 25
S 2 H metal P 57 P 58
P 20 28 * metal
P 20 35 * metal
W 18 26 22 37
S 2 V metal P 59 P 60
P 29 5 * ntrans
P 29 20 * ntrans
W 28 4 30 21
S 1 V ntrans P 61 P 62
P 35 5 * ntrans
P 35 20 * ntrans
W 34 4 36 21
S 1 V ntrans P 63 P 64
P 32 7 * ndif
P 32 18 * ndif
W 30 5 34 20
S 2 V ndif P 65 P 66
P 38 7 * ndif
P 38 18 * ndif
W 35 4 41 21
S 3 V ndif P 67 P 68
P 44 2 * ptie
P 44 18 * ptie
W 41 -1 47 21
S 3 V ptie P 69 P 70
P 20 17 * metal
P 32 17 * metal
W 18 15 34 19
S 2 H metal P 71 P 72
P 20 13 * metal
P 20 17 * metal
W 18 11 22 19
S 2 V metal P 73 P 74
P 32 13 * metal
P 32 17 * metal
W 30 11 34 19
S 2 V metal P 75 P 76
P 14 43 * metal
P 14 45 * metal
W 12 41 16 47 vdd
S 2 V metal P 77 P 78
P 44 43 * metal
P 44 45 * metal
W 43 42 45 46 vdd
S 1 V metal P 79 P 80
P 44 29 * metal
P 44 43 * metal
W 42 27 46 45 vdd
S 2 V metal P 81 P 82
P 44 43 * metal
P 47 43 * metal
W 36 35 55 51 vdd
S 8 H metal P 83 P 84
P 38 43 * metal
P 44 43 * metal
W 30 35 52 51 vdd
S 8 H metal P 85 P 86
P 38 43 * metal
P 38 45 * metal
W 36 41 40 47 vdd
S 2 V metal P 87 P 88
P 38 29 * metal
P 38 43 * metal
W 36 27 40 45 vdd
S 2 V metal P 89 P 90
P 26 43 * metal
P 38 43 * metal
W 18 35 46 51 vdd
S 8 H metal P 91 P 92
P 26 33 * metal
P 26 43 * metal
W 24 31 28 45 vdd
S 2 V metal P 93 P 94
P 14 43 * metal
P 26 43 * metal
W 6 35 34 51 vdd
S 8 H metal P 95 P 96
P 5 43 * metal
P 14 43 * metal
W -3 35 22 51 vdd
S 8 H metal P 97 P 98
P 14 33 * metal
P 14 43 * metal
W 12 31 16 45 vdd
S 2 V metal P 99 P 100
P 38 5 * metal
P 38 16 * metal
W 36 3 40 18 vss
S 2 V metal P 101 P 102
P 38 5 * metal
P 44 5 * metal
W 30 -3 52 13 vss
S 8 H metal P 103 P 104
P 44 5 * metal
P 47 5 * metal
W 36 -3 55 13 vss
S 8 H metal P 105 P 106
P 44 5 * metal
P 44 17 * metal
W 42 3 46 19 vss
S 2 V metal P 107 P 108
P 26 5 * metal
P 38 5 * metal
W 18 -3 46 13 vss
S 8 H metal P 109 P 110
P 5 5 * metal
P 26 5 * metal
W -3 -3 34 13 vss
S 8 H metal P 111 P 112
P 26 5 * metal
P 26 12 * metal
W 24 3 28 14 vss
S 2 V metal P 113 P 114
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 115 P 116
P 44 3 * allowM2
P 44 45 * allowM2
W 42 1 46 47
S 2 V allowM2 P 117 P 118
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 119 P 120
P 32 28 * metal
P 32 35 * metal
W 30 26 34 37
S 2 V metal P 121 P 122
P 17 28 * metal
P 32 28 * metal
W 15 26 34 30
S 2 H metal P 123 P 124
P 17 23 * metal
P 17 28 * metal
W 15 21 19 30
S 2 V metal P 125 P 126
P 14 23 * metal
P 17 23 * metal
W 12 21 19 25
S 2 H metal P 127 P 128
P 17 17 * metal
P 17 23 * metal
W 15 15 19 25
S 2 V metal P 129 P 130
P 17 17 * metal
P 20 17 * metal
W 15 15 22 19
S 2 H metal P 131 P 132
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 133 P 134
P 29 23 * poly
P 29 25 * poly
W 28 22 30 26
S 1 V poly P 135 P 136
P 29 20 * poly
P 29 23 * poly
W 28 19 30 24
S 1 V poly P 137 P 138
P 29 23 * poly
P 35 23 * poly
W 28 22 36 24
S 1 H poly P 139 P 140
P 35 20 * poly
P 35 23 * poly
W 34 19 36 24
S 1 V poly P 141 P 142
P 35 23 * poly
P 35 25 * poly
W 34 22 36 26
S 1 V poly P 143 P 144
E
