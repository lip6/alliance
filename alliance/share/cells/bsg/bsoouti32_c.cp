#cell1 bsoouti32_c idps_symb compose 41984 v7r5.6
# 3-Dec-93 15:04 21-Sep-93 11:28 bsg * .ly ref_con ref_ref cpf cvia cdn cdp
# cbn cbp .
V 4 VTIcompose 1.0
A 30 42 46 162
B 22 34 54 170
F F
C dat_2 30 60 metal2 2 1 * * W dat_2
C out2 30 65 metal2 2 2 * * W out_2_s
C vss1 30 162 metal2 8 3 * * W vss1
C vss1 46 162 metal2 8 4 * * E vss1
C vss 30 42 metal2 8 5 * * W vss
C vss 46 42 metal2 8 6 * * E vss
C vdd 46 102 metal2 6 7 * * E vdd
C vdd 30 102 metal2 6 8 * * W vdd
I cbp1 43 146 0 "cbp" ly * *
I cbp2 43 153 0 "cbp" ly * *
I cbn1 43 112 0 "cbn" ly * *
I cbn2 43 118 0 "cbn" ly * *
I cbn3 43 124 0 "cbn" ly * *
I cdp1 43 96 0 "cdp" ly * *
I cdp2 43 85 0 "cdp" ly * *
I cdp3 43 80 0 "cdp" ly * *
I cdp4 37 85 0 "cdp" ly * *
I cdn1 37 58 0 "cdn" ly * *
I cdn2 43 58 0 "cdn" ly * *
I cdn3 43 51 0 "cdn" ly * *
I via1 43 45 0 "cvia" ly * *
I via2 33 45 0 "cvia" ly * *
I cdp5 37 78 0 "cdp" ly * *
I cpf1 33 70 0 "cpf" ly * *
I via3 33 65 0 "cvia" ly * *
I cdn4 37 51 0 "cdn" ly * *
I cbp3 38 45 0 "cbp" ly * *
I cdp6 43 90 0 "cdp" ly * *
I via4 33 159 0 "cvia" ly * *
I cbp4 38 159 0 "cbp" ly * *
I via5 43 159 0 "cvia" ly * *
I cbn4 38 102 0 "cbn" ly * *
I via6 33 102 0 "cvia" ly * *
I via7 43 102 0 "cvia" ly * *
I cdp7 33 96 0 "cdp" ly * *
I cpf2 38 92 0 "cpf" ly * *
I via8 32 60 0 "cvia" ly * *
I nwell_2 46 90 0 "ref_ref" ly * *
I nwell_1 46 114 0 "ref_ref" ly * *
I out_d_2_4 43 75 0 "ref_con" ly * *
I out_d_2_5 43 70 0 "ref_con" ly * *
I out_d_2_6 43 65 0 "ref_con" ly * *
I in_d_2_7 32 60 0 "ref_con" ly * *
I in_d_2_8 32 55 0 "ref_con" ly * *
I in_d_2_9 32 50 0 "ref_con" ly * *
P 30 134 * allowM2
P 46 134 * allowM2
W 28 132 48 136
S 2 H allowM2 P 1 P 2
P 43 75 * allowM2
P 46 75 * allowM2
W 41 73 48 77
S 2 H allowM2 P 3 P 4
P 30 70 * allowM2
P 46 70 * allowM2
W 28 68 48 72
S 2 H allowM2 P 5 P 6
P 30 55 * allowM2
P 46 55 * allowM2
W 28 53 48 57
S 2 H allowM2 P 7 P 8
P 37 92 * metal
P 38 92 * metal
W 36 91 39 93
S 1 H metal P 9 P 10
P 37 73 * metal
P 37 92 * metal
W 36 72 38 93
S 1 V metal P 11 P 12
P 38 92 * poly
P 38 93 * poly
W 37 91 39 94
S 1 V poly P 13 P 14
P 35 95 * pdif
P 35 97 * pdif
W 32 92 38 100
S 3 V pdif P 15 P 16
P 41 95 * pdif
P 41 97 * pdif
W 38 92 44 100
S 3 V pdif P 17 P 18
P 38 93 * ptrans
P 38 99 * ptrans
W 37 92 39 100
S 1 V ptrans P 19 P 20
P 33 102 * metal
P 43 102 * metal
W 31 100 45 104
S 2 H metal P 21 P 22
P 43 80 * metal
P 43 102 * metal
W 41 78 45 104
S 2 V metal P 23 P 24
P 43 102 * metal
P 43 124 * metal
W 41 100 45 126
S 2 V metal P 25 P 26
P 43 65 * allowM2
P 46 65 * allowM2
W 41 63 48 67
S 2 H allowM2 P 27 P 28
P 32 50 * allowM2
P 46 50 * allowM2
W 30 48 48 52
S 2 H allowM2 P 29 P 30
P 32 50 * metal
P 32 60 * metal
W 31 49 33 61
S 1 V metal P 31 P 32
P 30 102 * metal2
P 46 102 * metal2
W 24 96 52 108
S 6 H metal2 P 33 P 34
P 30 42 * metal2
P 46 42 * metal2
W 22 34 54 50 vss
S 8 H metal2 P 35 P 36
P 30 162 * metal2
P 46 162 * metal2
W 22 154 54 170 vss1
S 8 H metal2 P 37 P 38
P 40 75 * ptrans
P 40 88 * ptrans
W 39 74 41 89
S 1 V ptrans P 39 P 40
P 33 65 * metal
P 33 96 * metal
W 32 64 34 97
S 1 V metal P 41 P 42
P 40 48 * ntrans
P 40 61 * ntrans
W 39 47 41 62
S 1 V ntrans P 43 P 44
P 33 45 * metal
P 43 45 * metal
W 31 43 45 47
S 2 H metal P 45 P 46
P 43 45 * metal
P 43 58 * metal
W 41 43 45 60
S 2 V metal P 47 P 48
P 33 70 * poly
P 40 70 * poly
W 32 69 41 71
S 1 H poly P 49 P 50
P 40 61 * poly
P 40 70 * poly
W 39 60 41 71
S 1 V poly P 51 P 52
P 40 70 * poly
P 40 75 * poly
W 39.5 69.5 41 75.5
S 1 V poly P 53 P 54
P 30 80 * allowM2
P 46 80 * allowM2
W 28 78 48 82
S 2 H allowM2 P 55 P 56
P 33 159 * metal
P 43 159 * metal
W 31 157 45 161
S 2 H metal P 57 P 58
P 30 60 * metal2
P 32 60 * metal2
W 28 58 34 62 dat_2
S 2 H metal2 P 59 P 60
P 30 65 * metal2
P 33 65 * metal2
W 29 64 34 66 out_2_s
S 2 H metal2 P 61 P 62
P 30 90 * allowM2
P 46 90 * allowM2
W 28 88 48 92
S 2 H allowM2 P 63 P 64
P 37 77 * pdif
P 37 86 * pdif
W 35 75 39 88
S 2 V pdif P 65 P 66
P 37 51 * ndif
P 37 59 * ndif
W 35 49 39 61
S 2 V ndif P 67 P 68
P 43 50 * ndif
P 43 59 * ndif
W 41 48 45 61
S 2 V ndif P 69 P 70
P 43 77 * pdif
P 43 97 * pdif
W 40 74 46 100
S 3 V pdif P 71 P 72
P 30 124 * allowM2
P 46 124 * allowM2
W 28 122 48 126
S 2 H allowM2 P 73 P 74
P 30 119 * allowM2
P 46 119 * allowM2
W 28 117 48 121
S 2 H allowM2 P 75 P 76
P 30 114 * allowM2
P 46 114 * allowM2
W 28 112 48 116
S 2 H allowM2 P 77 P 78
P 30 109 * allowM2
P 46 109 * allowM2
W 28 107 48 111
S 2 H allowM2 P 79 P 80
P 30 85 * allowM2
P 46 85 * allowM2
W 28 83 48 87
S 2 H allowM2 P 81 P 82
P 30 95 * allowM2
P 46 95 * allowM2
W 28 93 48 97
S 2 H allowM2 P 83 P 84
P 30 154 * allowM2
P 46 154 * allowM2
W 28 152 48 156
S 2 H allowM2 P 85 P 86
P 43 146 * metal
P 43 159 * metal
W 41 144 45 161
S 2 V metal P 87 P 88
P 37 65 * metal
P 37 73 * metal
W 36 64 38 74 out_2
S 1 V metal P 89 P 90
P 37 51 * metal
P 37 65 * metal
W 36 50 38 66 out_2
S 1 V metal P 91 P 92
P 37 65 * metal
P 43 65 * metal
W 36 64 44 66 out_2
S 1 H metal P 93 P 94
P 43 65 * metal
P 43 75 * metal
W 42 64 44 76 out_2
S 1 V metal P 95 P 96
P 30 149 * allowM2
P 46 149 * allowM2
W 28 147 48 151
S 2 H allowM2 P 97 P 98
P 30 144 * allowM2
P 46 144 * allowM2
W 28 142 48 146
S 2 H allowM2 P 99 P 100
P 29 102 * nwell
P 47 102 * nwell
W 27 76 49 128
S 52 H nwell P 101 P 102
E
