#cell1 nmx2sc_fp idps_symb compose *
# 19-Nov-97 18:56 19-Nov-97 18:56 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 29 65
B 5 5 29 65
F F
C s0i0 12 5 metal 1 1 * * S s0i0
C s1i0 22 5 metal 1 2 * * S s1i0
C s1i0 22 65 metal 1 3 * * N s1i0
C s0i0 12 65 metal 1 4 * * N s0i0
C vss 5 5 metal2 8 5 * * W vss
C vss 29 5 metal2 8 6 * * E vss
C vdd 5 65 metal2 6 7 * * W vdd
C vdd 29 65 metal2 6 8 * * E vdd
C s0i0 12 5 metal 1 9 * * S s0i0
C s1i0 22 5 metal 1 10 * * S s1i0
C s1i0 22 65 metal 1 11 * * N s1i0
C s0i0 12 65 metal 1 12 * * N s0i0
C vss 5 5 metal2 8 13 * * W vss
C vss 29 5 metal2 8 14 * * E vss
C vdd 5 65 metal2 6 15 * * W vdd
C vdd 29 65 metal2 6 16 * * E vdd
I cdn1 17 18 0 "cdn" ly * *
I cdn2 26 17 0 "cdn" ly * *
I cdn3 8 17 0 "cdn" ly * *
I cdp1 17 48 0 "cdp" ly * *
I cdp2 26 53 0 "cdp" ly * *
I cdp3 8 53 0 "cdp" ly * *
I cbn1 26 62 0 "cbn" ly * *
I cbn2 8 62 0 "cbn" ly * *
I cbp1 26 8 0 "cbp" ly * *
I cbp2 8 8 0 "cbp" ly * *
I cpf1 13 5 0 "cpf" ly * *
I cpf2 21 5 0 "cpf" ly * *
I cpf3 16 60 0 "cpf" ly * *
I cpf4 26 33 0 "cpf" ly * *
I cpf5 8 28 0 "cpf" ly * *
I cpf6 16 65 0 "cpf" ly * *
I via1 8 5 0 "cvia" ly * *
I via2 26 5 0 "cvia" ly * *
I via3 26 65 0 "cvia" ly * *
I via4 8 65 0 "cvia" ly * *
I i0_3 8 43 0 "ref_con" ly * *
I i1_3 26 43 0 "ref_con" ly * *
I i1_7 26 23 0 "ref_con" ly * *
I i0_5 8 33 0 "ref_con" ly * *
I i1_6 26 28 0 "ref_con" ly * *
I i0_4 8 38 0 "ref_con" ly * *
I i1_4 26 38 0 "ref_con" ly * *
I o_4 17 38 0 "ref_con" ly * *
I o_1 17 53 0 "ref_con" ly * *
I o_6 17 28 0 "ref_con" ly * *
I o_3 17 43 0 "ref_con" ly * *
I o_5 17 33 0 "ref_con" ly * *
I i1_2 26 48 0 "ref_con" ly * *
I i0_2 8 48 0 "ref_con" ly * *
I nwell_28 17 53 0 "ref_ref" ly * *
I o_7 17 23 0 "ref_con" ly * *
I i0_7 8 23 0 "ref_con" ly * *
I o_9 17 13 0 "ref_con" ly * *
P 5 53 * nwell
P 29 53 * nwell
W -23 25 57 81
S 28 H nwell P 1 P 2
P 11 11 * ntrans
P 11 23 * ntrans
W 10 10 12 24
S 1 V ntrans P 3 P 4
P 8 13 * ndif
P 8 21 * ndif
W 5 10 11 24
S 3 V ndif P 5 P 6
P 17 13 * ndif
P 17 21 * ndif
W 14 10 20 24
S 3 V ndif P 7 P 8
P 26 13 * ndif
P 26 21 * ndif
W 23 10 29 24
S 3 V ndif P 9 P 10
P 23 11 * ntrans
P 23 23 * ntrans
W 22 10 24 24
S 1 V ntrans P 11 P 12
P 20 11 * ntrans
P 20 23 * ntrans
W 19 10 21 24
S 1 V ntrans P 13 P 14
P 14 11 * ntrans
P 14 23 * ntrans
W 13 10 15 24
S 1 V ntrans P 15 P 16
P 17 40 * pdif
P 17 57 * pdif
W 14 37 20 60
S 3 V pdif P 17 P 18
P 26 40 * pdif
P 26 57 * pdif
W 23 37 29 60
S 3 V pdif P 19 P 20
P 8 40 * pdif
P 8 57 * pdif
W 5 37 11 60
S 3 V pdif P 21 P 22
P 11 38 * ptrans
P 11 59 * ptrans
W 10 37 12 60
S 1 V ptrans P 23 P 24
P 14 38 * ptrans
P 14 59 * ptrans
W 13 37 15 60
S 1 V ptrans P 25 P 26
P 20 38 * ptrans
P 20 59 * ptrans
W 19 37 21 60
S 1 V ptrans P 27 P 28
P 23 38 * ptrans
P 23 59 * ptrans
W 22 37 24 60
S 1 V ptrans P 29 P 30
P 14 5 * poly
P 14 11 * poly
W 13 4 15 12
S 1 V poly P 31 P 32
P 13 5 * poly
P 14 5 * poly
W 12 4 15 6
S 1 H poly P 33 P 34
P 20 5 * poly
P 20 11 * poly
W 19 4 21 12
S 1 V poly P 35 P 36
P 20 5 * poly
P 21 5 * poly
W 19 4 22 6
S 1 H poly P 37 P 38
P 20 59 * poly
P 20 65 * poly
W 19 58 21 66
S 1 V poly P 39 P 40
P 16 65 * poly
P 20 65 * poly
W 15 64 21 66
S 1 H poly P 41 P 42
P 14 59 * poly
P 16 59 * poly
W 13 58 17 60
S 1 H poly P 43 P 44
P 16 59 * poly
P 16 60 * poly
W 15 58 17 61
S 1 V poly P 45 P 46
P 11 28 * poly
P 11 38 * poly
W 10 27 12 39
S 1 V poly P 47 P 48
P 11 23 * poly
P 11 28 * poly
W 10 22 12 29
S 1 V poly P 49 P 50
P 8 28 * poly
P 11 28 * poly
W 5 25 14 31
S 3 H poly P 51 P 52
P 23 33 * poly
P 23 38 * poly
W 22 32 24 39
S 1 V poly P 53 P 54
P 23 23 * poly
P 23 33 * poly
W 22 22 24 34
S 1 V poly P 55 P 56
P 23 33 * poly
P 26 33 * poly
W 20 30 29 36
S 3 H poly P 57 P 58
P 8 5 * metal
P 8 17 * metal
W 6 3 10 19
S 2 V metal P 59 P 60
P 26 5 * metal
P 26 17 * metal
W 24 3 28 19
S 2 V metal P 61 P 62
P 12 5 * metal
P 13 5 * metal
W 11 4 14 6 s0i0
S 1 H metal P 63 P 64
P 12 5 * metal
P 12 65 * metal
W 11 4 13 66 s0i0
S 1 V metal P 65 P 66
P 22 5 * metal
P 22 60 * metal
W 21 4 23 61 s1i0
S 1 V metal P 67 P 68
P 21 5 * metal
P 22 5 * metal
W 20 4 23 6 s1i0
S 1 H metal P 69 P 70
P 8 23 * metal
P 8 48 * metal
W 7 22 9 49 i0
S 1 V metal P 71 P 72
P 22 60 * metal
P 22 65 * metal
W 21 59 23 66 s1i0
S 1 V metal P 73 P 74
P 16 60 * metal
P 22 60 * metal
W 15 59 23 61 s1i0
S 1 H metal P 75 P 76
P 26 23 * metal
P 26 48 * metal
W 25 22 27 49 i1
S 1 V metal P 77 P 78
P 8 53 * metal
P 8 65 * metal
W 6 51 10 67
S 2 V metal P 79 P 80
P 26 53 * metal
P 26 65 * metal
W 24 51 28 67
S 2 V metal P 81 P 82
P 12 65 * metal
P 16 65 * metal
W 11 64 17 66 s0i0
S 1 H metal P 83 P 84
P 17 13 * metal
P 17 53 * metal
W 16 12 18 54 o
S 1 V metal P 85 P 86
P 5 5 * metal2
P 29 5 * metal2
W -3 -3 37 13
S 8 H metal2 P 87 P 88
P 5 65 * metal2
P 29 65 * metal2
W -1 59 35 71
S 6 H metal2 P 89 P 90
P 5 53 * allowM2
P 29 53 * allowM2
W 3 51 31 55
S 2 H allowM2 P 91 P 92
P 5 58 * allowM2
P 29 58 * allowM2
W 3 56 31 60
S 2 H allowM2 P 93 P 94
P 5 13 * allowM2
P 29 13 * allowM2
W 3 11 31 15
S 2 H allowM2 P 95 P 96
P 5 18 * allowM2
P 29 18 * allowM2
W 3 16 31 20
S 2 H allowM2 P 97 P 98
P 5 23 * allowM2
P 29 23 * allowM2
W 3 21 31 25
S 2 H allowM2 P 99 P 100
P 5 28 * allowM2
P 29 28 * allowM2
W 3 26 31 30
S 2 H allowM2 P 101 P 102
P 5 33 * allowM2
P 29 33 * allowM2
W 3 31 31 35
S 2 H allowM2 P 103 P 104
P 5 38 * allowM2
P 29 38 * allowM2
W 3 36 31 40
S 2 H allowM2 P 105 P 106
P 5 43 * allowM2
P 29 43 * allowM2
W 3 41 31 45
S 2 H allowM2 P 107 P 108
P 5 48 * allowM2
P 29 48 * allowM2
W 3 46 31 50
S 2 H allowM2 P 109 P 110
E
