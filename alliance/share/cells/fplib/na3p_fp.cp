#cell1 na3p_fp idps_symb compose *
# 19-Nov-97 18:05 19-Nov-97 18:05 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 29 65
B 5 5 29 65
F F
C vss 5 5 metal2 8 1 * * W vss
C vss 29 5 metal2 8 2 * * E vss
C vdd 5 65 metal2 6 3 * * W vdd
C vdd 29 65 metal2 6 4 * * E vdd
I cbp1 23 5 0 "cbp" ly * *
I cbp2 17 5 0 "cbp" ly * *
I cbp3 11 5 0 "cbp" ly * *
I cdn1 26 23 0 "cdn" ly * *
I cdp1 8 58 0 "cdp" ly * *
I cdp2 26 53 0 "cdp" ly * *
I cpf1 15 33 0 "cpf" ly * *
I via1 26 5 0 "cvia" ly * *
I via2 20 5 0 "cvia" ly * *
I via3 26 65 0 "cvia" ly * *
I cdp3 14 58 0 "cdp" ly * *
I cdp4 20 58 0 "cdp" ly * *
I cdn2 8 13 0 "cdn" ly * *
I via4 14 5 0 "cvia" ly * *
I via5 20 65 0 "cvia" ly * *
I cpf2 9 33 0 "cpf" ly * *
I cpf3 21 33 0 "cpf" ly * *
I via6 8 5 0 "cvia" ly * *
I cbn1 23 65 0 "cbn" ly * *
I cbn2 17 65 0 "cbn" ly * *
I via7 8 65 0 "cvia" ly * *
I via8 14 65 0 "cvia" ly * *
I cbn3 11 65 0 "cbn" ly * *
I i1_2 14 48 0 "ref_con" ly * *
I i2_2 20 48 0 "ref_con" ly * *
I o_3 26 43 0 "ref_con" ly * *
I i0_1 8 53 0 "ref_con" ly * *
I i0_8 8 18 0 "ref_con" ly * *
I o_9 26 13 0 "ref_con" ly * *
I o_1 14 53 0 "ref_con" ly * *
I nwell_28 17 53 0 "ref_ref" ly * *
I i2_7 20 23 0 "ref_con" ly * *
I i1_7 14 23 0 "ref_con" ly * *
I i0_7 8 23 0 "ref_con" ly * *
I o_8 26 18 0 "ref_con" ly * *
I o_2 26 48 0 "ref_con" ly * *
I i0_2 8 48 0 "ref_con" ly * *
I o_0 26 58 0 "ref_con" ly * *
I o_6 26 28 0 "ref_con" ly * *
I i1_8 14 18 0 "ref_con" ly * *
I i1_9 14 13 0 "ref_con" ly * *
I i2_8 20 18 0 "ref_con" ly * *
I i2_9 20 13 0 "ref_con" ly * *
I i0_4 8 38 0 "ref_con" ly * *
I o_4 26 38 0 "ref_con" ly * *
I i2_6 20 28 0 "ref_con" ly * *
I i2_4 20 38 0 "ref_con" ly * *
I i2_3 20 43 0 "ref_con" ly * *
I i1_3 14 43 0 "ref_con" ly * *
I i1_4 14 38 0 "ref_con" ly * *
I i1_6 14 28 0 "ref_con" ly * *
I i0_6 8 28 0 "ref_con" ly * *
I i0_3 8 43 0 "ref_con" ly * *
I o_5 26 33 0 "ref_con" ly * *
P 23 38 * ptrans
P 23 62 * ptrans
W 22 37 24 63
S 1 V ptrans P 1 P 2
P 11 8 * ntrans
P 11 32 * ntrans
W 10 7 12 33
S 1 V ntrans P 3 P 4
P 17 8 * ntrans
P 17 32 * ntrans
W 16 7 18 33
S 1 V ntrans P 5 P 6
P 11 38 * ptrans
P 11 62 * ptrans
W 10 37 12 63
S 1 V ptrans P 7 P 8
P 17 38 * ptrans
P 17 62 * ptrans
W 16 37 18 63
S 1 V ptrans P 9 P 10
P 23 8 * ntrans
P 23 32 * ntrans
W 22 7 24 33
S 1 V ntrans P 11 P 12
P 8 5 * metal
P 26 5 * metal
W 6 3 28 7
S 2 H metal P 13 P 14
P 5 5 * metal2
P 29 5 * metal2
W -3 -3 37 13
S 8 H metal2 P 15 P 16
P 5 65 * metal2
P 29 65 * metal2
W -1 59 35 71
S 6 H metal2 P 17 P 18
P 5 13 * allowM2
P 29 13 * allowM2
W 3 11 31 15
S 2 H allowM2 P 19 P 20
P 5 23 * allowM2
P 29 23 * allowM2
W 3 21 31 25
S 2 H allowM2 P 21 P 22
P 5 28 * allowM2
P 29 28 * allowM2
W 3 26 31 30
S 2 H allowM2 P 23 P 24
P 5 33 * allowM2
P 29 33 * allowM2
W 3 31 31 35
S 2 H allowM2 P 25 P 26
P 5 38 * allowM2
P 29 38 * allowM2
W 3 36 31 40
S 2 H allowM2 P 27 P 28
P 5 43 * allowM2
P 29 43 * allowM2
W 3 41 31 45
S 2 H allowM2 P 29 P 30
P 5 48 * allowM2
P 29 48 * allowM2
W 3 46 31 50
S 2 H allowM2 P 31 P 32
P 5 53 * allowM2
P 29 53 * allowM2
W 3 51 31 55
S 2 H allowM2 P 33 P 34
P 5 58 * allowM2
P 29 58 * allowM2
W 3 56 31 60
S 2 H allowM2 P 35 P 36
P 5 18 * allowM2
P 29 18 * allowM2
W 3 16 31 20
S 2 H allowM2 P 37 P 38
P 26 10 * ndif
P 26 30 * ndif
W 23 7 29 33
S 3 V ndif P 39 P 40
P 20 10 * ndif
P 20 30 * ndif
W 18 8 22 32
S 2 V ndif P 41 P 42
P 14 10 * ndif
P 14 30 * ndif
W 12 8 16 32
S 2 V ndif P 43 P 44
P 8 10 * ndif
P 8 30 * ndif
W 5 7 11 33
S 3 V ndif P 45 P 46
P 5 53 * nwell
P 29 53 * nwell
W -23 25 57 81
S 28 H nwell P 47 P 48
P 20 58 * metal
P 20 65 * metal
W 18 56 22 67
S 2 V metal P 49 P 50
P 8 65 * metal
P 20 65 * metal
W 6 63 22 67
S 2 H metal P 51 P 52
P 8 58 * metal
P 8 65 * metal
W 6 56 10 67
S 2 V metal P 53 P 54
P 20 65 * metal
P 26 65 * metal
W 18 63 28 67
S 2 H metal P 55 P 56
P 8 5 * metal
P 8 13 * metal
W 6 3 10 15
S 2 V metal P 57 P 58
P 26 40 * pdif
P 26 60 * pdif
W 23 37 29 63
S 3 V pdif P 59 P 60
P 8 40 * pdif
P 8 60 * pdif
W 5 37 11 63
S 3 V pdif P 61 P 62
P 20 40 * pdif
P 20 60 * pdif
W 18 38 22 62
S 2 V pdif P 63 P 64
P 14 40 * pdif
P 14 60 * pdif
W 12 38 16 62
S 2 V pdif P 65 P 66
P 8 33 * metal
P 8 53 * metal
W 7 32 9 54 i0
S 1 V metal P 67 P 68
P 8 18 * metal
P 8 33 * metal
W 7 17 9 34 i0
S 1 V metal P 69 P 70
P 8 33 * metal
P 9 33 * metal
W 7 32 10 34 i0
S 1 H metal P 71 P 72
P 20 33 * metal
P 20 48 * metal
W 19 32 21 49 i2
S 1 V metal P 73 P 74
P 20 13 * metal
P 20 33 * metal
W 19 12 21 34 i2
S 1 V metal P 75 P 76
P 20 33 * metal
P 21 33 * metal
W 19 32 22 34 i2
S 1 H metal P 77 P 78
P 23 33 * poly
P 23 38 * poly
W 22 32 24 39
S 1 V poly P 79 P 80
P 23 32 * poly
P 23 33 * poly
W 22 31 24 34
S 1 V poly P 81 P 82
P 21 33 * poly
P 23 33 * poly
W 19 31 25 35
S 2 H poly P 83 P 84
P 17 33 * poly
P 17 38 * poly
W 16 32 18 39
S 1 V poly P 85 P 86
P 17 32 * poly
P 17 33 * poly
W 16 31 18 34
S 1 V poly P 87 P 88
P 15 33 * poly
P 17 33 * poly
W 13 31 19 35
S 2 H poly P 89 P 90
P 11 33 * poly
P 11 38 * poly
W 10 32 12 39
S 1 V poly P 91 P 92
P 11 32 * poly
P 11 33 * poly
W 10 31 12 34
S 1 V poly P 93 P 94
P 9 33 * poly
P 11 33 * poly
W 7 31 13 35
S 2 H poly P 95 P 96
P 14 33 * metal
P 14 48 * metal
W 13 32 15 49 i1
S 1 V metal P 97 P 98
P 14 33 * metal
P 15 33 * metal
W 13 32 16 34 i1
S 1 H metal P 99 P 100
P 14 13 * metal
P 14 33 * metal
W 13 12 15 34 i1
S 1 V metal P 101 P 102
P 26 13 * metal
P 26 49 * metal
W 25 12 27 50 o
S 1 V metal P 103 P 104
P 26 52 * metal
P 26 58 * metal
W 25 51 27 59 o
S 1 V metal P 105 P 106
P 26 49 * metal
P 26 52 * metal
W 25 48 27 53 o
S 1 V metal P 107 P 108
P 14 52 * metal
P 26 52 * metal
W 13 51 27 53 o
S 1 H metal P 109 P 110
P 14 52 * metal
P 14 58 * metal
W 13 51 15 59 o
S 1 V metal P 111 P 112
E
