#cell1 zbli_y idps_symb compose *
# 07-Oct-98 12:35 07-Oct-98 12:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 23 45
B 5 3 23 45
F F
C vss 23 5 metal 8 1 * * E vss
C vss 5 5 metal 8 2 * * W vss
C vdd 23 43 metal 8 3 * * E vdd
C vdd 5 43 metal 8 4 * * W vdd
C f 14 45 metal2 2 5 * * N f
C f 14 3 metal2 2 6 * * S f
C i 8 3 metal2 2 7 * * S i
C i 8 45 metal2 2 8 * * N i
I cbp1 20 3 0 "cbp" ly * *
I cbn1 8 37 0 "cbn" ly * *
I cbn2 8 41 0 "cbn" ly * *
I cdp1 20 35 0 "cdp" ly * *
I cbp2 8 3 0 "cbp" ly * *
I cbp3 14 3 0 "cbp" ly * *
I cbn3 8 45 0 "cbn" ly * *
I cdp2 14 31 0 "cdp" ly * *
I via12x1 14 28 0 "via12" ly * *
I via12x2 8 23 0 "via12" ly * *
I cpf1 15 23 0 "cpf" ly * *
I cdp3 19 45 0 "cdp" ly * *
I cpf2 9 18 0 "cpf" ly * *
I cpf3 20 18 0 "cpf" ly * *
I cdn1 8 12 0 "cdn" ly * *
I cdn2 14 13 0 "cdn" ly * *
I cdp4 8 30 0 "cdp" ly * *
P 8 3 * ptie
P 20 3 * ptie
W 6 1 22 5
S 2 H ptie P 1 P 2
P 16 41 * ptrans
P 22 41 * ptrans
W 13 38 25 44
S 3 H ptrans P 3 P 4
P 14 23 * metal
P 15 23 * metal
W 13 22 16 24
S 1 H metal P 5 P 6
P 14 13 * metal
P 14 30 * metal
W 13 12 15 31
S 1 V metal P 7 P 8
P 15 23 * poly
P 15 24 * poly
W 14 22 16 25
S 1 V poly P 9 P 10
P 14 28 * metal
P 14 31 * metal
W 12 26 16 33
S 2 V metal P 11 P 12
P 15 24 * poly
P 17 24 * poly
W 14 23 18 25
S 1 H poly P 13 P 14
P 17 24 * poly
P 17 34 * poly
W 16 23 18 35
S 1 V poly P 15 P 16
P 8 18 * metal
P 8 23 * metal
W 6 16 10 25
S 2 V metal P 17 P 18
P 8 18 * metal
P 9 18 * metal
W 7 17 10 19
S 1 H metal P 19 P 20
P 9 18 * poly
P 11 18 * poly
W 7 16 13 20
S 2 H poly P 21 P 22
P 11 17 * poly
P 11 25 * poly
W 10 16 12 26
S 1 V poly P 23 P 24
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 25 P 26
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 27 P 28
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 29 P 30
P 11 18 * poly
P 20 18 * poly
W 10 17 21 19
S 1 H poly P 31 P 32
P 14 27 * pdif
P 14 32 * pdif
W 11 24 17 35
S 3 V pdif P 33 P 34
P 8 10 * ndif
P 8 15 * ndif
W 6 8 10 17
S 2 V ndif P 35 P 36
P 14 10 * ndif
P 14 15 * ndif
W 12 8 16 17
S 2 V ndif P 37 P 38
P 5 5 * metal
P 23 5 * metal
W -3 -3 31 13 vss
S 8 H metal P 39 P 40
P 11 8 * ntrans
P 11 17 * ntrans
W 10 7 12 18
S 1 V ntrans P 41 P 42
P 5 39 * nwell
P 23 39 * nwell
W -21 13 49 65
S 26 H nwell P 43 P 44
P 5 43 * metal
P 23 43 * metal
W -3 35 31 51
S 8 H metal P 45 P 46
P 8 27 * pdif
P 8 32 * pdif
W 6 25 10 34
S 2 V pdif P 47 P 48
P 11 25 * ptrans
P 11 34 * ptrans
W 10 24 12 35
S 1 V ptrans P 49 P 50
P 20 18 * metal
P 20 35 * metal
W 19 17 21 36
S 1 V metal P 51 P 52
P 16 34 * poly
P 17 34 * poly
W 15 33 18 35
S 1 H poly P 53 P 54
P 16 34 * poly
P 16 41 * poly
W 15 33 17 42
S 1 V poly P 55 P 56
P 8 37 * ntie
P 8 45 * ntie
W 5 34 11 48
S 3 V ntie P 57 P 58
P 8 30 * metal
P 8 45 * metal
W 6 28 10 47
S 2 V metal P 59 P 60
P 8 45 * metal
P 19 45 * metal
W 6 43 21 47
S 2 H metal P 61 P 62
P 8 3 * metal
P 8 12 * metal
W 6 1 10 14
S 2 V metal P 63 P 64
P 8 3 * metal
P 20 3 * metal
W 6 1 22 5
S 2 H metal P 65 P 66
P 18 37 * pdif
P 20 37 * pdif
W 15 34 23 40
S 3 H pdif P 67 P 68
P 20 35 * pdif
P 20 37 * pdif
W 17 32 23 40
S 3 V pdif P 69 P 70
E
