#cell1 grmx4_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 3 4 29 61
B 3 4 29 61
F F
C vdd 10 4 metal 1 1 * * S vdd
C vdd 29 11 metal2 4 2 * * E vdd
C ck_13 29 17 metal2 2 3 * * E ck_13
C vdd 29 26 metal2 5 4 * * E vdd
C s3 29 34 metal2 2 5 * * E s3
C s2 29 41 metal2 2 6 * * E s2
C s1 29 46 metal2 2 7 * * E s1
C s0 29 55 metal2 2 8 * * E s0
C bl3_p 24 61 metal 1 9 * * N bl3_p
C bl1_p 12 61 metal 1 10 * * N bl1_p
C bl0_p 6 61 metal 1 11 * * N bl0_p
C s1 3 46 metal2 2 12 * * W s1
C s3 3 34 metal2 2 13 * * W s3
C s0 3 55 metal2 2 14 * * W s0
C s2 3 41 metal2 2 15 * * W s2
C bl2_p 18 61 metal 1 16 * * N bl2_p
C ck_13 3 17 metal2 2 17 * * W ck_13
C vdd 3 26 metal2 5 18 * * W vdd
C vdd 3 11 metal2 4 19 * * W vdd
C vss 29 4 metal 2 20 * * E vss
C vss 3 61 metal2 4 21 * * W vss
C vss 29 61 metal2 4 22 * * E vss
C o 23 4 metal 1 23 * * S o
I cbp1 29 51 0 "cbp" ly * *
I cbp2 29 46 0 "cbp" ly * *
I cbp3 29 56 0 "cbp" ly * *
I cpf1 14 54 0 "cpf" ly * *
I cbn1 6 15 0 "cbn" ly * *
I cdn1 9 37 0 "cdn" ly * *
I via1 14 45 0 "cvia" ly * *
I cpf2 19 41 0 "cpf" ly * *
I cdp1 7 21 0 "cdp" ly * *
I cdp2 17 22 0 "cdp" ly * *
I cdp3 23 22 0 "cdp" ly * *
I cdp4 12 10 0 "cdp" ly * *
I cdn2 29 11 0 "cdn" ly * *
I cdn3 23 11 0 "cdn" ly * *
I cpf3 17 12 0 "cpf" ly * *
I cpf4 12 18 0 "cpf" ly * *
I via2 7 26 0 "cvia" ly * *
I via3 23 26 0 "cvia" ly * *
I cpf5 20 17 0 "cpf" ly * *
I via4 24 17 0 "cvia" ly * *
I cdp5 12 4 0 "cdp" ly * *
I via5 7 11 0 "cvia" ly * *
I cdn4 6 52 0 "cdn" ly * *
I via6 15 50 0 "cvia" ly * *
I cdn5 21 46 0 "cdn" ly * *
I cdn6 24 39 0 "cdn" ly * *
I via7 15 40 0 "cvia" ly * *
I cpf6 24 31 0 "cpf" ly * *
I via8 20 33 0 "cvia" ly * *
I cdn7 15 35 0 "cdn" ly * *
I via9 29 61 0 "cvia" ly * *
I cpf7 10 42 0 "cpf" ly * *
P 16 13 * metal
P 16 35 * metal
W 15 12 17 36 bit_p
S 1 V metal P 1 P 2
P 12 8 * metal
P 12 9 * metal
W 10 6 14 11
S 2 V metal P 3 P 4
P 10 48 * metal
P 10 53 * metal
W 9 47 11 54
S 1 V metal P 5 P 6
P 23 21 * pdif
P 23 23 * pdif
W 21 19 25 25
S 2 V pdif P 7 P 8
P 29 4 * metal
P 29 21 * metal
W 27 2 31 23
S 2 V metal P 9 P 10
P 17 21 * pdif
P 17 22 * pdif
W 15 19 19 24
S 2 V pdif P 11 P 12
P 7 21 * pdif
P 7 22 * pdif
W 5 19 9 24
S 2 V pdif P 13 P 14
P 10 42 * metal
P 10 44 * metal
W 8 40 12 46
S 2 V metal P 15 P 16
P 29 35 * metal
P 29 61 * metal
W 27 33 31 63
S 2 V metal P 17 P 18
P 29 21 * metal
P 29 35 * metal
W 27 19 31 37
S 2 V metal P 19 P 20
P 12 10 * metal
P 12 18 * metal
W 10 8 14 20
S 2 V metal P 21 P 22
P 20 17 * metal
P 24 17 * metal
W 18 15 26 19
S 2 H metal P 23 P 24
P 15 7 * poly
P 17 7 * poly
W 14 6 18 8
S 1 H poly P 25 P 26
P 17 7 * poly
P 26 7 * poly
W 16 6 27 8
S 1 H poly P 27 P 28
P 6 4 * pdif
P 12 4 * pdif
W 3 1 15 7
S 3 H pdif P 29 P 30
P 7 4 * metal
P 12 4 * metal
W 5 2 14 6
S 2 H metal P 31 P 32
P 6 48 * metal
P 10 48 * metal
W 5 47 11 49
S 1 H metal P 33 P 34
P 6 38 * metal
P 9 38 * metal
W 5 37 10 39
S 1 H metal P 35 P 36
P 6 38 * metal
P 6 48 * metal
W 5 37 7 49
S 1 V metal P 37 P 38
P 12 32 * ntrans
P 12 41 * ntrans
W 11 31 13 42
S 1 V ntrans P 39 P 40
P 21 44 * ndif
P 21 49 * ndif
W 19 42 23 51
S 2 V ndif P 41 P 42
P 3 61 * metal2
P 29 61 * metal2
W -1 57 33 65
S 4 H metal2 P 43 P 44
P 23 23 * metal
P 23 25 * metal
W 21 21 25 27
S 2 V metal P 45 P 46
P 15 33 * ndif
P 21 33 * ndif
W 12 30 24 36
S 3 H ndif P 47 P 48
P 15 33 * ndif
P 15 46 * ndif
W 12 30 18 49
S 3 V ndif P 49 P 50
P 5 46 * ndif
P 15 46 * ndif
W 2 43 18 49
S 3 H ndif P 51 P 52
P 27 32 * poly
P 27 36 * poly
W 26 31 28 37
S 1 V poly P 53 P 54
P 18 36 * ntrans
P 27 36 * ntrans
W 17 35 28 37
S 1 H ntrans P 55 P 56
P 18 42 * ntrans
P 18 51 * ntrans
W 17 41 19 52
S 1 V ntrans P 57 P 58
P 3 11 * metal2
P 30 11 * metal2
W -1 7 34 15
S 4 H metal2 P 59 P 60
P 3 26 * metal2
P 30 26 * metal2
W -2 21 35 31
S 5 H metal2 P 61 P 62
P 3 17 * metal2
P 30 17 * metal2
W 1 15 32 19
S 2 H metal2 P 63 P 64
P 26 7 * ntrans
P 26 14 * ntrans
W 25 6 27 15
S 1 V ntrans P 65 P 66
P 4 7 * ptrans
P 15 7 * ptrans
W 3 6 16 8
S 1 H ptrans P 67 P 68
P 20 19 * ptrans
P 20 25 * ptrans
W 19 18 21 26
S 1 V ptrans P 69 P 70
P 12 19 * ptrans
P 12 24 * ptrans
W 8 15 16 28
S 4 V ptrans P 71 P 72
P 3 34 * metal2
P 30 34 * metal2
W 1 32 32 36
S 2 H metal2 P 73 P 74
P 3 49 * ntrans
P 12 49 * ntrans
W 2 48 13 50
S 1 H ntrans P 75 P 76
P 29 9 * ndif
P 29 12 * ndif
W 26 6 32 15
S 3 V ndif P 77 P 78
P 23 9 * ndif
P 23 12 * ndif
W 20 6 26 15
S 3 V ndif P 79 P 80
P 8 2 * nwell
P 8 21 * nwell
W -4 -10 20 33
S 12 V nwell P 81 P 82
P 8 21 * nwell
P 8 22 * nwell
W 4 17 12 26
S 4 V nwell P 83 P 84
P 8 22 * nwell
P 29 22 * nwell
W 4 18 33 26
S 4 H nwell P 85 P 86
P 2 22 * nwell
P 8 22 * nwell
W -2 18 12 26
S 4 H nwell P 87 P 88
P 24 32 * poly
P 27 32 * poly
W 23 31 28 33
S 1 H poly P 89 P 90
P 24 31 * poly
P 24 32 * poly
W 23 30 25 33
S 1 V poly P 91 P 92
P 20 31 * metal
P 20 33 * metal
W 18 29 22 35
S 2 V metal P 93 P 94
P 20 31 * metal
P 24 31 * metal
W 18 29 26 33
S 2 H metal P 95 P 96
P 18 41 * poly
P 18 42 * poly
W 17 40 19 43
S 1 V poly P 97 P 98
P 18 41 * poly
P 19 41 * poly
W 17 40 20 42
S 1 H poly P 99 P 100
P 19 40 * metal
P 19 41 * metal
W 17 38 21 43
S 2 V metal P 101 P 102
P 15 40 * metal
P 19 40 * metal
W 13 38 21 42
S 2 H metal P 103 P 104
P 22 39 * ndif
P 23 39 * ndif
W 20 37 25 41
S 2 H ndif P 105 P 106
P 3 58 * ptie
P 29 58 * ptie
W 1 56 31 60
S 2 H ptie P 107 P 108
P 29 46 * ptie
P 29 58 * ptie
W 26 43 32 61
S 3 V ptie P 109 P 110
P 7 15 * metal
P 7 26 * metal
W 5 13 9 28
S 2 V metal P 111 P 112
P 7 4 * metal
P 7 15 * metal
W 5 2 9 17
S 2 V metal P 113 P 114
P 6 15 * metal
P 7 15 * metal
W 4 13 9 17
S 2 H metal P 115 P 116
P 24 56 * metal
P 24 61 * metal
W 23 55 25 62 bl3_p
S 1 V metal P 117 P 118
P 24 56 * metal
P 25 56 * metal
W 23 55 26 57 bl3_p
S 1 H metal P 119 P 120
P 24 39 * metal
P 25 39 * metal
W 22 37 27 41 bl3_p
S 2 H metal P 121 P 122
P 25 39 * metal
P 25 56 * metal
W 24 38 26 57 bl3_p
S 1 V metal P 123 P 124
P 10 60 * metal
P 12 60 * metal
W 9 59 13 61 bl1_p
S 1 H metal P 125 P 126
P 10 53 * metal
P 10 60 * metal
W 9 52 11 61 bl1_p
S 1 V metal P 127 P 128
P 12 60 * metal
P 12 61 * metal
W 11 59 13 62 bl1_p
S 1 V metal P 129 P 130
P 18 60 * metal
P 20 60 * metal
W 17 59 21 61 bl2_p
S 1 H metal P 131 P 132
P 20 46 * metal
P 20 60 * metal
W 19 45 21 61 bl2_p
S 1 V metal P 133 P 134
P 20 46 * metal
P 21 46 * metal
W 19 45 22 47 bl2_p
S 1 H metal P 135 P 136
P 18 60 * metal
P 18 61 * metal
W 17 59 19 62 bl2_p
S 1 V metal P 137 P 138
P 3 41 * metal2
P 7 41 * metal2
W 1 39 9 43
S 2 H metal2 P 139 P 140
P 7 40 * metal2
P 7 41 * metal2
W 5 38 9 43
S 2 V metal2 P 141 P 142
P 7 40 * metal2
P 25 40 * metal2
W 5 38 27 42
S 2 H metal2 P 143 P 144
P 25 40 * metal2
P 25 41 * metal2
W 23 38 27 43
S 2 V metal2 P 145 P 146
P 25 41 * metal2
P 30 41 * metal2
W 23 39 32 43
S 2 H metal2 P 147 P 148
P 25 45 * metal2
P 25 46 * metal2
W 23 43 27 48 s1
S 2 V metal2 P 149 P 150
P 8 45 * metal2
P 25 45 * metal2
W 6 43 27 47 s1
S 2 H metal2 P 151 P 152
P 8 45 * metal2
P 8 46 * metal2
W 6 43 10 48 s1
S 2 V metal2 P 153 P 154
P 3 46 * metal2
P 8 46 * metal2
W 1 44 10 48 s1
S 2 H metal2 P 155 P 156
P 25 46 * metal2
P 29 46 * metal2
W 23 44 31 48 s1
S 2 H metal2 P 157 P 158
P 14 44 * metal
P 14 45 * metal
W 13 43 15 46
S 1 V metal P 159 P 160
P 10 44 * metal
P 14 44 * metal
W 9 43 15 45
S 1 H metal P 161 P 162
P 14 50 * metal
P 14 54 * metal
W 12 48 16 56
S 2 V metal P 163 P 164
P 14 50 * metal
P 15 50 * metal
W 12 48 17 52
S 2 H metal P 165 P 166
P 3 55 * metal2
P 15 55 * metal2
W 1 53 17 57
S 2 H metal2 P 167 P 168
P 15 55 * metal2
P 30 55 * metal2
W 13 53 32 57
S 2 H metal2 P 169 P 170
P 15 51 * metal2
P 15 55 * metal2
W 12 48 18 58
S 3 V metal2 P 171 P 172
P 12 53 * poly
P 14 53 * poly
W 11 52 15 54
S 1 H poly P 173 P 174
P 12 49 * poly
P 12 53 * poly
W 11 48 13 54
S 1 V poly P 175 P 176
P 14 53 * poly
P 14 54 * poly
W 13 52 15 55
S 1 V poly P 177 P 178
P 6 52 * metal
P 6 56 * metal
W 5 51 7 57 bl0_p
S 1 V metal P 179 P 180
P 6 56 * metal
P 6 61 * metal
W 5 55 7 62 bl0_p
S 1 V metal P 181 P 182
P 5 52 * ndif
P 10 52 * ndif
W 3 50 12 54
S 2 H ndif P 183 P 184
P 9 34 * ndif
P 9 39 * ndif
W 7 32 11 41
S 2 V ndif P 185 P 186
P 12 8 * metal
P 23 8 * metal
W 11 7 24 9
S 1 H metal P 187 P 188
P 23 4 * metal
P 23 8 * metal
W 22 3 24 9
S 1 V metal P 189 P 190
P 23 8 * metal
P 23 10 * metal
W 21 6 25 12
S 2 V metal P 191 P 192
P 10 41 * poly
P 12 41 * poly
W 9 40 13 42
S 1 H poly P 193 P 194
P 10 41 * poly
P 10 42 * poly
W 9 40 11 43
S 1 V poly P 195 P 196
P 20 17 * poly
P 20 19 * poly
W 19 16 21 20
S 1 V poly P 197 P 198
P 17 7 * poly
P 17 12 * poly
W 16 6 18 13
S 1 V poly P 199 P 200
P 16 13 * metal
P 17 13 * metal
W 15 12 18 14
S 1 H metal P 201 P 202
P 17 12 * metal
P 17 13 * metal
W 16 11 18 14
S 1 V metal P 203 P 204
E
