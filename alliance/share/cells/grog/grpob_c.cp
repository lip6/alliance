#cell1 grpob_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 17 -7 43 74
B 17 -7 43 74
F F
C vss 17 29 metal2 8 1 * * W vss
C vss 43 29 metal2 8 2 * * E vss
C vdd 43 19 metal2 2 3 * * E vdd
C vdd 17 19 metal2 8 4 * * W vdd
C f 32 -7 metal 1 5 * * S f
C vss 43 74 metal 2 6 * * E vss
C vdd 24 74 metal 1 7 * * N vdd
C i 37 74 metal 1 8 * * N i
I cbp1 43 18 0 "cbp" ly * *
I cbp2 37 18 0 "cbp" ly * *
I cbp3 43 22 0 "cbp" ly * *
I cbp4 37 22 0 "cbp" ly * *
I via1 37 27 0 "cvia" ly * *
I via2 43 27 0 "cvia" ly * *
I via3 43 31 0 "cvia" ly * *
I via4 37 31 0 "cvia" ly * *
I cbp5 43 10 0 "cbp" ly * *
I cbp6 37 10 0 "cbp" ly * *
I cbp7 37 14 0 "cbp" ly * *
I cbp8 43 14 0 "cbp" ly * *
I cbp9 43 34 0 "cbp" ly * *
I cbp10 37 34 0 "cbp" ly * *
I cbp11 37 38 0 "cbp" ly * *
I cbp12 43 38 0 "cbp" ly * *
I via5 20 17 0 "cvia" ly * *
I via6 26 17 0 "cvia" ly * *
I via7 26 21 0 "cvia" ly * *
I via8 20 21 0 "cvia" ly * *
I cbn1 26 9 0 "cbn" ly * *
I cbn2 20 9 0 "cbn" ly * *
I cbn3 26 13 0 "cbn" ly * *
I cbn4 20 13 0 "cbn" ly * *
I cdp1 25 32 0 "cdp" ly * *
I cdp2 25 38 0 "cdp" ly * *
I cdp3 25 50 0 "cdp" ly * *
I cdp4 25 56 0 "cdp" ly * *
I cdn1 38 50 0 "cdn" ly * *
I cdn2 38 56 0 "cdn" ly * *
I cbn5 22 68 0 "cbn" ly * *
I cdp5 20 26 0 "cdp" ly * *
I cdp6 26 26 0 "cdp" ly * *
I cdp7 26 62 0 "cdp" ly * *
I cdp8 20 62 0 "cdp" ly * *
I cdp9 20 44 0 "cdp" ly * *
I cdp10 26 44 0 "cdp" ly * *
I cdn3 37 44 0 "cdn" ly * *
I cdn4 43 44 0 "cdn" ly * *
I cdn5 43 62 0 "cdn" ly * *
I cdn6 37 62 0 "cdn" ly * *
I cxp1 21 29 0 "cxp" ly * *
I cxp2 21 41 0 "cxp" ly * *
I cxp3 21 47 0 "cxp" ly * *
I cxp4 21 59 0 "cxp" ly * *
I cxn1 41 47 0 "cxn" ly * *
I cxn2 41 59 0 "cxn" ly * *
I cbn6 26 68 0 "cbn" ly * *
I cbn7 18 68 0 "cbn" ly * *
I cbp13 43 68 0 "cbp" ly * *
I cbp14 37 68 0 "cbp" ly * *
I cpf1 37 73 0 "cpf" ly * *
P 36 62 * ndif
P 44 62 * ndif
W 33 59 47 65
S 3 H ndif P 1 P 2
P 36 44 * ndif
P 44 44 * ndif
W 33 41 47 47
S 3 H ndif P 3 P 4
P 22 7 * nwell
P 22 72 * nwell
W 10 -5 34 84
S 12 V nwell P 5 P 6
P 20 68 * ntie
P 26 68 * ntie
W 17 65 29 71
S 3 H ntie P 7 P 8
P 37 68 * ptie
P 43 68 * ptie
W 34 65 46 71
S 3 H ptie P 9 P 10
P 41 47 * ntrans
P 41 59 * ntrans
W 40 46 42 60
S 1 V ntrans P 11 P 12
P 34 59 * ntrans
P 41 59 * ntrans
W 33 58 42 60
S 1 H ntrans P 13 P 14
P 34 47 * ntrans
P 41 47 * ntrans
W 33 46 42 48
S 1 H ntrans P 15 P 16
P 21 47 * ptrans
P 21 59 * ptrans
W 20 46 22 60
S 1 V ptrans P 17 P 18
P 21 59 * ptrans
P 29 59 * ptrans
W 20 58 30 60
S 1 H ptrans P 19 P 20
P 21 47 * ptrans
P 29 47 * ptrans
W 20 46 30 48
S 1 H ptrans P 21 P 22
P 19 62 * pdif
P 27 62 * pdif
W 16 59 30 65
S 3 H pdif P 23 P 24
P 19 44 * pdif
P 27 44 * pdif
W 16 41 30 47
S 3 H pdif P 25 P 26
P 21 29 * ptrans
P 21 41 * ptrans
W 20 28 22 42
S 1 V ptrans P 27 P 28
P 21 41 * ptrans
P 29 41 * ptrans
W 20 40 30 42
S 1 H ptrans P 29 P 30
P 21 29 * ptrans
P 29 29 * ptrans
W 20 28 30 30
S 1 H ptrans P 31 P 32
P 19 26 * pdif
P 26 26 * pdif
W 16 23 29 29
S 3 H pdif P 33 P 34
P 37 49 * ndif
P 37 57 * ndif
W 34 46 40 60
S 3 V ndif P 35 P 36
P 25 49 * pdif
P 25 57 * pdif
W 20 44 30 62
S 5 V pdif P 37 P 38
P 25 31 * pdif
P 25 39 * pdif
W 20 26 30 44
S 5 V pdif P 39 P 40
P 17 29 * metal2
P 44 29 * metal2
W 9 21 52 37
S 8 H metal2 P 41 P 42
P 22 8 * ntie
P 22 21 * ntie
W 11 -3 33 32
S 11 V ntie P 43 P 44
P 17 19 * metal2
P 44 19 * metal2
W 9 11 52 27
S 8 H metal2 P 45 P 46
P 40 8 * ptie
P 40 39 * ptie
W 31 -1 49 48
S 9 V ptie P 47 P 48
P 40 8 * metal
P 40 45 * metal
W 32 0 48 53
S 8 V metal P 49 P 50
P 34 47 * poly
P 34 59 * poly
W 33 46 35 60
S 1 V poly P 51 P 52
P 34 59 * poly
P 34 72 * poly
W 33 58 35 73
S 1 V poly P 53 P 54
P 34 72 * poly
P 37 72 * poly
W 33 71 38 73
S 1 H poly P 55 P 56
P 37 72 * poly
P 37 73 * poly
W 36 71 38 74
S 1 V poly P 57 P 58
P 29 59 * poly
P 34 59 * poly
W 28 58 35 60
S 1 H poly P 59 P 60
P 29 47 * poly
P 29 59 * poly
W 28 46 30 60
S 1 V poly P 61 P 62
P 29 29 * poly
P 29 47 * poly
W 28 28 30 48
S 1 V poly P 63 P 64
P 29 47 * poly
P 34 47 * poly
W 28 46 35 48
S 1 H poly P 65 P 66
P 32 -7 * metal
P 32 38 * metal
W 31 -8 33 39
S 1 V metal P 67 P 68
P 32 50 * metal
P 38 50 * metal
W 31 49 39 51
S 1 H metal P 69 P 70
P 25 50 * metal
P 32 50 * metal
W 24 49 33 51
S 1 H metal P 71 P 72
P 32 38 * metal
P 32 50 * metal
W 31 37 33 51
S 1 V metal P 73 P 74
P 25 38 * metal
P 32 38 * metal
W 24 37 33 39
S 1 H metal P 75 P 76
P 25 32 * metal
P 25 38 * metal
W 24 31 26 39
S 1 V metal P 77 P 78
P 32 50 * metal
P 32 56 * metal
W 31 49 33 57
S 1 V metal P 79 P 80
P 25 56 * metal
P 32 56 * metal
W 24 55 33 57
S 1 H metal P 81 P 82
P 32 56 * metal
P 38 56 * metal
W 31 55 39 57
S 1 H metal P 83 P 84
P 37 73 * metal
P 37 74 * metal
W 36 72 38 75 i
S 1 V metal P 85 P 86
P 19 68 * metal
P 19 74 * metal
W 17 66 21 76
S 2 V metal P 87 P 88
P 19 68 * metal
P 26 68 * metal
W 17 66 28 70
S 2 H metal P 89 P 90
P 18 68 * metal
P 19 68 * metal
W 16 66 21 70
S 2 H metal P 91 P 92
P 19 62 * metal
P 19 68 * metal
W 17 60 21 70
S 2 V metal P 93 P 94
P 19 62 * metal
P 26 62 * metal
W 17 60 28 64
S 2 H metal P 95 P 96
P 19 44 * metal
P 19 62 * metal
W 17 42 21 64
S 2 V metal P 97 P 98
P 19 44 * metal
P 26 44 * metal
W 17 42 28 46
S 2 H metal P 99 P 100
P 19 8 * metal
P 19 44 * metal
W 17 6 21 46
S 2 V metal P 101 P 102
P 19 74 * metal
P 26 74 * metal
W 17 72 28 76
S 2 H metal P 103 P 104
P 43 68 * metal
P 43 74 * metal
W 41 66 45 76
S 2 V metal P 105 P 106
P 37 68 * metal
P 43 68 * metal
W 35 66 45 70
S 2 H metal P 107 P 108
P 43 62 * metal
P 43 68 * metal
W 41 60 45 70
S 2 V metal P 109 P 110
P 37 62 * metal
P 43 62 * metal
W 35 60 45 64
S 2 H metal P 111 P 112
P 43 8 * metal
P 43 62 * metal
W 41 6 45 64
S 2 V metal P 113 P 114
P 23 8 * metal
P 23 27 * metal
W 14 -1 32 36
S 9 V metal P 115 P 116
E
