#cell1 grmrs_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 41 36
B 0 0 41 36
F F
C vdd 0 10 metal2 3 1 * * W vdd
C ck_01 0 24 poly 1 2 * * W ck_01
C s1 13 36 metal 1 3 * * N s1
C s1 13 0 metal 1 4 * * S s1
C vss 41 27 metal2 3 5 * * E vss
C e1 41 22 metal2 2 6 * * E e1
C e2 41 16 metal2 2 7 * * E e2
C e3 41 4 metal2 2 8 * * E e3
C vdd 41 10 metal2 3 9 * * E vdd
C vss 0 27 metal2 3 10 * * W vss
C w1 0 34 metal2 2 11 * * W w1
I cbp1 2 10 0 "cbp" ly * *
I cdp1 37 33 0 "cdp" ly * *
I cdn1 2 33 0 "cdn" ly * *
I cdp2 35 20 0 "cdp" ly * *
I via1 6 33 0 "cvia" ly * *
I via2 32 34 0 "cvia" ly * *
I cdp3 27 33 0 "cdp" ly * *
I via3 4 4 0 "cvia" ly * *
I via4 17 4 0 "cvia" ly * *
I via5 40 16 0 "cvia" ly * *
I cpf1 40 12 0 "cpf" ly * *
I cpf2 30 16 0 "cpf" ly * *
I via6 30 20 0 "cvia" ly * *
I cpf3 17 22 0 "cpf" ly * *
I via7 40 4 0 "cvia" ly * *
I cpf4 29 3 0 "cpf" ly * *
I via8 9 27 0 "cvia" ly * *
I cdp4 22 33 0 "cdp" ly * *
I cdp5 24 15 0 "cdp" ly * *
I cdp6 24 9 0 "cdp" ly * *
I cdp7 24 3 0 "cdp" ly * *
I cdn2 8 4 0 "cdn" ly * *
I cdn3 9 16 0 "cdn" ly * *
I cdn4 6 27 0 "cdn" ly * *
I cdn5 2 20 0 "cdn" ly * *
I via9 35 9 0 "cvia" ly * *
I cpf5 13 32 0 "cpf" ly * *
I cbn1 31 9 0 "cbn" ly * *
P 40 3 * metal
P 40 4 * metal
W 39 2 41 5
S 1 V metal P 1 P 2
P 29 3 * metal
P 40 3 * metal
W 28 2 41 4
S 1 H metal P 3 P 4
P 23 9 * pdif
P 25 9 * pdif
W 21 7 27 11
S 2 H pdif P 5 P 6
P 23 15 * pdif
P 25 15 * pdif
W 21 13 27 17
S 2 H pdif P 7 P 8
P 24 9 * metal
P 30 9 * metal
W 22 7 32 11
S 2 H metal P 9 P 10
P 29 0 * nwell
P 29 36 * nwell
W 9 -20 49 56
S 20 V nwell P 11 P 12
P 0 10 * metal2
P 35 10 * metal2
W -3 7 38 13 vdd
S 3 H metal2 P 13 P 14
P 13 30 * poly
P 13 32 * poly
W 12 29 14 33
S 1 V poly P 15 P 16
P 9 30 * poly
P 13 30 * poly
W 8 29 14 31
S 1 H poly P 17 P 18
P 13 30 * poly
P 18 30 * poly
W 12 29 19 31
S 1 H poly P 19 P 20
P 5 4 * metal2
P 17 4 * metal2
W 3 2 19 6
S 2 H metal2 P 21 P 22
P 5 24 * ntrans
P 12 24 * ntrans
W 4 23 13 25
S 1 H ntrans P 23 P 24
P 5 19 * ntrans
P 12 19 * ntrans
W 4 18 13 20
S 1 H ntrans P 25 P 26
P 5 13 * ntrans
P 13 13 * ntrans
W 4 12 14 14
S 1 H ntrans P 27 P 28
P 5 10 * ntrans
P 13 10 * ntrans
W 4 9 14 11
S 1 H ntrans P 29 P 30
P 5 7 * ntrans
P 13 7 * ntrans
W 4 6 14 8
S 1 H ntrans P 31 P 32
P 21 12 * ptrans
P 27 12 * ptrans
W 20 11 28 13
S 1 H ptrans P 33 P 34
P 18 26 * ptrans
P 40 26 * ptrans
W 17 25 41 27
S 1 H ptrans P 35 P 36
P 18 30 * ptrans
P 40 30 * ptrans
W 17 29 41 31
S 1 H ptrans P 37 P 38
P 21 6 * ptrans
P 27 6 * ptrans
W 20 5 28 7
S 1 H ptrans P 39 P 40
P 40 4 * metal2
P 41 4 * metal2
W 38 2 43 6 e3
S 2 H metal2 P 41 P 42
P 2 30 * ntrans
P 9 30 * ntrans
W 1 29 10 31
S 1 H ntrans P 43 P 44
P 8 4 * ndif
P 10 4 * ndif
W 6 2 12 6
S 2 H ndif P 45 P 46
P 4 4 * metal
P 8 4 * metal
W 2 2 10 6
S 2 H metal P 47 P 48
P 13 6 * metal
P 13 24 * metal
W 12 5 14 25 s1
S 1 V metal P 49 P 50
P 13 35 * metal
P 13 36 * metal
W 12 34 14 37 s1
S 1 V metal P 51 P 52
P 13 10 * poly
P 18 10 * poly
W 12 9 19 11
S 1 H poly P 53 P 54
P 40 12 * metal
P 40 16 * metal
W 38 10 42 18
S 2 V metal P 55 P 56
P 35 9 * metal
P 35 20 * metal
W 33 7 37 22
S 2 V metal P 57 P 58
P 20 33 * pdif
P 37 33 * pdif
W 17 30 40 36
S 3 H pdif P 59 P 60
P 0 34 * metal2
P 32 34 * metal2
W -2 32 34 36 w1
S 2 H metal2 P 61 P 62
P 13 0 * metal
P 13 6 * metal
W 12 -1 14 7
S 1 V metal P 63 P 64
P 30 16 * metal
P 30 20 * metal
W 28 14 32 22
S 2 V metal P 65 P 66
P 18 23 * ptrans
P 40 23 * ptrans
W 17 22 41 24
S 1 H ptrans P 67 P 68
P 21 18 * ptrans
P 27 18 * ptrans
W 20 17 28 19
S 1 H ptrans P 69 P 70
P 13 24 * metal
P 13 35 * metal
W 12 23 14 36 s1
S 1 V metal P 71 P 72
P 7 16 * ndif
P 10 16 * ndif
W 4 13 13 19
S 3 H ndif P 73 P 74
P 2 10 * metal
P 9 10 * metal
W 1 9 10 11
S 1 H metal P 75 P 76
P 9 10 * metal
P 9 18 * metal
W 8 9 10 19
S 1 V metal P 77 P 78
P 17 15 * metal
P 24 15 * metal
W 16 14 25 16
S 1 H metal P 79 P 80
P 17 15 * metal
P 17 22 * metal
W 16 14 18 23
S 1 V metal P 81 P 82
P 17 3 * metal
P 17 15 * metal
W 16 2 18 16
S 1 V metal P 83 P 84
P 17 3 * metal
P 24 3 * metal
W 16 2 25 4
S 1 H metal P 85 P 86
P 9 19 * metal
P 9 27 * metal
W 8 18 10 28
S 1 V metal P 87 P 88
P 6 27 * metal
P 9 27 * metal
W 4 25 11 29
S 2 H metal P 89 P 90
P 2 33 * metal
P 6 33 * metal
W 0 31 8 35
S 2 H metal P 91 P 92
P 2 33 * ndif
P 7 33 * ndif
W -1 30 10 36
S 3 H ndif P 93 P 94
P 32 33 * metal
P 37 33 * metal
W 30 31 39 35
S 2 H metal P 95 P 96
P 22 33 * metal
P 32 33 * metal
W 20 31 34 35
S 2 H metal P 97 P 98
P 32 33 * metal
P 32 34 * metal
W 30 31 34 36
S 2 V metal P 99 P 100
P 2 20 * metal
P 2 33 * metal
W 1 19 3 34
S 1 V metal P 101 P 102
P 5 21 * ndif
P 5 22 * ndif
W -4 12 14 31
S 9 V ndif P 103 P 104
P 0 24 * poly
P 5 24 * poly
W -1 23 6 25 ck_01
S 1 H poly P 105 P 106
P 0 27 * metal2
P 41 27 * metal2
W -3 24 44 30 vss
S 3 H metal2 P 107 P 108
P 4 27 * ndif
P 6 27 * ndif
W 1 24 9 30
S 3 H ndif P 109 P 110
P 7 27 * ndif
P 9 27 * ndif
W 5 25 11 29
S 2 H ndif P 111 P 112
P 40 16 * metal2
P 41 16 * metal2
W 38 14 43 18 e2
S 2 H metal2 P 113 P 114
P 31 9 * metal
P 35 9 * metal
W 29 7 37 11
S 2 H metal P 115 P 116
P 33 20 * pdif
P 38 20 * pdif
W 30 17 41 23
S 3 H pdif P 117 P 118
P 12 24 * poly
P 12 26 * poly
W 11 23 13 27
S 1 V poly P 119 P 120
P 12 26 * poly
P 18 26 * poly
W 11 25 19 27
S 1 H poly P 121 P 122
P 15 18 * poly
P 21 18 * poly
W 14 17 22 19
S 1 H poly P 123 P 124
P 15 13 * poly
P 15 18 * poly
W 14 12 16 19
S 1 V poly P 125 P 126
P 13 13 * poly
P 15 13 * poly
W 12 12 16 14
S 1 H poly P 127 P 128
P 18 12 * poly
P 21 12 * poly
W 17 11 22 13
S 1 H poly P 129 P 130
P 18 10 * poly
P 18 12 * poly
W 17 9 19 13
S 1 V poly P 131 P 132
P 13 7 * poly
P 21 7 * poly
W 12 6 22 8
S 1 H poly P 133 P 134
P 21 6 * poly
P 21 7 * poly
W 20 5 22 8
S 1 V poly P 135 P 136
P 27 12 * poly
P 40 12 * poly
W 26 11 41 13
S 1 H poly P 137 P 138
P 30 22 * metal2
P 41 22 * metal2
W 28 20 43 24 e1
S 2 H metal2 P 139 P 140
P 30 20 * metal2
P 30 22 * metal2
W 27 17 33 25 e1
S 3 V metal2 P 141 P 142
P 35 9 * metal2
P 35 10 * metal2
W 32 6 38 13 vdd
S 3 V metal2 P 143 P 144
P 35 10 * metal2
P 41 10 * metal2
W 32 7 44 13 vdd
S 3 H metal2 P 145 P 146
P 12 21 * poly
P 17 21 * poly
W 11 20 18 22
S 1 H poly P 147 P 148
P 12 19 * poly
P 12 21 * poly
W 11 18 13 22
S 1 V poly P 149 P 150
P 17 21 * poly
P 17 23 * poly
W 16 20 18 24
S 1 V poly P 151 P 152
P 17 23 * poly
P 18 23 * poly
W 16 22 19 24
S 1 H poly P 153 P 154
P 28 4 * poly
P 29 4 * poly
W 27 3 30 5
S 1 H poly P 155 P 156
P 28 4 * poly
P 28 6 * poly
W 27 3 29 7
S 1 V poly P 157 P 158
P 27 6 * poly
P 28 6 * poly
W 26 5 29 7
S 1 H poly P 159 P 160
P 29 3 * poly
P 29 4 * poly
W 28 2 30 5
S 1 V poly P 161 P 162
P 27 17 * poly
P 30 17 * poly
W 26 16 31 18
S 1 H poly P 163 P 164
P 27 17 * poly
P 27 18 * poly
W 26 16 28 19
S 1 V poly P 165 P 166
P 30 16 * poly
P 30 17 * poly
W 29 15 31 18
S 1 V poly P 167 P 168
P 23 20 * pdif
P 25 20 * pdif
W 20 17 28 23
S 3 H pdif P 169 P 170
E
