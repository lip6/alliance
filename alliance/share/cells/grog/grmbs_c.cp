#cell1 grmbs_c idps_symb compose *
# 27-Mar-95 11:33 27-Mar-95 11:33 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 28 97
B 0 0 28 97
F F
C vss 0 97 metal 1 1 * * W vss
C vss 28 88 metal2 4 2 * * E vss
C e6 28 82 metal2 2 3 * * E e6
C vdd 28 76 metal2 4 4 * * E vdd
C e7 0 70 metal2 2 5 * * W e7
C e7 28 70 metal2 2 6 * * E e7
C e1 28 55 metal2 2 7 * * E e1
C vdd 28 44 metal2 4 8 * * E vdd
C e9 28 33 metal2 2 9 * * E e9
C e10 0 28 metal2 2 10 * * W e10
C e11 0 23 metal2 2 11 * * W e11
C e11 28 23 metal2 2 12 * * E e11
C e3 0 18 metal2 2 13 * * W e3
C e3 28 18 metal2 2 14 * * E e3
C e2 0 13 metal2 2 15 * * W e2
C e10 28 13 metal2 2 16 * * E e10
C e12 0 8 metal2 2 17 * * W e12
C e4 0 50 metal2 2 18 * * W e4
C e4 28 50 metal2 2 19 * * E e4
C e5 0 65 metal2 2 20 * * W e5
C e5 28 65 metal2 2 21 * * E e5
C e8 0 60 metal2 2 22 * * W e8
C e8 28 60 metal2 2 23 * * E e8
C e9 0 33 metal2 2 24 * * W e9
C e12 28 3 metal2 2 25 * * E e12
C e2 28 28 metal2 2 26 * * E e2
C vdd 0 44 metal2 4 27 * * W vdd
C vss 0 88 metal2 4 28 * * W vss
C e6 0 82 metal2 2 29 * * W e6
C vdd 0 76 metal2 4 30 * * W vdd
C e1 0 55 metal2 2 31 * * W e1
I via1 7 88 0 "cvia" ly * *
I via2 12 28 0 "cvia" ly * *
I via3 7 28 0 "cvia" ly * *
I via4 18 13 0 "cvia" ly * *
I via5 12 13 0 "cvia" ly * *
I via6 14 44 0 "cvia" ly * *
I via7 14 76 0 "cvia" ly * *
I cbp1 7 84 0 "cbp" ly * *
I cbp2 7 79 0 "cbp" ly * *
I cbp3 7 74 0 "cbp" ly * *
I cbp4 7 69 0 "cbp" ly * *
I cbp5 12 84 0 "cbp" ly * *
I cbp6 17 84 0 "cbp" ly * *
I cbp7 22 84 0 "cbp" ly * *
I via8 20 44 0 "cvia" ly * *
I cbn1 20 40 0 "cbn" ly * *
I cbn2 20 35 0 "cbn" ly * *
I cbn3 20 30 0 "cbn" ly * *
I cbn4 20 25 0 "cbn" ly * *
I cbn5 20 20 0 "cbn" ly * *
I cbn6 20 48 0 "cbn" ly * *
I cbn7 20 53 0 "cbn" ly * *
P 20 20 * metal
P 20 53 * metal
W 18 18 22 55
S 2 V metal P 1 P 2
P 20 20 * ntie
P 20 54 * ntie
W 17 17 23 57
S 3 V ntie P 3 P 4
P 7 84 * metal
P 22 84 * metal
W 5 82 24 86
S 2 H metal P 5 P 6
P 7 69 * metal
P 7 88 * metal
W 5 67 9 90
S 2 V metal P 7 P 8
P 7 88 * metal
P 7 96 * metal
W 6 87 8 97
S 1 V metal P 9 P 10
P 0 88 * metal2
P 28 88 * metal2
W -4 84 32 92 vss
S 4 H metal2 P 11 P 12
P 7 69 * ptie
P 7 84 * ptie
W 4 66 10 87
S 3 V ptie P 13 P 14
P 7 84 * ptie
P 22 84 * ptie
W 4 81 25 87
S 3 H ptie P 15 P 16
P 0 65 * metal2
P 28 65 * metal2
W -2 63 30 67 e5
S 2 H metal2 P 17 P 18
P 14 44 * metal
P 14 76 * metal
W 13 43 15 77
S 1 V metal P 19 P 20
P 18 9 * metal
P 18 13 * metal
W 17 8 19 14
S 1 V metal P 21 P 22
P 7 9 * metal
P 7 28 * metal
W 6 8 8 29
S 1 V metal P 23 P 24
P 7 9 * metal
P 18 9 * metal
W 6 8 19 10
S 1 H metal P 25 P 26
P 12 13 * metal
P 12 28 * metal
W 11 12 13 29
S 1 V metal P 27 P 28
P 0 13 * metal2
P 12 13 * metal2
W -2 11 14 15 e2
S 2 H metal2 P 29 P 30
P 12 28 * metal2
P 28 28 * metal2
W 10 26 30 30 e2
S 2 H metal2 P 31 P 32
P 0 28 * metal2
P 7 28 * metal2
W -2 26 9 30 e10
S 2 H metal2 P 33 P 34
P 18 13 * metal2
P 28 13 * metal2
W 16 11 30 15 e10
S 2 H metal2 P 35 P 36
P 17 15 * nwell
P 17 56 * nwell
W -1 -3 35 74
S 18 V nwell P 37 P 38
P 4 15 * nwell
P 4 28 * nwell
W -4 7 12 36
S 8 V nwell P 39 P 40
P 4 51 * nwell
P 4 56 * nwell
W -4 43 12 64
S 8 V nwell P 41 P 42
P 0 8 * metal2
P 4 8 * metal2
W -2 6 6 10 e12
S 2 H metal2 P 43 P 44
P 4 3 * metal2
P 4 8 * metal2
W 2 1 6 10 e12
S 2 V metal2 P 45 P 46
P 4 3 * metal2
P 28 3 * metal2
W 2 1 30 5 e12
S 2 H metal2 P 47 P 48
P 0 60 * metal2
P 28 60 * metal2
W -2 58 30 62 e8
S 2 H metal2 P 49 P 50
P 0 76 * metal2
P 28 76 * metal2
W -4 72 32 80 vdd
S 4 H metal2 P 51 P 52
P 0 82 * metal2
P 28 82 * metal2
W -2 80 30 84 e6
S 2 H metal2 P 53 P 54
P 0 55 * metal2
P 28 55 * metal2
W -2 53 30 57 e1
S 2 H metal2 P 55 P 56
P 0 44 * metal2
P 28 44 * metal2
W -4 40 32 48 vdd
S 4 H metal2 P 57 P 58
P 0 33 * metal2
P 28 33 * metal2
W -2 31 30 35 e9
S 2 H metal2 P 59 P 60
P 0 50 * metal2
P 28 50 * metal2
W -2 48 30 52 e4
S 2 H metal2 P 61 P 62
P 0 18 * metal2
P 28 18 * metal2
W -2 16 30 20 e3
S 2 H metal2 P 63 P 64
P 0 23 * metal2
P 28 23 * metal2
W -2 21 30 25 e11
S 2 H metal2 P 65 P 66
P 0 70 * metal2
P 28 70 * metal2
W -2 68 30 72 e7
S 2 H metal2 P 67 P 68
P 0 96 * metal
P 0 97 * metal
W -1 95 1 98
S 1 V metal P 69 P 70
P 0 96 * metal
P 7 96 * metal
W -1 95 8 97
S 1 H metal P 71 P 72
E
