#cell1 bsmmati32_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A 9 42 34 162
B 9 42 34 162
F F
C scom_1 9 129 metal2 2 1 * * W scom_1
C com_2 31 162 metal 1 2 * * N com_2
C vdd 9 102 metal2 6 3 * * W vdd
C vdd 34 102 metal2 6 4 * * E vdd
C in_4 34 75 metal2 2 5 * * E in_4
C out_2_s 9 65 metal2 2 6 * * W out_2_s
C out_2_s 34 65 metal2 2 7 * * E out_2_s
C dat_2 9 60 metal2 2 8 * * W dat_2
C dat_2 34 60 metal2 2 9 * * E dat_2
C com_1 11 42 metal 1 10 * * S com_1
C in_3 15 42 metal 1 11 * * S in_3
C in_4 19 42 metal 1 12 * * S in_4
C com_2 31 42 metal 1 13 * * S com_2
C com_1 11 162 metal 1 14 * * N com_1
C vss 34 42 metal2 8 15 * * E vss
C vss 9 42 metal2 8 16 * * W vss
C vss1 34 162 metal2 8 17 * * E vss1
C vss1 9 162 metal2 8 18 * * W vss1
C scom_1 34 129 metal2 2 19 * * E scom_1
I via1 27 162 0 "cvia" ly * *
I via2 27 42 0 "cvia" ly * *
I cpf1 11 65 0 "cpf" ly * *
I cpf2 31 55 0 "cpf" ly * *
I via3 27 75 0 "cvia" ly * *
I cdn1 21 60 0 "cdn" ly * *
I via4 21 65 0 "cvia" ly * *
I cdn2 27 60 0 "cdn" ly * *
I cdn3 15 60 0 "cdn" ly * *
I via5 27 102 0 "cvia" ly * *
I cbn1 23 102 0 "cbn" ly * *
I cbp1 23 162 0 "cbp" ly * *
I cbp2 23 42 0 "cbp" ly * *
I ref_cr_1 31 129 0 "ref_con" ly * *
I ref_cl_1 11 129 0 "ref_con" ly * *
P 9 129 * metal2
P 34 129 * metal2
W 7 127 36 131
S 2 H metal2 P 1 P 2
P 9 42 * metal2
P 34 42 * metal2
W 1 34 42 50 vss
S 8 H metal2 P 3 P 4
P 23 42 * metal
P 27 42 * metal
W 21 40 29 44
S 2 H metal P 5 P 6
P 9 144 * allowM2
P 34 144 * allowM2
W 7 142 36 146 in1
S 2 H allowM2 P 7 P 8
P 9 60 * metal2
P 34 60 * metal2
W 7 58 36 62
S 2 H metal2 P 9 P 10
P 23 102 * metal
P 27 102 * metal
W 21 100 29 104
S 2 H metal P 11 P 12
P 19 52 * metal
P 27 52 * metal
W 18 51 28 53
S 1 H metal P 13 P 14
P 19 42 * metal
P 19 52 * metal
W 18 41 20 53
S 1 V metal P 15 P 16
P 23 162 * metal
P 27 162 * metal
W 21 160 29 164
S 2 H metal P 17 P 18
P 8 102 * nwell
P 35 102 * nwell
W -44 50 87 154
S 52 H nwell P 19 P 20
P 27 57 * ndif
P 27 63 * ndif
W 25 55 29 65
S 2 V ndif P 21 P 22
P 15 57 * ndif
P 15 63 * ndif
W 13 55 17 65
S 2 V ndif P 23 P 24
P 9 65 * metal2
P 34 65 * metal2
W 7 63 36 67 out_2_s
S 2 H metal2 P 25 P 26
P 27 75 * metal2
P 34 75 * metal2
W 25 73 36 77
S 2 H metal2 P 27 P 28
P 11 65 * poly
P 18 65 * poly
W 10 64 19 66
S 1 H poly P 29 P 30
P 24 55 * poly
P 31 55 * poly
W 23 54 32 56
S 1 H poly P 31 P 32
P 27 52 * metal
P 27 75 * metal
W 26 51 28 76
S 1 V metal P 33 P 34
P 21 60 * metal
P 21 65 * metal
W 20 59 22 66 out_2_s
S 1 V metal P 35 P 36
P 21 57 * ndif
P 21 63 * ndif
W 19 55 23 65
S 2 V ndif P 37 P 38
P 24 55 * ntrans
P 24 65 * ntrans
W 23 54 25 66
S 1 V ntrans P 39 P 40
P 18 55 * ntrans
P 18 65 * ntrans
W 17 54 19 66
S 1 V ntrans P 41 P 42
P 31 42 * metal
P 31 162 * metal
W 30 41 32 163
S 1 V metal P 43 P 44
P 11 42 * metal
P 11 162 * metal
W 10 41 12 163
S 1 V metal P 45 P 46
P 9 149 * allowM2
P 34 149 * allowM2
W 7 147 36 151
S 2 H allowM2 P 47 P 48
P 9 134 * allowM2
P 34 134 * allowM2
W 7 132 36 136
S 2 H allowM2 P 49 P 50
P 9 124 * allowM2
P 34 124 * allowM2
W 7 122 36 126
S 2 H allowM2 P 51 P 52
P 9 119 * allowM2
P 34 119 * allowM2
W 7 117 36 121
S 2 H allowM2 P 53 P 54
P 9 114 * allowM2
P 34 114 * allowM2
W 7 112 36 116
S 2 H allowM2 P 55 P 56
P 9 109 * allowM2
P 34 109 * allowM2
W 7 107 36 111
S 2 H allowM2 P 57 P 58
P 9 102 * metal2
P 34 102 * metal2
W 3 96 40 108 vdd
S 6 H metal2 P 59 P 60
P 9 90 * allowM2
P 34 90 * allowM2
W 7 88 36 92
S 2 H allowM2 P 61 P 62
P 9 85 * allowM2
P 34 85 * allowM2
W 7 83 36 87
S 2 H allowM2 P 63 P 64
P 9 80 * allowM2
P 34 80 * allowM2
W 7 78 36 82
S 2 H allowM2 P 65 P 66
P 9 95 * allowM2
P 34 95 * allowM2
W 7 93 36 97
S 2 H allowM2 P 67 P 68
P 9 70 * allowM2
P 34 70 * allowM2
W 7 68 36 72
S 2 H allowM2 P 69 P 70
P 9 55 * allowM2
P 34 55 * allowM2
W 7 53 36 57
S 2 H allowM2 P 71 P 72
P 9 162 * metal2
P 34 162 * metal2
W 1 154 42 170 vss1
S 8 H metal2 P 73 P 74
P 15 42 * metal
P 15 60 * metal
W 14 41 16 61
S 1 V metal P 75 P 76
E
