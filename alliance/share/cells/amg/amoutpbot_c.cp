#cell1 amoutpbot_c idps_symb compose *
# 21-Feb-95 12:26 21-Feb-95 12:26 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 35 65
B 2 5 35 65
F F
C vss 2 5 metal2 2 1 * * W vss
C vdd 2 65 metal2 2 2 * * W vdd
C msb 23 65 metal 1 3 * * N msb
C mnext 2 38 metal2 2 4 * * W mnext
C mnext 5 65 metal 1 5 * * N mnext
C vss 35 5 metal2 8 6 * * E vss
C vdd 35 65 metal2 6 7 * * E vdd
I via1 5 38 0 "cvia" ly * *
I via2 19 65 0 "cvia" ly * *
I cbn1 19 60 0 "cbn" ly * *
I cbn2 19 56 0 "cbn" ly * *
I cbn3 19 52 0 "cbn" ly * *
I cbn4 19 48 0 "cbn" ly * *
I cbn5 19 44 0 "cbn" ly * *
I cbn6 15 44 0 "cbn" ly * *
I cbn7 11 44 0 "cbn" ly * *
I cbn8 15 52 0 "cbn" ly * *
I cbn9 11 52 0 "cbn" ly * *
I via3 19 5 0 "cvia" ly * *
I cbp1 19 11 0 "cbp" ly * *
I cbp2 19 15 0 "cbp" ly * *
I cbp3 19 19 0 "cbp" ly * *
I cbp4 19 23 0 "cbp" ly * *
I cbp5 19 27 0 "cbp" ly * *
I cbp6 15 27 0 "cbp" ly * *
I cbp7 11 27 0 "cbp" ly * *
I cbp8 7 27 0 "cbp" ly * *
I cbp9 15 19 0 "cbp" ly * *
I cbp10 11 19 0 "cbp" ly * *
I cbp11 7 19 0 "cbp" ly * *
I cbp12 15 11 0 "cbp" ly * *
I cbp13 11 11 0 "cbp" ly * *
I cbp14 7 11 0 "cbp" ly * *
I via4 27 58 0 "cvia" ly * *
I lsb_0 32 13 0 "ref_con" ly * *
I mnext_in 5 38 0 "ref_con" ly * *
I lsb_1 32 18 0 "ref_con" ly * *
I lsb_2 32 23 0 "ref_con" ly * *
I lsb_3 32 28 0 "ref_con" ly * *
I lsb_4 32 33 0 "ref_con" ly * *
I lsb_6 32 43 0 "ref_con" ly * *
I lsb_7 32 48 0 "ref_con" ly * *
I lsb_8 32 53 0 "ref_con" ly * *
I lsb_9 32 58 0 "ref_con" ly * *
I msb_9 27 58 0 "ref_con" ly * *
I msb_8 27 53 0 "ref_con" ly * *
I msb_7 27 48 0 "ref_con" ly * *
I msb_6 27 43 0 "ref_con" ly * *
I msb_4 27 33 0 "ref_con" ly * *
I msb_3 27 28 0 "ref_con" ly * *
I msb_2 27 23 0 "ref_con" ly * *
I msb_1 27 18 0 "ref_con" ly * *
I lsb_5 32 38 0 "ref_con" ly * *
I msb_5 27 38 0 "ref_con" ly * *
I lsb 35 13 0 "ref_ref" ly * *
I msb 35 38 0 "ref_ref" ly * *
P 27 58 * allowM2
P 35 58 * allowM2
W 25 56 37 60 p9
S 2 H allowM2 P 1 P 2
P 27 53 * allowM2
P 35 53 * allowM2
W 25 51 37 55 p8
S 2 H allowM2 P 3 P 4
P 27 48 * allowM2
P 35 48 * allowM2
W 25 46 37 50 p7
S 2 H allowM2 P 5 P 6
P 27 43 * allowM2
P 35 43 * allowM2
W 25 41 37 45 p6
S 2 H allowM2 P 7 P 8
P 27 38 * allowM2
P 35 38 * allowM2
W 25 36 37 40 p5
S 2 H allowM2 P 9 P 10
P 27 33 * allowM2
P 35 33 * allowM2
W 25 31 37 35 p4
S 2 H allowM2 P 11 P 12
P 27 28 * allowM2
P 35 28 * allowM2
W 25 26 37 30 p3
S 2 H allowM2 P 13 P 14
P 27 23 * allowM2
P 35 23 * allowM2
W 25 21 37 25 p2
S 2 H allowM2 P 15 P 16
P 27 18 * allowM2
P 35 18 * allowM2
W 25 16 37 20 p1
S 2 H allowM2 P 17 P 18
P 2 5 * metal2
P 35 5 * metal2
W -6 -3 43 13
S 8 H metal2 P 19 P 20
P 2 65 * metal2
P 35 65 * metal2
W -4 59 41 71
S 6 H metal2 P 21 P 22
P 5 38 * metal
P 5 65 * metal
W 4 37 6 66
S 1 V metal P 23 P 24
P 23 58 * metal
P 23 65 * metal
W 22 57 24 66
S 1 V metal P 25 P 26
P 2 52 * nwell
P 35 52 * nwell
W -24 26 61 78
S 26 H nwell P 27 P 28
P 2 38 * metal2
P 5 38 * metal2
W 0 36 7 40
S 2 H metal2 P 29 P 30
P 23 58 * metal
P 27 58 * metal
W 22 57 28 59
S 1 H metal P 31 P 32
P 27 18 * metal
P 27 58 * metal
W 26 17 28 59
S 1 V metal P 33 P 34
P 32 13 * metal
P 32 58 * metal
W 31 12 33 59
S 1 V metal P 35 P 36
P 19 44 * metal
P 19 65 * metal
W 17 42 21 67
S 2 V metal P 37 P 38
P 11 52 * metal
P 19 52 * metal
W 9 50 21 54
S 2 H metal P 39 P 40
P 11 44 * metal
P 19 44 * metal
W 9 42 21 46
S 2 H metal P 41 P 42
P 19 45 * ntie
P 19 60 * ntie
W 16 42 22 63
S 3 V ntie P 43 P 44
P 11 52 * ntie
P 19 52 * ntie
W 8 49 22 55
S 3 H ntie P 45 P 46
P 11 44 * ntie
P 19 44 * ntie
W 8 41 22 47
S 3 H ntie P 47 P 48
P 19 5 * metal
P 19 27 * metal
W 17 3 21 29
S 2 V metal P 49 P 50
P 7 27 * metal
P 19 27 * metal
W 5 25 21 29
S 2 H metal P 51 P 52
P 7 19 * metal
P 19 19 * metal
W 5 17 21 21
S 2 H metal P 53 P 54
P 7 11 * metal
P 19 11 * metal
W 5 9 21 13
S 2 H metal P 55 P 56
P 7 27 * ptie
P 19 27 * ptie
W 4 24 22 30
S 3 H ptie P 57 P 58
P 19 11 * ptie
P 19 27 * ptie
W 16 8 22 30
S 3 V ptie P 59 P 60
P 7 11 * ptie
P 19 11 * ptie
W 4 8 22 14
S 3 H ptie P 61 P 62
P 7 19 * ptie
P 19 19 * ptie
W 4 16 22 22
S 3 H ptie P 63 P 64
P 32 13 * allowM2
P 35 13 * allowM2
W 30 11 37 15 p0
S 2 H allowM2 P 65 P 66
E
