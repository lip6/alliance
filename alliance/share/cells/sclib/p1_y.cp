#cell1 p1_y idps_symb compose *
# 07-Oct-98 12:29 07-Oct-98 12:29 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 23 45
B 5 3 23 45
F F
C vdd 23 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 23 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
C i 14 45 metal2 2 5 * * N i
C i 14 3 metal2 2 6 * * S i
C t 20 45 metal2 2 7 * * N t
C t 20 3 metal2 2 8 * * S t
I cdn1 14 16 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdn3 14 12 0 "cdn" ly * *
I cdp1 14 41 0 "cdp" ly * *
I cdp2 20 34 0 "cdp" ly * *
I cdp3 20 28 0 "cdp" ly * *
I cpf1 17 22 0 "cpf" ly * *
I via12x1 20 31 0 "via12" ly * *
I cdp4 14 45 0 "cdp" ly * *
I cdn4 14 8 0 "cdn" ly * *
I cdn5 8 15 0 "cdn" ly * *
I cdp5 8 29 0 "cdp" ly * *
I cdp6 14 33 0 "cdp" ly * *
I cdp7 14 37 0 "cdp" ly * *
I cdp8 8 35 0 "cdp" ly * *
I cbp1 8 3 0 "cbp" ly * *
I cbn1 8 45 0 "cbn" ly * *
I via12x2 14 28 0 "via12" ly * *
I cpf2 11 24 0 "cpf" ly * *
I cdn6 20 17 0 "cdn" ly * *
P 20 13 * metal
P 20 17 * metal
W 18 11 22 19
S 2 V metal P 1 P 2
P 21 17 * metal
P 21 28 * metal
W 20 16 22 29
S 1 V metal P 3 P 4
P 20 17 * metal
P 21 17 * metal
W 19 16 22 18
S 1 H metal P 5 P 6
P 16 22 * metal
P 17 22 * metal
W 15 21 18 23
S 1 H metal P 7 P 8
P 7 20 * metal
P 16 20 * metal
W 6 19 17 21
S 1 H metal P 9 P 10
P 16 20 * metal
P 16 22 * metal
W 15 19 17 23
S 1 V metal P 11 P 12
P 7 15 * metal
P 8 15 * metal
W 6 14 9 16
S 1 H metal P 13 P 14
P 7 15 * metal
P 7 30 * metal
W 6 14 8 31
S 1 V metal P 15 P 16
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 17 P 18
P 8 29 * metal
P 8 35 * metal
W 6 27 10 37
S 2 V metal P 19 P 20
P 7 30 * metal
P 8 30 * metal
W 5 28 10 32
S 2 H metal P 21 P 22
P 8 43 * metal
P 8 45 * metal
W 6 41 10 47 vdd
S 2 V metal P 23 P 24
P 5 43 * metal
P 23 43 * metal
W -3 35 31 51 vdd
S 8 H metal P 25 P 26
P 14 33 * metal
P 14 45 * metal
W 12 31 16 47 vdd
S 2 V metal P 27 P 28
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 29 P 30
P 20 28 * metal
P 21 28 * metal
W 19 27 22 29
S 1 H metal P 31 P 32
P 20 28 * metal
P 20 34 * metal
W 18 26 22 36
S 2 V metal P 33 P 34
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 35 P 36
P 8 13 * ndif
P 8 18 * ndif
W 5 10 11 21
S 3 V ndif P 37 P 38
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 39 P 40
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 41 P 42
P 11 11 * ntrans
P 11 20 * ntrans
W 10 10 12 21
S 1 V ntrans P 43 P 44
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 45 P 46
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 47 P 48
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 49 P 50
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 51 P 52
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 53 P 54
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 55 P 56
P 17 5 * ntrans
P 17 20 * ntrans
W 16 4 18 21
S 1 V ntrans P 57 P 58
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 59 P 60
P 14 5 * metal
P 14 16 * metal
W 12 3 16 18 vss
S 2 V metal P 61 P 62
P 5 5 * metal
P 23 5 * metal
W -3 -3 31 13 vss
S 8 H metal P 63 P 64
P 8 3 * metal
P 8 5 * metal
W 7 2 9 6 vss
S 1 V metal P 65 P 66
P 5 39 * nwell
P 23 39 * nwell
W -21 13 49 65
S 26 H nwell P 67 P 68
P 12 24 * metal
P 12 28 * metal
W 11 23 13 29
S 1 V metal P 69 P 70
P 12 28 * metal
P 14 28 * metal
W 10 26 16 30
S 2 H metal P 71 P 72
P 11 24 * metal
P 12 24 * metal
W 9 22 14 26
S 2 H metal P 73 P 74
E
