#cell1 grnbs_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 187 186 214 256
B 187 186 214 256
F F
C e1 187 214 metal2 2 1 * * W e1
C vdd 187 203 metal2 5 2 * * W vdd
C vss 187 247 metal2 4 3 * * W vss
C e6 187 241 metal2 2 4 * * W e6
C vdd 187 235 metal2 4 5 * * W vdd
C e2 187 197 metal2 2 6 * * W e2
C e3 187 192 metal2 2 7 * * W e3
C ck_11 187 253 metal2 2 8 * * W ck_11
C e2 214 197 metal2 2 9 * * E e2
C ck 214 253 metal2 2 10 * * E ck
C e3 214 192 metal2 2 11 * * E e3
C e8 214 219 metal2 2 12 * * E e8
C e8 187 219 metal2 2 13 * * W e8
C e5 214 224 metal2 2 14 * * E e5
C e5 187 224 metal2 2 15 * * W e5
C e4 214 209 metal2 2 16 * * E e4
C e4 187 209 metal2 2 17 * * W e4
C vdd 214 203 metal2 5 18 * * E vdd
C e1 214 214 metal2 2 19 * * E e1
C e7 214 229 metal2 2 20 * * E e7
C e7 187 229 metal2 2 21 * * W e7
C vdd 214 235 metal2 4 22 * * E vdd
C e6 214 241 metal2 2 23 * * E e6
C vss 214 247 metal2 4 24 * * E vss
C vdd 187 229 metal 2 25 * * W vdd
C vss 187 255 metal 1 26 * * W vss
C vss 187 246 metal 2 27 * * W vss
I via1 209 253 0 "cvia" ly * *
I via2 196 253 0 "cvia" ly * *
I cpf1 202 253 0 "cpf" ly * *
I via3 205 247 0 "cvia" ly * *
I cdn1 205 242 0 "cdn" ly * *
I cdn2 199 247 0 "cdn" ly * *
I cdn3 199 242 0 "cdn" ly * *
I cdp1 199 224 0 "cdp" ly * *
I cdp2 199 229 0 "cdp" ly * *
I cdp3 199 219 0 "cdp" ly * *
I cdp4 193 224 0 "cdp" ly * *
I cdp5 193 219 0 "cdp" ly * *
I cdp6 193 229 0 "cdp" ly * *
I via4 187 203 0 "cvia" ly * *
I cbn1 187 209 0 "cbn" ly * *
I cbn2 187 213 0 "cbn" ly * *
I cbn3 187 217 0 "cbn" ly * *
I cbn4 187 221 0 "cbn" ly * *
I cbp1 211 247 0 "cbp" ly * *
I cbn5 187 225 0 "cbn" ly * *
I cbn6 187 229 0 "cbn" ly * *
I cbn7 187 233 0 "cbn" ly * *
I via5 190 247 0 "cvia" ly * *
I cbp2 187 246 0 "cbp" ly * *
I via6 205 235 0 "cvia" ly * *
I via7 205 203 0 "cvia" ly * *
P 202 250 * poly
P 202 253 * poly
W 201 249 203 254
S 1 V poly P 1 P 2
P 190 246 * metal
P 190 247 * metal
W 188 244 192 249 vss
S 2 V metal P 3 P 4
P 187 246 * metal
P 190 246 * metal
W 185 244 192 248 vss
S 2 H metal P 5 P 6
P 190 247 * metal
P 190 255 * metal
W 189 246 191 256 vss
S 1 V metal P 7 P 8
P 187 255 * metal
P 190 255 * metal
W 186 254 191 256 vss
S 1 H metal P 9 P 10
P 186 255 * metal
P 187 255 * metal
W 185 254 188 256 vss
S 1 H metal P 11 P 12
P 186 255 * metal
P 186 256 * metal
W 185 254 187 257 vss
S 1 V metal P 13 P 14
P 187 229 * metal
P 187 233 * metal
W 185 227 189 235 vdd
S 2 V metal P 15 P 16
P 187 232 * nwell
P 187 233 * nwell
W 183 228 191 237
S 4 V nwell P 17 P 18
P 187 233 * nwell
P 187 235 * nwell
W 183 229 191 239
S 4 V nwell P 19 P 20
P 187 210 * ntie
P 187 233 * ntie
W 184 207 190 236
S 3 V ntie P 21 P 22
P 195 207 * nwell
P 195 232 * nwell
W 175 187 215 252
S 20 V nwell P 23 P 24
P 187 253 * metal2
P 196 253 * metal2
W 185 251 198 255 ck_11
S 2 H metal2 P 25 P 26
P 209 253 * metal2
P 214 253 * metal2
W 207 251 216 255 ck
S 2 H metal2 P 27 P 28
P 202 239 * ntrans
P 202 250 * ntrans
W 201 238 203 251
S 1 V ntrans P 29 P 30
P 199 241 * ndif
P 199 247 * ndif
W 196 238 202 250
S 3 V ndif P 31 P 32
P 205 242 * ndif
P 205 248 * ndif
W 202 239 208 251
S 3 V ndif P 33 P 34
P 196 216 * ptrans
P 196 233 * ptrans
W 195 215 197 234
S 1 V ptrans P 35 P 36
P 193 218 * pdif
P 193 231 * pdif
W 190 215 196 234
S 3 V pdif P 37 P 38
P 199 219 * pdif
P 199 231 * pdif
W 196 216 202 234
S 3 V pdif P 39 P 40
P 196 247 * metal
P 199 247 * metal
W 194 245 201 249
S 2 H metal P 41 P 42
P 199 220 * metal
P 199 247 * metal
W 198 219 200 248
S 1 V metal P 43 P 44
P 196 247 * metal
P 196 253 * metal
W 195 246 197 254
S 1 V metal P 45 P 46
P 196 233 * poly
P 196 239 * poly
W 195 232 197 240
S 1 V poly P 47 P 48
P 196 239 * poly
P 202 239 * poly
W 195 238 203 240
S 1 H poly P 49 P 50
P 202 247 * metal2
P 214 247 * metal2
W 198 243 218 251 vss
S 4 H metal2 P 51 P 52
P 187 203 * metal2
P 204 203 * metal2
W 182 198 209 208 vdd
S 5 H metal2 P 53 P 54
P 204 203 * metal2
P 214 203 * metal2
W 199 198 219 208 vdd
S 5 H metal2 P 55 P 56
P 187 247 * metal2
P 202 247 * metal2
W 183 243 206 251 vss
S 4 H metal2 P 57 P 58
P 187 235 * metal2
P 214 235 * metal2
W 183 231 218 239 vdd
S 4 H metal2 P 59 P 60
P 187 203 * metal
P 187 229 * metal
W 185 201 189 231
S 2 V metal P 61 P 62
P 187 229 * metal
P 193 229 * metal
W 185 227 195 231
S 2 H metal P 63 P 64
P 209 253 * metal
P 209 253 * metal
W 208 252 210 254
S 1 H metal P 65 P 66
P 211 241 * ptie
P 211 248 * ptie
W 208 238 214 251
S 3 V ptie P 67 P 68
P 205 242 * metal
P 205 247 * metal
W 204 241 206 248
S 1 V metal P 69 P 70
P 205 247 * metal
P 211 247 * metal
W 204 246 212 248
S 1 H metal P 71 P 72
P 202 253 * metal
P 209 253 * metal
W 201 252 210 254 ck
S 1 H metal P 73 P 74
P 205 235 * metal
P 205 203 * metal
W 204 202 206 236
S 1 V metal P 75 P 76
P 214 209 * metal2
P 187 209 * metal2
W 185 207 216 211
S 2 H metal2 P 77 P 78
P 214 214 * metal2
P 187 214 * metal2
W 185 212 216 216
S 2 H metal2 P 79 P 80
P 214 219 * metal2
P 187 219 * metal2
W 185 217 216 221
S 2 H metal2 P 81 P 82
P 187 197 * metal2
P 214 197 * metal2
W 185 195 216 199
S 2 H metal2 P 83 P 84
P 187 192 * metal2
P 214 192 * metal2
W 185 190 216 194
S 2 H metal2 P 85 P 86
P 187 224 * metal2
P 214 224 * metal2
W 185 222 216 226
S 2 H metal2 P 87 P 88
P 187 229 * metal2
P 214 229 * metal2
W 185 227 216 231
S 2 H metal2 P 89 P 90
P 187 241 * metal2
P 214 241 * metal2
W 185 239 216 243
S 2 H metal2 P 91 P 92
E
