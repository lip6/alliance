#cell1 grmmx_c idps_symb compose *
# 27-Mar-95 11:33 27-Mar-95 11:33 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 116 40
B 0 0 116 40
F F
C vss 26 40 metal 2 1 * * N vss
C vss 87 40 metal 2 2 * * N vss
C ck_13 56 40 metal 1 3 * * N ck_13
C s1 116 22 metal2 2 4 * * E s1
C s2 116 16 metal2 2 5 * * E s2
C s3 116 11 metal2 2 6 * * E s3
C s0 116 40 metal2 2 7 * * E s0
C vss 87 0 metal 2 8 * * S vss
C vss 0 5 metal2 4 9 * * W vss
C bl1 32 40 metal 1 10 * * N bl1
C bl0 20 40 metal 1 11 * * N bl0
C bl2 81 40 metal 1 12 * * N bl2
C bl3 93 40 metal 1 13 * * N bl3
C vss 116 5 metal2 4 14 * * E vss
C s3 0 11 metal2 2 15 * * W s3
C s2 0 16 metal2 2 16 * * W s2
C s1 0 22 metal2 2 17 * * W s1
C s0 0 40 metal2 2 18 * * W s0
C vdd0 45 40 metal 1 19 * * N vdd0
C vdd1 68 40 metal 1 20 * * N vdd1
C vdd0 45 0 metal 2 21 * * S vdd0
C f 68 0 metal 1 22 * * S f
I cdn1 75 13 0 "cdn" ly * *
I cdn2 32 19 0 "cdn" ly * *
I cpf1 40 17 0 "cpf" ly * *
I cpf2 38 12 0 "cpf" ly * *
I via1 32 28 0 "cvia" ly * *
I cpf3 55 35 0 "cpf" ly * *
I cdp1 50 34 0 "cdp" ly * *
I cpf4 67 20 0 "cpf" ly * *
I cpf5 81 16 0 "cpf" ly * *
I cdn3 75 7 0 "cdn" ly * *
I cdn4 71 13 0 "cdn" ly * *
I cpf6 57 30 0 "cpf" ly * *
I cdn5 87 27 0 "cdn" ly * *
I cdn6 97 27 0 "cdn" ly * *
I cpf7 93 36 0 "cpf" ly * *
I cpf8 94 21 0 "cpf" ly * *
I via2 102 29 0 "cvia" ly * *
I cdp2 62 34 0 "cdp" ly * *
I cdp3 70 34 0 "cdp" ly * *
I cdp4 70 40 0 "cdp" ly * *
I via3 94 11 0 "cvia" ly * *
I via4 65 16 0 "cvia" ly * *
I via5 50 29 0 "cvia" ly * *
I via6 40 21 0 "cvia" ly * *
I via7 26 5 0 "cvia" ly * *
I via8 16 40 0 "cvia" ly * *
I cdn7 14 25 0 "cdn" ly * *
I cdn8 24 25 0 "cdn" ly * *
I via9 11 29 0 "cvia" ly * *
I cpf9 21 31 0 "cpf" ly * *
I cpf10 16 31 0 "cpf" ly * *
I cdp5 43 40 0 "cdp" ly * *
I cdn9 33 9 0 "cdn" ly * *
I cpf11 50 24 0 "cpf" ly * *
I cdn10 72 23 0 "cdn" ly * *
I via10 72 29 0 "cvia" ly * *
I via11 87 5 0 "cvia" ly * *
I cbp1 26 35 0 "cbp" ly * *
I cbp2 87 35 0 "cbp" ly * *
I cbp3 87 17 0 "cbp" ly * *
I cbp4 87 11 0 "cbp" ly * *
I cdp6 50 40 0 "cdp" ly * *
I cdp7 62 40 0 "cdp" ly * *
I cbp5 24 16 0 "cbp" ly * *
I cbp6 18 16 0 "cbp" ly * *
I cbp7 12 16 0 "cbp" ly * *
I cbn1 45 19 0 "cbn" ly * *
I cbn2 50 19 0 "cbn" ly * *
I cbn3 55 19 0 "cbn" ly * *
P 45 19 * ntie
P 55 19 * ntie
W 42 16 58 22
S 3 H ntie P 1 P 2
P 45 19 * metal
P 56 19 * metal
W 43 17 58 21 vdd0
S 2 H metal P 3 P 4
P 45 0 * metal
P 45 19 * metal
W 43 -2 47 21 vdd0
S 2 V metal P 5 P 6
P 45 19 * metal
P 45 40 * metal
W 43 17 47 42 vdd0
S 2 V metal P 7 P 8
P 45 40 * metal
P 50 40 * metal
W 43 38 52 42 vdd0
S 2 H metal P 9 P 10
P 43 40 * metal
P 45 40 * metal
W 41 38 47 42 vdd0
S 2 H metal P 11 P 12
P 52 17 * nwell
P 52 29 * nwell
W 34 -1 70 47
S 18 V nwell P 13 P 14
P 68 0 * metal
P 68 8 * metal
W 67 -1 69 9 f
S 1 V metal P 15 P 16
P 68 8 * metal
P 71 8 * metal
W 67 7 72 9 f
S 1 H metal P 17 P 18
P 61 8 * metal
P 68 8 * metal
W 60 7 69 9 f
S 1 H metal P 19 P 20
P 61 8 * metal
P 61 24 * metal
W 60 7 62 25 f
S 1 V metal P 21 P 22
P 71 8 * metal
P 75 8 * metal
W 70 7 76 9
S 1 H metal P 23 P 24
P 75 7 * metal
P 75 8 * metal
W 74 6 76 9
S 1 V metal P 25 P 26
P 0 5 * metal2
P 116 5 * metal2
W -4 1 120 9
S 4 H metal2 P 27 P 28
P 114 0 * allowM1
P 114 40 * allowM1
W 113 -1 115 41 tr
S 1 V allowM1 P 29 P 30
P 59 10 * poly
P 72 10 * poly
W 58 9 73 11
S 1 H poly P 31 P 32
P 41 24 * poly
P 50 24 * poly
W 40 23 51 25
S 1 H poly P 33 P 34
P 41 24 * poly
P 41 35 * poly
W 40 23 42 36
S 1 V poly P 35 P 36
P 43 30 * pdif
P 52 30 * pdif
W 40 27 55 33
S 3 H pdif P 37 P 38
P 41 35 * ptrans
P 46 35 * ptrans
W 37 31 50 39
S 4 H ptrans P 39 P 40
P 37 16 * poly
P 40 16 * poly
W 36 15 41 17
S 1 H poly P 41 P 42
P 40 16 * poly
P 40 17 * poly
W 39 15 41 18
S 1 V poly P 43 P 44
P 12 16 * ptie
P 24 16 * ptie
W 9 13 27 19
S 3 H ptie P 45 P 46
P 12 16 * metal
P 24 16 * metal
W 10 14 26 18
S 2 H metal P 47 P 48
P 24 22 * ndif
P 24 28 * ndif
W 21 19 27 31
S 3 V ndif P 49 P 50
P 14 22 * ndif
P 14 28 * ndif
W 12 20 16 30
S 2 V ndif P 51 P 52
P 24 25 * metal
P 26 25 * metal
W 22 23 28 27
S 2 H metal P 53 P 54
P 26 9 * metal
P 26 25 * metal
W 24 7 28 27
S 2 V metal P 55 P 56
P 26 25 * metal
P 26 40 * metal
W 24 23 28 42
S 2 V metal P 57 P 58
P 11 25 * metal
P 14 25 * metal
W 9 23 16 27
S 2 H metal P 59 P 60
P 11 25 * metal
P 11 29 * metal
W 9 23 13 31
S 2 V metal P 61 P 62
P 17 30 * poly
P 17 31 * poly
W 16 29 18 32
S 1 V poly P 63 P 64
P 16 31 * poly
P 17 31 * poly
W 15 30 18 32
S 1 H poly P 65 P 66
P 20 31 * metal
P 21 31 * metal
W 18 29 23 33
S 2 H metal P 67 P 68
P 20 31 * metal
P 20 40 * metal
W 19 30 21 41
S 1 V metal P 69 P 70
P 26 9 * metal
P 33 9 * metal
W 24 7 35 11
S 2 H metal P 71 P 72
P 29 9 * ndif
P 35 9 * ndif
W 26 6 38 12
S 3 H ndif P 73 P 74
P 38 12 * metal
P 38 13 * metal
W 36 10 40 15
S 2 V metal P 75 P 76
P 36 13 * metal
P 38 13 * metal
W 35 12 39 14
S 1 H metal P 77 P 78
P 70 23 * ndif
P 76 23 * ndif
W 68 21 78 25
S 2 H ndif P 79 P 80
P 65 32 * nwell
P 65 42 * nwell
W 52 19 78 55
S 13 V nwell P 81 P 82
P 78 16 * poly
P 81 16 * poly
W 77 15 82 17
S 1 H poly P 83 P 84
P 90 32 * poly
P 90 35 * poly
W 89 31 91 36
S 1 V poly P 85 P 86
P 26 5 * metal
P 26 9 * metal
W 24 3 28 11
S 2 V metal P 87 P 88
P 62 40 * metal
P 70 40 * metal
W 60 38 72 42
S 2 H metal P 89 P 90
P 94 11 * metal
P 94 21 * metal
W 93 10 95 22
S 1 V metal P 91 P 92
P 102 27 * metal
P 102 29 * metal
W 100 25 104 31
S 2 V metal P 93 P 94
P 97 27 * metal
P 102 27 * metal
W 95 25 104 29
S 2 H metal P 95 P 96
P 87 0 * metal
P 87 40 * metal
W 85 -2 89 42
S 2 V metal P 97 P 98
P 0 40 * metal2
P 116 40 * metal2
W -2 38 118 42
S 2 H metal2 P 99 P 100
P 0 11 * metal2
P 116 11 * metal2
W -2 9 118 13
S 2 H metal2 P 101 P 102
P 0 16 * metal2
P 116 16 * metal2
W -2 14 118 18 e3
S 2 H metal2 P 103 P 104
P 0 22 * metal2
P 116 22 * metal2
W -2 20 118 24
S 2 H metal2 P 105 P 106
P 87 12 * ptie
P 87 16 * ptie
W 85 10 89 18
S 2 V ptie P 107 P 108
P 93 36 * metal
P 93 40 * metal
W 92 35 94 41
S 1 V metal P 109 P 110
P 41 35 * nwell
P 61 35 * nwell
W 27 21 75 49
S 14 H nwell P 111 P 112
P 32 36 * metal
P 36 36 * metal
W 31 35 37 37
S 1 H metal P 113 P 114
P 36 13 * metal
P 36 36 * metal
W 35 12 37 37
S 1 V metal P 115 P 116
P 32 36 * metal
P 32 40 * metal
W 31 35 33 41
S 1 V metal P 117 P 118
P 81 16 * metal
P 81 40 * metal
W 80 15 82 41 n2
S 1 V metal P 119 P 120
P 97 24 * ndif
P 97 30 * ndif
W 95 22 99 32
S 2 V ndif P 121 P 122
P 87 24 * ndif
P 87 30 * ndif
W 84 21 90 33
S 3 V ndif P 123 P 124
P 87 36 * ptie
P 87 39 * ptie
W 85 34 89 41
S 2 V ptie P 125 P 126
P 72 23 * metal
P 72 29 * metal
W 71 22 73 30
S 1 V metal P 127 P 128
P 70 13 * ndif
P 76 13 * ndif
W 67 10 79 16
S 3 H ndif P 129 P 130
P 16 31 * metal
P 16 40 * metal
W 15 30 17 41
S 1 V metal P 131 P 132
P 29 19 * ndif
P 35 19 * ndif
W 26 16 38 22
S 3 H ndif P 133 P 134
P 17 20 * ntrans
P 17 30 * ntrans
W 16 19 18 31
S 1 V ntrans P 135 P 136
P 21 20 * ntrans
P 21 30 * ntrans
W 20 19 22 31
S 1 V ntrans P 137 P 138
P 72 10 * ntrans
P 78 10 * ntrans
W 71 9 79 11
S 1 H ntrans P 139 P 140
P 94 22 * ntrans
P 94 32 * ntrans
W 93 21 95 33
S 1 V ntrans P 141 P 142
P 90 22 * ntrans
P 90 32 * ntrans
W 89 21 91 33
S 1 V ntrans P 143 P 144
P 59 37 * ptrans
P 73 37 * ptrans
W 58 36 74 38
S 1 H ptrans P 145 P 146
P 68 16 * ntrans
P 78 16 * ntrans
W 67 15 79 17
S 1 H ntrans P 147 P 148
P 68 20 * ntrans
P 78 20 * ntrans
W 67 19 79 21
S 1 H ntrans P 149 P 150
P 11 29 * metal2
P 102 29 * metal2
W 9 27 104 31 i_p
S 2 H metal2 P 151 P 152
P 48 37 * ptrans
P 54 37 * ptrans
W 47 36 55 38
S 1 H ptrans P 153 P 154
P 27 12 * ntrans
P 37 12 * ntrans
W 26 11 38 13
S 1 H ntrans P 155 P 156
P 27 16 * ntrans
P 37 16 * ntrans
W 26 15 38 17
S 1 H ntrans P 157 P 158
P 71 13 * metal
P 75 13 * metal
W 69 11 77 15
S 2 H metal P 159 P 160
P 75 12 * metal
P 75 13 * metal
W 74 11 76 14
S 1 V metal P 161 P 162
P 71 12 * metal
P 86 12 * metal
W 70 11 87 13
S 1 H metal P 163 P 164
P 71 12 * metal
P 71 13 * metal
W 69 10 73 15
S 2 V metal P 165 P 166
P 94 21 * poly
P 94 22 * poly
W 93 20 95 23
S 1 V poly P 167 P 168
P 32 19 * metal
P 32 28 * metal
W 31 18 33 29
S 1 V metal P 169 P 170
P 40 17 * metal
P 40 21 * metal
W 38 15 42 23
S 2 V metal P 171 P 172
P 59 10 * poly
P 59 37 * poly
W 58 9 60 38
S 1 V poly P 173 P 174
P 57 30 * poly
P 59 30 * poly
W 56 29 60 31
S 1 H poly P 175 P 176
P 54 35 * poly
P 54 37 * poly
W 53 34 55 38
S 1 V poly P 177 P 178
P 54 35 * poly
P 55 35 * poly
W 53 34 56 36
S 1 H poly P 179 P 180
P 56 35 * metal
P 56 40 * metal
W 55 34 57 41
S 1 V metal P 181 P 182
P 55 35 * metal
P 56 35 * metal
W 54 34 57 36
S 1 H metal P 183 P 184
P 67 20 * poly
P 68 20 * poly
W 66 19 69 21
S 1 H poly P 185 P 186
P 61 34 * pdif
P 71 34 * pdif
W 58 31 74 37
S 3 H pdif P 187 P 188
P 61 40 * pdif
P 71 40 * pdif
W 58 37 74 43
S 3 H pdif P 189 P 190
P 90 35 * poly
P 93 35 * poly
W 89 34 94 36
S 1 H poly P 191 P 192
P 93 35 * poly
P 93 36 * poly
W 92 34 94 37
S 1 V poly P 193 P 194
P 65 16 * metal
P 65 20 * metal
W 63 14 67 22
S 2 V metal P 195 P 196
P 65 20 * metal
P 67 20 * metal
W 63 18 69 22
S 2 H metal P 197 P 198
P 50 30 * metal
P 57 30 * metal
W 49 29 58 31
S 1 H metal P 199 P 200
P 50 29 * metal
P 50 30 * metal
W 49 28 51 31
S 1 V metal P 201 P 202
P 50 30 * metal
P 50 34 * metal
W 49 29 51 35
S 1 V metal P 203 P 204
P 43 40 * pdif
P 52 40 * pdif
W 40 37 55 43
S 3 H pdif P 205 P 206
P 50 34 * pdif
P 52 34 * pdif
W 47 31 55 37
S 3 H pdif P 207 P 208
P 69 32 * nwell
P 69 42 * nwell
W 63 26 75 48
S 6 V nwell P 209 P 210
P 50 24 * metal
P 61 24 * metal
W 49 23 62 25
S 1 H metal P 211 P 212
P 61 24 * metal
P 61 34 * metal
W 60 23 62 35
S 1 V metal P 213 P 214
P 61 34 * metal
P 62 34 * metal
W 59 32 64 36
S 2 H metal P 215 P 216
P 62 34 * metal
P 70 34 * metal
W 60 32 72 36
S 2 H metal P 217 P 218
P 49 32 * pdif
P 52 32 * pdif
W 46 29 55 35
S 3 H pdif P 219 P 220
P 43 31 * pdif
P 44 31 * pdif
W 41 29 46 33
S 2 H pdif P 221 P 222
E
