#cell1 n1_fp idps_symb compose *
# 19-Nov-97 16:37 19-Nov-97 16:37 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 17 65
B 5 5 17 65
F F
C vdd 17 65 metal2 6 1 * * E vdd
C vss 17 5 metal2 8 2 * * E vss
C vss 5 5 metal2 8 3 * * W vss
C vdd 5 65 metal2 6 4 * * W vdd
I via1 8 65 0 "cvia" ly * *
I cdp1 14 53 0 "cdp" ly * *
I cdp2 8 58 0 "cdp" ly * *
I cdn1 14 13 0 "cdn" ly * *
I cbp1 11 5 0 "cbp" ly * *
I cdn2 8 13 0 "cdn" ly * *
I via2 8 5 0 "cvia" ly * *
I cpf1 9 18 0 "cpf" ly * *
I cbn1 11 65 0 "cbn" ly * *
I via3 14 65 0 "cvia" ly * *
I via4 14 5 0 "cvia" ly * *
I i_1 8 53 0 "ref_con" ly * *
I nwell_28 11 53 0 "ref_ref" ly * *
I o_0 14 58 0 "ref_con" ly * *
I o_2 14 48 0 "ref_con" ly * *
I i_2 8 48 0 "ref_con" ly * *
I i_3 8 43 0 "ref_con" ly * *
I o_4 14 38 0 "ref_con" ly * *
I o_3 14 43 0 "ref_con" ly * *
I o_5 14 33 0 "ref_con" ly * *
I o_6 14 28 0 "ref_con" ly * *
I o_7 14 23 0 "ref_con" ly * *
I o_8 14 18 0 "ref_con" ly * *
I i_7 8 23 0 "ref_con" ly * *
I i_6 8 28 0 "ref_con" ly * *
I i_5 8 33 0 "ref_con" ly * *
I i_4 8 38 0 "ref_con" ly * *
P 5 18 * allowM2
P 17 18 * allowM2
W 3 16 19 20
S 2 H allowM2 P 1 P 2
P 5 23 * allowM2
P 17 23 * allowM2
W 3 21 19 25
S 2 H allowM2 P 3 P 4
P 5 28 * allowM2
P 17 28 * allowM2
W 3 26 19 30
S 2 H allowM2 P 5 P 6
P 5 33 * allowM2
P 17 33 * allowM2
W 3 31 19 35
S 2 H allowM2 P 7 P 8
P 5 43 * allowM2
P 17 43 * allowM2
W 3 41 19 45
S 2 H allowM2 P 9 P 10
P 5 48 * allowM2
P 17 48 * allowM2
W 3 46 19 50
S 2 H allowM2 P 11 P 12
P 5 53 * allowM2
P 17 53 * allowM2
W 3 51 19 55
S 2 H allowM2 P 13 P 14
P 5 58 * allowM2
P 17 58 * allowM2
W 3 56 19 60
S 2 H allowM2 P 15 P 16
P 5 65 * metal2
P 17 65 * metal2
W -1 59 23 71 vdd
S 6 H metal2 P 17 P 18
P 14 13 * metal
P 14 58 * metal
W 13 12 15 59 o
S 1 V metal P 19 P 20
P 8 5 * metal
P 14 5 * metal
W 6 3 16 7
S 2 H metal P 21 P 22
P 8 5 * metal
P 8 13 * metal
W 6 3 10 15
S 2 V metal P 23 P 24
P 5 53 * nwell
P 17 53 * nwell
W -23 25 45 81 nwell
S 28 H nwell P 25 P 26
P 11 47 * ptrans
P 11 62 * ptrans
W 10 46 12 63
S 1 V ptrans P 27 P 28
P 8 49 * pdif
P 8 60 * pdif
W 5 46 11 63
S 3 V pdif P 29 P 30
P 14 49 * pdif
P 14 60 * pdif
W 11 46 17 63
S 3 V pdif P 31 P 32
P 11 8 * ntrans
P 11 17 * ntrans
W 10 7 12 18
S 1 V ntrans P 33 P 34
P 8 10 * ndif
P 8 15 * ndif
W 5 7 11 18
S 3 V ndif P 35 P 36
P 14 10 * ndif
P 14 15 * ndif
W 11 7 17 18
S 3 V ndif P 37 P 38
P 8 65 * metal
P 14 65 * metal
W 6 63 16 67
S 2 H metal P 39 P 40
P 8 58 * metal
P 8 65 * metal
W 6 56 10 67
S 2 V metal P 41 P 42
P 5 5 * metal2
P 17 5 * metal2
W -3 -3 25 13
S 8 H metal2 P 43 P 44
P 5 38 * allowM2
P 17 38 * allowM2
W 3 36 19 40
S 2 H allowM2 P 45 P 46
P 5 13 * allowM2
P 17 13 * allowM2
W 3 11 19 15
S 2 H allowM2 P 47 P 48
P 8 18 * metal
P 8 53 * metal
W 7 17 9 54 i
S 1 V metal P 49 P 50
P 8 18 * metal
P 9 18 * metal
W 7 17 10 19 i
S 1 H metal P 51 P 52
P 11 18 * poly
P 11 47 * poly
W 10 17 12 48
S 1 V poly P 53 P 54
P 11 17 * poly
P 11 18 * poly
W 10 16 12 19
S 1 V poly P 55 P 56
P 9 18 * poly
P 11 18 * poly
W 7 16 13 20
S 2 H poly P 57 P 58
E
