#cell1 grrbs1_c idps_symb compose *
# 27-Mar-95 11:35 27-Mar-95 11:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 28 97
B 0 0 28 97
F F
C e13 28 3 metal2 2 1 * * E e13
C n1 0 97 metal 1 2 * * W n1
C e1 28 94 metal2 2 3 * * E e1
C e10 0 33 metal2 2 4 * * W e10
C e9 28 60 metal2 2 5 * * E e9
C e9 0 60 metal2 2 6 * * W e9
C e6 28 65 metal2 2 7 * * E e6
C e6 0 65 metal2 2 8 * * W e6
C e5 28 50 metal2 2 9 * * E e5
C e5 0 50 metal2 2 10 * * W e5
C e13 0 8 metal2 2 11 * * W e13
C e11 28 13 metal2 2 12 * * E e11
C e3 0 13 metal2 2 13 * * W e3
C e4 28 18 metal2 2 14 * * E e4
C e4 0 18 metal2 2 15 * * W e4
C e12 28 23 metal2 2 16 * * E e12
C e12 0 23 metal2 2 17 * * W e12
C e3 28 28 metal2 2 18 * * E e3
C e11 0 28 metal2 2 19 * * W e11
C e10 28 33 metal2 2 20 * * E e10
C vdd 28 44 metal2 4 21 * * E vdd
C e2 28 55 metal2 2 22 * * E e2
C e8 28 70 metal2 2 23 * * E e8
C e8 0 70 metal2 2 24 * * W e8
C vdd 28 76 metal2 4 25 * * E vdd
C e7 28 82 metal2 2 26 * * E e7
C vss 28 88 metal2 4 27 * * E vss
C vss 0 88 metal2 4 28 * * W vss
C e7 0 82 metal2 2 29 * * W e7
C vdd 0 76 metal2 4 30 * * W vdd
C e2 0 55 metal2 2 31 * * W e2
C vdd 0 44 metal2 4 32 * * W vdd
I cbp1 6 84 0 "cbp" ly * *
I cbp2 6 80 0 "cbp" ly * *
I cbp3 16 80 0 "cbp" ly * *
I via1 8 28 0 "cvia" ly * *
I via2 14 13 0 "cvia" ly * *
I via3 20 13 0 "cvia" ly * *
I via4 14 28 0 "cvia" ly * *
I cdp1 10 48 0 "cdp" ly * *
I cdp2 10 54 0 "cdp" ly * *
I cdp3 16 48 0 "cdp" ly * *
I cdp4 16 54 0 "cdp" ly * *
I via5 24 44 0 "cvia" ly * *
I via6 6 87 0 "cvia" ly * *
I via7 20 94 0 "cvia" ly * *
I via8 16 43 0 "cvia" ly * *
I cdp5 10 43 0 "cdp" ly * *
I cdn1 10 70 0 "cdn" ly * *
I cdn2 10 65 0 "cdn" ly * *
I via9 16 88 0 "cvia" ly * *
I cdn3 16 65 0 "cdn" ly * *
I cdn4 16 70 0 "cdn" ly * *
I cpf1 14 60 0 "cpf" ly * *
I cbp4 16 84 0 "cbp" ly * *
I cbp5 16 76 0 "cbp" ly * *
I cbp6 6 76 0 "cbp" ly * *
I via10 24 76 0 "cvia" ly * *
I cbn1 24 53 0 "cbn" ly * *
I cbn2 24 37 0 "cbn" ly * *
I cbn3 17 37 0 "cbn" ly * *
I cbn4 10 37 0 "cbn" ly * *
I cbp7 2 87 0 "cbp" ly * *
P 6 76 * ptie
P 16 76 * ptie
W 3 73 19 79
S 3 H ptie P 1 P 2
P 24 37 * ntie
P 24 54 * ntie
W 21 34 27 57
S 3 V ntie P 3 P 4
P 10 37 * ntie
P 24 37 * ntie
W 7 34 27 40
S 3 H ntie P 5 P 6
P 20 60 * metal
P 20 94 * metal
W 19 59 21 95
S 1 V metal P 7 P 8
P 14 60 * metal
P 20 60 * metal
W 13 59 21 61
S 1 H metal P 9 P 10
P 10 43 * metal
P 10 96 * metal
W 9 42 11 97
S 1 V metal P 11 P 12
P 0 96 * metal
P 10 96 * metal
W -1 95 11 97
S 1 H metal P 13 P 14
P 16 65 * metal
P 16 88 * metal
W 14 63 18 90
S 2 V metal P 15 P 16
P 16 42 * pdif
P 16 54 * pdif
W 13 39 19 57
S 3 V pdif P 17 P 18
P 10 64 * ndif
P 10 71 * ndif
W 7 61 13 74
S 3 V ndif P 19 P 20
P 16 64 * ndif
P 16 71 * ndif
W 13 61 19 74
S 3 V ndif P 21 P 22
P 20 9 * metal
P 20 13 * metal
W 19 8 21 14
S 1 V metal P 23 P 24
P 8 9 * metal
P 8 28 * metal
W 7 8 9 29
S 1 V metal P 25 P 26
P 8 9 * metal
P 20 9 * metal
W 7 8 21 10
S 1 H metal P 27 P 28
P 14 13 * metal
P 14 28 * metal
W 13 12 15 29
S 1 V metal P 29 P 30
P 17 15 * nwell
P 17 56 * nwell
W -1 -3 35 74
S 18 V nwell P 31 P 32
P 0 44 * metal2
P 28 44 * metal2
W -4 40 32 48
S 4 H metal2 P 33 P 34
P 0 23 * metal2
P 28 23 * metal2
W -2 21 30 25
S 2 H metal2 P 35 P 36
P 0 18 * metal2
P 28 18 * metal2
W -2 16 30 20
S 2 H metal2 P 37 P 38
P 0 50 * metal2
P 28 50 * metal2
W -2 48 30 52
S 2 H metal2 P 39 P 40
P 0 33 * metal2
P 28 33 * metal2
W -2 31 30 35
S 2 H metal2 P 41 P 42
P 13 40 * ptrans
P 13 57 * ptrans
W 12 39 14 58
S 1 V ptrans P 43 P 44
P 20 94 * metal2
P 28 94 * metal2
W 18 92 30 96
S 2 H metal2 P 45 P 46
P 13 62 * ntrans
P 13 73 * ntrans
W 12 61 14 74
S 1 V ntrans P 47 P 48
P 0 96 * metal
P 0 97 * metal
W -1 95 1 98
S 1 V metal P 49 P 50
P 4 3 * metal2
P 28 3 * metal2
W 2 1 30 5
S 2 H metal2 P 51 P 52
P 4 3 * metal2
P 4 8 * metal2
W 2 1 6 10
S 2 V metal2 P 53 P 54
P 0 8 * metal2
P 4 8 * metal2
W -2 6 6 10
S 2 H metal2 P 55 P 56
P 4 51 * nwell
P 4 56 * nwell
W -4 43 12 64
S 8 V nwell P 57 P 58
P 4 15 * nwell
P 4 28 * nwell
W -4 7 12 36
S 8 V nwell P 59 P 60
P 14 28 * metal2
P 28 28 * metal2
W 12 26 30 30
S 2 H metal2 P 61 P 62
P 20 13 * metal2
P 28 13 * metal2
W 18 11 30 15
S 2 H metal2 P 63 P 64
P 0 28 * metal2
P 8 28 * metal2
W -2 26 10 30
S 2 H metal2 P 65 P 66
P 0 13 * metal2
P 14 13 * metal2
W -2 11 16 15
S 2 H metal2 P 67 P 68
P 10 42 * pdif
P 10 55 * pdif
W 7 39 13 58
S 3 V pdif P 69 P 70
P 16 43 * metal
P 16 53 * metal
W 14 41 18 55
S 2 V metal P 71 P 72
P 0 70 * metal2
P 28 70 * metal2
W -2 68 30 72
S 2 H metal2 P 73 P 74
P 0 65 * metal2
P 28 65 * metal2
W -2 63 30 67
S 2 H metal2 P 75 P 76
P 0 60 * metal2
P 28 60 * metal2
W -2 58 30 62
S 2 H metal2 P 77 P 78
P 0 55 * metal2
P 28 55 * metal2
W -2 53 30 57
S 2 H metal2 P 79 P 80
P 0 76 * metal2
P 28 76 * metal2
W -4 72 32 80
S 4 H metal2 P 81 P 82
P 0 82 * metal2
P 28 82 * metal2
W -2 80 30 84
S 2 H metal2 P 83 P 84
P 0 88 * metal2
P 28 88 * metal2
W -4 84 32 92
S 4 H metal2 P 85 P 86
P 2 87 * metal
P 6 87 * metal
W 0 85 8 89
S 2 H metal P 87 P 88
P 6 76 * metal
P 6 87 * metal
W 4 74 8 89
S 2 V metal P 89 P 90
P 16 83 * ptie
P 16 84 * ptie
W 13 80 19 87
S 3 V ptie P 91 P 92
P 16 76 * ptie
P 16 83 * ptie
W 13 73 19 86
S 3 V ptie P 93 P 94
P 6 83 * ptie
P 16 83 * ptie
W -5 72 27 94
S 11 H ptie P 95 P 96
P 6 76 * ptie
P 6 83 * ptie
W 3 73 9 86
S 3 V ptie P 97 P 98
P 6 83 * ptie
P 6 87 * ptie
W 3 80 9 90
S 3 V ptie P 99 P 100
P 2 87 * ptie
P 6 87 * ptie
W -1 84 9 90
S 3 H ptie P 101 P 102
P 24 37 * metal
P 24 76 * metal
W 22 35 26 78
S 2 V metal P 103 P 104
P 10 37 * metal
P 24 37 * metal
W 8 35 26 39
S 2 H metal P 105 P 106
P 13 60 * poly
P 13 62 * poly
W 12 59 14 63
S 1 V poly P 107 P 108
P 13 57 * poly
P 13 60 * poly
W 12 56 14 61
S 1 V poly P 109 P 110
P 13 60 * poly
P 14 60 * poly
W 12 59 15 61
S 1 H poly P 111 P 112
E
