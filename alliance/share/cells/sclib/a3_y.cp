#cell1 a3_y idps_symb compose *
# 07-Oct-98 12:03 07-Oct-98 12:03 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 35 45
B 5 3 35 45
F F
C vdd 35 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 5 5 metal 8 3 * * W vss
C vss 35 5 metal 8 4 * * E vss
C i0 14 3 metal2 2 5 * * S i0
C i0 14 45 metal2 2 6 * * N i0
C i1 20 3 metal2 2 7 * * S i1
C i1 20 45 metal2 2 8 * * N i1
C i2 26 45 metal2 2 9 * * N i2
C i2 26 3 metal2 2 10 * * S i2
C t 8 3 metal2 2 11 * * S t
C t 8 45 metal2 2 12 * * N t
I cbn1 20 45 0 "cbn" ly * *
I cbn2 26 45 0 "cbn" ly * *
I cbp1 20 3 0 "cbp" ly * *
I cbp2 26 3 0 "cbp" ly * *
I cdn1 32 13 0 "cdn" ly * *
I cdp1 14 37 0 "cdp" ly * *
I cdp2 14 33 0 "cdp" ly * *
I cdp3 20 34 0 "cdp" ly * *
I cdp4 26 38 0 "cdp" ly * *
I cdp5 26 34 0 "cdp" ly * *
I cdp6 32 34 0 "cdp" ly * *
I cpf1 18 19 0 "cpf" ly * *
I cpf2 23 19 0 "cpf" ly * *
I cpf3 28 19 0 "cpf" ly * *
I via12x1 20 14 0 "via12" ly * *
I via12x2 26 14 0 "via12" ly * *
I cbn3 32 45 0 "cbn" ly * *
I cbp3 32 3 0 "cbp" ly * *
I cbp4 8 3 0 "cbp" ly * *
I cdn2 8 13 0 "cdn" ly * *
I cdp7 8 34 0 "cdp" ly * *
I cdp8 8 28 0 "cdp" ly * *
I via12x3 8 22 0 "via12" ly * *
I via12x4 14 19 0 "via12" ly * *
I cpf4 13 24 0 "cpf" ly * *
I cdn3 14 12 0 "cdn" ly * *
I cbn4 8 45 0 "cbn" ly * *
I cbn5 14 45 0 "cbn" ly * *
I cdp9 14 29 0 "cdp" ly * *
I cbp5 14 3 0 "cbp" ly * *
P 20 14 * metal
P 22 14 * metal
W 18 12 24 16
S 2 H metal P 1 P 2
P 22 14 * metal
P 22 19 * metal
W 21 13 23 20
S 1 V metal P 3 P 4
P 22 19 * metal
P 23 19 * metal
W 21 18 24 20
S 1 H metal P 5 P 6
P 32 33 * pdif
P 32 38 * pdif
W 29 30 35 41
S 3 V pdif P 7 P 8
P 17 19 * poly
P 18 19 * poly
W 16 18 19 20
S 1 H poly P 9 P 10
P 17 17 * poly
P 17 31 * poly
W 16 16 18 32
S 1 V poly P 11 P 12
P 32 13 * metal
P 32 34 * metal
W 31 12 33 35
S 1 V metal P 13 P 14
P 13 24 * metal
P 32 24 * metal
W 12 23 33 25
S 1 H metal P 15 P 16
P 20 24 * metal
P 20 34 * metal
W 19 23 21 35
S 1 V metal P 17 P 18
P 28 19 * poly
P 29 19 * poly
W 27 18 30 20
S 1 H poly P 19 P 20
P 29 17 * poly
P 29 31 * poly
W 28 16 30 32
S 1 V poly P 21 P 22
P 8 13 * metal
P 8 34 * metal
W 7 12 9 35
S 1 V metal P 23 P 24
P 5 5 * metal
P 35 5 * metal
W -3 -3 43 13 vss
S 8 H metal P 25 P 26
P 26 34 * metal
P 26 43 * metal
W 24 32 28 45 vdd
S 2 V metal P 27 P 28
P 5 43 * metal
P 35 43 * metal
W -3 35 43 51 vdd
S 8 H metal P 29 P 30
P 26 14 * metal
P 27 14 * metal
W 25 13 28 15
S 1 H metal P 31 P 32
P 27 14 * metal
P 27 19 * metal
W 26 13 28 20
S 1 V metal P 33 P 34
P 27 19 * metal
P 28 19 * metal
W 26 18 29 20
S 1 H metal P 35 P 36
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 37 P 38
P 8 45 * ntie
P 32 45 * ntie
W 6 43 34 47
S 2 H ntie P 39 P 40
P 14 19 * metal
P 18 19 * metal
W 12 17 20 21
S 2 H metal P 41 P 42
P 14 10 * ndif
P 14 15 * ndif
W 11 7 17 18
S 3 V ndif P 43 P 44
P 8 3 * ptie
P 32 3 * ptie
W 6 1 34 5
S 2 H ptie P 45 P 46
P 11 24 * poly
P 13 24 * poly
W 9 22 15 26
S 2 H poly P 47 P 48
P 11 17 * poly
P 11 25 * poly
W 10 16 12 26
S 1 V poly P 49 P 50
P 5 39 * nwell
P 35 39 * nwell
W -21 13 61 65
S 26 H nwell P 51 P 52
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 53 P 54
P 14 27 * pdif
P 14 38 * pdif
W 11 24 17 41
S 3 V pdif P 55 P 56
P 26 33 * pdif
P 26 38 * pdif
W 23 30 29 41
S 3 V pdif P 57 P 58
P 32 10 * ndif
P 32 15 * ndif
W 29 7 35 18
S 3 V ndif P 59 P 60
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 61 P 62
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 63 P 64
P 8 10 * ndif
P 8 15 * ndif
W 5 7 11 18
S 3 V ndif P 65 P 66
P 11 8 * ntrans
P 11 17 * ntrans
W 10 7 12 18
S 1 V ntrans P 67 P 68
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 69 P 70
P 20 33 * pdif
P 20 38 * pdif
W 17 30 23 41
S 3 V pdif P 71 P 72
P 20 10 * ndif
P 20 15 * ndif
W 17 7 23 18
S 3 V ndif P 73 P 74
P 26 10 * ndif
P 26 15 * ndif
W 23 7 29 18
S 3 V ndif P 75 P 76
P 23 17 * poly
P 23 31 * poly
W 22 16 24 32
S 1 V poly P 77 P 78
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 79 P 80
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 81 P 82
P 29 8 * ntrans
P 29 17 * ntrans
W 28 7 30 18
S 1 V ntrans P 83 P 84
P 23 8 * ntrans
P 23 17 * ntrans
W 22 7 24 18
S 1 V ntrans P 85 P 86
P 17 8 * ntrans
P 17 17 * ntrans
W 16 7 18 18
S 1 V ntrans P 87 P 88
P 29 31 * ptrans
P 29 40 * ptrans
W 28 30 30 41
S 1 V ptrans P 89 P 90
P 23 31 * ptrans
P 23 40 * ptrans
W 22 30 24 41
S 1 V ptrans P 91 P 92
P 17 31 * ptrans
P 17 40 * ptrans
W 16 30 18 41
S 1 V ptrans P 93 P 94
P 14 29 * metal
P 14 45 * metal
W 12 27 16 47 vdd
S 2 V metal P 95 P 96
P 8 45 * metal
P 32 45 * metal
W 6 43 34 47 vdd
S 2 H metal P 97 P 98
P 8 3 * metal
P 32 3 * metal
W 6 1 34 5 vss
S 2 H metal P 99 P 100
P 14 3 * metal
P 14 12 * metal
W 12 1 16 14 vss
S 2 V metal P 101 P 102
E
