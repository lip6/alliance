#cell1 nxr2_fp idps_symb compose *
# 20-Nov-97 12:08 20-Nov-97 12:08 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 51 65
B 5 5 51 65
F F
C vss 5 5 metal2 8 1 * * W vss
C vdd 5 65 metal2 6 2 * * W vdd
C vdd 51 65 metal2 6 3 * * E vdd
C vss 51 5 metal2 8 4 * * E vss
I cpf1 19 37 0 "cpf" ly * *
I cpf2 12 43 0 "cpf" ly * *
I cdp1 42 47 0 "cdp" ly * *
I cdp2 42 53 0 "cdp" ly * *
I cpf3 37 29 0 "cpf" ly * *
I cpf4 43 33 0 "cpf" ly * *
I via1 42 28 0 "cvia" ly * *
I via2 8 28 0 "cvia" ly * *
I cdp3 20 53 0 "cdp" ly * *
I cdp4 20 58 0 "cdp" ly * *
I cdp5 14 53 0 "cdp" ly * *
I cdp6 14 58 0 "cdp" ly * *
I cdn1 42 18 0 "cdn" ly * *
I cdn2 36 18 0 "cdn" ly * *
I cdn3 30 12 0 "cdn" ly * *
I cdn4 30 18 0 "cdn" ly * *
I cdn5 42 12 0 "cdn" ly * *
I cdn6 36 12 0 "cdn" ly * *
I cdp7 48 58 0 "cdp" ly * *
I cdp8 48 53 0 "cdp" ly * *
I cdp9 30 58 0 "cdp" ly * *
I cdp10 30 53 0 "cdp" ly * *
I cdp11 30 47 0 "cdp" ly * *
I cdp12 30 42 0 "cdp" ly * *
I cdp13 42 42 0 "cdp" ly * *
I cdp14 42 58 0 "cdp" ly * *
I cdn7 48 18 0 "cdn" ly * *
I cbp1 29 5 0 "cbp" ly * *
I via3 8 65 0 "cvia" ly * *
I via4 14 5 0 "cvia" ly * *
I cbn1 17 65 0 "cbn" ly * *
I via5 14 65 0 "cvia" ly * *
I via6 20 5 0 "cvia" ly * *
I via7 20 65 0 "cvia" ly * *
I cbn2 23 65 0 "cbn" ly * *
I via8 8 5 0 "cvia" ly * *
I via9 26 5 0 "cvia" ly * *
I cbp2 11 5 0 "cbp" ly * *
I cbp3 17 5 0 "cbp" ly * *
I cbp4 23 5 0 "cbp" ly * *
I cbn3 11 65 0 "cbn" ly * *
I via10 26 65 0 "cvia" ly * *
I cbn4 29 65 0 "cbn" ly * *
I cdn8 8 12 0 "cdn" ly * *
I cdp15 8 58 0 "cdp" ly * *
I cdp16 8 53 0 "cdp" ly * *
I cdn9 20 12 0 "cdn" ly * *
I via11 32 5 0 "cvia" ly * *
I via12 38 5 0 "cvia" ly * *
I via13 44 5 0 "cvia" ly * *
I via14 32 65 0 "cvia" ly * *
I via15 38 65 0 "cvia" ly * *
I via16 44 65 0 "cvia" ly * *
I cbn5 35 65 0 "cbn" ly * *
I cbn6 41 65 0 "cbn" ly * *
I cbn7 47 65 0 "cbn" ly * *
I cbp5 47 5 0 "cbp" ly * *
I cbp6 41 5 0 "cbp" ly * *
I cbp7 35 5 0 "cbp" ly * *
I cpf5 19 24 0 "cpf" ly * *
I cdn10 8 19 0 "cdn" ly * *
I cdn11 20 18 0 "cdn" ly * *
I cpf6 20 29 0 "cpf" ly * *
I o_0 48 13 0 "ref_con" ly * *
I o_6 48 43 0 "ref_con" ly * *
I i0_2 14 23 0 "ref_con" ly * *
I i0_0 14 13 0 "ref_con" ly * *
I i0_1 14 18 0 "ref_con" ly * *
I i0_4 14 33 0 "ref_con" ly * *
I i0_5 14 38 0 "ref_con" ly * *
I i0_6 20 43 0 "ref_con" ly * *
I i1_0 25 13 0 "ref_con" ly * *
I i1_1 25 18 0 "ref_con" ly * *
I i1_2 25 23 0 "ref_con" ly * *
I i1_4 25 33 0 "ref_con" ly * *
I i1_5 25 38 0 "ref_con" ly * *
I i1_6 25 43 0 "ref_con" ly * *
I i1_7 25 48 0 "ref_con" ly * *
I i1_8 25 53 0 "ref_con" ly * *
I i1_9 25 58 0 "ref_con" ly * *
I nwell_28 50 53 0 "ref_ref" ly * *
I o_7 48 48 0 "ref_con" ly * *
I o_5 48 38 0 "ref_con" ly * *
I o_4 48 33 0 "ref_con" ly * *
I o_3 48 28 0 "ref_con" ly * *
I o_2 48 23 0 "ref_con" ly * *
P 19 37 * poly
P 17 37 * poly
W 16 36 20 38
S 1 H poly P 1 P 2
P 19 37 * metal
P 25 37 * metal
W 18 36 26 38
S 1 H metal P 3 P 4
P 14 43 * metal
P 12 43 * metal
W 10 41 16 45
S 2 H metal P 5 P 6
P 25 58 * metal
P 25 13 * metal
W 24 12 26 59 i1
S 1 V metal P 7 P 8
P 14 43 * metal
P 14 13 * metal
W 13 12 15 44 i0
S 1 V metal P 9 P 10
P 17 37 * poly
P 17 47 * poly
W 16 36 18 48
S 1 V poly P 11 P 12
P 37 29 * metal
P 25 29 * metal
W 24 28 38 30
S 1 H metal P 13 P 14
P 14 43 * metal
P 20 43 * metal
W 13 42 21 44
S 1 H metal P 15 P 16
P 37 29 * poly
P 39 29 * poly
W 36 28 40 30
S 1 H poly P 17 P 18
P 42 28 * metal2
P 8 28 * metal2
W 6 26 44 30
S 2 H metal2 P 19 P 20
P 42 33 * metal
P 42 28 * metal
W 40 26 44 35
S 2 V metal P 21 P 22
P 43 33 * metal
P 42 33 * metal
W 40 31 45 35
S 2 H metal P 23 P 24
P 43 33 * poly
P 45 33 * poly
W 40 30 48 36
S 3 H poly P 25 P 26
P 8 58 * metal
P 8 12 * metal
W 7 11 9 59
S 1 V metal P 27 P 28
P 20 49 * metal
P 8 49 * metal
W 7 48 21 50
S 1 H metal P 29 P 30
P 20 58 * metal
P 20 49 * metal
W 19 48 21 59
S 1 V metal P 31 P 32
P 14 53 * metal
P 14 65 * metal
W 12 51 16 67
S 2 V metal P 33 P 34
P 42 22 * metal
P 42 12 * metal
W 41 11 43 23
S 1 V metal P 35 P 36
P 30 22 * metal
P 42 22 * metal
W 29 21 43 23
S 1 H metal P 37 P 38
P 30 12 * metal
P 30 22 * metal
W 29 11 31 23
S 1 V metal P 39 P 40
P 36 18 * metal
P 36 5 * metal
W 34 3 38 20
S 2 V metal P 41 P 42
P 30 42 * metal
P 30 65 * metal
W 28 40 32 67
S 2 V metal P 43 P 44
P 48 53 * metal
P 48 65 * metal
W 46 51 50 67
S 2 V metal P 45 P 46
P 42 42 * metal
P 42 58 * metal
W 41 41 43 59
S 1 V metal P 47 P 48
P 48 42 * metal
P 42 42 * metal
W 41 41 49 43
S 1 H metal P 49 P 50
P 48 48 * metal
P 48 13 * metal
W 47 12 49 49 o
S 1 V metal P 51 P 52
P 45 47 * poly
P 45 23 * poly
W 44 22 46 48
S 1 V poly P 53 P 54
P 48 21 * ndif
P 48 10 * ndif
W 45 7 51 24
S 3 V ndif P 55 P 56
P 45 8 * ntrans
P 45 23 * ntrans
W 44 7 46 24
S 1 V ntrans P 57 P 58
P 48 60 * pdif
P 48 49 * pdif
W 45 46 51 63
S 3 V pdif P 59 P 60
P 45 47 * ptrans
P 45 62 * ptrans
W 44 46 46 63
S 1 V ptrans P 61 P 62
P 8 60 * pdif
P 8 49 * pdif
W 5 46 11 63
S 3 V pdif P 63 P 64
P 14 49 * pdif
P 14 60 * pdif
W 11 46 17 63
S 3 V pdif P 65 P 66
P 11 47 * ptrans
P 11 62 * ptrans
W 10 46 12 63
S 1 V ptrans P 67 P 68
P 20 60 * pdif
P 20 49 * pdif
W 17 46 23 63
S 3 V pdif P 69 P 70
P 17 62 * ptrans
P 17 47 * ptrans
W 16 46 18 63
S 1 V ptrans P 71 P 72
P 8 5 * metal
P 23 5 * metal
W 6 3 25 7
S 2 H metal P 73 P 74
P 8 65 * metal
P 23 65 * metal
W 6 63 25 67 vdd
S 2 H metal P 75 P 76
P 39 38 * poly
P 39 23 * poly
W 38 22 40 39
S 1 V poly P 77 P 78
P 33 38 * poly
P 33 23 * poly
W 32 22 34 39
S 1 V poly P 79 P 80
P 30 60 * pdif
P 30 40 * pdif
W 27 37 33 63
S 3 V pdif P 81 P 82
P 39 62 * ptrans
P 39 38 * ptrans
W 38 37 40 63
S 1 V ptrans P 83 P 84
P 33 38 * ptrans
P 33 62 * ptrans
W 32 37 34 63
S 1 V ptrans P 85 P 86
P 42 40 * pdif
P 42 60 * pdif
W 39 37 45 63
S 3 V pdif P 87 P 88
P 36 60 * pdif
P 36 40 * pdif
W 33 37 39 63
S 3 V pdif P 89 P 90
P 30 21 * ndif
P 30 10 * ndif
W 27 7 33 24
S 3 V ndif P 91 P 92
P 42 21 * ndif
P 42 10 * ndif
W 39 7 45 24
S 3 V ndif P 93 P 94
P 39 23 * ntrans
P 39 8 * ntrans
W 38 7 40 24
S 1 V ntrans P 95 P 96
P 33 8 * ntrans
P 33 23 * ntrans
W 32 7 34 24
S 1 V ntrans P 97 P 98
P 36 21 * ndif
P 36 10 * ndif
W 33 7 39 24
S 3 V ndif P 99 P 100
P 5 53 * nwell
P 51 53 * nwell
W -23 25 79 81 nwell
S 28 H nwell P 101 P 102
P 5 65 * metal2
P 51 65 * metal2
W -1 59 57 71 vdd
S 6 H metal2 P 103 P 104
P 5 5 * metal2
P 51 5 * metal2
W -3 -3 59 13
S 8 H metal2 P 105 P 106
P 5 13 * allowM2
P 51 13 * allowM2
W 3 11 53 15
S 2 H allowM2 P 107 P 108
P 5 18 * allowM2
P 51 18 * allowM2
W 3 16 53 20
S 2 H allowM2 P 109 P 110
P 5 23 * allowM2
P 51 23 * allowM2
W 3 21 53 25
S 2 H allowM2 P 111 P 112
P 5 33 * allowM2
P 51 33 * allowM2
W 3 31 53 35
S 2 H allowM2 P 113 P 114
P 5 38 * allowM2
P 51 38 * allowM2
W 3 36 53 40
S 2 H allowM2 P 115 P 116
P 5 43 * allowM2
P 51 43 * allowM2
W 3 41 53 45
S 2 H allowM2 P 117 P 118
P 5 48 * allowM2
P 51 48 * allowM2
W 3 46 53 50
S 2 H allowM2 P 119 P 120
P 5 53 * allowM2
P 51 53 * allowM2
W 3 51 53 55
S 2 H allowM2 P 121 P 122
P 5 58 * allowM2
P 51 58 * allowM2
W 3 56 53 60
S 2 H allowM2 P 123 P 124
P 48 28 * allowM2
P 51 28 * allowM2
W 46 26 53 30
S 2 H allowM2 P 125 P 126
P 23 65 * metal
P 47 65 * metal
W 21 63 49 67
S 2 H metal P 127 P 128
P 7 65 * ntie
P 48 65 * ntie
W 4 62 51 68
S 3 H ntie P 129 P 130
P 23 5 * metal
P 48 5 * metal
W 21 3 50 7
S 2 H metal P 131 P 132
P 7 5 * ptie
P 48 5 * ptie
W 4 2 51 8
S 3 H ptie P 133 P 134
P 20 21 * ndif
P 20 10 * ndif
W 17 7 23 24
S 3 V ndif P 135 P 136
P 17 8 * ntrans
P 17 23 * ntrans
W 16 7 18 24
S 1 V ntrans P 137 P 138
P 14 21 * ndif
P 14 10 * ndif
W 11 7 17 24
S 3 V ndif P 139 P 140
P 8 21 * ndif
P 8 10 * ndif
W 5 7 11 24
S 3 V ndif P 141 P 142
P 11 8 * ntrans
P 11 23 * ntrans
W 10 7 12 24
S 1 V ntrans P 143 P 144
P 11 47 * poly
P 11 23 * poly
W 10 22 12 48
S 1 V poly P 145 P 146
P 19 24 * metal
P 25 24 * metal
W 18 23 26 25
S 1 H metal P 147 P 148
P 19 24 * poly
P 17 24 * poly
W 16 23 20 25
S 1 H poly P 149 P 150
P 17 24 * poly
P 17 23 * poly
W 16 22 18 25
S 1 V poly P 151 P 152
P 20 18 * metal
P 20 5 * metal
W 18 3 22 20
S 2 V metal P 153 P 154
P 20 30 * poly
P 20 29 * poly
W 19 28 21 31
S 1 V poly P 155 P 156
P 14 29 * metal
P 20 29 * metal
W 13 28 21 30
S 1 H metal P 157 P 158
P 20 30 * poly
P 33 30 * poly
W 19 29 34 31
S 1 H poly P 159 P 160
E
