#cell1 nao4_y idps_symb compose *
# 07-Oct-98 12:17 07-Oct-98 12:17 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 41 45
B 5 3 41 45
F F
C i 38 45 metal2 2 1 * * N i
C i 38 3 metal2 2 2 * * S i
C f 32 45 metal2 2 3 * * N f
C f 32 3 metal2 2 4 * * S f
C j0 26 45 metal2 2 5 * * N j0
C j0 26 3 metal2 2 6 * * S j0
C j1 20 45 metal2 2 7 * * N j1
C j1 20 3 metal2 2 8 * * S j1
C j2 14 45 metal2 2 9 * * N j2
C j2 14 3 metal2 2 10 * * S j2
C vdd 41 43 metal 8 11 * * E vdd
C vss 41 5 metal 8 12 * * E vss
C vss 5 5 metal 8 13 * * W vss
C vdd 5 43 metal 8 14 * * W vdd
I cdp1 26 45 0 "cdp" ly * *
I cdp2 14 45 0 "cdp" ly * *
I cbn1 8 28 0 "cbn" ly * *
I cbn2 8 34 0 "cbn" ly * *
I cbn3 8 45 0 "cbn" ly * *
I cbn4 8 40 0 "cbn" ly * *
I cbp1 8 17 0 "cbp" ly * *
I cbp2 8 11 0 "cbp" ly * *
I cbp3 8 3 0 "cbp" ly * *
I cdn1 14 15 0 "cdn" ly * *
I cdn2 14 9 0 "cdn" ly * *
I cdn3 32 17 0 "cdn" ly * *
I cdn4 32 13 0 "cdn" ly * *
I cdn5 38 14 0 "cdn" ly * *
I cdp3 14 37 0 "cdp" ly * *
I cdp4 14 33 0 "cdp" ly * *
I cdp5 20 34 0 "cdp" ly * *
I cdp6 26 36 0 "cdp" ly * *
I cdp7 20 28 0 "cdp" ly * *
I cdp8 32 35 0 "cdp" ly * *
I cdp9 32 31 0 "cdp" ly * *
I cdp10 38 35 0 "cdp" ly * *
I cdp11 38 31 0 "cdp" ly * *
I cdp12 26 41 0 "cdp" ly * *
I cdp13 14 41 0 "cdp" ly * *
I cpf1 15 23 0 "cpf" ly * *
I cpf2 21 22 0 "cpf" ly * *
I cpf3 27 22 0 "cpf" ly * *
I cpf4 37 23 0 "cpf" ly * *
I via12x1 14 28 0 "via12" ly * *
I via12x2 20 17 0 "via12" ly * *
I via12x3 26 17 0 "via12" ly * *
I via12x4 32 22 0 "via12" ly * *
I via12x5 38 19 0 "via12" ly * *
P 32 4 * ndif
P 32 18 * ndif
W 29 1 35 21
S 3 V ndif P 1 P 2
P 8 5 * metal
P 8 17 * metal
W 6 3 10 19 vss
S 2 V metal P 3 P 4
P 5 5 * metal
P 41 5 * metal
W -3 -3 49 13 vss
S 8 H metal P 5 P 6
P 14 5 * metal
P 14 15 * metal
W 12 3 16 17 vss
S 2 V metal P 7 P 8
P 14 33 * metal
P 14 43 * metal
W 12 31 16 45 vdd
S 2 V metal P 9 P 10
P 5 43 * metal
P 41 43 * metal
W -3 35 49 51 vdd
S 8 H metal P 11 P 12
P 8 28 * metal
P 8 43 * metal
W 6 26 10 45 vdd
S 2 V metal P 13 P 14
P 32 31 * metal
P 32 35 * metal
W 30 29 34 37
S 2 V metal P 15 P 16
P 20 31 * metal
P 32 31 * metal
W 19 30 33 32
S 1 H metal P 17 P 18
P 20 28 * metal
P 20 34 * metal
W 18 26 22 36
S 2 V metal P 19 P 20
P 14 23 * metal
P 15 23 * metal
W 13 22 16 24
S 1 H metal P 21 P 22
P 14 23 * metal
P 14 28 * metal
W 13 22 15 29
S 1 V metal P 23 P 24
P 26 22 * metal
P 27 22 * metal
W 25 21 28 23
S 1 H metal P 25 P 26
P 26 17 * metal
P 26 22 * metal
W 25 16 27 23
S 1 V metal P 27 P 28
P 20 22 * metal
P 21 22 * metal
W 19 21 22 23
S 1 H metal P 29 P 30
P 20 17 * metal
P 20 22 * metal
W 19 16 21 23
S 1 V metal P 31 P 32
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 33 P 34
P 37 23 * metal
P 38 23 * metal
W 36 22 39 24
S 1 H metal P 35 P 36
P 38 19 * metal
P 38 23 * metal
W 36 17 40 25
S 2 V metal P 37 P 38
P 8 3 * ptie
P 8 17 * ptie
W 5 0 11 20
S 3 V ptie P 39 P 40
P 8 28 * ntie
P 8 43 * ntie
W 5 25 11 46
S 3 V ntie P 41 P 42
P 32 13 * metal
P 32 27 * metal
W 30 11 34 29
S 2 V metal P 43 P 44
P 32 27 * metal
P 38 27 * metal
W 31 26 39 28
S 1 H metal P 45 P 46
P 38 27 * metal
P 38 35 * metal
W 36 25 40 37
S 2 V metal P 47 P 48
P 38 13 * ndif
P 38 18 * ndif
W 35 10 41 21
S 3 V ndif P 49 P 50
P 15 23 * poly
P 17 23 * poly
W 13 21 19 25
S 2 H poly P 51 P 52
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 53 P 54
P 21 22 * poly
P 23 22 * poly
W 19 20 25 24
S 2 H poly P 55 P 56
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 57 P 58
P 27 22 * poly
P 29 22 * poly
W 25 20 31 24
S 2 H poly P 59 P 60
P 29 20 * poly
P 29 25 * poly
W 28 19 30 26
S 1 V poly P 61 P 62
P 5 39 * nwell
P 41 39 * nwell
W -21 13 67 65
S 26 H nwell P 63 P 64
P 26 4 * ndif
P 26 18 * ndif
W 23 1 29 21
S 3 V ndif P 65 P 66
P 20 4 * ndif
P 20 18 * ndif
W 17 1 23 21
S 3 V ndif P 67 P 68
P 14 4 * ndif
P 14 18 * ndif
W 11 1 17 21
S 3 V ndif P 69 P 70
P 38 27 * pdif
P 38 49 * pdif
W 35 24 41 52
S 3 V pdif P 71 P 72
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 73 P 74
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 75 P 76
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 77 P 78
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 79 P 80
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 j2
S 2 V metal2 P 81 P 82
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47 j1
S 2 V metal2 P 83 P 84
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 j0
S 2 V metal2 P 85 P 86
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47 f
S 2 V metal2 P 87 P 88
P 38 3 * metal2
P 38 45 * metal2
W 36 1 40 47 i
S 2 V metal2 P 89 P 90
P 35 11 * ntrans
P 35 20 * ntrans
W 34 10 36 21
S 1 V ntrans P 91 P 92
P 29 2 * ntrans
P 29 20 * ntrans
W 28 1 30 21
S 1 V ntrans P 93 P 94
P 23 2 * ntrans
P 23 20 * ntrans
W 22 1 24 21
S 1 V ntrans P 95 P 96
P 17 2 * ntrans
P 17 20 * ntrans
W 16 1 18 21
S 1 V ntrans P 97 P 98
P 35 25 * ptrans
P 35 51 * ptrans
W 34 24 36 52
S 1 V ptrans P 99 P 100
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 101 P 102
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 103 P 104
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 105 P 106
P 35 20 * poly
P 35 25 * poly
W 34 19 36 26
S 1 V poly P 107 P 108
P 35 23 * poly
P 37 23 * poly
W 33 21 39 25
S 2 H poly P 109 P 110
P 38 5 * metal
P 38 14 * metal
W 36 3 40 16 vss
S 2 V metal P 111 P 112
P 8 45 * metal
P 26 45 * metal
W 7 44 27 46 vdd
S 1 H metal P 113 P 114
P 26 43 * metal
P 26 45 * metal
W 25 42 27 46 vdd
S 1 V metal P 115 P 116
P 26 36 * metal
P 26 43 * metal
W 24 34 28 45 vdd
S 2 V metal P 117 P 118
P 8 3 * metal
P 8 5 * metal
W 7 2 9 6 vss
S 1 V metal P 119 P 120
E
