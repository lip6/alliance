#cell1 grp4_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 0 0 26 20
B 0 0 26 20
F F
C vdd 12 20 metal 19 1 * * N vdd
C ck_06 0 6 metal2 2 2 * * W ck_06
C bl0_p 3 0 metal 1 3 * * S bl0_p
C ck_06 26 6 metal2 2 4 * * E ck_06
C vss 26 0 metal 3 5 * * S vss
C bl1_p 9 0 metal 1 6 * * S bl1_p
C bl2_p 15 0 metal 1 7 * * S bl2_p
C bl3_p 21 0 metal 1 8 * * S bl3_p
C vss 26 20 metal 3 9 * * N vss
C vdd 0 15 metal2 10 10 * * W vdd
C vdd 26 15 metal2 10 11 * * E vdd
C ck_06p 26 6 poly 1 12 * * E ck_06p
I via1 4 17 0 "cvia" ly * *
I via2 9 17 0 "cvia" ly * *
I via3 15 17 0 "cvia" ly * *
I via4 20 17 0 "cvia" ly * *
I cdn1 3 9 0 "cdn" ly * *
I cdn2 3 3 0 "cdn" ly * *
I via5 4 12 0 "cvia" ly * *
I cdn3 9 9 0 "cdn" ly * *
I cdn4 9 3 0 "cdn" ly * *
I cdn5 15 9 0 "cdn" ly * *
I cdn6 15 3 0 "cdn" ly * *
I cdn7 21 9 0 "cdn" ly * *
I cdn8 21 3 0 "cdn" ly * *
I via6 20 12 0 "cvia" ly * *
I cbp1 26 15 0 "cbp" ly * *
I via7 15 12 0 "cvia" ly * *
I via8 9 12 0 "cvia" ly * *
P 21 0 * metal
P 21 3 * metal
W 20 -1 22 4 s1
S 1 V metal P 1 P 2
P 9 0 * metal
P 9 3 * metal
W 8 -1 10 4 s3
S 1 V metal P 3 P 4
P 0 15 * metal2
P 26 15 * metal2
W -10 5 36 25 vdd
S 10 H metal2 P 5 P 6
P 26 0 * metal
P 26 20 * metal
W 23 -3 29 23 vss
S 3 V metal P 7 P 8
P 0 15 * ptie
P 26 15 * ptie
W -2 13 28 17
S 2 H ptie P 9 P 10
P 24 6 * poly
P 26 6 * poly
W 23 5 27 7
S 1 H poly P 11 P 12
P 18 6 * ntrans
P 24 6 * ntrans
W 17 5 25 7
S 1 H ntrans P 13 P 14
P 12 6 * ntrans
P 18 6 * ntrans
W 11 5 19 7
S 1 H ntrans P 15 P 16
P 6 6 * ntrans
P 12 6 * ntrans
W 5 5 13 7
S 1 H ntrans P 17 P 18
P 0 6 * ntrans
P 6 6 * ntrans
W -1 5 7 7
S 1 H ntrans P 19 P 20
P 0 6 * metal2
P 26 6 * metal2
W -2 4 28 8
S 2 H metal2 P 21 P 22
P 3 0 * metal
P 3 3 * metal
W 2 -1 4 4
S 1 V metal P 23 P 24
P 15 0 * metal
P 15 3 * metal
W 14 -1 16 4 s2
S 1 V metal P 25 P 26
P 3 14 * metal
P 21 14 * metal
W -10 1 34 27 vdd
S 13 H metal P 27 P 28
E
