#cell1 amcsaitop_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 75 65
B 2 5 75 65
F F
C anext 2 23 metal2 2 1 * * W anext
C anext 22 65 metal 1 2 * * N anext
C cin 2 48 metal2 2 3 * * W cin
C cin 26 65 metal 1 4 * * N cin
C b 2 33 metal 1 5 * * W b
C vdd 75 65 metal2 2 6 * * E vdd
C vdd 2 65 metal2 2 7 * * W vdd
C vss 75 5 metal2 2 8 * * E vss
C vss 2 5 metal2 2 9 * * W vss
C sum 75 23 metal2 2 10 * * E sum
C cout 75 48 metal2 2 11 * * E cout
I cbn1 67 65 0 "cbn" ly * *
I cdp1 63 43 0 "cdp" ly * *
I cdp2 63 49 0 "cdp" ly * *
I cdn1 68 17 0 "cdn" ly * *
I cbp1 43 5 0 "cbp" ly * *
I cbp2 31 5 0 "cbp" ly * *
I via1 37 5 0 "cvia" ly * *
I cpf1 59 27 0 "cpf" ly * *
I cdp3 48 56 0 "cdp" ly * *
I via2 35 23 0 "cvia" ly * *
I cpf2 22 11 0 "cpf" ly * *
I via3 27 48 0 "cvia" ly * *
I cdn2 64 24 0 "cdn" ly * *
I via4 69 48 0 "cvia" ly * *
I cdn3 68 30 0 "cdn" ly * *
I cdp4 69 43 0 "cdp" ly * *
I via5 63 65 0 "cvia" ly * *
I cdp5 63 54 0 "cdp" ly * *
I cpf3 60 35 0 "cpf" ly * *
I cdn4 58 16 0 "cdn" ly * *
I cdn5 57 5 0 "cdn" ly * *
I cdn6 67 5 0 "cdn" ly * *
I via6 62 5 0 "cvia" ly * *
I cdn7 64 11 0 "cdn" ly * *
I cdn8 54 22 0 "cdn" ly * *
I cdp6 56 62 0 "cdp" ly * *
I cdp7 56 49 0 "cdp" ly * *
I cdp8 48 42 0 "cdp" ly * *
I cdp9 42 42 0 "cdp" ly * *
I cdp10 36 51 0 "cdp" ly * *
I via7 36 65 0 "cvia" ly * *
I cdp11 30 65 0 "cdp" ly * *
I cpf4 49 37 0 "cpf" ly * *
I cdp12 30 55 0 "cdp" ly * *
I cdn9 35 17 0 "cdn" ly * *
I cpf5 34 37 0 "cpf" ly * *
I cpf6 41 20 0 "cpf" ly * *
I cdn10 49 30 0 "cdn" ly * *
I cdn11 41 29 0 "cdn" ly * *
I cpf7 30 12 0 "cpf" ly * *
I cdn12 35 29 0 "cdn" ly * *
I cpf8 26 37 0 "cpf" ly * *
I cdp13 30 42 0 "cdp" ly * *
I via8 22 23 0 "cvia" ly * *
I via9 18 5 0 "cvia" ly * *
I cdn13 10 17 0 "cdn" ly * *
I cdn14 9 11 0 "cdn" ly * *
I cpf9 4 10 0 "cpf" ly * *
I cpf10 4 15 0 "cpf" ly * *
I cdn15 4 28 0 "cdn" ly * *
I cdn16 10 28 0 "cdn" ly * *
I cpf11 13 35 0 "cpf" ly * *
I cdn17 17 24 0 "cdn" ly * *
I cpf12 18 35 0 "cpf" ly * *
I cdp14 10 42 0 "cdp" ly * *
I cdn18 18 30 0 "cdn" ly * *
I via10 9 65 0 "cvia" ly * *
I cdp15 14 65 0 "cdp" ly * *
I cdp16 18 46 0 "cdp" ly * *
I cdp17 13 55 0 "cdp" ly * *
I cpf13 13 51 0 "cpf" ly * *
I cpf14 4 62 0 "cpf" ly * *
P 63 65 * metal
P 67 65 * metal
W 61 63 69 67
S 2 H metal P 1 P 2
P 63 54 * metal
P 63 65 * metal
W 62 53 64 66
S 1 V metal P 3 P 4
P 63 43 * metal
P 63 54 * metal
W 62 42 64 55
S 1 V metal P 5 P 6
P 22 11 * metal
P 22 23 * metal
W 21 10 23 24 a
S 1 V metal P 7 P 8
P 26 37 * metal
P 26 49 * metal
W 25 36 27 50
S 1 V metal P 9 P 10
P 64 17 * ndif
P 68 17 * ndif
W 62 15 70 19
S 2 H ndif P 11 P 12
P 61 14 * poly
P 62 14 * poly
W 60 13 63 15
S 1 H poly P 13 P 14
P 61 13 * poly
P 61 14 * poly
W 60 12 62 15
S 1 V poly P 15 P 16
P 48 27 * ndif
P 48 31 * ndif
W 46 25 50 33
S 2 V ndif P 17 P 18
P 48 49 * pdif
P 56 49 * pdif
W 45 46 59 52
S 3 H pdif P 19 P 20
P 48 56 * pdif
P 57 56 * pdif
W 45 53 60 59
S 3 H pdif P 21 P 22
P 62 65 * nwell
P 75 65 * nwell
W 58 61 79 69
S 4 H nwell P 23 P 24
P 30 5 * ptie
P 42 5 * ptie
W 27 2 45 8
S 3 H ptie P 25 P 26
P 31 5 * metal
P 43 5 * metal
W 29 3 45 7
S 2 H metal P 27 P 28
P 6 66 * nwell
P 39 66 * nwell
W 2 62 43 70
S 4 H nwell P 29 P 30
P 59 27 * poly
P 60 27 * poly
W 58 26 61 28
S 1 H poly P 31 P 32
P 70 14 * poly
P 73 14 * poly
W 69 13 74 15
S 1 H poly P 33 P 34
P 66 35 * poly
P 66 40 * poly
W 65 34 67 41
S 1 V poly P 35 P 36
P 60 35 * poly
P 66 35 * poly
W 59 34 67 36
S 1 H poly P 37 P 38
P 17 24 * metal
P 18 24 * metal
W 16 23 19 25
S 1 H metal P 39 P 40
P 17 24 * metal
P 18 24 * metal
W 16 23 19 25
S 1 H metal P 41 P 42
P 26 37 * poly
P 26 38 * poly
W 25 36 27 39
S 1 V poly P 43 P 44
P 24 38 * poly
P 26 38 * poly
W 23 37 27 39
S 1 H poly P 45 P 46
P 24 38 * poly
P 24 39 * poly
W 23 37 25 40
S 1 V poly P 47 P 48
P 20 39 * poly
P 21 39 * poly
W 19 38 22 40
S 1 H poly P 49 P 50
P 20 35 * poly
P 20 39 * poly
W 19 34 21 40
S 1 V poly P 51 P 52
P 22 33 * poly
P 22 35 * poly
W 21 32 23 36
S 1 V poly P 53 P 54
P 26 48 * metal
P 27 48 * metal
W 25 47 28 49
S 1 H metal P 55 P 56
P 2 33 * metal
P 4 33 * metal
W 1 32 5 34
S 1 H metal P 57 P 58
P 9 10 * metal
P 9 11 * metal
W 8 9 10 12
S 1 V metal P 59 P 60
P 4 8 * poly
P 4 9 * poly
W 3 7 5 10
S 1 V poly P 61 P 62
P 4 8 * poly
P 14 8 * poly
W 3 7 15 9
S 1 H poly P 63 P 64
P 4 10 * metal
P 9 10 * metal
W 3 9 10 11
S 1 H metal P 65 P 66
P 60 13 * poly
P 61 13 * poly
W 59 12 62 14
S 1 H poly P 67 P 68
P 73 54 * poly
P 73 61 * poly
W 72 53 74 62
S 1 V poly P 69 P 70
P 61 61 * poly
P 73 61 * poly
W 60 60 74 62
S 1 H poly P 71 P 72
P 61 59 * poly
P 61 61 * poly
W 60 58 62 62
S 1 V poly P 73 P 74
P 48 41 * metal
P 48 42 * metal
W 47 40 49 43
S 1 V metal P 75 P 76
P 48 41 * metal
P 53 41 * metal
W 47 40 54 42
S 1 H metal P 77 P 78
P 53 35 * metal
P 53 41 * metal
W 52 34 54 42
S 1 V metal P 79 P 80
P 53 35 * metal
P 60 35 * metal
W 52 34 61 36
S 1 H metal P 81 P 82
P 58 27 * metal
P 59 27 * metal
W 57 26 60 28
S 1 H metal P 83 P 84
P 36 56 * metal
P 48 56 * metal
W 35 55 49 57
S 1 H metal P 85 P 86
P 45 52 * poly
P 46 52 * poly
W 44 51 47 53
S 1 H poly P 87 P 88
P 56 49 * metal
P 56 62 * metal
W 55 48 57 63
S 1 V metal P 89 P 90
P 45 52 * poly
P 45 53 * poly
W 44 51 46 54
S 1 V poly P 91 P 92
P 46 59 * poly
P 46 60 * poly
W 45 58 47 61
S 1 V poly P 93 P 94
P 30 12 * poly
P 30 13 * poly
W 29 11 31 14
S 1 V poly P 95 P 96
P 26 12 * metal
P 30 12 * metal
W 25 11 31 13
S 1 H metal P 97 P 98
P 35 23 * metal2
P 75 23 * metal2
W 33 21 77 25 sum
S 2 H metal2 P 99 P 100
P 22 11 * metal
P 22 11 * metal
W 21 10 23 12
S 1 H metal P 101 P 102
P 26 48 * metal
P 26 48 * metal
W 25 47 27 49
S 1 H metal P 103 P 104
P 31 47 * metal
P 31 55 * metal
W 30 46 32 56
S 1 V metal P 105 P 106
P 26 48 * metal
P 26 65 * metal
W 25 47 27 66
S 1 V metal P 107 P 108
P 26 12 * metal
P 26 37 * metal
W 25 11 27 38
S 1 V metal P 109 P 110
P 22 23 * metal
P 22 65 * metal
W 21 22 23 66
S 1 V metal P 111 P 112
P 60 35 * poly
P 62 35 * poly
W 59 34 63 36
S 1 H poly P 113 P 114
P 68 16 * metal
P 68 17 * metal
W 67 15 69 18
S 1 V metal P 115 P 116
P 64 24 * metal
P 68 24 * metal
W 63 23 69 25
S 1 H metal P 117 P 118
P 13 35 * metal
P 14 35 * metal
W 12 34 15 36
S 1 H metal P 119 P 120
P 10 28 * metal
P 14 28 * metal
W 9 27 15 29
S 1 H metal P 121 P 122
P 14 28 * metal
P 14 35 * metal
W 13 27 15 36
S 1 V metal P 123 P 124
P 13 32 * poly
P 13 39 * poly
W 12 31 14 40
S 1 V poly P 125 P 126
P 38 40 * poly
P 39 40 * poly
W 37 39 40 41
S 1 H poly P 127 P 128
P 38 37 * poly
P 38 40 * poly
W 37 36 39 41
S 1 V poly P 129 P 130
P 34 37 * poly
P 38 37 * poly
W 33 36 39 38
S 1 H poly P 131 P 132
P 13 51 * metal
P 13 55 * metal
W 11 49 15 57
S 2 V metal P 133 P 134
P 14 8 * poly
P 14 11 * poly
W 13 7 15 12
S 1 V poly P 135 P 136
P 48 42 * metal
P 48 47 * metal
W 47 41 49 48
S 1 V metal P 137 P 138
P 42 37 * metal
P 45 37 * metal
W 41 36 46 38
S 1 H metal P 139 P 140
P 42 37 * metal
P 42 42 * metal
W 41 36 43 43
S 1 V metal P 141 P 142
P 68 30 * metal
P 69 30 * metal
W 67 29 70 31
S 1 H metal P 143 P 144
P 68 17 * metal
P 68 24 * metal
W 67 16 69 25
S 1 V metal P 145 P 146
P 62 24 * ndif
P 68 24 * ndif
W 59 21 71 27
S 3 H ndif P 147 P 148
P 45 19 * poly
P 52 19 * poly
W 44 18 53 20
S 1 H poly P 149 P 150
P 45 33 * poly
P 56 33 * poly
W 44 32 57 34
S 1 H poly P 151 P 152
P 60 27 * poly
P 60 35 * poly
W 59 26 61 36
S 1 V poly P 153 P 154
P 62 30 * ndif
P 68 30 * ndif
W 59 27 71 33
S 3 H ndif P 155 P 156
P 26 8 * poly
P 46 8 * poly
W 25 7 47 9
S 1 H poly P 157 P 158
P 26 8 * poly
P 26 11 * poly
W 25 7 27 12
S 1 V poly P 159 P 160
P 14 11 * poly
P 26 11 * poly
W 13 10 27 12
S 1 H poly P 161 P 162
P 2 52 * nwell
P 75 52 * nwell
W -24 26 101 78
S 26 H nwell P 163 P 164
P 69 48 * metal2
P 75 48 * metal2
W 67 46 77 50
S 2 H metal2 P 165 P 166
P 56 33 * poly
P 56 39 * poly
W 55 32 57 40
S 1 V poly P 167 P 168
P 56 39 * poly
P 59 39 * poly
W 55 38 60 40
S 1 H poly P 169 P 170
P 59 39 * poly
P 59 45 * poly
W 58 38 60 46
S 1 V poly P 171 P 172
P 68 5 * metal
P 68 16 * metal
W 67 4 69 17
S 1 V metal P 173 P 174
P 69 30 * metal
P 69 48 * metal
W 68 29 70 49 cout
S 1 V metal P 175 P 176
P 63 42 * pdif
P 63 55 * pdif
W 60 39 66 58
S 3 V pdif P 177 P 178
P 69 42 * pdif
P 69 55 * pdif
W 66 39 72 58
S 3 V pdif P 179 P 180
P 58 17 * metal
P 58 27 * metal
W 57 16 59 28
S 1 V metal P 181 P 182
P 35 13 * metal
P 45 13 * metal
W 34 12 46 14
S 1 H metal P 183 P 184
P 45 13 * metal
P 45 37 * metal
W 44 12 46 38
S 1 V metal P 185 P 186
P 73 14 * poly
P 73 55 * poly
W 72 13 74 56
S 1 V poly P 187 P 188
P 59 59 * poly
P 61 59 * poly
W 58 58 62 60
S 1 H poly P 189 P 190
P 57 5 * metal
P 68 5 * metal
W 56 4 69 6
S 1 H metal P 191 P 192
P 54 11 * metal
P 54 22 * metal
W 53 10 55 23
S 1 V metal P 193 P 194
P 54 11 * metal
P 64 11 * metal
W 53 10 65 12
S 1 H metal P 195 P 196
P 65 11 * ndif
P 68 11 * ndif
W 63 9 70 13
S 2 H ndif P 197 P 198
P 54 22 * ndif
P 58 22 * ndif
W 51 19 61 25
S 3 H ndif P 199 P 200
P 46 8 * poly
P 53 8 * poly
W 45 7 54 9
S 1 H poly P 201 P 202
P 54 16 * ndif
P 58 16 * ndif
W 52 14 60 18
S 2 H ndif P 203 P 204
P 63 5 * ndif
P 67 5 * ndif
W 60 2 70 8
S 3 H ndif P 205 P 206
P 55 5 * ndif
P 59 5 * ndif
W 52 2 62 8
S 3 H ndif P 207 P 208
P 60 27 * ntrans
P 70 27 * ntrans
W 59 26 71 28
S 1 H ntrans P 209 P 210
P 62 14 * ntrans
P 70 14 * ntrans
W 61 13 71 15
S 1 H ntrans P 211 P 212
P 61 8 * ntrans
P 69 8 * ntrans
W 60 7 70 9
S 1 H ntrans P 213 P 214
P 52 19 * ntrans
P 60 19 * ntrans
W 51 18 61 20
S 1 H ntrans P 215 P 216
P 52 13 * ntrans
P 60 13 * ntrans
W 51 12 61 14
S 1 H ntrans P 217 P 218
P 53 8 * ntrans
P 61 8 * ntrans
W 52 7 62 9
S 1 H ntrans P 219 P 220
P 48 62 * pdif
P 57 62 * pdif
W 45 59 60 65
S 3 H pdif P 221 P 222
P 49 30 * metal
P 49 37 * metal
W 48 29 50 38
S 1 V metal P 223 P 224
P 43 53 * poly
P 43 58 * poly
W 42 52 44 59
S 1 V poly P 225 P 226
P 43 53 * poly
P 45 53 * poly
W 42 52 46 54
S 1 H poly P 227 P 228
P 36 51 * metal
P 36 65 * metal
W 35 50 37 66
S 1 V metal P 229 P 230
P 36 42 * pdif
P 36 51 * pdif
W 33 39 39 54
S 3 V pdif P 231 P 232
P 42 42 * pdif
P 42 51 * pdif
W 39 39 45 54
S 3 V pdif P 233 P 234
P 30 65 * metal
P 36 65 * metal
W 29 64 37 66
S 1 H metal P 235 P 236
P 48 42 * pdif
P 57 42 * pdif
W 45 39 60 45
S 3 H pdif P 237 P 238
P 41 37 * poly
P 49 37 * poly
W 40 36 50 38
S 1 H poly P 239 P 240
P 43 16 * poly
P 45 16 * poly
W 42 15 46 17
S 1 H poly P 241 P 242
P 45 16 * poly
P 45 25 * poly
W 44 15 46 26
S 1 V poly P 243 P 244
P 31 47 * metal
P 48 47 * metal
W 30 46 49 48
S 1 H metal P 245 P 246
P 22 55 * pdif
P 31 55 * pdif
W 19 52 34 58
S 3 H pdif P 247 P 248
P 46 60 * poly
P 46 62 * poly
W 45 59 47 63
S 1 V poly P 249 P 250
P 33 58 * poly
P 43 58 * poly
W 32 57 44 59
S 1 H poly P 251 P 252
P 33 62 * poly
P 46 62 * poly
W 32 61 47 63
S 1 H poly P 253 P 254
P 22 65 * pdif
P 31 65 * pdif
W 19 62 34 68
S 3 H pdif P 255 P 256
P 35 13 * metal
P 35 23 * metal
W 34 12 36 24
S 1 V metal P 257 P 258
P 34 33 * metal
P 34 37 * metal
W 33 32 35 38
S 1 V metal P 259 P 260
P 2 5 * metal2
P 75 5 * metal2
W -6 -3 83 13
S 8 H metal2 P 261 P 262
P 39 40 * ptrans
P 39 53 * ptrans
W 38 39 40 54
S 1 V ptrans P 263 P 264
P 30 33 * metal
P 41 33 * metal
W 29 32 42 34
S 1 H metal P 265 P 266
P 30 33 * metal
P 30 42 * metal
W 29 32 31 43
S 1 V metal P 267 P 268
P 40 20 * poly
P 41 20 * poly
W 39 19 42 21
S 1 H poly P 269 P 270
P 43 13 * poly
P 43 16 * poly
W 42 12 44 17
S 1 V poly P 271 P 272
P 30 13 * poly
P 43 13 * poly
W 29 12 44 14
S 1 H poly P 273 P 274
P 41 33 * poly
P 41 37 * poly
W 40 32 42 38
S 1 V poly P 275 P 276
P 14 17 * poly
P 17 17 * poly
W 13 16 18 18
S 1 H poly P 277 P 278
P 14 17 * poly
P 14 20 * poly
W 13 16 15 21
S 1 V poly P 279 P 280
P 41 20 * metal
P 41 33 * metal
W 40 19 42 34 s2_s
S 1 V metal P 281 P 282
P 45 25 * ntrans
P 45 33 * ntrans
W 44 24 46 34
S 1 V ntrans P 283 P 284
P 19 17 * ndif
P 38 17 * ndif
W 16 14 41 20
S 3 H ndif P 285 P 286
P 41 27 * ndif
P 41 31 * ndif
W 38 24 44 34
S 3 V ndif P 287 P 288
P 35 27 * ndif
P 35 31 * ndif
W 32 24 38 34
S 3 V ndif P 289 P 290
P 18 5 * metal
P 18 24 * metal
W 17 4 19 25
S 1 V metal P 291 P 292
P 15 23 * ndif
P 30 23 * ndif
W 12 20 33 26
S 3 H ndif P 293 P 294
P 38 25 * ntrans
P 38 33 * ntrans
W 37 24 39 34
S 1 V ntrans P 295 P 296
P 26 36 * poly
P 26 38 * poly
W 25 35 27 39
S 1 V poly P 297 P 298
P 29 41 * pdif
P 29 50 * pdif
W 24 36 34 55
S 5 V pdif P 299 P 300
P 22 33 * poly
P 41 33 * poly
W 21 32 42 34
S 1 H poly P 301 P 302
P 18 41 * pdif
P 18 48 * pdif
W 16 39 20 50
S 2 V pdif P 303 P 304
P 10 42 * metal
P 18 42 * metal
W 9 41 19 43
S 1 H metal P 305 P 306
P 21 39 * ptrans
P 21 52 * ptrans
W 20 38 22 53
S 1 V ptrans P 307 P 308
P 66 40 * ptrans
P 66 57 * ptrans
W 65 39 67 58
S 1 V ptrans P 309 P 310
P 46 52 * ptrans
P 59 52 * ptrans
W 45 51 60 53
S 1 H ptrans P 311 P 312
P 46 59 * ptrans
P 59 59 * ptrans
W 45 58 60 60
S 1 H ptrans P 313 P 314
P 46 45 * ptrans
P 59 45 * ptrans
W 45 44 60 46
S 1 H ptrans P 315 P 316
P 20 58 * ptrans
P 33 58 * ptrans
W 19 57 34 59
S 1 H ptrans P 317 P 318
P 20 62 * ptrans
P 33 62 * ptrans
W 19 61 34 63
S 1 H ptrans P 319 P 320
P 17 11 * poly
P 17 17 * poly
W 16 10 18 18
S 1 V poly P 321 P 322
P 8 11 * ndif
P 12 11 * ndif
W 5 8 15 14
S 3 H ndif P 323 P 324
P 8 17 * ndif
P 12 17 * ndif
W 5 14 15 20
S 3 H ndif P 325 P 326
P 4 15 * poly
P 6 15 * poly
W 3 14 7 16
S 1 H poly P 327 P 328
P 13 20 * poly
P 13 24 * poly
W 12 19 14 25
S 1 V poly P 329 P 330
P 7 24 * poly
P 13 24 * poly
W 6 23 14 25
S 1 H poly P 331 P 332
P 9 65 * metal
P 18 65 * metal
W 7 63 20 67
S 2 H metal P 333 P 334
P 13 20 * poly
P 14 20 * poly
W 12 19 15 21
S 1 H poly P 335 P 336
P 6 14 * poly
P 6 15 * poly
W 5 13 7 16
S 1 V poly P 337 P 338
P 4 26 * ndif
P 4 30 * ndif
W 1 23 7 33
S 3 V ndif P 339 P 340
P 10 17 * metal
P 10 28 * metal
W 9 16 11 29 s1_s
S 1 V metal P 341 P 342
P 10 26 * ndif
P 10 30 * ndif
W 7 23 13 33
S 3 V ndif P 343 P 344
P 6 14 * ntrans
P 14 14 * ntrans
W 5 13 15 15
S 1 H ntrans P 345 P 346
P 7 24 * ntrans
P 7 32 * ntrans
W 6 23 8 33
S 1 V ntrans P 347 P 348
P 18 46 * metal
P 18 65 * metal
W 17 45 19 66
S 1 V metal P 349 P 350
P 26 29 * metal
P 35 29 * metal
W 25 28 36 30
S 1 H metal P 351 P 352
P 2 23 * metal2
P 22 23 * metal2
W 0 21 24 25
S 2 H metal2 P 353 P 354
P 17 20 * ntrans
P 40 20 * ntrans
W 16 19 41 21
S 1 H ntrans P 355 P 356
P 24 39 * ptrans
P 24 52 * ptrans
W 23 38 25 53
S 1 V ptrans P 357 P 358
P 18 35 * poly
P 22 35 * poly
W 17 34 23 36
S 1 H poly P 359 P 360
P 18 30 * metal
P 18 42 * metal
W 17 29 19 43
S 1 V metal P 361 P 362
P 15 30 * ndif
P 30 30 * ndif
W 12 27 33 33
S 3 H ndif P 363 P 364
P 13 27 * ntrans
P 32 27 * ntrans
W 12 26 33 28
S 1 H ntrans P 365 P 366
P 13 50 * poly
P 13 51 * poly
W 12 49 14 52
S 1 V poly P 367 P 368
P 9 65 * pdif
P 18 65 * pdif
W 6 62 21 68
S 3 H pdif P 369 P 370
P 9 55 * pdif
P 18 55 * pdif
W 6 52 21 58
S 3 H pdif P 371 P 372
P 16 41 * pdif
P 16 48 * pdif
W 14 39 18 50
S 2 V pdif P 373 P 374
P 10 41 * pdif
P 10 48 * pdif
W 7 38 13 51
S 3 V pdif P 375 P 376
P 13 27 * poly
P 13 32 * poly
W 12 26 14 33
S 1 V poly P 377 P 378
P 7 32 * poly
P 7 58 * poly
W 6 31 8 59
S 1 V poly P 379 P 380
P 13 39 * ptrans
P 13 50 * ptrans
W 12 38 14 51
S 1 V ptrans P 381 P 382
P 7 58 * ptrans
P 20 58 * ptrans
W 6 57 21 59
S 1 H ptrans P 383 P 384
P 7 62 * ptrans
P 20 62 * ptrans
W 6 61 21 63
S 1 H ptrans P 385 P 386
P 4 62 * poly
P 7 62 * poly
W 3 61 8 63
S 1 H poly P 387 P 388
P 4 15 * metal
P 4 62 * metal
W 3 14 5 63 b
S 1 V metal P 389 P 390
P 2 48 * metal2
P 26 48 * metal2
W 0 46 28 50
S 2 H metal2 P 391 P 392
P 2 65 * metal2
P 75 65 * metal2
W -4 59 81 71
S 6 H metal2 P 393 P 394
P 48 55 * pdif
P 57 55 * pdif
W 46 53 59 57
S 2 H pdif P 395 P 396
P 48 48 * pdif
P 57 48 * pdif
W 46 46 59 50
S 2 H pdif P 397 P 398
P 17 48 * pdif
P 17 41 * pdif
W 15 39 19 50
S 2 V pdif P 399 P 400
P 27 50 * pdif
P 27 41 * pdif
W 25 39 29 52
S 2 V pdif P 401 P 402
P 42 31 * ndif
P 42 27 * ndif
W 40 25 44 33
S 2 V ndif P 403 P 404
P 15 24 * ndif
P 30 24 * ndif
W 13 22 32 26
S 2 H ndif P 405 P 406
E
