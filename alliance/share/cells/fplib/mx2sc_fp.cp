#cell1 mx2sc_fp idps_symb compose *
# 19-Nov-97 19:05 19-Nov-97 19:05 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 38 65
B 5 5 38 65
F F
C vss 38 5 metal2 8 1 * * E vss
C vdd 38 65 metal2 6 2 * * E vdd
C s1i0 21 65 metal 1 3 * * N s1i0
C s1i0 21 5 metal 1 4 * * S s1i0
C s0i0 12 5 metal 1 5 * * S s0i0
C s0i0 12 65 metal 1 6 * * N s0i0
C vss 5 5 metal2 8 7 * * W vss
C vdd 5 65 metal2 6 8 * * W vdd
I cdn1 17 13 0 "cdn" ly * *
I cdn2 17 19 0 "cdn" ly * *
I cpf1 17 32 0 "cpf" ly * *
I cpf2 25 23 0 "cpf" ly * *
I cpf3 25 37 0 "cpf" ly * *
I cdn3 35 18 0 "cdn" ly * *
I cdn4 29 13 0 "cdn" ly * *
I cdp1 29 59 0 "cdp" ly * *
I cdp2 35 53 0 "cdp" ly * *
I cdp3 29 53 0 "cdp" ly * *
I cbn1 29 65 0 "cbn" ly * *
I cbn2 35 65 0 "cbn" ly * *
I cbp1 35 5 0 "cbp" ly * *
I cbp2 29 5 0 "cbp" ly * *
I via1 32 65 0 "cvia" ly * *
I via2 32 5 0 "cvia" ly * *
I cdp4 17 41 0 "cdp" ly * *
I cdp5 17 55 0 "cdp" ly * *
I cdn5 8 13 0 "cdn" ly * *
I via3 8 8 0 "cvia" ly * *
I cbp3 8 5 0 "cbp" ly * *
I cpf4 16 60 0 "cpf" ly * *
I cpf5 21 5 0 "cpf" ly * *
I via4 8 65 0 "cvia" ly * *
I cpf6 16 65 0 "cpf" ly * *
I cpf7 13 5 0 "cpf" ly * *
I cpf8 8 28 0 "cpf" ly * *
I cbn3 8 62 0 "cbn" ly * *
I cdp6 8 53 0 "cdp" ly * *
I cdp7 17 48 0 "cdp" ly * *
I o_2 35 48 0 "ref_con" ly * *
I o_0 35 58 0 "ref_con" ly * *
I o_3 35 43 0 "ref_con" ly * *
I o_4 35 38 0 "ref_con" ly * *
I o_5 35 33 0 "ref_con" ly * *
I o_6 35 28 0 "ref_con" ly * *
I i1_3 29 43 0 "ref_con" ly * *
I i1_2 29 48 0 "ref_con" ly * *
I nwell_28 37 53 0 "ref_ref" ly * *
I o_7 35 23 0 "ref_con" ly * *
I o_9 35 13 0 "ref_con" ly * *
I i1_7 29 23 0 "ref_con" ly * *
I i1_8 29 18 0 "ref_con" ly * *
I i1_4 29 38 0 "ref_con" ly * *
I i1_6 29 28 0 "ref_con" ly * *
I i0_8 8 18 0 "ref_con" ly * *
I i0_5 8 33 0 "ref_con" ly * *
I i0_7 8 23 0 "ref_con" ly * *
I i0_2 8 48 0 "ref_con" ly * *
I i0_3 8 43 0 "ref_con" ly * *
I i0_4 8 38 0 "ref_con" ly * *
P 5 5 * metal2
P 38 5 * metal2
W -3 -3 46 13
S 8 H metal2 P 1 P 2
P 5 13 * allowM2
P 38 13 * allowM2
W 3 11 40 15
S 2 H allowM2 P 3 P 4
P 5 18 * allowM2
P 38 18 * allowM2
W 3 16 40 20
S 2 H allowM2 P 5 P 6
P 5 23 * allowM2
P 38 23 * allowM2
W 3 21 40 25
S 2 H allowM2 P 7 P 8
P 5 28 * allowM2
P 38 28 * allowM2
W 3 26 40 30
S 2 H allowM2 P 9 P 10
P 5 33 * allowM2
P 38 33 * allowM2
W 3 31 40 35
S 2 H allowM2 P 11 P 12
P 5 38 * allowM2
P 38 38 * allowM2
W 3 36 40 40
S 2 H allowM2 P 13 P 14
P 5 43 * allowM2
P 38 43 * allowM2
W 3 41 40 45
S 2 H allowM2 P 15 P 16
P 5 48 * allowM2
P 38 48 * allowM2
W 3 46 40 50
S 2 H allowM2 P 17 P 18
P 5 53 * allowM2
P 38 53 * allowM2
W 3 51 40 55
S 2 H allowM2 P 19 P 20
P 5 58 * allowM2
P 38 58 * allowM2
W 3 56 40 60
S 2 H allowM2 P 21 P 22
P 5 65 * metal2
P 38 65 * metal2
W -1 59 44 71
S 6 H metal2 P 23 P 24
P 5 53 * nwell
P 38 53 * nwell
W -23 25 66 81
S 28 H nwell P 25 P 26
P 17 13 * metal
P 17 55 * metal
W 16 12 18 56 d
S 1 V metal P 27 P 28
P 17 32 * poly
P 32 32 * poly
W 16 31 33 33
S 1 H poly P 29 P 30
P 25 23 * metal
P 29 23 * metal
W 23 21 31 25
S 2 H metal P 31 P 32
P 25 37 * metal
P 29 37 * metal
W 23 35 31 39
S 2 H metal P 33 P 34
P 26 23 * poly
P 26 22 * poly
W 25 21 27 24
S 1 V poly P 35 P 36
P 25 23 * poly
P 26 23 * poly
W 24 22 27 24
S 1 H poly P 37 P 38
P 26 37 * poly
P 26 38 * poly
W 25 36 27 39
S 1 V poly P 39 P 40
P 25 37 * poly
P 26 37 * poly
W 24 36 27 38
S 1 H poly P 41 P 42
P 23 20 * ndif
P 23 12 * ndif
W 20 9 26 23
S 3 V ndif P 43 P 44
P 23 57 * pdif
P 23 40 * pdif
W 20 37 26 60
S 3 V pdif P 45 P 46
P 35 14 * ndif
P 35 19 * ndif
W 32 11 38 22
S 3 V ndif P 47 P 48
P 29 12 * ndif
P 29 20 * ndif
W 26 9 32 23
S 3 V ndif P 49 P 50
P 32 12 * ntrans
P 32 21 * ntrans
W 31 11 33 22
S 1 V ntrans P 51 P 52
P 26 10 * ntrans
P 26 22 * ntrans
W 25 9 27 23
S 1 V ntrans P 53 P 54
P 32 47 * ptrans
P 32 62 * ptrans
W 31 46 33 63
S 1 V ptrans P 55 P 56
P 29 40 * pdif
P 29 60 * pdif
W 26 37 32 63
S 3 V pdif P 57 P 58
P 26 38 * ptrans
P 26 59 * ptrans
W 25 37 27 60
S 1 V ptrans P 59 P 60
P 35 49 * pdif
P 35 60 * pdif
W 32 46 38 63
S 3 V pdif P 61 P 62
P 28 65 * ntie
P 36 65 * ntie
W 25 62 39 68
S 3 H ntie P 63 P 64
P 28 5 * ptie
P 36 5 * ptie
W 25 2 39 8
S 3 H ptie P 65 P 66
P 35 58 * metal
P 35 13 * metal
W 34 12 36 59 o
S 1 V metal P 67 P 68
P 29 53 * metal
P 29 65 * metal
W 27 51 31 67
S 2 V metal P 69 P 70
P 29 5 * metal
P 29 13 * metal
W 27 3 31 15
S 2 V metal P 71 P 72
P 29 18 * metal
P 29 48 * metal
W 28 17 30 49 i1
S 1 V metal P 73 P 74
P 29 65 * metal
P 35 65 * metal
W 27 63 37 67
S 2 H metal P 75 P 76
P 29 5 * metal
P 35 5 * metal
W 27 3 37 7
S 2 H metal P 77 P 78
P 11 22 * poly
P 11 28 * poly
W 10 21 12 29
S 1 V poly P 79 P 80
P 8 66 * ntie
P 8 61 * ntie
W 5 58 11 69
S 3 V ntie P 81 P 82
P 14 60 * poly
P 16 60 * poly
W 11 57 19 63
S 3 H poly P 83 P 84
P 14 60 * poly
P 14 59 * poly
W 13 58 15 61
S 1 V poly P 85 P 86
P 20 65 * poly
P 20 59 * poly
W 19 58 21 66
S 1 V poly P 87 P 88
P 16 65 * poly
P 20 65 * poly
W 15 64 21 66
S 1 H poly P 89 P 90
P 16 60 * metal
P 21 60 * metal
W 15 59 22 61
S 1 H metal P 91 P 92
P 12 65 * metal
P 16 65 * metal
W 11 64 17 66
S 1 H metal P 93 P 94
P 21 5 * metal
P 21 65 * metal
W 20 4 22 66 s1i0
S 1 V metal P 95 P 96
P 8 18 * metal
P 8 48 * metal
W 7 17 9 49 i0
S 1 V metal P 97 P 98
P 20 10 * ntrans
P 20 22 * ntrans
W 19 9 21 23
S 1 V ntrans P 99 P 100
P 17 12 * ndif
P 17 20 * ndif
W 14 9 20 23
S 3 V ndif P 101 P 102
P 8 12 * ndif
P 8 20 * ndif
W 5 9 11 23
S 3 V ndif P 103 P 104
P 11 10 * ntrans
P 11 22 * ntrans
W 10 9 12 23
S 1 V ntrans P 105 P 106
P 14 10 * ntrans
P 14 22 * ntrans
W 13 9 15 23
S 1 V ntrans P 107 P 108
P 20 5 * poly
P 20 10 * poly
W 19 4 21 11
S 1 V poly P 109 P 110
P 14 5 * poly
P 14 10 * poly
W 13 4 15 11
S 1 V poly P 111 P 112
P 8 5 * metal
P 8 13 * metal
W 6 3 10 15
S 2 V metal P 113 P 114
P 8 8 * ptie
P 8 4 * ptie
W 5 1 11 11
S 3 V ptie P 115 P 116
P 20 38 * ptrans
P 20 59 * ptrans
W 19 37 21 60
S 1 V ptrans P 117 P 118
P 20 5 * poly
P 21 5 * poly
W 19 4 22 6
S 1 H poly P 119 P 120
P 16 59 * poly
P 16 60 * poly
W 15 58 17 61
S 1 V poly P 121 P 122
P 8 53 * metal
P 8 65 * metal
W 6 51 10 67
S 2 V metal P 123 P 124
P 12 5 * metal
P 12 65 * metal
W 11 4 13 66 s0i0
S 1 V metal P 125 P 126
P 12 5 * metal
P 13 5 * metal
W 11 4 14 6 s0i0
S 1 H metal P 127 P 128
P 8 28 * poly
P 11 28 * poly
W 5 25 14 31
S 3 H poly P 129 P 130
P 11 28 * poly
P 11 38 * poly
W 10 27 12 39
S 1 V poly P 131 P 132
P 13 5 * poly
P 14 5 * poly
W 12 4 15 6
S 1 H poly P 133 P 134
P 8 40 * pdif
P 8 57 * pdif
W 5 37 11 60
S 3 V pdif P 135 P 136
P 17 40 * pdif
P 17 57 * pdif
W 14 37 20 60
S 3 V pdif P 137 P 138
P 14 38 * ptrans
P 14 59 * ptrans
W 13 37 15 60
S 1 V ptrans P 139 P 140
P 11 38 * ptrans
P 11 59 * ptrans
W 10 37 12 60
S 1 V ptrans P 141 P 142
P 32 47 * poly
P 32 21 * poly
W 31 20 33 48
S 1 V poly P 143 P 144
E
