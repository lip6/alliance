#cell1 d1_y idps_symb compose *
# 07-Oct-98 12:08 07-Oct-98 12:08 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 41 45
B 5 3 41 45
F F
C t 38 3 metal2 2 1 * * S t
C t 38 45 metal2 2 2 * * N t
C vdd 41 43 metal 8 3 * * E vdd
C vss 41 5 metal 8 4 * * E vss
C vdd 5 43 metal 8 5 * * W vdd
C vss 5 5 metal 8 6 * * W vss
C i 14 3 metal2 2 7 * * S i
C i 14 45 metal2 2 8 * * N i
I cbn1 32 45 0 "cbn" ly * *
I cbp1 32 3 0 "cbp" ly * *
I cbn2 20 45 0 "cbn" ly * *
I cbp2 20 3 0 "cbp" ly * *
I cdp1 26 29 0 "cdp" ly * *
I via12x1 38 25 0 "via12" ly * *
I cdp2 14 33 0 "cdp" ly * *
I cpf1 35 20 0 "cpf" ly * *
I cdn1 26 15 0 "cdn" ly * *
I cdn2 38 15 0 "cdn" ly * *
I cdn3 32 15 0 "cdn" ly * *
I cdp3 38 30 0 "cdp" ly * *
I cdp4 32 32 0 "cdp" ly * *
I cpf2 30 24 0 "cpf" ly * *
I cdn4 20 15 0 "cdn" ly * *
I cpf3 17 22 0 "cpf" ly * *
I cdp5 8 29 0 "cdp" ly * *
I cbp3 8 3 0 "cbp" ly * *
I via12x2 14 28 0 "via12" ly * *
I cpf4 12 24 0 "cpf" ly * *
I cdp6 20 30 0 "cdp" ly * *
I cbn3 8 45 0 "cbn" ly * *
I cdn5 14 15 0 "cdn" ly * *
I cdn6 20 15 0 "cdn" ly * *
I cdn7 8 15 0 "cdn" ly * *
I cbn4 26 45 0 "cbn" ly * *
I cbn5 14 45 0 "cbn" ly * *
I cbp4 26 3 0 "cbp" ly * *
I cbp5 14 3 0 "cbp" ly * *
I cbp6 38 3 0 "cbp" ly * *
I cbn6 38 45 0 "cbn" ly * *
P 35 9 * ntrans
P 35 18 * ntrans
W 34 8 36 19
S 1 V ntrans P 1 P 2
P 35 25 * ptrans
P 35 40 * ptrans
W 34 24 36 41
S 1 V ptrans P 3 P 4
P 26 29 * metal
P 26 28 * metal
W 25 27 27 30
S 1 V metal P 5 P 6
P 34 28 * metal
P 34 20 * metal
W 33 19 35 29
S 1 V metal P 7 P 8
P 34 28 * metal
P 26 28 * metal
W 25 27 35 29
S 1 H metal P 9 P 10
P 38 45 * metal2
P 38 3 * metal2
W 36 1 40 47
S 2 V metal2 P 11 P 12
P 26 45 * allowM2
P 26 3 * allowM2
W 24 1 28 47
S 2 V allowM2 P 13 P 14
P 26 20 * metal
P 26 15 * metal
W 25 14 27 21
S 1 V metal P 15 P 16
P 35 20 * metal
P 26 20 * metal
W 25 19 36 21
S 1 H metal P 17 P 18
P 30 24 * metal
P 21 24 * metal
W 20 23 31 25
S 1 H metal P 19 P 20
P 26 32 * pdif
P 26 27 * pdif
W 23 24 29 35
S 3 V pdif P 21 P 22
P 14 33 * metal
P 14 42 * metal
W 12 31 16 44 vdd
S 2 V metal P 23 P 24
P 39 30 * metal
P 38 30 * metal
W 37 29 40 31
S 1 H metal P 25 P 26
P 39 15 * metal
P 39 30 * metal
W 38 14 40 31
S 1 V metal P 27 P 28
P 21 15 * metal
P 21 27 * metal
W 20 14 22 28
S 1 V metal P 29 P 30
P 29 12 * ntrans
P 29 18 * ntrans
W 28 11 30 19
S 1 V ntrans P 31 P 32
P 38 32 * pdif
P 38 27 * pdif
W 35 24 41 35
S 3 V pdif P 33 P 34
P 32 32 * pdif
P 32 27 * pdif
W 29 24 35 35
S 3 V pdif P 35 P 36
P 29 25 * ptrans
P 29 34 * ptrans
W 28 24 30 35
S 1 V ptrans P 37 P 38
P 29 18 * poly
P 29 25 * poly
W 28 17 30 26
S 1 V poly P 39 P 40
P 29 24 * poly
P 30 24 * poly
W 28 23 31 25
S 1 H poly P 41 P 42
P 35 18 * poly
P 35 25 * poly
W 34 17 36 26
S 1 V poly P 43 P 44
P 32 7 * metal
P 32 15 * metal
W 30 5 34 17 vss
S 2 V metal P 45 P 46
P 32 32 * metal
P 32 41 * metal
W 30 30 34 43 vdd
S 2 V metal P 47 P 48
P 37 15 * metal
P 39 15 * metal
W 35 13 41 17
S 2 H metal P 49 P 50
P 20 3 * allowM2
P 20 45 * allowM2
W 18 1 22 47
S 2 V allowM2 P 51 P 52
P 20 32 * pdif
P 20 27 * pdif
W 17 24 23 35
S 3 V pdif P 53 P 54
P 32 45 * allowM2
P 32 3 * allowM2
W 30 1 34 47
S 2 V allowM2 P 55 P 56
P 5 39 * nwell
P 41 39 * nwell
W -21 13 67 65
S 26 H nwell P 57 P 58
P 5 5 * metal
P 41 5 * metal
W -3 -3 49 13 vss
S 8 H metal P 59 P 60
P 5 43 * metal
P 41 43 * metal
W -3 35 49 51 vdd
S 8 H metal P 61 P 62
P 20 27 * metal
P 21 27 * metal
W 18 25 23 29
S 2 H metal P 63 P 64
P 11 24 * poly
P 12 24 * poly
W 10 23 13 25
S 1 H poly P 65 P 66
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 67 P 68
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 69 P 70
P 17 25 * ptrans
P 17 34 * ptrans
W 16 24 18 35
S 1 V ptrans P 71 P 72
P 11 25 * ptrans
P 11 34 * ptrans
W 10 24 12 35
S 1 V ptrans P 73 P 74
P 14 32 * pdif
P 14 27 * pdif
W 11 24 17 35
S 3 V pdif P 75 P 76
P 20 32 * pdif
P 20 27 * pdif
W 17 24 23 35
S 3 V pdif P 77 P 78
P 8 32 * pdif
P 8 27 * pdif
W 5 24 11 35
S 3 V pdif P 79 P 80
P 20 27 * metal
P 20 30 * metal
W 18 25 22 32
S 2 V metal P 81 P 82
P 17 12 * ntrans
P 17 18 * ntrans
W 16 11 18 19
S 1 V ntrans P 83 P 84
P 11 12 * ntrans
P 11 18 * ntrans
W 10 11 12 19
S 1 V ntrans P 85 P 86
P 8 15 * metal
P 8 29 * metal
W 7 14 9 30
S 1 V metal P 87 P 88
P 14 7 * metal
P 14 15 * metal
W 12 5 16 17 vss
S 2 V metal P 89 P 90
P 11 18 * poly
P 11 25 * poly
W 10 17 12 26
S 1 V poly P 91 P 92
P 17 18 * poly
P 17 25 * poly
W 16 17 18 26
S 1 V poly P 93 P 94
P 8 19 * metal
P 17 19 * metal
W 7 18 18 20
S 1 H metal P 95 P 96
P 17 19 * metal
P 17 22 * metal
W 15 17 19 24
S 2 V metal P 97 P 98
P 19 15 * metal
P 21 15 * metal
W 17 13 23 17
S 2 H metal P 99 P 100
P 12 24 * metal
P 13 24 * metal
W 11 23 14 25
S 1 H metal P 101 P 102
P 13 24 * metal
P 13 28 * metal
W 12 23 14 29
S 1 V metal P 103 P 104
P 13 28 * metal
P 14 28 * metal
W 12 27 15 29
S 1 H metal P 105 P 106
P 7 45 * ntie
P 39 45 * ntie
W 4 42 42 48
S 3 H ntie P 107 P 108
P 7 3 * ptie
P 39 3 * ptie
W 4 0 42 6
S 3 H ptie P 109 P 110
E
