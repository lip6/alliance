#cell1 cry_y idps_symb compose *
# 07-Oct-98 12:08 07-Oct-98 12:08 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 3 41 45
B 5 3 41 45
F F
C pi 8 45 metal2 2 1 * * N pi
C si 32 45 metal2 2 2 * * N si
C pi 8 3 metal2 2 3 * * S pi
C ci 14 3 metal2 2 4 * * S ci
C si 32 3 metal2 2 5 * * S si
C f 20 45 metal2 2 6 * * N f
C f 20 3 metal2 2 7 * * S f
C vdd 5 43 metal 8 8 * * W vdd
C vdd 41 43 metal 8 9 * * E vdd
C vss 5 5 metal 8 10 * * W vss
C vss 41 5 metal 8 11 * * E vss
C ci 14 45 metal2 2 12 * * N ci
I cdp1 8 30 0 "cdp" ly * *
I cdp2 8 35 0 "cdp" ly * *
I cdp3 32 35 0 "cdp" ly * *
I via12x1 32 18 0 "via12" ly * *
I via12x2 14 35 0 "via12" ly * *
I via12x3 8 13 0 "via12" ly * *
I cpf1 25 18 0 "cpf" ly * *
I cpf2 28 26 0 "cpf" ly * *
I cpf3 15 26 0 "cpf" ly * *
I cpf4 37 22 0 "cpf" ly * *
I cpf5 9 22 0 "cpf" ly * *
I cdn1 8 8 0 "cdn" ly * *
I cdn2 32 8 0 "cdn" ly * *
I cdn3 38 13 0 "cdn" ly * *
I cdn4 26 13 0 "cdn" ly * *
I cdn5 20 13 0 "cdn" ly * *
I cdp4 20 35 0 "cdp" ly * *
I cdp5 38 30 0 "cdp" ly * *
I cdp6 26 35 0 "cdp" ly * *
I cdp7 32 40 0 "cdp" ly * *
I cdp8 8 40 0 "cdp" ly * *
I via12x4 20 17 0 "via12" ly * *
I via12x5 20 31 0 "via12" ly * *
I cbn1 38 46 0 "cbn" ly * *
I cbn2 32 46 0 "cbn" ly * *
I cbn3 26 46 0 "cbn" ly * *
I cbn4 20 46 0 "cbn" ly * *
I cbn5 14 46 0 "cbn" ly * *
I cbn6 8 46 0 "cbn" ly * *
I cbp1 8 2 0 "cbp" ly * *
I cbp2 14 2 0 "cbp" ly * *
I cbp3 20 2 0 "cbp" ly * *
I cbp4 26 2 0 "cbp" ly * *
I cbp5 32 2 0 "cbp" ly * *
I cbp6 38 2 0 "cbp" ly * *
P 11 27 * ptrans
P 11 43 * ptrans
W 10 26 12 44
S 1 V ptrans P 1 P 2
P 17 27 * ptrans
P 17 43 * ptrans
W 16 26 18 44
S 1 V ptrans P 3 P 4
P 23 27 * ptrans
P 23 43 * ptrans
W 22 26 24 44
S 1 V ptrans P 5 P 6
P 29 27 * ptrans
P 29 43 * ptrans
W 28 26 30 44
S 1 V ptrans P 7 P 8
P 35 27 * ptrans
P 35 43 * ptrans
W 34 26 36 44
S 1 V ptrans P 9 P 10
P 26 30 * metal
P 26 35 * metal
W 25 29 27 36
S 1 V metal P 11 P 12
P 32 3 * metal2
P 32 45 * metal2
W 30 1 34 47
S 2 V metal2 P 13 P 14
P 20 3 * metal2
P 20 45 * metal2
W 18 1 22 47
S 2 V metal2 P 15 P 16
P 14 3 * metal2
P 14 45 * metal2
W 12 1 16 47
S 2 V metal2 P 17 P 18
P 8 3 * metal2
P 8 45 * metal2
W 6 1 10 47
S 2 V metal2 P 19 P 20
P 20 31 * metal
P 20 35 * metal
W 18 29 22 37
S 2 V metal P 21 P 22
P 20 13 * metal
P 20 17 * metal
W 18 11 22 19
S 2 V metal P 23 P 24
P 8 30 * metal
P 8 40 * metal
W 6 28 10 42
S 2 V metal P 25 P 26
P 32 35 * metal
P 32 40 * metal
W 30 33 34 42
S 2 V metal P 27 P 28
P 38 7 * ndif
P 38 14 * ndif
W 36 5 40 16
S 2 V ndif P 29 P 30
P 32 7 * ndif
P 32 14 * ndif
W 30 5 34 16
S 2 V ndif P 31 P 32
P 26 7 * ndif
P 26 14 * ndif
W 24 5 28 16
S 2 V ndif P 33 P 34
P 20 7 * ndif
P 20 14 * ndif
W 18 5 22 16
S 2 V ndif P 35 P 36
P 14 7 * ndif
P 14 14 * ndif
W 12 5 16 16
S 2 V ndif P 37 P 38
P 8 7 * ndif
P 8 14 * ndif
W 5 4 11 17
S 3 V ndif P 39 P 40
P 38 29 * pdif
P 38 41 * pdif
W 36 27 40 43
S 2 V pdif P 41 P 42
P 32 29 * pdif
P 32 41 * pdif
W 30 27 34 43
S 2 V pdif P 43 P 44
P 26 29 * pdif
P 26 41 * pdif
W 24 27 28 43
S 2 V pdif P 45 P 46
P 20 29 * pdif
P 20 41 * pdif
W 18 27 22 43
S 2 V pdif P 47 P 48
P 14 29 * pdif
P 14 41 * pdif
W 12 27 16 43
S 2 V pdif P 49 P 50
P 8 29 * pdif
P 8 41 * pdif
W 5 26 11 44
S 3 V pdif P 51 P 52
P 25 18 * metal
P 32 18 * metal
W 24 17 33 19
S 1 H metal P 53 P 54
P 35 16 * poly
P 35 27 * poly
W 34 15 36 28
S 1 V poly P 55 P 56
P 29 16 * poly
P 29 27 * poly
W 28 15 30 28
S 1 V poly P 57 P 58
P 23 16 * poly
P 23 27 * poly
W 22 15 24 28
S 1 V poly P 59 P 60
P 17 16 * poly
P 17 27 * poly
W 16 15 18 28
S 1 V poly P 61 P 62
P 11 16 * poly
P 11 27 * poly
W 10 15 12 28
S 1 V poly P 63 P 64
P 14 26 * metal
P 14 35 * metal
W 13 25 15 36
S 1 V metal P 65 P 66
P 14 26 * metal
P 28 26 * metal
W 13 25 29 27
S 1 H metal P 67 P 68
P 8 13 * metal
P 8 21 * metal
W 7 12 9 22
S 1 V metal P 69 P 70
P 8 22 * metal
P 37 22 * metal
W 7 21 38 23
S 1 H metal P 71 P 72
P 35 5 * ntrans
P 35 16 * ntrans
W 34 4 36 17
S 1 V ntrans P 73 P 74
P 29 5 * ntrans
P 29 16 * ntrans
W 28 4 30 17
S 1 V ntrans P 75 P 76
P 23 5 * ntrans
P 23 16 * ntrans
W 22 4 24 17
S 1 V ntrans P 77 P 78
P 17 5 * ntrans
P 17 16 * ntrans
W 16 4 18 17
S 1 V ntrans P 79 P 80
P 11 5 * ntrans
P 11 16 * ntrans
W 10 4 12 17
S 1 V ntrans P 81 P 82
P 26 30 * metal
P 38 30 * metal
W 25 29 39 31
S 1 H metal P 83 P 84
P 26 13 * metal
P 38 13 * metal
W 25 12 39 14
S 1 H metal P 85 P 86
P 5 39 * nwell
P 41 39 * nwell
W -21 13 67 65
S 26 H nwell P 87 P 88
P 38 3 * allowM2
P 38 45 * allowM2
W 36 1 40 47
S 2 V allowM2 P 89 P 90
P 26 3 * allowM2
P 26 45 * allowM2
W 24 1 28 47
S 2 V allowM2 P 91 P 92
P 5 43 * metal
P 41 43 * metal
W -3 35 49 51
S 8 H metal P 93 P 94
P 5 5 * metal
P 41 5 * metal
W -3 -3 49 13
S 8 H metal P 95 P 96
P 37 22 * poly
P 35 22 * poly
W 32 19 40 25
S 3 H poly P 97 P 98
P 26 18 * poly
P 23 18 * poly
W 20 15 29 21
S 3 H poly P 99 P 100
P 14 26 * poly
P 17 26 * poly
W 11 23 20 29
S 3 H poly P 101 P 102
P 8 22 * poly
P 11 22 * poly
W 5 19 14 25
S 3 H poly P 103 P 104
E
