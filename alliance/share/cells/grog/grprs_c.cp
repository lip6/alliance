#cell1 grprs_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 222 218 263 254
B 222 218 263 254
F F
C o 222 252 metal2 2 1 * * W o
C vss 222 245 metal2 3 2 * * W vss
C vdd 263 228 metal2 3 3 * * E vdd
C e2 263 221 metal2 2 4 * * E e2
C e1 263 233 metal2 2 5 * * E e1
C vss 263 245 metal2 3 6 * * E vss
C ck 258 254 metal 1 7 * * N ck
C ck 258 218 metal 1 8 * * S ck
C vdd 222 228 metal2 3 9 * * W vdd
C ck_02 222 242 poly 1 10 * * W ck_02
I cdn1 229 245 0 "cdn" ly * *
I cdn2 228 233 0 "cdn" ly * *
I cdn3 228 239 0 "cdn" ly * *
I cdn4 228 224 0 "cdn" ly * *
I cdp1 241 221 0 "cdp" ly * *
I cdp2 240 227 0 "cdp" ly * *
I cdp3 241 233 0 "cdp" ly * *
I cdp4 240 251 0 "cdp" ly * *
I cdp5 240 242 0 "cdp" ly * *
I cdp6 250 242 0 "cdp" ly * *
I via1 232 245 0 "cvia" ly * *
I cpf1 250 225 0 "cpf" ly * *
I via2 252 221 0 "cvia" ly * *
I via3 245 227 0 "cvia" ly * *
I cbp1 229 251 0 "cbp" ly * *
I cbn1 249 236 0 "cbn" ly * *
I cpf2 250 231 0 "cpf" ly * *
I via4 254 233 0 "cvia" ly * *
I via5 245 251 0 "cvia" ly * *
I via6 224 252 0 "cvia" ly * *
I cdp7 245 242 0 "cdp" ly * *
I cdp8 250 251 0 "cdp" ly * *
I cpf3 236 236 0 "cpf" ly * *
P 259 218 * nwell
P 259 254 * nwell
W 255 214 263 258
S 4 V nwell P 1 P 2
P 258 254 * metal
P 258 257 * metal
W 257 253 259 258 ck
S 1 V metal P 3 P 4
P 258 218 * metal
P 258 254 * metal
W 257 217 259 255 ck
S 1 V metal P 5 P 6
P 227 239 * ndif
P 230 239 * ndif
W 224 236 233 242
S 3 H ndif P 7 P 8
P 225 242 * ntrans
P 232 242 * ntrans
W 224 241 233 243
S 1 H ntrans P 9 P 10
P 250 225 * metal
P 251 225 * metal
W 248 223 253 227
S 2 H metal P 11 P 12
P 251 221 * metal
P 251 225 * metal
W 250 220 252 226
S 1 V metal P 13 P 14
P 251 221 * metal
P 252 221 * metal
W 249 219 254 223
S 2 H metal P 15 P 16
P 250 231 * metal
P 250 232 * metal
W 248 229 252 234
S 2 V metal P 17 P 18
P 250 232 * metal
P 254 232 * metal
W 249 231 255 233
S 1 H metal P 19 P 20
P 254 232 * metal
P 254 233 * metal
W 252 230 256 235
S 2 V metal P 21 P 22
P 248 236 * ntie
P 259 236 * ntie
W 245 233 262 239
S 3 H ntie P 23 P 24
P 245 236 * metal
P 248 236 * metal
W 244 235 249 237
S 1 H metal P 25 P 26
P 245 242 * metal
P 250 242 * metal
W 244 241 251 243
S 1 H metal P 27 P 28
P 240 242 * metal
P 245 242 * metal
W 239 241 246 243
S 1 H metal P 29 P 30
P 245 236 * metal
P 245 242 * metal
W 244 235 246 243
S 1 V metal P 31 P 32
P 245 233 * metal
P 245 236 * metal
W 244 232 246 237
S 1 V metal P 33 P 34
P 241 233 * metal
P 245 233 * metal
W 240 232 246 234
S 1 H metal P 35 P 36
P 245 221 * metal
P 245 233 * metal
W 244 220 246 234
S 1 V metal P 37 P 38
P 241 221 * metal
P 245 221 * metal
W 240 220 246 222
S 1 H metal P 39 P 40
P 250 224 * poly
P 250 225 * poly
W 249 223 251 226
S 1 V poly P 41 P 42
P 245 224 * poly
P 250 224 * poly
W 244 223 251 225
S 1 H poly P 43 P 44
P 229 245 * metal
P 229 251 * metal
W 228 244 230 252
S 1 V metal P 45 P 46
P 228 233 * metal
P 232 233 * metal
W 227 232 233 234
S 1 H metal P 47 P 48
P 232 233 * metal
P 232 245 * metal
W 231 232 233 246
S 1 V metal P 49 P 50
P 229 245 * metal
P 232 245 * metal
W 227 243 234 247
S 2 H metal P 51 P 52
P 228 224 * metal
P 236 224 * metal
W 227 223 237 225
S 1 H metal P 53 P 54
P 236 224 * metal
P 236 227 * metal
W 235 223 237 228
S 1 V metal P 55 P 56
P 236 227 * metal
P 236 236 * metal
W 235 226 237 237
S 1 V metal P 57 P 58
P 236 227 * metal
P 240 227 * metal
W 235 226 241 228
S 1 H metal P 59 P 60
P 222 228 * metal2
P 250 228 * metal2
W 219 225 253 231 vdd
S 3 H metal2 P 61 P 62
P 250 228 * metal2
P 263 228 * metal2
W 247 225 266 231 vdd
S 3 H metal2 P 63 P 64
P 237 224 * poly
P 237 227 * poly
W 236 223 238 228
S 1 V poly P 65 P 66
P 232 227 * poly
P 237 227 * poly
W 231 226 238 228
S 1 H poly P 67 P 68
P 232 230 * poly
P 237 230 * poly
W 231 229 238 231
S 1 H poly P 69 P 70
P 232 236 * poly
P 236 236 * poly
W 231 235 237 237
S 1 H poly P 71 P 72
P 236 245 * poly
P 237 245 * poly
W 235 244 238 246
S 1 H poly P 73 P 74
P 236 236 * poly
P 236 245 * poly
W 235 235 237 246
S 1 V poly P 75 P 76
P 225 248 * poly
P 237 248 * poly
W 224 247 238 249
S 1 H poly P 77 P 78
P 225 242 * poly
P 225 248 * poly
W 224 241 226 249
S 1 V poly P 79 P 80
P 222 242 * poly
P 225 242 * poly
W 221 241 226 243
S 1 H poly P 81 P 82
P 227 245 * ndif
P 230 245 * ndif
W 225 243 232 247
S 2 H ndif P 83 P 84
P 248 251 * pdif
P 249 251 * pdif
W 245 248 252 254
S 3 H pdif P 85 P 86
P 252 221 * metal2
P 263 221 * metal2
W 250 219 265 223 e2
S 2 H metal2 P 87 P 88
P 254 233 * metal2
P 263 233 * metal2
W 252 231 265 235 e1
S 2 H metal2 P 89 P 90
P 241 227 * pdif
P 243 227 * pdif
W 238 224 246 230
S 3 H pdif P 91 P 92
P 239 221 * pdif
P 243 221 * pdif
W 236 218 246 224
S 3 H pdif P 93 P 94
P 239 233 * pdif
P 243 233 * pdif
W 236 230 246 236
S 3 H pdif P 95 P 96
P 239 242 * pdif
P 251 242 * pdif
W 236 239 254 245
S 3 H pdif P 97 P 98
P 240 251 * metal
P 250 251 * metal
W 239 250 251 252
S 1 H metal P 99 P 100
P 240 251 * pdif
P 247 251 * pdif
W 237 248 250 254
S 3 H pdif P 101 P 102
P 249 218 * nwell
P 249 254 * nwell
W 227 196 271 276
S 22 V nwell P 103 P 104
P 224 239 * metal
P 224 252 * metal
W 223 238 225 253
S 1 V metal P 105 P 106
P 224 239 * metal
P 228 239 * metal
W 223 238 229 240
S 1 H metal P 107 P 108
P 226 233 * ndif
P 230 233 * ndif
W 223 230 233 236
S 3 H ndif P 109 P 110
P 222 245 * metal2
P 263 245 * metal2
W 219 242 266 248 vss
S 3 H metal2 P 111 P 112
P 222 252 * metal2
P 245 252 * metal2
W 220 250 247 254 o
S 2 H metal2 P 113 P 114
P 226 224 * ndif
P 230 224 * ndif
W 224 222 232 226
S 2 H ndif P 115 P 116
P 237 224 * ptrans
P 245 224 * ptrans
W 236 223 246 225
S 1 H ptrans P 117 P 118
P 237 248 * ptrans
P 253 248 * ptrans
W 236 247 254 249
S 1 H ptrans P 119 P 120
P 237 245 * ptrans
P 253 245 * ptrans
W 236 244 254 246
S 1 H ptrans P 121 P 122
P 237 230 * ptrans
P 245 230 * ptrans
W 236 229 246 231
S 1 H ptrans P 123 P 124
P 224 227 * ntrans
P 232 227 * ntrans
W 223 226 233 228
S 1 H ntrans P 125 P 126
P 224 230 * ntrans
P 232 230 * ntrans
W 223 229 233 231
S 1 H ntrans P 127 P 128
P 225 236 * ntrans
P 232 236 * ntrans
W 224 235 233 237
S 1 H ntrans P 129 P 130
P 245 230 * poly
P 250 230 * poly
W 244 229 251 231
S 1 H poly P 131 P 132
P 250 230 * poly
P 250 231 * poly
W 249 229 251 232
S 1 V poly P 133 P 134
E
