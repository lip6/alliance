#cell1 op2_y idps_symb compose *
# 07-Oct-98 12:28 07-Oct-98 12:28 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 35 45
B 5 3 35 45
F F
C vdd 35 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 35 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
C t 14 45 metal2 2 5 * * N t
C t 14 3 metal2 2 6 * * S t
C i0 20 45 metal2 2 7 * * N i0
C i0 20 3 metal2 2 8 * * S i0
C i1 26 45 metal2 2 9 * * N i1
C i1 26 3 metal2 2 10 * * S i1
I cbp1 32 3 0 "cbp" ly * *
I cbn1 8 37 0 "cbn" ly * *
I cbn2 8 41 0 "cbn" ly * *
I cbp2 8 7 0 "cbp" ly * *
I cbp3 8 3 0 "cbp" ly * *
I cdn1 32 14 0 "cdn" ly * *
I cdn2 26 14 0 "cdn" ly * *
I cdp1 20 40 0 "cdp" ly * *
I cdp2 32 29 0 "cdp" ly * *
I cpf1 24 23 0 "cpf" ly * *
I cpf2 29 23 0 "cpf" ly * *
I via12x1 26 33 0 "via12" ly * *
I cdn3 20 14 0 "cdn" ly * *
I cdp3 14 28 0 "cdp" ly * *
I cdp4 14 34 0 "cdp" ly * *
I cdp5 20 34 0 "cdp" ly * *
I via12x2 14 21 0 "via12" ly * *
I via12x3 20 29 0 "via12" ly * *
I cpf3 19 23 0 "cpf" ly * *
I cbp4 26 3 0 "cbp" ly * *
I cbn3 8 45 0 "cbn" ly * *
I cbp5 8 16 0 "cbp" ly * *
I cdn4 14 13 0 "cdn" ly * *
I cdp6 32 35 0 "cdp" ly * *
I cdp7 20 46 0 "cdp" ly * *
I cdn5 20 7 0 "cdn" ly * *
I cbn4 8 28 0 "cbn" ly * *
I cbn5 8 33 0 "cbn" ly * *
I cbp6 8 11 0 "cbp" ly * *
P 5 39 * nwell
P 35 39 * nwell
W -21 13 61 65
S 26 H nwell P 1 P 2
P 20 5 * metal
P 20 14 * metal
W 18 3 22 16 vss
S 2 V metal P 3 P 4
P 32 5 * metal
P 32 14 * metal
W 30 3 34 16 vss
S 2 V metal P 5 P 6
P 5 5 * metal
P 35 5 * metal
W -3 -3 43 13 vss
S 8 H metal P 7 P 8
P 32 29 * metal
P 32 35 * metal
W 31 28 33 36
S 1 V metal P 9 P 10
P 32 29 * metal
P 33 29 * metal
W 31 28 34 30
S 1 H metal P 11 P 12
P 33 18 * metal
P 33 29 * metal
W 32 17 34 30
S 1 V metal P 13 P 14
P 26 14 * metal
P 26 18 * metal
W 24 12 28 20
S 2 V metal P 15 P 16
P 19 18 * metal
P 33 18 * metal
W 18 17 34 19
S 1 H metal P 17 P 18
P 19 18 * metal
P 19 23 * metal
W 18 17 20 24
S 1 V metal P 19 P 20
P 5 43 * metal
P 35 43 * metal
W -3 35 43 51 vdd
S 8 H metal P 21 P 22
P 20 34 * metal
P 20 46 * metal
W 18 32 22 48 vdd
S 2 V metal P 23 P 24
P 26 32 * metal
P 26 33 * metal
W 25 31 27 34
S 1 V metal P 25 P 26
P 26 32 * metal
P 28 32 * metal
W 25 31 29 33
S 1 H metal P 27 P 28
P 28 23 * metal
P 28 32 * metal
W 27 22 29 33
S 1 V metal P 29 P 30
P 28 23 * metal
P 29 23 * metal
W 27 22 30 24
S 1 H metal P 31 P 32
P 24 23 * metal
P 24 28 * metal
W 23 22 25 29
S 1 V metal P 33 P 34
P 20 28 * metal
P 24 28 * metal
W 19 27 25 29
S 1 H metal P 35 P 36
P 20 28 * metal
P 20 29 * metal
W 19 27 21 30
S 1 V metal P 37 P 38
P 14 13 * metal
P 14 34 * metal
W 12 11 16 36
S 2 V metal P 39 P 40
P 23 23 * poly
P 24 23 * poly
W 22 22 25 24
S 1 H poly P 41 P 42
P 23 19 * poly
P 23 25 * poly
W 22 18 24 26
S 1 V poly P 43 P 44
P 17 23 * poly
P 19 23 * poly
W 15 21 21 25
S 2 H poly P 45 P 46
P 17 19 * poly
P 17 25 * poly
W 16 18 18 26
S 1 V poly P 47 P 48
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 49 P 50
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 51 P 52
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47 i0
S 2 V metal2 P 53 P 54
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 55 P 56
P 26 12 * ndif
P 26 17 * ndif
W 23 9 29 20
S 3 V ndif P 57 P 58
P 8 3 * ptie
P 8 16 * ptie
W 5 0 11 19
S 3 V ptie P 59 P 60
P 14 6 * ndif
P 14 17 * ndif
W 11 3 17 20
S 3 V ndif P 61 P 62
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 63 P 64
P 20 6 * ndif
P 20 17 * ndif
W 17 3 23 20
S 3 V ndif P 65 P 66
P 17 4 * ntrans
P 17 19 * ntrans
W 16 3 18 20
S 1 V ntrans P 67 P 68
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 69 P 70
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 71 P 72
P 29 19 * poly
P 29 25 * poly
W 28 18 30 26
S 1 V poly P 73 P 74
P 32 12 * ndif
P 32 17 * ndif
W 29 9 35 20
S 3 V ndif P 75 P 76
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 77 P 78
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 79 P 80
P 32 27 * pdif
P 32 49 * pdif
W 29 24 35 52
S 3 V pdif P 81 P 82
P 29 10 * ntrans
P 29 19 * ntrans
W 28 9 30 20
S 1 V ntrans P 83 P 84
P 23 10 * ntrans
P 23 19 * ntrans
W 22 9 24 20
S 1 V ntrans P 85 P 86
P 29 25 * ptrans
P 29 51 * ptrans
W 28 24 30 52
S 1 V ptrans P 87 P 88
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 89 P 90
P 8 28 * metal
P 8 45 * metal
W 6 26 10 47 vdd
S 2 V metal P 91 P 92
P 8 28 * ntie
P 8 45 * ntie
W 5 25 11 48
S 3 V ntie P 93 P 94
P 26 3 * ptie
P 32 3 * ptie
W 24 1 34 5
S 2 H ptie P 95 P 96
P 8 3 * metal
P 8 16 * metal
W 6 1 10 18 vss
S 2 V metal P 97 P 98
P 8 3 * metal
P 32 3 * metal
W 6 1 34 5 vss
S 2 H metal P 99 P 100
E
