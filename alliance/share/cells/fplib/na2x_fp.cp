#cell1 na2x_fp idps_symb compose *
# 19-Nov-97 18:44 19-Nov-97 18:44 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 26 65
B 5 5 26 65
F F
C i1 7 65 metal 1 1 * * N i1
C i1 7 5 metal 1 2 * * S i1
C vdd 5 65 metal2 6 3 * * W vdd
C vss 5 5 metal2 8 4 * * W vss
C vdd 26 65 metal2 6 5 * * E vdd
C vss 26 5 metal2 8 6 * * E vss
I cdn1 11 18 0 "cdn" ly * *
I cbn1 20 65 0 "cbn" ly * *
I cdn2 23 13 0 "cdn" ly * *
I via1 11 5 0 "cvia" ly * *
I cdp1 17 53 0 "cdp" ly * *
I cdp2 11 53 0 "cdp" ly * *
I via2 23 65 0 "cvia" ly * *
I via3 11 65 0 "cvia" ly * *
I cbn2 14 65 0 "cbn" ly * *
I cdp3 11 58 0 "cdp" ly * *
I cdp4 23 58 0 "cdp" ly * *
I cpf1 22 33 0 "cpf" ly * *
I cpf2 12 38 0 "cpf" ly * *
I cbp1 14 5 0 "cbp" ly * *
I cbp2 20 5 0 "cbp" ly * *
I via4 23 5 0 "cvia" ly * *
I via5 17 65 0 "cvia" ly * *
I via6 17 5 0 "cvia" ly * *
I cdn3 11 13 0 "cdn" ly * *
I nwell_28 14 53 0 "ref_ref" ly * *
I o_0 17 58 0 "ref_con" ly * *
I o_2 17 48 0 "ref_con" ly * *
I i0_2 23 48 0 "ref_con" ly * *
I i0_8 23 18 0 "ref_con" ly * *
I i0_7 23 23 0 "ref_con" ly * *
I o_3 17 43 0 "ref_con" ly * *
I o_4 17 38 0 "ref_con" ly * *
I i0_4 23 38 0 "ref_con" ly * *
I o_5 17 33 0 "ref_con" ly * *
I i0_3 23 43 0 "ref_con" ly * *
I i0_6 23 28 0 "ref_con" ly * *
I o_6 17 28 0 "ref_con" ly * *
I o_7 17 23 0 "ref_con" ly * *
I o_8 17 18 0 "ref_con" ly * *
I o_9 17 13 0 "ref_con" ly * *
I i0_1 23 53 0 "ref_con" ly * *
P 11 12 * metal
P 11 13 * metal
W 10 11 12 14
S 1 V metal P 1 P 2
P 11 12 * metal
P 17 12 * metal
W 10 11 18 13
S 1 H metal P 3 P 4
P 11 17 * metal
P 11 18 * metal
W 10 16 12 19 o
S 1 V metal P 5 P 6
P 17 17 * metal
P 17 58 * metal
W 16 16 18 59 o
S 1 V metal P 7 P 8
P 17 12 * metal
P 17 17 * metal
W 16 11 18 18 o
S 1 V metal P 9 P 10
P 11 17 * metal
P 17 17 * metal
W 10 16 18 18 o
S 1 H metal P 11 P 12
P 14 8 * ntrans
P 14 23 * ntrans
W 13 7 15 24
S 1 V ntrans P 13 P 14
P 20 8 * ntrans
P 20 23 * ntrans
W 19 7 21 24
S 1 V ntrans P 15 P 16
P 23 10 * ndif
P 23 21 * ndif
W 20 7 26 24
S 3 V ndif P 17 P 18
P 20 47 * ptrans
P 20 62 * ptrans
W 19 46 21 63
S 1 V ptrans P 19 P 20
P 14 47 * ptrans
P 14 62 * ptrans
W 13 46 15 63
S 1 V ptrans P 21 P 22
P 23 49 * pdif
P 23 60 * pdif
W 20 46 26 63
S 3 V pdif P 23 P 24
P 11 49 * pdif
P 11 60 * pdif
W 8 46 14 63
S 3 V pdif P 25 P 26
P 23 58 * metal
P 23 65 * metal
W 21 56 25 67
S 2 V metal P 27 P 28
P 5 53 * nwell
P 26 53 * nwell
W -23 25 54 81
S 28 H nwell P 29 P 30
P 17 10 * ndif
P 17 21 * ndif
W 15 8 19 23
S 2 V ndif P 31 P 32
P 17 49 * pdif
P 17 60 * pdif
W 15 47 19 62
S 2 V pdif P 33 P 34
P 5 65 * metal2
P 26 65 * metal2
W -1 59 32 71
S 6 H metal2 P 35 P 36
P 5 5 * metal2
P 26 5 * metal2
W -3 -3 34 13
S 8 H metal2 P 37 P 38
P 5 13 * allowM2
P 26 13 * allowM2
W 3 11 28 15
S 2 H allowM2 P 39 P 40
P 5 18 * allowM2
P 26 18 * allowM2
W 3 16 28 20
S 2 H allowM2 P 41 P 42
P 5 23 * allowM2
P 26 23 * allowM2
W 3 21 28 25
S 2 H allowM2 P 43 P 44
P 5 28 * allowM2
P 26 28 * allowM2
W 3 26 28 30
S 2 H allowM2 P 45 P 46
P 5 33 * allowM2
P 26 33 * allowM2
W 3 31 28 35
S 2 H allowM2 P 47 P 48
P 5 43 * allowM2
P 26 43 * allowM2
W 3 41 28 45
S 2 H allowM2 P 49 P 50
P 5 48 * allowM2
P 26 48 * allowM2
W 3 46 28 50
S 2 H allowM2 P 51 P 52
P 5 53 * allowM2
P 26 53 * allowM2
W 3 51 28 55
S 2 H allowM2 P 53 P 54
P 5 38 * allowM2
P 26 38 * allowM2
W 3 36 28 40
S 2 H allowM2 P 55 P 56
P 5 58 * allowM2
P 26 58 * allowM2
W 3 56 28 60
S 2 H allowM2 P 57 P 58
P 11 10 * ndif
P 11 21 * ndif
W 8 7 14 24
S 3 V ndif P 59 P 60
P 11 5 * metal
P 23 5 * metal
W 9 3 25 7
S 2 H metal P 61 P 62
P 23 5 * metal
P 23 13 * metal
W 21 3 25 15
S 2 V metal P 63 P 64
P 7 38 * metal
P 7 65 * metal
W 6 37 8 66 i1
S 1 V metal P 65 P 66
P 7 5 * metal
P 7 38 * metal
W 6 4 8 39 i1
S 1 V metal P 67 P 68
P 7 38 * metal
P 12 38 * metal
W 6 37 13 39 i1
S 1 H metal P 69 P 70
P 23 33 * metal
P 23 53 * metal
W 22 32 24 54 i0
S 1 V metal P 71 P 72
P 23 18 * metal
P 23 33 * metal
W 22 17 24 34
S 1 V metal P 73 P 74
P 22 33 * metal
P 23 33 * metal
W 21 32 24 34 i0
S 1 H metal P 75 P 76
P 20 33 * poly
P 20 47 * poly
W 19 32 21 48
S 1 V poly P 77 P 78
P 20 23 * poly
P 20 33 * poly
W 19 22 21 34
S 1 V poly P 79 P 80
P 20 33 * poly
P 22 33 * poly
W 18 31 24 35
S 2 H poly P 81 P 82
P 14 38 * poly
P 14 47 * poly
W 13 37 15 48
S 1 V poly P 83 P 84
P 14 23 * poly
P 14 38 * poly
W 13 22 15 39
S 1 V poly P 85 P 86
P 12 38 * poly
P 14 38 * poly
W 10 36 16 40
S 2 H poly P 87 P 88
P 11 53 * metal
P 11 58 * metal
W 9 51 13 60
S 2 V metal P 89 P 90
P 11 58 * metal
P 11 65 * metal
W 9 56 13 67
S 2 V metal P 91 P 92
P 12 65 * metal
P 23 65 * metal
W 10 63 25 67
S 2 H metal P 93 P 94
P 11 65 * metal
P 12 65 * metal
W 9 63 14 67
S 2 H metal P 95 P 96
E
