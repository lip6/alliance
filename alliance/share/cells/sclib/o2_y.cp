#cell1 o2_y idps_symb compose *
# 07-Oct-98 12:26 07-Oct-98 12:26 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 29 45
B 5 3 29 45
F F
C vdd 29 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 29 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
C t 8 45 metal2 2 5 * * N t
C t 8 3 metal2 2 6 * * S t
C i0 14 45 metal2 2 7 * * N i0
C i0 14 3 metal2 2 8 * * S i0
C i1 20 45 metal2 2 9 * * N i1
C i1 20 3 metal2 2 10 * * S i1
I cbp1 26 3 0 "cbp" ly * *
I cbp2 14 3 0 "cbp" ly * *
I cdn1 26 13 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdp1 14 40 0 "cdp" ly * *
I cdp2 26 29 0 "cdp" ly * *
I cpf1 18 23 0 "cpf" ly * *
I cpf2 23 23 0 "cpf" ly * *
I via12x1 20 33 0 "via12" ly * *
I cdn3 14 10 0 "cdn" ly * *
I cdn4 14 15 0 "cdn" ly * *
I cdp3 8 29 0 "cdp" ly * *
I cdp4 8 34 0 "cdp" ly * *
I cdp5 14 34 0 "cdp" ly * *
I via12x2 8 21 0 "via12" ly * *
I via12x3 14 29 0 "via12" ly * *
I cpf3 13 21 0 "cpf" ly * *
I cbp3 20 3 0 "cbp" ly * *
I cbn1 8 45 0 "cbn" ly * *
I cbp4 8 3 0 "cbp" ly * *
I cdn5 8 15 0 "cdn" ly * *
I cdp6 26 35 0 "cdp" ly * *
P 11 21 * poly
P 13 21 * poly
W 9 19 15 23
S 2 H poly P 1 P 2
P 11 19 * poly
P 11 25 * poly
W 10 18 12 26
S 1 V poly P 3 P 4
P 17 23 * poly
P 18 23 * poly
W 16 22 19 24
S 1 H poly P 5 P 6
P 17 17 * poly
P 17 25 * poly
W 16 16 18 26
S 1 V poly P 7 P 8
P 14 34 * metal
P 14 43 * metal
W 12 32 16 45 vdd
S 2 V metal P 9 P 10
P 20 13 * metal
P 20 19 * metal
W 19 12 21 20
S 1 V metal P 11 P 12
P 13 19 * metal
P 13 21 * metal
W 11 17 15 23
S 2 V metal P 13 P 14
P 13 19 * metal
P 27 19 * metal
W 12 18 28 20
S 1 H metal P 15 P 16
P 27 19 * metal
P 27 29 * metal
W 26 18 28 30
S 1 V metal P 17 P 18
P 26 29 * metal
P 27 29 * metal
W 25 28 28 30
S 1 H metal P 19 P 20
P 26 29 * metal
P 26 35 * metal
W 25 28 27 36
S 1 V metal P 21 P 22
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 23 P 24
P 26 5 * metal
P 26 13 * metal
W 24 3 28 15 vss
S 2 V metal P 25 P 26
P 22 23 * metal
P 23 23 * metal
W 21 22 24 24
S 1 H metal P 27 P 28
P 20 32 * metal
P 20 33 * metal
W 19 31 21 34
S 1 V metal P 29 P 30
P 20 32 * metal
P 22 32 * metal
W 19 31 23 33
S 1 H metal P 31 P 32
P 22 23 * metal
P 22 32 * metal
W 21 22 23 33
S 1 V metal P 33 P 34
P 18 23 * metal
P 18 28 * metal
W 17 22 19 29
S 1 V metal P 35 P 36
P 14 28 * metal
P 18 28 * metal
W 13 27 19 29
S 1 H metal P 37 P 38
P 14 28 * metal
P 14 29 * metal
W 13 27 15 30
S 1 V metal P 39 P 40
P 8 15 * metal
P 8 34 * metal
W 6 13 10 36
S 2 V metal P 41 P 42
P 26 3 * allowM2
P 26 45 * allowM2
W 24 1 28 47
S 2 V allowM2 P 43 P 44
P 26 27 * pdif
P 26 42 * pdif
W 23 24 29 45
S 3 V pdif P 45 P 46
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 47 P 48
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 i0
S 2 V metal2 P 49 P 50
P 8 12 * ndif
P 8 17 * ndif
W 5 9 11 20
S 3 V ndif P 51 P 52
P 26 12 * ndif
P 26 15 * ndif
W 23 9 29 18
S 3 V ndif P 53 P 54
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 55 P 56
P 20 12 * ndif
P 20 15 * ndif
W 17 9 23 18
S 3 V ndif P 57 P 58
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 59 P 60
P 14 27 * pdif
P 14 42 * pdif
W 11 24 17 45
S 3 V pdif P 61 P 62
P 14 10 * ndif
P 14 17 * ndif
W 11 7 17 20
S 3 V ndif P 63 P 64
P 11 10 * ntrans
P 11 19 * ntrans
W 10 9 12 20
S 1 V ntrans P 65 P 66
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 67 P 68
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 69 P 70
P 23 17 * poly
P 23 25 * poly
W 22 16 24 26
S 1 V poly P 71 P 72
P 20 27 * pdif
P 20 42 * pdif
W 17 24 23 45
S 3 V pdif P 73 P 74
P 23 10 * ntrans
P 23 17 * ntrans
W 22 9 24 18
S 1 V ntrans P 75 P 76
P 17 10 * ntrans
P 17 17 * ntrans
W 16 9 18 18
S 1 V ntrans P 77 P 78
P 23 25 * ptrans
P 23 44 * ptrans
W 22 24 24 45
S 1 V ptrans P 79 P 80
P 17 25 * ptrans
P 17 44 * ptrans
W 16 24 18 45
S 1 V ptrans P 81 P 82
P 8 3 * ptie
P 26 3 * ptie
W 6 1 28 5
S 2 H ptie P 83 P 84
P 8 43 * metal
P 8 45 * metal
W 7 42 9 46 vdd
S 1 V metal P 85 P 86
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 87 P 88
P 8 3 * metal
P 26 3 * metal
W 6 1 28 5 vss
S 2 H metal P 89 P 90
P 14 3 * metal
P 14 15 * metal
W 12 1 16 17 vss
S 2 V metal P 91 P 92
E
