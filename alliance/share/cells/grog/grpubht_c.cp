#cell1 grpubht_c idps_symb compose *
# 27-Mar-95 11:35 27-Mar-95 11:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 677 -80 687 1
B 677 -80 687 1
F F
C vdd 677 -71 metal2 8 1 * * W vdd
C vdd 687 -71 metal2 8 2 * * E vdd
C vss 677 -53 metal2 8 3 * * W vss
C vss 687 -53 metal2 8 4 * * E vss
C hzb 677 -11 metal2 2 5 * * W hzb
C hz 677 -22 metal2 2 6 * * W hz
C hzb 687 -11 metal2 2 7 * * E hzb
C hz 687 -22 metal2 2 8 * * E hz
P 677 -22 * metal2
P 687 -22 * metal2
W 675 -24 689 -20
S 2 H metal2 P 1 P 2
P 677 -11 * metal2
P 687 -11 * metal2
W 675 -13 689 -9
S 2 H metal2 P 3 P 4
P 682 -80 * nwell
P 682 1 * nwell
W 672 -90 692 11
S 10 V nwell P 5 P 6
P 682 -80 * allowM1
P 682 1 * allowM1
W 681 -81 683 2 tr
S 1 V allowM1 P 7 P 8
P 677 -53 * metal2
P 687 -53 * metal2
W 669 -61 695 -45
S 8 H metal2 P 9 P 10
P 677 -71 * metal2
P 687 -71 * metal2
W 669 -79 695 -63
S 8 H metal2 P 11 P 12
E
