#cell1 na2_y idps_symb compose *
# 07-Oct-98 12:13 07-Oct-98 12:13 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 23 45
B 5 3 23 45
F F
C i0 20 45 metal2 2 1 * * N i0
C f 14 45 metal2 2 2 * * N f
C i1 8 45 metal2 2 3 * * N i1
C i0 20 3 metal2 2 4 * * S i0
C f 14 3 metal2 2 5 * * S f
C i1 8 3 metal2 2 6 * * S i1
C vdd 23 43 metal 8 7 * * E vdd
C vdd 5 43 metal 8 8 * * W vdd
C vss 5 5 metal 8 9 * * W vss
C vss 23 5 metal 8 10 * * E vss
I cdp1 20 37 0 "cdp" ly * *
I cdp2 8 37 0 "cdp" ly * *
I cbn1 14 45 0 "cbn" ly * *
I cbp1 14 2 0 "cbp" ly * *
I cdn1 8 13 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdp3 8 33 0 "cdp" ly * *
I cdp4 20 33 0 "cdp" ly * *
I cpf1 9 22 0 "cpf" ly * *
I cpf2 19 22 0 "cpf" ly * *
I via12x1 8 28 0 "via12" ly * *
I via12x2 20 28 0 "via12" ly * *
I via12x3 14 22 0 "via12" ly * *
I cdn3 8 8 0 "cdn" ly * *
I cbp2 20 2 0 "cbp" ly * *
I cbp3 8 2 0 "cbp" ly * *
I cbn2 20 45 0 "cbn" ly * *
I cbn3 8 45 0 "cbn" ly * *
I cdp5 14 29 0 "cdp" ly * *
I cdp6 14 35 0 "cdp" ly * *
P 5 5 * metal
P 23 5 * metal
W -3 -3 31 13 vss
S 8 H metal P 1 P 2
P 8 2 * ptie
P 20 2 * ptie
W 6 0 22 4
S 2 H ptie P 3 P 4
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 5 P 6
P 20 27 * pdif
P 20 38 * pdif
W 17 24 23 41
S 3 V pdif P 7 P 8
P 20 22 * metal
P 20 28 * metal
W 18 20 22 30
S 2 V metal P 9 P 10
P 19 22 * metal
P 20 22 * metal
W 17 20 22 24
S 2 H metal P 11 P 12
P 5 43 * metal
P 23 43 * metal
W -3 35 31 51 vdd
S 8 H metal P 13 P 14
P 8 33 * metal
P 8 43 * metal
W 6 31 10 45 vdd
S 2 V metal P 15 P 16
P 8 22 * metal
P 8 28 * metal
W 6 20 10 30
S 2 V metal P 17 P 18
P 8 22 * metal
P 9 22 * metal
W 6 20 11 24
S 2 H metal P 19 P 20
P 14 30 * metal
P 14 35 * metal
W 12 28 16 37
S 2 V metal P 21 P 22
P 14 13 * metal
P 14 30 * metal
W 13 12 15 31
S 1 V metal P 23 P 24
P 14 13 * metal
P 20 13 * metal
W 13 12 21 14
S 1 H metal P 25 P 26
P 17 22 * poly
P 19 22 * poly
W 15 20 21 24
S 2 H poly P 27 P 28
P 17 20 * poly
P 17 25 * poly
W 16 19 18 26
S 1 V poly P 29 P 30
P 9 22 * poly
P 11 22 * poly
W 7 20 13 24
S 2 H poly P 31 P 32
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 33 P 34
P 8 45 * ntie
P 20 45 * ntie
W 6 43 22 47
S 2 H ntie P 35 P 36
P 5 39 * nwell
P 23 39 * nwell
W -21 13 49 65
S 26 H nwell P 37 P 38
P 8 7 * ndif
P 8 18 * ndif
W 5 4 11 21
S 3 V ndif P 39 P 40
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 i1
S 2 V metal2 P 41 P 42
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 f
S 2 V metal2 P 43 P 44
P 14 27 * pdif
P 14 38 * pdif
W 11 24 17 41
S 3 V pdif P 45 P 46
P 20 7 * ndif
P 20 18 * ndif
W 17 4 23 21
S 3 V ndif P 47 P 48
P 14 7 * ndif
P 14 18 * ndif
W 11 4 17 21
S 3 V ndif P 49 P 50
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47 i0
S 2 V metal2 P 51 P 52
P 17 5 * ntrans
P 17 20 * ntrans
W 16 4 18 21
S 1 V ntrans P 53 P 54
P 11 5 * ntrans
P 11 20 * ntrans
W 10 4 12 21
S 1 V ntrans P 55 P 56
P 17 25 * ptrans
P 17 40 * ptrans
W 16 24 18 41
S 1 V ptrans P 57 P 58
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 59 P 60
P 20 43 * metal
P 20 45 * metal
W 19 42 21 46 vdd
S 1 V metal P 61 P 62
P 8 45 * metal
P 20 45 * metal
W 7 44 21 46 vdd
S 1 H metal P 63 P 64
P 20 33 * metal
P 20 43 * metal
W 18 31 22 45 vdd
S 2 V metal P 65 P 66
P 8 2 * metal
P 8 13 * metal
W 6 0 10 15 vss
S 2 V metal P 67 P 68
P 8 2 * metal
P 20 2 * metal
W 6 0 22 4 vss
S 2 H metal P 69 P 70
E
