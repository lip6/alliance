#cell1 ammuxpin_c idps_symb compose *
# 21-Feb-95 12:26 21-Feb-95 12:26 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 79 65
B 2 5 79 65
F F
C c2j 33 65 metal 1 1 * * N c2j
C d2j 11 65 metal 1 2 * * N d2j
C vdd 79 65 metal2 2 3 * * E vdd
C vss 79 5 metal2 2 4 * * E vss
C ai 79 38 metal2 2 5 * * E ai
C vdd 2 65 metal2 2 6 * * W vdd
C vss 2 5 metal2 2 7 * * W vss
C n2j 63 65 metal 1 8 * * N n2j
C n2j 63 5 metal 1 9 * * S n2j
C c2j 33 5 metal 1 10 * * S c2j
C d2jbar 22 65 metal 1 11 * * N d2jbar
C d2jbar 22 5 metal 1 12 * * S d2jbar
C aim1 7 5 poly 1 13 * * S aim1
C pi2j 53 5 metal 1 14 * * S pi2j
C pi2jnext 53 65 metal 1 15 * * N pi2jnext
C pi2jnext 79 48 metal2 2 16 * * E pi2jnext
C ai 2 38 metal2 2 17 * * W ai
C ai 7 65 poly 1 18 * * N ai
C d2j 11 5 metal 1 19 * * S d2j
I via1 4 65 0 "cvia" ly * *
I cdp1 5 56 0 "cdp" ly * *
I cpf1 33 65 0 "cpf" ly * *
I cpf2 11 65 0 "cpf" ly * *
I cdp2 46 56 0 "cdp" ly * *
I via2 57 65 0 "cvia" ly * *
I cdp3 75 56 0 "cdp" ly * *
I cpf3 5 46 0 "cpf" ly * *
I via3 5 38 0 "cvia" ly * *
I cdp4 17 56 0 "cdp" ly * *
I cdn1 17 23 0 "cdn" ly * *
I cdn2 5 24 0 "cdn" ly * *
I via4 4 5 0 "cvia" ly * *
I cpf4 22 5 0 "cpf" ly * *
I via5 26 38 0 "cvia" ly * *
I cpf5 26 32 0 "cpf" ly * *
I cdn3 27 24 0 "cdn" ly * *
I cpf6 33 32 0 "cpf" ly * *
I cdn4 37 24 0 "cdn" ly * *
I cdn5 43 24 0 "cdn" ly * *
I cdp5 38 56 0 "cdp" ly * *
I cpf7 46 18 0 "cpf" ly * *
I cdn6 49 24 0 "cdn" ly * *
I cpf8 49 33 0 "cpf" ly * *
I cpf9 17 42 0 "cpf" ly * *
I cpf10 43 37 0 "cpf" ly * *
I cpf11 43 45 0 "cpf" ly * *
I cpf12 63 65 0 "cpf" ly * *
I cdn7 73 22 0 "cdn" ly * *
I cdn8 57 22 0 "cdn" ly * *
I cdn9 57 13 0 "cdn" ly * *
I cdn10 73 13 0 "cdn" ly * *
I cdn11 67 18 0 "cdn" ly * *
I cdp6 57 56 0 "cdp" ly * *
I cdp7 65 56 0 "cdp" ly * *
I cpf13 22 65 0 "cpf" ly * *
I via6 73 5 0 "cvia" ly * *
I via7 28 65 0 "cvia" ly * *
I via8 57 5 0 "cvia" ly * *
I via9 27 5 0 "cvia" ly * *
I cdp8 28 56 0 "cdp" ly * *
I cpf14 67 32 0 "cpf" ly * *
I cpf15 59 32 0 "cpf" ly * *
I via10 58 48 0 "cvia" ly * *
I cbn1 28 47 0 "cbn" ly * *
I cpf16 27 13 0 "cpf" ly * *
I cpf17 37 13 0 "cpf" ly * *
I cbp1 45 13 0 "cbp" ly * *
I cbp2 41 13 0 "cbp" ly * *
I cbp3 49 13 0 "cbp" ly * *
I cpf18 11 5 0 "cpf" ly * *
I via11 76 65 0 "cvia" ly * *
I d2j_in 11 18 0 "ref_con" ly * *
I d2jbar_in 22 13 0 "ref_con" ly * *
P 33 26 * ndif
P 33 21 * ndif
W 31 19 35 28
S 2 V ndif P 1 P 2
P 37 26 * ndif
P 37 21 * ndif
W 35 19 39 28
S 2 V ndif P 3 P 4
P 49 26 * ndif
P 49 21 * ndif
W 47 19 51 28
S 2 V ndif P 5 P 6
P 63 24 * ndif
P 63 11 * ndif
W 61 9 65 26
S 2 V ndif P 7 P 8
P 67 24 * ndif
P 67 11 * ndif
W 65 9 69 26
S 2 V ndif P 9 P 10
P 75 58 * pdif
P 75 53 * pdif
W 73 51 77 60
S 2 V pdif P 11 P 12
P 56 58 * pdif
P 56 53 * pdif
W 54 51 58 60
S 2 V pdif P 13 P 14
P 38 58 * pdif
P 38 53 * pdif
W 36 51 40 60
S 2 V pdif P 15 P 16
P 32 58 * pdif
P 32 53 * pdif
W 30 51 34 60
S 2 V pdif P 17 P 18
P 28 58 * pdif
P 28 53 * pdif
W 26 51 30 60
S 2 V pdif P 19 P 20
P 2 5 * metal2
P 79 5 * metal2
W -6 -3 87 13
S 8 H metal2 P 21 P 22
P 2 65 * metal2
P 79 65 * metal2
W -4 59 85 71
S 6 H metal2 P 23 P 24
P 20 19 * ntrans
P 20 28 * ntrans
W 19 18 21 29
S 1 V ntrans P 25 P 26
P 30 19 * ntrans
P 30 28 * ntrans
W 29 18 31 29
S 1 V ntrans P 27 P 28
P 4 56 * metal
P 4 65 * metal
W 3 55 5 66
S 1 V metal P 29 P 30
P 17 21 * ndif
P 17 26 * ndif
W 14 18 20 29
S 3 V ndif P 31 P 32
P 27 21 * ndif
P 27 26 * ndif
W 24 18 30 29
S 3 V ndif P 33 P 34
P 17 23 * metal
P 17 56 * metal
W 16 22 18 57
S 1 V metal P 35 P 36
P 5 21 * ndif
P 5 26 * ndif
W 2 18 8 29
S 3 V ndif P 37 P 38
P 4 5 * metal
P 4 24 * metal
W 3 4 5 25
S 1 V metal P 39 P 40
P 26 32 * metal
P 26 38 * metal
W 25 31 27 39
S 1 V metal P 41 P 42
P 27 5 * metal
P 27 24 * metal
W 26 4 28 25
S 1 V metal P 43 P 44
P 46 19 * ntrans
P 46 28 * ntrans
W 45 18 47 29
S 1 V ntrans P 45 P 46
P 35 21 * ndif
P 35 26 * ndif
W 29 15 41 32
S 6 V ndif P 47 P 48
P 43 21 * ndif
P 43 26 * ndif
W 40 18 46 29
S 3 V ndif P 49 P 50
P 37 18 * metal
P 37 49 * metal
W 36 17 38 50
S 1 V metal P 51 P 52
P 37 18 * metal
P 46 18 * metal
W 36 17 47 19
S 1 H metal P 53 P 54
P 46 18 * poly
P 46 19 * poly
W 45 17 47 20
S 1 V poly P 55 P 56
P 50 21 * ndif
P 50 26 * ndif
W 47 18 53 29
S 3 V ndif P 57 P 58
P 17 42 * poly
P 39 42 * poly
W 16 41 40 43
S 1 H poly P 59 P 60
P 49 24 * metal
P 49 33 * metal
W 48 23 50 34
S 1 V metal P 61 P 62
P 43 41 * metal
P 47 41 * metal
W 42 40 48 42
S 1 H metal P 63 P 64
P 43 24 * metal
P 43 41 * metal
W 42 23 44 42 t_s
S 1 V metal P 65 P 66
P 39 33 * poly
P 39 49 * poly
W 38 32 40 50
S 1 V poly P 67 P 68
P 39 49 * poly
P 49 49 * poly
W 38 48 50 50
S 1 H poly P 69 P 70
P 49 49 * poly
P 49 51 * poly
W 48 48 50 52
S 1 V poly P 71 P 72
P 57 56 * metal
P 57 65 * metal
W 56 55 58 66
S 1 V metal P 73 P 74
P 53 45 * poly
P 53 51 * poly
W 52 44 54 52
S 1 V poly P 75 P 76
P 37 45 * metal
P 43 45 * metal
W 36 44 44 46
S 1 H metal P 77 P 78
P 73 11 * ndif
P 73 24 * ndif
W 70 8 76 27
S 3 V ndif P 79 P 80
P 57 11 * ndif
P 57 24 * ndif
W 54 8 60 27
S 3 V ndif P 81 P 82
P 57 5 * metal
P 57 22 * metal
W 56 4 58 23
S 1 V metal P 83 P 84
P 47 41 * metal
P 47 49 * metal
W 46 40 48 50
S 1 V metal P 85 P 86
P 43 37 * poly
P 68 37 * poly
W 42 36 69 38
S 1 H poly P 87 P 88
P 25 46 * poly
P 25 51 * poly
W 24 45 26 52
S 1 V poly P 89 P 90
P 72 26 * poly
P 72 51 * poly
W 71 25 73 52
S 1 V poly P 91 P 92
P 49 51 * ptrans
P 49 60 * ptrans
W 48 50 50 61
S 1 V ptrans P 93 P 94
P 68 51 * ptrans
P 68 60 * ptrans
W 67 50 69 61
S 1 V ptrans P 95 P 96
P 60 9 * ntrans
P 60 26 * ntrans
W 59 8 61 27
S 1 V ntrans P 97 P 98
P 62 65 * poly
P 72 65 * poly
W 61 64 73 66
S 1 H poly P 99 P 100
P 72 60 * poly
P 72 65 * poly
W 71 59 73 66
S 1 V poly P 101 P 102
P 13 46 * poly
P 25 46 * poly
W 12 45 26 47
S 1 H poly P 103 P 104
P 11 5 * poly
P 11 12 * poly
W 10 4 12 13
S 1 V poly P 105 P 106
P 18 5 * poly
P 22 5 * poly
W 17 4 23 6
S 1 H poly P 107 P 108
P 73 5 * metal
P 73 22 * metal
W 72 4 74 23
S 1 V metal P 109 P 110
P 43 45 * poly
P 53 45 * poly
W 42 44 54 46
S 1 H poly P 111 P 112
P 28 47 * metal
P 28 65 * metal
W 27 46 29 66
S 1 V metal P 113 P 114
P 65 11 * ndif
P 65 24 * ndif
W 60 6 70 29
S 5 V ndif P 115 P 116
P 33 5 * metal
P 33 65 * metal
W 32 4 34 66 c2j
S 1 V metal P 117 P 118
P 22 5 * metal
P 22 65 * metal
W 21 4 23 66 d2jbar
S 1 V metal P 119 P 120
P 7 60 * poly
P 7 65 * poly
W 6 59 8 66
S 1 V poly P 121 P 122
P 70 26 * poly
P 72 26 * poly
W 69 25 73 27
S 1 H poly P 123 P 124
P 20 51 * ptrans
P 20 60 * ptrans
W 19 50 21 61
S 1 V ptrans P 125 P 126
P 65 53 * pdif
P 65 58 * pdif
W 62 50 68 61
S 3 V pdif P 127 P 128
P 57 53 * pdif
P 57 58 * pdif
W 54 50 60 61
S 3 V pdif P 129 P 130
P 17 53 * pdif
P 17 58 * pdif
W 15 51 19 60
S 2 V pdif P 131 P 132
P 11 53 * pdif
P 11 58 * pdif
W 9 51 13 60
S 2 V pdif P 133 P 134
P 5 53 * pdif
P 5 58 * pdif
W 2 50 8 61
S 3 V pdif P 135 P 136
P 30 53 * pdif
P 30 58 * pdif
W 25 48 35 63
S 5 V pdif P 137 P 138
P 46 53 * pdif
P 46 58 * pdif
W 43 50 49 61
S 3 V pdif P 139 P 140
P 39 53 * pdif
P 39 58 * pdif
W 36 50 42 61
S 3 V pdif P 141 P 142
P 37 49 * metal
P 38 49 * metal
W 36 48 39 50
S 1 H metal P 143 P 144
P 38 49 * metal
P 38 56 * metal
W 37 48 39 57
S 1 V metal P 145 P 146
P 53 5 * metal
P 53 32 * metal
W 52 4 54 33 pi2j
S 1 V metal P 147 P 148
P 8 28 * poly
P 8 40 * poly
W 7 27 9 41
S 1 V poly P 149 P 150
P 11 21 * ndif
P 11 26 * ndif
W 9 19 13 28
S 2 V ndif P 151 P 152
P 7 60 * poly
P 8 60 * poly
W 6 59 9 61
S 1 H poly P 153 P 154
P 7 5 * poly
P 7 9 * poly
W 6 4 8 10
S 1 V poly P 155 P 156
P 7 9 * poly
P 8 9 * poly
W 6 8 9 10
S 1 H poly P 157 P 158
P 2 52 * nwell
P 79 52 * nwell
W -24 26 105 78
S 26 H nwell P 159 P 160
P 39 33 * poly
P 49 33 * poly
W 38 32 50 34
S 1 H poly P 161 P 162
P 11 5 * metal
P 11 30 * metal
W 10 4 12 31
S 1 V metal P 163 P 164
P 8 9 * poly
P 8 19 * poly
W 7 8 9 20 aim1
S 1 V poly P 165 P 166
P 18 5 * poly
P 18 19 * poly
W 17 4 19 20
S 1 V poly P 167 P 168
P 30 28 * poly
P 33 28 * poly
W 29 27 34 29
S 1 H poly P 169 P 170
P 8 40 * poly
P 13 40 * poly
W 7 39 14 41
S 1 H poly P 171 P 172
P 13 40 * poly
P 13 46 * poly
W 12 39 14 47
S 1 V poly P 173 P 174
P 26 28 * poly
P 26 32 * poly
W 25 27 27 33
S 1 V poly P 175 P 176
P 24 28 * poly
P 26 28 * poly
W 23 27 27 29
S 1 H poly P 177 P 178
P 11 12 * poly
P 14 12 * poly
W 10 11 15 13
S 1 H poly P 179 P 180
P 14 12 * poly
P 14 19 * poly
W 13 11 15 20
S 1 V poly P 181 P 182
P 11 60 * poly
P 11 65 * poly
W 10 59 12 66
S 1 V poly P 183 P 184
P 11 60 * poly
P 14 60 * poly
W 10 59 15 61
S 1 H poly P 185 P 186
P 22 60 * poly
P 22 65 * poly
W 21 59 23 66
S 1 V poly P 187 P 188
P 20 60 * poly
P 22 60 * poly
W 19 59 23 61
S 1 H poly P 189 P 190
P 33 60 * poly
P 33 65 * poly
W 32 59 34 66
S 1 V poly P 191 P 192
P 33 60 * poly
P 35 60 * poly
W 32 59 36 61
S 1 H poly P 193 P 194
P 11 50 * metal
P 12 50 * metal
W 10 49 13 51
S 1 H metal P 195 P 196
P 12 30 * metal
P 12 50 * metal
W 11 29 13 51 d2j
S 1 V metal P 197 P 198
P 11 30 * metal
P 12 30 * metal
W 10 29 13 31
S 1 H metal P 199 P 200
P 11 50 * metal
P 11 65 * metal
W 10 49 12 66
S 1 V metal P 201 P 202
P 67 18 * metal
P 67 43 * metal
W 66 17 68 44
S 1 V metal P 203 P 204
P 63 5 * metal
P 63 52 * metal
W 62 4 64 53 n2j
S 1 V metal P 205 P 206
P 40 28 * poly
P 40 33 * poly
W 39 27 41 34
S 1 V poly P 207 P 208
P 46 49 * metal
P 46 56 * metal
W 45 48 47 57
S 1 V metal P 209 P 210
P 46 49 * metal
P 47 49 * metal
W 45 48 48 50
S 1 H metal P 211 P 212
P 75 53 * pdif
P 75 58 * pdif
W 72 50 78 61
S 3 V pdif P 213 P 214
P 4 56 * metal
P 5 56 * metal
W 3 55 6 57
S 1 H metal P 215 P 216
P 4 24 * metal
P 5 24 * metal
W 3 23 6 25
S 1 H metal P 217 P 218
P 53 32 * metal
P 59 32 * metal
W 52 31 60 33
S 1 H metal P 219 P 220
P 59 32 * poly
P 67 32 * poly
W 58 31 68 33
S 1 H poly P 221 P 222
P 53 48 * metal
P 53 65 * metal
W 52 47 54 66
S 1 V metal P 223 P 224
P 53 48 * metal
P 58 48 * metal
W 52 47 59 49
S 1 H metal P 225 P 226
P 58 48 * metal2
P 79 48 * metal2
W 56 46 81 50
S 2 H metal2 P 227 P 228
P 5 38 * metal
P 5 46 * metal
W 4 37 6 47 ai
S 1 V metal P 229 P 230
P 5 46 * poly
P 5 51 * poly
W 4 45 6 52
S 1 V poly P 231 P 232
P 5 51 * poly
P 8 51 * poly
W 4 50 9 52
S 1 H poly P 233 P 234
P 2 38 * metal2
P 79 38 * metal2
W 0 36 81 40
S 2 H metal2 P 235 P 236
P 18 19 * poly
P 20 19 * poly
W 17 18 21 20
S 1 H poly P 237 P 238
P 8 51 * ptrans
P 8 60 * ptrans
W 7 50 9 61
S 1 V ptrans P 239 P 240
P 14 51 * ptrans
P 14 60 * ptrans
W 13 50 15 61
S 1 V ptrans P 241 P 242
P 25 51 * ptrans
P 25 60 * ptrans
W 24 50 26 61
S 1 V ptrans P 243 P 244
P 35 51 * ptrans
P 35 60 * ptrans
W 34 50 36 61
S 1 V ptrans P 245 P 246
P 53 51 * ptrans
P 53 60 * ptrans
W 52 50 54 61
S 1 V ptrans P 247 P 248
P 72 51 * ptrans
P 72 60 * ptrans
W 71 50 73 61
S 1 V ptrans P 249 P 250
P 8 19 * ntrans
P 8 28 * ntrans
W 7 18 9 29
S 1 V ntrans P 251 P 252
P 14 19 * ntrans
P 14 28 * ntrans
W 13 18 15 29
S 1 V ntrans P 253 P 254
P 24 19 * ntrans
P 24 28 * ntrans
W 23 18 25 29
S 1 V ntrans P 255 P 256
P 40 19 * ntrans
P 40 28 * ntrans
W 39 18 41 29
S 1 V ntrans P 257 P 258
P 70 9 * ntrans
P 70 26 * ntrans
W 69 8 71 27
S 1 V ntrans P 259 P 260
P 61 60 * metal
P 63 60 * metal
W 60 59 64 61
S 1 H metal P 261 P 262
P 61 52 * metal
P 61 60 * metal
W 60 51 62 61
S 1 V metal P 263 P 264
P 61 52 * metal
P 63 52 * metal
W 60 51 64 53
S 1 H metal P 265 P 266
P 63 60 * metal
P 63 65 * metal
W 62 59 64 66
S 1 V metal P 267 P 268
P 65 56 * metal
P 70 56 * metal
W 64 55 71 57
S 1 H metal P 269 P 270
P 70 43 * metal
P 70 56 * metal
W 69 42 71 57
S 1 V metal P 271 P 272
P 67 43 * metal
P 70 43 * metal
W 66 42 71 44
S 1 H metal P 273 P 274
P 27 13 * poly
P 37 13 * poly
W 26 12 38 14
S 1 H poly P 275 P 276
P 37 13 * metal
P 49 13 * metal
W 35 11 51 15
S 2 H metal P 277 P 278
P 41 13 * ptie
P 49 13 * ptie
W 38 10 52 16
S 3 H ptie P 279 P 280
P 68 37 * poly
P 68 51 * poly
W 67 36 69 52
S 1 V poly P 281 P 282
P 55 26 * poly
P 55 37 * poly
W 54 25 56 38
S 1 V poly P 283 P 284
P 55 26 * poly
P 60 26 * poly
W 54 25 61 27
S 1 H poly P 285 P 286
P 76 56 * metal
P 76 65 * metal
W 75 55 77 66
S 1 V metal P 287 P 288
P 75 56 * metal
P 76 56 * metal
W 73 54 78 58
S 2 H metal P 289 P 290
P 33 28 * poly
P 33 32 * poly
W 32 27 34 33
S 1 V poly P 291 P 292
E
