#cell1 ammuxibot_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 79 65
B 2 5 79 65
F F
C pi2j 79 33 metal 1 1 * * E pi2j
C n2j 63 65 metal 1 2 * * N n2j
C c2j 33 65 metal 1 3 * * N c2j
C vdd 79 65 metal2 2 4 * * E vdd
C vss 2 5 metal2 2 5 * * W vss
C vss 79 5 metal2 2 6 * * E vss
C mi 53 65 metal 1 7 * * N mi
C ai 7 5 poly 1 8 * * S ai
C c2j 33 5 metal 1 9 * * S c2j
C mi 53 5 metal 1 10 * * S mi
C n2j 63 5 metal 1 11 * * S n2j
C vdd 2 65 metal2 2 12 * * W vdd
C sum 2 23 metal2 2 13 * * W sum
C sum 79 23 metal2 2 14 * * E sum
C cout 2 48 metal2 2 15 * * W cout
C cout 79 48 metal2 2 16 * * E cout
C d2jbar 22 5 metal 1 17 * * S d2jbar
C ai 2 38 metal2 2 18 * * W ai
C ai 79 38 metal2 2 19 * * E ai
I cpf1 33 51 0 "cpf" ly * *
I cdp1 47 50 0 "cdp" ly * *
I via1 58 65 0 "cvia" ly * *
I cdp2 76 47 0 "cdp" ly * *
I via2 76 65 0 "cvia" ly * *
I cdp3 23 44 0 "cdp" ly * *
I cdn1 17 16 0 "cdn" ly * *
I cdn2 5 17 0 "cdn" ly * *
I via3 4 5 0 "cvia" ly * *
I cpf2 22 8 0 "cpf" ly * *
I cdn3 27 17 0 "cdn" ly * *
I cdn4 37 17 0 "cdn" ly * *
I cdn5 43 17 0 "cdn" ly * *
I cdp4 38 44 0 "cdp" ly * *
I cpf3 46 9 0 "cpf" ly * *
I cdn6 49 17 0 "cdn" ly * *
I cpf4 63 40 0 "cpf" ly * *
I cdn7 73 22 0 "cdn" ly * *
I cdn8 57 22 0 "cdn" ly * *
I cdn9 57 13 0 "cdn" ly * *
I cdn10 73 13 0 "cdn" ly * *
I cpf5 63 5 0 "cpf" ly * *
I cdn11 67 22 0 "cdn" ly * *
I cdn12 67 17 0 "cdn" ly * *
I cdp5 58 50 0 "cdp" ly * *
I cdp6 67 47 0 "cdp" ly * *
I via4 73 5 0 "cvia" ly * *
I via5 28 65 0 "cvia" ly * *
I via6 57 5 0 "cvia" ly * *
I via7 27 5 0 "cvia" ly * *
I cdp7 29 44 0 "cdp" ly * *
I via8 4 38 0 "cvia" ly * *
I cpf6 4 29 0 "cpf" ly * *
I via9 48 38 0 "cvia" ly * *
I cpf7 33 5 0 "cpf" ly * *
I cpf8 17 31 0 "cpf" ly * *
I cpf9 49 25 0 "cpf" ly * *
I cpf10 48 58 0 "cpf" ly * *
I cpf11 48 31 0 "cpf" ly * *
I cpf12 43 42 0 "cpf" ly * *
I cbp1 57 29 0 "cbp" ly * *
I cbn1 28 59 0 "cbn" ly * *
I cbn2 76 58 0 "cbn" ly * *
I cdp8 17 44 0 "cdp" ly * *
I cbn3 24 59 0 "cbn" ly * *
I cbn4 20 59 0 "cbn" ly * *
I cbn5 16 59 0 "cbn" ly * *
I cbn6 12 59 0 "cbn" ly * *
I cbn7 8 59 0 "cbn" ly * *
I cbn8 8 55 0 "cbn" ly * *
I cbn9 8 51 0 "cbn" ly * *
I cbn10 8 47 0 "cbn" ly * *
I cbn11 8 43 0 "cbn" ly * *
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
P 17 14 * ndif
P 17 19 * ndif
W 14 11 20 22
S 3 V ndif P 5 P 6
P 27 14 * ndif
P 27 19 * ndif
W 24 11 30 22
S 3 V ndif P 7 P 8
P 17 16 * metal
P 17 35 * metal
W 16 15 18 36
S 1 V metal P 9 P 10
P 5 14 * ndif
P 5 19 * ndif
W 2 11 8 22
S 3 V ndif P 11 P 12
P 4 5 * metal
P 4 17 * metal
W 3 4 5 18
S 1 V metal P 13 P 14
P 27 5 * metal
P 27 17 * metal
W 26 4 28 18
S 1 V metal P 15 P 16
P 40 12 * ntrans
P 40 21 * ntrans
W 39 11 41 22
S 1 V ntrans P 17 P 18
P 46 12 * ntrans
P 46 21 * ntrans
W 45 11 47 22
S 1 V ntrans P 19 P 20
P 37 9 * metal
P 46 9 * metal
W 36 8 47 10
S 1 H metal P 21 P 22
P 46 9 * poly
P 46 12 * poly
W 45 8 47 13
S 1 V poly P 23 P 24
P 50 14 * ndif
P 50 19 * ndif
W 47 11 53 22
S 3 V ndif P 25 P 26
P 58 50 * metal
P 58 65 * metal
W 57 49 59 66
S 1 V metal P 27 P 28
P 73 11 * ndif
P 73 24 * ndif
W 70 8 76 27
S 3 V ndif P 29 P 30
P 57 11 * ndif
P 57 24 * ndif
W 54 8 60 27
S 3 V ndif P 31 P 32
P 57 5 * metal
P 57 22 * metal
W 56 4 58 23
S 1 V metal P 33 P 34
P 76 47 * metal
P 76 65 * metal
W 75 46 77 66
S 1 V metal P 35 P 36
P 35 39 * ptrans
P 35 48 * ptrans
W 34 38 36 49
S 1 V ptrans P 37 P 38
P 50 45 * ptrans
P 50 54 * ptrans
W 49 44 51 55
S 1 V ptrans P 39 P 40
P 55 45 * ptrans
P 55 54 * ptrans
W 54 44 56 55
S 1 V ptrans P 41 P 42
P 70 42 * ptrans
P 70 51 * ptrans
W 69 41 71 52
S 1 V ptrans P 43 P 44
P 73 42 * ptrans
P 73 51 * ptrans
W 72 41 74 52
S 1 V ptrans P 45 P 46
P 26 39 * ptrans
P 26 48 * ptrans
W 25 38 27 49
S 1 V ptrans P 47 P 48
P 60 9 * ntrans
P 60 26 * ntrans
W 59 8 61 27
S 1 V ntrans P 49 P 50
P 70 9 * ntrans
P 70 26 * ntrans
W 69 8 71 27
S 1 V ntrans P 51 P 52
P 63 40 * poly
P 70 40 * poly
W 62 39 71 41
S 1 H poly P 53 P 54
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
P 70 26 * poly
P 73 26 * poly
W 69 25 74 27
S 1 H poly P 69 P 70
P 20 39 * ptrans
P 20 48 * ptrans
W 19 38 21 49
S 1 V ptrans P 71 P 72
P 66 44 * pdif
P 66 49 * pdif
W 63 41 69 52
S 3 V pdif P 73 P 74
P 58 47 * pdif
P 58 52 * pdif
W 55 44 61 55
S 3 V pdif P 75 P 76
P 17 41 * pdif
P 17 46 * pdif
W 14 38 20 49
S 3 V pdif P 77 P 78
P 30 41 * pdif
P 30 46 * pdif
W 25 36 35 51
S 5 V pdif P 79 P 80
P 47 47 * pdif
P 47 52 * pdif
W 44 44 50 55
S 3 V pdif P 81 P 82
P 39 41 * pdif
P 39 46 * pdif
W 36 38 42 49
S 3 V pdif P 83 P 84
P 53 5 * metal
P 53 65 * metal
W 52 4 54 66
S 1 V metal P 85 P 86
P 11 14 * ndif
P 11 19 * ndif
W 9 12 13 21
S 2 V ndif P 87 P 88
P 7 5 * poly
P 7 9 * poly
W 6 4 8 10
S 1 V poly P 89 P 90
P 7 9 * poly
P 8 9 * poly
W 6 8 9 10
S 1 H poly P 91 P 92
P 2 52 * nwell
P 79 52 * nwell
W -24 26 105 78
S 26 H nwell P 93 P 94
P 67 17 * metal
P 67 47 * metal
W 66 16 68 48
S 1 V metal P 95 P 96
P 63 5 * metal
P 63 65 * metal
W 62 4 64 66 n2j
S 1 V metal P 97 P 98
P 62 5 * metal
P 63 5 * metal
W 61 4 64 6
S 1 H metal P 99 P 100
P 76 44 * pdif
P 76 49 * pdif
W 73 41 79 52
S 3 V pdif P 101 P 102
P 73 26 * poly
P 73 42 * poly
W 72 25 74 43
S 1 V poly P 103 P 104
P 8 9 * poly
P 8 12 * poly
W 7 8 9 13
S 1 V poly P 105 P 106
P 14 12 * poly
P 17 12 * poly
W 13 11 18 13
S 1 H poly P 107 P 108
P 17 8 * poly
P 17 12 * poly
W 16 7 18 13
S 1 V poly P 109 P 110
P 4 21 * poly
P 4 29 * poly
W 3 20 5 30 ai
S 1 V poly P 111 P 112
P 4 21 * poly
P 8 21 * poly
W 3 20 9 22
S 1 H poly P 113 P 114
P 4 29 * metal
P 4 38 * metal
W 3 28 5 39
S 1 V metal P 115 P 116
P 26 35 * poly
P 26 39 * poly
W 25 34 27 40
S 1 V poly P 117 P 118
P 2 38 * metal2
P 79 38 * metal2
W 0 36 81 40
S 2 H metal2 P 119 P 120
P 33 5 * poly
P 33 12 * poly
W 32 4 34 13
S 1 V poly P 121 P 122
P 37 9 * metal
P 37 28 * metal
W 36 8 38 29
S 1 V metal P 123 P 124
P 17 31 * poly
P 30 31 * poly
W 16 30 31 32
S 1 H poly P 125 P 126
P 40 21 * poly
P 40 25 * poly
W 39 20 41 26
S 1 V poly P 127 P 128
P 49 17 * metal
P 49 25 * metal
W 48 16 50 26
S 1 V metal P 129 P 130
P 38 58 * metal
P 48 58 * metal
W 37 57 49 59
S 1 H metal P 131 P 132
P 30 25 * poly
P 49 25 * poly
W 29 24 50 26
S 1 H poly P 133 P 134
P 30 25 * poly
P 30 31 * poly
W 29 24 31 32
S 1 V poly P 135 P 136
P 43 17 * metal
P 43 50 * metal
W 42 16 44 51 t_s
S 1 V metal P 137 P 138
P 48 31 * metal
P 48 38 * metal
W 47 30 49 39
S 1 V metal P 139 P 140
P 39 31 * poly
P 48 31 * poly
W 38 30 49 32
S 1 H poly P 141 P 142
P 39 31 * poly
P 39 35 * poly
W 38 30 40 36
S 1 V poly P 143 P 144
P 26 35 * poly
P 39 35 * poly
W 25 34 40 36
S 1 H poly P 145 P 146
P 43 50 * metal
P 47 50 * metal
W 42 49 48 51
S 1 H metal P 147 P 148
P 38 28 * metal
P 38 58 * metal
W 37 27 39 59
S 1 V metal P 149 P 150
P 37 28 * metal
P 38 28 * metal
W 36 27 39 29
S 1 H metal P 151 P 152
P 31 12 * poly
P 33 12 * poly
W 30 11 34 13
S 1 H poly P 153 P 154
P 4 17 * metal
P 5 17 * metal
W 3 16 6 18
S 1 H metal P 155 P 156
P 28 54 * metal
P 29 54 * metal
W 27 53 30 55
S 1 H metal P 157 P 158
P 29 44 * metal
P 29 54 * metal
W 28 43 30 55
S 1 V metal P 159 P 160
P 23 41 * pdif
P 23 46 * pdif
W 21 39 25 48
S 2 V pdif P 161 P 162
P 8 12 * ntrans
P 8 21 * ntrans
W 7 11 9 22
S 1 V ntrans P 163 P 164
P 14 12 * ntrans
P 14 21 * ntrans
W 13 11 15 22
S 1 V ntrans P 165 P 166
P 31 12 * ntrans
P 31 21 * ntrans
W 30 11 32 22
S 1 V ntrans P 167 P 168
P 2 23 * metal2
P 79 23 * metal2
W 0 21 81 25
S 2 H metal2 P 169 P 170
P 17 8 * poly
P 22 8 * poly
W 16 7 23 9
S 1 H poly P 171 P 172
P 17 8 * poly
P 17 8 * poly
W 16 7 18 9
S 1 H poly P 173 P 174
P 33 51 * poly
P 35 51 * poly
W 32 50 36 52
S 1 H poly P 175 P 176
P 35 48 * poly
P 35 51 * poly
W 34 47 36 52
S 1 V poly P 177 P 178
P 57 22 * metal
P 57 29 * metal
W 56 21 58 30
S 1 V metal P 179 P 180
P 48 54 * poly
P 50 54 * poly
W 47 53 51 55
S 1 H poly P 181 P 182
P 48 54 * poly
P 48 58 * poly
W 47 53 49 59
S 1 V poly P 183 P 184
P 47 62 * poly
P 55 62 * poly
W 46 61 56 63
S 1 H poly P 185 P 186
P 43 42 * poly
P 43 62 * poly
W 42 41 44 63
S 1 V poly P 187 P 188
P 43 62 * poly
P 47 62 * poly
W 42 61 48 63
S 1 H poly P 189 P 190
P 73 51 * poly
P 73 61 * poly
W 72 50 74 62
S 1 V poly P 191 P 192
P 49 25 * poly
P 53 25 * poly
W 48 24 54 26
S 1 H poly P 193 P 194
P 53 25 * poly
P 53 40 * poly
W 52 24 54 41
S 1 V poly P 195 P 196
P 53 40 * poly
P 55 40 * poly
W 52 39 56 41
S 1 H poly P 197 P 198
P 55 40 * poly
P 55 45 * poly
W 54 39 56 46
S 1 V poly P 199 P 200
P 55 61 * poly
P 55 62 * poly
W 54 60 56 63
S 1 V poly P 201 P 202
P 55 61 * poly
P 73 61 * poly
W 54 60 74 62
S 1 H poly P 203 P 204
P 43 14 * ndif
P 43 19 * ndif
W 41 12 45 21
S 2 V ndif P 205 P 206
P 36 14 * ndif
P 36 19 * ndif
W 31 9 41 24
S 5 V ndif P 207 P 208
P 2 48 * metal2
P 79 48 * metal2
W 0 46 81 50
S 2 H metal2 P 209 P 210
P 22 5 * metal
P 22 8 * metal
W 21 4 23 9
S 1 V metal P 211 P 212
P 23 35 * metal
P 23 44 * metal
W 22 34 24 45
S 1 V metal P 213 P 214
P 17 35 * metal
P 23 35 * metal
W 16 34 24 36
S 1 H metal P 215 P 216
P 12 39 * poly
P 20 39 * poly
W 11 38 21 40
S 1 H poly P 217 P 218
P 12 26 * poly
P 12 39 * poly
W 11 25 13 40
S 1 V poly P 219 P 220
P 12 26 * poly
P 14 26 * poly
W 11 25 15 27
S 1 H poly P 221 P 222
P 14 21 * poly
P 14 26 * poly
W 13 20 15 27
S 1 V poly P 223 P 224
P 17 44 * metal
P 17 54 * metal
W 16 43 18 55
S 1 V metal P 225 P 226
P 17 54 * metal
P 28 54 * metal
W 16 53 29 55
S 1 H metal P 227 P 228
P 70 40 * poly
P 70 42 * poly
W 69 39 71 43
S 1 V poly P 229 P 230
P 8 59 * metal
P 28 59 * metal
W 6 57 30 61
S 2 H metal P 231 P 232
P 8 43 * metal
P 8 59 * metal
W 6 41 10 61
S 2 V metal P 233 P 234
P 8 43 * ntie
P 8 59 * ntie
W 5 40 11 62
S 3 V ntie P 235 P 236
P 8 59 * ntie
P 28 59 * ntie
W 5 56 31 62
S 3 H ntie P 237 P 238
P 38 46 * pdif
P 38 41 * pdif
W 36 39 40 48
S 2 V pdif P 239 P 240
P 29 46 * pdif
P 29 41 * pdif
W 27 39 31 48
S 2 V pdif P 241 P 242
P 32 46 * pdif
P 32 41 * pdif
W 30 39 34 48
S 2 V pdif P 243 P 244
P 47 52 * pdif
P 47 47 * pdif
W 45 45 49 54
S 2 V pdif P 245 P 246
P 37 19 * ndif
P 37 14 * ndif
W 35 12 39 21
S 2 V ndif P 247 P 248
P 34 19 * ndif
P 34 14 * ndif
W 32 12 36 21
S 2 V ndif P 249 P 250
P 49 19 * ndif
P 49 14 * ndif
W 47 12 51 21
S 2 V ndif P 251 P 252
P 67 24 * ndif
P 67 11 * ndif
W 65 9 69 26
S 2 V ndif P 253 P 254
P 63 24 * ndif
P 63 11 * ndif
W 61 9 65 26
S 2 V ndif P 255 P 256
P 28 19 * ndif
P 28 14 * ndif
W 26 12 30 21
S 2 V ndif P 257 P 258
P 67 49 * pdif
P 67 44 * pdif
W 65 42 69 51
S 2 V pdif P 259 P 260
E
