#cell1 buff_fp idps_symb compose *
# 19-Nov-97 16:52 19-Nov-97 16:52 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 23 65
B 5 5 23 65
F F
C vdd 5 65 metal2 6 1 * * W vdd
C vdd 23 65 metal2 6 2 * * E vdd
C vss 23 5 metal2 8 3 * * E vss
C vss 5 5 metal2 8 4 * * W vss
I cbn1 11 65 0 "cbn" ly * *
I cbn2 17 65 0 "cbn" ly * *
I via1 14 65 0 "cvia" ly * *
I via2 20 65 0 "cvia" ly * *
I via3 8 65 0 "cvia" ly * *
I cdp1 14 58 0 "cdp" ly * *
I cdp2 20 53 0 "cdp" ly * *
I cdp3 8 53 0 "cdp" ly * *
I cdp4 8 58 0 "cdp" ly * *
I via4 8 5 0 "cvia" ly * *
I via5 14 5 0 "cvia" ly * *
I via6 20 5 0 "cvia" ly * *
I cbp1 11 5 0 "cbp" ly * *
I cbp2 17 5 0 "cbp" ly * *
I cdn1 20 13 0 "cdn" ly * *
I cdn2 14 13 0 "cdn" ly * *
I cdn3 8 13 0 "cdn" ly * *
I cpf1 13 33 0 "cpf" ly * *
I cpf2 15 23 0 "cpf" ly * *
I o_0 20 58 0 "ref_con" ly * *
I o_2 20 48 0 "ref_con" ly * *
I o_3 20 43 0 "ref_con" ly * *
I o_4 20 38 0 "ref_con" ly * *
I o_5 20 33 0 "ref_con" ly * *
I o_6 20 28 0 "ref_con" ly * *
I o_7 20 23 0 "ref_con" ly * *
I o_8 20 18 0 "ref_con" ly * *
I i_1 14 53 0 "ref_con" ly * *
I i_2 14 48 0 "ref_con" ly * *
I i_3 14 43 0 "ref_con" ly * *
I i_4 14 38 0 "ref_con" ly * *
I i_6 14 28 0 "ref_con" ly * *
I nwell_28 22 53 0 "ref_ref" ly * *
P 17 38 * ptrans
P 17 62 * ptrans
W 16 37 18 63
S 1 V ptrans P 1 P 2
P 11 49 * ptrans
P 11 62 * ptrans
W 10 48 12 63
S 1 V ptrans P 3 P 4
P 14 40 * pdif
P 14 60 * pdif
W 11 37 17 63
S 3 V pdif P 5 P 6
P 20 40 * pdif
P 20 60 * pdif
W 17 37 23 63
S 3 V pdif P 7 P 8
P 8 51 * pdif
P 8 60 * pdif
W 5 48 11 63
S 3 V pdif P 9 P 10
P 5 53 * nwell
P 23 53 * nwell
W -23 25 51 81
S 28 H nwell P 11 P 12
P 5 65 * metal2
P 23 65 * metal2
W -1 59 29 71
S 6 H metal2 P 13 P 14
P 5 58 * allowM2
P 23 58 * allowM2
W 3 56 25 60
S 2 H allowM2 P 15 P 16
P 5 53 * allowM2
P 23 53 * allowM2
W 3 51 25 55
S 2 H allowM2 P 17 P 18
P 5 48 * allowM2
P 23 48 * allowM2
W 3 46 25 50
S 2 H allowM2 P 19 P 20
P 5 43 * allowM2
P 23 43 * allowM2
W 3 41 25 45
S 2 H allowM2 P 21 P 22
P 5 38 * allowM2
P 23 38 * allowM2
W 3 36 25 40
S 2 H allowM2 P 23 P 24
P 5 5 * metal2
P 23 5 * metal2
W -3 -3 31 13
S 8 H metal2 P 25 P 26
P 11 8 * ntrans
P 11 16 * ntrans
W 10 7 12 17
S 1 V ntrans P 27 P 28
P 17 8 * ntrans
P 17 22 * ntrans
W 16 7 18 23
S 1 V ntrans P 29 P 30
P 14 10 * ndif
P 14 20 * ndif
W 11 7 17 23
S 3 V ndif P 31 P 32
P 20 10 * ndif
P 20 20 * ndif
W 17 7 23 23
S 3 V ndif P 33 P 34
P 8 10 * ndif
P 8 14 * ndif
W 5 7 11 17
S 3 V ndif P 35 P 36
P 5 13 * allowM2
P 23 13 * allowM2
W 3 11 25 15
S 2 H allowM2 P 37 P 38
P 5 33 * allowM2
P 23 33 * allowM2
W 3 31 25 35
S 2 H allowM2 P 39 P 40
P 5 28 * allowM2
P 23 28 * allowM2
W 3 26 25 30
S 2 H allowM2 P 41 P 42
P 5 23 * allowM2
P 23 23 * allowM2
W 3 21 25 25
S 2 H allowM2 P 43 P 44
P 5 18 * allowM2
P 23 18 * allowM2
W 3 16 25 20
S 2 H allowM2 P 45 P 46
P 17 22 * poly
P 17 38 * poly
W 16 21 18 39
S 1 V poly P 47 P 48
P 11 16 * poly
P 11 49 * poly
W 10 15 12 50
S 1 V poly P 49 P 50
P 8 13 * metal
P 8 58 * metal
W 7 12 9 59
S 1 V metal P 51 P 52
P 14 58 * metal
P 14 65 * metal
W 12 56 16 67
S 2 V metal P 53 P 54
P 9 65 * metal
P 20 65 * metal
W 7 63 22 67
S 2 H metal P 55 P 56
P 8 5 * metal
P 20 5 * metal
W 6 3 22 7
S 2 H metal P 57 P 58
P 14 5 * metal
P 14 12 * metal
W 12 3 16 14
S 2 V metal P 59 P 60
P 11 33 * poly
P 14 33 * poly
W 8 30 17 36
S 3 H poly P 61 P 62
P 14 23 * poly
P 17 23 * poly
W 11 20 20 26
S 3 H poly P 63 P 64
P 14 28 * metal
P 14 53 * metal
W 13 27 15 54 i
S 1 V metal P 65 P 66
P 20 13 * metal
P 20 58 * metal
W 19 12 21 59 o
S 1 V metal P 67 P 68
P 13 33 * metal
P 14 33 * metal
W 12 32 15 34
S 1 H metal P 69 P 70
P 8 23 * metal
P 15 23 * metal
W 7 22 16 24
S 1 H metal P 71 P 72
E
