#cell1 grmrw1_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 15 97
B 0 0 15 97
F F
C e4 15 55 metal2 2 1 * * E e4
C e5 15 13 metal2 2 2 * * E e5
C e5 0 13 metal2 2 3 * * W e5
C e8 15 65 metal2 2 4 * * E e8
C e10 15 70 metal2 2 5 * * E e10
C e10 0 70 metal2 2 6 * * W e10
C e15 15 8 metal2 2 7 * * E e15
C e13 15 28 metal2 2 8 * * E e13
C e14 15 23 metal2 2 9 * * E e14
C vdd1 15 44 metal2 4 10 * * E vdd1
C e12 15 33 metal2 2 11 * * E e12
C vdd2 15 76 metal2 4 12 * * E vdd2
C e9 15 82 metal2 2 13 * * E e9
C vss0 15 38 metal2 2 14 * * E vss0
C vss1 15 88 metal2 4 15 * * E vss1
C s3 0 96 metal 1 16 * * W s3
C ck_11 0 94 metal2 2 17 * * W ck_11
C ck_11 15 94 metal2 2 18 * * E ck_11
C e11 15 60 metal2 2 19 * * E e11
C e7 15 50 metal2 2 20 * * E e7
C e15 0 8 metal2 2 21 * * W e15
C vdd0 0 21 metal 3 22 * * W vdd0
C e14 0 23 metal2 2 23 * * W e14
C e13 0 28 metal2 2 24 * * W e13
C e12 0 33 metal2 2 25 * * W e12
C vss0 0 38 metal2 2 26 * * W vss0
C vdd1 0 44 metal2 4 27 * * W vdd1
C e7 0 50 metal2 2 28 * * W e7
C e11 0 60 metal2 2 29 * * W e11
C e8 0 65 metal2 2 30 * * W e8
C vdd2 0 76 metal2 4 31 * * W vdd2
C e9 0 82 metal2 2 32 * * W e9
C vss1 0 88 metal2 4 33 * * W vss1
I cpf1 3 46 0 "cpf" ly * *
I via1 -1 50 0 "cvia" ly * *
I cdn1 0 41 0 "cdn" ly * *
I cbn1 4 54 0 "cbn" ly * *
I via2 0 75 0 "cvia" ly * *
I cbn2 12 17 0 "cbn" ly * *
I cbn3 6 17 0 "cbn" ly * *
I via3 12 55 0 "cvia" ly * *
I cpf2 12 45 0 "cpf" ly * *
I via4 0 38 0 "cvia" ly * *
I cdp1 6 70 0 "cdp" ly * *
I cdp2 6 75 0 "cdp" ly * *
I via5 12 75 0 "cvia" ly * *
I cdp3 0 65 0 "cdp" ly * *
I cdp4 0 70 0 "cdp" ly * *
I via6 12 87 0 "cvia" ly * *
I via7 0 87 0 "cvia" ly * *
I cdp5 12 65 0 "cdp" ly * *
I cdp6 12 70 0 "cdp" ly * *
I cdp7 0 25 0 "cdp" ly * *
I cpf3 8 60 0 "cpf" ly * *
I cdp8 6 26 0 "cdp" ly * *
I cdp9 12 26 0 "cdp" ly * *
I cdn2 10 40 0 "cdn" ly * *
I cdp10 6 65 0 "cdp" ly * *
I cdn3 0 90 0 "cdn" ly * *
I cdn4 12 90 0 "cdn" ly * *
I cdn5 6 87 0 "cdn" ly * *
P 0 21 * metal
P 13 21 * metal
W -3 18 16 24
S 3 H metal P 1 P 2
P 3 84 * ntrans
P 3 92 * ntrans
W 2 83 4 93
S 1 V ntrans P 3 P 4
P 9 84 * ntrans
P 9 92 * ntrans
W 8 83 10 93
S 1 V ntrans P 5 P 6
P 9 61 * ptrans
P 9 78 * ptrans
W 8 60 10 79
S 1 V ptrans P 7 P 8
P 9 78 * poly
P 9 84 * poly
W 8 77 10 85
S 1 V poly P 9 P 10
P 3 78 * poly
P 3 84 * poly
W 2 77 4 85
S 1 V poly P 11 P 12
P 3 61 * ptrans
P 3 78 * ptrans
W 2 60 4 79
S 1 V ptrans P 13 P 14
P 3 35 * ntrans
P 3 44 * ntrans
W 2 34 4 45
S 1 V ntrans P 15 P 16
P 7 35 * ntrans
P 7 44 * ntrans
W 6 34 8 45
S 1 V ntrans P 17 P 18
P 9 20 * ptrans
P 9 29 * ptrans
W 8 19 10 30
S 1 V ptrans P 19 P 20
P 3 20 * ptrans
P 3 29 * ptrans
W 2 19 4 30
S 1 V ptrans P 21 P 22
P 0 88 * metal2
P 15 88 * metal2
W -4 84 19 92
S 4 H metal2 P 23 P 24
P 0 38 * metal2
P 15 38 * metal2
W -2 36 17 40
S 2 H metal2 P 25 P 26
P 0 86 * ndif
P 0 90 * ndif
W -3 83 3 93
S 3 V ndif P 27 P 28
P 12 86 * ndif
P 12 90 * ndif
W 9 83 15 93
S 3 V ndif P 29 P 30
P 0 39 * metal
P 0 41 * metal
W -2 37 2 43
S 2 V metal P 31 P 32
P 6 63 * pdif
P 6 76 * pdif
W 4 61 8 78
S 2 V pdif P 33 P 34
P 0 96 * metal
P 6 96 * metal
W -1 95 7 97
S 1 H metal P 35 P 36
P 6 87 * metal
P 6 96 * metal
W 5 86 7 97
S 1 V metal P 37 P 38
P 12 63 * pdif
P 12 76 * pdif
W 9 60 15 79
S 3 V pdif P 39 P 40
P 10 37 * ndif
P 10 42 * ndif
W 8 35 12 44
S 2 V ndif P 41 P 42
P 0 82 * metal2
P 15 82 * metal2
W -2 80 17 84
S 2 H metal2 P 43 P 44
P 0 76 * metal2
P 15 76 * metal2
W -4 72 19 80
S 4 H metal2 P 45 P 46
P 0 33 * metal2
P 15 33 * metal2
W -2 31 17 35
S 2 H metal2 P 47 P 48
P 0 44 * metal2
P 15 44 * metal2
W -4 40 19 48
S 4 H metal2 P 49 P 50
P 0 23 * metal2
P 15 23 * metal2
W -2 21 17 25
S 2 H metal2 P 51 P 52
P 0 28 * metal2
P 15 28 * metal2
W -2 26 17 30
S 2 H metal2 P 53 P 54
P 0 8 * metal2
P 15 8 * metal2
W -2 6 17 10
S 2 H metal2 P 55 P 56
P 0 94 * metal2
P 15 94 * metal2
W -2 92 17 96
S 2 H metal2 P 57 P 58
P 0 70 * metal2
P 15 70 * metal2
W -2 68 17 72
S 2 H metal2 P 59 P 60
P 0 65 * metal2
P 15 65 * metal2
W -2 63 17 67
S 2 H metal2 P 61 P 62
P 0 60 * metal2
P 15 60 * metal2
W -2 58 17 62
S 2 H metal2 P 63 P 64
P 0 37 * ndif
P 0 42 * ndif
W -3 34 3 45
S 3 V ndif P 65 P 66
P 0 50 * metal2
P 15 50 * metal2
W -2 48 17 52
S 2 H metal2 P 67 P 68
P 0 13 * metal2
P 15 13 * metal2
W -2 11 17 15
S 2 H metal2 P 69 P 70
P 12 45 * metal
P 12 55 * metal
W 11 44 13 56
S 1 V metal P 71 P 72
P 12 55 * metal2
P 15 55 * metal2
W 10 53 17 57
S 2 H metal2 P 73 P 74
P 6 51 * nwell
P 6 77 * nwell
W -10 35 22 93
S 16 V nwell P 75 P 76
P 5 18 * metal
P 13 18 * metal
W 2 15 16 21
S 3 H metal P 77 P 78
P 6 86 * ndif
P 6 90 * ndif
W 4 84 8 92
S 2 V ndif P 79 P 80
P 6 76 * metal
P 6 87 * metal
W 5 75 7 88
S 1 V metal P 81 P 82
P 7 15 * nwell
P 7 28 * nwell
W -11 -3 25 46
S 18 V nwell P 83 P 84
P 3 44 * poly
P 3 46 * poly
W 2 43 4 47
S 1 V poly P 85 P 86
P 9 29 * poly
P 9 31 * poly
W 8 28 10 32
S 1 V poly P 87 P 88
P 7 31 * poly
P 9 31 * poly
W 6 30 10 32
S 1 H poly P 89 P 90
P 7 31 * poly
P 7 35 * poly
W 6 30 8 36
S 1 V poly P 91 P 92
P 0 63 * pdif
P 0 76 * pdif
W -3 60 3 79
S 3 V pdif P 93 P 94
P 12 22 * pdif
P 12 27 * pdif
W 10 20 14 29
S 2 V pdif P 95 P 96
P 0 87 * metal
P 0 90 * metal
W -2 85 2 92
S 2 V metal P 97 P 98
P 12 87 * metal
P 12 90 * metal
W 10 85 14 92
S 2 V metal P 99 P 100
P 6 65 * metal
P 6 75 * metal
W 4 63 8 77
S 2 V metal P 101 P 102
P 0 66 * metal
P 0 75 * metal
W -1 65 1 76
S 1 V metal P 103 P 104
P 12 65 * metal
P 12 75 * metal
W 11 64 13 76
S 1 V metal P 105 P 106
P 3 29 * poly
P 3 35 * poly
W 2 28 4 36
S 1 V poly P 107 P 108
P 6 22 * pdif
P 6 27 * pdif
W 4 20 8 29
S 2 V pdif P 109 P 110
P 0 22 * pdif
P 0 27 * pdif
W -3 19 3 30
S 3 V pdif P 111 P 112
P 12 22 * metal
P 12 26 * metal
W 10 20 14 28
S 2 V metal P 113 P 114
P 11 22 * metal
P 12 22 * metal
W 10 21 13 23
S 1 H metal P 115 P 116
P 0 21 * metal
P 0 25 * metal
W -2 19 2 27
S 2 V metal P 117 P 118
P 7 44 * poly
P 12 44 * poly
W 6 43 13 45
S 1 H poly P 119 P 120
P 12 44 * poly
P 12 45 * poly
W 11 43 13 46
S 1 V poly P 121 P 122
P 6 26 * metal
P 8 26 * metal
W 4 24 10 28
S 2 H metal P 123 P 124
P 8 26 * metal
P 8 40 * metal
W 7 25 9 41
S 1 V metal P 125 P 126
P 8 40 * metal
P 10 40 * metal
W 6 38 12 42
S 2 H metal P 127 P 128
P 8 40 * metal
P 8 60 * metal
W 7 39 9 61
S 1 V metal P 129 P 130
P 8 61 * poly
P 9 61 * poly
W 7 60 10 62
S 1 H poly P 131 P 132
P 3 61 * poly
P 8 61 * poly
W 2 60 9 62
S 1 H poly P 133 P 134
P 8 60 * poly
P 8 61 * poly
W 7 59 9 62
S 1 V poly P 135 P 136
P -1 47 * metal
P 3 47 * metal
W -2 46 4 48
S 1 H metal P 137 P 138
P -1 47 * metal
P -1 50 * metal
W -3 45 1 52
S 2 V metal P 139 P 140
P 3 46 * metal
P 3 47 * metal
W 1 44 5 49
S 2 V metal P 141 P 142
P -1 53 * ntie
P 4 53 * ntie
W -4 50 7 56
S 3 H ntie P 143 P 144
P 4 52 * ntie
P 4 53 * ntie
W 1 49 7 56
S 3 V ntie P 145 P 146
P 0 55 * metal
P 4 55 * metal
W -1 54 5 56
S 1 H metal P 147 P 148
P 0 55 * metal
P 0 65 * metal
W -2 53 2 67
S 2 V metal P 149 P 150
P 4 54 * metal
P 4 55 * metal
W 2 52 6 57
S 2 V metal P 151 P 152
E
