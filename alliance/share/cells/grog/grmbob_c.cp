#cell1 grmbob_c idps_symb compose *
# 27-Mar-95 11:33 27-Mar-95 11:33 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A -63 151 -58 218
B -63 151 -58 218
F F
C vss -61 218 metal 3 1 * * N vss
C vss -63 173 metal2 8 2 * * W vss
C vss -58 173 metal2 8 3 * * E vss
C vdd -58 163 metal2 8 4 * * E vdd
C vdd -63 163 metal2 8 5 * * W vdd
I cbp1 -61 213 0 "cbp" ly * *
I cbp2 -61 209 0 "cbp" ly * *
I cbp3 -61 205 0 "cbp" ly * *
I cbp4 -61 201 0 "cbp" ly * *
I cbp5 -61 197 0 "cbp" ly * *
I cbp6 -61 193 0 "cbp" ly * *
I cbp7 -61 189 0 "cbp" ly * *
I cbp8 -61 185 0 "cbp" ly * *
I cbp9 -61 181 0 "cbp" ly * *
I via1 -61 175 0 "cvia" ly * *
I via2 -61 171 0 "cvia" ly * *
P -61 160 * metal
P -61 218 * metal
W -64 157 -58 221 vss
S 3 V metal P 1 P 2
P -61 181 * ptie
P -61 218 * ptie
W -64 178 -58 221
S 3 V ptie P 3 P 4
P -63 173 * metal2
P -58 173 * metal2
W -71 165 -50 181
S 8 H metal2 P 5 P 6
P -63 163 * metal2
P -58 163 * metal2
W -71 155 -50 171 vdd
S 8 H metal2 P 7 P 8
E
