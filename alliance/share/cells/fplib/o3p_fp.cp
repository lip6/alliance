#cell1 o3p_fp idps_symb compose *
# 19-Nov-97 17:13 19-Nov-97 17:13 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 35 65
B 5 5 35 65
F F
C vss 35 5 metal2 8 1 * * E vss
C vss 5 5 metal2 8 2 * * W vss
C vdd 35 65 metal2 6 3 * * E vdd
C vdd 5 65 metal2 6 4 * * W vdd
I cdn1 26 18 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdn3 14 13 0 "cdn" ly * *
I cpf1 9 23 0 "cpf" ly * *
I cbp1 11 5 0 "cbp" ly * *
I via1 8 5 0 "cvia" ly * *
I via2 8 65 0 "cvia" ly * *
I cbn1 11 65 0 "cbn" ly * *
I cdp1 8 58 0 "cdp" ly * *
I cpf2 27 23 0 "cpf" ly * *
I via3 32 5 0 "cvia" ly * *
I cbp2 29 5 0 "cbp" ly * *
I via4 32 65 0 "cvia" ly * *
I cdp2 32 53 0 "cdp" ly * *
I cdn4 32 18 0 "cdn" ly * *
I cbn2 29 65 0 "cbn" ly * *
I cbn3 23 65 0 "cbn" ly * *
I cdn5 8 13 0 "cdn" ly * *
I cdn6 26 13 0 "cdn" ly * *
I via5 14 5 0 "cvia" ly * *
I via6 26 65 0 "cvia" ly * *
I via7 14 65 0 "cvia" ly * *
I cbn4 17 65 0 "cbn" ly * *
I cdp3 26 58 0 "cdp" ly * *
I cpf3 25 33 0 "cpf" ly * *
I cpf4 15 28 0 "cpf" ly * *
I cbp3 17 5 0 "cbp" ly * *
I cbp4 23 5 0 "cbp" ly * *
I via8 26 5 0 "cvia" ly * *
I via9 20 65 0 "cvia" ly * *
I via10 20 5 0 "cvia" ly * *
I no_8 20 18 0 "ref_con" ly * *
I no_7 20 23 0 "ref_con" ly * *
I no_6 20 28 0 "ref_con" ly * *
I no_5 20 33 0 "ref_con" ly * *
I no_4 20 38 0 "ref_con" ly * *
I no_3 20 43 0 "ref_con" ly * *
I no_2 20 48 0 "ref_con" ly * *
I no_1 20 53 0 "ref_con" ly * *
I no_0 20 58 0 "ref_con" ly * *
I o_3 32 43 0 "ref_con" ly * *
I i2_1 26 53 0 "ref_con" ly * *
I i0_1 8 53 0 "ref_con" ly * *
I i0_2 8 48 0 "ref_con" ly * *
I i2_2 26 48 0 "ref_con" ly * *
I i1_2 14 48 0 "ref_con" ly * *
I i1_1 14 53 0 "ref_con" ly * *
I i0_5 8 33 0 "ref_con" ly * *
I i1_7 14 23 0 "ref_con" ly * *
I i0_6 8 28 0 "ref_con" ly * *
I i0_4 8 38 0 "ref_con" ly * *
I i0_3 8 43 0 "ref_con" ly * *
I nwell_28 23 53 0 "ref_ref" ly * *
I o_0 32 58 0 "ref_con" ly * *
I o_9 32 13 0 "ref_con" ly * *
I o_7 32 23 0 "ref_con" ly * *
I o_6 32 28 0 "ref_con" ly * *
I o_5 32 33 0 "ref_con" ly * *
I o_4 32 38 0 "ref_con" ly * *
I o_2 32 48 0 "ref_con" ly * *
I i1_3 14 43 0 "ref_con" ly * *
I i2_4 26 38 0 "ref_con" ly * *
I i1_4 14 38 0 "ref_con" ly * *
I i2_3 26 43 0 "ref_con" ly * *
I i1_5 14 33 0 "ref_con" ly * *
I i2_6 26 28 0 "ref_con" ly * *
P 17 8 * ntrans
P 17 17 * ntrans
W 16 7 18 18
S 1 V ntrans P 1 P 2
P 23 8 * ntrans
P 23 17 * ntrans
W 22 7 24 18
S 1 V ntrans P 3 P 4
P 26 10 * ndif
P 26 20 * ndif
W 23 7 29 23
S 3 V ndif P 5 P 6
P 23 38 * ptrans
P 23 62 * ptrans
W 22 37 24 63
S 1 V ptrans P 7 P 8
P 17 38 * ptrans
P 17 62 * ptrans
W 16 37 18 63
S 1 V ptrans P 9 P 10
P 26 40 * pdif
P 26 60 * pdif
W 23 37 29 63
S 3 V pdif P 11 P 12
P 14 40 * pdif
P 14 60 * pdif
W 11 37 17 63
S 3 V pdif P 13 P 14
P 14 65 * metal
P 26 65 * metal
W 12 63 28 67
S 2 H metal P 15 P 16
P 5 53 * nwell
P 35 53 * nwell
W -23 25 63 81
S 28 H nwell P 17 P 18
P 20 40 * pdif
P 20 60 * pdif
W 18 38 22 62
S 2 V pdif P 19 P 20
P 5 65 * metal2
P 35 65 * metal2
W -1 59 41 71
S 6 H metal2 P 21 P 22
P 5 5 * metal2
P 35 5 * metal2
W -3 -3 43 13
S 8 H metal2 P 23 P 24
P 5 13 * allowM2
P 35 13 * allowM2
W 3 11 37 15
S 2 H allowM2 P 25 P 26
P 5 18 * allowM2
P 35 18 * allowM2
W 3 16 37 20
S 2 H allowM2 P 27 P 28
P 5 23 * allowM2
P 35 23 * allowM2
W 3 21 37 25
S 2 H allowM2 P 29 P 30
P 5 28 * allowM2
P 35 28 * allowM2
W 3 26 37 30
S 2 H allowM2 P 31 P 32
P 5 33 * allowM2
P 35 33 * allowM2
W 3 31 37 35
S 2 H allowM2 P 33 P 34
P 5 43 * allowM2
P 35 43 * allowM2
W 3 41 37 45
S 2 H allowM2 P 35 P 36
P 5 48 * allowM2
P 35 48 * allowM2
W 3 46 37 50
S 2 H allowM2 P 37 P 38
P 5 53 * allowM2
P 35 53 * allowM2
W 3 51 37 55
S 2 H allowM2 P 39 P 40
P 5 38 * allowM2
P 35 38 * allowM2
W 3 36 37 40
S 2 H allowM2 P 41 P 42
P 5 58 * allowM2
P 35 58 * allowM2
W 3 56 37 60
S 2 H allowM2 P 43 P 44
P 14 10 * ndif
P 14 15 * ndif
W 11 7 17 18
S 3 V ndif P 45 P 46
P 14 28 * metal
P 14 53 * metal
W 13 27 15 54 i1
S 1 V metal P 47 P 48
P 14 23 * metal
P 14 28 * metal
W 13 22 15 29 i1
S 1 V metal P 49 P 50
P 14 28 * metal
P 15 28 * metal
W 13 27 16 29 i1
S 1 H metal P 51 P 52
P 26 33 * metal
P 26 53 * metal
W 25 32 27 54 i2
S 1 V metal P 53 P 54
P 26 28 * metal
P 26 33 * metal
W 25 27 27 34 i2
S 1 V metal P 55 P 56
P 25 33 * metal
P 26 33 * metal
W 24 32 27 34 i2
S 1 H metal P 57 P 58
P 23 33 * poly
P 23 38 * poly
W 22 32 24 39
S 1 V poly P 59 P 60
P 23 17 * poly
P 23 33 * poly
W 22 16 24 34
S 1 V poly P 61 P 62
P 23 33 * poly
P 25 33 * poly
W 21 31 27 35
S 2 H poly P 63 P 64
P 17 28 * poly
P 17 38 * poly
W 16 27 18 39
S 1 V poly P 65 P 66
P 17 17 * poly
P 17 28 * poly
W 16 16 18 29
S 1 V poly P 67 P 68
P 15 28 * poly
P 17 28 * poly
W 13 26 19 30
S 2 H poly P 69 P 70
P 20 10 * ndif
P 20 15 * ndif
W 18 8 22 17
S 2 V ndif P 71 P 72
P 29 8 * ntrans
P 29 22 * ntrans
W 28 7 30 23
S 1 V ntrans P 73 P 74
P 29 38 * ptrans
P 29 62 * ptrans
W 28 37 30 63
S 1 V ptrans P 75 P 76
P 32 13 * metal
P 32 58 * metal
W 31 12 33 59 o
S 1 V metal P 77 P 78
P 32 40 * pdif
P 32 60 * pdif
W 29 37 35 63
S 3 V pdif P 79 P 80
P 26 58 * metal
P 26 65 * metal
W 24 56 28 67
S 2 V metal P 81 P 82
P 26 65 * metal
P 32 65 * metal
W 24 63 34 67
S 2 H metal P 83 P 84
P 32 10 * ndif
P 32 20 * ndif
W 29 7 35 23
S 3 V ndif P 85 P 86
P 11 8 * ntrans
P 11 17 * ntrans
W 10 7 12 18
S 1 V ntrans P 87 P 88
P 8 10 * ndif
P 8 15 * ndif
W 5 7 11 18
S 3 V ndif P 89 P 90
P 8 40 * pdif
P 8 60 * pdif
W 5 37 11 63
S 3 V pdif P 91 P 92
P 11 38 * ptrans
P 11 62 * ptrans
W 10 37 12 63
S 1 V ptrans P 93 P 94
P 8 23 * metal
P 8 53 * metal
W 7 22 9 54 i0
S 1 V metal P 95 P 96
P 8 23 * metal
P 8 23 * metal
W 7 22 9 24 i0
S 1 H metal P 97 P 98
P 8 23 * metal
P 9 23 * metal
W 7 22 10 24 i0
S 1 H metal P 99 P 100
P 11 23 * poly
P 11 38 * poly
W 10 22 12 39
S 1 V poly P 101 P 102
P 11 17 * poly
P 11 23 * poly
W 10 16 12 24
S 1 V poly P 103 P 104
P 9 23 * poly
P 11 23 * poly
W 8 22 12 24
S 1 H poly P 105 P 106
P 29 23 * poly
P 29 38 * poly
W 28 22 30 39
S 1 V poly P 107 P 108
P 29 22 * poly
P 29 23 * poly
W 28 21 30 24
S 1 V poly P 109 P 110
P 27 23 * poly
P 29 23 * poly
W 26 22 30 24
S 1 H poly P 111 P 112
P 8 65 * metal
P 14 65 * metal
W 6 63 16 67
S 2 H metal P 113 P 114
P 26 5 * metal
P 32 5 * metal
W 24 3 34 7
S 2 H metal P 115 P 116
P 26 5 * metal
P 26 18 * metal
W 24 3 28 20
S 2 V metal P 117 P 118
P 14 5 * metal
P 26 5 * metal
W 12 3 28 7
S 2 H metal P 119 P 120
P 8 5 * metal
P 14 5 * metal
W 6 3 16 7
S 2 H metal P 121 P 122
P 14 5 * metal
P 14 13 * metal
W 12 3 16 15
S 2 V metal P 123 P 124
P 8 13 * metal
P 8 17 * metal
W 7 12 9 18
S 1 V metal P 125 P 126
P 8 17 * metal
P 20 17 * metal
W 7 16 21 18
S 1 H metal P 127 P 128
P 20 13 * metal
P 20 17 * metal
W 19 12 21 18
S 1 V metal P 129 P 130
P 20 17 * metal
P 20 17 * metal
W 19 16 21 18
S 1 H metal P 131 P 132
P 20 17 * metal
P 20 22 * metal
W 19 16 21 23
S 1 V metal P 133 P 134
P 20 22 * metal
P 27 22 * metal
W 19 21 28 23
S 1 H metal P 135 P 136
P 20 22 * metal
P 20 59 * metal
W 19 21 21 60
S 1 V metal P 137 P 138
P 8 59 * metal
P 20 59 * metal
W 7 58 21 60
S 1 H metal P 139 P 140
P 8 58 * metal
P 8 59 * metal
W 7 57 9 60
S 1 V metal P 141 P 142
P 27 22 * metal
P 27 23 * metal
W 26 21 28 24
S 1 V metal P 143 P 144
E
