#cell1 one_y idps_symb compose *
# 07-Oct-98 12:27 07-Oct-98 12:27 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 17 45
B 5 3 17 45
F F
C t 14 3 metal2 2 1 * * S t
C vdd 17 43 metal 8 2 * * E vdd
C vdd 5 43 metal 8 3 * * W vdd
C t 14 45 metal2 2 4 * * N t
C vss 17 5 metal 8 5 * * E vss
C vss 5 5 metal 8 6 * * W vss
I cdp1 8 40 0 "cdp" ly * *
I cbp1 14 15 0 "cbp" ly * *
I cbp2 14 10 0 "cbp" ly * *
I cdp2 14 35 0 "cdp" ly * *
I cdp3 8 29 0 "cdp" ly * *
I cdp4 8 35 0 "cdp" ly * *
I via12x1 14 29 0 "via12" ly * *
I cpf1 9 23 0 "cpf" ly * *
I cbp3 14 3 0 "cbp" ly * *
I cbn1 8 46 0 "cbn" ly * *
I cbn2 14 46 0 "cbn" ly * *
I cbp4 8 3 0 "cbp" ly * *
P 8 46 * ntie
P 14 46 * ntie
W 6 44 16 48
S 2 H ntie P 1 P 2
P 5 39 * nwell
P 17 39 * nwell
W -21 13 43 65
S 26 H nwell P 3 P 4
P 11 25 * ptrans
P 11 43 * ptrans
W 10 24 12 44
S 1 V ptrans P 5 P 6
P 14 29 * metal
P 14 35 * metal
W 13 28 15 36
S 1 V metal P 7 P 8
P 14 3 * ptie
P 14 3 * ptie
W 12 1 16 5
S 2 H ptie P 9 P 10
P 8 3 * ptie
P 14 3 * ptie
W 6 1 16 5
S 2 H ptie P 11 P 12
P 14 3 * ptie
P 14 16 * ptie
W 11 0 17 19
S 3 V ptie P 13 P 14
P 8 27 * pdif
P 8 40 * pdif
W 5 24 11 43
S 3 V pdif P 15 P 16
P 14 27 * pdif
P 14 41 * pdif
W 11 24 17 44
S 3 V pdif P 17 P 18
P 9 4 * metal
P 14 4 * metal
W 7 2 16 6 vss
S 2 H metal P 19 P 20
P 8 4 * metal
P 9 4 * metal
W 6 2 11 6 vss
S 2 H metal P 21 P 22
P 9 4 * metal
P 9 5 * metal
W 7 2 11 7 vss
S 2 V metal P 23 P 24
P 5 5 * metal
P 9 5 * metal
W -3 -3 17 13 vss
S 8 H metal P 25 P 26
P 9 5 * metal
P 14 5 * metal
W 1 -3 22 13 vss
S 8 H metal P 27 P 28
P 14 5 * metal
P 17 5 * metal
W 6 -3 25 13 vss
S 8 H metal P 29 P 30
P 14 4 * metal
P 14 5 * metal
W 13 3 15 6 vss
S 1 V metal P 31 P 32
P 14 5 * metal
P 14 15 * metal
W 12 3 16 17 vss
S 2 V metal P 33 P 34
P 9 5 * metal
P 9 24 * metal
W 7 3 11 26 vss
S 2 V metal P 35 P 36
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 37 P 38
P 8 43 * metal
P 17 43 * metal
W 0 35 25 51 vdd
S 8 H metal P 39 P 40
P 5 43 * metal
P 8 43 * metal
W -3 35 16 51 vdd
S 8 H metal P 41 P 42
P 8 43 * metal
P 8 46 * metal
W 7 42 9 47 vdd
S 1 V metal P 43 P 44
P 8 29 * metal
P 8 43 * metal
W 6 27 10 45 vdd
S 2 V metal P 45 P 46
P 8 3 * allowM2
P 8 46 * allowM2
W 6 1 10 48
S 2 V allowM2 P 47 P 48
P 11 23 * poly
P 11 25 * poly
W 10 22 12 26
S 1 V poly P 49 P 50
P 9 23 * poly
P 11 23 * poly
W 8 22 12 24
S 1 H poly P 51 P 52
E
