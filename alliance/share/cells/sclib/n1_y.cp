#cell1 n1_y idps_symb compose *
# 07-Oct-98 12:12 07-Oct-98 12:12 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 17 45
B 5 3 17 45
F F
C f 8 45 metal2 2 1 * * N f
C i 14 3 metal2 2 2 * * S i
C vdd 17 43 metal 8 3 * * E vdd
C vss 17 5 metal 8 4 * * E vss
C vss 5 5 metal 8 5 * * W vss
C vdd 5 43 metal 8 6 * * W vdd
C i 14 45 metal2 2 7 * * N i
C f 8 3 metal2 2 8 * * S f
I cdn1 14 16 0 "cdn" ly * *
I cdn2 8 16 0 "cdn" ly * *
I cdp1 8 28 0 "cdp" ly * *
I cdp2 8 34 0 "cdp" ly * *
I cdp3 14 33 0 "cdp" ly * *
I cdp4 14 39 0 "cdp" ly * *
I via12x1 8 21 0 "via12" ly * *
I via12x2 14 28 0 "via12" ly * *
I cpf1 13 23 0 "cpf" ly * *
I cbp1 14 3 0 "cbp" ly * *
I cbn1 8 45 0 "cbn" ly * *
I cbn2 14 45 0 "cbn" ly * *
I cbp2 8 3 0 "cbp" ly * *
P 5 43 * metal
P 17 43 * metal
W -3 35 25 51 vdd
S 8 H metal P 1 P 2
P 5 5 * metal
P 17 5 * metal
W -3 -3 25 13 vss
S 8 H metal P 3 P 4
P 8 16 * metal
P 8 34 * metal
W 6 14 10 36
S 2 V metal P 5 P 6
P 11 23 * poly
P 13 23 * poly
W 9 21 15 25
S 2 H poly P 7 P 8
P 11 20 * poly
P 11 25 * poly
W 10 19 12 26
S 1 V poly P 9 P 10
P 13 23 * metal
P 14 23 * metal
W 11 21 16 25
S 2 H metal P 11 P 12
P 14 23 * metal
P 14 28 * metal
W 13 22 15 29
S 1 V metal P 13 P 14
P 8 13 * ndif
P 8 18 * ndif
W 5 10 11 21
S 3 V ndif P 15 P 16
P 8 3 * ptie
P 14 3 * ptie
W 6 1 16 5
S 2 H ptie P 17 P 18
P 8 45 * ntie
P 14 45 * ntie
W 6 43 16 47
S 2 H ntie P 19 P 20
P 5 39 * nwell
P 17 39 * nwell
W -21 13 43 65
S 26 H nwell P 21 P 22
P 8 27 * pdif
P 8 38 * pdif
W 5 24 11 41
S 3 V pdif P 23 P 24
P 14 27 * pdif
P 14 39 * pdif
W 11 24 17 42
S 3 V pdif P 25 P 26
P 14 13 * ndif
P 14 18 * ndif
W 11 10 17 21
S 3 V ndif P 27 P 28
P 11 11 * ntrans
P 11 20 * ntrans
W 10 10 12 21
S 1 V ntrans P 29 P 30
P 11 25 * ptrans
P 11 40 * ptrans
W 10 24 12 41
S 1 V ptrans P 31 P 32
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 i
S 2 V metal2 P 33 P 34
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47 f
S 2 V metal2 P 35 P 36
P 14 43 * metal
P 14 45 * metal
W 13 42 15 46 vdd
S 1 V metal P 37 P 38
P 8 45 * metal
P 14 45 * metal
W 7 44 15 46 vdd
S 1 H metal P 39 P 40
P 14 33 * metal
P 14 43 * metal
W 12 31 16 45 vdd
S 2 V metal P 41 P 42
P 8 3 * metal
P 14 3 * metal
W 6 1 16 5 vss
S 2 H metal P 43 P 44
P 14 3 * metal
P 14 16 * metal
W 12 1 16 18 vss
S 2 V metal P 45 P 46
E
