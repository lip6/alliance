#cell1 ammuxi1_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 79 65
B 2 5 79 65
F F
C pi2j 79 33 metal 1 1 * * E pi2j
C n2j 63 65 metal 1 2 * * N n2j
C c2j 33 65 metal 1 3 * * N c2j
C d2j 11 65 metal 1 4 * * N d2j
C vdd 79 65 metal2 2 5 * * E vdd
C vss 2 5 metal2 2 6 * * W vss
C vss 79 5 metal2 2 7 * * E vss
C ai 7 5 poly 1 8 * * S ai
C d2j 11 5 metal 1 9 * * S d2j
C c2j 33 5 metal 1 10 * * S c2j
C n2j 63 5 metal 1 11 * * S n2j
C vdd 2 65 metal2 2 12 * * W vdd
C sum 2 23 metal2 2 13 * * W sum
C sum 79 23 metal2 2 14 * * E sum
C aim1 7 65 poly 1 15 * * N aim1
C d2jbar 22 5 metal 1 16 * * S d2jbar
C d2jbar 22 5 metal 1 17 * * S d2jbar
C d2jbar 22 65 metal 1 18 * * N d2jbar
C ai 2 38 metal2 2 19 * * W ai
C ai 79 38 metal2 2 20 * * E ai
I via1 4 65 0 "cvia" ly * *
I cdp1 5 44 0 "cdp" ly * *
I cpf1 33 51 0 "cpf" ly * *
I cdp2 47 50 0 "cdp" ly * *
I via2 58 65 0 "cvia" ly * *
I cdp3 76 47 0 "cdp" ly * *
I via3 76 65 0 "cvia" ly * *
I cdp4 17 44 0 "cdp" ly * *
I cdn1 17 16 0 "cdn" ly * *
I cdn2 5 17 0 "cdn" ly * *
I via4 4 5 0 "cvia" ly * *
I cpf2 22 8 0 "cpf" ly * *
I cdn3 27 17 0 "cdn" ly * *
I cdn4 37 17 0 "cdn" ly * *
I cdn5 43 17 0 "cdn" ly * *
I cdp5 38 44 0 "cdp" ly * *
I cpf3 46 9 0 "cpf" ly * *
I cdn6 49 17 0 "cdn" ly * *
I cpf4 63 36 0 "cpf" ly * *
I cdn7 73 22 0 "cdn" ly * *
I cdn8 57 22 0 "cdn" ly * *
I cdn9 57 13 0 "cdn" ly * *
I cdn10 73 13 0 "cdn" ly * *
I cpf5 63 5 0 "cpf" ly * *
I cdn11 67 22 0 "cdn" ly * *
I cdn12 67 17 0 "cdn" ly * *
I cdp6 58 50 0 "cdp" ly * *
I cdp7 67 47 0 "cdp" ly * *
I via5 73 5 0 "cvia" ly * *
I via6 28 65 0 "cvia" ly * *
I via7 57 5 0 "cvia" ly * *
I via8 27 5 0 "cvia" ly * *
I cdp8 29 44 0 "cdp" ly * *
I cpf6 22 35 0 "cpf" ly * *
I cpf7 11 60 0 "cpf" ly * *
I cpf8 9 28 0 "cpf" ly * *
I via9 4 38 0 "cvia" ly * *
I cpf9 4 29 0 "cpf" ly * *
I via10 48 38 0 "cvia" ly * *
I cpf10 33 5 0 "cpf" ly * *
I cpf11 17 31 0 "cpf" ly * *
I cpf12 49 25 0 "cpf" ly * *
I cpf13 48 58 0 "cpf" ly * *
I cpf14 48 31 0 "cpf" ly * *
I cpf15 43 42 0 "cpf" ly * *
I cbp1 57 29 0 "cbp" ly * *
I cbn1 28 59 0 "cbn" ly * *
I cbn2 76 58 0 "cbn" ly * *
I c2j_in 33 58 0 "ref_con" ly * *
I n2j_in 63 53 0 "ref_con" ly * *
P 2 5 * metal2
P 79 5 * metal2
W -6 -3 87 13
S 8 H metal2 P 1 P 2
P 2 65 * metal2
P 79 65 * metal2
W -4 59 85 71
S 6 H metal2 P 3 P 4
P 4 44 * metal
P 4 65 * metal
W 3 43 5 66
S 1 V metal P 5 P 6
P 17 14 * ndif
P 17 19 * ndif
W 14 11 20 22
S 3 V ndif P 7 P 8
P 28 14 * ndif
P 28 19 * ndif
W 25 11 31 22
S 3 V ndif P 9 P 10
P 17 16 * metal
P 17 43 * metal
W 16 15 18 44
S 1 V metal P 11 P 12
P 5 14 * ndif
P 5 19 * ndif
W 2 11 8 22
S 3 V ndif P 13 P 14
P 4 5 * metal
P 4 17 * metal
W 3 4 5 18
S 1 V metal P 15 P 16
P 27 5 * metal
P 27 17 * metal
W 26 4 28 18
S 1 V metal P 17 P 18
P 40 12 * ntrans
P 40 21 * ntrans
W 39 11 41 22
S 1 V ntrans P 19 P 20
P 46 12 * ntrans
P 46 21 * ntrans
W 45 11 47 22
S 1 V ntrans P 21 P 22
P 37 9 * metal
P 46 9 * metal
W 36 8 47 10
S 1 H metal P 23 P 24
P 46 9 * poly
P 46 12 * poly
W 45 8 47 13
S 1 V poly P 25 P 26
P 50 14 * ndif
P 50 19 * ndif
W 47 11 53 22
S 3 V ndif P 27 P 28
P 58 50 * metal
P 58 65 * metal
W 57 49 59 66
S 1 V metal P 29 P 30
P 73 11 * ndif
P 73 24 * ndif
W 70 8 76 27
S 3 V ndif P 31 P 32
P 57 11 * ndif
P 57 24 * ndif
W 54 8 60 27
S 3 V ndif P 33 P 34
P 57 5 * metal
P 57 22 * metal
W 56 4 58 23
S 1 V metal P 35 P 36
P 76 47 * metal
P 76 65 * metal
W 75 46 77 66
S 1 V metal P 37 P 38
P 35 39 * ptrans
P 35 48 * ptrans
W 34 38 36 49
S 1 V ptrans P 39 P 40
P 50 45 * ptrans
P 50 54 * ptrans
W 49 44 51 55
S 1 V ptrans P 41 P 42
P 55 45 * ptrans
P 55 54 * ptrans
W 54 44 56 55
S 1 V ptrans P 43 P 44
P 70 42 * ptrans
P 70 51 * ptrans
W 69 41 71 52
S 1 V ptrans P 45 P 46
P 73 42 * ptrans
P 73 51 * ptrans
W 72 41 74 52
S 1 V ptrans P 47 P 48
P 26 39 * ptrans
P 26 48 * ptrans
W 25 38 27 49
S 1 V ptrans P 49 P 50
P 60 9 * ntrans
P 60 26 * ntrans
W 59 8 61 27
S 1 V ntrans P 51 P 52
P 70 9 * ntrans
P 70 26 * ntrans
W 69 8 71 27
S 1 V ntrans P 53 P 54
P 67 33 * metal
P 79 33 * metal
W 66 32 80 34 pi2j
S 1 H metal P 55 P 56
P 73 5 * metal
P 73 22 * metal
W 72 4 74 23
S 1 V metal P 57 P 58
P 28 54 * metal
P 28 65 * metal
W 27 53 29 66
S 1 V metal P 59 P 60
P 65 11 * ndif
P 65 24 * ndif
W 60 6 70 29
S 5 V ndif P 61 P 62
P 60 9 * poly
P 63 9 * poly
W 59 8 64 10
S 1 H poly P 63 P 64
P 63 5 * poly
P 63 9 * poly
W 62 4 64 10
S 1 V poly P 65 P 66
P 33 5 * metal
P 33 65 * metal
W 32 4 34 66 c2j
S 1 V metal P 67 P 68
P 22 5 * metal
P 22 65 * metal
W 21 4 23 66 d2jbar
S 1 V metal P 69 P 70
P 7 48 * poly
P 7 65 * poly
W 6 47 8 66
S 1 V poly P 71 P 72
P 8 39 * ptrans
P 8 48 * ptrans
W 7 38 9 49
S 1 V ptrans P 73 P 74
P 70 26 * poly
P 73 26 * poly
W 69 25 74 27
S 1 H poly P 75 P 76
P 14 39 * ptrans
P 14 48 * ptrans
W 13 38 15 49
S 1 V ptrans P 77 P 78
P 20 39 * ptrans
P 20 48 * ptrans
W 19 38 21 49
S 1 V ptrans P 79 P 80
P 66 44 * pdif
P 66 49 * pdif
W 63 41 69 52
S 3 V pdif P 81 P 82
P 58 47 * pdif
P 58 52 * pdif
W 55 44 61 55
S 3 V pdif P 83 P 84
P 17 41 * pdif
P 17 46 * pdif
W 15 39 19 48
S 2 V pdif P 85 P 86
P 11 41 * pdif
P 11 46 * pdif
W 9 39 13 48
S 2 V pdif P 87 P 88
P 5 41 * pdif
P 5 46 * pdif
W 2 38 8 49
S 3 V pdif P 89 P 90
P 30 41 * pdif
P 30 46 * pdif
W 25 36 35 51
S 5 V pdif P 91 P 92
P 47 47 * pdif
P 47 52 * pdif
W 44 44 50 55
S 3 V pdif P 93 P 94
P 39 41 * pdif
P 39 46 * pdif
W 36 38 42 49
S 3 V pdif P 95 P 96
P 11 14 * ndif
P 11 19 * ndif
W 9 12 13 21
S 2 V ndif P 97 P 98
P 7 48 * poly
P 8 48 * poly
W 6 47 9 49
S 1 H poly P 99 P 100
P 7 5 * poly
P 7 9 * poly
W 6 4 8 10
S 1 V poly P 101 P 102
P 7 9 * poly
P 8 9 * poly
W 6 8 9 10
S 1 H poly P 103 P 104
P 2 52 * nwell
P 79 52 * nwell
W -24 26 105 78
S 26 H nwell P 105 P 106
P 11 5 * metal
P 11 23 * metal
W 10 4 12 24
S 1 V metal P 107 P 108
P 11 48 * metal
P 12 48 * metal
W 10 47 13 49
S 1 H metal P 109 P 110
P 12 34 * metal
P 12 48 * metal
W 11 33 13 49 d2j
S 1 V metal P 111 P 112
P 9 34 * metal
P 12 34 * metal
W 8 33 13 35
S 1 H metal P 113 P 114
P 11 48 * metal
P 11 65 * metal
W 10 47 12 66
S 1 V metal P 115 P 116
P 67 17 * metal
P 67 47 * metal
W 66 16 68 48
S 1 V metal P 117 P 118
P 63 5 * metal
P 63 65 * metal
W 62 4 64 66 n2j
S 1 V metal P 119 P 120
P 62 5 * metal
P 63 5 * metal
W 61 4 64 6
S 1 H metal P 121 P 122
P 76 44 * pdif
P 76 49 * pdif
W 73 41 79 52
S 3 V pdif P 123 P 124
P 14 39 * poly
P 14 39 * poly
W 13 38 15 40
S 1 H poly P 125 P 126
P 14 35 * poly
P 22 35 * poly
W 13 34 23 36
S 1 H poly P 127 P 128
P 11 60 * poly
P 17 60 * poly
W 10 59 18 61
S 1 H poly P 129 P 130
P 73 26 * poly
P 73 42 * poly
W 72 25 74 43
S 1 V poly P 131 P 132
P 17 48 * poly
P 17 60 * poly
W 16 47 18 61
S 1 V poly P 133 P 134
P 17 48 * poly
P 20 48 * poly
W 16 47 21 49
S 1 H poly P 135 P 136
P 8 9 * poly
P 8 12 * poly
W 7 8 9 13
S 1 V poly P 137 P 138
P 14 35 * poly
P 14 39 * poly
W 13 34 15 40
S 1 V poly P 139 P 140
P 14 12 * poly
P 17 12 * poly
W 13 11 18 13
S 1 H poly P 141 P 142
P 17 8 * poly
P 17 12 * poly
W 16 7 18 13
S 1 V poly P 143 P 144
P 9 24 * poly
P 20 24 * poly
W 8 23 21 25
S 1 H poly P 145 P 146
P 20 21 * poly
P 20 24 * poly
W 19 20 21 25
S 1 V poly P 147 P 148
P 4 21 * poly
P 4 29 * poly
W 3 20 5 30
S 1 V poly P 149 P 150
P 4 21 * poly
P 8 21 * poly
W 3 20 9 22
S 1 H poly P 151 P 152
P 4 29 * metal
P 4 38 * metal
W 3 28 5 39
S 1 V metal P 153 P 154
P 26 35 * poly
P 26 39 * poly
W 25 34 27 40
S 1 V poly P 155 P 156
P 2 38 * metal2
P 79 38 * metal2
W 0 36 81 40 ai
S 2 H metal2 P 157 P 158
P 8 32 * poly
P 8 39 * poly
W 7 31 9 40 aim1
S 1 V poly P 159 P 160
P 13 27 * poly
P 24 27 * poly
W 12 26 25 28
S 1 H poly P 161 P 162
P 24 21 * poly
P 24 27 * poly
W 23 20 25 28
S 1 V poly P 163 P 164
P 33 5 * poly
P 33 12 * poly
W 32 4 34 13
S 1 V poly P 165 P 166
P 37 9 * metal
P 37 28 * metal
W 36 8 38 29
S 1 V metal P 167 P 168
P 17 31 * poly
P 30 31 * poly
W 16 30 31 32
S 1 H poly P 169 P 170
P 40 21 * poly
P 40 25 * poly
W 39 20 41 26
S 1 V poly P 171 P 172
P 49 17 * metal
P 49 25 * metal
W 48 16 50 26
S 1 V metal P 173 P 174
P 38 58 * metal
P 48 58 * metal
W 37 57 49 59
S 1 H metal P 175 P 176
P 30 25 * poly
P 49 25 * poly
W 29 24 50 26
S 1 H poly P 177 P 178
P 30 25 * poly
P 30 31 * poly
W 29 24 31 32
S 1 V poly P 179 P 180
P 43 17 * metal
P 43 50 * metal
W 42 16 44 51 t_s
S 1 V metal P 181 P 182
P 48 31 * metal
P 48 38 * metal
W 47 30 49 39
S 1 V metal P 183 P 184
P 39 31 * poly
P 48 31 * poly
W 38 30 49 32
S 1 H poly P 185 P 186
P 39 31 * poly
P 39 35 * poly
W 38 30 40 36
S 1 V poly P 187 P 188
P 26 35 * poly
P 39 35 * poly
W 25 34 40 36
S 1 H poly P 189 P 190
P 43 50 * metal
P 47 50 * metal
W 42 49 48 51
S 1 H metal P 191 P 192
P 38 28 * metal
P 38 58 * metal
W 37 27 39 59
S 1 V metal P 193 P 194
P 37 28 * metal
P 38 28 * metal
W 36 27 39 29
S 1 H metal P 195 P 196
P 31 12 * poly
P 33 12 * poly
W 30 11 34 13
S 1 H poly P 197 P 198
P 8 32 * poly
P 13 32 * poly
W 7 31 14 33
S 1 H poly P 199 P 200
P 13 27 * poly
P 13 32 * poly
W 12 26 14 33
S 1 V poly P 201 P 202
P 9 23 * metal
P 9 34 * metal
W 8 22 10 35
S 1 V metal P 203 P 204
P 9 23 * metal
P 11 23 * metal
W 8 22 12 24
S 1 H metal P 205 P 206
P 9 24 * poly
P 9 28 * poly
W 8 23 10 29
S 1 V poly P 207 P 208
P 4 44 * metal
P 5 44 * metal
W 3 43 6 45
S 1 H metal P 209 P 210
P 4 17 * metal
P 5 17 * metal
W 3 16 6 18
S 1 H metal P 211 P 212
P 28 54 * metal
P 29 54 * metal
W 27 53 30 55
S 1 H metal P 213 P 214
P 29 44 * metal
P 29 54 * metal
W 28 43 30 55
S 1 V metal P 215 P 216
P 23 41 * pdif
P 23 46 * pdif
W 21 39 25 48
S 2 V pdif P 217 P 218
P 8 12 * ntrans
P 8 21 * ntrans
W 7 11 9 22
S 1 V ntrans P 219 P 220
P 14 12 * ntrans
P 14 21 * ntrans
W 13 11 15 22
S 1 V ntrans P 221 P 222
P 20 12 * ntrans
P 20 21 * ntrans
W 19 11 21 22
S 1 V ntrans P 223 P 224
P 24 12 * ntrans
P 24 21 * ntrans
W 23 11 25 22
S 1 V ntrans P 225 P 226
P 31 12 * ntrans
P 31 21 * ntrans
W 30 11 32 22
S 1 V ntrans P 227 P 228
P 2 23 * metal2
P 79 23 * metal2
W 0 21 81 25
S 2 H metal2 P 229 P 230
P 17 8 * poly
P 22 8 * poly
W 16 7 23 9
S 1 H poly P 231 P 232
P 17 8 * poly
P 17 8 * poly
W 16 7 18 9
S 1 H poly P 233 P 234
P 33 51 * poly
P 35 51 * poly
W 32 50 36 52
S 1 H poly P 235 P 236
P 35 48 * poly
P 35 51 * poly
W 34 47 36 52
S 1 V poly P 237 P 238
P 57 22 * metal
P 57 29 * metal
W 56 21 58 30
S 1 V metal P 239 P 240
P 48 54 * poly
P 50 54 * poly
W 47 53 51 55
S 1 H poly P 241 P 242
P 48 54 * poly
P 48 58 * poly
W 47 53 49 59
S 1 V poly P 243 P 244
P 47 62 * poly
P 55 62 * poly
W 46 61 56 63
S 1 H poly P 245 P 246
P 43 42 * poly
P 43 62 * poly
W 42 41 44 63
S 1 V poly P 247 P 248
P 43 62 * poly
P 47 62 * poly
W 42 61 48 63
S 1 H poly P 249 P 250
P 73 51 * poly
P 73 61 * poly
W 72 50 74 62
S 1 V poly P 251 P 252
P 49 25 * poly
P 53 25 * poly
W 48 24 54 26
S 1 H poly P 253 P 254
P 53 25 * poly
P 53 40 * poly
W 52 24 54 41
S 1 V poly P 255 P 256
P 53 40 * poly
P 55 40 * poly
W 52 39 56 41
S 1 H poly P 257 P 258
P 55 40 * poly
P 55 45 * poly
W 54 39 56 46
S 1 V poly P 259 P 260
P 55 61 * poly
P 55 62 * poly
W 54 60 56 63
S 1 V poly P 261 P 262
P 55 61 * poly
P 73 61 * poly
W 54 60 74 62
S 1 H poly P 263 P 264
P 43 14 * ndif
P 43 19 * ndif
W 41 12 45 21
S 2 V ndif P 265 P 266
P 36 14 * ndif
P 36 19 * ndif
W 31 9 41 24
S 5 V ndif P 267 P 268
P 70 36 * poly
P 70 42 * poly
W 69 35 71 43
S 1 V poly P 269 P 270
P 63 36 * poly
P 70 36 * poly
W 62 35 71 37
S 1 H poly P 271 P 272
P 67 24 * ndif
P 67 11 * ndif
W 65 9 69 26
S 2 V ndif P 273 P 274
P 63 24 * ndif
P 63 11 * ndif
W 61 9 65 26
S 2 V ndif P 275 P 276
P 49 19 * ndif
P 49 14 * ndif
W 47 12 51 21
S 2 V ndif P 277 P 278
P 27 19 * ndif
P 27 14 * ndif
W 25 12 29 21
S 2 V ndif P 279 P 280
P 34 19 * ndif
P 34 14 * ndif
W 32 12 36 21
S 2 V ndif P 281 P 282
P 32 46 * pdif
P 32 41 * pdif
W 30 39 34 48
S 2 V pdif P 283 P 284
P 67 49 * pdif
P 67 44 * pdif
W 65 42 69 51
S 2 V pdif P 285 P 286
P 38 46 * pdif
P 38 41 * pdif
W 36 39 40 48
S 2 V pdif P 287 P 288
P 31 46 * pdif
P 31 41 * pdif
W 29 39 33 48
S 2 V pdif P 289 P 290
E
