#cell1 grprw2_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 188 186 209 256
B 188 186 209 256
F F
C n2 209 252 metal 1 1 * * E n2
C vdd 209 235 metal2 4 2 * * E vdd
C vdd 209 203 metal2 5 3 * * E vdd
C vss 209 247 metal2 4 4 * * E vss
C e6 209 241 metal2 2 5 * * E e6
C e5 188 224 metal2 2 6 * * W e5
C vss 191 186 metal 4 7 * * S vss
C n1 203 256 metal 1 8 * * N n1
C ck_11 209 253 metal2 2 9 * * E ck_11
C e7 209 229 metal2 2 10 * * E e7
C e1 188 214 metal2 2 11 * * W e1
C e4 188 209 metal2 2 12 * * W e4
C e4 209 209 metal2 2 13 * * E e4
C n2 207 256 metal 1 14 * * N n2
C e8 209 219 metal2 2 15 * * E e8
C e5 209 224 metal2 2 16 * * E e5
C e1 209 214 metal2 2 17 * * E e1
C e2 209 192 poly 1 18 * * E e2
C vss 209 194 metal 3 19 * * E vss
C e7 188 229 metal2 2 20 * * W e7
C e8 188 219 metal2 2 21 * * W e8
C e3 209 192 metal2 2 22 * * E e3
C ck_11 188 253 metal2 2 23 * * W ck_11
C vss 188 247 metal2 4 24 * * W vss
C e6 188 241 metal2 2 25 * * W e6
C vdd 188 235 metal2 4 26 * * W vdd
C vdd 188 203 metal2 5 27 * * W vdd
C vss 191 256 metal 4 28 * * N vss
I cdn1 203 246 0 "cdn" ly * *
I cdn2 209 245 0 "cdn" ly * *
I cdn3 197 245 0 "cdn" ly * *
I cdp1 203 224 0 "cdp" ly * *
I cdp2 203 212 0 "cdp" ly * *
I cdn4 209 197 0 "cdn" ly * *
I cdn5 200 197 0 "cdn" ly * *
I cpf1 203 219 0 "cpf" ly * *
I cdp3 209 229 0 "cdp" ly * *
I cdp4 209 224 0 "cdp" ly * *
I via1 197 248 0 "cvia" ly * *
I via2 209 248 0 "cvia" ly * *
I cdp5 197 229 0 "cdp" ly * *
I cdp6 197 224 0 "cdp" ly * *
I via3 209 234 0 "cvia" ly * *
I cdp7 203 234 0 "cdp" ly * *
I cdp8 203 229 0 "cdp" ly * *
I via4 197 234 0 "cvia" ly * *
I cbn1 197 218 0 "cbn" ly * *
I cpf2 202 191 0 "cpf" ly * *
I cbp1 191 251 0 "cbp" ly * *
I cbp2 191 245 0 "cbp" ly * *
I via5 197 191 0 "cvia" ly * *
I cdp9 209 209 0 "cdp" ly * *
I cdp10 197 211 0 "cdp" ly * *
I cbn2 209 218 0 "cbn" ly * *
I cbp3 191 198 0 "cbp" ly * *
I cbp4 191 194 0 "cbp" ly * *
I via6 209 203 0 "cvia" ly * *
I via7 191 248 0 "cvia" ly * *
P 203 208 * pdif
P 203 211 * pdif
W 201 206 205 213
S 2 V pdif P 1 P 2
P 191 245 * ptie
P 191 251 * ptie
W 188 242 194 254
S 3 V ptie P 3 P 4
P 191 195 * ptie
P 191 199 * ptie
W 188 192 194 202
S 3 V ptie P 5 P 6
P 209 194 * ndif
P 209 199 * ndif
W 206 191 212 202
S 3 V ndif P 7 P 8
P 200 197 * metal
P 200 202 * metal
W 199 196 201 203
S 1 V metal P 9 P 10
P 203 202 * metal
P 203 212 * metal
W 202 201 204 213
S 1 V metal P 11 P 12
P 200 202 * metal
P 203 202 * metal
W 199 201 204 203
S 1 H metal P 13 P 14
P 209 194 * metal
P 209 198 * metal
W 207 192 211 200 vss
S 2 V metal P 15 P 16
P 191 186 * metal
P 209 186 * metal
W 189 184 211 188 vss
S 2 H metal P 17 P 18
P 209 186 * metal
P 209 194 * metal
W 207 184 211 196 vss
S 2 V metal P 19 P 20
P 209 209 * pdif
P 209 213 * pdif
W 206 206 212 216
S 3 V pdif P 21 P 22
P 197 208 * pdif
P 197 213 * pdif
W 194 205 200 216
S 3 V pdif P 23 P 24
P 200 201 * poly
P 200 206 * poly
W 199 200 201 207
S 1 V poly P 25 P 26
P 200 201 * poly
P 203 201 * poly
W 199 200 204 202
S 1 H poly P 27 P 28
P 203 201 * poly
P 203 201 * poly
W 202 200 204 202
S 1 H poly P 29 P 30
P 203 219 * poly
P 203 221 * poly
W 202 218 204 222
S 1 V poly P 31 P 32
P 200 221 * poly
P 203 221 * poly
W 199 220 204 222
S 1 H poly P 33 P 34
P 203 221 * poly
P 206 221 * poly
W 202 220 207 222
S 1 H poly P 35 P 36
P 197 223 * pdif
P 197 235 * pdif
W 194 220 200 238
S 3 V pdif P 37 P 38
P 194 245 * metal
P 194 248 * metal
W 189 240 199 253 vss
S 5 V metal P 39 P 40
P 191 248 * metal
P 194 248 * metal
W 189 246 196 250 vss
S 2 H metal P 41 P 42
P 194 248 * metal
P 197 248 * metal
W 192 246 199 250 vss
S 2 H metal P 43 P 44
P 191 186 * metal
P 191 245 * metal
W 187 182 195 249 vss
S 4 V metal P 45 P 46
P 191 245 * metal
P 191 248 * metal
W 187 241 195 252 vss
S 4 V metal P 47 P 48
P 191 245 * metal
P 194 245 * metal
W 189 243 196 247 vss
S 2 H metal P 49 P 50
P 194 245 * metal
P 197 245 * metal
W 192 243 199 247 vss
S 2 H metal P 51 P 52
P 197 245 * metal
P 197 248 * metal
W 195 243 199 250 vss
S 2 V metal P 53 P 54
P 191 248 * metal
P 191 256 * metal
W 187 244 195 260 vss
S 4 V metal P 55 P 56
P 209 245 * metal
P 209 248 * metal
W 207 243 211 250
S 2 V metal P 57 P 58
P 200 194 * ndif
P 200 199 * ndif
W 197 191 203 202
S 3 V ndif P 59 P 60
P 188 192 * metal2
P 209 192 * metal2
W 186 190 211 194 e3
S 2 H metal2 P 61 P 62
P 209 203 * metal
P 209 208 * metal
W 207 201 211 210
S 2 V metal P 63 P 64
P 206 192 * poly
P 209 192 * poly
W 205 191 210 193 e2
S 1 H poly P 65 P 66
P 203 223 * pdif
P 203 235 * pdif
W 200 220 206 238
S 3 V pdif P 67 P 68
P 206 201 * poly
P 206 206 * poly
W 205 200 207 207
S 1 V poly P 69 P 70
P 200 206 * ptrans
P 200 215 * ptrans
W 199 205 201 216
S 1 V ptrans P 71 P 72
P 188 214 * metal2
P 209 214 * metal2
W 186 212 211 216 e1
S 2 H metal2 P 73 P 74
P 207 252 * metal
P 209 252 * metal
W 206 251 210 253 n2
S 1 H metal P 75 P 76
P 207 252 * metal
P 207 256 * metal
W 206 251 208 257 n2
S 1 V metal P 77 P 78
P 199 207 * nwell
P 199 236 * nwell
W 175 183 223 260
S 24 V nwell P 79 P 80
P 197 211 * metal
P 197 234 * metal
W 195 209 199 236
S 2 V metal P 81 P 82
P 203 244 * ndif
P 203 248 * ndif
W 200 241 206 251
S 3 V ndif P 83 P 84
P 188 219 * metal2
P 209 219 * metal2
W 186 217 211 221 e8
S 2 H metal2 P 85 P 86
P 188 209 * metal2
P 209 209 * metal2
W 186 207 211 211 e4
S 2 H metal2 P 87 P 88
P 188 229 * metal2
P 209 229 * metal2
W 186 227 211 231 e7
S 2 H metal2 P 89 P 90
P 188 253 * metal2
P 209 253 * metal2
W 186 251 211 255 ck_11
S 2 H metal2 P 91 P 92
P 203 212 * metal
P 203 219 * metal
W 202 211 204 220
S 1 V metal P 93 P 94
P 203 246 * metal
P 203 256 * metal
W 202 245 204 257 n1
S 1 V metal P 95 P 96
P 209 223 * pdif
P 209 235 * pdif
W 206 220 212 238
S 3 V pdif P 97 P 98
P 209 245 * ndif
P 209 249 * ndif
W 206 242 212 252
S 3 V ndif P 99 P 100
P 197 245 * ndif
P 197 249 * ndif
W 194 242 200 252
S 3 V ndif P 101 P 102
P 188 224 * metal2
P 209 224 * metal2
W 186 222 211 226 e5
S 2 H metal2 P 103 P 104
P 209 209 * metal
P 209 234 * metal
W 207 207 211 236
S 2 V metal P 105 P 106
P 188 241 * metal2
P 209 241 * metal2
W 186 239 211 243 e6
S 2 H metal2 P 107 P 108
P 188 247 * metal2
P 209 247 * metal2
W 184 243 213 251 vss
S 4 H metal2 P 109 P 110
P 188 203 * metal2
P 209 203 * metal2
W 183 198 214 208 vdd
S 5 H metal2 P 111 P 112
P 188 235 * metal2
P 209 235 * metal2
W 184 231 213 239 vdd
S 4 H metal2 P 113 P 114
P 206 192 * ntrans
P 206 201 * ntrans
W 205 191 207 202
S 1 V ntrans P 115 P 116
P 203 192 * ntrans
P 203 201 * ntrans
W 202 191 204 202
S 1 V ntrans P 117 P 118
P 206 206 * ptrans
P 206 215 * ptrans
W 205 205 207 216
S 1 V ptrans P 119 P 120
P 200 221 * ptrans
P 200 237 * ptrans
W 199 220 201 238
S 1 V ptrans P 121 P 122
P 200 237 * poly
P 200 242 * poly
W 199 236 201 243
S 1 V poly P 123 P 124
P 206 237 * poly
P 206 242 * poly
W 205 236 207 243
S 1 V poly P 125 P 126
P 203 224 * metal
P 203 246 * metal
W 202 223 204 247
S 1 V metal P 127 P 128
P 206 221 * ptrans
P 206 237 * ptrans
W 205 220 207 238
S 1 V ptrans P 129 P 130
P 206 242 * ntrans
P 206 250 * ntrans
W 205 241 207 251
S 1 V ntrans P 131 P 132
P 200 242 * ntrans
P 200 250 * ntrans
W 199 241 201 251
S 1 V ntrans P 133 P 134
P 197 191 * metal
P 202 191 * metal
W 196 190 203 192
S 1 H metal P 135 P 136
P 203 191 * poly
P 203 192 * poly
W 202 190 204 193
S 1 V poly P 137 P 138
P 202 191 * poly
P 203 191 * poly
W 201 190 204 192
S 1 H poly P 139 P 140
E
