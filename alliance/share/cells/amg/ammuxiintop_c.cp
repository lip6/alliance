#cell1 ammuxiintop_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 79 65
B 2 5 79 65
F F
C n2j 63 65 metal 1 1 * * N n2j
C c2j 33 65 metal 1 2 * * N c2j
C vdd 79 65 metal2 2 3 * * E vdd
C vss 2 5 metal2 2 4 * * W vss
C vss 79 5 metal2 2 5 * * E vss
C pi2j 79 23 metal2 2 6 * * E pi2j
C vdd 2 65 metal2 2 7 * * W vdd
C aim1 7 65 poly 1 8 * * N aim1
I cpf1 33 65 0 "cpf" ly * *
I cdp1 47 50 0 "cdp" ly * *
I via1 57 65 0 "cvia" ly * *
I cdp2 76 47 0 "cdp" ly * *
I via2 76 65 0 "cvia" ly * *
I cdp3 21 44 0 "cdp" ly * *
I cdn1 21 16 0 "cdn" ly * *
I cdn2 27 17 0 "cdn" ly * *
I cdn3 37 17 0 "cdn" ly * *
I cdn4 43 17 0 "cdn" ly * *
I cdp4 38 44 0 "cdp" ly * *
I cpf2 46 9 0 "cpf" ly * *
I cdn5 49 17 0 "cdn" ly * *
I cpf3 63 59 0 "cpf" ly * *
I cdn6 73 17 0 "cdn" ly * *
I cdn7 57 22 0 "cdn" ly * *
I cdn8 57 13 0 "cdn" ly * *
I cdn9 73 12 0 "cdn" ly * *
I cpf4 63 5 0 "cpf" ly * *
I cdn10 67 22 0 "cdn" ly * *
I cdn11 67 17 0 "cdn" ly * *
I cdp5 58 50 0 "cdp" ly * *
I cdp6 67 47 0 "cdp" ly * *
I via3 73 5 0 "cvia" ly * *
I via4 28 65 0 "cvia" ly * *
I via5 57 5 0 "cvia" ly * *
I via6 27 5 0 "cvia" ly * *
I cdp7 29 44 0 "cdp" ly * *
I cpf5 33 5 0 "cpf" ly * *
I cpf6 49 25 0 "cpf" ly * *
I cpf7 44 61 0 "cpf" ly * *
I cpf8 43 42 0 "cpf" ly * *
I via7 77 23 0 "cvia" ly * *
I cbp1 27 24 0 "cbp" ly * *
I cbn1 28 58 0 "cbn" ly * *
I cbn2 76 61 0 "cbn" ly * *
I cbn3 76 57 0 "cbn" ly * *
I cpf9 57 30 0 "cpf" ly * *
I cpf10 57 42 0 "cpf" ly * *
I cdp8 15 44 0 "cdp" ly * *
I cdn12 15 16 0 "cdn" ly * *
I cpf11 26 31 0 "cpf" ly * *
I cbn4 15 54 0 "cbn" ly * *
I cbn5 19 54 0 "cbn" ly * *
I cbn6 23 54 0 "cbn" ly * *
I cbn7 27 54 0 "cbn" ly * *
I cbp2 10 24 0 "cbp" ly * *
I cbp3 10 29 0 "cbp" ly * *
I c2j_in 33 58 0 "ref_con" ly * *
I n2j_in 63 53 0 "ref_con" ly * *
P 2 65 * metal2
P 79 65 * metal2
W -4 59 85 71
S 6 H metal2 P 1 P 2
P 18 12 * ntrans
P 18 21 * ntrans
W 17 11 19 22
S 1 V ntrans P 3 P 4
P 30 12 * ntrans
P 30 21 * ntrans
W 29 11 31 22
S 1 V ntrans P 5 P 6
P 27 14 * ndif
P 27 19 * ndif
W 24 11 30 22
S 3 V ndif P 7 P 8
P 21 16 * metal
P 21 44 * metal
W 20 15 22 45 a
S 1 V metal P 9 P 10
P 27 5 * metal
P 27 24 * metal
W 26 4 28 25
S 1 V metal P 11 P 12
P 40 12 * ntrans
P 40 21 * ntrans
W 39 11 41 22
S 1 V ntrans P 13 P 14
P 46 12 * ntrans
P 46 21 * ntrans
W 45 11 47 22
S 1 V ntrans P 15 P 16
P 35 14 * ndif
P 35 19 * ndif
W 29 8 41 25
S 6 V ndif P 17 P 18
P 43 14 * ndif
P 43 19 * ndif
W 40 11 46 22
S 3 V ndif P 19 P 20
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
P 57 50 * metal
P 57 65 * metal
W 56 49 58 66
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
P 77 23 * metal
P 77 30 * metal
W 76 22 78 31
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
P 60 9 * ntrans
P 60 26 * ntrans
W 59 8 61 27
S 1 V ntrans P 49 P 50
P 70 9 * ntrans
P 70 26 * ntrans
W 69 8 71 27
S 1 V ntrans P 51 P 52
P 70 51 * poly
P 70 53 * poly
W 69 50 71 54
S 1 V poly P 53 P 54
P 67 30 * metal
P 77 30 * metal
W 66 29 78 31 pi2j
S 1 H metal P 55 P 56
P 73 5 * metal
P 73 17 * metal
W 72 4 74 18
S 1 V metal P 57 P 58
P 29 44 * metal
P 29 54 * metal
W 28 43 30 55
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
P 7 48 * poly
P 7 65 * poly
W 6 47 8 66 aim1
S 1 V poly P 69 P 70
P 70 26 * poly
P 73 26 * poly
W 69 25 74 27
S 1 H poly P 71 P 72
P 18 39 * ptrans
P 18 48 * ptrans
W 17 38 19 49
S 1 V ptrans P 73 P 74
P 66 44 * pdif
P 66 49 * pdif
W 63 41 69 52
S 3 V pdif P 75 P 76
P 58 47 * pdif
P 58 52 * pdif
W 55 44 61 55
S 3 V pdif P 77 P 78
P 21 41 * pdif
P 21 46 * pdif
W 18 38 24 49
S 3 V pdif P 79 P 80
P 30 41 * pdif
P 30 46 * pdif
W 25 36 35 51
S 5 V pdif P 81 P 82
P 47 47 * pdif
P 47 52 * pdif
W 44 44 50 55
S 3 V pdif P 83 P 84
P 39 41 * pdif
P 39 46 * pdif
W 36 38 42 49
S 3 V pdif P 85 P 86
P 7 48 * poly
P 9 48 * poly
W 6 47 10 49
S 1 H poly P 87 P 88
P 2 52 * nwell
P 79 52 * nwell
W -24 26 105 78
S 26 H nwell P 89 P 90
P 67 17 * metal
P 67 47 * metal
W 66 16 68 48
S 1 V metal P 91 P 92
P 63 5 * metal
P 63 65 * metal
W 62 4 64 66 n2j
S 1 V metal P 93 P 94
P 62 5 * metal
P 63 5 * metal
W 61 4 64 6
S 1 H metal P 95 P 96
P 76 44 * pdif
P 76 49 * pdif
W 73 41 79 52
S 3 V pdif P 97 P 98
P 73 26 * poly
P 73 42 * poly
W 72 25 74 43
S 1 V poly P 99 P 100
P 37 48 * poly
P 37 60 * poly
W 36 47 38 61
S 1 V poly P 101 P 102
P 35 48 * poly
P 37 48 * poly
W 34 47 38 49
S 1 H poly P 103 P 104
P 33 5 * poly
P 33 12 * poly
W 32 4 34 13
S 1 V poly P 105 P 106
P 37 9 * metal
P 37 28 * metal
W 36 8 38 29
S 1 V metal P 107 P 108
P 40 21 * poly
P 40 25 * poly
W 39 20 41 26
S 1 V poly P 109 P 110
P 49 17 * metal
P 49 25 * metal
W 48 16 50 26
S 1 V metal P 111 P 112
P 38 61 * metal
P 44 61 * metal
W 37 60 45 62
S 1 H metal P 113 P 114
P 44 61 * poly
P 50 61 * poly
W 43 60 51 62
S 1 H poly P 115 P 116
P 50 54 * poly
P 50 61 * poly
W 49 53 51 62
S 1 V poly P 117 P 118
P 30 25 * poly
P 49 25 * poly
W 29 24 50 26
S 1 H poly P 119 P 120
P 30 25 * poly
P 30 31 * poly
W 29 24 31 32
S 1 V poly P 121 P 122
P 43 17 * metal
P 43 50 * metal
W 42 16 44 51 t_s
S 1 V metal P 123 P 124
P 43 50 * metal
P 47 50 * metal
W 42 49 48 51
S 1 H metal P 125 P 126
P 38 28 * metal
P 38 61 * metal
W 37 27 39 62
S 1 V metal P 127 P 128
P 37 28 * metal
P 38 28 * metal
W 36 27 39 29
S 1 H metal P 129 P 130
P 30 12 * poly
P 33 12 * poly
W 29 11 34 13
S 1 H poly P 131 P 132
P 77 23 * metal2
P 79 23 * metal2
W 75 21 81 25
S 2 H metal2 P 133 P 134
P 2 5 * metal2
P 79 5 * metal2
W -6 -3 87 13
S 8 H metal2 P 135 P 136
P 28 54 * metal
P 28 65 * metal
W 26 52 30 67
S 2 V metal P 137 P 138
P 28 54 * metal
P 29 54 * metal
W 27 53 30 55
S 1 H metal P 139 P 140
P 57 50 * metal
P 58 50 * metal
W 56 49 59 51
S 1 H metal P 141 P 142
P 33 64 * poly
P 33 65 * poly
W 32 63 34 66
S 1 V poly P 143 P 144
P 33 61 * poly
P 37 61 * poly
W 32 60 38 62
S 1 H poly P 145 P 146
P 37 60 * poly
P 37 61 * poly
W 36 59 38 62
S 1 V poly P 147 P 148
P 63 59 * poly
P 70 59 * poly
W 62 58 71 60
S 1 H poly P 149 P 150
P 70 52 * poly
P 70 59 * poly
W 69 51 71 60
S 1 V poly P 151 P 152
P 33 61 * poly
P 33 65 * poly
W 32 60 34 66
S 1 V poly P 153 P 154
P 76 57 * metal
P 76 65 * metal
W 74 55 78 67
S 2 V metal P 155 P 156
P 76 57 * ntie
P 76 61 * ntie
W 73 54 79 64
S 3 V ntie P 157 P 158
P 21 14 * ndif
P 21 19 * ndif
W 18 11 24 22
S 3 V ndif P 159 P 160
P 51 35 * poly
P 51 42 * poly
W 50 34 52 43
S 1 V poly P 161 P 162
P 51 35 * poly
P 73 35 * poly
W 50 34 74 36
S 1 H poly P 163 P 164
P 43 42 * poly
P 51 42 * poly
W 42 41 52 43
S 1 H poly P 165 P 166
P 49 25 * poly
P 53 25 * poly
W 48 24 54 26
S 1 H poly P 167 P 168
P 53 25 * poly
P 53 30 * poly
W 52 24 54 31
S 1 V poly P 169 P 170
P 53 30 * poly
P 57 30 * poly
W 52 29 58 31
S 1 H poly P 171 P 172
P 57 30 * metal
P 57 42 * metal
W 56 29 58 43
S 1 V metal P 173 P 174
P 55 42 * poly
P 57 42 * poly
W 54 41 58 43
S 1 H poly P 175 P 176
P 55 42 * poly
P 55 45 * poly
W 54 41 56 46
S 1 V poly P 177 P 178
P 9 48 * poly
P 18 48 * poly
W 8 47 19 49
S 1 H poly P 179 P 180
P 18 21 * poly
P 18 39 * poly
W 17 20 19 40
S 1 V poly P 181 P 182
P 15 41 * pdif
P 15 46 * pdif
W 12 38 18 49
S 3 V pdif P 183 P 184
P 15 14 * ndif
P 15 19 * ndif
W 12 11 18 22
S 3 V ndif P 185 P 186
P 15 44 * metal
P 15 54 * metal
W 14 43 16 55
S 1 V metal P 187 P 188
P 15 54 * metal
P 28 54 * metal
W 13 52 30 56
S 2 H metal P 189 P 190
P 15 10 * metal
P 15 16 * metal
W 14 9 16 17
S 1 V metal P 191 P 192
P 15 10 * metal
P 27 10 * metal
W 14 9 28 11
S 1 H metal P 193 P 194
P 21 31 * metal
P 26 31 * metal
W 20 30 27 32
S 1 H metal P 195 P 196
P 26 31 * poly
P 30 31 * poly
W 25 30 31 32
S 1 H poly P 197 P 198
P 15 54 * ntie
P 29 54 * ntie
W 12 51 32 57
S 3 H ntie P 199 P 200
P 28 54 * ntie
P 28 58 * ntie
W 25 51 31 61
S 3 V ntie P 201 P 202
P 10 16 * metal
P 15 16 * metal
W 9 15 16 17
S 1 H metal P 203 P 204
P 10 16 * metal
P 10 29 * metal
W 8 14 12 31
S 2 V metal P 205 P 206
P 10 24 * ptie
P 10 29 * ptie
W 7 21 13 32
S 3 V ptie P 207 P 208
P 29 46 * pdif
P 29 41 * pdif
W 27 39 31 48
S 2 V pdif P 209 P 210
P 32 46 * pdif
P 32 41 * pdif
W 30 39 34 48
S 2 V pdif P 211 P 212
P 38 46 * pdif
P 38 41 * pdif
W 36 39 40 48
S 2 V pdif P 213 P 214
P 47 52 * pdif
P 47 47 * pdif
W 45 45 49 54
S 2 V pdif P 215 P 216
P 67 49 * pdif
P 67 44 * pdif
W 65 42 69 51
S 2 V pdif P 217 P 218
P 63 24 * ndif
P 63 11 * ndif
W 61 9 65 26
S 2 V ndif P 219 P 220
P 67 24 * ndif
P 67 11 * ndif
W 65 9 69 26
S 2 V ndif P 221 P 222
P 49 19 * ndif
P 49 14 * ndif
W 47 12 51 21
S 2 V ndif P 223 P 224
P 33 19 * ndif
P 33 14 * ndif
W 31 12 35 21
S 2 V ndif P 225 P 226
E
