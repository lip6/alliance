#cell1 grmrw0_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 20 97
B 0 0 20 97
F F
C s3 20 96 metal 1 1 * * E s3
C s3 12 97 metal 1 2 * * N s3
C ck_11 20 94 metal2 2 3 * * E ck_11
C ck_11 0 94 metal2 2 4 * * W ck_11
C e11 20 60 metal2 2 5 * * E e11
C e11 0 60 metal2 2 6 * * W e11
C e9 0 82 metal2 2 7 * * W e9
C vdd 0 76 metal2 4 8 * * W vdd
C e10 0 70 metal2 2 9 * * W e10
C e10 20 70 metal2 2 10 * * E e10
C e14 0 23 metal2 2 11 * * W e14
C e15 0 8 metal2 2 12 * * W e15
C e13 0 28 metal2 2 13 * * W e13
C e12 0 33 metal2 2 14 * * W e12
C vdd 0 44 metal2 4 15 * * W vdd
C vdd 0 97 metal 5 16 * * N vdd
C vdd 0 0 metal 5 17 * * S vdd
C vdd 20 76 metal2 4 18 * * E vdd
C vdd 20 44 metal2 4 19 * * E vdd
C vss1 20 88 metal2 4 20 * * E vss1
C e9 20 82 metal2 2 21 * * E e9
C e12 20 33 metal2 2 22 * * E e12
C e8 0 65 metal2 2 23 * * W e8
C s4 6 97 metal 1 24 * * N s4
C e13 20 28 metal2 2 25 * * E e13
C e15 20 8 metal2 2 26 * * E e15
C e14 20 23 metal2 2 27 * * E e14
C e7 20 50 metal2 2 28 * * E e7
C e5 20 13 metal2 2 29 * * E e5
C vdd 20 21 metal 3 30 * * E vdd
C vss0 20 38 metal2 2 31 * * E vss0
I via1 2 51 0 "cvia" ly * *
I cdn1 14 89 0 "cdn" ly * *
I cdn2 8 85 0 "cdn" ly * *
I cdp1 8 65 0 "cdp" ly * *
I cdp2 20 65 0 "cdp" ly * *
I cdp3 14 60 0 "cdp" ly * *
I cdn3 10 40 0 "cdn" ly * *
I cdp4 14 26 0 "cdp" ly * *
I cpf1 7 55 0 "cpf" ly * *
I cbn1 0 70 0 "cbn" ly * *
I cbn2 0 65 0 "cbn" ly * *
I cbn3 0 60 0 "cbn" ly * *
I cbn4 1 26 0 "cbn" ly * *
I cdp5 8 24 0 "cdp" ly * *
I cdp6 20 25 0 "cdp" ly * *
I via2 8 88 0 "cvia" ly * *
I cdp7 8 70 0 "cdp" ly * *
I cdp8 8 60 0 "cdp" ly * *
I via3 7 76 0 "cvia" ly * *
I via4 20 75 0 "cvia" ly * *
I cdp9 14 70 0 "cdp" ly * *
I cdp10 14 65 0 "cdp" ly * *
I cbn5 1 22 0 "cbn" ly * *
I cdn4 20 41 0 "cdn" ly * *
I via5 20 38 0 "cvia" ly * *
I via6 19 50 0 "cvia" ly * *
I cpf2 17 46 0 "cpf" ly * *
I cpf3 11 16 0 "cpf" ly * *
I via7 15 16 0 "cvia" ly * *
I via8 2 47 0 "cvia" ly * *
I via9 2 43 0 "cvia" ly * *
I via10 3 76 0 "cvia" ly * *
I cdp11 20 60 0 "cdp" ly * *
I cdn5 20 90 0 "cdn" ly * *
I via11 20 87 0 "cvia" ly * *
I cbn6 12 53 0 "cbn" ly * *
I cdp12 20 70 0 "cdp" ly * *
P 12 53 * metal
P 12 55 * metal
W 10 51 14 57
S 2 V metal P 1 P 2
P 12 55 * metal
P 20 55 * metal
W 11 54 21 56
S 1 H metal P 3 P 4
P 1 21 * metal
P 20 21 * metal
W -2 18 23 24 vdd
S 3 H metal P 5 P 6
P 20 20 * metal
P 20 23 * metal
W 18 18 22 25
S 2 V metal P 7 P 8
P 14 60 * metal
P 14 89 * metal
W 13 59 15 90
S 1 V metal P 9 P 10
P 11 83 * ntrans
P 11 91 * ntrans
W 10 82 12 92
S 1 V ntrans P 11 P 12
P 14 89 * metal
P 14 92 * metal
W 12 87 16 94 n1
S 2 V metal P 13 P 14
P 10 20 * nwell
P 10 28 * nwell
W -14 -4 34 52
S 24 V nwell P 15 P 16
P 20 37 * ndif
P 20 42 * ndif
W 17 34 23 45
S 3 V ndif P 17 P 18
P 5 59 * metal
P 5 77 * metal
W -3 51 13 85
S 8 V metal P 19 P 20
P 8 60 * metal
P 8 70 * metal
W 7 59 9 71 vdd
S 1 V metal P 21 P 22
P 0 0 * metal
P 0 97 * metal
W -5 -5 5 102 vdd
S 5 V metal P 23 P 24
P 1 22 * ntie
P 1 26 * ntie
W -2 19 4 29
S 3 V ntie P 25 P 26
P 0 59 * ntie
P 0 72 * ntie
W -3 56 3 75
S 3 V ntie P 27 P 28
P 20 86 * ndif
P 20 91 * ndif
W 17 83 23 94
S 3 V ndif P 29 P 30
P 8 84 * ndif
P 8 89 * ndif
W 5 81 11 92
S 3 V ndif P 31 P 32
P 8 59 * pdif
P 8 72 * pdif
W 5 56 11 75
S 3 V pdif P 33 P 34
P 20 22 * pdif
P 20 27 * pdif
W 17 19 23 30
S 3 V pdif P 35 P 36
P 8 22 * pdif
P 8 27 * pdif
W 5 19 11 30
S 3 V pdif P 37 P 38
P 20 87 * metal
P 20 90 * metal
W 18 85 22 92
S 2 V metal P 39 P 40
P 8 85 * metal
P 8 88 * metal
W 6 83 10 90
S 2 V metal P 41 P 42
P 14 22 * pdif
P 14 27 * pdif
W 12 20 16 29
S 2 V pdif P 43 P 44
P 11 29 * poly
P 11 35 * poly
W 10 28 12 36
S 1 V poly P 45 P 46
P 11 35 * poly
P 13 35 * poly
W 10 34 14 36
S 1 H poly P 47 P 48
P 17 29 * poly
P 17 35 * poly
W 16 28 18 36
S 1 V poly P 49 P 50
P 20 55 * metal
P 20 75 * metal
W 18 53 22 77
S 2 V metal P 51 P 52
P 17 74 * poly
P 17 84 * poly
W 16 73 18 85
S 1 V poly P 53 P 54
P 17 84 * ntrans
P 17 92 * ntrans
W 16 83 18 93
S 1 V ntrans P 55 P 56
P 14 26 * metal
P 14 31 * metal
W 13 25 15 32
S 1 V metal P 57 P 58
P 14 86 * ndif
P 14 90 * ndif
W 12 84 16 92
S 2 V ndif P 59 P 60
P 0 23 * metal2
P 20 23 * metal2
W -2 21 22 25 e14
S 2 H metal2 P 61 P 62
P 0 8 * metal2
P 20 8 * metal2
W -2 6 22 10 e15
S 2 H metal2 P 63 P 64
P 0 33 * metal2
P 20 33 * metal2
W -2 31 22 35 e12
S 2 H metal2 P 65 P 66
P 0 28 * metal2
P 20 28 * metal2
W -2 26 22 30 e13
S 2 H metal2 P 67 P 68
P 0 60 * metal2
P 20 60 * metal2
W -2 58 22 62
S 2 H metal2 P 69 P 70
P 1 70 * metal2
P 21 70 * metal2
W -1 68 23 72 e10
S 2 H metal2 P 71 P 72
P 0 94 * metal2
P 20 94 * metal2
W -2 92 22 96
S 2 H metal2 P 73 P 74
P 20 59 * pdif
P 20 72 * pdif
W 17 56 23 75
S 3 V pdif P 75 P 76
P 14 59 * pdif
P 14 72 * pdif
W 11 56 17 75
S 3 V pdif P 77 P 78
P 20 38 * metal
P 20 41 * metal
W 18 36 22 43 vss
S 2 V metal P 79 P 80
P 10 37 * ndif
P 10 42 * ndif
W 8 35 12 44
S 2 V ndif P 81 P 82
P 6 92 * metal
P 6 97 * metal
W 5 91 7 98 n1
S 1 V metal P 83 P 84
P 6 92 * metal
P 14 92 * metal
W 5 91 15 93 n1
S 1 H metal P 85 P 86
P 12 96 * metal
P 12 97 * metal
W 11 95 13 98
S 1 V metal P 87 P 88
P 12 96 * metal
P 20 96 * metal
W 11 95 21 97
S 1 H metal P 89 P 90
P 0 65 * metal2
P 20 65 * metal2
W -2 63 22 67 e8
S 2 H metal2 P 91 P 92
P 0 82 * metal2
P 20 82 * metal2
W -2 80 22 84 e9
S 2 H metal2 P 93 P 94
P 7 88 * metal2
P 20 88 * metal2
W 3 84 24 92 vss1
S 4 H metal2 P 95 P 96
P 0 76 * metal2
P 20 76 * metal2
W -4 72 24 80 vdd
S 4 H metal2 P 97 P 98
P 11 20 * ptrans
P 11 29 * ptrans
W 10 19 12 30
S 1 V ptrans P 99 P 100
P 17 20 * ptrans
P 17 29 * ptrans
W 16 19 18 30
S 1 V ptrans P 101 P 102
P 17 35 * ntrans
P 17 44 * ntrans
W 16 34 18 45
S 1 V ntrans P 103 P 104
P 13 35 * ntrans
P 13 44 * ntrans
W 12 34 14 45
S 1 V ntrans P 105 P 106
P 11 57 * ptrans
P 11 74 * ptrans
W 10 56 12 75
S 1 V ptrans P 107 P 108
P 11 74 * poly
P 11 83 * poly
W 10 73 12 84
S 1 V poly P 109 P 110
P 17 57 * ptrans
P 17 74 * ptrans
W 16 56 18 75
S 1 V ptrans P 111 P 112
P 11 16 * metal
P 15 16 * metal
W 9 14 17 18
S 2 H metal P 113 P 114
P 15 13 * metal2
P 20 13 * metal2
W 13 11 22 15 e5
S 2 H metal2 P 115 P 116
P 15 13 * metal2
P 15 16 * metal2
W 12 10 18 19 e5
S 3 V metal2 P 117 P 118
P 2 43 * metal
P 2 51 * metal
W 0 41 4 53
S 2 V metal P 119 P 120
P 0 44 * metal2
P 20 44 * metal2
W -4 40 24 48 vdd
S 4 H metal2 P 121 P 122
P 2 44 * metal2
P 2 51 * metal2
W -1 41 5 54 vdd
S 3 V metal2 P 123 P 124
P 0 42 * metal2
P 0 52 * metal2
W -2 40 2 54 vdd
S 2 V metal2 P 125 P 126
P 11 53 * ntie
P 20 53 * ntie
W 8 50 23 56
S 3 H ntie P 127 P 128
P 20 23 * metal
P 20 25 * metal
W 18 21 22 27
S 2 V metal P 129 P 130
P 8 21 * metal
P 8 24 * metal
W 6 19 10 26 vdd
S 2 V metal P 131 P 132
P 7 40 * metal
P 10 40 * metal
W 5 38 12 42
S 2 H metal P 133 P 134
P 7 31 * metal
P 7 40 * metal
W 6 30 8 41
S 1 V metal P 135 P 136
P 7 40 * metal
P 7 55 * metal
W 6 39 8 56
S 1 V metal P 137 P 138
P 7 31 * metal
P 14 31 * metal
W 6 30 15 32
S 1 H metal P 139 P 140
P 10 51 * nwell
P 10 73 * nwell
W -14 27 34 97
S 24 V nwell P 141 P 142
P 7 57 * poly
P 17 57 * poly
W 6 56 18 58
S 1 H poly P 143 P 144
P 7 55 * poly
P 7 57 * poly
W 4 52 10 60
S 3 V poly P 145 P 146
P 17 47 * metal
P 19 47 * metal
W 16 46 20 48
S 1 H metal P 147 P 148
P 19 47 * metal
P 19 50 * metal
W 17 45 21 52
S 2 V metal P 149 P 150
P 17 46 * metal
P 17 47 * metal
W 15 44 19 49
S 2 V metal P 151 P 152
P 19 50 * metal2
P 20 50 * metal2
W 17 48 22 52 e7
S 2 H metal2 P 153 P 154
P 17 44 * poly
P 17 46 * poly
W 16 43 18 47
S 1 V poly P 155 P 156
P 11 16 * poly
P 11 20 * poly
W 10 15 12 21
S 1 V poly P 157 P 158
E
