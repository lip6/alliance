#cell1 grmrl_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 66 36
B 0 0 66 36
F F
C w1 0 33 metal2 2 1 * * W w1
C vdd 5 0 metal 7 2 * * S vdd
C vss 44 36 metal 4 3 * * N vss
C vss 44 0 metal 4 4 * * S vss
C s2 56 36 metal 1 5 * * N s2
C s2 56 0 metal 1 6 * * S s2
C s1 60 36 metal 1 7 * * N s1
C s1 60 0 metal 1 8 * * S s1
C vss 66 27 metal2 3 9 * * E vss
C s4 12 0 metal 1 10 * * S s4
C s4 12 36 metal 1 11 * * N s4
C s3 18 0 metal 1 12 * * S s3
C s3 18 36 metal 1 13 * * N s3
C w2 0 21 metal2 2 14 * * W w2
C e1 66 34 metal2 2 15 * * E e1
C vdd 0 27 metal2 3 16 * * W vdd
C w3 0 15 metal2 2 17 * * W w3
C ck_01 64 36 metal 1 18 * * N ck_01
C ck_01 64 0 metal 1 19 * * S ck_01
C w4 0 3 metal2 2 20 * * W w4
C vdd 0 9 metal2 3 21 * * W vdd
C vdd 5 36 metal 7 22 * * N vdd
C ck_02 0 30 poly 1 23 * * W ck_02
C ck_01 66 24 poly 1 24 * * E ck_01
C vdd 66 10 metal2 3 25 * * E vdd
I cdn1 31 27 0 "cdn" ly * *
I cdn2 35 27 0 "cdn" ly * *
I cdn3 32 33 0 "cdn" ly * *
I cdn4 32 21 0 "cdn" ly * *
I cdn5 22 15 0 "cdn" ly * *
I cdn6 32 3 0 "cdn" ly * *
I cdp1 8 27 0 "cdp" ly * *
I cdn7 22 9 0 "cdn" ly * *
I cdn8 32 9 0 "cdn" ly * *
I via1 27 15 0 "cvia" ly * *
I via2 27 3 0 "cvia" ly * *
I cdn9 22 3 0 "cdn" ly * *
I cdn10 32 15 0 "cdn" ly * *
I cdn11 44 4 0 "cdn" ly * *
I cdn12 38 4 0 "cdn" ly * *
I via3 26 33 0 "cvia" ly * *
I via4 26 21 0 "cvia" ly * *
I cdn13 38 33 0 "cdn" ly * *
I cdn14 44 11 0 "cdn" ly * *
I cpf1 56 24 0 "cpf" ly * *
I cpf2 60 30 0 "cpf" ly * *
I cbp1 48 27 0 "cbp" ly * *
I cbn1 3 33 0 "cbn" ly * *
I cpf3 17 12 0 "cpf" ly * *
I cpf4 13 6 0 "cpf" ly * *
I via5 3 27 0 "cvia" ly * *
I via6 44 27 0 "cvia" ly * *
I cpf5 52 15 0 "cpf" ly * *
I cdn15 38 21 0 "cdn" ly * *
I via7 38 9 0 "cvia" ly * *
I via8 38 27 0 "cvia" ly * *
I via9 52 34 0 "cvia" ly * *
I cbn2 3 5 0 "cbn" ly * *
I cbn3 3 13 0 "cbn" ly * *
I via10 8 9 0 "cvia" ly * *
I via11 3 9 0 "cvia" ly * *
I cdn16 50 4 0 "cdn" ly * *
I cbp2 48 20 0 "cbp" ly * *
I cpf6 64 25 0 "cpf" ly * *
I via12 50 9 0 "cvia" ly * *
I cdp2 20 27 0 "cdp" ly * *
P 45 30 * poly
P 60 30 * poly
W 44 29 61 31
S 1 H poly P 1 P 2
P 52 15 * metal
P 52 34 * metal
W 51 14 53 35
S 1 V metal P 3 P 4
P 52 14 * poly
P 52 15 * poly
W 51 13 53 16
S 1 V poly P 5 P 6
P 41 14 * poly
P 52 14 * poly
W 40 13 53 15
S 1 H poly P 7 P 8
P 64 24 * poly
P 64 25 * poly
W 63 23 65 26
S 1 V poly P 9 P 10
P 64 24 * poly
P 66 24 * poly
W 63 23 67 25
S 1 H poly P 11 P 12
P 45 24 * poly
P 56 24 * poly
W 44 23 57 25
S 1 H poly P 13 P 14
P 17 12 * poly
P 18 12 * poly
W 16 11 19 13
S 1 H poly P 15 P 16
P 12 6 * metal
P 13 6 * metal
W 11 5 14 7 s4
S 1 H metal P 17 P 18
P 12 0 * metal
P 12 6 * metal
W 11 -1 13 7 s4
S 1 V metal P 19 P 20
P 12 6 * metal
P 12 36 * metal
W 11 5 13 37 s4
S 1 V metal P 21 P 22
P 56 10 * metal2
P 66 10 * metal2
W 53 7 69 13 vdd
S 3 H metal2 P 23 P 24
P 0 9 * metal2
P 9 9 * metal2
W -3 6 12 12 vdd
S 3 H metal2 P 25 P 26
P 9 9 * metal2
P 32 9 * metal2
W 6 6 35 12 vdd
S 3 H metal2 P 27 P 28
P 32 4 * metal2
P 32 9 * metal2
W 29 1 35 12 vdd
S 3 V metal2 P 29 P 30
P 32 4 * metal2
P 56 4 * metal2
W 29 1 59 7 vdd
S 3 H metal2 P 31 P 32
P 56 4 * metal2
P 56 10 * metal2
W 53 1 59 13 vdd
S 3 V metal2 P 33 P 34
P 38 9 * metal2
P 38 27 * metal2
W 36 7 40 29
S 2 V metal2 P 35 P 36
P 38 9 * metal2
P 50 9 * metal2
W 36 7 52 11
S 2 H metal2 P 37 P 38
P 44 20 * metal
P 48 20 * metal
W 42 18 50 22 vss
S 2 H metal P 39 P 40
P 44 0 * metal
P 44 20 * metal
W 40 -4 48 24 vss
S 4 V metal P 41 P 42
P 44 20 * metal
P 44 27 * metal
W 40 16 48 31 vss
S 4 V metal P 43 P 44
P 44 27 * metal
P 44 36 * metal
W 40 23 48 40 vss
S 4 V metal P 45 P 46
P 44 27 * metal
P 48 27 * metal
W 42 25 50 29 vss
S 2 H metal P 47 P 48
P 44 4 * ndif
P 44 11 * ndif
W 42 2 46 13
S 2 V ndif P 49 P 50
P 3 3 * ntie
P 3 16 * ntie
W 0 0 6 19
S 3 V ntie P 51 P 52
P 16 12 * metal
P 16 31 * metal
W 15 11 17 32
S 1 V metal P 53 P 54
P 14 24 * nwell
P 14 29 * nwell
W -2 8 30 45
S 16 V nwell P 55 P 56
P 9 25 * pdif
P 9 28 * pdif
W 4 20 14 33
S 5 V pdif P 57 P 58
P 16 12 * metal
P 18 12 * metal
W 14 10 20 14
S 2 H metal P 59 P 60
P 30 27 * ndif
P 43 27 * ndif
W 28 25 45 29
S 2 H ndif P 61 P 62
P 32 27 * metal
P 38 27 * metal
W 30 25 40 29
S 2 H metal P 63 P 64
P 26 21 * metal
P 38 21 * metal
W 24 19 40 23
S 2 H metal P 65 P 66
P 50 4 * metal
P 50 9 * metal
W 49 3 51 10
S 1 V metal P 67 P 68
P 18 0 * metal
P 18 12 * metal
W 17 -1 19 13 s3
S 1 V metal P 69 P 70
P 4 0 * nwell
P 4 36 * nwell
W -4 -8 12 44
S 8 V nwell P 71 P 72
P 5 0 * metal
P 5 36 * metal
W -2 -7 12 43 vdd
S 7 V metal P 73 P 74
P 2 21 * nwell
P 5 21 * nwell
W -2 17 9 25
S 4 H nwell P 75 P 76
P 0 21 * nwell
P 2 21 * nwell
W -4 17 6 25
S 4 H nwell P 77 P 78
P 2 9 * nwell
P 5 9 * nwell
W -2 5 9 13
S 4 H nwell P 79 P 80
P 0 9 * nwell
P 2 9 * nwell
W -4 5 6 13
S 4 H nwell P 81 P 82
P 0 15 * nwell
P 5 15 * nwell
W -4 11 9 19
S 4 H nwell P 83 P 84
P 23 9 * metal
P 38 9 * metal
W 21 7 40 11
S 2 H metal P 85 P 86
P 38 5 * metal
P 38 8 * metal
W 36 3 40 10
S 2 V metal P 87 P 88
P 16 31 * metal
P 18 31 * metal
W 15 30 19 32
S 1 H metal P 89 P 90
P 18 31 * metal
P 18 36 * metal
W 17 30 19 37
S 1 V metal P 91 P 92
P 19 25 * pdif
P 19 28 * pdif
W 14 20 24 33
S 5 V pdif P 93 P 94
P 28 9 * ndif
P 33 9 * ndif
W 26 7 35 11
S 2 H ndif P 95 P 96
P 24 9 * ndif
P 28 9 * ndif
W 22 7 30 11
S 2 H ndif P 97 P 98
P 20 3 * ndif
P 33 3 * ndif
W 17 0 36 6
S 3 H ndif P 99 P 100
P 20 15 * ndif
P 33 15 * ndif
W 17 12 36 18
S 3 H ndif P 101 P 102
P 50 3 * ndif
P 50 12 * ndif
W 47 0 53 15
S 3 V ndif P 103 P 104
P 49 20 * ptie
P 58 20 * ptie
W 46 17 61 23
S 3 H ptie P 105 P 106
P 47 1 * ntrans
P 47 14 * ntrans
W 46 0 48 15
S 1 V ntrans P 107 P 108
P 0 30 * poly
P 14 30 * poly
W -1 29 15 31 ck_03
S 1 H poly P 109 P 110
P 14 23 * ptrans
P 14 30 * ptrans
W 13 22 15 31
S 1 V ptrans P 111 P 112
P 0 3 * metal2
P 27 3 * metal2
W -2 1 29 5 w4
S 2 H metal2 P 113 P 114
P 64 0 * metal
P 64 36 * metal
W 63 -1 65 37 ck_01
S 1 V metal P 115 P 116
P 0 15 * metal2
P 27 15 * metal2
W -2 13 29 17 w3
S 2 H metal2 P 117 P 118
P 0 27 * metal2
P 4 27 * metal2
W -3 24 7 30 vdd
S 3 H metal2 P 119 P 120
P 52 34 * metal2
P 66 34 * metal2
W 50 32 68 36 e1
S 2 H metal2 P 121 P 122
P 45 27 * metal2
P 66 27 * metal2
W 42 24 69 30 vss
S 3 H metal2 P 123 P 124
P 56 0 * metal
P 56 36 * metal
W 55 -1 57 37 s2
S 1 V metal P 125 P 126
P 0 21 * metal2
P 26 21 * metal2
W -2 19 28 23 w2
S 2 H metal2 P 127 P 128
P 21 27 * metal
P 32 27 * metal
W 20 26 33 28 tr_p
S 1 H metal P 129 P 130
P 30 21 * ndif
P 43 21 * ndif
W 27 18 46 24
S 3 H ndif P 131 P 132
P 30 33 * ndif
P 43 33 * ndif
W 27 30 46 36
S 3 H ndif P 133 P 134
P 13 6 * poly
P 18 6 * poly
W 12 5 19 7
S 1 H poly P 135 P 136
P 38 3 * ndif
P 38 12 * ndif
W 35 0 41 15
S 3 V ndif P 137 P 138
P 23 15 * metal
P 32 15 * metal
W 21 13 34 17
S 2 H metal P 139 P 140
P 20 9 * ndif
P 23 9 * ndif
W 17 6 26 12
S 3 H ndif P 141 P 142
P 23 3 * metal
P 32 3 * metal
W 21 1 34 5
S 2 H metal P 143 P 144
P 60 0 * metal
P 60 36 * metal
W 59 -1 61 37 s1
S 1 V metal P 145 P 146
P 41 1 * ntrans
P 41 14 * ntrans
W 40 0 42 15
S 1 V ntrans P 147 P 148
P 0 33 * metal2
P 26 33 * metal2
W -2 31 28 35 w1
S 2 H metal2 P 149 P 150
P 18 6 * ntrans
P 35 6 * ntrans
W 17 5 36 7
S 1 H ntrans P 151 P 152
P 18 12 * ntrans
P 35 12 * ntrans
W 17 11 36 13
S 1 H ntrans P 153 P 154
P 26 33 * metal
P 38 33 * metal
W 24 31 40 35
S 2 H metal P 155 P 156
P 28 24 * ntrans
P 45 24 * ntrans
W 27 23 46 25
S 1 H ntrans P 157 P 158
P 28 30 * ntrans
P 45 30 * ntrans
W 27 29 46 31
S 1 H ntrans P 159 P 160
P 11 28 * pdif
P 11 25 * pdif
W 9 23 13 30
S 2 V pdif P 161 P 162
P 17 28 * pdif
P 17 25 * pdif
W 15 23 19 30
S 2 V pdif P 163 P 164
E
