#cell1 grpbs_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 187 186 214 256
B 187 186 214 256
F F
C vss 214 247 metal2 4 1 * * E vss
C e6 214 241 metal2 2 2 * * E e6
C vdd 214 235 metal2 4 3 * * E vdd
C e7 187 229 metal2 2 4 * * W e7
C e7 214 229 metal2 2 5 * * E e7
C e1 214 214 metal2 2 6 * * E e1
C vdd 214 203 metal2 5 7 * * E vdd
C e4 187 209 metal2 2 8 * * W e4
C e4 214 209 metal2 2 9 * * E e4
C e5 187 224 metal2 2 10 * * W e5
C e5 214 224 metal2 2 11 * * E e5
C e8 187 219 metal2 2 12 * * W e8
C e8 214 219 metal2 2 13 * * E e8
C e3 214 192 metal2 2 14 * * E e3
C ck 214 253 metal2 2 15 * * E ck
C e2 214 197 metal2 2 16 * * E e2
C ck_11 187 253 metal2 2 17 * * W ck_11
C e3 187 192 metal2 2 18 * * W e3
C e2 187 197 metal2 2 19 * * W e2
C vdd 187 235 metal2 4 20 * * W vdd
C e6 187 241 metal2 2 21 * * W e6
C vss 187 247 metal2 4 22 * * W vss
C vdd 187 203 metal2 5 23 * * W vdd
C ck 209 256 metal 1 24 * * N ck
C e1 187 214 metal2 2 25 * * W e1
I cbp1 211 247 0 "cbp" ly * *
I cbn1 187 221 0 "cbn" ly * *
I cbn2 187 217 0 "cbn" ly * *
I cbn3 187 213 0 "cbn" ly * *
I cbn4 187 209 0 "cbn" ly * *
I via1 187 203 0 "cvia" ly * *
I cdp1 193 229 0 "cdp" ly * *
I cdp2 193 219 0 "cdp" ly * *
I cdp3 193 224 0 "cdp" ly * *
I cdp4 199 219 0 "cdp" ly * *
I cdp5 199 229 0 "cdp" ly * *
I cdp6 199 224 0 "cdp" ly * *
I cdn1 199 242 0 "cdn" ly * *
I cdn2 199 247 0 "cdn" ly * *
I cdn3 205 242 0 "cdn" ly * *
I via2 205 247 0 "cvia" ly * *
I cpf1 202 253 0 "cpf" ly * *
I via3 196 253 0 "cvia" ly * *
I via4 209 253 0 "cvia" ly * *
I via5 205 235 0 "cvia" ly * *
I via6 205 203 0 "cvia" ly * *
P 209 253 * metal
P 209 256 * metal
W 208 252 210 257 ck
S 1 V metal P 1 P 2
P 202 253 * metal
P 209 253 * metal
W 201 252 210 254 ck
S 1 H metal P 3 P 4
P 205 247 * metal
P 211 247 * metal
W 204 246 212 248
S 1 H metal P 5 P 6
P 205 242 * metal
P 205 247 * metal
W 204 241 206 248
S 1 V metal P 7 P 8
P 211 241 * ptie
P 211 248 * ptie
W 208 238 214 251
S 3 V ptie P 9 P 10
P 209 253 * metal
P 209 253 * metal
W 208 252 210 254
S 1 H metal P 11 P 12
P 187 229 * metal
P 193 229 * metal
W 185 227 195 231
S 2 H metal P 13 P 14
P 187 203 * metal
P 187 229 * metal
W 185 201 189 231
S 2 V metal P 15 P 16
P 188 229 * metal2
P 214 229 * metal2
W 186 227 216 231 e7
S 2 H metal2 P 17 P 18
P 187 229 * metal2
P 188 229 * metal2
W 185 227 190 231 e7
S 2 H metal2 P 19 P 20
P 201 214 * metal2
P 212 214 * metal2
W 199 212 214 216 e1
S 2 H metal2 P 21 P 22
P 187 214 * metal2
P 201 214 * metal2
W 185 212 203 216 e1
S 2 H metal2 P 23 P 24
P 197 219 * metal2
P 214 219 * metal2
W 195 217 216 221 e8
S 2 H metal2 P 25 P 26
P 187 219 * metal2
P 197 219 * metal2
W 185 217 199 221 e8
S 2 H metal2 P 27 P 28
P 187 197 * metal2
P 214 197 * metal2
W 185 195 216 199 e2
S 2 H metal2 P 29 P 30
P 187 192 * metal2
P 214 192 * metal2
W 185 190 216 194 e3
S 2 H metal2 P 31 P 32
P 187 241 * metal2
P 214 241 * metal2
W 185 239 216 243 e6
S 2 H metal2 P 33 P 34
P 187 235 * metal2
P 214 235 * metal2
W 183 231 218 239 vdd
S 4 H metal2 P 35 P 36
P 187 247 * metal2
P 202 247 * metal2
W 183 243 206 251 vss
S 4 H metal2 P 37 P 38
P 204 203 * metal2
P 214 203 * metal2
W 199 198 219 208 vdd
S 5 H metal2 P 39 P 40
P 187 203 * metal2
P 204 203 * metal2
W 182 198 209 208 vdd
S 5 H metal2 P 41 P 42
P 202 247 * metal2
P 214 247 * metal2
W 198 243 218 251 vss
S 4 H metal2 P 43 P 44
P 196 239 * poly
P 202 239 * poly
W 195 238 203 240
S 1 H poly P 45 P 46
P 196 233 * poly
P 196 239 * poly
W 195 232 197 240
S 1 V poly P 47 P 48
P 196 247 * metal
P 196 253 * metal
W 195 246 197 254
S 1 V metal P 49 P 50
P 199 220 * metal
P 199 247 * metal
W 198 219 200 248
S 1 V metal P 51 P 52
P 196 247 * metal
P 199 247 * metal
W 194 245 201 249
S 2 H metal P 53 P 54
P 199 219 * pdif
P 199 231 * pdif
W 196 216 202 234
S 3 V pdif P 55 P 56
P 193 218 * pdif
P 193 231 * pdif
W 190 215 196 234
S 3 V pdif P 57 P 58
P 196 216 * ptrans
P 196 233 * ptrans
W 195 215 197 234
S 1 V ptrans P 59 P 60
P 205 242 * ndif
P 205 248 * ndif
W 202 239 208 251
S 3 V ndif P 61 P 62
P 199 241 * ndif
P 199 247 * ndif
W 196 238 202 250
S 3 V ndif P 63 P 64
P 202 239 * ntrans
P 202 250 * ntrans
W 201 238 203 251
S 1 V ntrans P 65 P 66
P 193 209 * metal2
P 214 209 * metal2
W 191 207 216 211 e4
S 2 H metal2 P 67 P 68
P 187 209 * metal2
P 193 209 * metal2
W 185 207 195 211 e4
S 2 H metal2 P 69 P 70
P 212 214 * metal2
P 214 214 * metal2
W 210 212 216 216 e1
S 2 H metal2 P 71 P 72
P 209 253 * metal2
P 214 253 * metal2
W 207 251 216 255 ck
S 2 H metal2 P 73 P 74
P 187 253 * metal2
P 196 253 * metal2
W 185 251 198 255 ck_11
S 2 H metal2 P 75 P 76
P 195 207 * nwell
P 195 232 * nwell
W 175 187 215 252
S 20 V nwell P 77 P 78
P 187 224 * metal2
P 214 224 * metal2
W 185 222 216 226 e5
S 2 H metal2 P 79 P 80
P 202 250 * poly
P 202 253 * poly
W 201 249 203 254
S 1 V poly P 81 P 82
P 187 209 * ntie
P 187 221 * ntie
W 184 206 190 224
S 3 V ntie P 83 P 84
P 205 235 * metal
P 205 203 * metal
W 204 202 206 236
S 1 V metal P 85 P 86
E
