#cell1 amcoin_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 79 65
B 2 5 79 65
F F
C vss 2 5 metal2 2 1 * * W vss
C vss 79 5 metal2 2 2 * * E vss
C vdd 2 65 metal2 2 3 * * W vdd
C vdd 79 65 metal2 2 4 * * E vdd
C d0 11 65 metal 1 5 * * N d0
C vss 7 65 poly 1 6 * * N vss
C d0 2 18 metal2 2 7 * * W d0
C d0bar 2 13 metal2 2 8 * * W d0bar
C d0bar 22 65 metal 1 9 * * N d0bar
C x0 79 23 metal2 2 10 * * E x0
C y1 79 48 metal2 2 11 * * E y1
C y1 33 65 metal 1 12 * * N y1
C x0 53 65 metal 1 13 * * N x0
I via1 11 18 0 "cvia" ly * *
I via2 22 13 0 "cvia" ly * *
I via3 53 23 0 "cvia" ly * *
I via4 33 48 0 "cvia" ly * *
I via5 7 5 0 "cvia" ly * *
I cpf1 7 23 0 "cpf" ly * *
I via6 53 5 0 "cvia" ly * *
I cbp1 53 11 0 "cbp" ly * *
I cbp2 53 15 0 "cbp" ly * *
I cbp3 57 15 0 "cbp" ly * *
I cbp4 61 15 0 "cbp" ly * *
I cbp5 65 15 0 "cbp" ly * *
I cbp6 49 15 0 "cbp" ly * *
I cbp7 45 15 0 "cbp" ly * *
I cbp8 41 15 0 "cbp" ly * *
I cbp9 37 15 0 "cbp" ly * *
I cbp10 33 15 0 "cbp" ly * *
I via7 57 65 0 "cvia" ly * *
I cbn1 61 56 0 "cbn" ly * *
I cbn2 65 56 0 "cbn" ly * *
I cbn3 69 56 0 "cbn" ly * *
I cbn4 73 56 0 "cbn" ly * *
I cbn5 65 51 0 "cbn" ly * *
I cbn6 65 47 0 "cbn" ly * *
I cbn7 65 43 0 "cbn" ly * *
I cbp11 33 19 0 "cbp" ly * *
I cbp12 33 24 0 "cbp" ly * *
I cbp13 45 19 0 "cbp" ly * *
I cbp14 45 24 0 "cbp" ly * *
I cbp15 33 29 0 "cbp" ly * *
I cbp16 45 29 0 "cbp" ly * *
I via8 28 65 0 "cvia" ly * *
I cbn8 28 59 0 "cbn" ly * *
I cbn9 28 54 0 "cbn" ly * *
I cbn10 28 49 0 "cbn" ly * *
I cbn11 28 44 0 "cbn" ly * *
P 2 65 * metal2
P 79 65 * metal2
W -4 59 85 71
S 6 H metal2 P 1 P 2
P 2 5 * metal2
P 79 5 * metal2
W -6 -3 87 13
S 8 H metal2 P 3 P 4
P 2 13 * metal2
P 22 13 * metal2
W 0 11 24 15
S 2 H metal2 P 5 P 6
P 2 18 * metal2
P 11 18 * metal2
W 0 16 13 20
S 2 H metal2 P 7 P 8
P 53 23 * metal2
P 79 23 * metal2
W 51 21 81 25
S 2 H metal2 P 9 P 10
P 11 18 * metal
P 11 65 * metal
W 10 17 12 66
S 1 V metal P 11 P 12
P 22 13 * metal
P 22 65 * metal
W 21 12 23 66
S 1 V metal P 13 P 14
P 33 48 * metal
P 33 65 * metal
W 32 47 34 66
S 1 V metal P 15 P 16
P 33 48 * metal2
P 79 48 * metal2
W 31 46 81 50
S 2 H metal2 P 17 P 18
P 2 52 * nwell
P 79 52 * nwell
W -24 26 105 78
S 26 H nwell P 19 P 20
P 7 5 * metal
P 7 23 * metal
W 6 4 8 24
S 1 V metal P 21 P 22
P 7 23 * poly
P 7 65 * poly
W 6 22 8 66
S 1 V poly P 23 P 24
P 33 15 * metal
P 65 15 * metal
W 31 13 67 17
S 2 H metal P 25 P 26
P 33 15 * ptie
P 65 15 * ptie
W 30 12 68 18
S 3 H ptie P 27 P 28
P 57 56 * metal
P 57 65 * metal
W 56 55 58 66
S 1 V metal P 29 P 30
P 57 56 * metal
P 73 56 * metal
W 55 54 75 58
S 2 H metal P 31 P 32
P 53 23 * metal
P 53 65 * metal
W 52 22 54 66
S 1 V metal P 33 P 34
P 53 5 * metal
P 53 15 * metal
W 51 3 55 17
S 2 V metal P 35 P 36
P 53 11 * ptie
P 53 15 * ptie
W 50 8 56 18
S 3 V ptie P 37 P 38
P 61 56 * ntie
P 73 56 * ntie
W 58 53 76 59
S 3 H ntie P 39 P 40
P 65 43 * metal
P 65 56 * metal
W 63 41 67 58
S 2 V metal P 41 P 42
P 65 43 * ntie
P 65 56 * ntie
W 62 40 68 59
S 3 V ntie P 43 P 44
P 45 15 * metal
P 45 29 * metal
W 43 13 47 31
S 2 V metal P 45 P 46
P 33 15 * metal
P 33 29 * metal
W 31 13 35 31
S 2 V metal P 47 P 48
P 33 15 * ptie
P 33 29 * ptie
W 30 12 36 32
S 3 V ptie P 49 P 50
P 45 15 * ptie
P 45 29 * ptie
W 42 12 48 32
S 3 V ptie P 51 P 52
P 28 44 * metal
P 28 65 * metal
W 26 42 30 67
S 2 V metal P 53 P 54
P 28 44 * ntie
P 28 59 * ntie
W 25 41 31 62
S 3 V ntie P 55 P 56
E
