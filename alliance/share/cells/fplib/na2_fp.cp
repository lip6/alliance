#cell1 na2_fp idps_symb compose *
# 19-Nov-97 17:50 19-Nov-97 17:50 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 23 65
B 5 5 23 65
F F
C vdd 5 65 metal2 6 1 * * W vdd
C vdd 23 65 metal2 6 2 * * E vdd
C vss 5 5 metal2 8 3 * * W vss
C vss 23 5 metal2 8 4 * * E vss
I cbn1 17 65 0 "cbn" ly * *
I cdn1 8 13 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I via1 8 5 0 "cvia" ly * *
I cdp1 14 53 0 "cdp" ly * *
I via2 20 65 0 "cvia" ly * *
I via3 8 65 0 "cvia" ly * *
I cbn2 11 65 0 "cbn" ly * *
I cdp2 8 58 0 "cdp" ly * *
I cdp3 20 58 0 "cdp" ly * *
I cpf1 19 33 0 "cpf" ly * *
I cpf2 9 38 0 "cpf" ly * *
I cbp1 11 5 0 "cbp" ly * *
I cbp2 17 5 0 "cbp" ly * *
I via4 20 5 0 "cvia" ly * *
I via5 14 65 0 "cvia" ly * *
I via6 14 5 0 "cvia" ly * *
I nwell_28 14 53 0 "ref_ref" ly * *
I i0_1 8 53 0 "ref_con" ly * *
I i1_1 20 53 0 "ref_con" ly * *
I o_0 14 58 0 "ref_con" ly * *
I i0_2 8 48 0 "ref_con" ly * *
I o_2 14 48 0 "ref_con" ly * *
I i1_2 20 48 0 "ref_con" ly * *
I i1_8 20 18 0 "ref_con" ly * *
I i1_7 20 23 0 "ref_con" ly * *
I i0_7 8 23 0 "ref_con" ly * *
I i0_8 8 18 0 "ref_con" ly * *
I o_3 14 43 0 "ref_con" ly * *
I o_4 14 38 0 "ref_con" ly * *
I i0_3 8 43 0 "ref_con" ly * *
I i1_4 20 38 0 "ref_con" ly * *
I i0_5 8 33 0 "ref_con" ly * *
I o_5 14 33 0 "ref_con" ly * *
I i1_3 20 43 0 "ref_con" ly * *
I i0_6 8 28 0 "ref_con" ly * *
I i1_6 20 28 0 "ref_con" ly * *
I o_6 14 28 0 "ref_con" ly * *
I o_7 14 23 0 "ref_con" ly * *
I o_8 14 18 0 "ref_con" ly * *
I o_9 14 13 0 "ref_con" ly * *
P 11 8 * ntrans
P 11 23 * ntrans
W 10 7 12 24
S 1 V ntrans P 1 P 2
P 17 8 * ntrans
P 17 23 * ntrans
W 16 7 18 24
S 1 V ntrans P 3 P 4
P 20 10 * ndif
P 20 21 * ndif
W 17 7 23 24
S 3 V ndif P 5 P 6
P 17 47 * ptrans
P 17 62 * ptrans
W 16 46 18 63
S 1 V ptrans P 7 P 8
P 11 47 * ptrans
P 11 62 * ptrans
W 10 46 12 63
S 1 V ptrans P 9 P 10
P 20 49 * pdif
P 20 60 * pdif
W 17 46 23 63
S 3 V pdif P 11 P 12
P 8 49 * pdif
P 8 60 * pdif
W 5 46 11 63
S 3 V pdif P 13 P 14
P 8 65 * metal
P 20 65 * metal
W 6 63 22 67
S 2 H metal P 15 P 16
P 8 58 * metal
P 8 65 * metal
W 6 56 10 67
S 2 V metal P 17 P 18
P 20 58 * metal
P 20 65 * metal
W 18 56 22 67
S 2 V metal P 19 P 20
P 5 53 * nwell
P 23 53 * nwell
W -23 25 51 81
S 28 H nwell P 21 P 22
P 14 10 * ndif
P 14 21 * ndif
W 12 8 16 23
S 2 V ndif P 23 P 24
P 14 49 * pdif
P 14 60 * pdif
W 12 47 16 62
S 2 V pdif P 25 P 26
P 14 12 * metal
P 14 58 * metal
W 13 11 15 59 o
S 1 V metal P 27 P 28
P 5 65 * metal2
P 23 65 * metal2
W -1 59 29 71
S 6 H metal2 P 29 P 30
P 5 5 * metal2
P 23 5 * metal2
W -3 -3 31 13
S 8 H metal2 P 31 P 32
P 5 13 * allowM2
P 23 13 * allowM2
W 3 11 25 15
S 2 H allowM2 P 33 P 34
P 5 18 * allowM2
P 23 18 * allowM2
W 3 16 25 20
S 2 H allowM2 P 35 P 36
P 5 23 * allowM2
P 23 23 * allowM2
W 3 21 25 25
S 2 H allowM2 P 37 P 38
P 5 28 * allowM2
P 23 28 * allowM2
W 3 26 25 30
S 2 H allowM2 P 39 P 40
P 5 33 * allowM2
P 23 33 * allowM2
W 3 31 25 35
S 2 H allowM2 P 41 P 42
P 5 43 * allowM2
P 23 43 * allowM2
W 3 41 25 45
S 2 H allowM2 P 43 P 44
P 5 48 * allowM2
P 23 48 * allowM2
W 3 46 25 50
S 2 H allowM2 P 45 P 46
P 5 53 * allowM2
P 23 53 * allowM2
W 3 51 25 55
S 2 H allowM2 P 47 P 48
P 5 38 * allowM2
P 23 38 * allowM2
W 3 36 25 40
S 2 H allowM2 P 49 P 50
P 5 58 * allowM2
P 23 58 * allowM2
W 3 56 25 60
S 2 H allowM2 P 51 P 52
P 8 10 * ndif
P 8 21 * ndif
W 5 7 11 24
S 3 V ndif P 53 P 54
P 8 5 * metal
P 20 5 * metal
W 6 3 22 7
S 2 H metal P 55 P 56
P 8 5 * metal
P 8 13 * metal
W 6 3 10 15
S 2 V metal P 57 P 58
P 8 38 * metal
P 8 53 * metal
W 7 37 9 54 i0
S 1 V metal P 59 P 60
P 8 18 * metal
P 8 38 * metal
W 7 17 9 39 i0
S 1 V metal P 61 P 62
P 8 38 * metal
P 9 38 * metal
W 7 37 10 39 i0
S 1 H metal P 63 P 64
P 20 33 * metal
P 20 53 * metal
W 19 32 21 54 i1
S 1 V metal P 65 P 66
P 20 18 * metal
P 20 33 * metal
W 19 17 21 34 i1
S 1 V metal P 67 P 68
P 19 33 * metal
P 20 33 * metal
W 18 32 21 34 i1
S 1 H metal P 69 P 70
P 17 33 * poly
P 17 47 * poly
W 16 32 18 48
S 1 V poly P 71 P 72
P 17 23 * poly
P 17 33 * poly
W 16 22 18 34
S 1 V poly P 73 P 74
P 17 33 * poly
P 19 33 * poly
W 15 31 21 35
S 2 H poly P 75 P 76
P 11 38 * poly
P 11 47 * poly
W 10 37 12 48
S 1 V poly P 77 P 78
P 11 23 * poly
P 11 38 * poly
W 10 22 12 39
S 1 V poly P 79 P 80
P 9 38 * poly
P 11 38 * poly
W 7 36 13 40
S 2 H poly P 81 P 82
P 20 12 * metal
P 20 13 * metal
W 19 11 21 14
S 1 V metal P 83 P 84
P 14 12 * metal
P 20 12 * metal
W 13 11 21 13
S 1 H metal P 85 P 86
E
