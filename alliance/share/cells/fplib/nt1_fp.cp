#cell1 nt1_fp idps_symb compose *
# 20-Nov-97 11:22 20-Nov-97 11:22 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 5 5 29 65
B 5 5 29 65
F F
C en 12 5 metal 1 1 * * S en
C en 12 65 metal 1 2 * * N en
C vdd 5 65 metal2 6 3 * * W vdd
C vss 5 5 metal2 8 4 * * W vss
C vdd 29 65 metal2 6 5 * * E vdd
C vss 29 5 metal2 8 6 * * E vss
C enb 22 5 metal 1 7 * * S enb
C enb 22 65 metal 1 8 * * N enb
I cpf1 8 33 0 "cpf" ly * *
I cdn1 20 18 0 "cdn" ly * *
I cpf2 13 5 0 "cpf" ly * *
I cdp1 20 43 0 "cdp" ly * *
I cpf3 16 65 0 "cpf" ly * *
I cdn2 8 14 0 "cdn" ly * *
I cdn3 8 18 0 "cdn" ly * *
I via1 8 5 0 "cvia" ly * *
I cdp2 20 53 0 "cdp" ly * *
I cdp3 8 53 0 "cdp" ly * *
I via2 8 65 0 "cvia" ly * *
I cbn1 8 62 0 "cbn" ly * *
I cbp1 8 8 0 "cbp" ly * *
I o_8 25 18 0 "ref_con" ly * *
I o_3 25 43 0 "ref_con" ly * *
I o_1 25 53 0 "ref_con" ly * *
I nwell_28 14 53 0 "ref_ref" ly * *
I i_4 8 38 0 "ref_con" ly * *
I i_2 8 48 0 "ref_con" ly * *
I o_2 20 48 0 "ref_con" ly * *
I o_9 20 13 0 "ref_con" ly * *
I o_7 20 23 0 "ref_con" ly * *
I i_7 8 23 0 "ref_con" ly * *
I i_3 8 43 0 "ref_con" ly * *
I o_4 20 38 0 "ref_con" ly * *
I o_5 20 33 0 "ref_con" ly * *
I o_0 20 58 0 "ref_con" ly * *
I i_6 8 28 0 "ref_con" ly * *
I o_6 20 28 0 "ref_con" ly * *
P 13 5 * poly
P 14 5 * poly
W 12 4 15 6
S 1 H poly P 1 P 2
P 14 11 * poly
P 17 11 * poly
W 13 10 18 12
S 1 H poly P 3 P 4
P 14 5 * poly
P 14 11 * poly
W 13 4 15 12
S 1 V poly P 5 P 6
P 20 18 * metal
P 25 18 * metal
W 19 17 26 19
S 1 H metal P 7 P 8
P 20 43 * metal
P 25 43 * metal
W 19 42 26 44
S 1 H metal P 9 P 10
P 20 53 * metal
P 25 53 * metal
W 19 52 26 54
S 1 H metal P 11 P 12
P 11 11 * ntrans
P 11 23 * ntrans
W 10 10 12 24
S 1 V ntrans P 13 P 14
P 17 11 * ntrans
P 17 23 * ntrans
W 16 10 18 24
S 1 V ntrans P 15 P 16
P 20 13 * ndif
P 20 21 * ndif
W 17 10 23 24
S 3 V ndif P 17 P 18
P 17 38 * ptrans
P 17 59 * ptrans
W 16 37 18 60
S 1 V ptrans P 19 P 20
P 11 38 * ptrans
P 11 59 * ptrans
W 10 37 12 60
S 1 V ptrans P 21 P 22
P 20 40 * pdif
P 20 57 * pdif
W 17 37 23 60
S 3 V pdif P 23 P 24
P 8 40 * pdif
P 8 57 * pdif
W 5 37 11 60
S 3 V pdif P 25 P 26
P 8 53 * metal
P 8 65 * metal
W 6 51 10 67
S 2 V metal P 27 P 28
P 14 13 * ndif
P 14 21 * ndif
W 12 11 16 23
S 2 V ndif P 29 P 30
P 14 40 * pdif
P 14 57 * pdif
W 12 38 16 59
S 2 V pdif P 31 P 32
P 8 13 * ndif
P 8 21 * ndif
W 5 10 11 24
S 3 V ndif P 33 P 34
P 8 5 * metal
P 8 18 * metal
W 6 3 10 20
S 2 V metal P 35 P 36
P 8 23 * metal
P 8 48 * metal
W 7 22 9 49 i
S 1 V metal P 37 P 38
P 20 13 * metal
P 20 58 * metal
W 19 12 21 59 o
S 1 V metal P 39 P 40
P 17 59 * poly
P 17 65 * poly
W 16 58 18 66
S 1 V poly P 41 P 42
P 16 65 * poly
P 17 65 * poly
W 15 64 18 66
S 1 H poly P 43 P 44
P 12 5 * metal
P 13 5 * metal
W 11 4 14 6
S 1 H metal P 45 P 46
P 12 5 * metal
P 12 65 * metal
W 11 4 13 66
S 1 V metal P 47 P 48
P 16 9 * metal
P 16 65 * metal
W 15 8 17 66
S 1 V metal P 49 P 50
P 11 33 * poly
P 11 38 * poly
W 10 32 12 39
S 1 V poly P 51 P 52
P 11 23 * poly
P 11 33 * poly
W 10 22 12 34
S 1 V poly P 53 P 54
P 8 33 * poly
P 11 33 * poly
W 5 30 14 36
S 3 H poly P 55 P 56
P 5 5 * metal2
P 29 5 * metal2
W -3 -3 37 13
S 8 H metal2 P 57 P 58
P 5 13 * allowM2
P 29 13 * allowM2
W 3 11 31 15
S 2 H allowM2 P 59 P 60
P 5 18 * allowM2
P 29 18 * allowM2
W 3 16 31 20
S 2 H allowM2 P 61 P 62
P 5 23 * allowM2
P 29 23 * allowM2
W 3 21 31 25
S 2 H allowM2 P 63 P 64
P 5 28 * allowM2
P 29 28 * allowM2
W 3 26 31 30
S 2 H allowM2 P 65 P 66
P 5 33 * allowM2
P 29 33 * allowM2
W 3 31 31 35
S 2 H allowM2 P 67 P 68
P 5 38 * allowM2
P 29 38 * allowM2
W 3 36 31 40
S 2 H allowM2 P 69 P 70
P 5 43 * allowM2
P 29 43 * allowM2
W 3 41 31 45
S 2 H allowM2 P 71 P 72
P 5 48 * allowM2
P 29 48 * allowM2
W 3 46 31 50
S 2 H allowM2 P 73 P 74
P 5 53 * allowM2
P 29 53 * allowM2
W 3 51 31 55
S 2 H allowM2 P 75 P 76
P 5 58 * allowM2
P 29 58 * allowM2
W 3 56 31 60
S 2 H allowM2 P 77 P 78
P 5 53 * nwell
P 29 53 * nwell
W -23 25 57 81
S 28 H nwell P 79 P 80
P 5 65 * metal2
P 29 65 * metal2
W -1 59 35 71
S 6 H metal2 P 81 P 82
P 16 9 * metal
P 22 9 * metal
W 15 8 23 10
S 1 H metal P 83 P 84
P 22 5 * metal
P 22 9 * metal
W 21 4 23 10
S 1 V metal P 85 P 86
P 16 65 * metal
P 22 65 * metal
W 15 64 23 66
S 1 H metal P 87 P 88
E
