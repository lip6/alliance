#cell1 sum_y idps_symb compose *
# 07-Oct-98 12:30 07-Oct-98 12:30 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 53 45
B 5 3 53 45
F F
C vdd 53 43 metal 8 1 * * E vdd
C vdd 5 43 metal 8 2 * * W vdd
C vss 5 5 metal 8 3 * * W vss
C vss 53 5 metal 8 4 * * E vss
C pi 14 45 metal2 2 5 * * N pi
C ci 20 45 metal2 2 6 * * N ci
C si 26 45 metal2 2 7 * * N si
C c0b 44 45 metal2 2 8 * * N c0b
C f 50 45 metal2 2 9 * * N f
C pi 14 3 metal2 2 10 * * S pi
C ci 20 3 metal2 2 11 * * S ci
C si 26 3 metal2 2 12 * * S si
C c0b 44 3 metal2 2 13 * * S c0b
C f 50 3 metal2 2 14 * * S f
I cdn1 8 13 0 "cdn" ly * *
I cpf1 13 26 0 "cpf" ly * *
I cpf2 19 21 0 "cpf" ly * *
I cpf3 26 18 0 "cpf" ly * *
I via12x1 26 13 0 "via12" ly * *
I via12x2 20 13 0 "via12" ly * *
I via12x3 14 13 0 "via12" ly * *
I cdp1 50 35 0 "cdp" ly * *
I cdp2 8 30 0 "cdp" ly * *
I cdp3 8 35 0 "cdp" ly * *
I cdp4 32 35 0 "cdp" ly * *
I via12x4 44 26 0 "via12" ly * *
I cpf4 33 21 0 "cpf" ly * *
I cpf5 39 21 0 "cpf" ly * *
I cpf6 45 21 0 "cpf" ly * *
I cdn2 26 8 0 "cdn" ly * *
I cdn3 38 8 0 "cdn" ly * *
I cdn4 44 13 0 "cdn" ly * *
I cdn5 32 13 0 "cdn" ly * *
I cdn6 50 13 0 "cdn" ly * *
I cdp5 44 35 0 "cdp" ly * *
I cdp6 26 35 0 "cdp" ly * *
I cdp7 26 40 0 "cdp" ly * *
I cdp8 38 40 0 "cdp" ly * *
I via12x5 50 17 0 "via12" ly * *
I via12x6 50 30 0 "via12" ly * *
I cbn1 50 46 0 "cbn" ly * *
I cbn2 44 46 0 "cbn" ly * *
I cbn3 38 46 0 "cbn" ly * *
I cbn4 32 46 0 "cbn" ly * *
I cbn5 26 46 0 "cbn" ly * *
I cbn6 20 46 0 "cbn" ly * *
I cbn7 14 46 0 "cbn" ly * *
I cbn8 8 46 0 "cbn" ly * *
I cbp1 8 2 0 "cbp" ly * *
I cbp2 14 2 0 "cbp" ly * *
I cbp3 20 2 0 "cbp" ly * *
I cbp4 26 2 0 "cbp" ly * *
I cbp5 32 2 0 "cbp" ly * *
I cbp6 38 2 0 "cbp" ly * *
I cbp7 44 2 0 "cbp" ly * *
I cbp8 50 2 0 "cbp" ly * *
P 8 3 * allowM2
P 8 45 * allowM2
W 6 1 10 47
S 2 V allowM2 P 1 P 2
P 20 13 * metal
P 20 21 * metal
W 19 12 21 22
S 1 V metal P 3 P 4
P 44 21 * metal
P 44 26 * metal
W 43 20 45 27
S 1 V metal P 5 P 6
P 26 13 * metal
P 26 18 * metal
W 25 12 27 19
S 1 V metal P 7 P 8
P 14 13 * metal
P 14 26 * metal
W 13 12 15 27
S 1 V metal P 9 P 10
P 11 26 * poly
P 12 26 * poly
W 10 25 13 27
S 1 H poly P 11 P 12
P 17 21 * poly
P 19 21 * poly
W 16 20 20 22
S 1 H poly P 13 P 14
P 45 21 * poly
P 47 21 * poly
W 44 20 48 22
S 1 H poly P 15 P 16
P 39 21 * poly
P 41 21 * poly
W 38 20 42 22
S 1 H poly P 17 P 18
P 33 21 * poly
P 35 21 * poly
W 32 20 36 22
S 1 H poly P 19 P 20
P 23 18 * poly
P 29 18 * poly
W 20 15 32 21
S 3 H poly P 21 P 22
P 39 21 * metal
P 39 26 * metal
W 38 20 40 27
S 1 V metal P 23 P 24
P 13 26 * metal
P 39 26 * metal
W 12 25 40 27
S 1 H metal P 25 P 26
P 50 13 * metal
P 50 35 * metal
W 48 11 52 37
S 2 V metal P 27 P 28
P 8 30 * metal
P 50 30 * metal
W 7 29 51 31
S 1 H metal P 29 P 30
P 8 13 * metal
P 8 35 * metal
W 7 12 9 36
S 1 V metal P 31 P 32
P 50 3 * metal2
P 50 45 * metal2
W 48 1 52 47
S 2 V metal2 P 33 P 34
P 44 7 * ndif
P 44 14 * ndif
W 42 5 46 16
S 2 V ndif P 35 P 36
P 50 7 * ndif
P 50 14 * ndif
W 47 4 53 17
S 3 V ndif P 37 P 38
P 50 29 * pdif
P 50 41 * pdif
W 47 26 53 44
S 3 V pdif P 39 P 40
P 44 29 * pdif
P 44 41 * pdif
W 42 27 46 43
S 2 V pdif P 41 P 42
P 41 16 * poly
P 41 27 * poly
W 40 15 42 28
S 1 V poly P 43 P 44
P 47 16 * poly
P 47 27 * poly
W 46 15 48 28
S 1 V poly P 45 P 46
P 47 27 * ptrans
P 47 43 * ptrans
W 46 26 48 44
S 1 V ptrans P 47 P 48
P 41 27 * ptrans
P 41 43 * ptrans
W 40 26 42 44
S 1 V ptrans P 49 P 50
P 47 5 * ntrans
P 47 16 * ntrans
W 46 4 48 17
S 1 V ntrans P 51 P 52
P 41 5 * ntrans
P 41 16 * ntrans
W 40 4 42 17
S 1 V ntrans P 53 P 54
P 11 27 * ptrans
P 11 43 * ptrans
W 10 26 12 44
S 1 V ptrans P 55 P 56
P 17 27 * ptrans
P 17 43 * ptrans
W 16 26 18 44
S 1 V ptrans P 57 P 58
P 23 27 * ptrans
P 23 43 * ptrans
W 22 26 24 44
S 1 V ptrans P 59 P 60
P 29 27 * ptrans
P 29 43 * ptrans
W 28 26 30 44
S 1 V ptrans P 61 P 62
P 35 27 * ptrans
P 35 43 * ptrans
W 34 26 36 44
S 1 V ptrans P 63 P 64
P 44 3 * metal2
P 44 45 * metal2
W 42 1 46 47
S 2 V metal2 P 65 P 66
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 67 P 68
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47 pi
S 2 V metal2 P 69 P 70
P 26 3 * metal2
P 26 45 * metal2
W 24 1 28 47
S 2 V metal2 P 71 P 72
P 26 35 * metal
P 26 40 * metal
W 24 33 28 42
S 2 V metal P 73 P 74
P 38 7 * ndif
P 38 14 * ndif
W 36 5 40 16
S 2 V ndif P 75 P 76
P 32 7 * ndif
P 32 14 * ndif
W 30 5 34 16
S 2 V ndif P 77 P 78
P 26 7 * ndif
P 26 14 * ndif
W 24 5 28 16
S 2 V ndif P 79 P 80
P 20 7 * ndif
P 20 14 * ndif
W 18 5 22 16
S 2 V ndif P 81 P 82
P 14 7 * ndif
P 14 14 * ndif
W 12 5 16 16
S 2 V ndif P 83 P 84
P 8 7 * ndif
P 8 14 * ndif
W 5 4 11 17
S 3 V ndif P 85 P 86
P 38 29 * pdif
P 38 41 * pdif
W 36 27 40 43
S 2 V pdif P 87 P 88
P 32 29 * pdif
P 32 41 * pdif
W 30 27 34 43
S 2 V pdif P 89 P 90
P 26 29 * pdif
P 26 41 * pdif
W 24 27 28 43
S 2 V pdif P 91 P 92
P 20 29 * pdif
P 20 41 * pdif
W 18 27 22 43
S 2 V pdif P 93 P 94
P 14 29 * pdif
P 14 41 * pdif
W 12 27 16 43
S 2 V pdif P 95 P 96
P 8 29 * pdif
P 8 41 * pdif
W 5 26 11 44
S 3 V pdif P 97 P 98
P 35 16 * poly
P 35 27 * poly
W 34 15 36 28
S 1 V poly P 99 P 100
P 29 16 * poly
P 29 27 * poly
W 28 15 30 28
S 1 V poly P 101 P 102
P 23 16 * poly
P 23 27 * poly
W 22 15 24 28
S 1 V poly P 103 P 104
P 17 16 * poly
P 17 27 * poly
W 16 15 18 28
S 1 V poly P 105 P 106
P 11 16 * poly
P 11 27 * poly
W 10 15 12 28
S 1 V poly P 107 P 108
P 19 22 * metal
P 33 22 * metal
W 18 21 34 23
S 1 H metal P 109 P 110
P 35 5 * ntrans
P 35 16 * ntrans
W 34 4 36 17
S 1 V ntrans P 111 P 112
P 29 5 * ntrans
P 29 16 * ntrans
W 28 4 30 17
S 1 V ntrans P 113 P 114
P 23 5 * ntrans
P 23 16 * ntrans
W 22 4 24 17
S 1 V ntrans P 115 P 116
P 17 5 * ntrans
P 17 16 * ntrans
W 16 4 18 17
S 1 V ntrans P 117 P 118
P 11 5 * ntrans
P 11 16 * ntrans
W 10 4 12 17
S 1 V ntrans P 119 P 120
P 32 35 * metal
P 44 35 * metal
W 31 34 45 36
S 1 H metal P 121 P 122
P 32 13 * metal
P 44 13 * metal
W 31 12 45 14
S 1 H metal P 123 P 124
P 5 39 * nwell
P 53 39 * nwell
W -21 13 79 65
S 26 H nwell P 125 P 126
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 127 P 128
P 32 3 * allowM2
P 32 45 * allowM2
W 30 1 34 47
S 2 V allowM2 P 129 P 130
P 5 43 * metal
P 53 43 * metal
W -3 35 61 51
S 8 H metal P 131 P 132
P 5 5 * metal
P 53 5 * metal
W -3 -3 61 13
S 8 H metal P 133 P 134
E
