#cell1 mx3sc_fp idps_symb compose *
# 20-Nov-97 10:57 20-Nov-97 10:57 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 54 65
B 5 5 54 65
F F
C s3i0 30 65 metal 1 1 * * N s3i0
C s2i0 39 65 metal 1 2 * * N s2i0
C s0i0 12 65 metal 1 3 * * N s0i0
C s2i0 39 5 metal 1 4 * * S s2i0
C s3i0 30 5 metal 1 5 * * S s3i0
C s0i0 12 5 metal 1 6 * * S s0i0
C s1i0 21 65 metal 1 7 * * N s1i0
C s1i0 21 5 metal 1 8 * * S s1i0
C vdd 5 65 metal2 6 9 * * W vdd
C vss 5 5 metal2 8 10 * * W vss
C vdd 54 65 metal2 6 11 * * E vdd
C vss 54 5 metal2 8 12 * * E vss
I cpf1 16 65 0 "cpf" ly * *
I cdp1 50 58 0 "cdp" ly * *
I cdp2 50 51 0 "cdp" ly * *
I cpf2 48 45 0 "cpf" ly * *
I cpf3 48 21 0 "cpf" ly * *
I cdp3 44 59 0 "cdp" ly * *
I cpf4 51 32 0 "cpf" ly * *
I cpf5 17 29 0 "cpf" ly * *
I cdn1 17 19 0 "cdn" ly * *
I cdn2 8 13 0 "cdn" ly * *
I cdn3 26 13 0 "cdn" ly * *
I cbp1 8 5 0 "cbp" ly * *
I via1 8 8 0 "cvia" ly * *
I cdn4 17 13 0 "cdn" ly * *
I cbp2 26 5 0 "cbp" ly * *
I via2 26 8 0 "cvia" ly * *
I cbp3 47 5 0 "cbp" ly * *
I cpf6 21 5 0 "cpf" ly * *
I cdn5 50 13 0 "cdn" ly * *
I cdn6 44 13 0 "cdn" ly * *
I cbn1 47 65 0 "cbn" ly * *
I via3 44 65 0 "cvia" ly * *
I via4 44 5 0 "cvia" ly * *
I via5 50 65 0 "cvia" ly * *
I cpf7 25 37 0 "cpf" ly * *
I cpf8 25 24 0 "cpf" ly * *
I cpf9 8 28 0 "cpf" ly * *
I cdp4 17 43 0 "cdp" ly * *
I cdp5 17 53 0 "cdp" ly * *
I cdp6 44 53 0 "cdp" ly * *
I cdp7 35 48 0 "cdp" ly * *
I cpf10 31 5 0 "cpf" ly * *
I cpf11 39 5 0 "cpf" ly * *
I cpf12 34 65 0 "cpf" ly * *
I cpf13 34 60 0 "cpf" ly * *
I cdp8 17 48 0 "cdp" ly * *
I cdp9 8 53 0 "cdp" ly * *
I cbn2 8 62 0 "cbn" ly * *
I cpf14 13 5 0 "cpf" ly * *
I cpf15 16 60 0 "cpf" ly * *
I via6 8 65 0 "cvia" ly * *
I via7 26 65 0 "cvia" ly * *
I cbn3 26 62 0 "cbn" ly * *
I cdp10 26 53 0 "cdp" ly * *
I cdn7 35 18 0 "cdn" ly * *
I i0_8 8 18 0 "ref_con" ly * *
I i2_7 44 23 0 "ref_con" ly * *
I nwell_28 53 51 0 "ref_ref" ly * *
I i1_8 26 18 0 "ref_con" ly * *
I i1_5 26 33 0 "ref_con" ly * *
I o_4 35 38 0 "ref_con" ly * *
I o_6 35 28 0 "ref_con" ly * *
I o_5 35 33 0 "ref_con" ly * *
I o_7 35 23 0 "ref_con" ly * *
I o_3 35 43 0 "ref_con" ly * *
I o_1 35 53 0 "ref_con" ly * *
I i0_3 8 43 0 "ref_con" ly * *
I i0_5 8 33 0 "ref_con" ly * *
I i0_4 8 38 0 "ref_con" ly * *
I i0_2 8 48 0 "ref_con" ly * *
I i0_7 8 23 0 "ref_con" ly * *
I i1_2 26 48 0 "ref_con" ly * *
I i1_3 26 43 0 "ref_con" ly * *
I i2_2 44 48 0 "ref_con" ly * *
I i2_3 44 43 0 "ref_con" ly * *
I i2_6 44 28 0 "ref_con" ly * *
I i2_8 44 18 0 "ref_con" ly * *
I o_9 35 13 0 "ref_con" ly * *
I i2_4 44 38 0 "ref_con" ly * *
P 34 60 * poly
P 32 60 * poly
W 29 57 37 63
S 3 H poly P 1 P 2
P 32 60 * poly
P 32 59 * poly
W 31 58 33 61
S 1 V poly P 3 P 4
P 14 60 * poly
P 16 60 * poly
W 11 57 19 63
S 3 H poly P 5 P 6
P 14 60 * poly
P 14 59 * poly
W 13 58 15 61
S 1 V poly P 7 P 8
P 16 65 * poly
P 20 65 * poly
W 15 64 21 66
S 1 H poly P 9 P 10
P 12 65 * metal
P 16 65 * metal
W 11 64 17 66
S 1 H metal P 11 P 12
P 8 66 * ntie
P 8 61 * ntie
W 5 58 11 69
S 3 V ntie P 13 P 14
P 26 66 * ntie
P 26 61 * ntie
W 23 58 29 69
S 3 V ntie P 15 P 16
P 43 65 * ntie
P 51 65 * ntie
W 40 62 54 68
S 3 H ntie P 17 P 18
P 50 50 * metal
P 50 58 * metal
W 49 49 51 59
S 1 V metal P 19 P 20
P 52 50 * metal
P 50 50 * metal
W 49 49 53 51
S 1 H metal P 21 P 22
P 52 13 * metal
P 52 50 * metal
W 51 12 53 51 ni2
S 1 V metal P 23 P 24
P 47 45 * poly
P 48 45 * poly
W 46 44 49 46
S 1 H poly P 25 P 26
P 47 47 * poly
P 47 45 * poly
W 46 44 48 48
S 1 V poly P 27 P 28
P 44 45 * metal
P 48 45 * metal
W 42 43 50 47
S 2 H metal P 29 P 30
P 47 17 * poly
P 47 21 * poly
W 46 16 48 22
S 1 V poly P 31 P 32
P 47 21 * poly
P 48 21 * poly
W 46 20 49 22
S 1 H poly P 33 P 34
P 44 21 * metal
P 48 21 * metal
W 42 19 50 23
S 2 H metal P 35 P 36
P 26 33 * metal
P 26 18 * metal
W 25 17 27 34
S 1 V metal P 37 P 38
P 26 37 * metal
P 26 48 * metal
W 25 36 27 49 i1
S 1 V metal P 39 P 40
P 11 22 * poly
P 11 38 * poly
W 10 21 12 39
S 1 V poly P 41 P 42
P 23 22 * poly
P 23 25 * poly
W 22 21 24 26
S 1 V poly P 43 P 44
P 29 22 * poly
P 29 38 * poly
W 28 21 30 39
S 1 V poly P 45 P 46
P 41 22 * poly
P 41 38 * poly
W 40 21 42 39
S 1 V poly P 47 P 48
P 44 10 * ndif
P 44 20 * ndif
W 41 7 47 23
S 3 V ndif P 49 P 50
P 41 32 * poly
P 51 32 * poly
W 40 31 52 33
S 1 H poly P 51 P 52
P 51 32 * metal
P 52 32 * metal
W 50 31 53 33
S 1 H metal P 53 P 54
P 17 29 * poly
P 29 29 * poly
W 16 28 30 30
S 1 H poly P 55 P 56
P 8 18 * metal
P 8 48 * metal
W 7 17 9 49 i0
S 1 V metal P 57 P 58
P 17 13 * metal
P 17 53 * metal
W 16 12 18 54 o1
S 1 V metal P 59 P 60
P 8 5 * metal
P 8 13 * metal
W 6 3 10 15
S 2 V metal P 61 P 62
P 20 10 * ntrans
P 20 22 * ntrans
W 19 9 21 23
S 1 V ntrans P 63 P 64
P 14 10 * ntrans
P 14 22 * ntrans
W 13 9 15 23
S 1 V ntrans P 65 P 66
P 26 12 * ndif
P 26 20 * ndif
W 23 9 29 23
S 3 V ndif P 67 P 68
P 23 10 * ntrans
P 23 22 * ntrans
W 22 9 24 23
S 1 V ntrans P 69 P 70
P 26 12 * ndif
P 26 20 * ndif
W 23 9 29 23
S 3 V ndif P 71 P 72
P 29 10 * ntrans
P 29 22 * ntrans
W 28 9 30 23
S 1 V ntrans P 73 P 74
P 32 10 * ntrans
P 32 22 * ntrans
W 31 9 33 23
S 1 V ntrans P 75 P 76
P 11 10 * ntrans
P 11 22 * ntrans
W 10 9 12 23
S 1 V ntrans P 77 P 78
P 8 12 * ndif
P 8 20 * ndif
W 5 9 11 23
S 3 V ndif P 79 P 80
P 17 12 * ndif
P 17 20 * ndif
W 14 9 20 23
S 3 V ndif P 81 P 82
P 35 12 * ndif
P 35 20 * ndif
W 32 9 38 23
S 3 V ndif P 83 P 84
P 38 10 * ntrans
P 38 22 * ntrans
W 37 9 39 23
S 1 V ntrans P 85 P 86
P 41 10 * ntrans
P 41 22 * ntrans
W 40 9 42 23
S 1 V ntrans P 87 P 88
P 38 5 * poly
P 38 10 * poly
W 37 4 39 11
S 1 V poly P 89 P 90
P 32 5 * poly
P 32 10 * poly
W 31 4 33 11
S 1 V poly P 91 P 92
P 43 5 * ptie
P 48 5 * ptie
W 40 2 51 8
S 3 H ptie P 93 P 94
P 31 5 * metal
P 30 5 * metal
W 29 4 32 6
S 1 H metal P 95 P 96
P 31 5 * poly
P 32 5 * poly
W 30 4 33 6
S 1 H poly P 97 P 98
P 21 5 * poly
P 20 5 * poly
W 19 4 22 6
S 1 H poly P 99 P 100
P 20 4 * poly
P 20 10 * poly
W 19 3 21 11
S 1 V poly P 101 P 102
P 14 5 * poly
P 14 10 * poly
W 13 4 15 11
S 1 V poly P 103 P 104
P 21 5 * metal
P 21 65 * metal
W 20 4 22 66 s1i0
S 1 V metal P 105 P 106
P 26 33 * metal
P 26 37 * metal
W 24 31 28 39
S 2 V metal P 107 P 108
P 5 5 * metal2
P 54 5 * metal2
W -3 -3 62 13
S 8 H metal2 P 109 P 110
P 5 65 * metal2
P 54 65 * metal2
W -1 59 60 71
S 6 H metal2 P 111 P 112
P 5 53 * nwell
P 54 53 * nwell
W -23 25 82 81
S 28 H nwell P 113 P 114
P 5 13 * allowM2
P 54 13 * allowM2
W 3 11 56 15
S 2 H allowM2 P 115 P 116
P 5 18 * allowM2
P 54 18 * allowM2
W 3 16 56 20
S 2 H allowM2 P 117 P 118
P 5 23 * allowM2
P 54 23 * allowM2
W 3 21 56 25
S 2 H allowM2 P 119 P 120
P 5 28 * allowM2
P 54 28 * allowM2
W 3 26 56 30
S 2 H allowM2 P 121 P 122
P 5 33 * allowM2
P 54 33 * allowM2
W 3 31 56 35
S 2 H allowM2 P 123 P 124
P 5 38 * allowM2
P 54 38 * allowM2
W 3 36 56 40
S 2 H allowM2 P 125 P 126
P 5 43 * allowM2
P 54 43 * allowM2
W 3 41 56 45
S 2 H allowM2 P 127 P 128
P 5 48 * allowM2
P 54 48 * allowM2
W 3 46 56 50
S 2 H allowM2 P 129 P 130
P 5 53 * allowM2
P 54 53 * allowM2
W 3 51 56 55
S 2 H allowM2 P 131 P 132
P 5 58 * allowM2
P 54 58 * allowM2
W 3 56 56 60
S 2 H allowM2 P 133 P 134
P 44 5 * metal
P 44 13 * metal
W 42 3 46 15
S 2 V metal P 135 P 136
P 44 53 * metal
P 44 65 * metal
W 42 51 46 67
S 2 V metal P 137 P 138
P 47 8 * ntrans
P 47 17 * ntrans
W 46 7 48 18
S 1 V ntrans P 139 P 140
P 50 10 * ndif
P 50 15 * ndif
W 47 7 53 18
S 3 V ndif P 141 P 142
P 44 49 * pdif
P 44 60 * pdif
W 41 46 47 63
S 3 V pdif P 143 P 144
P 47 47 * ptrans
P 47 62 * ptrans
W 46 46 48 63
S 1 V ptrans P 145 P 146
P 50 49 * pdif
P 50 60 * pdif
W 47 46 53 63
S 3 V pdif P 147 P 148
P 44 65 * metal
P 50 65 * metal
W 42 63 52 67
S 2 H metal P 149 P 150
P 44 5 * metal
P 44 13 * metal
W 42 3 46 15
S 2 V metal P 151 P 152
P 23 37 * poly
P 26 37 * poly
W 20 34 29 40
S 3 H poly P 153 P 154
P 23 24 * poly
P 26 24 * poly
W 20 21 29 27
S 3 H poly P 155 P 156
P 23 36 * poly
P 23 38 * poly
W 22 35 24 39
S 1 V poly P 157 P 158
P 25 37 * metal
P 26 37 * metal
W 23 35 28 39
S 2 H metal P 159 P 160
P 25 24 * metal
P 26 24 * metal
W 23 22 28 26
S 2 H metal P 161 P 162
P 26 5 * metal
P 26 13 * metal
W 24 3 28 15
S 2 V metal P 163 P 164
P 8 28 * poly
P 11 28 * poly
W 5 25 14 31
S 3 H poly P 165 P 166
P 32 38 * ptrans
P 32 59 * ptrans
W 31 37 33 60
S 1 V ptrans P 167 P 168
P 29 38 * ptrans
P 29 59 * ptrans
W 28 37 30 60
S 1 V ptrans P 169 P 170
P 44 40 * pdif
P 44 57 * pdif
W 41 37 47 60
S 3 V pdif P 171 P 172
P 41 38 * ptrans
P 41 59 * ptrans
W 40 37 42 60
S 1 V ptrans P 173 P 174
P 35 40 * pdif
P 35 57 * pdif
W 32 37 38 60
S 3 V pdif P 175 P 176
P 38 38 * ptrans
P 38 59 * ptrans
W 37 37 39 60
S 1 V ptrans P 177 P 178
P 34 65 * poly
P 38 65 * poly
W 33 64 39 66
S 1 H poly P 179 P 180
P 38 5 * poly
P 39 5 * poly
W 37 4 40 6
S 1 H poly P 181 P 182
P 38 59 * poly
P 38 65 * poly
W 37 58 39 66
S 1 V poly P 183 P 184
P 35 13 * metal
P 35 53 * metal
W 34 12 36 54 o
S 1 V metal P 185 P 186
P 16 60 * metal
P 21 60 * metal
W 15 59 22 61 s1i0
S 1 H metal P 187 P 188
P 17 40 * pdif
P 17 57 * pdif
W 14 37 20 60
S 3 V pdif P 189 P 190
P 8 40 * pdif
P 8 57 * pdif
W 5 37 11 60
S 3 V pdif P 191 P 192
P 11 38 * ptrans
P 11 59 * ptrans
W 10 37 12 60
S 1 V ptrans P 193 P 194
P 14 38 * ptrans
P 14 59 * ptrans
W 13 37 15 60
S 1 V ptrans P 195 P 196
P 20 38 * ptrans
P 20 59 * ptrans
W 19 37 21 60
S 1 V ptrans P 197 P 198
P 13 5 * poly
P 14 5 * poly
W 12 4 15 6
S 1 H poly P 199 P 200
P 20 59 * poly
P 20 65 * poly
W 19 58 21 66
S 1 V poly P 201 P 202
P 12 5 * metal
P 12 65 * metal
W 11 4 13 66 s0i0
S 1 V metal P 203 P 204
P 8 53 * metal
P 8 65 * metal
W 6 51 10 67
S 2 V metal P 205 P 206
P 26 53 * metal
P 26 65 * metal
W 24 51 28 67
S 2 V metal P 207 P 208
P 23 38 * ptrans
P 23 59 * ptrans
W 22 37 24 60
S 1 V ptrans P 209 P 210
P 26 40 * pdif
P 26 57 * pdif
W 23 37 29 60
S 3 V pdif P 211 P 212
P 26 40 * pdif
P 26 57 * pdif
W 23 37 29 60
S 3 V pdif P 213 P 214
P 50 13 * metal
P 52 13 * metal
W 48 11 54 15
S 2 H metal P 215 P 216
P 30 5 * metal
P 31 5 * metal
W 29 4 32 6
S 1 H metal P 217 P 218
P 12 5 * metal
P 13 5 * metal
W 11 4 14 6
S 1 H metal P 219 P 220
P 30 65 * metal
P 34 65 * metal
W 29 64 35 66
S 1 H metal P 221 P 222
P 34 60 * metal
P 39 60 * metal
W 33 59 40 61 s2i0
S 1 H metal P 223 P 224
P 39 5 * metal
P 39 65 * metal
W 38 4 40 66 s2i0
S 1 V metal P 225 P 226
P 30 5 * metal
P 30 65 * metal
W 29 4 31 66 s3i0
S 1 V metal P 227 P 228
P 26 8 * ptie
P 26 4 * ptie
W 23 1 29 11
S 3 V ptie P 229 P 230
P 8 8 * ptie
P 8 4 * ptie
W 5 1 11 11
S 3 V ptie P 231 P 232
P 44 5 * metal
P 47 5 * metal
W 42 3 49 7
S 2 H metal P 233 P 234
P 44 23 * metal
P 44 18 * metal
W 42 16 46 25
S 2 V metal P 235 P 236
P 44 21 * metal
P 44 43 * metal
W 43 20 45 44 i2
S 1 V metal P 237 P 238
P 44 48 * metal
P 44 43 * metal
W 42 41 46 50
S 2 V metal P 239 P 240
E
