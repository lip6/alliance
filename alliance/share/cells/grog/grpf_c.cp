#cell1 grpf_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 35 6 44 87
B 35 6 44 87
F F
C vss 35 42 metal2 8 1 * * W vss
C vss 44 42 metal2 8 2 * * E vss
C vdd 35 32 metal2 8 3 * * W vdd
C vdd 44 32 metal2 8 4 * * E vdd
I cbn1 40 25 0 "cbn" ly * *
I via1 40 32 0 "cvia" ly * *
P 35 32 * metal2
P 44 32 * metal2
W 27 24 52 40
S 8 H metal2 P 1 P 2
P 35 42 * metal2
P 44 42 * metal2
W 27 34 52 50
S 8 H metal2 P 3 P 4
P 39 20 * nwell
P 39 87 * nwell
W 29 10 49 97
S 10 V nwell P 5 P 6
P 39 21 * ntie
P 39 34 * ntie
W 30 12 48 43
S 9 V ntie P 7 P 8
P 40 25 * metal
P 40 32 * metal
W 39 24 41 33
S 1 V metal P 9 P 10
E
