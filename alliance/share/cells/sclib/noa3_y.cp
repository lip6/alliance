#cell1 noa3_y idps_symb compose *
# 07-Oct-98 12:21 07-Oct-98 12:21 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 29 45
B 5 3 29 45
F F
C f 26 45 metal2 2 1 * * N f
C f 26 3 metal2 2 2 * * S f
C i1 14 45 metal2 2 3 * * N i1
C i0 8 45 metal2 2 4 * * N i0
C i0 8 3 metal2 2 5 * * S i0
C i1 14 3 metal2 2 6 * * S i1
C vdd 5 43 metal 8 7 * * W vdd
C vss 5 5 metal 8 8 * * W vss
C i2 20 45 metal2 2 9 * * N i2
C i2 20 3 metal2 2 10 * * S i2
C vss 29 5 metal 8 11 * * E vss
C vdd 29 43 metal 8 12 * * E vdd
I cdp1 8 33 0 "cdp" ly * *
I cdp2 8 41 0 "cdp" ly * *
I cdp3 8 45 0 "cdp" ly * *
I cdn1 14 9 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdn3 8 13 0 "cdn" ly * *
I cdn4 26 13 0 "cdn" ly * *
I cdn5 14 13 0 "cdn" ly * *
I cdn6 8 17 0 "cdn" ly * *
I cdn7 20 17 0 "cdn" ly * *
I cdn8 26 17 0 "cdn" ly * *
I via12x1 26 23 0 "via12" ly * *
I cdp4 26 37 0 "cdp" ly * *
I cdp5 8 37 0 "cdp" ly * *
I cbn1 26 45 0 "cbn" ly * *
I cbp1 8 3 0 "cbp" ly * *
I cbp2 14 3 0 "cbp" ly * *
I via12x2 20 28 0 "via12" ly * *
I via12x3 14 28 0 "via12" ly * *
I via12x4 8 28 0 "via12" ly * *
I cpf1 21 23 0 "cpf" ly * *
I cpf2 15 23 0 "cpf" ly * *
I cpf3 9 23 0 "cpf" ly * *
I cdp6 20 34 0 "cdp" ly * *
P 26 37 * metal
P 26 45 * metal
W 24 35 28 47 vdd
S 2 V metal P 1 P 2
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 3 P 4
P 8 3 * metal
P 8 5 * metal
W 6 1 10 7 vss
S 2 V metal P 5 P 6
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 7 P 8
P 14 3 * metal
P 14 13 * metal
W 12 1 16 15 vss
S 2 V metal P 9 P 10
P 26 27 * pdif
P 26 37 * pdif
W 23 24 29 40
S 3 V pdif P 11 P 12
P 20 23 * metal
P 20 28 * metal
W 19 22 21 29
S 1 V metal P 13 P 14
P 20 23 * metal
P 21 23 * metal
W 18 21 23 25
S 2 H metal P 15 P 16
P 14 23 * metal
P 14 28 * metal
W 13 22 15 29
S 1 V metal P 17 P 18
P 14 23 * metal
P 15 23 * metal
W 12 21 17 25
S 2 H metal P 19 P 20
P 8 23 * metal
P 8 28 * metal
W 7 22 9 29
S 1 V metal P 21 P 22
P 8 23 * metal
P 9 23 * metal
W 6 21 11 25
S 2 H metal P 23 P 24
P 20 33 * metal
P 20 34 * metal
W 18 31 22 36
S 2 V metal P 25 P 26
P 26 17 * metal
P 26 33 * metal
W 25 16 27 34
S 1 V metal P 27 P 28
P 20 33 * metal
P 26 33 * metal
W 19 32 27 34
S 1 H metal P 29 P 30
P 26 7 * ndif
P 26 18 * ndif
W 23 4 29 21
S 3 V ndif P 31 P 32
P 8 8 * ndif
P 8 18 * ndif
W 5 5 11 21
S 3 V ndif P 33 P 34
P 9 23 * poly
P 11 23 * poly
W 7 21 13 25
S 2 H poly P 35 P 36
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 37 P 38
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
P 8 27 * pdif
P 8 49 * pdif
W 5 24 11 52
S 3 V pdif P 47 P 48
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 49 P 50
P 8 3 * ptie
P 14 3 * ptie
W 6 1 16 5
S 2 H ptie P 51 P 52
P 20 13 * metal
P 20 18 * metal
W 18 11 22 20
S 2 V metal P 53 P 54
P 8 18 * metal
P 20 18 * metal
W 7 17 21 19
S 1 H metal P 55 P 56
P 23 5 * ntrans
P 23 20 * ntrans
W 22 4 24 21
S 1 V ntrans P 57 P 58
P 14 8 * ndif
P 14 18 * ndif
W 11 5 17 21
S 3 V ndif P 59 P 60
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 61 P 62
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 i0
S 2 V metal2 P 63 P 64
P 17 6 * ntrans
P 17 20 * ntrans
W 16 5 18 21
S 1 V ntrans P 65 P 66
P 11 6 * ntrans
P 11 20 * ntrans
W 10 5 12 21
S 1 V ntrans P 67 P 68
P 20 27 * pdif
P 20 38 * pdif
W 17 24 23 41
S 3 V pdif P 69 P 70
P 23 25 * ptrans
P 23 40 * ptrans
W 22 24 24 41
S 1 V ptrans P 71 P 72
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 73 P 74
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 75 P 76
P 11 25 * ptrans
P 11 51 * ptrans
W 10 24 12 52
S 1 V ptrans P 77 P 78
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 79 P 80
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 f
S 2 V metal2 P 81 P 82
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 83 P 84
P 26 13 * metal
P 26 17 * metal
W 24 11 28 19
S 2 V metal P 85 P 86
P 8 33 * metal
P 8 45 * metal
W 6 31 10 47 vdd
S 2 V metal P 87 P 88
P 8 13 * metal
P 8 18 * metal
W 6 11 10 20
S 2 V metal P 89 P 90
E
