#cell1 bsmlefi32_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A 8 42 34 162
B 8 42 34 162
F F
C com_1 8 50 metal2 2 1 * * W com_1
C vss1 8 162 metal2 8 2 * * W vss1
C vss1 34 162 metal2 8 3 * * E vss1
C com_2 31 162 metal 1 4 * * N com_2
C scom_1 8 129 metal2 2 5 * * W scom_1
C scom_1 34 129 metal2 2 6 * * E scom_1
C vdd 8 102 metal2 6 7 * * W vdd
C vdd 34 102 metal2 6 8 * * E vdd
C dat_2 8 60 metal2 2 9 * * W dat_2
C dat_2 34 60 metal2 2 10 * * E dat_2
C out_2_s 34 65 metal2 2 11 * * E out_2_s
C com_1 11 42 metal 1 12 * * S com_1
C com_2 31 42 metal 1 13 * * S com_2
C vss 8 42 metal2 8 14 * * W vss
C vss 34 42 metal2 8 15 * * E vss
C in_3 15 42 metal 1 16 * * S in_3
C com_1 11 162 metal 1 17 * * N com_1
C in_2 8 75 metal2 2 18 * * W in_2
I via1 11 50 0 "cvia" ly * *
I via2 27 162 0 "cvia" ly * *
I via3 27 42 0 "cvia" ly * *
I cpf1 11 65 0 "cpf" ly * *
I cpf2 31 65 0 "cpf" ly * *
I via4 15 75 0 "cvia" ly * *
I cdn1 21 60 0 "cdn" ly * *
I via5 21 65 0 "cvia" ly * *
I cdn2 27 60 0 "cdn" ly * *
I cdn3 15 60 0 "cdn" ly * *
I via6 27 102 0 "cvia" ly * *
I cbn1 23 102 0 "cbn" ly * *
I cbp1 23 162 0 "cbp" ly * *
I cbp2 23 42 0 "cbp" ly * *
I ref_cl_1 11 129 0 "ref_con" ly * *
I ref_cr_1 31 129 0 "ref_con" ly * *
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
P 15 42 * metal
P 15 52 * metal
W 14 41 16 53
S 1 V metal P 7 P 8
P 8 80 * allowM2
P 34 80 * allowM2
W 6 78 36 82
S 2 H allowM2 P 9 P 10
P 15 60 * metal
P 15 75 * metal
W 14 59 16 76
S 1 V metal P 11 P 12
P 23 42 * metal
P 27 42 * metal
W 21 40 29 44
S 2 H metal P 13 P 14
P 8 144 * allowM2
P 34 144 * allowM2
W 6 142 36 146 dat_1
S 2 H allowM2 P 15 P 16
P 8 60 * metal2
P 34 60 * metal2
W 6 58 36 62
S 2 H metal2 P 17 P 18
P 23 102 * metal
P 27 102 * metal
W 21 100 29 104
S 2 H metal P 19 P 20
P 23 162 * metal
P 27 162 * metal
W 21 160 29 164
S 2 H metal P 21 P 22
P 7 102 * nwell
P 35 102 * nwell
W -45 50 87 154
S 52 H nwell P 23 P 24
P 8 75 * metal2
P 15 75 * metal2
W 6 73 17 77
S 2 H metal2 P 25 P 26
P 21 65 * metal2
P 34 65 * metal2
W 19 63 36 67 out_2_s
S 2 H metal2 P 27 P 28
P 8 129 * metal2
P 34 129 * metal2
W 6 127 36 131
S 2 H metal2 P 29 P 30
P 8 50 * metal2
P 11 50 * metal2
W 6 48 13 52
S 2 H metal2 P 31 P 32
P 11 65 * poly
P 18 65 * poly
W 10 64 19 66
S 1 H poly P 33 P 34
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
P 8 162 * metal2
P 34 162 * metal2
W 0 154 42 170
S 8 H metal2 P 47 P 48
P 8 149 * allowM2
P 34 149 * allowM2
W 6 147 36 151
S 2 H allowM2 P 49 P 50
P 8 134 * allowM2
P 34 134 * allowM2
W 6 132 36 136
S 2 H allowM2 P 51 P 52
P 8 124 * allowM2
P 34 124 * allowM2
W 6 122 36 126
S 2 H allowM2 P 53 P 54
P 8 119 * allowM2
P 34 119 * allowM2
W 6 117 36 121
S 2 H allowM2 P 55 P 56
P 8 114 * allowM2
P 34 114 * allowM2
W 6 112 36 116
S 2 H allowM2 P 57 P 58
P 8 109 * allowM2
P 34 109 * allowM2
W 6 107 36 111
S 2 H allowM2 P 59 P 60
P 8 102 * metal2
P 34 102 * metal2
W 2 96 40 108
S 6 H metal2 P 61 P 62
P 8 90 * allowM2
P 34 90 * allowM2
W 6 88 36 92
S 2 H allowM2 P 63 P 64
P 8 85 * allowM2
P 34 85 * allowM2
W 6 83 36 87
S 2 H allowM2 P 65 P 66
P 8 95 * allowM2
P 34 95 * allowM2
W 6 93 36 97
S 2 H allowM2 P 67 P 68
P 8 70 * allowM2
P 34 70 * allowM2
W 6 68 36 72
S 2 H allowM2 P 69 P 70
P 8 55 * allowM2
P 34 55 * allowM2
W 6 53 36 57
S 2 H allowM2 P 71 P 72
P 8 42 * metal2
P 34 42 * metal2
W 0 34 42 50
S 8 H metal2 P 73 P 74
P 15 52 * metal
P 27 52 * metal
W 14 51 28 53
S 1 H metal P 75 P 76
P 27 52 * metal
P 27 60 * metal
W 26 51 28 61
S 1 V metal P 77 P 78
E
