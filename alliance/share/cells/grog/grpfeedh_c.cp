#cell1 grpfeedh_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 141 68
B 0 0 141 68
F F
C vss 0 27 metal2 8 1 * * W vss
C vdd 0 9 metal2 8 2 * * W vdd
C vss 0 27 metal2 8 3 * * W vss
C vdd 0 9 metal2 8 4 * * W vdd
C vss 78 0 metal 8 5 * * S vss
C vdd 39 0 metal 7 6 * * S vdd
C vss 141 49 metal2 8 7 * * E vss
C vdd 141 39 metal2 8 8 * * E vdd
C vss 78 68 metal 4 9 * * N vss
C vdd 39 68 metal 7 10 * * N vdd
C vss 3 68 metal 3 11 * * N vss
I via1 78 47 0 "cvia" ly * *
I via2 39 35 0 "cvia" ly * *
I via3 86 3 0 "cvia" ly * *
I via4 70 3 0 "cvia" ly * *
I cbn1 136 2 0 "cbn" ly * *
I cbn2 136 7 0 "cbn" ly * *
I cbn3 136 11 0 "cbn" ly * *
I cbn4 136 15 0 "cbn" ly * *
I cbn5 136 19 0 "cbn" ly * *
I cbn6 136 23 0 "cbn" ly * *
I cbn7 136 27 0 "cbn" ly * *
I via5 136 39 0 "cvia" ly * *
I cbn8 132 3 0 "cbn" ly * *
I cbn9 128 3 0 "cbn" ly * *
I cbn10 124 3 0 "cbn" ly * *
I cbn11 120 3 0 "cbn" ly * *
I cbn12 116 3 0 "cbn" ly * *
I cbn13 112 3 0 "cbn" ly * *
I cbn14 108 3 0 "cbn" ly * *
I cbn15 104 3 0 "cbn" ly * *
I cbn16 100 3 0 "cbn" ly * *
I cbn17 96 3 0 "cbn" ly * *
I cbn18 92 3 0 "cbn" ly * *
I cbn19 45 3 0 "cbn" ly * *
I cbn20 49 3 0 "cbn" ly * *
I cbn21 53 3 0 "cbn" ly * *
I cbn22 57 3 0 "cbn" ly * *
I cbn23 61 3 0 "cbn" ly * *
I cbn24 65 3 0 "cbn" ly * *
I cbn25 41 3 0 "cbn" ly * *
I cbn26 41 15 0 "cbn" ly * *
I cbn27 41 19 0 "cbn" ly * *
I cbn28 41 23 0 "cbn" ly * *
I cbn29 41 27 0 "cbn" ly * *
I cbp1 136 49 0 "cbp" ly * *
I cbp2 136 54 0 "cbp" ly * *
I cbp3 136 59 0 "cbp" ly * *
I cbp4 131 49 0 "cbp" ly * *
I cbp5 126 49 0 "cbp" ly * *
I cbp6 121 49 0 "cbp" ly * *
I cbp7 116 49 0 "cbp" ly * *
I cbp8 111 49 0 "cbp" ly * *
I cbp9 106 49 0 "cbp" ly * *
I cbp10 101 49 0 "cbp" ly * *
I cbp11 96 49 0 "cbp" ly * *
I cbp12 91 49 0 "cbp" ly * *
I cbp13 86 49 0 "cbp" ly * *
I cbp14 81 49 0 "cbp" ly * *
I cbp15 75 49 0 "cbp" ly * *
I cbp16 70 49 0 "cbp" ly * *
I cbp17 65 49 0 "cbp" ly * *
I cbp18 60 49 0 "cbp" ly * *
I cbp19 55 49 0 "cbp" ly * *
I cbp20 50 49 0 "cbp" ly * *
I cbp21 50 53 0 "cbp" ly * *
I cbp22 50 57 0 "cbp" ly * *
I cbp23 50 61 0 "cbp" ly * *
I cbp24 78 61 0 "cbp" ly * *
I cbp25 78 57 0 "cbp" ly * *
I via6 78 51 0 "cvia" ly * *
I via7 39 43 0 "cvia" ly * *
I via8 78 31 0 "cvia" ly * *
I via9 78 23 0 "cvia" ly * *
I via10 78 27 0 "cvia" ly * *
I via11 41 7 0 "cvia" ly * *
I via12 41 11 0 "cvia" ly * *
I via13 5 27 0 "cvia" ly * *
I cbp26 10 27 0 "cbp" ly * *
I cbp27 15 27 0 "cbp" ly * *
I cbp28 25 27 0 "cbp" ly * *
I cbp29 29 27 0 "cbp" ly * *
I via14 20 27 0 "cvia" ly * *
I via15 39 39 0 "cvia" ly * *
P 136 2 * ntie
P 136 30 * ntie
W 133 -1 139 33
S 3 V ntie P 1 P 2
P 41 3 * ntie
P 41 29 * ntie
W 37 -1 45 33
S 4 V ntie P 3 P 4
P 38 2 * nwell
P 141 2 * nwell
W 34 -2 145 6
S 4 H nwell P 5 P 6
P 40 39 * metal2
P 141 39 * metal2
W 32 31 149 47 vdd
S 8 H metal2 P 7 P 8
P 3 27 * metal
P 3 68 * metal
W 0 24 6 71 vss
S 3 V metal P 9 P 10
P 3 27 * metal
P 29 27 * metal
W 1 25 31 29 vss
S 2 H metal P 11 P 12
P 39 3 * metal
P 39 68 * metal
W 32 -4 46 75 vdd
S 7 V metal P 13 P 14
P 50 49 * ptie
P 50 62 * ptie
W 47 46 53 65
S 3 V ptie P 15 P 16
P 136 49 * ptie
P 136 62 * ptie
W 133 46 139 65
S 3 V ptie P 17 P 18
P 78 49 * metal
P 78 68 * metal
W 74 45 82 72 vss
S 4 V metal P 19 P 20
P 136 49 * metal
P 136 62 * metal
W 134 47 138 64 vss
S 2 V metal P 21 P 22
P 50 49 * metal
P 50 62 * metal
W 48 47 52 64 vss
S 2 V metal P 23 P 24
P 70 3 * metal2
P 86 3 * metal2
W 66 -1 90 7
S 4 H metal2 P 25 P 26
P 40 9 * metal2
P 42 9 * metal2
W 32 1 50 17
S 8 H metal2 P 27 P 28
P 39 36 * metal2
P 39 42 * metal2
W 36 33 42 45
S 3 V metal2 P 29 P 30
P 78 0 * metal
P 78 49 * metal
W 70 -8 86 57 vss
S 8 V metal P 31 P 32
P 50 49 * metal
P 78 49 * metal
W 48 47 80 51 vss
S 2 H metal P 33 P 34
P 78 49 * metal
P 136 49 * metal
W 76 47 138 51 vss
S 2 H metal P 35 P 36
P 50 49 * ptie
P 136 49 * ptie
W 47 46 139 52
S 3 H ptie P 37 P 38
P 78 50 * ptie
P 78 62 * ptie
W 75 47 81 65
S 3 V ptie P 39 P 40
P 78 22 * metal2
P 78 27 * metal2
W 74 18 82 31
S 4 V metal2 P 41 P 42
P 78 27 * metal2
P 78 32 * metal2
W 74 23 82 36
S 4 V metal2 P 43 P 44
P 40 27 * metal2
P 78 27 * metal2
W 32 19 86 35
S 8 H metal2 P 45 P 46
P 41 3 * ntie
P 86 3 * ntie
W 37 -1 90 7
S 4 H ntie P 47 P 48
P 86 3 * ntie
P 134 3 * ntie
W 82 -1 138 7
S 4 H ntie P 49 P 50
P 136 2 * ntie
P 140 2 * ntie
W 133 -1 143 5
S 3 H ntie P 51 P 52
P 86 2 * metal
P 86 3 * metal
W 84 0 88 5
S 2 V metal P 53 P 54
P 136 2 * metal
P 140 2 * metal
W 134 0 142 4
S 2 H metal P 55 P 56
P 136 2 * metal
P 136 3 * metal
W 134 0 138 5
S 2 V metal P 57 P 58
P 136 3 * metal
P 136 38 * metal
W 134 1 138 40
S 2 V metal P 59 P 60
P 136 38 * metal
P 136 39 * metal
W 134 36 138 41
S 2 V metal P 61 P 62
P 86 3 * metal
P 136 3 * metal
W 82 -1 140 7
S 4 H metal P 63 P 64
P 39 3 * metal
P 70 3 * metal
W 35 -1 74 7 vdd
S 4 H metal P 65 P 66
P 39 0 * metal
P 39 3 * metal
W 32 -7 46 10 vdd
S 7 V metal P 67 P 68
P 38 16 * nwell
P 141 16 * nwell
W 8 -14 171 46
S 30 H nwell P 69 P 70
P 77 49 * metal2
P 141 49 * metal2
W 69 41 149 57
S 8 H metal2 P 71 P 72
P 4 27 * ptie
P 30 27 * ptie
W 1 24 33 30
S 3 H ptie P 73 P 74
P 0 27 * metal2
P 40 27 * metal2
W -8 19 48 35
S 8 H metal2 P 75 P 76
P 0 9 * metal2
P 40 9 * metal2
W -8 1 48 17
S 8 H metal2 P 77 P 78
P 0 27 * metal2
P 40 27 * metal2
W -8 19 48 35
S 8 H metal2 P 79 P 80
P 0 9 * metal2
P 40 9 * metal2
W -8 1 48 17
S 8 H metal2 P 81 P 82
E
