#cell1 amcsapintop_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 75 65
B 2 5 75 65
F F
C vss 2 5 metal2 2 1 * * W vss
C vss 75 5 metal2 2 2 * * E vss
C vdd 2 65 metal2 2 3 * * W vdd
C vdd 75 65 metal2 2 4 * * E vdd
C cout 75 23 metal2 2 5 * * E cout
C ai 2 38 metal2 2 6 * * W ai
C ai 75 38 metal2 2 7 * * E ai
C mi 73 65 metal 1 8 * * N mi
C mi 73 5 metal 1 9 * * S mi
C sum 52 5 metal 1 10 * * S sum
C cin 26 65 metal 1 11 * * N cin
C b 2 11 metal 1 12 * * W b
C cin 26 5 metal 1 13 * * S cin
C anext 22 65 metal 1 14 * * N anext
C anext 22 5 metal 1 15 * * S anext
I via1 4 48 0 "cvia" ly * *
I cpf1 4 62 0 "cpf" ly * *
I cdp1 10 55 0 "cdp" ly * *
I cdp2 14 65 0 "cdp" ly * *
I via2 9 65 0 "cvia" ly * *
I cpf2 17 35 0 "cpf" ly * *
I cdn1 10 28 0 "cdn" ly * *
I cdn2 4 28 0 "cdn" ly * *
I cpf3 4 20 0 "cpf" ly * *
I cpf4 13 7 0 "cpf" ly * *
I cdn3 8 11 0 "cdn" ly * *
I cdn4 10 17 0 "cdn" ly * *
I cdp3 54 49 0 "cdp" ly * *
I cdp4 60 43 0 "cdp" ly * *
I via3 69 23 0 "cvia" ly * *
I cdp5 60 52 0 "cdp" ly * *
I cdp6 54 43 0 "cdp" ly * *
I cdp7 41 49 0 "cdp" ly * *
I cdp8 41 43 0 "cdp" ly * *
I cdp9 47 43 0 "cdp" ly * *
I cdn5 60 18 0 "cdn" ly * *
I cdn6 54 17 0 "cdn" ly * *
I via4 62 5 0 "cvia" ly * *
I via5 42 65 0 "cvia" ly * *
I cpf5 57 33 0 "cpf" ly * *
I cdn7 41 17 0 "cdn" ly * *
I cdn8 18 29 0 "cdn" ly * *
I cdn9 18 23 0 "cdn" ly * *
I cdn10 18 17 0 "cdn" ly * *
I cdp10 17 55 0 "cdp" ly * *
I cdn11 30 16 0 "cdn" ly * *
I cdn12 30 23 0 "cdn" ly * *
I cdn13 30 29 0 "cdn" ly * *
I cdp11 34 48 0 "cdp" ly * *
I cdp12 34 43 0 "cdp" ly * *
I via6 18 5 0 "cvia" ly * *
I cbp1 54 26 0 "cbp" ly * *
I cbp2 68 11 0 "cbp" ly * *
I cbn1 32 55 0 "cbn" ly * *
I via7 22 48 0 "cvia" ly * *
P 4 48 * metal2
P 22 48 * metal2
W 2 46 24 50
S 2 H metal2 P 1 P 2
P 4 20 * metal
P 4 62 * metal
W 3 19 5 63 a
S 1 V metal P 3 P 4
P 4 62 * poly
P 7 62 * poly
W 3 61 8 63
S 1 H poly P 5 P 6
P 7 62 * ptrans
P 20 62 * ptrans
W 6 61 21 63
S 1 H ptrans P 7 P 8
P 7 58 * ptrans
P 20 58 * ptrans
W 6 57 21 59
S 1 H ptrans P 9 P 10
P 7 32 * poly
P 7 58 * poly
W 6 31 8 59
S 1 V poly P 11 P 12
P 9 55 * pdif
P 18 55 * pdif
W 6 52 21 58
S 3 H pdif P 13 P 14
P 9 65 * pdif
P 18 65 * pdif
W 6 62 21 68
S 3 H pdif P 15 P 16
P 24 13 * ntrans
P 24 32 * ntrans
W 23 12 25 33
S 1 V ntrans P 17 P 18
P 18 5 * metal
P 18 29 * metal
W 17 4 19 30
S 1 V metal P 19 P 20
P 22 5 * metal
P 22 65 * metal
W 21 4 23 66
S 1 V metal P 21 P 22
P 7 24 * ntrans
P 7 32 * ntrans
W 6 23 8 33
S 1 V ntrans P 23 P 24
P 6 14 * ntrans
P 14 14 * ntrans
W 5 13 15 15
S 1 H ntrans P 25 P 26
P 10 26 * ndif
P 10 30 * ndif
W 7 23 13 33
S 3 V ndif P 27 P 28
P 10 17 * metal
P 10 28 * metal
W 9 16 11 29 s1_s
S 1 V metal P 29 P 30
P 2 11 * metal
P 8 11 * metal
W 1 10 9 12 b
S 1 H metal P 31 P 32
P 4 26 * ndif
P 4 30 * ndif
W 1 23 7 33
S 3 V ndif P 33 P 34
P 6 14 * poly
P 6 20 * poly
W 5 13 7 21
S 1 V poly P 35 P 36
P 9 65 * metal
P 14 65 * metal
W 8 64 15 66
S 1 H metal P 37 P 38
P 4 20 * poly
P 6 20 * poly
W 3 19 7 21
S 1 H poly P 39 P 40
P 8 17 * ndif
P 12 17 * ndif
W 5 14 15 20
S 3 H ndif P 41 P 42
P 8 11 * ndif
P 12 11 * ndif
W 5 8 15 14
S 3 H ndif P 43 P 44
P 26 5 * metal
P 26 65 * metal
W 25 4 27 66
S 1 V metal P 45 P 46
P 57 40 * ptrans
P 57 57 * ptrans
W 56 39 58 58
S 1 V ptrans P 47 P 48
P 2 5 * metal2
P 75 5 * metal2
W -6 -3 83 13
S 8 H metal2 P 49 P 50
P 60 42 * pdif
P 60 55 * pdif
W 57 39 63 58
S 3 V pdif P 51 P 52
P 54 42 * pdif
P 54 55 * pdif
W 51 39 57 58
S 3 V pdif P 53 P 54
P 73 5 * metal
P 73 65 * metal
W 72 4 74 66
S 1 V metal P 55 P 56
P 69 23 * metal2
P 75 23 * metal2
W 67 21 77 25
S 2 H metal2 P 57 P 58
P 4 7 * metal
P 4 11 * metal
W 3 6 5 12
S 1 V metal P 59 P 60
P 14 7 * poly
P 14 10 * poly
W 13 6 15 11
S 1 V poly P 61 P 62
P 4 7 * metal
P 13 7 * metal
W 3 6 14 8
S 1 H metal P 63 P 64
P 13 7 * poly
P 14 7 * poly
W 12 6 15 8
S 1 H poly P 65 P 66
P 52 5 * metal
P 52 7 * metal
W 51 4 53 8
S 1 V metal P 67 P 68
P 50 7 * metal
P 52 7 * metal
W 49 6 53 8
S 1 H metal P 69 P 70
P 2 65 * metal2
P 75 65 * metal2
W -4 59 81 71
S 6 H metal2 P 71 P 72
P 2 38 * metal2
P 75 38 * metal2
W 0 36 77 40
S 2 H metal2 P 73 P 74
P 10 35 * metal
P 17 35 * metal
W 9 34 18 36
S 1 H metal P 75 P 76
P 9 28 * metal
P 10 28 * metal
W 8 27 11 29
S 1 H metal P 77 P 78
P 9 28 * metal
P 9 35 * metal
W 8 27 10 36
S 1 V metal P 79 P 80
P 9 35 * metal
P 10 35 * metal
W 8 34 11 36
S 1 H metal P 81 P 82
P 2 52 * nwell
P 75 52 * nwell
W -24 26 101 78
S 26 H nwell P 83 P 84
P 50 40 * ptrans
P 50 53 * ptrans
W 49 39 51 54
S 1 V ptrans P 85 P 86
P 44 40 * ptrans
P 44 53 * ptrans
W 43 39 45 54
S 1 V ptrans P 87 P 88
P 47 42 * pdif
P 47 51 * pdif
W 45 40 49 53
S 2 V pdif P 89 P 90
P 41 42 * pdif
P 41 51 * pdif
W 38 39 44 54
S 3 V pdif P 91 P 92
P 20 62 * poly
P 50 62 * poly
W 19 61 51 63
S 1 H poly P 93 P 94
P 50 53 * poly
P 50 62 * poly
W 49 52 51 63
S 1 V poly P 95 P 96
P 20 58 * poly
P 44 58 * poly
W 19 57 45 59
S 1 H poly P 97 P 98
P 44 53 * poly
P 44 58 * poly
W 43 52 45 59
S 1 V poly P 99 P 100
P 44 13 * ntrans
P 44 21 * ntrans
W 43 12 45 22
S 1 V ntrans P 101 P 102
P 50 13 * ntrans
P 50 21 * ntrans
W 49 12 51 22
S 1 V ntrans P 103 P 104
P 47 15 * ndif
P 47 19 * ndif
W 45 13 49 21
S 2 V ndif P 105 P 106
P 44 21 * poly
P 44 40 * poly
W 43 20 45 41
S 1 V poly P 107 P 108
P 50 21 * poly
P 50 40 * poly
W 49 20 51 41
S 1 V poly P 109 P 110
P 57 23 * poly
P 57 40 * poly
W 56 22 58 41
S 1 V poly P 111 P 112
P 57 13 * ntrans
P 57 23 * ntrans
W 56 12 58 24
S 1 V ntrans P 113 P 114
P 41 15 * ndif
P 41 19 * ndif
W 38 12 44 22
S 3 V ndif P 115 P 116
P 60 15 * ndif
P 60 21 * ndif
W 57 12 63 24
S 3 V ndif P 117 P 118
P 14 10 * poly
P 44 10 * poly
W 13 9 45 11
S 1 H poly P 119 P 120
P 64 18 * metal
P 64 43 * metal
W 63 17 65 44 cout
S 1 V metal P 121 P 122
P 64 23 * metal
P 69 23 * metal
W 63 22 70 24
S 1 H metal P 123 P 124
P 54 15 * ndif
P 54 21 * ndif
W 52 13 56 23
S 2 V ndif P 125 P 126
P 53 15 * ndif
P 53 19 * ndif
W 51 13 55 21
S 2 V ndif P 127 P 128
P 54 11 * metal
P 54 17 * metal
W 53 10 55 18
S 1 V metal P 129 P 130
P 54 11 * metal
P 62 11 * metal
W 53 10 63 12
S 1 H metal P 131 P 132
P 62 5 * metal
P 62 11 * metal
W 61 4 63 12
S 1 V metal P 133 P 134
P 41 49 * metal
P 54 49 * metal
W 40 48 55 50
S 1 H metal P 135 P 136
P 54 43 * metal
P 54 49 * metal
W 53 42 55 50
S 1 V metal P 137 P 138
P 47 33 * metal
P 47 43 * metal
W 46 32 48 44
S 1 V metal P 139 P 140
P 47 33 * metal
P 57 33 * metal
W 46 32 58 34
S 1 H metal P 141 P 142
P 60 43 * metal
P 60 52 * metal
W 59 42 61 53
S 1 V metal P 143 P 144
P 60 43 * metal
P 64 43 * metal
W 59 42 65 44
S 1 H metal P 145 P 146
P 60 18 * metal
P 64 18 * metal
W 59 17 65 19
S 1 H metal P 147 P 148
P 41 17 * metal
P 41 33 * metal
W 40 16 42 34
S 1 V metal P 149 P 150
P 41 33 * metal
P 47 33 * metal
W 40 32 48 34
S 1 H metal P 151 P 152
P 44 10 * poly
P 44 13 * poly
W 43 9 45 14
S 1 V poly P 153 P 154
P 19 15 * ndif
P 19 30 * ndif
W 12 8 26 37
S 7 V ndif P 155 P 156
P 17 35 * poly
P 24 35 * poly
W 16 34 25 36
S 1 H poly P 157 P 158
P 24 32 * poly
P 24 35 * poly
W 23 31 25 36
S 1 V poly P 159 P 160
P 37 40 * ptrans
P 37 51 * ptrans
W 36 39 38 52
S 1 V ptrans P 161 P 162
P 10 55 * metal
P 17 55 * metal
W 9 54 18 56
S 1 H metal P 163 P 164
P 17 35 * metal
P 17 55 * metal
W 16 34 18 56
S 1 V metal P 165 P 166
P 29 15 * ndif
P 29 30 * ndif
W 22 8 36 37
S 7 V ndif P 167 P 168
P 34 42 * pdif
P 34 49 * pdif
W 31 39 37 52
S 3 V pdif P 169 P 170
P 34 29 * metal
P 34 48 * metal
W 33 28 35 49
S 1 V metal P 171 P 172
P 30 16 * metal
P 30 29 * metal
W 29 15 31 30
S 1 V metal P 173 P 174
P 30 29 * metal
P 34 29 * metal
W 29 28 35 30
S 1 H metal P 175 P 176
P 30 12 * metal
P 30 16 * metal
W 29 11 31 17
S 1 V metal P 177 P 178
P 37 35 * poly
P 37 40 * poly
W 36 34 38 41
S 1 V poly P 179 P 180
P 24 35 * poly
P 37 35 * poly
W 23 34 38 36
S 1 H poly P 181 P 182
P 42 56 * metal
P 42 65 * metal
W 41 55 43 66
S 1 V metal P 183 P 184
P 41 56 * metal
P 42 56 * metal
W 40 55 43 57
S 1 H metal P 185 P 186
P 41 43 * metal
P 41 56 * metal
W 40 42 42 57
S 1 V metal P 187 P 188
P 54 17 * metal
P 54 26 * metal
W 53 16 55 27
S 1 V metal P 189 P 190
P 62 11 * metal
P 68 11 * metal
W 61 10 69 12
S 1 H metal P 191 P 192
P 7 66 * nwell
P 20 66 * nwell
W 3 62 24 70
S 4 H nwell P 193 P 194
P 32 55 * metal
P 41 55 * metal
W 31 54 42 56
S 1 H metal P 195 P 196
P 50 7 * metal
P 50 12 * metal
W 49 6 51 13
S 1 V metal P 197 P 198
P 30 12 * metal
P 50 12 * metal
W 29 11 51 13 sum
S 1 H metal P 199 P 200
P 40 49 * pdif
P 40 42 * pdif
W 38 40 42 51
S 2 V pdif P 201 P 202
P 53 51 * pdif
P 53 42 * pdif
W 51 40 55 53
S 2 V pdif P 203 P 204
E
