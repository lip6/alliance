#cell1 annup_y idps_symb compose *
# 07-Oct-98 12:06 07-Oct-98 12:06 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 35 45
B 5 3 35 45
F F
C i1 8 45 metal2 2 1 * * N i1
C i4 20 45 metal2 2 2 * * N i4
C i3 32 45 metal2 2 3 * * N i3
C i1 8 3 metal2 2 4 * * S i1
C i2 14 3 metal2 2 5 * * S i2
C i3 32 3 metal2 2 6 * * S i3
C f 26 3 metal2 2 7 * * S f
C vdd 5 43 metal 8 8 * * W vdd
C vdd 35 43 metal 8 9 * * E vdd
C vss 5 5 metal 8 10 * * W vss
C vss 35 5 metal 8 11 * * E vss
C i2 14 45 metal2 2 12 * * N i2
C i4 20 3 metal2 2 13 * * S i4
C f 26 45 metal2 2 14 * * N f
I cbp1 32 3 0 "cbp" ly * *
I cbp2 26 3 0 "cbp" ly * *
I cbp3 20 3 0 "cbp" ly * *
I cbp4 14 3 0 "cbp" ly * *
I cbp5 8 3 0 "cbp" ly * *
I cbn1 32 45 0 "cbn" ly * *
I cbn2 26 45 0 "cbn" ly * *
I cbn3 20 45 0 "cbn" ly * *
I cbn4 14 45 0 "cbn" ly * *
I cbn5 8 45 0 "cbn" ly * *
I cdn1 20 17 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdn3 32 13 0 "cdn" ly * *
I cdn4 32 17 0 "cdn" ly * *
I cdn5 14 14 0 "cdn" ly * *
I cdn6 14 10 0 "cdn" ly * *
I cdp1 8 32 0 "cdp" ly * *
I cdp2 8 36 0 "cdp" ly * *
I cdp3 32 36 0 "cdp" ly * *
I cdp4 32 32 0 "cdp" ly * *
I cpf1 16 22 0 "cpf" ly * *
I cpf2 22 22 0 "cpf" ly * *
I cpf3 32 22 0 "cpf" ly * *
I cdn7 26 17 0 "cdn" ly * *
I cdp5 20 32 0 "cdp" ly * *
I via12x1 26 32 0 "via12" ly * *
I via12x2 8 27 0 "via12" ly * *
I via12x3 14 27 0 "via12" ly * *
I via12x4 20 27 0 "via12" ly * *
I via12x5 32 27 0 "via12" ly * *
I cpf4 8 22 0 "cpf" ly * *
I cdn8 8 13 0 "cdn" ly * *
I cdn9 8 17 0 "cdn" ly * *
P 5 5 * metal
P 35 5 * metal
W -3 -3 43 13
S 8 H metal P 1 P 2
P 5 43 * metal
P 35 43 * metal
W -3 35 43 51
S 8 H metal P 3 P 4
P 11 25 * ptrans
P 11 39 * ptrans
W 10 24 12 40
S 1 V ptrans P 5 P 6
P 17 25 * ptrans
P 17 39 * ptrans
W 16 24 18 40
S 1 V ptrans P 7 P 8
P 23 25 * ptrans
P 23 39 * ptrans
W 22 24 24 40
S 1 V ptrans P 9 P 10
P 29 25 * ptrans
P 29 39 * ptrans
W 28 24 30 40
S 1 V ptrans P 11 P 12
P 11 9 * ntrans
P 11 20 * ntrans
W 10 8 12 21
S 1 V ntrans P 13 P 14
P 17 9 * ntrans
P 17 20 * ntrans
W 16 8 18 21
S 1 V ntrans P 15 P 16
P 23 9 * ntrans
P 23 20 * ntrans
W 22 8 24 21
S 1 V ntrans P 17 P 18
P 29 9 * ntrans
P 29 20 * ntrans
W 28 8 30 21
S 1 V ntrans P 19 P 20
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 21 P 22
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 23 P 24
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 25 P 26
P 29 20 * poly
P 29 25 * poly
W 28 19 30 26
S 1 V poly P 27 P 28
P 20 27 * pdif
P 20 37 * pdif
W 17 24 23 40
S 3 V pdif P 29 P 30
P 14 27 * pdif
P 14 37 * pdif
W 11 24 17 40
S 3 V pdif P 31 P 32
P 26 27 * pdif
P 26 37 * pdif
W 23 24 29 40
S 3 V pdif P 33 P 34
P 14 11 * ndif
P 14 18 * ndif
W 11 8 17 21
S 3 V ndif P 35 P 36
P 20 11 * ndif
P 20 18 * ndif
W 17 8 23 21
S 3 V ndif P 37 P 38
P 26 11 * ndif
P 26 18 * ndif
W 23 8 29 21
S 3 V ndif P 39 P 40
P 32 11 * ndif
P 32 18 * ndif
W 29 8 35 21
S 3 V ndif P 41 P 42
P 14 3 * metal
P 14 14 * metal
W 12 1 16 16
S 2 V metal P 43 P 44
P 8 27 * pdif
P 8 37 * pdif
W 5 24 11 40
S 3 V pdif P 45 P 46
P 32 27 * pdif
P 32 37 * pdif
W 29 24 35 40
S 3 V pdif P 47 P 48
P 8 32 * metal
P 8 43 * metal
W 6 30 10 45
S 2 V metal P 49 P 50
P 32 32 * metal
P 32 43 * metal
W 30 30 34 45
S 2 V metal P 51 P 52
P 16 22 * poly
P 17 22 * poly
W 13 19 20 25
S 3 H poly P 53 P 54
P 22 22 * poly
P 23 22 * poly
W 19 19 26 25
S 3 H poly P 55 P 56
P 29 22 * poly
P 32 22 * poly
W 26 19 35 25
S 3 H poly P 57 P 58
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 59 P 60
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 61 P 62
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 63 P 64
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47
S 2 V metal2 P 65 P 66
P 20 12 * metal
P 20 18 * metal
W 18 10 22 20
S 2 V metal P 67 P 68
P 20 12 * metal
P 32 12 * metal
W 19 11 33 13
S 1 H metal P 69 P 70
P 32 12 * metal
P 32 17 * metal
W 30 10 34 19
S 2 V metal P 71 P 72
P 20 32 * metal
P 26 32 * metal
W 18 30 28 34
S 2 H metal P 73 P 74
P 26 17 * metal
P 26 32 * metal
W 25 16 27 33
S 1 V metal P 75 P 76
P 32 22 * metal
P 32 27 * metal
W 30 20 34 29
S 2 V metal P 77 P 78
P 20 27 * metal
P 22 27 * metal
W 18 25 24 29
S 2 H metal P 79 P 80
P 22 22 * metal
P 22 27 * metal
W 20 20 24 29
S 2 V metal P 81 P 82
P 8 22 * poly
P 11 22 * poly
W 5 19 14 25
S 3 H poly P 83 P 84
P 8 22 * metal
P 8 27 * metal
W 6 20 10 29
S 2 V metal P 85 P 86
P 14 22 * metal
P 16 22 * metal
W 12 20 18 24
S 2 H metal P 87 P 88
P 14 22 * metal
P 14 27 * metal
W 12 20 16 29
S 2 V metal P 89 P 90
P 8 11 * ndif
P 8 18 * ndif
W 5 8 11 21
S 3 V ndif P 91 P 92
P 8 13 * metal
P 8 18 * metal
W 6 11 10 20
S 2 V metal P 93 P 94
P 8 18 * metal
P 20 18 * metal
W 7 17 21 19
S 1 H metal P 95 P 96
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 97 P 98
P 5 39 * nwell
P 35 39 * nwell
W -21 13 61 65
S 26 H nwell P 99 P 100
P 8 45 * ntie
P 32 45 * ntie
W 6 43 34 47
S 2 H ntie P 101 P 102
P 8 3 * ptie
P 32 3 * ptie
W 6 1 34 5
S 2 H ptie P 103 P 104
E
