#cell1 a4_y idps_symb compose *
# 07-Oct-98 12:04 07-Oct-98 12:04 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 41 45
B 5 3 41 45
F F
C t 8 3 metal2 2 1 * * S t
C t 8 45 metal2 2 2 * * N t
C i3 14 3 metal2 2 3 * * S i3
C i3 14 45 metal2 2 4 * * N i3
C i2 32 45 metal2 2 5 * * N i2
C i2 32 3 metal2 2 6 * * S i2
C i1 26 3 metal2 2 7 * * S i1
C i1 26 45 metal2 2 8 * * N i1
C i0 20 45 metal2 2 9 * * N i0
C i0 20 3 metal2 2 10 * * S i0
C vss 5 5 metal 8 11 * * W vss
C vss 41 5 metal 8 12 * * E vss
C vdd 5 43 metal 8 13 * * W vdd
C vdd 41 43 metal 8 14 * * E vdd
I cdn1 14 9 0 "cdn" ly * *
I cbn1 38 45 0 "cbn" ly * *
I via12x1 8 21 0 "via12" ly * *
I cbp1 8 3 0 "cbp" ly * *
I via12x2 32 14 0 "via12" ly * *
I cpf1 13 24 0 "cpf" ly * *
I cdp1 8 28 0 "cdp" ly * *
I cdp2 8 33 0 "cdp" ly * *
I cdn2 8 14 0 "cdn" ly * *
I cbn2 20 45 0 "cbn" ly * *
I cdn3 14 5 0 "cdn" ly * *
I via12x3 14 14 0 "via12" ly * *
I via12x4 26 14 0 "via12" ly * *
I via12x5 20 14 0 "via12" ly * *
I cpf2 35 19 0 "cpf" ly * *
I cpf3 29 19 0 "cpf" ly * *
I cpf4 23 19 0 "cpf" ly * *
I cpf5 17 19 0 "cpf" ly * *
I cdp3 38 33 0 "cdp" ly * *
I cdp4 38 39 0 "cdp" ly * *
I cdp5 32 28 0 "cdp" ly * *
I cdp6 32 33 0 "cdp" ly * *
I cdp7 26 33 0 "cdp" ly * *
I cdp8 26 39 0 "cdp" ly * *
I cdp9 20 28 0 "cdp" ly * *
I cdp10 20 33 0 "cdp" ly * *
I cdp11 14 33 0 "cdp" ly * *
I cdp12 14 39 0 "cdp" ly * *
I cdn4 38 14 0 "cdn" ly * *
I cbn3 14 45 0 "cbn" ly * *
I cbn4 26 45 0 "cbn" ly * *
I cbn5 8 45 0 "cbn" ly * *
I cbn6 32 45 0 "cbn" ly * *
P 17 25 * ptrans
P 17 34 * ptrans
W 16 24 18 35
S 1 V ptrans P 1 P 2
P 23 25 * ptrans
P 23 34 * ptrans
W 22 24 24 35
S 1 V ptrans P 3 P 4
P 29 25 * ptrans
P 29 34 * ptrans
W 28 24 30 35
S 1 V ptrans P 5 P 6
P 35 25 * ptrans
P 35 34 * ptrans
W 34 24 36 35
S 1 V ptrans P 7 P 8
P 17 2 * ntrans
P 17 17 * ntrans
W 16 1 18 18
S 1 V ntrans P 9 P 10
P 23 2 * ntrans
P 23 17 * ntrans
W 22 1 24 18
S 1 V ntrans P 11 P 12
P 29 2 * ntrans
P 29 17 * ntrans
W 28 1 30 18
S 1 V ntrans P 13 P 14
P 35 2 * ntrans
P 35 17 * ntrans
W 34 1 36 18
S 1 V ntrans P 15 P 16
P 5 39 * nwell
P 41 39 * nwell
W -21 13 67 65
S 26 H nwell P 17 P 18
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 19 P 20
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 21 P 22
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47
S 2 V metal2 P 23 P 24
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 25 P 26
P 29 17 * poly
P 29 25 * poly
W 28 16 30 26
S 1 V poly P 27 P 28
P 23 17 * poly
P 23 25 * poly
W 22 16 24 26
S 1 V poly P 29 P 30
P 38 4 * ndif
P 38 15 * ndif
W 35 1 41 18
S 3 V ndif P 31 P 32
P 32 4 * ndif
P 32 15 * ndif
W 29 1 35 18
S 3 V ndif P 33 P 34
P 26 4 * ndif
P 26 15 * ndif
W 23 1 29 18
S 3 V ndif P 35 P 36
P 20 4 * ndif
P 20 15 * ndif
W 17 1 23 18
S 3 V ndif P 37 P 38
P 14 4 * ndif
P 14 15 * ndif
W 11 1 17 18
S 3 V ndif P 39 P 40
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 41 P 42
P 11 8 * ntrans
P 11 17 * ntrans
W 10 7 12 18
S 1 V ntrans P 43 P 44
P 8 10 * ndif
P 8 15 * ndif
W 5 7 11 18
S 3 V ndif P 45 P 46
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 47 P 48
P 35 17 * poly
P 35 25 * poly
W 34 16 36 26
S 1 V poly P 49 P 50
P 17 17 * poly
P 17 25 * poly
W 16 16 18 26
S 1 V poly P 51 P 52
P 11 17 * poly
P 11 25 * poly
W 10 16 12 26
S 1 V poly P 53 P 54
P 11 24 * poly
P 13 24 * poly
W 9 22 15 26
S 2 H poly P 55 P 56
P 33 19 * metal
P 35 19 * metal
W 31 17 37 21
S 2 H metal P 57 P 58
P 33 14 * metal
P 33 19 * metal
W 32 13 34 20
S 1 V metal P 59 P 60
P 32 14 * metal
P 33 14 * metal
W 30 12 35 16
S 2 H metal P 61 P 62
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 63 P 64
P 27 14 * metal
P 27 19 * metal
W 26 13 28 20
S 1 V metal P 65 P 66
P 27 19 * metal
P 29 19 * metal
W 25 17 31 21
S 2 H metal P 67 P 68
P 26 14 * metal
P 27 14 * metal
W 25 13 28 15
S 1 H metal P 69 P 70
P 20 14 * metal
P 21 14 * metal
W 18 12 23 16
S 2 H metal P 71 P 72
P 21 14 * metal
P 21 19 * metal
W 20 13 22 20
S 1 V metal P 73 P 74
P 21 19 * metal
P 23 19 * metal
W 19 17 25 21
S 2 H metal P 75 P 76
P 8 45 * ntie
P 38 45 * ntie
W 6 43 40 47
S 2 H ntie P 77 P 78
P 26 27 * pdif
P 26 39 * pdif
W 23 24 29 42
S 3 V pdif P 79 P 80
P 38 27 * pdif
P 38 39 * pdif
W 35 24 41 42
S 3 V pdif P 81 P 82
P 14 27 * pdif
P 14 39 * pdif
W 11 24 17 42
S 3 V pdif P 83 P 84
P 20 27 * pdif
P 20 33 * pdif
W 17 24 23 36
S 3 V pdif P 85 P 86
P 32 27 * pdif
P 32 33 * pdif
W 29 24 35 36
S 3 V pdif P 87 P 88
P 8 14 * metal
P 8 33 * metal
W 6 12 10 35
S 2 V metal P 89 P 90
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 91 P 92
P 38 33 * metal
P 38 43 * metal
W 36 31 40 45 vdd
S 2 V metal P 93 P 94
P 5 43 * metal
P 41 43 * metal
W -3 35 49 51 vdd
S 8 H metal P 95 P 96
P 14 33 * metal
P 14 43 * metal
W 12 31 16 45 vdd
S 2 V metal P 97 P 98
P 15 14 * metal
P 15 19 * metal
W 14 13 16 20
S 1 V metal P 99 P 100
P 15 19 * metal
P 17 19 * metal
W 13 17 19 21
S 2 H metal P 101 P 102
P 14 14 * metal
P 15 14 * metal
W 13 13 16 15
S 1 H metal P 103 P 104
P 39 14 * metal
P 39 27 * metal
W 38 13 40 28
S 1 V metal P 105 P 106
P 38 14 * metal
P 39 14 * metal
W 36 12 41 16
S 2 H metal P 107 P 108
P 20 27 * metal
P 20 33 * metal
W 18 25 22 35
S 2 V metal P 109 P 110
P 14 27 * metal
P 39 27 * metal
W 13 26 40 28
S 1 H metal P 111 P 112
P 14 24 * metal
P 14 27 * metal
W 13 23 15 28
S 1 V metal P 113 P 114
P 13 24 * metal
P 14 24 * metal
W 11 22 16 26
S 2 H metal P 115 P 116
P 32 27 * metal
P 32 33 * metal
W 30 25 34 35
S 2 V metal P 117 P 118
P 26 43 * metal
P 26 45 * metal
W 25 42 27 46 vdd
S 1 V metal P 119 P 120
P 8 45 * metal
P 38 45 * metal
W 7 44 39 46 vdd
S 1 H metal P 121 P 122
P 26 33 * metal
P 26 43 * metal
W 24 31 28 45 vdd
S 2 V metal P 123 P 124
P 8 3 * metal
P 8 5 * metal
W 7 2 9 6
S 1 V metal P 125 P 126
P 5 5 * metal
P 41 5 * metal
W -3 -3 49 13
S 8 H metal P 127 P 128
P 14 5 * metal
P 14 9 * metal
W 12 3 16 11
S 2 V metal P 129 P 130
E
