#cell1 gruobfe_c idps_symb compose *
# 27-Mar-95 11:35 27-Mar-95 11:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 203 53 208 158
B 203 53 208 158
F F
C vss 208 129 metal2 7 1 * * E vss
C e0 208 136 metal2 2 2 * * E e0
C vdd 208 142 metal2 4 3 * * E vdd
C e1 208 158 metal2 2 4 * * E e1
C vdd 203 142 metal2 4 5 * * W vdd
C vss 203 129 metal2 7 6 * * W vss
C e0 203 136 metal2 2 7 * * W e0
C e1 203 158 metal2 2 8 * * W e1
C vdd0 208 65 metal2 8 9 * * E vdd0
C vdd0 203 65 metal2 8 10 * * W vdd0
C vss0 208 75 metal2 8 11 * * E vss0
C vss0 203 75 metal2 8 12 * * W vss0
P 206 53 * allowM1
P 206 158 * allowM1
W 205 52 207 159 tr
S 1 V allowM1 P 1 P 2
P 203 158 * metal2
P 208 158 * metal2
W 201 156 210 160 e1
S 2 H metal2 P 3 P 4
P 203 75 * metal2
P 208 75 * metal2
W 195 67 216 83
S 8 H metal2 P 5 P 6
P 203 65 * metal2
P 208 65 * metal2
W 195 57 216 73
S 8 H metal2 P 7 P 8
P 203 142 * metal2
P 208 142 * metal2
W 199 138 212 146 vdd
S 4 H metal2 P 9 P 10
P 203 136 * metal2
P 208 136 * metal2
W 201 134 210 138 e0
S 2 H metal2 P 11 P 12
P 203 129 * metal2
P 208 129 * metal2
W 196 122 215 136 vss
S 7 H metal2 P 13 P 14
E
