#cell1 grpubob_c idps_symb compose *
# 27-Mar-95 11:35 27-Mar-95 11:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 35 6 42 87
B 35 6 42 87
F F
C vss 37 87 metal 3 1 * * N vss
C vss 35 42 metal2 8 2 * * W vss
C vss 42 42 metal2 8 3 * * E vss
C vdd 35 32 metal2 8 4 * * W vdd
C vdd 42 32 metal2 8 5 * * E vdd
I via1 37 44 0 "cvia" ly * *
I via2 37 40 0 "cvia" ly * *
P 35 32 * metal2
P 42 32 * metal2
W 27 24 50 40
S 8 H metal2 P 1 P 2
P 35 42 * metal2
P 42 42 * metal2
W 27 34 50 50
S 8 H metal2 P 3 P 4
P 37 39 * metal
P 37 87 * metal
W 34 36 40 90
S 3 V metal P 5 P 6
E
