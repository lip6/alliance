#cell1 na4_y idps_symb compose *
# 07-Oct-98 12:15 07-Oct-98 12:15 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 12 41 54
B 5 12 41 54
F F
C vdd 5 52 metal 8 1 * * W vdd
C vss 5 14 metal 8 2 * * W vss
C i3 14 12 metal2 2 3 * * S i3
C i2 20 12 metal2 2 4 * * S i2
C i1 26 12 metal2 2 5 * * S i1
C i0 32 12 metal2 2 6 * * S i0
C f 38 12 metal2 2 7 * * S f
C vss 41 14 metal 8 8 * * E vss
C f 38 54 metal2 2 9 * * N f
C i0 32 54 metal2 2 10 * * N i0
C i1 26 54 metal2 2 11 * * N i1
C i2 20 54 metal2 2 12 * * N i2
C i3 14 54 metal2 2 13 * * N i3
C vdd 41 52 metal 8 14 * * E vdd
I cbn1 38 54 0 "cbn" ly * *
I cbn2 32 54 0 "cbn" ly * *
I cbn3 26 54 0 "cbn" ly * *
I cbn4 20 54 0 "cbn" ly * *
I cbn5 14 54 0 "cbn" ly * *
I cbn6 8 54 0 "cbn" ly * *
I cbn7 8 47 0 "cbn" ly * *
I cbn8 8 40 0 "cbn" ly * *
I cbp1 8 26 0 "cbp" ly * *
I cbp2 8 20 0 "cbp" ly * *
I cbp3 8 12 0 "cbp" ly * *
I cdn1 14 26 0 "cdn" ly * *
I cdn2 38 23 0 "cdn" ly * *
I cdp1 14 48 0 "cdp" ly * *
I cdp2 14 42 0 "cdp" ly * *
I cdp3 20 43 0 "cdp" ly * *
I cdp4 20 37 0 "cdp" ly * *
I cdp5 26 48 0 "cdp" ly * *
I cdp6 26 42 0 "cdp" ly * *
I cdp7 32 43 0 "cdp" ly * *
I cdp8 32 37 0 "cdp" ly * *
I cdp9 38 48 0 "cdp" ly * *
I cdp10 38 42 0 "cdp" ly * *
I cpf1 15 31 0 "cpf" ly * *
I cpf2 21 31 0 "cpf" ly * *
I cpf3 27 31 0 "cpf" ly * *
I cpf4 33 31 0 "cpf" ly * *
I via12x1 20 26 0 "via12" ly * *
I via12x2 26 26 0 "via12" ly * *
I via12x3 32 26 0 "via12" ly * *
I via12x4 14 37 0 "via12" ly * *
I via12x5 38 31 0 "via12" ly * *
I cdn3 14 14 0 "cdn" ly * *
I cdn4 14 20 0 "cdn" ly * *
P 32 37 * metal
P 32 43 * metal
W 30 35 34 45
S 2 V metal P 1 P 2
P 20 38 * metal
P 20 43 * metal
W 18 36 22 45
S 2 V metal P 3 P 4
P 20 37 * metal
P 20 38 * metal
W 19 36 21 39
S 1 V metal P 5 P 6
P 20 37 * metal
P 38 37 * metal
W 19 36 39 38
S 1 H metal P 7 P 8
P 38 23 * metal
P 38 37 * metal
W 36 21 40 39
S 2 V metal P 9 P 10
P 14 14 * metal
P 14 26 * metal
W 12 12 16 28 vss
S 2 V metal P 11 P 12
P 5 14 * metal
P 41 14 * metal
W -3 6 49 22 vss
S 8 H metal P 13 P 14
P 5 52 * metal
P 41 52 * metal
W -3 44 49 60 vdd
S 8 H metal P 15 P 16
P 8 40 * metal
P 8 52 * metal
W 6 38 10 54 vdd
S 2 V metal P 17 P 18
P 14 42 * metal
P 14 52 * metal
W 12 40 16 54 vdd
S 2 V metal P 19 P 20
P 26 42 * metal
P 26 52 * metal
W 24 40 28 54 vdd
S 2 V metal P 21 P 22
P 15 31 * poly
P 17 31 * poly
W 13 29 19 33
S 2 H poly P 23 P 24
P 17 29 * poly
P 17 34 * poly
W 16 28 18 35
S 1 V poly P 25 P 26
P 21 31 * poly
P 23 31 * poly
W 19 29 25 33
S 2 H poly P 27 P 28
P 23 29 * poly
P 23 34 * poly
W 22 28 24 35
S 1 V poly P 29 P 30
P 27 31 * poly
P 29 31 * poly
W 25 29 31 33
S 2 H poly P 31 P 32
P 29 29 * poly
P 29 34 * poly
W 28 28 30 35
S 1 V poly P 33 P 34
P 33 31 * poly
P 35 31 * poly
W 31 29 37 33
S 2 H poly P 35 P 36
P 35 29 * poly
P 35 34 * poly
W 34 28 36 35
S 1 V poly P 37 P 38
P 8 12 * allowM2
P 8 54 * allowM2
W 6 10 10 56
S 2 V allowM2 P 39 P 40
P 8 12 * ptie
P 8 26 * ptie
W 5 9 11 29
S 3 V ptie P 41 P 42
P 38 12 * metal2
P 38 54 * metal2
W 36 10 40 56 f
S 2 V metal2 P 43 P 44
P 32 31 * metal
P 33 31 * metal
W 30 29 35 33
S 2 H metal P 45 P 46
P 32 26 * metal
P 32 31 * metal
W 31 25 33 32
S 1 V metal P 47 P 48
P 26 31 * metal
P 27 31 * metal
W 24 29 29 33
S 2 H metal P 49 P 50
P 26 26 * metal
P 26 31 * metal
W 25 25 27 32
S 1 V metal P 51 P 52
P 20 31 * metal
P 21 31 * metal
W 18 29 23 33
S 2 H metal P 53 P 54
P 20 26 * metal
P 20 31 * metal
W 19 25 21 32
S 1 V metal P 55 P 56
P 14 31 * metal
P 15 31 * metal
W 12 29 17 33
S 2 H metal P 57 P 58
P 14 31 * metal
P 14 37 * metal
W 13 30 15 38
S 1 V metal P 59 P 60
P 14 4 * ndif
P 14 27 * ndif
W 11 1 17 30
S 3 V ndif P 61 P 62
P 20 4 * ndif
P 20 27 * ndif
W 17 1 23 30
S 3 V ndif P 63 P 64
P 26 4 * ndif
P 26 27 * ndif
W 23 1 29 30
S 3 V ndif P 65 P 66
P 32 4 * ndif
P 32 27 * ndif
W 29 1 35 30
S 3 V ndif P 67 P 68
P 38 4 * ndif
P 38 27 * ndif
W 35 1 41 30
S 3 V ndif P 69 P 70
P 26 36 * pdif
P 26 47 * pdif
W 23 33 29 50
S 3 V pdif P 71 P 72
P 14 36 * pdif
P 14 47 * pdif
W 11 33 17 50
S 3 V pdif P 73 P 74
P 38 36 * pdif
P 38 47 * pdif
W 35 33 41 50
S 3 V pdif P 75 P 76
P 32 36 * pdif
P 32 47 * pdif
W 29 33 35 50
S 3 V pdif P 77 P 78
P 20 36 * pdif
P 20 47 * pdif
W 17 33 23 50
S 3 V pdif P 79 P 80
P 32 12 * metal2
P 32 54 * metal2
W 30 10 34 56 i0
S 2 V metal2 P 81 P 82
P 26 12 * metal2
P 26 54 * metal2
W 24 10 28 56 i1
S 2 V metal2 P 83 P 84
P 20 12 * metal2
P 20 54 * metal2
W 18 10 22 56 i2
S 2 V metal2 P 85 P 86
P 14 12 * metal2
P 14 54 * metal2
W 12 10 16 56 i3
S 2 V metal2 P 87 P 88
P 5 48 * nwell
P 41 48 * nwell
W -21 22 67 74
S 26 H nwell P 89 P 90
P 35 2 * ntrans
P 35 29 * ntrans
W 34 1 36 30
S 1 V ntrans P 91 P 92
P 29 2 * ntrans
P 29 29 * ntrans
W 28 1 30 30
S 1 V ntrans P 93 P 94
P 23 2 * ntrans
P 23 29 * ntrans
W 22 1 24 30
S 1 V ntrans P 95 P 96
P 17 2 * ntrans
P 17 29 * ntrans
W 16 1 18 30
S 1 V ntrans P 97 P 98
P 35 34 * ptrans
P 35 49 * ptrans
W 34 33 36 50
S 1 V ptrans P 99 P 100
P 29 34 * ptrans
P 29 49 * ptrans
W 28 33 30 50
S 1 V ptrans P 101 P 102
P 23 34 * ptrans
P 23 49 * ptrans
W 22 33 24 50
S 1 V ptrans P 103 P 104
P 17 34 * ptrans
P 17 49 * ptrans
W 16 33 18 50
S 1 V ptrans P 105 P 106
P 8 40 * ntie
P 8 54 * ntie
W 5 37 11 57
S 3 V ntie P 107 P 108
P 8 54 * ntie
P 38 54 * ntie
W 6 52 40 56
S 2 H ntie P 109 P 110
P 8 54 * metal
P 38 54 * metal
W 7 53 39 55 vdd
S 1 H metal P 111 P 112
P 38 52 * metal
P 38 54 * metal
W 37 51 39 55 vdd
S 1 V metal P 113 P 114
P 38 42 * metal
P 38 52 * metal
W 36 40 40 54 vdd
S 2 V metal P 115 P 116
P 8 12 * metal
P 8 26 * metal
W 6 10 10 28 vss
S 2 V metal P 117 P 118
E
