#cell1 no2x_fp idps_symb compose *
# 19-Nov-97 18:32 19-Nov-97 18:32 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 26 65
B 5 5 26 65
F F
C i1 7 5 metal 1 1 * * S i1
C i1 7 65 metal 1 2 * * N i1
C vss 26 5 metal2 8 3 * * E vss
C vss 5 5 metal2 8 4 * * W vss
C vdd 26 65 metal2 6 5 * * E vdd
C vdd 5 65 metal2 6 6 * * W vdd
I cdp1 11 42 0 "cdp" ly * *
I cdp2 11 47 0 "cdp" ly * *
I cpf1 12 33 0 "cpf" ly * *
I cbn1 20 65 0 "cbn" ly * *
I cbn2 14 65 0 "cbn" ly * *
I via1 23 65 0 "cvia" ly * *
I cbp1 20 5 0 "cbp" ly * *
I cbp2 14 5 0 "cbp" ly * *
I via2 23 5 0 "cvia" ly * *
I cdp3 11 57 0 "cdp" ly * *
I cdn1 23 13 0 "cdn" ly * *
I cdn2 11 13 0 "cdn" ly * *
I cdn3 17 13 0 "cdn" ly * *
I cdp4 11 52 0 "cdp" ly * *
I via3 11 5 0 "cvia" ly * *
I cdp5 23 58 0 "cdp" ly * *
I via4 11 65 0 "cvia" ly * *
I cpf2 22 28 0 "cpf" ly * *
I via5 17 65 0 "cvia" ly * *
I via6 17 5 0 "cvia" ly * *
I i0_1 23 53 0 "ref_con" ly * *
I nwell_28 17 53 0 "ref_ref" ly * *
I i0_4 23 38 0 "ref_con" ly * *
I i0_3 23 43 0 "ref_con" ly * *
I i0_2 23 48 0 "ref_con" ly * *
I i0_7 23 23 0 "ref_con" ly * *
I o_5 17 33 0 "ref_con" ly * *
I o_4 17 38 0 "ref_con" ly * *
I o_3 17 43 0 "ref_con" ly * *
I i0_5 23 33 0 "ref_con" ly * *
I i0_8 23 18 0 "ref_con" ly * *
I o_6 17 28 0 "ref_con" ly * *
I o_2 17 48 0 "ref_con" ly * *
I o_1 17 53 0 "ref_con" ly * *
I o_0 17 58 0 "ref_con" ly * *
I o_7 17 23 0 "ref_con" ly * *
I o_8 17 18 0 "ref_con" ly * *
P 17 10 * ndif
P 17 15 * ndif
W 15 8 19 17
S 2 V ndif P 1 P 2
P 5 13 * allowM2
P 26 13 * allowM2
W 3 11 28 15
S 2 H allowM2 P 3 P 4
P 5 18 * allowM2
P 26 18 * allowM2
W 3 16 28 20
S 2 H allowM2 P 5 P 6
P 5 23 * allowM2
P 26 23 * allowM2
W 3 21 28 25
S 2 H allowM2 P 7 P 8
P 5 28 * allowM2
P 26 28 * allowM2
W 3 26 28 30
S 2 H allowM2 P 9 P 10
P 5 33 * allowM2
P 26 33 * allowM2
W 3 31 28 35
S 2 H allowM2 P 11 P 12
P 5 38 * allowM2
P 26 38 * allowM2
W 3 36 28 40
S 2 H allowM2 P 13 P 14
P 5 43 * allowM2
P 26 43 * allowM2
W 3 41 28 45
S 2 H allowM2 P 15 P 16
P 5 48 * allowM2
P 26 48 * allowM2
W 3 46 28 50
S 2 H allowM2 P 17 P 18
P 5 53 * allowM2
P 26 53 * allowM2
W 3 51 28 55
S 2 H allowM2 P 19 P 20
P 5 58 * allowM2
P 26 58 * allowM2
W 3 56 28 60
S 2 H allowM2 P 21 P 22
P 17 40 * pdif
P 17 60 * pdif
W 15 38 19 62
S 2 V pdif P 23 P 24
P 5 65 * metal2
P 26 65 * metal2
W -1 59 32 71
S 6 H metal2 P 25 P 26
P 11 65 * metal
P 23 65 * metal
W 9 63 25 67
S 2 H metal P 27 P 28
P 5 5 * metal2
P 26 5 * metal2
W -3 -3 34 13
S 8 H metal2 P 29 P 30
P 11 5 * metal
P 23 5 * metal
W 9 3 25 7
S 2 H metal P 31 P 32
P 23 5 * metal
P 23 13 * metal
W 21 3 25 15
S 2 V metal P 33 P 34
P 23 40 * pdif
P 23 60 * pdif
W 20 37 26 63
S 3 V pdif P 35 P 36
P 20 38 * ptrans
P 20 62 * ptrans
W 19 37 21 63
S 1 V ptrans P 37 P 38
P 14 38 * ptrans
P 14 62 * ptrans
W 13 37 15 63
S 1 V ptrans P 39 P 40
P 11 5 * metal
P 11 13 * metal
W 9 3 13 15
S 2 V metal P 41 P 42
P 23 58 * metal
P 23 65 * metal
W 21 56 25 67
S 2 V metal P 43 P 44
P 5 53 * nwell
P 26 53 * nwell
W -23 25 54 81
S 28 H nwell P 45 P 46
P 11 10 * ndif
P 11 15 * ndif
W 8 7 14 18
S 3 V ndif P 47 P 48
P 23 10 * ndif
P 23 15 * ndif
W 20 7 26 18
S 3 V ndif P 49 P 50
P 11 40 * pdif
P 11 60 * pdif
W 8 37 14 63
S 3 V pdif P 51 P 52
P 14 8 * ntrans
P 14 17 * ntrans
W 13 7 15 18
S 1 V ntrans P 53 P 54
P 20 8 * ntrans
P 20 17 * ntrans
W 19 7 21 18
S 1 V ntrans P 55 P 56
P 7 5 * metal
P 7 33 * metal
W 6 4 8 34 i1
S 1 V metal P 57 P 58
P 7 33 * metal
P 7 65 * metal
W 6 32 8 66 i1
S 1 V metal P 59 P 60
P 7 33 * metal
P 12 33 * metal
W 6 32 13 34 i1
S 1 H metal P 61 P 62
P 23 28 * metal
P 23 53 * metal
W 22 27 24 54 i0
S 1 V metal P 63 P 64
P 23 18 * metal
P 23 28 * metal
W 22 17 24 29 i0
S 1 V metal P 65 P 66
P 22 28 * metal
P 23 28 * metal
W 21 27 24 29 i0
S 1 H metal P 67 P 68
P 14 33 * poly
P 14 38 * poly
W 13 32 15 39
S 1 V poly P 69 P 70
P 14 17 * poly
P 14 33 * poly
W 13 16 15 34
S 1 V poly P 71 P 72
P 12 33 * poly
P 14 33 * poly
W 11 32 15 34
S 1 H poly P 73 P 74
P 20 28 * poly
P 20 38 * poly
W 19 27 21 39
S 1 V poly P 75 P 76
P 20 17 * poly
P 20 28 * poly
W 19 16 21 29
S 1 V poly P 77 P 78
P 20 28 * poly
P 22 28 * poly
W 18 26 24 30
S 2 H poly P 79 P 80
P 11 57 * metal
P 17 57 * metal
W 10 56 18 58 o
S 1 H metal P 81 P 82
P 11 47 * metal
P 17 47 * metal
W 10 46 18 48 o
S 1 H metal P 83 P 84
P 17 47 * metal
P 17 52 * metal
W 16 46 18 53 o
S 1 V metal P 85 P 86
P 17 52 * metal
P 17 58 * metal
W 16 51 18 59 o
S 1 V metal P 87 P 88
P 11 52 * metal
P 17 52 * metal
W 10 51 18 53 o
S 1 H metal P 89 P 90
P 17 42 * metal
P 17 47 * metal
W 16 41 18 48 o
S 1 V metal P 91 P 92
P 17 13 * metal
P 17 42 * metal
W 16 12 18 43 o
S 1 V metal P 93 P 94
P 11 42 * metal
P 17 42 * metal
W 10 41 18 43 o
S 1 H metal P 95 P 96
E
