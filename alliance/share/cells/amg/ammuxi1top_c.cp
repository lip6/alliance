#cell1 ammuxi1top_c idps_symb compose *
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
C vdd 2 65 metal2 2 7 * * W vdd
C sum 2 23 metal2 2 8 * * W sum
C sum 79 23 metal2 2 9 * * E sum
C aim1 7 65 poly 1 10 * * N aim1
I cpf1 33 51 0 "cpf" ly * *
I cdp1 47 50 0 "cdp" ly * *
I via1 58 65 0 "cvia" ly * *
I cdp2 76 47 0 "cdp" ly * *
I via2 76 65 0 "cvia" ly * *
I cdp3 21 44 0 "cdp" ly * *
I cdn1 15 16 0 "cdn" ly * *
I cdn2 27 17 0 "cdn" ly * *
I cdn3 37 17 0 "cdn" ly * *
I cdn4 43 17 0 "cdn" ly * *
I cdp4 38 44 0 "cdp" ly * *
I cpf2 46 9 0 "cpf" ly * *
I cdn5 49 17 0 "cdn" ly * *
I cpf3 63 40 0 "cpf" ly * *
I cdn6 73 22 0 "cdn" ly * *
I cdn7 57 22 0 "cdn" ly * *
I cdn8 57 13 0 "cdn" ly * *
I cdn9 73 13 0 "cdn" ly * *
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
I cpf7 48 58 0 "cpf" ly * *
I cpf8 43 42 0 "cpf" ly * *
I cbp1 57 29 0 "cbp" ly * *
I cbn1 28 59 0 "cbn" ly * *
I cbn2 76 58 0 "cbn" ly * *
I cdp8 15 44 0 "cdp" ly * *
I cdn12 21 16 0 "cdn" ly * *
I cpf9 26 31 0 "cpf" ly * *
I cbn3 28 54 0 "cbn" ly * *
I cbn4 24 54 0 "cbn" ly * *
I cbn5 20 54 0 "cbn" ly * *
I cbn6 16 54 0 "cbn" ly * *
I cbp2 8 16 0 "cbp" ly * *
I cbp3 8 20 0 "cbp" ly * *
I cbp4 8 24 0 "cbp" ly * *
I cbp5 8 28 0 "cbp" ly * *
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
P 15 14 * ndif
P 15 19 * ndif
W 12 11 18 22
S 3 V ndif P 5 P 6
P 27 14 * ndif
P 27 19 * ndif
W 24 11 30 22
S 3 V ndif P 7 P 8
P 27 5 * metal
P 27 17 * metal
W 26 4 28 18
S 1 V metal P 9 P 10
P 40 12 * ntrans
P 40 21 * ntrans
W 39 11 41 22
S 1 V ntrans P 11 P 12
P 46 12 * ntrans
P 46 21 * ntrans
W 45 11 47 22
S 1 V ntrans P 13 P 14
P 37 9 * metal
P 46 9 * metal
W 36 8 47 10
S 1 H metal P 15 P 16
P 46 9 * poly
P 46 12 * poly
W 45 8 47 13
S 1 V poly P 17 P 18
P 50 14 * ndif
P 50 19 * ndif
W 47 11 53 22
S 3 V ndif P 19 P 20
P 58 50 * metal
P 58 65 * metal
W 57 49 59 66
S 1 V metal P 21 P 22
P 73 11 * ndif
P 73 24 * ndif
W 70 8 76 27
S 3 V ndif P 23 P 24
P 57 11 * ndif
P 57 24 * ndif
W 54 8 60 27
S 3 V ndif P 25 P 26
P 57 5 * metal
P 57 22 * metal
W 56 4 58 23
S 1 V metal P 27 P 28
P 76 47 * metal
P 76 65 * metal
W 75 46 77 66
S 1 V metal P 29 P 30
P 35 39 * ptrans
P 35 48 * ptrans
W 34 38 36 49
S 1 V ptrans P 31 P 32
P 50 45 * ptrans
P 50 54 * ptrans
W 49 44 51 55
S 1 V ptrans P 33 P 34
P 55 45 * ptrans
P 55 54 * ptrans
W 54 44 56 55
S 1 V ptrans P 35 P 36
P 70 42 * ptrans
P 70 51 * ptrans
W 69 41 71 52
S 1 V ptrans P 37 P 38
P 73 42 * ptrans
P 73 51 * ptrans
W 72 41 74 52
S 1 V ptrans P 39 P 40
P 60 9 * ntrans
P 60 26 * ntrans
W 59 8 61 27
S 1 V ntrans P 41 P 42
P 70 9 * ntrans
P 70 26 * ntrans
W 69 8 71 27
S 1 V ntrans P 43 P 44
P 63 40 * poly
P 70 40 * poly
W 62 39 71 41
S 1 H poly P 45 P 46
P 67 33 * metal
P 79 33 * metal
W 66 32 80 34 pi2j
S 1 H metal P 47 P 48
P 73 5 * metal
P 73 22 * metal
W 72 4 74 23
S 1 V metal P 49 P 50
P 28 54 * metal
P 28 65 * metal
W 26 52 30 67
S 2 V metal P 51 P 52
P 65 11 * ndif
P 65 24 * ndif
W 60 6 70 29
S 5 V ndif P 53 P 54
P 60 9 * poly
P 63 9 * poly
W 59 8 64 10
S 1 H poly P 55 P 56
P 63 5 * poly
P 63 9 * poly
W 62 4 64 10
S 1 V poly P 57 P 58
P 33 5 * metal
P 33 65 * metal
W 32 4 34 66 c2j
S 1 V metal P 59 P 60
P 7 48 * poly
P 7 65 * poly
W 6 47 8 66
S 1 V poly P 61 P 62
P 18 39 * ptrans
P 18 48 * ptrans
W 17 38 19 49
S 1 V ptrans P 63 P 64
P 70 26 * poly
P 73 26 * poly
W 69 25 74 27
S 1 H poly P 65 P 66
P 66 44 * pdif
P 66 49 * pdif
W 63 41 69 52
S 3 V pdif P 67 P 68
P 58 47 * pdif
P 58 52 * pdif
W 55 44 61 55
S 3 V pdif P 69 P 70
P 21 41 * pdif
P 21 46 * pdif
W 18 38 24 49
S 3 V pdif P 71 P 72
P 30 41 * pdif
P 30 46 * pdif
W 25 36 35 51
S 5 V pdif P 73 P 74
P 47 47 * pdif
P 47 52 * pdif
W 44 44 50 55
S 3 V pdif P 75 P 76
P 39 41 * pdif
P 39 46 * pdif
W 36 38 42 49
S 3 V pdif P 77 P 78
P 7 48 * poly
P 8 48 * poly
W 6 47 9 49
S 1 H poly P 79 P 80
P 2 52 * nwell
P 79 52 * nwell
W -24 26 105 78
S 26 H nwell P 81 P 82
P 67 17 * metal
P 67 47 * metal
W 66 16 68 48
S 1 V metal P 83 P 84
P 63 5 * metal
P 63 65 * metal
W 62 4 64 66 n2j
S 1 V metal P 85 P 86
P 62 5 * metal
P 63 5 * metal
W 61 4 64 6
S 1 H metal P 87 P 88
P 76 44 * pdif
P 76 49 * pdif
W 73 41 79 52
S 3 V pdif P 89 P 90
P 73 26 * poly
P 73 42 * poly
W 72 25 74 43
S 1 V poly P 91 P 92
P 17 48 * poly
P 18 48 * poly
W 16 47 19 49
S 1 H poly P 93 P 94
P 33 5 * poly
P 33 12 * poly
W 32 4 34 13
S 1 V poly P 95 P 96
P 37 9 * metal
P 37 28 * metal
W 36 8 38 29
S 1 V metal P 97 P 98
P 26 31 * poly
P 30 31 * poly
W 25 30 31 32
S 1 H poly P 99 P 100
P 40 21 * poly
P 40 25 * poly
W 39 20 41 26
S 1 V poly P 101 P 102
P 49 17 * metal
P 49 25 * metal
W 48 16 50 26
S 1 V metal P 103 P 104
P 38 58 * metal
P 48 58 * metal
W 37 57 49 59
S 1 H metal P 105 P 106
P 30 25 * poly
P 49 25 * poly
W 29 24 50 26
S 1 H poly P 107 P 108
P 30 25 * poly
P 30 31 * poly
W 29 24 31 32
S 1 V poly P 109 P 110
P 43 17 * metal
P 43 50 * metal
W 42 16 44 51 t_s
S 1 V metal P 111 P 112
P 43 50 * metal
P 47 50 * metal
W 42 49 48 51
S 1 H metal P 113 P 114
P 38 28 * metal
P 38 58 * metal
W 37 27 39 59
S 1 V metal P 115 P 116
P 37 28 * metal
P 38 28 * metal
W 36 27 39 29
S 1 H metal P 117 P 118
P 31 12 * poly
P 33 12 * poly
W 30 11 34 13
S 1 H poly P 119 P 120
P 28 54 * metal
P 29 54 * metal
W 27 53 30 55
S 1 H metal P 121 P 122
P 29 44 * metal
P 29 54 * metal
W 28 43 30 55
S 1 V metal P 123 P 124
P 18 12 * ntrans
P 18 21 * ntrans
W 17 11 19 22
S 1 V ntrans P 125 P 126
P 31 12 * ntrans
P 31 21 * ntrans
W 30 11 32 22
S 1 V ntrans P 127 P 128
P 2 23 * metal2
P 79 23 * metal2
W 0 21 81 25
S 2 H metal2 P 129 P 130
P 33 51 * poly
P 35 51 * poly
W 32 50 36 52
S 1 H poly P 131 P 132
P 35 48 * poly
P 35 51 * poly
W 34 47 36 52
S 1 V poly P 133 P 134
P 57 22 * metal
P 57 29 * metal
W 56 21 58 30
S 1 V metal P 135 P 136
P 48 54 * poly
P 50 54 * poly
W 47 53 51 55
S 1 H poly P 137 P 138
P 48 54 * poly
P 48 58 * poly
W 47 53 49 59
S 1 V poly P 139 P 140
P 47 62 * poly
P 55 62 * poly
W 46 61 56 63
S 1 H poly P 141 P 142
P 43 42 * poly
P 43 62 * poly
W 42 41 44 63
S 1 V poly P 143 P 144
P 43 62 * poly
P 47 62 * poly
W 42 61 48 63
S 1 H poly P 145 P 146
P 73 51 * poly
P 73 61 * poly
W 72 50 74 62
S 1 V poly P 147 P 148
P 49 25 * poly
P 53 25 * poly
W 48 24 54 26
S 1 H poly P 149 P 150
P 53 25 * poly
P 53 40 * poly
W 52 24 54 41
S 1 V poly P 151 P 152
P 53 40 * poly
P 55 40 * poly
W 52 39 56 41
S 1 H poly P 153 P 154
P 55 40 * poly
P 55 45 * poly
W 54 39 56 46
S 1 V poly P 155 P 156
P 55 61 * poly
P 55 62 * poly
W 54 60 56 63
S 1 V poly P 157 P 158
P 55 61 * poly
P 73 61 * poly
W 54 60 74 62
S 1 H poly P 159 P 160
P 43 14 * ndif
P 43 19 * ndif
W 41 12 45 21
S 2 V ndif P 161 P 162
P 36 14 * ndif
P 36 19 * ndif
W 31 9 41 24
S 5 V ndif P 163 P 164
P 8 48 * poly
P 17 48 * poly
W 7 47 18 49
S 1 H poly P 165 P 166
P 17 54 * metal
P 28 54 * metal
W 15 52 30 56
S 2 H metal P 167 P 168
P 15 41 * pdif
P 15 46 * pdif
W 12 38 18 49
S 3 V pdif P 169 P 170
P 15 44 * metal
P 15 54 * metal
W 14 43 16 55
S 1 V metal P 171 P 172
P 15 54 * metal
P 17 54 * metal
W 14 53 18 55
S 1 H metal P 173 P 174
P 21 14 * ndif
P 21 19 * ndif
W 18 11 24 22
S 3 V ndif P 175 P 176
P 18 21 * poly
P 18 39 * poly
W 17 20 19 40
S 1 V poly P 177 P 178
P 21 31 * metal
P 26 31 * metal
W 20 30 27 32
S 1 H metal P 179 P 180
P 21 31 * metal
P 21 44 * metal
W 20 30 22 45
S 1 V metal P 181 P 182
P 21 16 * metal
P 21 31 * metal
W 20 15 22 32
S 1 V metal P 183 P 184
P 15 10 * metal
P 15 16 * metal
W 14 9 16 17
S 1 V metal P 185 P 186
P 15 10 * metal
P 27 10 * metal
W 14 9 28 11
S 1 H metal P 187 P 188
P 70 40 * poly
P 70 42 * poly
W 69 39 71 43
S 1 V poly P 189 P 190
P 16 54 * ntie
P 28 54 * ntie
W 13 51 31 57
S 3 H ntie P 191 P 192
P 28 54 * ntie
P 28 59 * ntie
W 25 51 31 62
S 3 V ntie P 193 P 194
P 8 16 * metal
P 15 16 * metal
W 6 14 17 18
S 2 H metal P 195 P 196
P 8 16 * metal
P 8 28 * metal
W 6 14 10 30
S 2 V metal P 197 P 198
P 8 16 * ptie
P 8 28 * ptie
W 5 13 11 31
S 3 V ptie P 199 P 200
P 28 19 * ndif
P 28 14 * ndif
W 26 12 30 21
S 2 V ndif P 201 P 202
P 35 19 * ndif
P 35 14 * ndif
W 33 12 37 21
S 2 V ndif P 203 P 204
P 58 24 * ndif
P 58 11 * ndif
W 56 9 60 26
S 2 V ndif P 205 P 206
P 67 24 * ndif
P 67 11 * ndif
W 65 9 69 26
S 2 V ndif P 207 P 208
P 67 49 * pdif
P 67 44 * pdif
W 65 42 69 51
S 2 V pdif P 209 P 210
P 32 46 * pdif
P 32 41 * pdif
W 30 39 34 48
S 2 V pdif P 211 P 212
P 38 46 * pdif
P 38 41 * pdif
W 36 39 40 48
S 2 V pdif P 213 P 214
P 34 19 * ndif
P 34 14 * ndif
W 32 12 36 21
S 2 V ndif P 215 P 216
P 49 19 * ndif
P 49 14 * ndif
W 47 12 51 21
S 2 V ndif P 217 P 218
P 63 24 * ndif
P 63 11 * ndif
W 61 9 65 26
S 2 V ndif P 219 P 220
E
