#cell1 tsn_y idps_symb compose *
# 07-Oct-98 12:32 07-Oct-98 12:32 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 29 45
B 5 3 29 45
F F
C vdd 29 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 29 5 metal 8 3 * * E vss
C vss 5 5 metal 8 4 * * W vss
C i 14 45 metal2 2 5 * * N i
C i 14 3 metal2 2 6 * * S i
C v 20 45 metal2 2 7 * * N v
C v 20 3 metal2 2 8 * * S v
C f 26 45 metal2 2 9 * * N f
C f 26 3 metal2 2 10 * * S f
I cdn1 8 13 0 "cdn" ly * *
I cdp1 26 28 0 "cdp" ly * *
I cdp2 26 32 0 "cdp" ly * *
I via12x1 26 23 0 "via12" ly * *
I cdp3 14 37 0 "cdp" ly * *
I cdn2 14 13 0 "cdn" ly * *
I cpf1 21 24 0 "cpf" ly * *
I cpf2 21 19 0 "cpf" ly * *
I cpf3 16 24 0 "cpf" ly * *
I cpf4 12 19 0 "cpf" ly * *
I via12x2 20 13 0 "via12" ly * *
I via12x3 14 29 0 "via12" ly * *
I cdn3 26 13 0 "cdn" ly * *
I cbn1 8 43 0 "cbn" ly * *
I cbp1 8 5 0 "cbp" ly * *
I cdp4 8 33 0 "cdp" ly * *
I cdp5 14 43 0 "cdp" ly * *
I cdn4 14 7 0 "cdn" ly * *
P 5 5 * metal
P 29 5 * metal
W -3 -3 37 13 vss
S 8 H metal P 1 P 2
P 8 11 * ndif
P 8 16 * ndif
W 5 8 11 19
S 3 V ndif P 3 P 4
P 5 43 * metal
P 29 43 * metal
W -3 35 37 51 vdd
S 8 H metal P 5 P 6
P 8 43 * metal
P 8 45 * metal
W 7 42 9 46 vdd
S 1 V metal P 7 P 8
P 26 13 * metal
P 26 33 * metal
W 24 11 28 35
S 2 V metal P 9 P 10
P 20 5 * ndif
P 20 16 * ndif
W 17 2 23 19
S 3 V ndif P 11 P 12
P 14 5 * ndif
P 14 16 * ndif
W 11 2 17 19
S 3 V ndif P 13 P 14
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 v1
S 2 V metal2 P 15 P 16
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47 f
S 2 V metal2 P 17 P 18
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47 v2
S 2 V metal2 P 19 P 20
P 5 39 * nwell
P 29 39 * nwell
W -21 13 55 65
S 26 H nwell P 21 P 22
P 11 18 * poly
P 11 25 * poly
W 10 17 12 26
S 1 V poly P 23 P 24
P 17 18 * poly
P 17 25 * poly
W 16 17 18 26
S 1 V poly P 25 P 26
P 21 18 * poly
P 23 18 * poly
W 20 17 24 19
S 1 H poly P 27 P 28
P 21 25 * poly
P 23 25 * poly
W 20 24 24 26
S 1 H poly P 29 P 30
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 31 P 32
P 14 27 * pdif
P 14 49 * pdif
W 11 24 17 52
S 3 V pdif P 33 P 34
P 20 27 * pdif
P 20 49 * pdif
W 17 24 23 52
S 3 V pdif P 35 P 36
P 26 27 * pdif
P 26 49 * pdif
W 23 24 29 52
S 3 V pdif P 37 P 38
P 26 5 * ndif
P 26 16 * ndif
W 23 2 29 19
S 3 V ndif P 39 P 40
P 8 13 * metal
P 8 33 * metal
W 7 12 9 34
S 1 V metal P 41 P 42
P 8 33 * metal
P 21 33 * metal
W 7 32 22 34
S 1 H metal P 43 P 44
P 21 24 * metal
P 21 33 * metal
W 20 23 22 34
S 1 V metal P 45 P 46
P 20 13 * metal
P 20 19 * metal
W 18 11 22 21
S 2 V metal P 47 P 48
P 12 19 * metal
P 21 19 * metal
W 10 17 23 21
S 2 H metal P 49 P 50
P 14 24 * metal
P 14 29 * metal
W 12 22 16 31
S 2 V metal P 51 P 52
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 53 P 54
P 14 24 * metal
P 16 24 * metal
W 12 22 18 26
S 2 H metal P 55 P 56
P 14 37 * metal
P 14 43 * metal
W 12 35 16 45
S 2 V metal P 57 P 58
P 14 5 * metal
P 14 13 * metal
W 12 3 16 15
S 2 V metal P 59 P 60
P 23 25 * ptrans
P 23 51 * ptrans
W 22 24 24 52
S 1 V ptrans P 61 P 62
P 17 25 * ptrans
P 17 51 * ptrans
W 16 24 18 52
S 1 V ptrans P 63 P 64
P 23 3 * ntrans
P 23 18 * ntrans
W 22 2 24 19
S 1 V ntrans P 65 P 66
P 17 3 * ntrans
P 17 18 * ntrans
W 16 2 18 19
S 1 V ntrans P 67 P 68
P 11 9 * ntrans
P 11 18 * ntrans
W 10 8 12 19
S 1 V ntrans P 69 P 70
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 71 P 72
E
