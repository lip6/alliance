#cell1 n1xr2x_fp idps_symb compose *
# 20-Nov-97 12:10 20-Nov-97 12:10 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 49 65
B 5 5 49 65
F F
C vss 5 5 metal2 8 1 * * W vss
C vdd 5 65 metal2 6 2 * * W vdd
C vdd 49 65 metal2 6 3 * * E vdd
C vss 49 5 metal2 8 4 * * E vss
C i1 24 65 metal 1 5 * * N i1
C i1 24 5 metal 1 6 * * S i1
I via1 34 65 0 "cvia" ly * *
I via2 40 65 0 "cvia" ly * *
I via3 28 65 0 "cvia" ly * *
I via4 28 5 0 "cvia" ly * *
I via5 34 5 0 "cvia" ly * *
I via6 40 5 0 "cvia" ly * *
I cpf1 35 23 0 "cpf" ly * *
I cbp1 37 5 0 "cbp" ly * *
I cbp2 31 5 0 "cbp" ly * *
I cbn1 37 65 0 "cbn" ly * *
I cbn2 31 65 0 "cbn" ly * *
I cdp1 40 53 0 "cdp" ly * *
I cdp2 34 58 0 "cdp" ly * *
I cdp3 34 53 0 "cdp" ly * *
I cdn1 40 13 0 "cdn" ly * *
I cdn2 34 13 0 "cdn" ly * *
I cbn3 43 65 0 "cbn" ly * *
I via7 46 65 0 "cvia" ly * *
I via8 46 5 0 "cvia" ly * *
I cbp3 43 5 0 "cbp" ly * *
I via9 20 5 0 "cvia" ly * *
I via10 14 5 0 "cvia" ly * *
I via11 8 5 0 "cvia" ly * *
I via12 8 65 0 "cvia" ly * *
I via13 14 65 0 "cvia" ly * *
I via14 20 65 0 "cvia" ly * *
I cbn4 17 65 0 "cbn" ly * *
I cbn5 11 65 0 "cbn" ly * *
I cbp4 17 5 0 "cbp" ly * *
I cbp5 11 5 0 "cbp" ly * *
I cbp6 8 11 0 "cbp" ly * *
I cbp7 8 17 0 "cbp" ly * *
I cbp8 8 23 0 "cbp" ly * *
I cbp9 8 29 0 "cbp" ly * *
I cbn6 8 59 0 "cbn" ly * *
I cbn7 8 53 0 "cbn" ly * *
I cbn8 8 47 0 "cbn" ly * *
I cbn9 8 41 0 "cbn" ly * *
I i0_2 29 48 0 "ref_con" ly * *
I i0_3 29 43 0 "ref_con" ly * *
I i0_6 29 28 0 "ref_con" ly * *
I i0_5 29 33 0 "ref_con" ly * *
I i0_4 29 38 0 "ref_con" ly * *
I i0_0 29 58 0 "ref_con" ly * *
I o_1 45 53 0 "ref_con" ly * *
I o_6 40 28 0 "ref_con" ly * *
I nwell_28 37 53 0 "ref_ref" ly * *
I o_0 40 58 0 "ref_con" ly * *
I o_2 40 48 0 "ref_con" ly * *
I o_4 40 38 0 "ref_con" ly * *
I o_3 40 43 0 "ref_con" ly * *
I i0_1 29 53 0 "ref_con" ly * *
I o_5 40 33 0 "ref_con" ly * *
I o_7 40 23 0 "ref_con" ly * *
I o_8 40 18 0 "ref_con" ly * *
I i0_7 29 23 0 "ref_con" ly * *
I i0_8 29 18 0 "ref_con" ly * *
I i0_9 29 13 0 "ref_con" ly * *
I o_9 45 13 0 "ref_con" ly * *
I nwell_28 48 53 0 "ref_ref" ly * *
P 5 5 * metal2
P 49 5 * metal2
W -3 -3 57 13
S 8 H metal2 P 1 P 2
P 5 13 * allowM2
P 49 13 * allowM2
W 3 11 51 15
S 2 H allowM2 P 3 P 4
P 5 18 * allowM2
P 49 18 * allowM2
W 3 16 51 20
S 2 H allowM2 P 5 P 6
P 5 23 * allowM2
P 49 23 * allowM2
W 3 21 51 25
S 2 H allowM2 P 7 P 8
P 5 28 * allowM2
P 49 28 * allowM2
W 3 26 51 30
S 2 H allowM2 P 9 P 10
P 5 33 * allowM2
P 49 33 * allowM2
W 3 31 51 35
S 2 H allowM2 P 11 P 12
P 5 38 * allowM2
P 49 38 * allowM2
W 3 36 51 40
S 2 H allowM2 P 13 P 14
P 5 43 * allowM2
P 49 43 * allowM2
W 3 41 51 45
S 2 H allowM2 P 15 P 16
P 5 48 * allowM2
P 49 48 * allowM2
W 3 46 51 50
S 2 H allowM2 P 17 P 18
P 5 53 * allowM2
P 49 53 * allowM2
W 3 51 51 55
S 2 H allowM2 P 19 P 20
P 5 58 * allowM2
P 49 58 * allowM2
W 3 56 51 60
S 2 H allowM2 P 21 P 22
P 5 65 * metal2
P 49 65 * metal2
W -1 59 55 71 vdd
S 6 H metal2 P 23 P 24
P 5 53 * nwell
P 49 53 * nwell
W -23 25 77 81 nwell
S 28 H nwell P 25 P 26
P 29 24 * metal
P 35 24 * metal
W 28 23 36 25 i0
S 1 H metal P 27 P 28
P 34 53 * metal
P 34 65 * metal
W 32 51 36 67
S 2 V metal P 29 P 30
P 40 53 * metal
P 45 53 * metal
W 39 52 46 54
S 1 H metal P 31 P 32
P 35 23 * metal
P 35 24 * metal
W 34 22 36 25
S 1 V metal P 33 P 34
P 40 13 * metal
P 40 58 * metal
W 39 12 41 59 o
S 1 V metal P 35 P 36
P 34 5 * metal
P 34 13 * metal
W 32 3 36 15
S 2 V metal P 37 P 38
P 29 13 * metal
P 29 58 * metal
W 28 12 30 59 i0
S 1 V metal P 39 P 40
P 40 13 * metal
P 45 13 * metal
W 39 12 46 14
S 1 H metal P 41 P 42
P 24 5 * metal
P 24 65 * metal
W 23 4 25 66 i1
S 1 V metal P 43 P 44
P 37 17 * poly
P 37 47 * poly
W 36 16 38 48
S 1 V poly P 45 P 46
P 35 23 * poly
P 37 23 * poly
W 32 20 40 26
S 3 H poly P 47 P 48
P 37 47 * ptrans
P 37 62 * ptrans
W 36 46 38 63
S 1 V ptrans P 49 P 50
P 34 49 * pdif
P 34 60 * pdif
W 31 46 37 63
S 3 V pdif P 51 P 52
P 40 49 * pdif
P 40 60 * pdif
W 37 46 43 63
S 3 V pdif P 53 P 54
P 37 8 * ntrans
P 37 17 * ntrans
W 36 7 38 18
S 1 V ntrans P 55 P 56
P 34 10 * ndif
P 34 15 * ndif
W 31 7 37 18
S 3 V ndif P 57 P 58
P 40 10 * ndif
P 40 15 * ndif
W 37 7 43 18
S 3 V ndif P 59 P 60
P 28 65 * metal
P 46 65 * metal
W 26 63 48 67
S 2 H metal P 61 P 62
P 28 5 * metal
P 46 5 * metal
W 26 3 48 7
S 2 H metal P 63 P 64
P 8 65 * metal
P 20 65 * metal
W 6 63 22 67
S 2 H metal P 65 P 66
P 8 65 * metal
P 8 41 * metal
W 6 39 10 67
S 2 V metal P 67 P 68
P 8 29 * metal
P 8 5 * metal
W 6 3 10 31
S 2 V metal P 69 P 70
P 8 5 * metal
P 20 5 * metal
W 6 3 22 7
S 2 H metal P 71 P 72
P 8 30 * ptie
P 8 5 * ptie
W 5 2 11 33
S 3 V ptie P 73 P 74
P 27 5 * ptie
P 47 5 * ptie
W 24 2 50 8
S 3 H ptie P 75 P 76
P 47 65 * ntie
P 27 65 * ntie
W 24 62 50 68
S 3 H ntie P 77 P 78
P 21 65 * ntie
P 7 65 * ntie
W 4 62 24 68
S 3 H ntie P 79 P 80
P 8 65 * ntie
P 8 40 * ntie
W 5 37 11 68
S 3 V ntie P 81 P 82
P 7 5 * ptie
P 21 5 * ptie
W 4 2 24 8
S 3 H ptie P 83 P 84
E
