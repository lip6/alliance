#cell1 nao3_y idps_symb compose *
# 07-Oct-98 12:16 07-Oct-98 12:16 mbkvti400 * .cp
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
I cdp1 26 35 0 "cdp" ly * *
I cbn1 26 46 0 "cbn" ly * *
I cbn2 20 46 0 "cbn" ly * *
I cbn3 14 46 0 "cbn" ly * *
I cbn4 8 46 0 "cbn" ly * *
I cdn1 20 13 0 "cdn" ly * *
I cdn2 26 14 0 "cdn" ly * *
I cdn3 8 13 0 "cdn" ly * *
I cdp2 26 28 0 "cdp" ly * *
I cdp3 20 35 0 "cdp" ly * *
I cdp4 8 35 0 "cdp" ly * *
I cdp5 14 37 0 "cdp" ly * *
I via12x1 26 23 0 "via12" ly * *
I via12x2 8 28 0 "via12" ly * *
I via12x3 14 28 0 "via12" ly * *
I via12x4 20 28 0 "via12" ly * *
I cpf1 8 23 0 "cpf" ly * *
I cpf2 15 23 0 "cpf" ly * *
I cpf3 21 23 0 "cpf" ly * *
I cbp1 26 3 0 "cbp" ly * *
I cdn4 20 9 0 "cdn" ly * *
I cbp2 20 3 0 "cbp" ly * *
I cbp3 14 3 0 "cbp" ly * *
I cbp4 8 3 0 "cbp" ly * *
P 8 3 * ptie
P 26 3 * ptie
W 6 1 28 5
S 2 H ptie P 1 P 2
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 3 P 4
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 5 P 6
P 14 9 * ndif
P 14 16 * ndif
W 11 6 17 19
S 3 V ndif P 7 P 8
P 8 32 * metal
P 8 35 * metal
W 6 30 10 37
S 2 V metal P 9 P 10
P 8 32 * metal
P 20 32 * metal
W 7 31 21 33
S 1 H metal P 11 P 12
P 20 32 * metal
P 20 35 * metal
W 18 30 22 37
S 2 V metal P 13 P 14
P 8 27 * pdif
P 8 41 * pdif
W 5 24 11 44
S 3 V pdif P 15 P 16
P 20 27 * pdif
P 20 41 * pdif
W 17 24 23 44
S 3 V pdif P 17 P 18
P 20 23 * metal
P 20 28 * metal
W 19 22 21 29
S 1 V metal P 19 P 20
P 20 23 * metal
P 21 23 * metal
W 18 21 23 25
S 2 H metal P 21 P 22
P 14 23 * metal
P 14 28 * metal
W 13 22 15 29
S 1 V metal P 23 P 24
P 14 23 * metal
P 15 23 * metal
W 12 21 17 25
S 2 H metal P 25 P 26
P 8 23 * metal
P 8 28 * metal
W 7 22 9 29
S 1 V metal P 27 P 28
P 21 23 * poly
P 23 23 * poly
W 19 21 25 25
S 2 H poly P 29 P 30
P 23 18 * poly
P 23 25 * poly
W 22 17 24 26
S 1 V poly P 31 P 32
P 15 23 * poly
P 17 23 * poly
W 13 21 19 25
S 2 H poly P 33 P 34
P 17 18 * poly
P 17 25 * poly
W 16 17 18 26
S 1 V poly P 35 P 36
P 8 23 * poly
P 11 23 * poly
W 5 20 14 26
S 3 H poly P 37 P 38
P 11 18 * poly
P 11 25 * poly
W 10 17 12 26
S 1 V poly P 39 P 40
P 8 13 * metal
P 8 18 * metal
W 7 12 9 19
S 1 V metal P 41 P 42
P 8 18 * metal
P 26 18 * metal
W 7 17 27 19
S 1 H metal P 43 P 44
P 26 14 * metal
P 26 35 * metal
W 25 13 27 36
S 1 V metal P 45 P 46
P 26 27 * pdif
P 26 41 * pdif
W 23 24 29 44
S 3 V pdif P 47 P 48
P 26 13 * ndif
P 26 16 * ndif
W 23 10 29 19
S 3 V ndif P 49 P 50
P 14 27 * pdif
P 14 41 * pdif
W 11 24 17 44
S 3 V pdif P 51 P 52
P 11 25 * ptrans
P 11 43 * ptrans
W 10 24 12 44
S 1 V ptrans P 53 P 54
P 17 25 * ptrans
P 17 43 * ptrans
W 16 24 18 44
S 1 V ptrans P 55 P 56
P 23 11 * ntrans
P 23 18 * ntrans
W 22 10 24 19
S 1 V ntrans P 57 P 58
P 8 9 * ndif
P 8 16 * ndif
W 5 6 11 19
S 3 V ndif P 59 P 60
P 20 9 * ndif
P 20 16 * ndif
W 17 6 23 19
S 3 V ndif P 61 P 62
P 17 7 * ntrans
P 17 18 * ntrans
W 16 6 18 19
S 1 V ntrans P 63 P 64
P 11 7 * ntrans
P 11 18 * ntrans
W 10 6 12 19
S 1 V ntrans P 65 P 66
P 8 46 * ntie
P 26 46 * ntie
W 6 44 28 48
S 2 H ntie P 67 P 68
P 23 25 * ptrans
P 23 43 * ptrans
W 22 24 24 44
S 1 V ptrans P 69 P 70
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 i0
S 2 V metal2 P 71 P 72
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47 i2
S 2 V metal2 P 73 P 74
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 f
S 2 V metal2 P 75 P 76
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 77 P 78
P 8 3 * metal
P 26 3 * metal
W 7 2 27 4
S 1 H metal P 79 P 80
P 20 3 * metal
P 20 13 * metal
W 18 1 22 15 vss
S 2 V metal P 81 P 82
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 83 P 84
P 8 46 * metal
P 14 46 * metal
W 6 44 16 48 vdd
S 2 H metal P 85 P 86
P 14 37 * metal
P 14 46 * metal
W 12 35 16 48 vdd
S 2 V metal P 87 P 88
P 14 46 * metal
P 26 46 * metal
W 12 44 28 48 vdd
S 2 H metal P 89 P 90
E
