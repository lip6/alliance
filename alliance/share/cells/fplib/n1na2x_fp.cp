#cell1 n1na2x_fp idps_symb compose *
# 19-Nov-97 18:39 19-Nov-97 18:39 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 26 65
B 5 5 26 65
F F
C i1 7 5 metal 1 1 * * S i1
C i1 7 65 metal 1 2 * * N i1
C vss 5 5 metal2 8 3 * * W vss
C vss 26 5 metal2 8 4 * * E vss
C vdd 5 65 metal2 6 5 * * W vdd
C vdd 26 65 metal2 6 6 * * E vdd
I via1 17 65 0 "cvia" ly * *
I via2 23 65 0 "cvia" ly * *
I via3 17 5 0 "cvia" ly * *
I via4 23 5 0 "cvia" ly * *
I cbp1 20 5 0 "cbp" ly * *
I cbn1 20 65 0 "cbn" ly * *
I cdp1 23 53 0 "cdp" ly * *
I cdp2 17 58 0 "cdp" ly * *
I cdn1 23 13 0 "cdn" ly * *
I cdn2 17 13 0 "cdn" ly * *
I cbn2 14 65 0 "cbn" ly * *
I via5 11 65 0 "cvia" ly * *
I cbp2 14 5 0 "cbp" ly * *
I via6 11 5 0 "cvia" ly * *
I cpf1 18 23 0 "cpf" ly * *
I cdp3 17 53 0 "cdp" ly * *
I o_5 23 33 0 "ref_con" ly * *
I o_6 23 28 0 "ref_con" ly * *
I o_7 23 23 0 "ref_con" ly * *
I o_8 23 18 0 "ref_con" ly * *
I nwell_28 20 53 0 "ref_ref" ly * *
I o_0 23 58 0 "ref_con" ly * *
I o_2 23 48 0 "ref_con" ly * *
I o_4 23 38 0 "ref_con" ly * *
I o_3 23 43 0 "ref_con" ly * *
I i0_1 12 53 0 "ref_con" ly * *
I i0_2 12 48 0 "ref_con" ly * *
I i0_3 12 43 0 "ref_con" ly * *
I i0_7 12 23 0 "ref_con" ly * *
I i0_6 12 28 0 "ref_con" ly * *
I i0_5 12 33 0 "ref_con" ly * *
I i0_4 12 38 0 "ref_con" ly * *
I i0_0 12 58 0 "ref_con" ly * *
I i0_8 12 18 0 "ref_con" ly * *
I i0_9 12 13 0 "ref_con" ly * *
P 18 23 * metal
P 18 24 * metal
W 17 22 19 25
S 1 V metal P 1 P 2
P 12 24 * metal
P 18 24 * metal
W 11 23 19 25 i0
S 1 H metal P 3 P 4
P 20 17 * poly
P 20 47 * poly
W 19 16 21 48
S 1 V poly P 5 P 6
P 18 23 * poly
P 20 23 * poly
W 15 20 23 26
S 3 H poly P 7 P 8
P 7 5 * metal
P 7 65 * metal
W 6 4 8 66 i1
S 1 V metal P 9 P 10
P 5 53 * nwell
P 26 53 * nwell
W -23 25 54 81 nwell
S 28 H nwell P 11 P 12
P 5 58 * allowM2
P 26 58 * allowM2
W 3 56 28 60
S 2 H allowM2 P 13 P 14
P 5 53 * allowM2
P 26 53 * allowM2
W 3 51 28 55
S 2 H allowM2 P 15 P 16
P 5 48 * allowM2
P 26 48 * allowM2
W 3 46 28 50
S 2 H allowM2 P 17 P 18
P 5 43 * allowM2
P 26 43 * allowM2
W 3 41 28 45
S 2 H allowM2 P 19 P 20
P 5 38 * allowM2
P 26 38 * allowM2
W 3 36 28 40
S 2 H allowM2 P 21 P 22
P 5 33 * allowM2
P 26 33 * allowM2
W 3 31 28 35
S 2 H allowM2 P 23 P 24
P 5 28 * allowM2
P 26 28 * allowM2
W 3 26 28 30
S 2 H allowM2 P 25 P 26
P 5 18 * allowM2
P 26 18 * allowM2
W 3 16 28 20
S 2 H allowM2 P 27 P 28
P 5 23 * allowM2
P 26 23 * allowM2
W 3 21 28 25
S 2 H allowM2 P 29 P 30
P 5 13 * allowM2
P 26 13 * allowM2
W 3 11 28 15
S 2 H allowM2 P 31 P 32
P 5 5 * metal2
P 26 5 * metal2
W -3 -3 34 13
S 8 H metal2 P 33 P 34
P 5 65 * metal2
P 26 65 * metal2
W -1 59 32 71 vdd
S 6 H metal2 P 35 P 36
P 23 13 * metal
P 23 58 * metal
W 22 12 24 59 o
S 1 V metal P 37 P 38
P 17 5 * metal
P 17 13 * metal
W 15 3 19 15
S 2 V metal P 39 P 40
P 20 47 * ptrans
P 20 62 * ptrans
W 19 46 21 63
S 1 V ptrans P 41 P 42
P 17 49 * pdif
P 17 60 * pdif
W 14 46 20 63
S 3 V pdif P 43 P 44
P 23 49 * pdif
P 23 60 * pdif
W 20 46 26 63
S 3 V pdif P 45 P 46
P 20 8 * ntrans
P 20 17 * ntrans
W 19 7 21 18
S 1 V ntrans P 47 P 48
P 17 10 * ndif
P 17 15 * ndif
W 14 7 20 18
S 3 V ndif P 49 P 50
P 23 10 * ndif
P 23 15 * ndif
W 20 7 26 18
S 3 V ndif P 51 P 52
P 11 65 * metal
P 23 65 * metal
W 9 63 25 67
S 2 H metal P 53 P 54
P 11 5 * metal
P 23 5 * metal
W 9 3 25 7
S 2 H metal P 55 P 56
P 12 13 * metal
P 12 58 * metal
W 11 12 13 59 i0
S 1 V metal P 57 P 58
P 17 53 * metal
P 17 65 * metal
W 15 51 19 67
S 2 V metal P 59 P 60
E
