#cell1 no2p_fp idps_symb compose *
# 19-Nov-97 17:19 19-Nov-97 17:19 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 41 65
B 5 5 41 65
F F
C vss 5 5 metal2 8 1 * * W vss
C vss 41 5 metal2 8 2 * * E vss
C vdd 5 65 metal2 6 3 * * W vdd
C vdd 41 65 metal2 6 4 * * E vdd
I cdp1 20 50 0 "cdp" ly * *
I cdn1 14 18 0 "cdn" ly * *
I cbn1 23 65 0 "cbn" ly * *
I cbp1 23 5 0 "cbp" ly * *
I cdn2 14 13 0 "cdn" ly * *
I cdn3 20 13 0 "cdn" ly * *
I cdn4 8 13 0 "cdn" ly * *
I cdp2 14 58 0 "cdp" ly * *
I cdp3 8 53 0 "cdp" ly * *
I cbn2 17 65 0 "cbn" ly * *
I cbn3 11 65 0 "cbn" ly * *
I cbp2 17 5 0 "cbp" ly * *
I cbp3 11 5 0 "cbp" ly * *
I cpf1 13 23 0 "cpf" ly * *
I cpf2 15 28 0 "cpf" ly * *
I via1 14 5 0 "cvia" ly * *
I via2 8 5 0 "cvia" ly * *
I via3 20 65 0 "cvia" ly * *
I via4 20 5 0 "cvia" ly * *
I via5 14 65 0 "cvia" ly * *
I via6 8 65 0 "cvia" ly * *
I via7 38 5 0 "cvia" ly * *
I via8 38 65 0 "cvia" ly * *
I via9 32 5 0 "cvia" ly * *
I via10 32 65 0 "cvia" ly * *
I via11 26 65 0 "cvia" ly * *
I via12 26 5 0 "cvia" ly * *
I cpf3 37 28 0 "cpf" ly * *
I cpf4 27 33 0 "cpf" ly * *
I cbp4 29 5 0 "cbp" ly * *
I cbp5 35 5 0 "cbp" ly * *
I cbn4 29 65 0 "cbn" ly * *
I cbn5 35 65 0 "cbn" ly * *
I cdp4 38 58 0 "cdp" ly * *
I cdp5 26 58 0 "cdp" ly * *
I cdn5 26 13 0 "cdn" ly * *
I cdn6 32 13 0 "cdn" ly * *
I cdn7 38 13 0 "cdn" ly * *
I i1_1 38 53 0 "ref_con" ly * *
I o_2 8 48 0 "ref_con" ly * *
I nwell_28 23 53 0 "ref_ref" ly * *
I o_0 8 58 0 "ref_con" ly * *
I i1_3 38 43 0 "ref_con" ly * *
I i1_4 38 38 0 "ref_con" ly * *
I i0_2 26 48 0 "ref_con" ly * *
I i0_3 26 43 0 "ref_con" ly * *
I i0_4 26 38 0 "ref_con" ly * *
I i0_1 26 53 0 "ref_con" ly * *
I i1_5 38 33 0 "ref_con" ly * *
I i0_7 26 23 0 "ref_con" ly * *
I i0_6 26 28 0 "ref_con" ly * *
I i1_7 38 23 0 "ref_con" ly * *
I i1_2 38 48 0 "ref_con" ly * *
I i1_8 38 18 0 "ref_con" ly * *
I i0_8 26 18 0 "ref_con" ly * *
I o_3 8 43 0 "ref_con" ly * *
I o_4 8 38 0 "ref_con" ly * *
I o_5 8 33 0 "ref_con" ly * *
I o_6 8 28 0 "ref_con" ly * *
I o_7 8 23 0 "ref_con" ly * *
I o_8 8 18 0 "ref_con" ly * *
P 38 28 * metal
P 38 53 * metal
W 37 27 39 54 i1
S 1 V metal P 1 P 2
P 38 58 * metal
P 38 65 * metal
W 36 56 40 67
S 2 V metal P 3 P 4
P 26 58 * metal
P 26 59 * metal
W 25 57 27 60
S 1 V metal P 5 P 6
P 20 54 * metal
P 20 59 * metal
W 19 53 21 60
S 1 V metal P 7 P 8
P 20 59 * metal
P 32 59 * metal
W 19 58 33 60
S 1 H metal P 9 P 10
P 20 13 * metal
P 20 50 * metal
W 19 12 21 51
S 1 V metal P 11 P 12
P 14 28 * metal
P 14 54 * metal
W 13 27 15 55
S 1 V metal P 13 P 14
P 14 54 * metal
P 20 54 * metal
W 13 53 21 55
S 1 H metal P 15 P 16
P 35 28 * poly
P 37 28 * poly
W 34 27 38 29
S 1 H poly P 17 P 18
P 27 33 * poly
P 29 33 * poly
W 26 32 30 34
S 1 H poly P 19 P 20
P 11 23 * poly
P 13 23 * poly
W 10 22 14 24
S 1 H poly P 21 P 22
P 15 28 * poly
P 17 28 * poly
W 14 27 18 29
S 1 H poly P 23 P 24
P 14 28 * metal
P 15 28 * metal
W 13 27 16 29
S 1 H metal P 25 P 26
P 13 23 * metal
P 20 23 * metal
W 12 22 21 24
S 1 H metal P 27 P 28
P 8 13 * metal
P 8 58 * metal
W 7 12 9 59 o
S 1 V metal P 29 P 30
P 23 39 * nwell
P 23 67 * nwell
W -13 3 59 103
S 36 V nwell P 31 P 32
P 17 17 * poly
P 17 47 * poly
W 16 16 18 48
S 1 V poly P 33 P 34
P 5 13 * allowM2
P 41 13 * allowM2
W 3 11 43 15
S 2 H allowM2 P 35 P 36
P 5 18 * allowM2
P 41 18 * allowM2
W 3 16 43 20
S 2 H allowM2 P 37 P 38
P 5 23 * allowM2
P 41 23 * allowM2
W 3 21 43 25
S 2 H allowM2 P 39 P 40
P 5 28 * allowM2
P 41 28 * allowM2
W 3 26 43 30
S 2 H allowM2 P 41 P 42
P 5 33 * allowM2
P 41 33 * allowM2
W 3 31 43 35
S 2 H allowM2 P 43 P 44
P 5 38 * allowM2
P 41 38 * allowM2
W 3 36 43 40
S 2 H allowM2 P 45 P 46
P 5 43 * allowM2
P 41 43 * allowM2
W 3 41 43 45
S 2 H allowM2 P 47 P 48
P 5 48 * allowM2
P 41 48 * allowM2
W 3 46 43 50
S 2 H allowM2 P 49 P 50
P 5 53 * allowM2
P 41 53 * allowM2
W 3 51 43 55
S 2 H allowM2 P 51 P 52
P 5 58 * allowM2
P 41 58 * allowM2
W 3 56 43 60
S 2 H allowM2 P 53 P 54
P 20 10 * ndif
P 20 15 * ndif
W 17 7 23 18
S 3 V ndif P 55 P 56
P 17 8 * ntrans
P 17 17 * ntrans
W 16 7 18 18
S 1 V ntrans P 57 P 58
P 20 49 * pdif
P 20 60 * pdif
W 17 46 23 63
S 3 V pdif P 59 P 60
P 17 47 * ptrans
P 17 62 * ptrans
W 16 46 18 63
S 1 V ptrans P 61 P 62
P 14 58 * metal
P 14 65 * metal
W 12 56 16 67
S 2 V metal P 63 P 64
P 26 33 * metal
P 27 33 * metal
W 25 32 28 34 i0
S 1 H metal P 65 P 66
P 26 18 * metal
P 26 33 * metal
W 25 17 27 34 i0
S 1 V metal P 67 P 68
P 26 33 * metal
P 26 53 * metal
W 25 32 27 54 i0
S 1 V metal P 69 P 70
P 37 28 * metal
P 38 28 * metal
W 36 27 39 29 i1
S 1 H metal P 71 P 72
P 38 18 * metal
P 38 28 * metal
W 37 17 39 29 i1
S 1 V metal P 73 P 74
P 8 5 * metal
P 38 5 * metal
W 6 3 40 7
S 2 H metal P 75 P 76
P 8 65 * metal
P 38 65 * metal
W 6 63 40 67
S 2 H metal P 77 P 78
P 5 5 * metal2
P 41 5 * metal2
W -3 -3 49 13 vss
S 8 H metal2 P 79 P 80
P 5 65 * metal2
P 41 65 * metal2
W -1 59 47 71 vdd
S 6 H metal2 P 81 P 82
P 14 5 * metal
P 14 18 * metal
W 12 3 16 20
S 2 V metal P 83 P 84
P 29 17 * poly
P 29 38 * poly
W 28 16 30 39
S 1 V poly P 85 P 86
P 35 17 * poly
P 35 38 * poly
W 34 16 36 39
S 1 V poly P 87 P 88
P 29 8 * ntrans
P 29 17 * ntrans
W 28 7 30 18
S 1 V ntrans P 89 P 90
P 35 8 * ntrans
P 35 17 * ntrans
W 34 7 36 18
S 1 V ntrans P 91 P 92
P 26 10 * ndif
P 26 15 * ndif
W 23 7 29 18
S 3 V ndif P 93 P 94
P 38 10 * ndif
P 38 15 * ndif
W 35 7 41 18
S 3 V ndif P 95 P 96
P 32 10 * ndif
P 32 15 * ndif
W 30 8 34 17
S 2 V ndif P 97 P 98
P 26 5 * metal
P 26 13 * metal
W 24 3 28 15
S 2 V metal P 99 P 100
P 38 5 * metal
P 38 13 * metal
W 36 3 40 15
S 2 V metal P 101 P 102
P 32 13 * metal
P 32 59 * metal
W 31 12 33 60
S 1 V metal P 103 P 104
P 29 38 * ptrans
P 29 62 * ptrans
W 28 37 30 63
S 1 V ptrans P 105 P 106
P 35 38 * ptrans
P 35 62 * ptrans
W 34 37 36 63
S 1 V ptrans P 107 P 108
P 26 40 * pdif
P 26 60 * pdif
W 23 37 29 63
S 3 V pdif P 109 P 110
P 38 40 * pdif
P 38 60 * pdif
W 35 37 41 63
S 3 V pdif P 111 P 112
P 11 22 * poly
P 11 38 * poly
W 10 21 12 39
S 1 V poly P 113 P 114
P 11 8 * ntrans
P 11 22 * ntrans
W 10 7 12 23
S 1 V ntrans P 115 P 116
P 11 38 * ptrans
P 11 62 * ptrans
W 10 37 12 63
S 1 V ptrans P 117 P 118
P 8 40 * pdif
P 8 60 * pdif
W 5 37 11 63
S 3 V pdif P 119 P 120
P 14 40 * pdif
P 14 60 * pdif
W 11 37 17 63
S 3 V pdif P 121 P 122
P 8 10 * ndif
P 8 20 * ndif
W 5 7 11 23
S 3 V ndif P 123 P 124
P 14 10 * ndif
P 14 20 * ndif
W 11 7 17 23
S 3 V ndif P 125 P 126
P 32 40 * pdif
P 32 60 * pdif
W 30 38 34 62
S 2 V pdif P 127 P 128
E
