#cell1 gruwi_c idps_symb compose *
# 27-Mar-95 11:35 27-Mar-95 11:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 925 79 1066 104
B 925 79 1066 104
F F
C vss 1003 79 metal 4 1 * * S vss
C vss 1003 104 metal 4 2 * * N vss
C vdd 964 79 metal 7 3 * * S vdd
C vdd 964 104 metal 7 4 * * N vdd
C vss 928 104 metal 3 5 * * N vss
C s0 1066 95 metal2 2 6 * * E s0
C s1 1066 81 metal2 2 7 * * E s1
C vdd 1066 101 metal2 4 8 * * E vdd
C vss 1066 88 metal2 7 9 * * E vss
C vdd 925 101 metal2 4 10 * * W vdd
C s0 925 95 metal2 2 11 * * W s0
C s1 925 81 metal2 2 12 * * W s1
C vss 925 88 metal2 7 13 * * W vss
I via1 1003 86 0 "cvia" ly * *
I via2 1003 90 0 "cvia" ly * *
I via3 966 101 0 "cvia" ly * *
I via4 962 101 0 "cvia" ly * *
I via5 928 90 0 "cvia" ly * *
I via6 928 86 0 "cvia" ly * *
P 925 101 * metal2
P 1066 101 * metal2
W 921 97 1070 105
S 4 H metal2 P 1 P 2
P 925 95 * metal2
P 1066 95 * metal2
W 923 93 1068 97 s0
S 2 H metal2 P 3 P 4
P 925 88 * metal2
P 1066 88 * metal2
W 918 81 1073 95
S 7 H metal2 P 5 P 6
P 1003 79 * metal
P 1003 104 * metal
W 999 75 1007 108 vss
S 4 V metal P 7 P 8
P 964 79 * metal
P 964 104 * metal
W 957 72 971 111
S 7 V metal P 9 P 10
P 928 85 * metal
P 928 104 * metal
W 925 82 931 107
S 3 V metal P 11 P 12
P 925 81 * metal2
P 1066 81 * metal2
W 923 79 1068 83 s1
S 2 H metal2 P 13 P 14
E
