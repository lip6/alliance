#cell1 no3_y idps_symb compose *
# 07-Oct-98 12:20 07-Oct-98 12:20 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 35 45
B 5 3 35 45
F F
C vdd 5 43 metal 8 1 * * W vdd
C vss 5 5 metal 8 2 * * W vss
C f 32 3 metal2 2 3 * * S f
C f 32 45 metal2 2 4 * * N f
C i0 26 3 metal2 2 5 * * S i0
C i0 26 45 metal2 2 6 * * N i0
C i1 20 3 metal2 2 7 * * S i1
C i1 20 45 metal2 2 8 * * N i1
C i2 14 3 metal2 2 9 * * S i2
C i2 14 45 metal2 2 10 * * N i2
C vdd 35 43 metal 8 11 * * E vdd
C vss 35 5 metal 8 12 * * E vss
I cbp1 20 3 0 "cbp" ly * *
I cbp2 32 3 0 "cbp" ly * *
I cbn1 8 28 0 "cbn" ly * *
I cbn2 8 32 0 "cbn" ly * *
I cbn3 8 37 0 "cbn" ly * *
I cbn4 8 41 0 "cbn" ly * *
I cbn5 8 45 0 "cbn" ly * *
I cbp3 8 17 0 "cbp" ly * *
I cbp4 8 13 0 "cbp" ly * *
I cbp5 8 7 0 "cbp" ly * *
I cbp6 8 3 0 "cbp" ly * *
I cdn1 14 17 0 "cdn" ly * *
I cdn2 14 13 0 "cdn" ly * *
I cdn3 20 17 0 "cdn" ly * *
I cdn4 20 13 0 "cdn" ly * *
I cdn5 26 12 0 "cdn" ly * *
I cdn6 26 8 0 "cdn" ly * *
I cdn7 32 17 0 "cdn" ly * *
I cdn8 32 13 0 "cdn" ly * *
I cdp1 14 41 0 "cdp" ly * *
I cdp2 14 37 0 "cdp" ly * *
I cdp3 14 33 0 "cdp" ly * *
I cdp4 14 45 0 "cdp" ly * *
I cdp5 32 35 0 "cdp" ly * *
I cdp6 32 28 0 "cdp" ly * *
I cpf1 15 23 0 "cpf" ly * *
I cpf2 21 23 0 "cpf" ly * *
I cpf3 27 23 0 "cpf" ly * *
I via12x1 14 28 0 "via12" ly * *
I via12x2 20 28 0 "via12" ly * *
I via12x3 26 28 0 "via12" ly * *
I via12x4 32 20 0 "via12" ly * *
I cdn9 14 8 0 "cdn" ly * *
P 8 28 * metal
P 8 43 * metal
W 6 26 10 45 vdd
S 2 V metal P 1 P 2
P 5 43 * metal
P 35 43 * metal
W -3 35 43 51 vdd
S 8 H metal P 3 P 4
P 14 33 * metal
P 14 45 * metal
W 12 31 16 47 vdd
S 2 V metal P 5 P 6
P 14 5 * metal
P 14 17 * metal
W 12 3 16 19 vss
S 2 V metal P 7 P 8
P 26 5 * metal
P 26 12 * metal
W 24 3 28 14 vss
S 2 V metal P 9 P 10
P 5 5 * metal
P 35 5 * metal
W -3 -3 43 13 vss
S 8 H metal P 11 P 12
P 26 23 * metal
P 27 23 * metal
W 25 22 28 24
S 1 H metal P 13 P 14
P 26 23 * metal
P 26 28 * metal
W 25 22 27 29
S 1 V metal P 15 P 16
P 20 23 * metal
P 21 23 * metal
W 19 22 22 24
S 1 H metal P 17 P 18
P 20 23 * metal
P 20 28 * metal
W 19 22 21 29
S 1 V metal P 19 P 20
P 14 23 * metal
P 15 23 * metal
W 13 22 16 24
S 1 H metal P 21 P 22
P 14 23 * metal
P 14 28 * metal
W 13 22 15 29
S 1 V metal P 23 P 24
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 25 P 26
P 8 5 * ptie
P 8 17 * ptie
W 5 2 11 20
S 3 V ptie P 27 P 28
P 14 8 * ndif
P 14 18 * ndif
W 11 5 17 21
S 3 V ndif P 29 P 30
P 26 8 * ndif
P 26 18 * ndif
W 23 5 29 21
S 3 V ndif P 31 P 32
P 20 13 * ndif
P 20 18 * ndif
W 17 10 23 21
S 3 V ndif P 33 P 34
P 32 13 * ndif
P 32 18 * ndif
W 29 10 35 21
S 3 V ndif P 35 P 36
P 20 17 * metal
P 32 17 * metal
W 19 16 33 18
S 1 H metal P 37 P 38
P 15 23 * poly
P 17 23 * poly
W 13 21 19 25
S 2 H poly P 39 P 40
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 41 P 42
P 21 23 * poly
P 23 23 * poly
W 19 21 25 25
S 2 H poly P 43 P 44
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 45 P 46
P 27 23 * poly
P 29 23 * poly
W 25 21 31 25
S 2 H poly P 47 P 48
P 29 20 * poly
P 29 25 * poly
W 28 19 30 26
S 1 V poly P 49 P 50
P 8 28 * ntie
P 8 43 * ntie
W 5 25 11 46
S 3 V ntie P 51 P 52
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 53 P 54
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 55 P 56
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 57 P 58
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 59 P 60
P 5 39 * nwell
P 35 39 * nwell
W -21 13 61 65
S 26 H nwell P 61 P 62
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 i2
S 2 V metal2 P 63 P 64
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47 i1
S 2 V metal2 P 65 P 66
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 i0
S 2 V metal2 P 67 P 68
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47 f
S 2 V metal2 P 69 P 70
P 29 11 * ntrans
P 29 20 * ntrans
W 28 10 30 21
S 1 V ntrans P 71 P 72
P 23 11 * ntrans
P 23 20 * ntrans
W 22 10 24 21
S 1 V ntrans P 73 P 74
P 17 11 * ntrans
P 17 20 * ntrans
W 16 10 18 21
S 1 V ntrans P 75 P 76
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 77 P 78
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 79 P 80
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 81 P 82
P 20 13 * metal
P 20 17 * metal
W 18 11 22 19
S 2 V metal P 83 P 84
P 32 13 * metal
P 32 35 * metal
W 30 11 34 37
S 2 V metal P 85 P 86
P 8 3 * metal
P 32 3 * metal
W 6 1 34 5 vss
S 2 H metal P 87 P 88
P 8 3 * metal
P 8 17 * metal
W 6 1 10 19 vss
S 2 V metal P 89 P 90
P 8 45 * metal
P 14 45 * metal
W 7 44 15 46
S 1 H metal P 91 P 92
E
