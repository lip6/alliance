#cell1 nop2_y idps_symb compose *
# 07-Oct-98 12:22 07-Oct-98 12:22 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 41 45
B 5 3 41 45
F F
C vdd 5 43 metal 8 1 * * W vdd
C vss 5 5 metal 8 2 * * W vss
C f 8 3 metal2 2 3 * * S f
C i0 32 3 metal2 2 4 * * S i0
C i1 26 3 metal2 2 5 * * S i1
C f 8 45 metal2 2 6 * * N f
C i0 32 45 metal2 2 7 * * N i0
C i1 26 45 metal2 2 8 * * N i1
C vdd 41 43 metal 8 9 * * E vdd
C vss 41 5 metal 8 10 * * E vss
I cbn1 20 43 0 "cbn" ly * *
I cdn1 26 14 0 "cdn" ly * *
I cdn2 26 10 0 "cdn" ly * *
I cdn3 38 10 0 "cdn" ly * *
I cdp1 26 33 0 "cdp" ly * *
I cdp2 26 37 0 "cdp" ly * *
I cdp3 26 41 0 "cdp" ly * *
I cdp4 26 45 0 "cdp" ly * *
I cdp5 38 34 0 "cdp" ly * *
I cdp6 38 28 0 "cdp" ly * *
I cpf1 28 23 0 "cpf" ly * *
I cpf2 34 23 0 "cpf" ly * *
I via12x1 26 28 0 "via12" ly * *
I via12x2 32 28 0 "via12" ly * *
I via12x3 8 21 0 "via12" ly * *
I cdp7 20 28 0 "cdp" ly * *
I cdp8 14 28 0 "cdp" ly * *
I cdp9 8 28 0 "cdp" ly * *
I cdp10 20 34 0 "cdp" ly * *
I cdp11 14 32 0 "cdp" ly * *
I cdp12 8 34 0 "cdp" ly * *
I cdp13 14 37 0 "cdp" ly * *
I cdp14 14 42 0 "cdp" ly * *
I cdp15 14 46 0 "cdp" ly * *
I cdn4 20 14 0 "cdn" ly * *
I cdn5 14 17 0 "cdn" ly * *
I cdn6 14 13 0 "cdn" ly * *
I cdn7 14 8 0 "cdn" ly * *
I cdn8 8 13 0 "cdn" ly * *
I cdn9 8 17 0 "cdn" ly * *
I cpf3 15 22 0 "cpf" ly * *
I cbp1 20 3 0 "cbp" ly * *
I cbp2 26 3 0 "cbp" ly * *
I cbp3 32 3 0 "cbp" ly * *
I cbp4 38 3 0 "cbp" ly * *
I cpf4 23 19 0 "cpf" ly * *
I cdn10 38 14 0 "cdn" ly * *
I cdn11 32 13 0 "cdn" ly * *
P 26 43 * metal
P 26 45 * metal
W 25 42 27 46 vdd
S 1 V metal P 1 P 2
P 26 33 * metal
P 26 43 * metal
W 24 31 28 45 vdd
S 2 V metal P 3 P 4
P 5 43 * metal
P 41 43 * metal
W -3 35 49 51 vdd
S 8 H metal P 5 P 6
P 14 28 * metal
P 14 43 * metal
W 12 26 16 45 vdd
S 2 V metal P 7 P 8
P 14 43 * metal
P 14 46 * metal
W 13 42 15 47 vdd
S 1 V metal P 9 P 10
P 32 28 * metal
P 33 28 * metal
W 31 27 34 29
S 1 H metal P 11 P 12
P 33 23 * metal
P 33 28 * metal
W 32 22 34 29
S 1 V metal P 13 P 14
P 33 23 * metal
P 34 23 * metal
W 32 22 35 24
S 1 H metal P 15 P 16
P 34 23 * poly
P 35 23 * poly
W 33 22 36 24
S 1 H poly P 17 P 18
P 35 17 * poly
P 35 25 * poly
W 34 16 36 26
S 1 V poly P 19 P 20
P 28 23 * poly
P 29 23 * poly
W 27 22 30 24
S 1 H poly P 21 P 22
P 29 17 * poly
P 29 25 * poly
W 28 16 30 26
S 1 V poly P 23 P 24
P 11 22 * poly
P 15 22 * poly
W 10 21 16 23
S 1 H poly P 25 P 26
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 27 P 28
P 14 5 * metal
P 14 17 * metal
W 12 3 16 19 vss
S 2 V metal P 29 P 30
P 5 5 * metal
P 41 5 * metal
W -3 -3 49 13 vss
S 8 H metal P 31 P 32
P 8 7 * ndif
P 8 18 * ndif
W 5 4 11 21
S 3 V ndif P 33 P 34
P 38 28 * metal
P 38 34 * metal
W 36 26 40 36
S 2 V metal P 35 P 36
P 38 19 * metal
P 38 28 * metal
W 37 18 39 29
S 1 V metal P 37 P 38
P 32 13 * metal
P 32 19 * metal
W 31 12 33 20
S 1 V metal P 39 P 40
P 23 19 * metal
P 38 19 * metal
W 22 18 39 20
S 1 H metal P 41 P 42
P 19 14 * metal
P 20 14 * metal
W 18 13 21 15
S 1 H metal P 43 P 44
P 19 14 * metal
P 19 28 * metal
W 18 13 20 29
S 1 V metal P 45 P 46
P 15 22 * metal
P 19 22 * metal
W 14 21 20 23
S 1 H metal P 47 P 48
P 19 28 * metal
P 20 28 * metal
W 18 27 21 29
S 1 H metal P 49 P 50
P 20 28 * metal
P 20 34 * metal
W 18 26 22 36
S 2 V metal P 51 P 52
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47 i0
S 2 V metal2 P 53 P 54
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 55 P 56
P 14 3 * allowM2
P 14 45 * allowM2
W 12 1 16 47
S 2 V allowM2 P 57 P 58
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 59 P 60
P 38 10 * ndif
P 38 15 * ndif
W 35 7 41 18
S 3 V ndif P 61 P 62
P 20 3 * ptie
P 38 3 * ptie
W 18 1 40 5
S 2 H ptie P 63 P 64
P 20 10 * ndif
P 20 15 * ndif
W 17 7 23 18
S 3 V ndif P 65 P 66
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 67 P 68
P 26 9 * ndif
P 26 15 * ndif
W 23 6 29 18
S 3 V ndif P 69 P 70
P 8 13 * metal
P 8 34 * metal
W 6 11 10 36
S 2 V metal P 71 P 72
P 27 23 * metal
P 28 23 * metal
W 26 22 29 24
S 1 H metal P 73 P 74
P 27 23 * metal
P 27 28 * metal
W 26 22 28 29
S 1 V metal P 75 P 76
P 26 28 * metal
P 27 28 * metal
W 25 27 28 29
S 1 H metal P 77 P 78
P 8 27 * pdif
P 8 49 * pdif
W 5 24 11 52
S 3 V pdif P 79 P 80
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 81 P 82
P 20 27 * pdif
P 20 38 * pdif
W 17 24 23 41
S 3 V pdif P 83 P 84
P 32 10 * ndif
P 32 15 * ndif
W 29 7 35 18
S 3 V ndif P 85 P 86
P 23 17 * poly
P 23 25 * poly
W 22 16 24 26
S 1 V poly P 87 P 88
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 89 P 90
P 11 5 * ntrans
P 11 20 * ntrans
W 10 4 12 21
S 1 V ntrans P 91 P 92
P 11 25 * ptrans
P 11 51 * ptrans
W 10 24 12 52
S 1 V ptrans P 93 P 94
P 23 8 * ntrans
P 23 17 * ntrans
W 22 7 24 18
S 1 V ntrans P 95 P 96
P 23 25 * ptrans
P 23 40 * ptrans
W 22 24 24 41
S 1 V ptrans P 97 P 98
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 99 P 100
P 38 27 * pdif
P 38 49 * pdif
W 35 24 41 52
S 3 V pdif P 101 P 102
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 i1
S 2 V metal2 P 103 P 104
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 f
S 2 V metal2 P 105 P 106
P 5 39 * nwell
P 41 39 * nwell
W -21 13 67 65
S 26 H nwell P 107 P 108
P 35 8 * ntrans
P 35 17 * ntrans
W 34 7 36 18
S 1 V ntrans P 109 P 110
P 29 8 * ntrans
P 29 17 * ntrans
W 28 7 30 18
S 1 V ntrans P 111 P 112
P 35 25 * ptrans
P 35 51 * ptrans
W 34 24 36 52
S 1 V ptrans P 113 P 114
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 115 P 116
P 38 5 * metal
P 38 14 * metal
W 36 3 40 16 vss
S 2 V metal P 117 P 118
P 26 3 * metal
P 26 14 * metal
W 24 1 28 16 vss
S 2 V metal P 119 P 120
P 20 3 * metal
P 38 3 * metal
W 18 1 40 5 vss
S 2 H metal P 121 P 122
E
