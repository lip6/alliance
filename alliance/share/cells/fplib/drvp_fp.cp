#cell1 drvp_fp idps_symb compose *
# 19-Nov-97 16:59 19-Nov-97 16:59 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 41 65
B 5 5 41 65
F F
C vss 5 5 metal2 8 1 * * W vss
C vss 41 5 metal2 8 2 * * E vss
C vdd 41 65 metal2 6 3 * * E vdd
C vdd 5 65 metal2 6 4 * * W vdd
I cdp1 32 58 0 "cdp" ly * *
I cdp2 32 48 0 "cdp" ly * *
I via1 14 5 0 "cvia" ly * *
I via2 20 65 0 "cvia" ly * *
I via3 14 5 0 "cvia" ly * *
I via4 14 65 0 "cvia" ly * *
I via5 20 5 0 "cvia" ly * *
I cbp1 17 5 0 "cbp" ly * *
I cbn1 17 65 0 "cbn" ly * *
I cdp3 14 56 0 "cdp" ly * *
I cdn1 14 18 0 "cdn" ly * *
I cpf1 13 28 0 "cpf" ly * *
I cbn2 23 65 0 "cbn" ly * *
I cbn3 11 65 0 "cbn" ly * *
I via6 26 65 0 "cvia" ly * *
I via7 8 65 0 "cvia" ly * *
I cbn4 29 65 0 "cbn" ly * *
I cbp2 23 5 0 "cbp" ly * *
I cbp3 11 5 0 "cbp" ly * *
I via8 26 5 0 "cvia" ly * *
I via9 8 5 0 "cvia" ly * *
I cbp4 29 5 0 "cbp" ly * *
I via10 32 65 0 "cvia" ly * *
I cbn5 35 65 0 "cbn" ly * *
I via11 38 65 0 "cvia" ly * *
I cdp4 38 58 0 "cdp" ly * *
I cdp5 26 58 0 "cdp" ly * *
I via12 32 5 0 "cvia" ly * *
I cbp5 35 5 0 "cbp" ly * *
I via13 38 5 0 "cvia" ly * *
I cdn2 38 13 0 "cdn" ly * *
I cdn3 26 13 0 "cdn" ly * *
I cdp6 38 43 0 "cdp" ly * *
I cdp7 8 43 0 "cdp" ly * *
I cdp8 8 48 0 "cdp" ly * *
I cdp9 20 53 0 "cdp" ly * *
I cdp10 20 43 0 "cdp" ly * *
I cdp11 38 53 0 "cdp" ly * *
I cdp12 38 48 0 "cdp" ly * *
I cdp13 26 53 0 "cdp" ly * *
I cdp14 26 48 0 "cdp" ly * *
I cdp15 26 43 0 "cdp" ly * *
I cdn4 26 18 0 "cdn" ly * *
I cdn5 8 13 0 "cdn" ly * *
I cdn6 8 18 0 "cdn" ly * *
I cdn7 20 13 0 "cdn" ly * *
I cpf2 16 23 0 "cpf" ly * *
I cdn8 32 18 0 "cdn" ly * *
I cdn9 14 13 0 "cdn" ly * *
I cdn10 38 18 0 "cdn" ly * *
I o_2 20 48 0 "ref_con" ly * *
I o_0 20 58 0 "ref_con" ly * *
I o_3 32 43 0 "ref_con" ly * *
I nwell_28 11 53 0 "ref_ref" ly * *
I nwell_28 17 53 0 "ref_ref" ly * *
I o_7 32 23 0 "ref_con" ly * *
I o_5 32 33 0 "ref_con" ly * *
I o_4 32 38 0 "ref_con" ly * *
I o_1 32 53 0 "ref_con" ly * *
I i_1 8 53 0 "ref_con" ly * *
I i_2 14 48 0 "ref_con" ly * *
I i_3 14 43 0 "ref_con" ly * *
I i_4 14 38 0 "ref_con" ly * *
I i_5 14 33 0 "ref_con" ly * *
I i_0 8 58 0 "ref_con" ly * *
I o_6 26 28 0 "ref_con" ly * *
I o_8 20 18 0 "ref_con" ly * *
I o_9 32 13 0 "ref_con" ly * *
P 20 37 * metal
P 32 37 * metal
W 19 36 33 38 o
S 1 H metal P 1 P 2
P 20 37 * metal
P 20 58 * metal
W 19 36 21 59 o
S 1 V metal P 3 P 4
P 20 27 * metal
P 20 37 * metal
W 19 26 21 38 o
S 1 V metal P 5 P 6
P 32 13 * metal
P 32 27 * metal
W 31 12 33 28 o
S 1 V metal P 7 P 8
P 26 27 * metal
P 32 27 * metal
W 25 26 33 28 o
S 1 H metal P 9 P 10
P 26 27 * metal
P 26 28 * metal
W 25 26 27 29 o
S 1 V metal P 11 P 12
P 20 27 * metal
P 26 27 * metal
W 19 26 27 28 o
S 1 H metal P 13 P 14
P 20 13 * metal
P 20 27 * metal
W 19 12 21 28 o
S 1 V metal P 15 P 16
P 32 27 * metal
P 32 37 * metal
W 31 26 33 38 o
S 1 V metal P 17 P 18
P 32 37 * metal
P 32 58 * metal
W 31 36 33 59 o
S 1 V metal P 19 P 20
P 23 31 * poly
P 23 38 * poly
W 22 30 24 39
S 1 V poly P 21 P 22
P 23 30 * poly
P 23 31 * poly
W 22 29 24 32
S 1 V poly P 23 P 24
P 23 22 * poly
P 23 30 * poly
W 22 21 24 31
S 1 V poly P 25 P 26
P 17 30 * poly
P 23 30 * poly
W 16 29 24 31
S 1 H poly P 27 P 28
P 16 23 * poly
P 17 23 * poly
W 15 22 18 24
S 1 H poly P 29 P 30
P 17 22 * poly
P 17 23 * poly
W 16 21 18 24
S 1 V poly P 31 P 32
P 17 23 * poly
P 17 30 * poly
W 16 22 18 31
S 1 V poly P 33 P 34
P 17 30 * poly
P 17 38 * poly
W 16 29 18 39
S 1 V poly P 35 P 36
P 23 31 * poly
P 29 31 * poly
W 22 30 30 32
S 1 H poly P 37 P 38
P 29 22 * poly
P 29 30 * poly
W 28 21 30 31
S 1 V poly P 39 P 40
P 29 30 * poly
P 35 30 * poly
W 28 29 36 31
S 1 H poly P 41 P 42
P 35 22 * poly
P 35 30 * poly
W 34 21 36 31
S 1 V poly P 43 P 44
P 35 30 * poly
P 35 38 * poly
W 34 29 36 39
S 1 V poly P 45 P 46
P 29 30 * poly
P 29 31 * poly
W 28 29 30 32
S 1 V poly P 47 P 48
P 29 31 * poly
P 29 38 * poly
W 28 30 30 39
S 1 V poly P 49 P 50
P 11 28 * poly
P 13 28 * poly
W 10 27 14 29
S 1 H poly P 51 P 52
P 11 22 * poly
P 11 28 * poly
W 10 21 12 29
S 1 V poly P 53 P 54
P 11 28 * poly
P 11 38 * poly
W 10 27 12 39
S 1 V poly P 55 P 56
P 16 23 * metal
P 16 24 * metal
W 15 22 17 25
S 1 V metal P 57 P 58
P 8 24 * metal
P 16 24 * metal
W 7 23 17 25
S 1 H metal P 59 P 60
P 8 13 * metal
P 8 24 * metal
W 7 12 9 25
S 1 V metal P 61 P 62
P 8 24 * metal
P 8 48 * metal
W 7 23 9 49
S 1 V metal P 63 P 64
P 8 52 * metal
P 14 52 * metal
W 7 51 15 53 i
S 1 H metal P 65 P 66
P 14 28 * metal
P 14 52 * metal
W 13 27 15 53 i
S 1 V metal P 67 P 68
P 13 28 * metal
P 14 28 * metal
W 12 27 15 29 i
S 1 H metal P 69 P 70
P 14 56 * metal
P 14 65 * metal
W 12 54 16 67
S 2 V metal P 71 P 72
P 8 65 * metal
P 14 65 * metal
W 6 63 16 67
S 2 H metal P 73 P 74
P 14 65 * metal
P 26 65 * metal
W 12 63 28 67
S 2 H metal P 75 P 76
P 38 43 * metal
P 38 65 * metal
W 36 41 40 67
S 2 V metal P 77 P 78
P 26 65 * metal
P 38 65 * metal
W 24 63 40 67
S 2 H metal P 79 P 80
P 26 43 * metal
P 26 65 * metal
W 24 41 28 67
S 2 V metal P 81 P 82
P 14 5 * metal
P 14 18 * metal
W 12 3 16 20
S 2 V metal P 83 P 84
P 8 5 * metal
P 14 5 * metal
W 6 3 16 7
S 2 H metal P 85 P 86
P 14 5 * metal
P 26 5 * metal
W 12 3 28 7
S 2 H metal P 87 P 88
P 38 5 * metal
P 38 18 * metal
W 36 3 40 20
S 2 V metal P 89 P 90
P 26 5 * metal
P 38 5 * metal
W 24 3 40 7
S 2 H metal P 91 P 92
P 26 5 * metal
P 26 18 * metal
W 24 3 28 20
S 2 V metal P 93 P 94
P 5 53 * nwell
P 41 53 * nwell
W -23 25 69 81 nwell
S 28 H nwell P 95 P 96
P 5 5 * metal2
P 41 5 * metal2
W -3 -3 49 13
S 8 H metal2 P 97 P 98
P 5 13 * allowM2
P 41 13 * allowM2
W 3 11 43 15
S 2 H allowM2 P 99 P 100
P 5 18 * allowM2
P 41 18 * allowM2
W 3 16 43 20
S 2 H allowM2 P 101 P 102
P 5 23 * allowM2
P 41 23 * allowM2
W 3 21 43 25
S 2 H allowM2 P 103 P 104
P 5 28 * allowM2
P 41 28 * allowM2
W 3 26 43 30
S 2 H allowM2 P 105 P 106
P 5 33 * allowM2
P 41 33 * allowM2
W 3 31 43 35
S 2 H allowM2 P 107 P 108
P 5 38 * allowM2
P 41 38 * allowM2
W 3 36 43 40
S 2 H allowM2 P 109 P 110
P 5 43 * allowM2
P 41 43 * allowM2
W 3 41 43 45
S 2 H allowM2 P 111 P 112
P 5 48 * allowM2
P 41 48 * allowM2
W 3 46 43 50
S 2 H allowM2 P 113 P 114
P 5 53 * allowM2
P 41 53 * allowM2
W 3 51 43 55
S 2 H allowM2 P 115 P 116
P 5 58 * allowM2
P 41 58 * allowM2
W 3 56 43 60
S 2 H allowM2 P 117 P 118
P 5 65 * metal2
P 41 65 * metal2
W -1 59 47 71 vdd
S 6 H metal2 P 119 P 120
P 29 38 * ptrans
P 29 62 * ptrans
W 28 37 30 63
S 1 V ptrans P 121 P 122
P 23 38 * ptrans
P 23 62 * ptrans
W 22 37 24 63
S 1 V ptrans P 123 P 124
P 14 40 * pdif
P 14 60 * pdif
W 11 37 17 63
S 3 V pdif P 125 P 126
P 17 38 * ptrans
P 17 62 * ptrans
W 16 37 18 63
S 1 V ptrans P 127 P 128
P 8 40 * pdif
P 8 60 * pdif
W 5 37 11 63
S 3 V pdif P 129 P 130
P 11 38 * ptrans
P 11 62 * ptrans
W 10 37 12 63
S 1 V ptrans P 131 P 132
P 38 40 * pdif
P 38 60 * pdif
W 35 37 41 63
S 3 V pdif P 133 P 134
P 35 38 * ptrans
P 35 62 * ptrans
W 34 37 36 63
S 1 V ptrans P 135 P 136
P 38 10 * ndif
P 38 20 * ndif
W 35 7 41 23
S 3 V ndif P 137 P 138
P 35 8 * ntrans
P 35 22 * ntrans
W 34 7 36 23
S 1 V ntrans P 139 P 140
P 32 10 * ndif
P 32 20 * ndif
W 29 7 35 23
S 3 V ndif P 141 P 142
P 29 8 * ntrans
P 29 22 * ntrans
W 28 7 30 23
S 1 V ntrans P 143 P 144
P 26 10 * ndif
P 26 20 * ndif
W 23 7 29 23
S 3 V ndif P 145 P 146
P 23 8 * ntrans
P 23 22 * ntrans
W 22 7 24 23
S 1 V ntrans P 147 P 148
P 20 10 * ndif
P 20 20 * ndif
W 17 7 23 23
S 3 V ndif P 149 P 150
P 17 8 * ntrans
P 17 22 * ntrans
W 16 7 18 23
S 1 V ntrans P 151 P 152
P 14 10 * ndif
P 14 20 * ndif
W 11 7 17 23
S 3 V ndif P 153 P 154
P 11 8 * ntrans
P 11 22 * ntrans
W 10 7 12 23
S 1 V ntrans P 155 P 156
P 8 10 * ndif
P 8 20 * ndif
W 5 7 11 23
S 3 V ndif P 157 P 158
P 32 40 * pdif
P 32 60 * pdif
W 29 37 35 63
S 3 V pdif P 159 P 160
P 26 40 * pdif
P 26 60 * pdif
W 23 37 29 63
S 3 V pdif P 161 P 162
P 20 40 * pdif
P 20 60 * pdif
W 17 37 23 63
S 3 V pdif P 163 P 164
P 8 53 * metal
P 8 58 * metal
W 7 52 9 59
S 1 V metal P 165 P 166
E
