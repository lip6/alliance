#cell1 ndrvp_fp idps_symb compose *
# 19-Nov-97 16:48 19-Nov-97 16:48 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 53 65
B 5 5 53 65
F F
C vdd 53 65 metal2 6 1 * * E vdd
C vdd 5 65 metal2 6 2 * * W vdd
C vss 5 5 metal2 8 3 * * W vss
C vss 53 5 metal2 8 4 * * E vss
I cpf1 28 31 0 "cpf" ly * *
I cpf2 15 26 0 "cpf" ly * *
I cpf3 9 28 0 "cpf" ly * *
I cdn1 20 18 0 "cdn" ly * *
I cdn2 20 13 0 "cdn" ly * *
I cdp1 20 43 0 "cdp" ly * *
I cdp2 20 48 0 "cdp" ly * *
I cdp3 20 58 0 "cdp" ly * *
I cdp4 20 53 0 "cdp" ly * *
I cdp5 26 53 0 "cdp" ly * *
I cdp6 26 48 0 "cdp" ly * *
I via1 32 65 0 "cvia" ly * *
I via2 26 65 0 "cvia" ly * *
I via3 32 5 0 "cvia" ly * *
I cbp1 29 5 0 "cbp" ly * *
I cbn1 29 65 0 "cbn" ly * *
I cbn2 35 65 0 "cbn" ly * *
I cbn3 23 65 0 "cbn" ly * *
I via4 38 65 0 "cvia" ly * *
I via5 20 65 0 "cvia" ly * *
I cbn4 41 65 0 "cbn" ly * *
I cbp2 35 5 0 "cbp" ly * *
I via6 38 5 0 "cvia" ly * *
I cbp3 41 5 0 "cbp" ly * *
I via7 26 5 0 "cvia" ly * *
I via8 26 5 0 "cvia" ly * *
I cdn3 26 13 0 "cdn" ly * *
I cdp7 26 58 0 "cdp" ly * *
I via9 14 65 0 "cvia" ly * *
I via10 14 5 0 "cvia" ly * *
I via11 8 65 0 "cvia" ly * *
I via12 8 5 0 "cvia" ly * *
I cbp4 11 5 0 "cbp" ly * *
I cbn5 11 65 0 "cbn" ly * *
I cdp8 14 58 0 "cdp" ly * *
I cdp9 8 58 0 "cdp" ly * *
I cdn4 14 13 0 "cdn" ly * *
I cdn5 8 13 0 "cdn" ly * *
I via13 20 5 0 "cvia" ly * *
I cbp5 23 5 0 "cbp" ly * *
I cbp6 17 5 0 "cbp" ly * *
I cbn6 17 65 0 "cbn" ly * *
I via14 44 5 0 "cvia" ly * *
I via15 50 5 0 "cvia" ly * *
I cbp7 47 5 0 "cbp" ly * *
I cbn7 47 65 0 "cbn" ly * *
I via16 44 65 0 "cvia" ly * *
I via17 50 65 0 "cvia" ly * *
I cdn6 38 13 0 "cdn" ly * *
I cdn7 38 18 0 "cdn" ly * *
I cdn8 32 18 0 "cdn" ly * *
I cdn9 26 18 0 "cdn" ly * *
I cdp10 38 43 0 "cdp" ly * *
I cdp11 32 43 0 "cdp" ly * *
I cdp12 26 43 0 "cdp" ly * *
I cdp13 32 53 0 "cdp" ly * *
I cdp14 32 58 0 "cdp" ly * *
I cdp15 38 58 0 "cdp" ly * *
I cdp16 38 53 0 "cdp" ly * *
I cdp17 38 48 0 "cdp" ly * *
I cdp18 50 58 0 "cdp" ly * *
I cdp19 50 48 0 "cdp" ly * *
I cdp20 50 53 0 "cdp" ly * *
I cdp21 50 43 0 "cdp" ly * *
I cdn10 50 18 0 "cdn" ly * *
I cdn11 50 13 0 "cdn" ly * *
I cdn12 44 13 0 "cdn" ly * *
I cdp22 44 48 0 "cdp" ly * *
I o_9 32 13 0 "ref_con" ly * *
I o_2 32 48 0 "ref_con" ly * *
I o_6 50 28 0 "ref_con" ly * *
I i_8 8 18 0 "ref_con" ly * *
I i_7 8 23 0 "ref_con" ly * *
I i_1 8 53 0 "ref_con" ly * *
I i_5 8 33 0 "ref_con" ly * *
I i_4 8 38 0 "ref_con" ly * *
I i_3 8 43 0 "ref_con" ly * *
I i_2 8 48 0 "ref_con" ly * *
I nwell_28 29 53 0 "ref_ref" ly * *
I nono.nwell_28 11 53 0 "ref_ref" ly * *
I o_0 44 58 0 "ref_con" ly * *
I o_1 44 53 0 "ref_con" ly * *
I o_4 44 38 0 "ref_con" ly * *
I o_5 44 33 0 "ref_con" ly * *
I o_7 44 23 0 "ref_con" ly * *
I o_8 44 18 0 "ref_con" ly * *
I o_3 44 43 0 "ref_con" ly * *
P 5 53 * nwell
P 53 53 * nwell
W -23 25 81 81 nwell
S 28 H nwell P 1 P 2
P 5 58 * allowM2
P 53 58 * allowM2
W 3 56 55 60
S 2 H allowM2 P 3 P 4
P 5 65 * metal2
P 53 65 * metal2
W -1 59 59 71 vdd
S 6 H metal2 P 5 P 6
P 5 13 * allowM2
P 53 13 * allowM2
W 3 11 55 15
S 2 H allowM2 P 7 P 8
P 5 18 * allowM2
P 53 18 * allowM2
W 3 16 55 20
S 2 H allowM2 P 9 P 10
P 5 23 * allowM2
P 53 23 * allowM2
W 3 21 55 25
S 2 H allowM2 P 11 P 12
P 5 28 * allowM2
P 53 28 * allowM2
W 3 26 55 30
S 2 H allowM2 P 13 P 14
P 5 33 * allowM2
P 53 33 * allowM2
W 3 31 55 35
S 2 H allowM2 P 15 P 16
P 5 38 * allowM2
P 53 38 * allowM2
W 3 36 55 40
S 2 H allowM2 P 17 P 18
P 5 43 * allowM2
P 53 43 * allowM2
W 3 41 55 45
S 2 H allowM2 P 19 P 20
P 5 48 * allowM2
P 53 48 * allowM2
W 3 46 55 50
S 2 H allowM2 P 21 P 22
P 5 53 * allowM2
P 53 53 * allowM2
W 3 51 55 55
S 2 H allowM2 P 23 P 24
P 5 5 * metal2
P 53 5 * metal2
W -3 -3 61 13
S 8 H metal2 P 25 P 26
P 44 28 * metal
P 50 28 * metal
W 43 27 51 29
S 1 H metal P 27 P 28
P 20 31 * metal
P 27 31 * metal
W 19 30 28 32
S 1 H metal P 29 P 30
P 15 26 * poly
P 23 26 * poly
W 14 25 24 27
S 1 H poly P 31 P 32
P 8 18 * metal
P 8 53 * metal
W 7 17 9 54 i
S 1 V metal P 33 P 34
P 9 28 * poly
P 11 28 * poly
W 7 26 13 30
S 2 H poly P 35 P 36
P 8 28 * metal
P 9 28 * metal
W 7 27 10 29
S 1 H metal P 37 P 38
P 8 58 * metal
P 8 65 * metal
W 6 56 10 67
S 2 V metal P 39 P 40
P 14 13 * metal
P 14 58 * metal
W 13 12 15 59
S 1 V metal P 41 P 42
P 8 5 * metal
P 8 13 * metal
W 6 3 10 15
S 2 V metal P 43 P 44
P 14 10 * ndif
P 14 15 * ndif
W 11 7 17 18
S 3 V ndif P 45 P 46
P 20 13 * metal
P 20 57 * metal
W 19 12 21 58
S 1 V metal P 47 P 48
P 20 40 * pdif
P 20 60 * pdif
W 17 37 23 63
S 3 V pdif P 49 P 50
P 23 22 * poly
P 23 38 * poly
W 22 21 24 39
S 1 V poly P 51 P 52
P 26 40 * pdif
P 26 60 * pdif
W 23 37 29 63
S 3 V pdif P 53 P 54
P 23 38 * ptrans
P 23 62 * ptrans
W 22 37 24 63
S 1 V ptrans P 55 P 56
P 20 10 * ndif
P 20 20 * ndif
W 17 7 23 23
S 3 V ndif P 57 P 58
P 26 10 * ndif
P 26 20 * ndif
W 23 7 29 23
S 3 V ndif P 59 P 60
P 23 8 * ntrans
P 23 22 * ntrans
W 22 7 24 23
S 1 V ntrans P 61 P 62
P 32 10 * ndif
P 32 19 * ndif
W 29 7 35 22
S 3 V ndif P 63 P 64
P 8 5 * metal
P 14 5 * metal
W 6 3 16 7
S 2 H metal P 65 P 66
P 14 12 * ndif
P 14 14 * ndif
W 11 9 17 17
S 3 V ndif P 67 P 68
P 8 12 * ndif
P 8 14 * ndif
W 5 9 11 17
S 3 V ndif P 69 P 70
P 29 39 * ptrans
P 29 62 * ptrans
W 28 38 30 63
S 1 V ptrans P 71 P 72
P 11 47 * ptrans
P 11 62 * ptrans
W 10 46 12 63
S 1 V ptrans P 73 P 74
P 29 8 * ntrans
P 29 21 * ntrans
W 28 7 30 22
S 1 V ntrans P 75 P 76
P 11 8 * ntrans
P 11 17 * ntrans
W 10 7 12 18
S 1 V ntrans P 77 P 78
P 29 21 * poly
P 29 39 * poly
W 28 20 30 40
S 1 V poly P 79 P 80
P 11 17 * poly
P 11 47 * poly
W 10 16 12 48
S 1 V poly P 81 P 82
P 8 10 * ndif
P 8 15 * ndif
W 5 7 11 18
S 3 V ndif P 83 P 84
P 35 21 * poly
P 35 39 * poly
W 34 20 36 40
S 1 V poly P 85 P 86
P 35 39 * ptrans
P 35 62 * ptrans
W 34 38 36 63
S 1 V ptrans P 87 P 88
P 35 8 * ntrans
P 35 21 * ntrans
W 34 7 36 22
S 1 V ntrans P 89 P 90
P 41 21 * poly
P 41 39 * poly
W 40 20 42 40
S 1 V poly P 91 P 92
P 41 39 * ptrans
P 41 62 * ptrans
W 40 38 42 63
S 1 V ptrans P 93 P 94
P 41 8 * ntrans
P 41 21 * ntrans
W 40 7 42 22
S 1 V ntrans P 95 P 96
P 47 21 * poly
P 47 39 * poly
W 46 20 48 40
S 1 V poly P 97 P 98
P 50 41 * pdif
P 50 60 * pdif
W 47 38 53 63
S 3 V pdif P 99 P 100
P 47 39 * ptrans
P 47 62 * ptrans
W 46 38 48 63
S 1 V ptrans P 101 P 102
P 50 10 * ndif
P 50 19 * ndif
W 47 7 53 22
S 3 V ndif P 103 P 104
P 47 8 * ntrans
P 47 21 * ntrans
W 46 7 48 22
S 1 V ntrans P 105 P 106
P 26 5 * metal
P 26 18 * metal
W 24 3 28 20
S 2 V metal P 107 P 108
P 29 28 * poly
P 47 28 * poly
W 28 27 48 29
S 1 H poly P 109 P 110
P 38 43 * metal
P 38 65 * metal
W 36 41 40 67
S 2 V metal P 111 P 112
P 44 13 * metal
P 44 58 * metal
W 43 12 45 59 o
S 1 V metal P 113 P 114
P 50 43 * metal
P 50 65 * metal
W 48 41 52 67
S 2 V metal P 115 P 116
P 38 5 * metal
P 38 18 * metal
W 36 3 40 20
S 2 V metal P 117 P 118
P 50 5 * metal
P 50 18 * metal
W 48 3 52 20
S 2 V metal P 119 P 120
P 32 13 * metal
P 32 58 * metal
W 31 12 33 59
S 1 V metal P 121 P 122
P 26 43 * metal
P 26 65 * metal
W 24 41 28 67
S 2 V metal P 123 P 124
P 32 28 * metal
P 44 28 * metal
W 31 27 45 29
S 1 H metal P 125 P 126
P 8 65 * metal
P 50 65 * metal
W 6 63 52 67
S 2 H metal P 127 P 128
P 8 5 * metal
P 50 5 * metal
W 6 3 52 7
S 2 H metal P 129 P 130
P 44 41 * pdif
P 44 60 * pdif
W 41 38 47 63
S 3 V pdif P 131 P 132
P 38 41 * pdif
P 38 60 * pdif
W 35 38 41 63
S 3 V pdif P 133 P 134
P 32 41 * pdif
P 32 60 * pdif
W 29 38 35 63
S 3 V pdif P 135 P 136
P 26 41 * pdif
P 26 60 * pdif
W 23 38 29 63
S 3 V pdif P 137 P 138
P 14 49 * pdif
P 14 60 * pdif
W 11 46 17 63
S 3 V pdif P 139 P 140
P 8 49 * pdif
P 8 60 * pdif
W 5 46 11 63
S 3 V pdif P 141 P 142
P 38 10 * ndif
P 38 19 * ndif
W 35 7 41 22
S 3 V ndif P 143 P 144
P 44 10 * ndif
P 44 19 * ndif
W 41 7 47 22
S 3 V ndif P 145 P 146
P 26 10 * ndif
P 26 19 * ndif
W 23 7 29 22
S 3 V ndif P 147 P 148
E
