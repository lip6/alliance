#cell1 ndrv_y idps_symb compose *
# 07-Oct-98 12:18 07-Oct-98 12:18 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 29 45
B 5 3 29 45
F F
C vdd 5 43 metal 8 1 * * W vdd
C vss 5 5 metal 8 2 * * W vss
C vdd 29 43 metal 8 3 * * E vdd
C vss 29 5 metal 8 4 * * E vss
C i 26 45 metal2 2 5 * * N i
C i 26 3 metal2 2 6 * * S i
C f 14 45 metal2 2 7 * * N f
C f 14 3 metal2 2 8 * * S f
I cdp1 14 45 0 "cdp" ly * *
I cdn1 14 8 0 "cdn" ly * *
I cdn2 20 17 0 "cdn" ly * *
I cbp1 8 3 0 "cbp" ly * *
I cbn1 8 29 0 "cbn" ly * *
I cbn2 8 37 0 "cbn" ly * *
I cbn3 8 33 0 "cbn" ly * *
I cbn4 8 41 0 "cbn" ly * *
I cbn5 8 45 0 "cbn" ly * *
I cbp2 8 17 0 "cbp" ly * *
I cbp3 8 13 0 "cbp" ly * *
I cbp4 8 9 0 "cbp" ly * *
I cdn3 14 12 0 "cdn" ly * *
I cdn4 20 13 0 "cdn" ly * *
I cdn5 26 15 0 "cdn" ly * *
I cdn6 26 9 0 "cdn" ly * *
I cdp2 14 41 0 "cdp" ly * *
I cdp3 14 36 0 "cdp" ly * *
I cdp4 20 34 0 "cdp" ly * *
I cdp5 26 42 0 "cdp" ly * *
I cdp6 26 36 0 "cdp" ly * *
I cdp7 26 30 0 "cdp" ly * *
I via12x1 14 23 0 "via12" ly * *
I via12x2 26 23 0 "via12" ly * *
I cdp8 20 28 0 "cdp" ly * *
I cpf1 22 23 0 "cpf" ly * *
P 20 28 * metal
P 20 34 * metal
W 18 26 22 36
S 2 V metal P 1 P 2
P 14 28 * metal
P 20 28 * metal
W 12 26 22 30
S 2 H metal P 3 P 4
P 14 17 * metal
P 14 28 * metal
W 12 15 16 30
S 2 V metal P 5 P 6
P 14 17 * metal
P 20 17 * metal
W 12 15 22 19
S 2 H metal P 7 P 8
P 20 13 * metal
P 20 17 * metal
W 18 11 22 19
S 2 V metal P 9 P 10
P 22 23 * metal
P 26 23 * metal
W 20 21 28 25
S 2 H metal P 11 P 12
P 26 5 * metal
P 26 15 * metal
W 24 3 28 17 vss
S 2 V metal P 13 P 14
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 15 P 16
P 14 5 * metal
P 14 12 * metal
W 12 3 16 14 vss
S 2 V metal P 17 P 18
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 19 P 20
P 26 30 * metal
P 26 43 * metal
W 24 28 28 45 vdd
S 2 V metal P 21 P 22
P 14 36 * metal
P 14 43 * metal
W 12 34 16 45 vdd
S 2 V metal P 23 P 24
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 25 P 26
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 27 P 28
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 f
S 2 V metal2 P 29 P 30
P 8 29 * ntie
P 8 43 * ntie
W 5 26 11 46
S 3 V ntie P 31 P 32
P 8 3 * ptie
P 8 17 * ptie
W 5 0 11 20
S 3 V ptie P 33 P 34
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 35 P 36
P 26 7 * ndif
P 26 18 * ndif
W 23 4 29 21
S 3 V ndif P 37 P 38
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 39 P 40
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 41 P 42
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 43 P 44
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 45 P 46
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 47 P 48
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 49 P 50
P 23 20 * poly
P 23 25 * poly
W 22 19 24 26
S 1 V poly P 51 P 52
P 17 23 * poly
P 23 23 * poly
W 16 22 24 24
S 1 H poly P 53 P 54
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 55 P 56
P 23 5 * ntrans
P 23 20 * ntrans
W 22 4 24 21
S 1 V ntrans P 57 P 58
P 17 5 * ntrans
P 17 20 * ntrans
W 16 4 18 21
S 1 V ntrans P 59 P 60
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 61 P 62
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 63 P 64
P 8 45 * metal
P 14 45 * metal
W 6 43 16 47 vdd
S 2 H metal P 65 P 66
P 8 29 * metal
P 8 45 * metal
W 6 27 10 47 vdd
S 2 V metal P 67 P 68
P 8 3 * metal
P 8 17 * metal
W 6 1 10 19 vss
S 2 V metal P 69 P 70
E
