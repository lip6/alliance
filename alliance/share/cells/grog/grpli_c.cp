#cell1 grpli_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 137 74 147 142
B 137 74 147 142
F F
C vss 137 123 metal2 8 1 * * W vss
C vss 147 123 metal2 8 2 * * E vss
C vdd 147 113 metal2 8 3 * * E vdd
C vdd 137 113 metal2 8 4 * * W vdd
C i 140 142 metal 1 5 * * N i
C f 145 142 metal 1 6 * * N f
C vdd 147 76 metal 2 7 * * E vdd
C vdd 137 76 metal 2 8 * * W vdd
I cpf1 140 141 0 "cpf" ly * *
I cxn1 140 133 0 "cxn" ly * *
I cdn1 141 136 0 "cdn" ly * *
I cxn2 140 127 0 "cxn" ly * *
I cdn2 141 124 0 "cdn" ly * *
I cdn3 143 130 0 "cdn" ly * *
I cbp1 141 115 0 "cbp" ly * *
I via1 141 121 0 "cvia" ly * *
I cxp1 140 85 0 "cxp" ly * *
I cxp2 140 91 0 "cxp" ly * *
I cxp3 140 94 0 "cxp" ly * *
I cxp4 140 100 0 "cxp" ly * *
I cdp1 143 88 0 "cdp" ly * *
I cdp2 143 97 0 "cdp" ly * *
I cdp3 141 103 0 "cdp" ly * *
I cbn1 145 76 0 "cbn" ly * *
I cdp4 141 82 0 "cdp" ly * *
I cbn2 141 76 0 "cbn" ly * *
I via2 141 110 0 "cvia" ly * *
P 141 115 * ptie
P 141 119 * ptie
W 138 112 144 122
S 3 V ptie P 1 P 2
P 145 130 * metal
P 145 142 * metal
W 144 129 146 143 f
S 1 V metal P 3 P 4
P 143 130 * metal
P 145 130 * metal
W 141 128 147 132 f
S 2 H metal P 5 P 6
P 145 97 * metal
P 145 130 * metal
W 144 96 146 131 f
S 1 V metal P 7 P 8
P 143 97 * metal
P 145 97 * metal
W 141 95 147 99 f
S 2 H metal P 9 P 10
P 145 88 * metal
P 145 97 * metal
W 144 87 146 98 f
S 1 V metal P 11 P 12
P 143 88 * metal
P 145 88 * metal
W 141 86 147 90 f
S 2 H metal P 13 P 14
P 142 74 * nwell
P 142 105 * nwell
W 130 62 154 117
S 12 V nwell P 15 P 16
P 140 140 * poly
P 140 141 * poly
W 137 137 143 144
S 3 V poly P 17 P 18
P 140 140 * poly
P 146 140 * poly
W 139 139 147 141
S 1 H poly P 19 P 20
P 146 85 * poly
P 146 140 * poly
W 145 84 147 141
S 1 V poly P 21 P 22
P 141 103 * metal
P 141 110 * metal
W 139 101 143 112
S 2 V metal P 23 P 24
P 137 76 * metal
P 141 76 * metal
W 135 74 143 78
S 2 H metal P 25 P 26
P 141 76 * metal
P 147 76 * metal
W 139 74 149 78
S 2 H metal P 27 P 28
P 139 103 * metal
P 141 103 * metal
W 137 101 143 105
S 2 H metal P 29 P 30
P 139 82 * metal
P 139 103 * metal
W 138 81 140 104
S 1 V metal P 31 P 32
P 139 82 * metal
P 141 82 * metal
W 137 80 143 84
S 2 H metal P 33 P 34
P 141 76 * metal
P 141 82 * metal
W 139 74 143 84
S 2 V metal P 35 P 36
P 141 115 * metal
P 141 124 * metal
W 139 113 143 126
S 2 V metal P 37 P 38
P 139 124 * metal
P 141 124 * metal
W 137 122 143 126
S 2 H metal P 39 P 40
P 137 123 * metal2
P 147 123 * metal2
W 129 115 155 131 vss
S 8 H metal2 P 41 P 42
P 139 136 * metal
P 141 136 * metal
W 137 134 143 138
S 2 H metal P 43 P 44
P 139 124 * metal
P 139 136 * metal
W 138 123 140 137
S 1 V metal P 45 P 46
P 140 141 * metal
P 140 142 * metal
W 139 140 141 143
S 1 V metal P 47 P 48
P 137 113 * metal2
P 147 113 * metal2
W 129 105 155 121
S 8 H metal2 P 49 P 50
P 137 76 * ntie
P 147 76 * ntie
W 134 73 150 79
S 3 H ntie P 51 P 52
P 138 82 * pdif
P 144 82 * pdif
W 135 79 147 85
S 3 H pdif P 53 P 54
P 140 85 * ptrans
P 146 85 * ptrans
W 139 84 147 86
S 1 H ptrans P 55 P 56
P 140 91 * ptrans
P 146 91 * ptrans
W 139 90 147 92
S 1 H ptrans P 57 P 58
P 140 85 * ptrans
P 140 91 * ptrans
W 139 84 141 92
S 1 V ptrans P 59 P 60
P 140 94 * ptrans
P 146 94 * ptrans
W 139 93 147 95
S 1 H ptrans P 61 P 62
P 140 100 * ptrans
P 146 100 * ptrans
W 139 99 147 101
S 1 H ptrans P 63 P 64
P 140 94 * ptrans
P 140 100 * ptrans
W 139 93 141 101
S 1 V ptrans P 65 P 66
P 138 103 * pdif
P 144 103 * pdif
W 135 100 147 106
S 3 H pdif P 67 P 68
P 138 124 * ndif
P 144 124 * ndif
W 135 121 147 127
S 3 H ndif P 69 P 70
P 140 127 * ntrans
P 146 127 * ntrans
W 139 126 147 128
S 1 H ntrans P 71 P 72
P 140 133 * ntrans
P 146 133 * ntrans
W 139 132 147 134
S 1 H ntrans P 73 P 74
P 140 127 * ntrans
P 140 133 * ntrans
W 139 126 141 134
S 1 V ntrans P 75 P 76
P 138 136 * ndif
P 144 136 * ndif
W 135 133 147 139
S 3 H ndif P 77 P 78
E
