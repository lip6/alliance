#cell1 na3_y idps_symb compose *
# 07-Oct-98 12:14 07-Oct-98 12:14 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 29 45
B 5 3 29 45
F F
C f 26 3 metal2 2 1 * * S f
C i0 20 3 metal2 2 2 * * S i0
C i1 14 3 metal2 2 3 * * S i1
C i2 8 3 metal2 2 4 * * S i2
C vss 29 5 metal 8 5 * * E vss
C vss 5 5 metal 8 6 * * W vss
C f 26 45 metal2 2 7 * * N f
C i0 20 45 metal2 2 8 * * N i0
C i1 14 45 metal2 2 9 * * N i1
C i2 8 45 metal2 2 10 * * N i2
C vdd 29 43 metal 8 11 * * E vdd
C vdd 5 43 metal 8 12 * * W vdd
I cbn1 14 45 0 "cbn" ly * *
I cbn2 20 45 0 "cbn" ly * *
I cbp1 14 2 0 "cbp" ly * *
I cbp2 20 2 0 "cbp" ly * *
I cdn1 8 14 0 "cdn" ly * *
I cdn2 8 8 0 "cdn" ly * *
I cdn3 26 14 0 "cdn" ly * *
I cdp1 8 39 0 "cdp" ly * *
I cdp2 8 33 0 "cdp" ly * *
I cdp3 14 28 0 "cdp" ly * *
I cdp4 14 34 0 "cdp" ly * *
I cdp5 20 39 0 "cdp" ly * *
I cdp6 20 33 0 "cdp" ly * *
I cdp7 26 34 0 "cdp" ly * *
I cpf1 8 22 0 "cpf" ly * *
I cpf2 15 22 0 "cpf" ly * *
I cpf3 21 22 0 "cpf" ly * *
I via12x1 14 17 0 "via12" ly * *
I via12x2 20 17 0 "via12" ly * *
I via12x3 26 22 0 "via12" ly * *
I via12x4 8 28 0 "via12" ly * *
I cbn3 26 45 0 "cbn" ly * *
I cbn4 8 45 0 "cbn" ly * *
I cbp3 26 2 0 "cbp" ly * *
I cbp4 8 2 0 "cbp" ly * *
I cdp8 26 28 0 "cdp" ly * *
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 1 P 2
P 26 43 * metal
P 26 45 * metal
W 24 41 28 47 vdd
S 2 V metal P 3 P 4
P 20 33 * metal
P 20 45 * metal
W 18 31 22 47 vdd
S 2 V metal P 5 P 6
P 14 43 * metal
P 14 45 * metal
W 12 41 16 47 vdd
S 2 V metal P 7 P 8
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 9 P 10
P 8 33 * metal
P 8 45 * metal
W 6 31 10 47 vdd
S 2 V metal P 11 P 12
P 20 22 * metal
P 21 22 * metal
W 18 20 23 24
S 2 H metal P 13 P 14
P 20 17 * metal
P 20 22 * metal
W 18 15 22 24
S 2 V metal P 15 P 16
P 14 22 * metal
P 15 22 * metal
W 12 20 17 24
S 2 H metal P 17 P 18
P 14 17 * metal
P 14 22 * metal
W 12 15 16 24
S 2 V metal P 19 P 20
P 8 45 * ntie
P 26 45 * ntie
W 6 43 28 47
S 2 H ntie P 21 P 22
P 8 2 * ptie
P 26 2 * ptie
W 6 0 28 4
S 2 H ptie P 23 P 24
P 26 14 * metal
P 26 34 * metal
W 24 12 28 36
S 2 V metal P 25 P 26
P 8 22 * metal
P 8 28 * metal
W 7 21 9 29
S 1 V metal P 27 P 28
P 14 28 * metal
P 14 34 * metal
W 12 26 16 36
S 2 V metal P 29 P 30
P 14 28 * metal
P 26 28 * metal
W 12 26 28 30
S 2 H metal P 31 P 32
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 f
S 2 V metal2 P 33 P 34
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 35 P 36
P 8 7 * ndif
P 8 18 * ndif
W 5 4 11 21
S 3 V ndif P 37 P 38
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 39 P 40
P 21 22 * poly
P 23 22 * poly
W 19 20 25 24
S 2 H poly P 41 P 42
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 43 P 44
P 15 22 * poly
P 17 22 * poly
W 13 20 19 24
S 2 H poly P 45 P 46
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 47 P 48
P 8 22 * poly
P 11 22 * poly
W 5 19 14 25
S 3 H poly P 49 P 50
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 51 P 52
P 8 27 * pdif
P 8 39 * pdif
W 5 24 11 42
S 3 V pdif P 53 P 54
P 20 27 * pdif
P 20 39 * pdif
W 17 24 23 42
S 3 V pdif P 55 P 56
P 14 27 * pdif
P 14 38 * pdif
W 11 24 17 41
S 3 V pdif P 57 P 58
P 26 27 * pdif
P 26 38 * pdif
W 23 24 29 41
S 3 V pdif P 59 P 60
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 61 P 62
P 26 7 * ndif
P 26 18 * ndif
W 23 4 29 21
S 3 V ndif P 63 P 64
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 65 P 66
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 67 P 68
P 23 5 * ntrans
P 23 20 * ntrans
W 22 4 24 21
S 1 V ntrans P 69 P 70
P 17 5 * ntrans
P 17 20 * ntrans
W 16 4 18 21
S 1 V ntrans P 71 P 72
P 11 5 * ntrans
P 11 20 * ntrans
W 10 4 12 21
S 1 V ntrans P 73 P 74
P 23 25 * ptrans
P 23 40 * ptrans
W 22 24 24 41
S 1 V ptrans P 75 P 76
P 17 25 * ptrans
P 17 40 * ptrans
W 16 24 18 41
S 1 V ptrans P 77 P 78
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 79 P 80
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 81 P 82
P 8 2 * metal
P 8 14 * metal
W 6 0 10 16 vss
S 2 V metal P 83 P 84
P 8 2 * metal
P 26 2 * metal
W 6 0 28 4 vss
S 2 H metal P 85 P 86
E
