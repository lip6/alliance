#cell1 t1_fp idps_symb compose *
# 20-Nov-97 11:16 20-Nov-97 11:16 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 53 65
B 5 5 53 65
F F
C en 34 5 metal 1 1 * * S en
C vdd 5 65 metal2 6 2 * * W vdd
C vss 5 5 metal2 8 3 * * W vss
C vss 53 5 metal2 8 4 * * E vss
C enb 7 65 metal 1 5 * * N enb
C enb 7 5 metal 1 6 * * S enb
C en 34 65 metal 1 7 * * N en
C vdd 53 65 metal2 6 8 * * E vdd
I cbn1 20 65 0 "cbn" ly * *
I via1 23 65 0 "cvia" ly * *
I via2 17 65 0 "cvia" ly * *
I cbn2 26 65 0 "cbn" ly * *
I via3 17 5 0 "cvia" ly * *
I via4 29 65 0 "cvia" ly * *
I via5 23 5 0 "cvia" ly * *
I via6 38 65 0 "cvia" ly * *
I via7 29 5 0 "cvia" ly * *
I cbn3 14 65 0 "cbn" ly * *
I via8 11 65 0 "cvia" ly * *
I cbp1 14 5 0 "cbp" ly * *
I cpf1 31 10 0 "cpf" ly * *
I cdn1 23 11 0 "cdn" ly * *
I cdp1 11 43 0 "cdp" ly * *
I cdp2 17 53 0 "cdp" ly * *
I cdp3 23 57 0 "cdp" ly * *
I cdp4 29 53 0 "cdp" ly * *
I cdn2 29 15 0 "cdn" ly * *
I cpf2 30 39 0 "cpf" ly * *
I cbn4 41 65 0 "cbn" ly * *
I cbp2 47 5 0 "cbp" ly * *
I cdp5 38 58 0 "cdp" ly * *
I cdp6 50 53 0 "cdp" ly * *
I via9 44 65 0 "cvia" ly * *
I cdn3 11 13 0 "cdn" ly * *
I cdn4 23 17 0 "cdn" ly * *
I cdn5 44 15 0 "cdn" ly * *
I cdn6 50 18 0 "cdn" ly * *
I via10 44 5 0 "cvia" ly * *
I cpf3 18 28 0 "cpf" ly * *
I cpf4 34 33 0 "cpf" ly * *
I cpf5 8 31 0 "cpf" ly * *
I cbp3 41 5 0 "cbp" ly * *
I cdn7 17 13 0 "cdn" ly * *
I cdp7 44 43 0 "cdp" ly * *
I cdn8 44 21 0 "cdn" ly * *
I cdp8 11 48 0 "cdp" ly * *
I cdp9 29 58 0 "cdp" ly * *
I cdp10 17 58 0 "cdp" ly * *
I cdp11 38 53 0 "cdp" ly * *
I cdp12 44 58 0 "cdp" ly * *
I cdp13 38 48 0 "cdp" ly * *
I cbp4 20 5 0 "cbp" ly * *
I cbp5 26 5 0 "cbp" ly * *
I cdp14 11 58 0 "cdp" ly * *
I cdn9 38 18 0 "cdn" ly * *
I cdp15 50 58 0 "cdp" ly * *
I cdp16 11 53 0 "cdp" ly * *
I cdp17 38 43 0 "cdp" ly * *
I cbn5 47 65 0 "cbn" ly * *
I via11 50 65 0 "cvia" ly * *
I via12 11 5 0 "cvia" ly * *
I via13 50 5 0 "cvia" ly * *
I cdp18 50 48 0 "cdp" ly * *
I i_5 17 33 0 "ref_con" ly * *
I i_7 17 23 0 "ref_con" ly * *
I i_4 17 38 0 "ref_con" ly * *
I i_3 17 43 0 "ref_con" ly * *
I i_2 17 48 0 "ref_con" ly * *
I o_5 50 33 0 "ref_con" ly * *
I o_6 50 28 0 "ref_con" ly * *
I o_7 50 23 0 "ref_con" ly * *
I o_9 50 13 0 "ref_con" ly * *
I o_4 50 38 0 "ref_con" ly * *
I o_3 50 43 0 "ref_con" ly * *
I o_2 44 48 0 "ref_con" ly * *
I o_1 44 53 0 "ref_con" ly * *
I nwell_28 32 53 0 "ref_ref" ly * *
P 34 5 * metal
P 35 5 * metal
W 33 4 36 6
S 1 H metal P 1 P 2
P 11 43 * metal
P 12 43 * metal
W 10 42 13 44
S 1 H metal P 3 P 4
P 11 43 * metal
P 11 43 * metal
W 10 42 12 44
S 1 H metal P 5 P 6
P 11 43 * metal
P 11 58 * metal
W 10 42 12 59
S 1 V metal P 7 P 8
P 11 13 * metal
P 12 13 * metal
W 10 12 13 14
S 1 H metal P 9 P 10
P 23 10 * metal
P 31 10 * metal
W 22 9 32 11
S 1 H metal P 11 P 12
P 23 10 * metal
P 23 17 * metal
W 22 9 24 18
S 1 V metal P 13 P 14
P 12 17 * metal
P 23 17 * metal
W 11 16 24 18
S 1 H metal P 15 P 16
P 12 13 * metal
P 12 17 * metal
W 11 12 13 18
S 1 V metal P 17 P 18
P 12 17 * metal
P 12 43 * metal
W 11 16 13 44
S 1 V metal P 19 P 20
P 7 31 * metal
P 8 31 * metal
W 6 30 9 32 enb
S 1 H metal P 21 P 22
P 7 5 * metal
P 7 31 * metal
W 6 4 8 32 enb
S 1 V metal P 23 P 24
P 7 31 * metal
P 7 65 * metal
W 6 30 8 66 enb
S 1 V metal P 25 P 26
P 8 31 * poly
P 14 31 * poly
W 7 30 15 32
S 1 H poly P 27 P 28
P 14 17 * poly
P 14 31 * poly
W 13 16 15 32
S 1 V poly P 29 P 30
P 14 31 * poly
P 14 39 * poly
W 13 30 15 40
S 1 V poly P 31 P 32
P 38 18 * metal
P 38 22 * metal
W 37 17 39 23
S 1 V metal P 33 P 34
P 50 41 * pdif
P 50 60 * pdif
W 47 38 53 63
S 3 V pdif P 35 P 36
P 11 41 * pdif
P 11 60 * pdif
W 8 38 14 63
S 3 V pdif P 37 P 38
P 38 14 * ndif
P 38 23 * ndif
W 35 11 41 26
S 3 V ndif P 39 P 40
P 29 13 * ndif
P 29 19 * ndif
W 26 10 32 22
S 3 V ndif P 41 P 42
P 47 10 * poly
P 47 12 * poly
W 46 9 48 13
S 1 V poly P 43 P 44
P 41 10 * poly
P 47 10 * poly
W 40 9 48 11
S 1 H poly P 45 P 46
P 29 51 * pdif
P 29 60 * pdif
W 26 48 32 63
S 3 V pdif P 47 P 48
P 17 53 * metal
P 17 58 * metal
W 16 52 18 59
S 1 V metal P 49 P 50
P 29 53 * metal
P 29 58 * metal
W 28 52 30 59
S 1 V metal P 51 P 52
P 38 41 * pdif
P 38 60 * pdif
W 35 38 41 63
S 3 V pdif P 53 P 54
P 44 42 * metal
P 44 58 * metal
W 43 41 45 59
S 1 V metal P 55 P 56
P 44 16 * metal
P 44 21 * metal
W 42 14 46 23
S 2 V metal P 57 P 58
P 44 28 * metal
P 44 42 * metal
W 43 27 45 43 o
S 1 V metal P 59 P 60
P 50 28 * metal
P 50 43 * metal
W 49 27 51 44 o
S 1 V metal P 61 P 62
P 50 13 * metal
P 50 28 * metal
W 49 12 51 29 o
S 1 V metal P 63 P 64
P 44 28 * metal
P 50 28 * metal
W 43 27 51 29 o
S 1 H metal P 65 P 66
P 38 28 * metal
P 44 28 * metal
W 37 27 45 29 o
S 1 H metal P 67 P 68
P 38 22 * metal
P 38 28 * metal
W 37 21 39 29 o
S 1 V metal P 69 P 70
P 30 39 * poly
P 47 39 * poly
W 29 38 48 40
S 1 H poly P 71 P 72
P 5 65 * metal2
P 53 65 * metal2
W -1 59 59 71
S 6 H metal2 P 73 P 74
P 17 5 * metal
P 17 13 * metal
W 15 3 19 15
S 2 V metal P 75 P 76
P 11 5 * metal
P 17 5 * metal
W 9 3 19 7
S 2 H metal P 77 P 78
P 17 5 * metal
P 29 5 * metal
W 15 3 31 7
S 2 H metal P 79 P 80
P 50 14 * ndif
P 50 23 * ndif
W 47 11 53 26
S 3 V ndif P 81 P 82
P 41 5 * metal
P 44 5 * metal
W 39 3 46 7
S 2 H metal P 83 P 84
P 44 5 * metal
P 44 15 * metal
W 42 3 46 17
S 2 V metal P 85 P 86
P 44 5 * metal
P 50 5 * metal
W 42 3 52 7
S 2 H metal P 87 P 88
P 50 48 * metal
P 50 65 * metal
W 48 46 52 67
S 2 V metal P 89 P 90
P 38 65 * metal
P 50 65 * metal
W 36 63 52 67
S 2 H metal P 91 P 92
P 38 43 * metal
P 38 65 * metal
W 36 41 40 67
S 2 V metal P 93 P 94
P 26 33 * poly
P 34 33 * poly
W 25 32 35 34
S 1 H poly P 95 P 96
P 26 21 * poly
P 26 33 * poly
W 25 20 27 34
S 1 V poly P 97 P 98
P 26 33 * poly
P 26 49 * poly
W 25 32 27 50
S 1 V poly P 99 P 100
P 5 13 * allowM2
P 53 13 * allowM2
W 3 11 55 15
S 2 H allowM2 P 101 P 102
P 31 10 * poly
P 41 10 * poly
W 30 9 42 11
S 1 H poly P 103 P 104
P 41 10 * poly
P 41 12 * poly
W 40 9 42 13
S 1 V poly P 105 P 106
P 29 15 * metal
P 29 53 * metal
W 28 14 30 54
S 1 V metal P 107 P 108
P 17 53 * metal
P 29 53 * metal
W 16 52 30 54
S 1 H metal P 109 P 110
P 17 28 * metal
P 18 28 * metal
W 16 27 19 29 i
S 1 H metal P 111 P 112
P 17 23 * metal
P 17 28 * metal
W 16 22 18 29 i
S 1 V metal P 113 P 114
P 17 28 * metal
P 17 48 * metal
W 16 27 18 49 i
S 1 V metal P 115 P 116
P 18 28 * poly
P 20 28 * poly
W 17 27 21 29
S 1 H poly P 117 P 118
P 20 21 * poly
P 20 28 * poly
W 19 20 21 29
S 1 V poly P 119 P 120
P 20 28 * poly
P 20 39 * poly
W 19 27 21 40
S 1 V poly P 121 P 122
P 23 57 * metal
P 23 65 * metal
W 21 55 25 67
S 2 V metal P 123 P 124
P 23 65 * metal
P 29 65 * metal
W 21 63 31 67
S 2 H metal P 125 P 126
P 11 65 * metal
P 23 65 * metal
W 9 63 25 67
S 2 H metal P 127 P 128
P 44 14 * ndif
P 44 23 * ndif
W 41 11 47 26
S 3 V ndif P 129 P 130
P 47 12 * ntrans
P 47 25 * ntrans
W 46 11 48 26
S 1 V ntrans P 131 P 132
P 44 41 * pdif
P 44 60 * pdif
W 41 38 47 63
S 3 V pdif P 133 P 134
P 47 39 * ptrans
P 47 62 * ptrans
W 46 38 48 63
S 1 V ptrans P 135 P 136
P 41 39 * ptrans
P 41 62 * ptrans
W 40 38 42 63
S 1 V ptrans P 137 P 138
P 41 12 * ntrans
P 41 25 * ntrans
W 40 11 42 26
S 1 V ntrans P 139 P 140
P 17 41 * pdif
P 17 60 * pdif
W 14 38 20 63
S 3 V pdif P 141 P 142
P 23 51 * pdif
P 23 60 * pdif
W 20 48 26 63
S 3 V pdif P 143 P 144
P 26 49 * ptrans
P 26 62 * ptrans
W 25 48 27 63
S 1 V ptrans P 145 P 146
P 20 39 * ptrans
P 20 62 * ptrans
W 19 38 21 63
S 1 V ptrans P 147 P 148
P 14 39 * ptrans
P 14 62 * ptrans
W 13 38 15 63
S 1 V ptrans P 149 P 150
P 5 53 * nwell
P 53 53 * nwell
W -23 25 81 81 nwell
S 28 H nwell P 151 P 152
P 14 8 * ntrans
P 14 17 * ntrans
W 13 7 15 18
S 1 V ntrans P 153 P 154
P 11 10 * ndif
P 11 15 * ndif
W 8 7 14 18
S 3 V ndif P 155 P 156
P 17 10 * ndif
P 17 15 * ndif
W 14 7 20 18
S 3 V ndif P 157 P 158
P 23 10 * ndif
P 23 19 * ndif
W 20 7 26 22
S 3 V ndif P 159 P 160
P 5 18 * allowM2
P 53 18 * allowM2
W 3 16 55 20
S 2 H allowM2 P 161 P 162
P 5 58 * allowM2
P 53 58 * allowM2
W 3 56 55 60
S 2 H allowM2 P 163 P 164
P 5 53 * allowM2
P 53 53 * allowM2
W 3 51 55 55
S 2 H allowM2 P 165 P 166
P 5 48 * allowM2
P 53 48 * allowM2
W 3 46 55 50
S 2 H allowM2 P 167 P 168
P 5 43 * allowM2
P 53 43 * allowM2
W 3 41 55 45
S 2 H allowM2 P 169 P 170
P 5 38 * allowM2
P 53 38 * allowM2
W 3 36 55 40
S 2 H allowM2 P 171 P 172
P 5 33 * allowM2
P 53 33 * allowM2
W 3 31 55 35
S 2 H allowM2 P 173 P 174
P 5 28 * allowM2
P 53 28 * allowM2
W 3 26 55 30
S 2 H allowM2 P 175 P 176
P 5 23 * allowM2
P 53 23 * allowM2
W 3 21 55 25
S 2 H allowM2 P 177 P 178
P 5 5 * metal2
P 53 5 * metal2
W -3 -3 61 13
S 8 H metal2 P 179 P 180
P 26 8 * ntrans
P 26 21 * ntrans
W 25 7 27 22
S 1 V ntrans P 181 P 182
P 20 8 * ntrans
P 20 21 * ntrans
W 19 7 21 22
S 1 V ntrans P 183 P 184
P 34 14 * metal
P 34 65 * metal
W 33 13 35 66 en
S 1 V metal P 185 P 186
P 34 14 * metal
P 35 14 * metal
W 33 13 36 15 en
S 1 H metal P 187 P 188
P 35 5 * metal
P 35 14 * metal
W 34 4 36 15 en
S 1 V metal P 189 P 190
E
