#cell1 nxr2x_fp idps_symb compose *
# 20-Nov-97 12:12 20-Nov-97 12:12 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 49 65
B 5 5 49 65
F F
C vdd 5 65 metal2 6 1 * * W vdd
C vss 5 5 metal2 8 2 * * W vss
C i1 24 65 metal 1 3 * * N i1
C i1 24 5 metal 1 4 * * S i1
C vss 49 5 metal2 8 5 * * E vss
C vdd 49 65 metal2 6 6 * * E vdd
I cpf1 20 29 0 "cpf" ly * *
I cdn1 20 18 0 "cdn" ly * *
I cdn2 8 19 0 "cdn" ly * *
I cpf2 19 24 0 "cpf" ly * *
I cdn3 20 12 0 "cdn" ly * *
I cdp1 8 53 0 "cdp" ly * *
I cdp2 8 58 0 "cdp" ly * *
I cdn4 8 12 0 "cdn" ly * *
I cdp3 14 58 0 "cdp" ly * *
I cdp4 14 53 0 "cdp" ly * *
I cdp5 20 58 0 "cdp" ly * *
I cdp6 20 53 0 "cdp" ly * *
I via1 8 28 0 "cvia" ly * *
I cpf3 12 43 0 "cpf" ly * *
I cpf4 19 37 0 "cpf" ly * *
I via2 40 28 0 "cvia" ly * *
I cpf5 41 33 0 "cpf" ly * *
I cpf6 35 29 0 "cpf" ly * *
I cdp7 46 58 0 "cdp" ly * *
I cdp8 40 53 0 "cdp" ly * *
I cdp9 40 47 0 "cdp" ly * *
I cdp10 40 58 0 "cdp" ly * *
I cdp11 40 42 0 "cdp" ly * *
I cdp12 28 42 0 "cdp" ly * *
I cdp13 28 47 0 "cdp" ly * *
I cdp14 28 53 0 "cdp" ly * *
I cdp15 28 58 0 "cdp" ly * *
I cdp16 46 53 0 "cdp" ly * *
I cdn5 46 18 0 "cdn" ly * *
I cdn6 34 12 0 "cdn" ly * *
I cdn7 40 12 0 "cdn" ly * *
I cdn8 28 18 0 "cdn" ly * *
I cdn9 28 12 0 "cdn" ly * *
I cdn10 34 18 0 "cdn" ly * *
I cdn11 40 18 0 "cdn" ly * *
I via3 8 5 0 "cvia" ly * *
I via4 14 5 0 "cvia" ly * *
I via5 20 5 0 "cvia" ly * *
I via6 20 65 0 "cvia" ly * *
I via7 14 65 0 "cvia" ly * *
I via8 8 65 0 "cvia" ly * *
I via9 28 65 0 "cvia" ly * *
I via10 34 65 0 "cvia" ly * *
I via11 40 65 0 "cvia" ly * *
I via12 46 65 0 "cvia" ly * *
I via13 46 5 0 "cvia" ly * *
I via14 40 5 0 "cvia" ly * *
I via15 34 5 0 "cvia" ly * *
I via16 28 5 0 "cvia" ly * *
I cbp1 43 5 0 "cbp" ly * *
I cbp2 37 5 0 "cbp" ly * *
I cbp3 31 5 0 "cbp" ly * *
I cbp4 17 5 0 "cbp" ly * *
I cbp5 11 5 0 "cbp" ly * *
I cbn1 43 65 0 "cbn" ly * *
I cbn2 37 65 0 "cbn" ly * *
I cbn3 31 65 0 "cbn" ly * *
I cbn4 17 65 0 "cbn" ly * *
I cbn5 11 65 0 "cbn" ly * *
I i0_6 20 43 0 "ref_con" ly * *
I i0_5 14 38 0 "ref_con" ly * *
I i0_4 14 33 0 "ref_con" ly * *
I i0_1 14 18 0 "ref_con" ly * *
I i0_0 14 13 0 "ref_con" ly * *
I i0_2 14 23 0 "ref_con" ly * *
I o_6 46 43 0 "ref_con" ly * *
I o_4 46 33 0 "ref_con" ly * *
I o_5 46 38 0 "ref_con" ly * *
I o_7 46 48 0 "ref_con" ly * *
I nwell_28 48 53 0 "ref_ref" ly * *
I o_3 46 28 0 "ref_con" ly * *
I o_2 46 23 0 "ref_con" ly * *
I o_0 46 13 0 "ref_con" ly * *
P 14 29 * metal
P 20 29 * metal
W 13 28 21 30
S 1 H metal P 1 P 2
P 20 30 * poly
P 20 29 * poly
W 19 28 21 31
S 1 V poly P 3 P 4
P 20 18 * metal
P 20 5 * metal
W 18 3 22 20
S 2 V metal P 5 P 6
P 17 24 * poly
P 17 23 * poly
W 16 22 18 25
S 1 V poly P 7 P 8
P 19 24 * poly
P 17 24 * poly
W 16 23 20 25
S 1 H poly P 9 P 10
P 11 47 * poly
P 11 23 * poly
W 10 22 12 48
S 1 V poly P 11 P 12
P 11 8 * ntrans
P 11 23 * ntrans
W 10 7 12 24
S 1 V ntrans P 13 P 14
P 8 21 * ndif
P 8 10 * ndif
W 5 7 11 24
S 3 V ndif P 15 P 16
P 14 21 * ndif
P 14 10 * ndif
W 11 7 17 24
S 3 V ndif P 17 P 18
P 17 8 * ntrans
P 17 23 * ntrans
W 16 7 18 24
S 1 V ntrans P 19 P 20
P 20 21 * ndif
P 20 10 * ndif
W 17 7 23 24
S 3 V ndif P 21 P 22
P 17 62 * ptrans
P 17 47 * ptrans
W 16 46 18 63
S 1 V ptrans P 23 P 24
P 20 60 * pdif
P 20 49 * pdif
W 17 46 23 63
S 3 V pdif P 25 P 26
P 11 47 * ptrans
P 11 62 * ptrans
W 10 46 12 63
S 1 V ptrans P 27 P 28
P 14 49 * pdif
P 14 60 * pdif
W 11 46 17 63
S 3 V pdif P 29 P 30
P 8 60 * pdif
P 8 49 * pdif
W 5 46 11 63
S 3 V pdif P 31 P 32
P 14 53 * metal
P 14 65 * metal
W 12 51 16 67
S 2 V metal P 33 P 34
P 20 58 * metal
P 20 49 * metal
W 19 48 21 59
S 1 V metal P 35 P 36
P 20 49 * metal
P 8 49 * metal
W 7 48 21 50
S 1 H metal P 37 P 38
P 8 58 * metal
P 8 12 * metal
W 7 11 9 59
S 1 V metal P 39 P 40
P 14 43 * metal
P 20 43 * metal
W 13 42 21 44
S 1 H metal P 41 P 42
P 17 37 * poly
P 17 47 * poly
W 16 36 18 48
S 1 V poly P 43 P 44
P 14 43 * metal
P 14 13 * metal
W 13 12 15 44 i0
S 1 V metal P 45 P 46
P 14 43 * metal
P 12 43 * metal
W 10 41 16 45
S 2 H metal P 47 P 48
P 19 37 * poly
P 17 37 * poly
W 16 36 20 38
S 1 H poly P 49 P 50
P 24 65 * metal
P 24 5 * metal
W 23 4 25 66
S 1 V metal P 51 P 52
P 19 37 * metal
P 24 37 * metal
W 18 36 25 38
S 1 H metal P 53 P 54
P 19 24 * metal
P 24 24 * metal
W 18 23 25 25
S 1 H metal P 55 P 56
P 20 30 * poly
P 31 30 * poly
W 19 29 32 31
S 1 H poly P 57 P 58
P 41 33 * metal
P 40 33 * metal
W 38 31 43 35
S 2 H metal P 59 P 60
P 40 33 * metal
P 40 28 * metal
W 38 26 42 35
S 2 V metal P 61 P 62
P 46 42 * metal
P 40 42 * metal
W 39 41 47 43
S 1 H metal P 63 P 64
P 40 42 * metal
P 40 58 * metal
W 39 41 41 59
S 1 V metal P 65 P 66
P 46 53 * metal
P 46 65 * metal
W 44 51 48 67
S 2 V metal P 67 P 68
P 28 42 * metal
P 28 65 * metal
W 26 40 30 67
S 2 V metal P 69 P 70
P 46 48 * metal
P 46 13 * metal
W 45 12 47 49 o
S 1 V metal P 71 P 72
P 34 18 * metal
P 34 5 * metal
W 32 3 36 20
S 2 V metal P 73 P 74
P 28 12 * metal
P 28 22 * metal
W 27 11 29 23
S 1 V metal P 75 P 76
P 28 22 * metal
P 40 22 * metal
W 27 21 41 23
S 1 H metal P 77 P 78
P 40 22 * metal
P 40 12 * metal
W 39 11 41 23
S 1 V metal P 79 P 80
P 41 33 * poly
P 43 33 * poly
W 38 30 46 36
S 3 H poly P 81 P 82
P 35 29 * poly
P 37 29 * poly
W 34 28 38 30
S 1 H poly P 83 P 84
P 31 38 * poly
P 31 23 * poly
W 30 22 32 39
S 1 V poly P 85 P 86
P 37 38 * poly
P 37 23 * poly
W 36 22 38 39
S 1 V poly P 87 P 88
P 43 47 * poly
P 43 23 * poly
W 42 22 44 48
S 1 V poly P 89 P 90
P 43 47 * ptrans
P 43 62 * ptrans
W 42 46 44 63
S 1 V ptrans P 91 P 92
P 46 60 * pdif
P 46 49 * pdif
W 43 46 49 63
S 3 V pdif P 93 P 94
P 40 40 * pdif
P 40 60 * pdif
W 37 37 43 63
S 3 V pdif P 95 P 96
P 31 38 * ptrans
P 31 62 * ptrans
W 30 37 32 63
S 1 V ptrans P 97 P 98
P 37 62 * ptrans
P 37 38 * ptrans
W 36 37 38 63
S 1 V ptrans P 99 P 100
P 28 60 * pdif
P 28 40 * pdif
W 25 37 31 63
S 3 V pdif P 101 P 102
P 34 60 * pdif
P 34 40 * pdif
W 31 37 37 63
S 3 V pdif P 103 P 104
P 37 23 * ntrans
P 37 8 * ntrans
W 36 7 38 24
S 1 V ntrans P 105 P 106
P 40 21 * ndif
P 40 10 * ndif
W 37 7 43 24
S 3 V ndif P 107 P 108
P 28 21 * ndif
P 28 10 * ndif
W 25 7 31 24
S 3 V ndif P 109 P 110
P 43 8 * ntrans
P 43 23 * ntrans
W 42 7 44 24
S 1 V ntrans P 111 P 112
P 46 21 * ndif
P 46 10 * ndif
W 43 7 49 24
S 3 V ndif P 113 P 114
P 34 21 * ndif
P 34 10 * ndif
W 31 7 37 24
S 3 V ndif P 115 P 116
P 31 8 * ntrans
P 31 23 * ntrans
W 30 7 32 24
S 1 V ntrans P 117 P 118
P 46 28 * allowM2
P 49 28 * allowM2
W 44 26 51 30
S 2 H allowM2 P 119 P 120
P 34 29 * metal
P 24 29 * metal
W 23 28 35 30
S 1 H metal P 121 P 122
P 40 28 * metal2
P 8 28 * metal2
W 6 26 42 30
S 2 H metal2 P 123 P 124
P 8 65 * metal
P 20 65 * metal
W 6 63 22 67
S 2 H metal P 125 P 126
P 28 65 * metal
P 46 65 * metal
W 26 63 48 67
S 2 H metal P 127 P 128
P 8 5 * metal
P 20 5 * metal
W 6 3 22 7
S 2 H metal P 129 P 130
P 28 5 * metal
P 46 5 * metal
W 26 3 48 7
S 2 H metal P 131 P 132
P 7 5 * ptie
P 21 5 * ptie
W 4 2 24 8
S 3 H ptie P 133 P 134
P 27 5 * ptie
P 47 5 * ptie
W 24 2 50 8
S 3 H ptie P 135 P 136
P 7 65 * ntie
P 21 65 * ntie
W 4 62 24 68
S 3 H ntie P 137 P 138
P 27 65 * ntie
P 47 65 * ntie
W 24 62 50 68
S 3 H ntie P 139 P 140
P 5 53 * nwell
P 49 53 * nwell
W -23 25 77 81 nwell
S 28 H nwell P 141 P 142
P 5 5 * metal2
P 49 5 * metal2
W -3 -3 57 13
S 8 H metal2 P 143 P 144
P 5 65 * metal2
P 49 65 * metal2
W -1 59 55 71 vdd
S 6 H metal2 P 145 P 146
P 5 58 * allowM2
P 49 58 * allowM2
W 3 56 51 60
S 2 H allowM2 P 147 P 148
P 5 53 * allowM2
P 49 53 * allowM2
W 3 51 51 55
S 2 H allowM2 P 149 P 150
P 5 48 * allowM2
P 49 48 * allowM2
W 3 46 51 50
S 2 H allowM2 P 151 P 152
P 5 43 * allowM2
P 49 43 * allowM2
W 3 41 51 45
S 2 H allowM2 P 153 P 154
P 5 38 * allowM2
P 49 38 * allowM2
W 3 36 51 40
S 2 H allowM2 P 155 P 156
P 5 33 * allowM2
P 49 33 * allowM2
W 3 31 51 35
S 2 H allowM2 P 157 P 158
P 5 23 * allowM2
P 49 23 * allowM2
W 3 21 51 25
S 2 H allowM2 P 159 P 160
P 5 18 * allowM2
P 49 18 * allowM2
W 3 16 51 20
S 2 H allowM2 P 161 P 162
P 5 13 * allowM2
P 49 13 * allowM2
W 3 11 51 15
S 2 H allowM2 P 163 P 164
E
