#cell1 bsmrig32_c idps_symb compose *
# 31-Jan-95 18:25 31-Jan-95 18:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 20 42 34 162
B 20 42 34 162
F F
C in_1 21 162 metal 1 1 * * N in_1
C vss1 20 162 metal2 8 2 * * W vss1
C vss1 34 162 metal2 8 3 * * E vss1
C vss 20 42 metal2 8 4 * * W vss
C vss 34 42 metal2 8 5 * * E vss
C com 31 42 metal 1 6 * * S com
C in_3 21 42 metal 1 7 * * S in_3
C dat_2 34 60 metal2 2 8 * * E dat_2
C dat_2 20 60 metal2 2 9 * * W dat_2
C out_2_s 34 65 metal2 2 10 * * E out_2_s
C out_2_s 20 65 metal2 2 11 * * W out_2_s
C in_3 34 75 metal2 2 12 * * E in_3
C in_2 20 75 metal2 2 13 * * W in_2
C vdd 34 102 metal2 6 14 * * E vdd
C vdd 20 102 metal2 6 15 * * W vdd
C out_1_s 20 139 metal2 2 16 * * W out_1_s
C out_1_s 34 139 metal2 2 17 * * E out_1_s
C in_1 20 154 metal2 2 18 * * W in_1
C in_2 34 154 metal2 2 19 * * E in_2
C dat_1 34 144 metal2 2 20 * * E dat_1
C dat_1 20 144 metal2 2 21 * * W dat_1
C com 31 162 metal 1 22 * * N com
I cbp1 26 46 0 "cbp" ly * *
I cdn1 27 144 0 "cdn" ly * *
I cpf1 31 139 0 "cpf" ly * *
I cdn2 21 144 0 "cdn" ly * *
I via1 21 139 0 "cvia" ly * *
I via2 27 154 0 "cvia" ly * *
I via3 21 154 0 "cvia" ly * *
I cdn3 27 60 0 "cdn" ly * *
I via4 21 65 0 "cvia" ly * *
I cdn4 21 60 0 "cdn" ly * *
I via5 21 75 0 "cvia" ly * *
I via6 27 75 0 "cvia" ly * *
I cpf2 31 65 0 "cpf" ly * *
I via7 26 42 0 "cvia" ly * *
I via8 26 102 0 "cvia" ly * *
I cbn1 26 88 0 "cbn" ly * *
I cbn2 26 95 0 "cbn" ly * *
I cbn3 26 107 0 "cbn" ly * *
I via9 26 162 0 "cvia" ly * *
I cbp2 26 159 0 "cbp" ly * *
P 26 159 * metal
P 26 162 * metal
W 24 157 28 164
S 2 V metal P 1 P 2
P 26 88 * metal
P 26 107 * metal
W 24 86 28 109
S 2 V metal P 3 P 4
P 19 102 * nwell
P 35 102 * nwell
W -33 50 87 154
S 52 H nwell P 5 P 6
P 20 162 * metal2
P 34 162 * metal2
W 12 154 42 170 vss1
S 8 H metal2 P 7 P 8
P 26 42 * metal
P 26 46 * metal
W 24 40 28 48
S 2 V metal P 9 P 10
P 27 52 * metal
P 27 75 * metal
W 26 51 28 76
S 1 V metal P 11 P 12
P 21 52 * metal
P 27 52 * metal
W 20 51 28 53
S 1 H metal P 13 P 14
P 21 42 * metal
P 21 52 * metal
W 20 41 22 53
S 1 V metal P 15 P 16
P 27 111 * metal
P 27 154 * metal
W 26 110 28 155
S 1 V metal P 17 P 18
P 21 111 * metal
P 27 111 * metal
W 20 110 28 112
S 1 H metal P 19 P 20
P 21 75 * metal
P 21 111 * metal
W 20 74 22 112
S 1 V metal P 21 P 22
P 20 55 * allowM2
P 34 55 * allowM2
W 18 53 36 57
S 2 H allowM2 P 23 P 24
P 21 70 * allowM2
P 34 70 * allowM2
W 19 68 36 72
S 2 H allowM2 P 25 P 26
P 20 95 * allowM2
P 34 95 * allowM2
W 18 93 36 97
S 2 H allowM2 P 27 P 28
P 20 80 * allowM2
P 34 80 * allowM2
W 18 78 36 82
S 2 H allowM2 P 29 P 30
P 20 85 * allowM2
P 34 85 * allowM2
W 18 83 36 87
S 2 H allowM2 P 31 P 32
P 20 90 * allowM2
P 34 90 * allowM2
W 18 88 36 92
S 2 H allowM2 P 33 P 34
P 20 102 * metal2
P 34 102 * metal2
W 14 96 40 108 vdd
S 6 H metal2 P 35 P 36
P 20 109 * allowM2
P 34 109 * allowM2
W 18 107 36 111
S 2 H allowM2 P 37 P 38
P 20 114 * allowM2
P 34 114 * allowM2
W 18 112 36 116
S 2 H allowM2 P 39 P 40
P 20 119 * allowM2
P 34 119 * allowM2
W 18 117 36 121
S 2 H allowM2 P 41 P 42
P 20 124 * allowM2
P 34 124 * allowM2
W 18 122 36 126
S 2 H allowM2 P 43 P 44
P 20 134 * allowM2
P 34 134 * allowM2
W 18 132 36 136
S 2 H allowM2 P 45 P 46
P 20 149 * allowM2
P 34 149 * allowM2
W 18 147 36 151
S 2 H allowM2 P 47 P 48
P 31 42 * metal
P 31 162 * metal
W 30 41 32 163
S 1 V metal P 49 P 50
P 24 139 * ntrans
P 24 149 * ntrans
W 23 138 25 150
S 1 V ntrans P 51 P 52
P 21 141 * ndif
P 21 147 * ndif
W 19 139 23 149
S 2 V ndif P 53 P 54
P 24 139 * poly
P 31 139 * poly
W 23 138 32 140
S 1 H poly P 55 P 56
P 21 139 * metal
P 21 144 * metal
W 20 138 22 145 out_1_s
S 1 V metal P 57 P 58
P 20 154 * metal2
P 21 154 * metal2
W 18 152 23 156
S 2 H metal2 P 59 P 60
P 27 154 * metal2
P 34 154 * metal2
W 25 152 36 156
S 2 H metal2 P 61 P 62
P 21 154 * metal
P 21 162 * metal
W 20 153 22 163
S 1 V metal P 63 P 64
P 24 55 * ntrans
P 24 65 * ntrans
W 23 54 25 66
S 1 V ntrans P 65 P 66
P 21 57 * ndif
P 21 63 * ndif
W 19 55 23 65
S 2 V ndif P 67 P 68
P 21 60 * metal
P 21 65 * metal
W 20 59 22 66 out_2_s
S 1 V metal P 69 P 70
P 24 65 * poly
P 31 65 * poly
W 23 64 32 66
S 1 H poly P 71 P 72
P 27 75 * metal2
P 34 75 * metal2
W 25 73 36 77
S 2 H metal2 P 73 P 74
P 20 75 * metal2
P 21 75 * metal2
W 18 73 23 77
S 2 H metal2 P 75 P 76
P 27 141 * ndif
P 27 147 * ndif
W 25 139 29 149
S 2 V ndif P 77 P 78
P 20 139 * metal2
P 34 139 * metal2
W 18 137 36 141 out_1_s
S 2 H metal2 P 79 P 80
P 20 65 * metal2
P 34 65 * metal2
W 18 63 36 67 out_2_s
S 2 H metal2 P 81 P 82
P 27 57 * ndif
P 27 63 * ndif
W 25 55 29 65
S 2 V ndif P 83 P 84
P 20 60 * metal2
P 34 60 * metal2
W 18 58 36 62
S 2 H metal2 P 85 P 86
P 20 144 * metal2
P 34 144 * metal2
W 18 142 36 146
S 2 H metal2 P 87 P 88
P 20 42 * metal2
P 34 42 * metal2
W 12 34 42 50 vss
S 8 H metal2 P 89 P 90
E
