#cell1 grprw1_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 197 186 212 256
B 197 186 212 256
F F
C vdd0 197 203 metal2 5 1 * * W vdd0
C vdd0 212 203 metal2 5 2 * * E vdd0
C vdd1 212 235 metal2 4 3 * * E vdd1
C vdd1 197 235 metal2 4 4 * * W vdd1
C vss 212 247 metal2 4 5 * * E vss
C e6 212 241 metal2 2 6 * * E e6
C e3 212 192 metal2 2 7 * * E e3
C ck_11 212 253 metal2 2 8 * * E ck_11
C e7 197 229 metal2 2 9 * * W e7
C e7 212 229 metal2 2 10 * * E e7
C e5 212 224 metal2 2 11 * * E e5
C e1 212 214 metal2 2 12 * * E e1
C e8 212 219 metal2 2 13 * * E e8
C e4 212 209 metal2 2 14 * * E e4
C e1p 197 206 poly 1 15 * * W e1p
C vss 197 190 metal 2 16 * * W vss
C vss 212 190 metal 2 17 * * E vss
C vss 197 194 metal 2 18 * * W vss
C e3 197 192 metal2 2 19 * * W e3
C e1 197 214 metal2 2 20 * * W e1
C e8 197 219 metal2 2 21 * * W e8
C e6 197 241 metal2 2 22 * * W e6
C vss 197 247 metal2 4 23 * * W vss
C ck_11 197 253 metal2 2 24 * * W ck_11
C n1 197 255 metal 1 25 * * W n1
C e5 197 224 metal2 2 26 * * W e5
I cdn1 203 246 0 "cdn" ly * *
I cdn2 209 245 0 "cdn" ly * *
I cdn3 197 245 0 "cdn" ly * *
I cdp1 197 229 0 "cdp" ly * *
I cdp2 209 229 0 "cdp" ly * *
I cpf1 203 219 0 "cpf" ly * *
I cdp3 209 224 0 "cdp" ly * *
I via1 197 248 0 "cvia" ly * *
I via2 209 248 0 "cvia" ly * *
I cdp4 197 224 0 "cdp" ly * *
I via3 209 234 0 "cvia" ly * *
I cdp5 203 210 0 "cdp" ly * *
I cdp6 203 234 0 "cdp" ly * *
I cdp7 203 229 0 "cdp" ly * *
I cdp8 203 224 0 "cdp" ly * *
I cpf2 209 203 0 "cpf" ly * *
I via4 209 209 0 "cvia" ly * *
I via5 197 234 0 "cvia" ly * *
I cdp9 197 209 0 "cdp" ly * *
I cdn4 206 197 0 "cdn" ly * *
I cdp10 209 214 0 "cdp" ly * *
I cdn5 197 196 0 "cdn" ly * *
I via6 197 202 0 "cvia" ly * *
I via7 197 214 0 "cvia" ly * *
I cbp1 215 194 0 "cbp" ly * *
I cbp2 215 198 0 "cbp" ly * *
I cbp3 215 245 0 "cbp" ly * *
I cbp4 215 251 0 "cbp" ly * *
I via8 215 248 0 "cvia" ly * *
P 197 235 * metal2
P 212 235 * metal2
W 193 231 216 239
S 4 H metal2 P 1 P 2
P 197 203 * metal2
P 212 203 * metal2
W 192 198 217 208
S 5 H metal2 P 3 P 4
P 203 219 * poly
P 203 221 * poly
W 202 218 204 222 inv
S 1 V poly P 5 P 6
P 200 221 * poly
P 203 221 * poly
W 199 220 204 222 inv
S 1 H poly P 7 P 8
P 203 221 * poly
P 206 221 * poly
W 202 220 207 222 inv
S 1 H poly P 9 P 10
P 212 247 * metal2
P 215 247 * metal2
W 208 243 219 251 vss
S 4 H metal2 P 11 P 12
P 197 247 * metal2
P 212 247 * metal2
W 193 243 216 251 vss
S 4 H metal2 P 13 P 14
P 215 245 * ptie
P 215 251 * ptie
W 212 242 218 254
S 3 V ptie P 15 P 16
P 215 195 * ptie
P 215 199 * ptie
W 212 192 218 202
S 3 V ptie P 17 P 18
P 215 190 * metal
P 215 256 * metal
W 211 186 219 260 vss
S 4 V metal P 19 P 20
P 212 190 * metal
P 215 190 * metal
W 210 188 217 192 vss
S 2 H metal P 21 P 22
P 197 190 * metal
P 212 190 * metal
W 195 188 214 192 vss
S 2 H metal P 23 P 24
P 197 190 * metal
P 197 194 * metal
W 195 188 199 196 vss
S 2 V metal P 25 P 26
P 197 194 * metal
P 197 196 * metal
W 195 192 199 198 vss
S 2 V metal P 27 P 28
P 206 237 * poly
P 206 242 * poly
W 205 236 207 243
S 1 V poly P 29 P 30
P 200 237 * poly
P 200 242 * poly
W 199 236 201 243
S 1 V poly P 31 P 32
P 207 203 * poly
P 207 204 * poly
W 206 202 208 205
S 1 V poly P 33 P 34
P 206 203 * poly
P 206 206 * poly
W 205 202 207 207
S 1 V poly P 35 P 36
P 203 203 * poly
P 206 203 * poly
W 202 202 207 204
S 1 H poly P 37 P 38
P 203 201 * poly
P 203 203 * poly
W 202 200 204 204
S 1 V poly P 39 P 40
P 206 203 * poly
P 207 203 * poly
W 205 202 208 204
S 1 H poly P 41 P 42
P 203 208 * metal
P 203 210 * metal
W 202 207 204 211
S 1 V metal P 43 P 44
P 203 210 * metal
P 203 219 * metal
W 202 209 204 220
S 1 V metal P 45 P 46
P 209 203 * metal
P 209 209 * metal
W 208 202 210 210
S 1 V metal P 47 P 48
P 209 209 * metal2
P 212 209 * metal2
W 207 207 214 211
S 2 H metal2 P 49 P 50
P 209 208 * pdif
P 209 213 * pdif
W 206 205 212 216
S 3 V pdif P 51 P 52
P 203 208 * pdif
P 203 213 * pdif
W 201 206 205 215
S 2 V pdif P 53 P 54
P 206 194 * ndif
P 206 199 * ndif
W 204 192 208 201
S 2 V ndif P 55 P 56
P 197 209 * pdif
P 197 213 * pdif
W 194 206 200 216
S 3 V pdif P 57 P 58
P 203 229 * pdif
P 203 234 * pdif
W 201 227 205 236
S 2 V pdif P 59 P 60
P 197 223 * pdif
P 197 235 * pdif
W 194 220 200 238
S 3 V pdif P 61 P 62
P 209 245 * metal
P 209 248 * metal
W 207 243 211 250
S 2 V metal P 63 P 64
P 197 245 * metal
P 197 248 * metal
W 195 243 199 250
S 2 V metal P 65 P 66
P 197 202 * metal
P 197 209 * metal
W 195 200 199 211 vdd
S 2 V metal P 67 P 68
P 197 206 * poly
P 200 206 * poly
W 196 205 201 207 e1p
S 1 H poly P 69 P 70
P 200 201 * poly
P 200 206 * poly
W 199 200 201 207 e1p
S 1 V poly P 71 P 72
P 209 214 * metal
P 209 234 * metal
W 208 213 210 235
S 1 V metal P 73 P 74
P 203 197 * metal
P 206 197 * metal
W 201 195 208 199
S 2 H metal P 75 P 76
P 203 197 * metal
P 203 208 * metal
W 202 196 204 209
S 1 V metal P 77 P 78
P 197 214 * metal2
P 212 214 * metal2
W 195 212 214 216 e1
S 2 H metal2 P 79 P 80
P 206 206 * ptrans
P 206 215 * ptrans
W 205 205 207 216
S 1 V ptrans P 81 P 82
P 203 224 * metal
P 203 246 * metal
W 202 223 204 247
S 1 V metal P 83 P 84
P 203 247 * ndif
P 203 248 * ndif
W 200 244 206 251
S 3 V ndif P 85 P 86
P 206 207 * nwell
P 206 236 * nwell
W 184 185 228 258
S 22 V nwell P 87 P 88
P 203 244 * ndif
P 203 245 * ndif
W 200 241 206 248
S 3 V ndif P 89 P 90
P 197 194 * ndif
P 197 199 * ndif
W 194 191 200 202
S 3 V ndif P 91 P 92
P 197 219 * metal2
P 212 219 * metal2
W 195 217 214 221 e8
S 2 H metal2 P 93 P 94
P 197 224 * metal2
P 212 224 * metal2
W 195 222 214 226 e5
S 2 H metal2 P 95 P 96
P 197 229 * metal2
P 212 229 * metal2
W 195 227 214 231 e7
S 2 H metal2 P 97 P 98
P 197 253 * metal2
P 212 253 * metal2
W 195 251 214 255 ck_11
S 2 H metal2 P 99 P 100
P 197 192 * metal2
P 212 192 * metal2
W 195 190 214 194 e3
S 2 H metal2 P 101 P 102
P 197 241 * metal2
P 212 241 * metal2
W 195 239 214 243 e6
S 2 H metal2 P 103 P 104
P 209 223 * pdif
P 209 235 * pdif
W 206 220 212 238
S 3 V pdif P 105 P 106
P 203 246 * metal
P 203 255 * metal
W 202 245 204 256 n1
S 1 V metal P 107 P 108
P 197 255 * metal
P 203 255 * metal
W 196 254 204 256 n1
S 1 H metal P 109 P 110
P 203 226 * pdif
P 203 227 * pdif
W 200 223 206 230
S 3 V pdif P 111 P 112
P 209 246 * ndif
P 209 249 * ndif
W 206 243 212 252
S 3 V ndif P 113 P 114
P 197 247 * ndif
P 197 249 * ndif
W 194 244 200 252
S 3 V ndif P 115 P 116
P 203 192 * ntrans
P 203 201 * ntrans
W 202 191 204 202
S 1 V ntrans P 117 P 118
P 200 192 * ntrans
P 200 201 * ntrans
W 199 191 201 202
S 1 V ntrans P 119 P 120
P 200 206 * ptrans
P 200 215 * ptrans
W 199 205 201 216
S 1 V ptrans P 121 P 122
P 200 221 * ptrans
P 200 237 * ptrans
W 199 220 201 238
S 1 V ptrans P 123 P 124
P 206 221 * ptrans
P 206 237 * ptrans
W 205 220 207 238
S 1 V ptrans P 125 P 126
P 206 242 * ntrans
P 206 250 * ntrans
W 205 241 207 251
S 1 V ntrans P 127 P 128
P 200 242 * ntrans
P 200 250 * ntrans
W 199 241 201 251
S 1 V ntrans P 129 P 130
P 207 203 * poly
P 209 203 * poly
W 206 202 210 204
S 1 H poly P 131 P 132
P 197 234 * metal
P 197 224 * metal
W 196 223 198 235
S 1 V metal P 133 P 134
E
