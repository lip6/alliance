#cell1 amoutitop1_c idps_symb compose *
# 21-Feb-95 12:26 21-Feb-95 12:26 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 35 65
B 2 5 35 65
F F
C vss 2 5 metal2 2 1 * * W vss
C vdd 2 65 metal2 2 2 * * W vdd
C mnext 2 38 metal2 2 3 * * W mnext
C mnext 14 65 metal 1 4 * * N mnext
C vss 35 5 metal2 8 5 * * E vss
C vdd 35 65 metal2 6 6 * * E vdd
I via1 14 38 0 "cvia" ly * *
I via2 19 65 0 "cvia" ly * *
I cbn1 19 60 0 "cbn" ly * *
I cbn2 19 56 0 "cbn" ly * *
I cbn3 19 52 0 "cbn" ly * *
I cbn4 19 48 0 "cbn" ly * *
I cbn5 19 44 0 "cbn" ly * *
I cbn6 23 60 0 "cbn" ly * *
I cbn7 27 60 0 "cbn" ly * *
I cbn8 31 60 0 "cbn" ly * *
I cbn9 23 52 0 "cbn" ly * *
I cbn10 27 52 0 "cbn" ly * *
I cbn11 31 52 0 "cbn" ly * *
I cbn12 23 44 0 "cbn" ly * *
I cbn13 27 44 0 "cbn" ly * *
I cbn14 31 44 0 "cbn" ly * *
I via3 19 5 0 "cvia" ly * *
I cbp1 19 11 0 "cbp" ly * *
I cbp2 19 15 0 "cbp" ly * *
I cbp3 19 19 0 "cbp" ly * *
I cbp4 19 23 0 "cbp" ly * *
I cbp5 19 27 0 "cbp" ly * *
I cbp6 15 11 0 "cbp" ly * *
I cbp7 11 11 0 "cbp" ly * *
I cbp8 7 11 0 "cbp" ly * *
I cbp9 23 11 0 "cbp" ly * *
I cbp10 27 11 0 "cbp" ly * *
I cbp11 31 11 0 "cbp" ly * *
I cbp12 15 19 0 "cbp" ly * *
I cbp13 11 19 0 "cbp" ly * *
I cbp14 7 19 0 "cbp" ly * *
I cbp15 23 19 0 "cbp" ly * *
I cbp16 27 19 0 "cbp" ly * *
I cbp17 31 19 0 "cbp" ly * *
I cbp18 23 27 0 "cbp" ly * *
I cbp19 27 27 0 "cbp" ly * *
I cbp20 31 27 0 "cbp" ly * *
I cbp21 15 27 0 "cbp" ly * *
I cbp22 11 27 0 "cbp" ly * *
I cbp23 7 27 0 "cbp" ly * *
I mnext_in 14 38 0 "ref_con" ly * *
P 2 5 * metal2
P 35 5 * metal2
W -6 -3 43 13
S 8 H metal2 P 1 P 2
P 2 65 * metal2
P 35 65 * metal2
W -4 59 41 71
S 6 H metal2 P 3 P 4
P 2 52 * nwell
P 35 52 * nwell
W -24 26 61 78
S 26 H nwell P 5 P 6
P 2 38 * metal2
P 14 38 * metal2
W 0 36 16 40
S 2 H metal2 P 7 P 8
P 14 38 * metal
P 14 65 * metal
W 13 37 15 66
S 1 V metal P 9 P 10
P 19 44 * metal
P 19 65 * metal
W 17 42 21 67
S 2 V metal P 11 P 12
P 19 60 * metal
P 31 60 * metal
W 17 58 33 62
S 2 H metal P 13 P 14
P 19 52 * metal
P 31 52 * metal
W 17 50 33 54
S 2 H metal P 15 P 16
P 19 44 * metal
P 31 44 * metal
W 17 42 33 46
S 2 H metal P 17 P 18
P 19 60 * ntie
P 31 60 * ntie
W 16 57 34 63
S 3 H ntie P 19 P 20
P 19 52 * ntie
P 31 52 * ntie
W 16 49 34 55
S 3 H ntie P 21 P 22
P 19 44 * ntie
P 31 44 * ntie
W 16 41 34 47
S 3 H ntie P 23 P 24
P 19 44 * ntie
P 19 60 * ntie
W 16 41 22 63
S 3 V ntie P 25 P 26
P 19 5 * metal
P 19 27 * metal
W 17 3 21 29
S 2 V metal P 27 P 28
P 7 27 * metal
P 31 27 * metal
W 5 25 33 29
S 2 H metal P 29 P 30
P 7 19 * metal
P 31 19 * metal
W 5 17 33 21
S 2 H metal P 31 P 32
P 7 11 * metal
P 31 11 * metal
W 5 9 33 13
S 2 H metal P 33 P 34
P 7 27 * ptie
P 31 27 * ptie
W 4 24 34 30
S 3 H ptie P 35 P 36
P 19 11 * ptie
P 19 27 * ptie
W 16 8 22 30
S 3 V ptie P 37 P 38
P 7 11 * ptie
P 31 11 * ptie
W 4 8 34 14
S 3 H ptie P 39 P 40
P 7 19 * ptie
P 31 19 * ptie
W 4 16 34 22
S 3 H ptie P 41 P 42
E
