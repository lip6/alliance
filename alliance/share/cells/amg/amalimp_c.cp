#cell1 amalimp_c idps_symb compose *
# 21-Feb-95 12:25 21-Feb-95 12:25 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 2 5 25 65
B 2 5 25 65
F F
C cout 25 23 metal2 2 1 * * E cout
C cout 2 23 metal2 2 2 * * W cout
C sum 25 48 metal2 2 3 * * E sum
C sum 2 48 metal2 2 4 * * W sum
C ai 25 38 metal2 2 5 * * E ai
C ai 2 38 metal2 2 6 * * W ai
C vdd 19 5 metal 1 7 * * S vdd
C vss 8 65 metal 1 8 * * N vss
C vss 2 5 metal2 2 9 * * W vss
C vdd 2 65 metal2 2 10 * * W vdd
C vdd 25 65 metal2 2 11 * * E vdd
C vss 25 5 metal2 2 12 * * E vss
I cbn1 21 50 0 "cbn" ly * *
I cbn2 17 50 0 "cbn" ly * *
I cbn3 21 46 0 "cbn" ly * *
I cbp1 10 22 0 "cbp" ly * *
I cbp2 6 22 0 "cbp" ly * *
I cbp3 10 26 0 "cbp" ly * *
I cbp4 6 26 0 "cbp" ly * *
I cbn4 17 42 0 "cbn" ly * *
I cbn5 21 42 0 "cbn" ly * *
I cbn6 17 46 0 "cbn" ly * *
I cbp5 10 30 0 "cbp" ly * *
I cbp6 6 30 0 "cbp" ly * *
I via1 22 65 0 "cvia" ly * *
I via2 16 65 0 "cvia" ly * *
I via3 11 5 0 "cvia" ly * *
I via4 5 5 0 "cvia" ly * *
P 2 48 * metal2
P 25 48 * metal2
W 0 46 27 50
S 2 H metal2 P 1 P 2
P 2 38 * metal2
P 25 38 * metal2
W 0 36 27 40
S 2 H metal2 P 3 P 4
P 2 23 * metal2
P 25 23 * metal2
W 0 21 27 25
S 2 H metal2 P 5 P 6
P 8 21 * ptie
P 8 31 * ptie
W 0 13 16 39
S 8 V ptie P 7 P 8
P 19 41 * ntie
P 19 51 * ntie
W 11 33 27 59
S 8 V ntie P 9 P 10
P 19 5 * metal
P 19 65 * metal
W 11 -3 27 73
S 8 V metal P 11 P 12
P 8 5 * metal
P 8 65 * metal
W 0 -3 16 73
S 8 V metal P 13 P 14
P 2 52 * nwell
P 25 52 * nwell
W -24 26 51 78
S 26 H nwell P 15 P 16
P 2 65 * metal2
P 25 65 * metal2
W -4 59 31 71
S 6 H metal2 P 17 P 18
P 2 5 * metal2
P 25 5 * metal2
W -6 -3 33 13
S 8 H metal2 P 19 P 20
E
