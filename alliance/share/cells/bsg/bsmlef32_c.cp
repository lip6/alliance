#cell1 bsmlef32_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A 8 42 34 162
B 8 42 34 162
F F
C dat_1 8 144 metal2 2 1 * * W dat_1
C dat_1 34 144 metal2 2 2 * * E dat_1
C vss1 8 162 metal2 8 3 * * W vss1
C vss1 34 162 metal2 8 4 * * E vss1
C in_1 8 154 metal2 2 5 * * W in_1
C in_2 34 154 metal2 2 6 * * E in_2
C in_1 15 162 metal 1 7 * * N in_1
C com_2 31 162 metal 1 8 * * N com_2
C out_1_s 34 139 metal2 2 9 * * E out_1_s
C scom_1 8 129 metal2 2 10 * * W scom_1
C scom_1 34 129 metal2 2 11 * * E scom_1
C vdd 8 102 metal2 6 12 * * W vdd
C vdd 34 102 metal2 6 13 * * E vdd
C dat_2 8 60 metal2 2 14 * * W dat_2
C dat_2 34 60 metal2 2 15 * * E dat_2
C out_2_s 34 65 metal2 2 16 * * E out_2_s
C scom_2 8 50 metal2 2 17 * * W scom_2
C scom_2 34 50 metal2 2 18 * * E scom_2
C com_1 11 42 metal 1 19 * * S com_1
C com_2 31 42 metal 1 20 * * S com_2
C in_3 34 75 metal2 2 21 * * E in_3
C vss 8 42 metal2 8 22 * * W vss
C vss 34 42 metal2 8 23 * * E vss
C in_3 15 42 metal 1 24 * * S in_3
C com_1 11 162 metal 1 25 * * N com_1
C in_2 8 75 metal2 2 26 * * W in_2
I via1 27 162 0 "cvia" ly * *
I via2 27 42 0 "cvia" ly * *
I cpf1 11 65 0 "cpf" ly * *
I cpf2 31 65 0 "cpf" ly * *
I via3 27 75 0 "cvia" ly * *
I via4 15 75 0 "cvia" ly * *
I cdn1 21 60 0 "cdn" ly * *
I via5 21 65 0 "cvia" ly * *
I cdn2 27 60 0 "cdn" ly * *
I cdn3 15 60 0 "cdn" ly * *
I via6 15 154 0 "cvia" ly * *
I via7 27 154 0 "cvia" ly * *
I via8 21 139 0 "cvia" ly * *
I cdn4 21 144 0 "cdn" ly * *
I cpf3 31 139 0 "cpf" ly * *
I cpf4 11 139 0 "cpf" ly * *
I cdn5 27 144 0 "cdn" ly * *
I cdn6 15 144 0 "cdn" ly * *
I via9 27 102 0 "cvia" ly * *
I cbn1 23 102 0 "cbn" ly * *
I cbp1 23 162 0 "cbp" ly * *
I cbp2 23 42 0 "cbp" ly * *
I ref_cr_2 31 50 0 "ref_con" ly * *
I ref_cl_2 11 50 0 "ref_con" ly * *
I ref_cr_1 31 129 0 "ref_con" ly * *
I ref_cl_1 11 129 0 "ref_con" ly * *
P 15 57 * ndif
P 15 63 * ndif
W 13 55 17 65
S 2 V ndif P 1 P 2
P 27 57 * ndif
P 27 63 * ndif
W 25 55 29 65
S 2 V ndif P 3 P 4
P 24 65 * poly
P 31 65 * poly
W 23 64 32 66
S 1 H poly P 5 P 6
P 15 52 * metal
P 27 52 * metal
W 14 51 28 53
S 1 H metal P 7 P 8
P 15 42 * metal
P 15 52 * metal
W 14 41 16 53
S 1 V metal P 9 P 10
P 8 80 * allowM2
P 34 80 * allowM2
W 6 78 36 82
S 2 H allowM2 P 11 P 12
P 15 154 * metal
P 15 162 * metal
W 14 153 16 163
S 1 V metal P 13 P 14
P 15 144 * metal
P 15 154 * metal
W 14 143 16 155
S 1 V metal P 15 P 16
P 15 75 * metal
P 15 111 * metal
W 14 74 16 112
S 1 V metal P 17 P 18
P 15 60 * metal
P 15 75 * metal
W 14 59 16 76
S 1 V metal P 19 P 20
P 23 42 * metal
P 27 42 * metal
W 21 40 29 44
S 2 H metal P 21 P 22
P 8 144 * metal2
P 34 144 * metal2
W 6 142 36 146 dat_1
S 2 H metal2 P 23 P 24
P 8 60 * metal2
P 34 60 * metal2
W 6 58 36 62
S 2 H metal2 P 25 P 26
P 23 102 * metal
P 27 102 * metal
W 21 100 29 104
S 2 H metal P 27 P 28
P 23 162 * metal
P 27 162 * metal
W 21 160 29 164
S 2 H metal P 29 P 30
P 7 102 * nwell
P 35 102 * nwell
W -45 50 87 154
S 52 H nwell P 31 P 32
P 8 75 * metal2
P 15 75 * metal2
W 6 73 17 77 in_2
S 2 H metal2 P 33 P 34
P 21 65 * metal2
P 34 65 * metal2
W 19 63 36 67 out_2_s
S 2 H metal2 P 35 P 36
P 21 139 * metal2
P 34 139 * metal2
W 19 137 36 141 out_1_s
S 2 H metal2 P 37 P 38
P 8 129 * metal2
P 34 129 * metal2
W 6 127 36 131
S 2 H metal2 P 39 P 40
P 27 141 * ndif
P 27 147 * ndif
W 25 139 29 149
S 2 V ndif P 41 P 42
P 15 141 * ndif
P 15 147 * ndif
W 13 139 17 149
S 2 V ndif P 43 P 44
P 8 50 * metal2
P 34 50 * metal2
W 6 48 36 52
S 2 H metal2 P 45 P 46
P 27 75 * metal2
P 34 75 * metal2
W 25 73 36 77
S 2 H metal2 P 47 P 48
P 11 65 * poly
P 18 65 * poly
W 10 64 19 66
S 1 H poly P 49 P 50
P 27 52 * metal
P 27 75 * metal
W 26 51 28 76
S 1 V metal P 51 P 52
P 15 111 * metal
P 27 111 * metal
W 14 110 28 112
S 1 H metal P 53 P 54
P 21 60 * metal
P 21 65 * metal
W 20 59 22 66 out_2_s
S 1 V metal P 55 P 56
P 21 57 * ndif
P 21 63 * ndif
W 19 55 23 65
S 2 V ndif P 57 P 58
P 24 55 * ntrans
P 24 65 * ntrans
W 23 54 25 66
S 1 V ntrans P 59 P 60
P 18 55 * ntrans
P 18 65 * ntrans
W 17 54 19 66
S 1 V ntrans P 61 P 62
P 27 154 * metal2
P 34 154 * metal2
W 25 152 36 156
S 2 H metal2 P 63 P 64
P 8 154 * metal2
P 15 154 * metal2
W 6 152 17 156 in_1
S 2 H metal2 P 65 P 66
P 21 139 * metal
P 21 144 * metal
W 20 138 22 145 out_1_s
S 1 V metal P 67 P 68
P 24 139 * poly
P 31 139 * poly
W 23 138 32 140
S 1 H poly P 69 P 70
P 11 139 * poly
P 18 139 * poly
W 10 138 19 140
S 1 H poly P 71 P 72
P 21 141 * ndif
P 21 147 * ndif
W 19 139 23 149
S 2 V ndif P 73 P 74
P 24 139 * ntrans
P 24 149 * ntrans
W 23 138 25 150
S 1 V ntrans P 75 P 76
P 27 111 * metal
P 27 154 * metal
W 26 110 28 155
S 1 V metal P 77 P 78
P 31 42 * metal
P 31 162 * metal
W 30 41 32 163
S 1 V metal P 79 P 80
P 11 42 * metal
P 11 162 * metal
W 10 41 12 163
S 1 V metal P 81 P 82
P 18 139 * ntrans
P 18 149 * ntrans
W 17 138 19 150
S 1 V ntrans P 83 P 84
P 8 162 * metal2
P 34 162 * metal2
W 0 154 42 170
S 8 H metal2 P 85 P 86
P 8 149 * allowM2
P 34 149 * allowM2
W 6 147 36 151
S 2 H allowM2 P 87 P 88
P 8 134 * allowM2
P 34 134 * allowM2
W 6 132 36 136
S 2 H allowM2 P 89 P 90
P 8 124 * allowM2
P 34 124 * allowM2
W 6 122 36 126
S 2 H allowM2 P 91 P 92
P 8 119 * allowM2
P 34 119 * allowM2
W 6 117 36 121
S 2 H allowM2 P 93 P 94
P 8 114 * allowM2
P 34 114 * allowM2
W 6 112 36 116
S 2 H allowM2 P 95 P 96
P 8 109 * allowM2
P 34 109 * allowM2
W 6 107 36 111
S 2 H allowM2 P 97 P 98
P 8 102 * metal2
P 34 102 * metal2
W 2 96 40 108
S 6 H metal2 P 99 P 100
P 8 90 * allowM2
P 34 90 * allowM2
W 6 88 36 92
S 2 H allowM2 P 101 P 102
P 8 85 * allowM2
P 34 85 * allowM2
W 6 83 36 87
S 2 H allowM2 P 103 P 104
P 8 95 * allowM2
P 34 95 * allowM2
W 6 93 36 97
S 2 H allowM2 P 105 P 106
P 8 70 * allowM2
P 34 70 * allowM2
W 6 68 36 72
S 2 H allowM2 P 107 P 108
P 8 55 * allowM2
P 34 55 * allowM2
W 6 53 36 57
S 2 H allowM2 P 109 P 110
P 8 42 * metal2
P 34 42 * metal2
W 0 34 42 50
S 8 H metal2 P 111 P 112
E
