#cell1 b1_y idps_symb compose *
# 07-Oct-98 12:06 07-Oct-98 12:06 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 29 45
B 5 3 29 45
F F
C t 20 3 metal2 2 1 * * S t
C t 20 45 metal2 2 2 * * N t
C i 14 3 metal2 2 3 * * S i
C i 14 45 metal2 2 4 * * N i
C vss 5 5 metal 8 5 * * W vss
C vss 29 5 metal 8 6 * * E vss
C vdd 5 43 metal 8 7 * * W vdd
C vdd 29 43 metal 8 8 * * E vdd
I cdp1 20 30 0 "cdp" ly * *
I cpf1 12 24 0 "cpf" ly * *
I cdp2 26 30 0 "cdp" ly * *
I cdp3 26 36 0 "cdp" ly * *
I cdp4 26 41 0 "cdp" ly * *
I cdp5 26 45 0 "cdp" ly * *
I cdn1 26 8 0 "cdn" ly * *
I cdn2 26 13 0 "cdn" ly * *
I cdn3 26 17 0 "cdn" ly * *
I via12x1 14 28 0 "via12" ly * *
I cbn1 8 46 0 "cbn" ly * *
I cbp1 8 3 0 "cbp" ly * *
I cdp6 8 35 0 "cdp" ly * *
I cdp7 14 33 0 "cdp" ly * *
I cdp8 8 29 0 "cdp" ly * *
I cdn4 8 17 0 "cdn" ly * *
I cdn5 8 13 0 "cdn" ly * *
I cdn6 14 8 0 "cdn" ly * *
I cdp9 14 45 0 "cdp" ly * *
I via12x2 20 27 0 "via12" ly * *
I cpf2 17 22 0 "cpf" ly * *
I cdp10 20 34 0 "cdp" ly * *
I cdp11 14 39 0 "cdp" ly * *
I cdn7 20 13 0 "cdn" ly * *
I cdn8 14 14 0 "cdn" ly * *
P 17 20 * metal
P 17 22 * metal
W 15 18 19 24
S 2 V metal P 1 P 2
P 16 20 * metal
P 17 20 * metal
W 15 19 18 21
S 1 H metal P 3 P 4
P 16 19 * metal
P 16 20 * metal
W 15 18 17 21
S 1 V metal P 5 P 6
P 8 19 * metal
P 16 19 * metal
W 7 18 17 20
S 1 H metal P 7 P 8
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 9 P 10
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 11 P 12
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 13 P 14
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 15 P 16
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 17 P 18
P 8 27 * pdif
P 8 41 * pdif
W 5 24 11 44
S 3 V pdif P 19 P 20
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 21 P 22
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 23 P 24
P 17 5 * ntrans
P 17 20 * ntrans
W 16 4 18 21
S 1 V ntrans P 25 P 26
P 23 5 * ntrans
P 23 20 * ntrans
W 22 4 24 21
S 1 V ntrans P 27 P 28
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 29 P 30
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 31 P 32
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 33 P 34
P 26 7 * ndif
P 26 18 * ndif
W 23 4 29 21
S 3 V ndif P 35 P 36
P 8 10 * ndif
P 8 18 * ndif
W 5 7 11 21
S 3 V ndif P 37 P 38
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 39 P 40
P 26 3 * allowM2
P 26 45 * allowM2
W 24 1 28 47
S 2 V allowM2 P 41 P 42
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 43 P 44
P 17 21 * poly
P 23 21 * poly
W 16 20 24 22
S 1 H poly P 45 P 46
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 47 P 48
P 26 30 * metal
P 26 43 * metal
W 24 28 28 45 vdd
S 2 V metal P 49 P 50
P 26 43 * metal
P 26 45 * metal
W 25 42 27 46 vdd
S 1 V metal P 51 P 52
P 21 14 * metal
P 21 27 * metal
W 20 13 22 28
S 1 V metal P 53 P 54
P 8 13 * metal
P 8 19 * metal
W 6 11 10 21
S 2 V metal P 55 P 56
P 8 29 * metal
P 8 35 * metal
W 6 27 10 37
S 2 V metal P 57 P 58
P 8 19 * metal
P 8 29 * metal
W 7 18 9 30
S 1 V metal P 59 P 60
P 12 25 * metal
P 13 25 * metal
W 11 24 14 26
S 1 H metal P 61 P 62
P 13 25 * metal
P 13 28 * metal
W 12 24 14 29
S 1 V metal P 63 P 64
P 13 28 * metal
P 14 28 * metal
W 12 27 15 29
S 1 H metal P 65 P 66
P 12 24 * metal
P 12 25 * metal
W 11 23 13 26
S 1 V metal P 67 P 68
P 11 25 * ptrans
P 11 43 * ptrans
W 10 24 12 44
S 1 V ptrans P 69 P 70
P 11 8 * ntrans
P 11 20 * ntrans
W 10 7 12 21
S 1 V ntrans P 71 P 72
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 73 P 74
P 11 24 * poly
P 12 24 * poly
W 10 23 13 25
S 1 H poly P 75 P 76
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 77 P 78
P 8 43 * metal
P 8 46 * metal
W 7 42 9 47 vdd
S 1 V metal P 79 P 80
P 14 33 * metal
P 14 45 * metal
W 12 31 16 47 vdd
S 2 V metal P 81 P 82
P 14 5 * metal
P 14 14 * metal
W 12 3 16 16 vss
S 2 V metal P 83 P 84
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 85 P 86
P 8 3 * metal
P 8 5 * metal
W 7 2 9 6 vss
S 1 V metal P 87 P 88
P 26 5 * metal
P 26 17 * metal
W 24 3 28 19 vss
S 2 V metal P 89 P 90
P 20 14 * metal
P 21 14 * metal
W 19 13 22 15
S 1 H metal P 91 P 92
P 20 13 * metal
P 20 14 * metal
W 19 12 21 15
S 1 V metal P 93 P 94
P 20 27 * metal
P 21 27 * metal
W 18 25 23 29
S 2 H metal P 95 P 96
P 20 27 * metal
P 20 34 * metal
W 18 25 22 36
S 2 V metal P 97 P 98
E
