#cell1 drv_fp idps_symb compose *
# 19-Nov-97 16:56 19-Nov-97 16:56 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 29 65
B 5 5 29 65
F F
C vss 5 5 metal2 8 1 * * W vss
C vss 29 5 metal2 8 2 * * E vss
C vdd 29 65 metal2 6 3 * * E vdd
C vdd 5 65 metal2 6 4 * * W vdd
I cpf1 15 23 0 "cpf" ly * *
I cdn1 20 18 0 "cdn" ly * *
I cdn2 26 13 0 "cdn" ly * *
I cdp1 26 53 0 "cdp" ly * *
I cdp2 26 58 0 "cdp" ly * *
I cdp3 8 58 0 "cdp" ly * *
I cpf2 12 28 0 "cpf" ly * *
I cdp4 8 53 0 "cdp" ly * *
I cdn3 14 13 0 "cdn" ly * *
I cdp5 20 48 0 "cdp" ly * *
I cbn1 11 65 0 "cbn" ly * *
I via1 20 65 0 "cvia" ly * *
I via2 8 65 0 "cvia" ly * *
I via3 26 65 0 "cvia" ly * *
I via4 14 65 0 "cvia" ly * *
I cbp1 23 5 0 "cbp" ly * *
I cbp2 11 5 0 "cbp" ly * *
I cbp3 17 5 0 "cbp" ly * *
I via5 8 5 0 "cvia" ly * *
I via6 14 5 0 "cvia" ly * *
I via7 26 5 0 "cvia" ly * *
I via8 20 5 0 "cvia" ly * *
I cbn2 17 65 0 "cbn" ly * *
I cbn3 23 65 0 "cbn" ly * *
I cdn4 14 13 0 "cdn" ly * *
I cdn5 8 13 0 "cdn" ly * *
I cdp6 14 58 0 "cdp" ly * *
I o_9 20 13 0 "ref_con" ly * *
I o_2 26 48 0 "ref_con" ly * *
I o_8 26 18 0 "ref_con" ly * *
I i_3 14 43 0 "ref_con" ly * *
I i_2 14 48 0 "ref_con" ly * *
I i_1 14 53 0 "ref_con" ly * *
I i_4 14 38 0 "ref_con" ly * *
I i_5 14 33 0 "ref_con" ly * *
I o_7 20 23 0 "ref_con" ly * *
I o_3 20 43 0 "ref_con" ly * *
I o_5 20 33 0 "ref_con" ly * *
I o_0 20 58 0 "ref_con" ly * *
I o_1 20 53 0 "ref_con" ly * *
I o_4 20 38 0 "ref_con" ly * *
I nwell_26 17 54 0 "ref_ref" ly * *
P 8 24 * metal
P 15 24 * metal
W 7 23 16 25
S 1 H metal P 1 P 2
P 15 23 * metal
P 15 24 * metal
W 14 22 16 25
S 1 V metal P 3 P 4
P 15 23 * poly
P 17 23 * poly
W 12 20 20 26
S 3 H poly P 5 P 6
P 15 23 * poly
P 17 23 * poly
W 13 21 19 25
S 2 H poly P 7 P 8
P 14 28 * metal
P 14 53 * metal
W 13 27 15 54 i
S 1 V metal P 9 P 10
P 12 28 * metal
P 14 28 * metal
W 10 26 16 30
S 2 H metal P 11 P 12
P 5 5 * metal2
P 29 5 * metal2
W -3 -3 37 13
S 8 H metal2 P 13 P 14
P 5 13 * allowM2
P 29 13 * allowM2
W 3 11 31 15
S 2 H allowM2 P 15 P 16
P 5 18 * allowM2
P 29 18 * allowM2
W 3 16 31 20
S 2 H allowM2 P 17 P 18
P 5 23 * allowM2
P 29 23 * allowM2
W 3 21 31 25
S 2 H allowM2 P 19 P 20
P 5 28 * allowM2
P 29 28 * allowM2
W 3 26 31 30
S 2 H allowM2 P 21 P 22
P 5 33 * allowM2
P 29 33 * allowM2
W 3 31 31 35
S 2 H allowM2 P 23 P 24
P 5 38 * allowM2
P 29 38 * allowM2
W 3 36 31 40
S 2 H allowM2 P 25 P 26
P 5 43 * allowM2
P 29 43 * allowM2
W 3 41 31 45
S 2 H allowM2 P 27 P 28
P 5 65 * metal2
P 29 65 * metal2
W -1 59 35 71
S 6 H metal2 P 29 P 30
P 5 58 * allowM2
P 29 58 * allowM2
W 3 56 31 60
S 2 H allowM2 P 31 P 32
P 5 48 * allowM2
P 29 48 * allowM2
W 3 46 31 50
S 2 H allowM2 P 33 P 34
P 5 53 * nwell
P 29 53 * nwell
W -23 25 57 81
S 28 H nwell P 35 P 36
P 5 53 * allowM2
P 29 53 * allowM2
W 3 51 31 55
S 2 H allowM2 P 37 P 38
P 20 48 * metal
P 26 48 * metal
W 19 47 27 49
S 1 H metal P 39 P 40
P 26 53 * metal
P 26 65 * metal
W 24 51 28 67
S 2 V metal P 41 P 42
P 26 5 * metal
P 26 13 * metal
W 24 3 28 15
S 2 V metal P 43 P 44
P 20 18 * metal
P 26 18 * metal
W 19 17 27 19
S 1 H metal P 45 P 46
P 26 40 * pdif
P 26 60 * pdif
W 23 37 29 63
S 3 V pdif P 47 P 48
P 26 10 * ndif
P 26 20 * ndif
W 23 7 29 23
S 3 V ndif P 49 P 50
P 17 27 * poly
P 23 27 * poly
W 16 26 24 28
S 1 H poly P 51 P 52
P 23 8 * ntrans
P 23 22 * ntrans
W 22 7 24 23
S 1 V ntrans P 53 P 54
P 23 38 * ptrans
P 23 62 * ptrans
W 22 37 24 63
S 1 V ptrans P 55 P 56
P 23 22 * poly
P 23 38 * poly
W 22 21 24 39
S 1 V poly P 57 P 58
P 8 13 * metal
P 8 58 * metal
W 7 12 9 59
S 1 V metal P 59 P 60
P 14 10 * ndif
P 14 20 * ndif
W 11 7 17 23
S 3 V ndif P 61 P 62
P 17 8 * ntrans
P 17 22 * ntrans
W 16 7 18 23
S 1 V ntrans P 63 P 64
P 20 10 * ndif
P 20 20 * ndif
W 17 7 23 23
S 3 V ndif P 65 P 66
P 14 40 * pdif
P 14 60 * pdif
W 11 37 17 63
S 3 V pdif P 67 P 68
P 17 38 * ptrans
P 17 62 * ptrans
W 16 37 18 63
S 1 V ptrans P 69 P 70
P 20 40 * pdif
P 20 60 * pdif
W 17 37 23 63
S 3 V pdif P 71 P 72
P 17 22 * poly
P 17 38 * poly
W 16 21 18 39
S 1 V poly P 73 P 74
P 20 13 * metal
P 20 58 * metal
W 19 12 21 59 o
S 1 V metal P 75 P 76
P 14 5 * metal
P 14 14 * metal
W 12 3 16 16
S 2 V metal P 77 P 78
P 14 58 * metal
P 14 65 * metal
W 12 56 16 67
S 2 V metal P 79 P 80
P 8 5 * metal
P 26 5 * metal
W 6 3 28 7
S 2 H metal P 81 P 82
P 8 65 * metal
P 26 65 * metal
W 6 63 28 67
S 2 H metal P 83 P 84
P 8 49 * pdif
P 8 60 * pdif
W 5 46 11 63
S 3 V pdif P 85 P 86
P 11 47 * ptrans
P 11 62 * ptrans
W 10 46 12 63
S 1 V ptrans P 87 P 88
P 11 8 * ntrans
P 11 17 * ntrans
W 10 7 12 18
S 1 V ntrans P 89 P 90
P 8 10 * ndif
P 8 15 * ndif
W 5 7 11 18
S 3 V ndif P 91 P 92
P 11 17 * poly
P 11 47 * poly
W 10 16 12 48
S 1 V poly P 93 P 94
E
