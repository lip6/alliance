#cell1 grmoth_c idps_symb compose *
# 27-Mar-95 11:33 27-Mar-95 11:33 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 680 -66 684 1
B 680 -66 684 1
F F
C hz 684 -22 metal2 2 1 * * E hz
C hzb 684 -11 metal2 2 2 * * E hzb
C hz 680 -22 metal2 2 3 * * W hz
C hzb 680 -11 metal2 2 4 * * W hzb
C vdd 680 -54 metal2 8 5 * * W vdd
C vdd 684 -54 metal2 8 6 * * E vdd
C vss 680 -44 metal2 8 7 * * W vss
C vss 684 -44 metal2 8 8 * * E vss
P 682 -66 * allowM1
P 682 1 * allowM1
W 681 -67 683 2 tr
S 1 V allowM1 P 1 P 2
P 680 -44 * metal2
P 684 -44 * metal2
W 672 -52 692 -36
S 8 H metal2 P 3 P 4
P 680 -54 * metal2
P 684 -54 * metal2
W 672 -62 692 -46
S 8 H metal2 P 5 P 6
P 680 -11 * metal2
P 684 -11 * metal2
W 678 -13 686 -9 hzb
S 2 H metal2 P 7 P 8
P 680 -22 * metal2
P 684 -22 * metal2
W 678 -24 686 -20 hz
S 2 H metal2 P 9 P 10
E
