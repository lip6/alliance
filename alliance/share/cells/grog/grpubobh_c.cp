#cell1 grpubobh_c idps_symb compose *
# 27-Mar-95 11:35 27-Mar-95 11:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 35 15 42 96
B 35 15 42 96
F F
C vss 37 96 metal 2 1 * * N vss
C vss 35 42 metal2 8 2 * * W vss
C vss 42 42 metal2 8 3 * * E vss
C vdd 35 24 metal2 8 4 * * W vdd
C vdd 42 24 metal2 8 5 * * E vdd
I via1 37 44 0 "cvia" ly * *
I via2 37 40 0 "cvia" ly * *
P 35 24 * metal2
P 42 24 * metal2
W 27 16 50 32
S 8 H metal2 P 1 P 2
P 35 42 * metal2
P 42 42 * metal2
W 27 34 50 50
S 8 H metal2 P 3 P 4
P 37 39 * metal
P 37 96 * metal
W 35 37 39 98
S 2 V metal P 5 P 6
E
