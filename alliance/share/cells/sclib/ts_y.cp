#cell1 ts_y idps_symb compose *
# 07-Oct-98 12:32 07-Oct-98 12:32 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 6 53 48
B 5 6 53 48
F F
C vdd 5 46 metal 8 1 * * W vdd
C vdd 53 46 metal 8 2 * * E vdd
C vss 5 8 metal 8 3 * * W vss
C vss 53 8 metal 8 4 * * E vss
C i 38 48 metal2 2 5 * * N i
C i 38 6 metal2 2 6 * * S i
C t 44 48 metal2 2 7 * * N t
C t 44 6 metal2 2 8 * * S t
C v 26 48 metal2 2 9 * * N v
C v 26 6 metal2 2 10 * * S v
I cdp1 20 33 0 "cdp" ly * *
I cdp2 44 32 0 "cdp" ly * *
I cdp3 14 37 0 "cdp" ly * *
I cdp4 32 37 0 "cdp" ly * *
I cdn1 20 19 0 "cdn" ly * *
I cdn2 44 19 0 "cdn" ly * *
I cdn3 32 16 0 "cdn" ly * *
I cdn4 8 19 0 "cdn" ly * *
I cdn5 14 19 0 "cdn" ly * *
I cdp5 8 37 0 "cdp" ly * *
I cpf1 19 26 0 "cpf" ly * *
I cpf2 29 23 0 "cpf" ly * *
I cpf3 24 27 0 "cpf" ly * *
I via12x1 26 33 0 "via12" ly * *
I cpf4 49 23 0 "cpf" ly * *
I cdn6 50 11 0 "cdn" ly * *
I cdn7 38 11 0 "cdn" ly * *
I cdn8 26 11 0 "cdn" ly * *
I cbp1 32 6 0 "cbp" ly * *
I cbp2 8 6 0 "cbp" ly * *
I cpf5 34 25 0 "cpf" ly * *
I cpf6 39 27 0 "cpf" ly * *
I cdp6 50 41 0 "cdp" ly * *
I cdp7 38 41 0 "cdp" ly * *
I cdp8 26 41 0 "cdp" ly * *
I via12x2 38 19 0 "via12" ly * *
I via12x3 44 37 0 "via12" ly * *
I cbn1 32 48 0 "cbn" ly * *
I cbn2 8 48 0 "cbn" ly * *
I cbn3 14 48 0 "cbn" ly * *
I cbn4 20 48 0 "cbn" ly * *
I cbp3 14 6 0 "cbp" ly * *
I cbp4 20 6 0 "cbp" ly * *
I cdp9 38 48 0 "cdp" ly * *
I cdp10 50 48 0 "cdp" ly * *
I cdp11 50 32 0 "cdp" ly * *
I cdn9 26 6 0 "cdn" ly * *
I cdn10 38 6 0 "cdn" ly * *
I cdn11 50 6 0 "cdn" ly * *
P 5 8 * metal
P 53 8 * metal
W -3 0 61 16
S 8 H metal P 1 P 2
P 5 46 * metal
P 53 46 * metal
W -3 38 61 54
S 8 H metal P 3 P 4
P 11 28 * ptrans
P 11 45 * ptrans
W 10 27 12 46
S 1 V ptrans P 5 P 6
P 23 28 * ptrans
P 23 45 * ptrans
W 22 27 24 46
S 1 V ptrans P 7 P 8
P 29 28 * ptrans
P 29 45 * ptrans
W 28 27 30 46
S 1 V ptrans P 9 P 10
P 35 28 * ptrans
P 35 45 * ptrans
W 34 27 36 46
S 1 V ptrans P 11 P 12
P 5 42 * nwell
P 53 42 * nwell
W -21 16 79 68
S 26 H nwell P 13 P 14
P 39 28 * poly
P 47 28 * poly
W 38 27 48 29
S 1 H poly P 15 P 16
P 41 22 * poly
P 49 22 * poly
W 40 21 50 23
S 1 H poly P 17 P 18
P 35 22 * poly
P 35 28 * poly
W 34 21 36 29
S 1 V poly P 19 P 20
P 23 28 * poly
P 29 28 * poly
W 22 27 30 29
S 1 H poly P 21 P 22
P 23 22 * poly
P 23 28 * poly
W 22 21 24 29
S 1 V poly P 23 P 24
P 11 22 * poly
P 23 22 * poly
W 10 21 24 23
S 1 H poly P 25 P 26
P 8 15 * metal
P 49 15 * metal
W 7 14 50 16
S 1 H metal P 27 P 28
P 14 37 * metal
P 39 37 * metal
W 13 36 40 38
S 1 H metal P 29 P 30
P 14 19 * metal
P 14 37 * metal
W 13 18 15 38
S 1 V metal P 31 P 32
P 8 15 * metal
P 8 37 * metal
W 7 14 9 38
S 1 V metal P 33 P 34
P 20 19 * metal
P 20 33 * metal
W 19 18 21 34
S 1 V metal P 35 P 36
P 11 26 * poly
P 11 28 * poly
W 10 25 12 29
S 1 V poly P 37 P 38
P 11 26 * poly
P 19 26 * poly
W 10 25 20 27
S 1 H poly P 39 P 40
P 20 23 * metal
P 29 23 * metal
W 19 22 30 24
S 1 H metal P 41 P 42
P 44 19 * metal
P 44 37 * metal
W 42 17 46 39
S 2 V metal P 43 P 44
P 38 6 * ndif
P 38 20 * ndif
W 35 3 41 23
S 3 V ndif P 45 P 46
P 44 9 * ndif
P 44 19 * ndif
W 41 6 47 22
S 3 V ndif P 47 P 48
P 32 14 * ndif
P 32 20 * ndif
W 29 11 35 23
S 3 V ndif P 49 P 50
P 50 6 * ndif
P 50 20 * ndif
W 47 3 53 23
S 3 V ndif P 51 P 52
P 49 15 * metal
P 49 23 * metal
W 48 14 50 24
S 1 V metal P 53 P 54
P 39 27 * metal
P 39 37 * metal
W 38 26 40 38
S 1 V metal P 55 P 56
P 44 30 * pdif
P 44 52 * pdif
W 41 27 47 55
S 3 V pdif P 57 P 58
P 38 30 * pdif
P 38 52 * pdif
W 35 27 41 55
S 3 V pdif P 59 P 60
P 32 30 * pdif
P 32 43 * pdif
W 30 28 34 45
S 2 V pdif P 61 P 62
P 26 30 * pdif
P 26 43 * pdif
W 24 28 28 45
S 2 V pdif P 63 P 64
P 20 30 * pdif
P 20 43 * pdif
W 17 27 23 46
S 3 V pdif P 65 P 66
P 14 30 * pdif
P 14 43 * pdif
W 11 27 17 46
S 3 V pdif P 67 P 68
P 8 30 * pdif
P 8 43 * pdif
W 5 27 11 46
S 3 V pdif P 69 P 70
P 50 30 * pdif
P 50 52 * pdif
W 47 27 53 55
S 3 V pdif P 71 P 72
P 26 6 * ndif
P 26 20 * ndif
W 23 3 29 23
S 3 V ndif P 73 P 74
P 20 14 * ndif
P 20 20 * ndif
W 17 11 23 23
S 3 V ndif P 75 P 76
P 14 14 * ndif
P 14 19 * ndif
W 11 11 17 22
S 3 V ndif P 77 P 78
P 8 14 * ndif
P 8 20 * ndif
W 5 11 11 23
S 3 V ndif P 79 P 80
P 34 22 * metal
P 38 22 * metal
W 33 21 39 23
S 1 H metal P 81 P 82
P 38 19 * metal
P 38 22 * metal
W 36 17 40 24
S 2 V metal P 83 P 84
P 25 27 * metal
P 25 33 * metal
W 23 25 27 35
S 2 V metal P 85 P 86
P 26 41 * metal
P 26 46 * metal
W 24 39 28 48
S 2 V metal P 87 P 88
P 38 41 * metal
P 38 46 * metal
W 36 39 40 48
S 2 V metal P 89 P 90
P 50 32 * metal
P 50 46 * metal
W 48 30 52 48
S 2 V metal P 91 P 92
P 44 6 * metal2
P 44 48 * metal2
W 42 4 46 50
S 2 V metal2 P 93 P 94
P 38 6 * metal2
P 38 48 * metal2
W 36 4 40 50
S 2 V metal2 P 95 P 96
P 26 6 * metal2
P 26 48 * metal2
W 24 4 28 50
S 2 V metal2 P 97 P 98
P 50 6 * allowM2
P 50 48 * allowM2
W 48 4 52 50
S 2 V allowM2 P 99 P 100
P 32 6 * allowM2
P 32 48 * allowM2
W 30 4 34 50
S 2 V allowM2 P 101 P 102
P 20 6 * allowM2
P 20 48 * allowM2
W 18 4 22 50
S 2 V allowM2 P 103 P 104
P 14 6 * allowM2
P 14 48 * allowM2
W 12 4 16 50
S 2 V allowM2 P 105 P 106
P 8 6 * allowM2
P 8 48 * allowM2
W 6 4 10 50
S 2 V allowM2 P 107 P 108
P 34 22 * metal
P 34 25 * metal
W 32 20 36 27
S 2 V metal P 109 P 110
P 47 28 * ptrans
P 47 54 * ptrans
W 46 27 48 55
S 1 V ptrans P 111 P 112
P 41 28 * ptrans
P 41 54 * ptrans
W 40 27 42 55
S 1 V ptrans P 113 P 114
P 47 7 * ntrans
P 47 22 * ntrans
W 46 6 48 23
S 1 V ntrans P 115 P 116
P 41 7 * ntrans
P 41 22 * ntrans
W 40 6 42 23
S 1 V ntrans P 117 P 118
P 29 12 * ntrans
P 29 22 * ntrans
W 28 11 30 23
S 1 V ntrans P 119 P 120
P 23 12 * ntrans
P 23 22 * ntrans
W 22 11 24 23
S 1 V ntrans P 121 P 122
P 11 12 * ntrans
P 11 22 * ntrans
W 10 11 12 23
S 1 V ntrans P 123 P 124
P 35 12 * ntrans
P 35 22 * ntrans
W 34 11 36 23
S 1 V ntrans P 125 P 126
E
