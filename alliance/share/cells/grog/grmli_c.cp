#cell1 grmli_c idps_symb compose *
# 27-Mar-95 11:33 27-Mar-95 11:33 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 137 88 147 155
B 137 88 147 155
F F
C vdd 137 90 metal 2 1 * * W vdd
C vdd 147 90 metal 2 2 * * E vdd
C f 145 155 metal 1 3 * * N f
C i 140 155 metal 1 4 * * N i
C vdd 137 121 metal2 8 5 * * W vdd
C vdd 147 121 metal2 8 6 * * E vdd
C vss 147 132 metal2 8 7 * * E vss
C vss 137 132 metal2 8 8 * * W vss
I via1 141 121 0 "cvia" ly * *
I cbn1 141 90 0 "cbn" ly * *
I cdp1 141 97 0 "cdp" ly * *
I cbn2 145 90 0 "cbn" ly * *
I cdp2 141 118 0 "cdp" ly * *
I cdp3 143 112 0 "cdp" ly * *
I cdp4 143 103 0 "cdp" ly * *
I cxp1 140 115 0 "cxp" ly * *
I cxp2 140 109 0 "cxp" ly * *
I cxp3 140 106 0 "cxp" ly * *
I cxp4 140 100 0 "cxp" ly * *
I via2 141 132 0 "cvia" ly * *
I cbp1 141 129 0 "cbp" ly * *
I cdn1 143 141 0 "cdn" ly * *
I cdn2 141 135 0 "cdn" ly * *
I cxn1 140 138 0 "cxn" ly * *
I cdn3 141 147 0 "cdn" ly * *
I cxn2 140 144 0 "cxn" ly * *
I cpf1 140 152 0 "cpf" ly * *
P 138 147 * ndif
P 144 147 * ndif
W 135 144 147 150
S 3 H ndif P 1 P 2
P 140 138 * ntrans
P 140 144 * ntrans
W 139 137 141 145
S 1 V ntrans P 3 P 4
P 140 144 * ntrans
P 146 144 * ntrans
W 139 143 147 145
S 1 H ntrans P 5 P 6
P 140 138 * ntrans
P 146 138 * ntrans
W 139 137 147 139
S 1 H ntrans P 7 P 8
P 138 135 * ndif
P 144 135 * ndif
W 135 132 147 138
S 3 H ndif P 9 P 10
P 138 118 * pdif
P 144 118 * pdif
W 135 115 147 121
S 3 H pdif P 11 P 12
P 140 109 * ptrans
P 140 115 * ptrans
W 139 108 141 116
S 1 V ptrans P 13 P 14
P 140 115 * ptrans
P 146 115 * ptrans
W 139 114 147 116
S 1 H ptrans P 15 P 16
P 140 109 * ptrans
P 146 109 * ptrans
W 139 108 147 110
S 1 H ptrans P 17 P 18
P 140 100 * ptrans
P 140 106 * ptrans
W 139 99 141 107
S 1 V ptrans P 19 P 20
P 140 106 * ptrans
P 146 106 * ptrans
W 139 105 147 107
S 1 H ptrans P 21 P 22
P 140 100 * ptrans
P 146 100 * ptrans
W 139 99 147 101
S 1 H ptrans P 23 P 24
P 138 96 * pdif
P 144 96 * pdif
W 135 93 147 99
S 3 H pdif P 25 P 26
P 137 90 * ntie
P 147 90 * ntie
W 134 87 150 93
S 3 H ntie P 27 P 28
P 137 121 * metal2
P 147 121 * metal2
W 129 113 155 129
S 8 H metal2 P 29 P 30
P 140 152 * metal
P 140 155 * metal
W 139 151 141 156
S 1 V metal P 31 P 32
P 139 135 * metal
P 139 147 * metal
W 138 134 140 148
S 1 V metal P 33 P 34
P 139 147 * metal
P 141 147 * metal
W 137 145 143 149
S 2 H metal P 35 P 36
P 137 132 * metal2
P 147 132 * metal2
W 129 124 155 140 vss
S 8 H metal2 P 37 P 38
P 139 135 * metal
P 141 135 * metal
W 137 133 143 137
S 2 H metal P 39 P 40
P 141 129 * metal
P 141 135 * metal
W 139 127 143 137
S 2 V metal P 41 P 42
P 141 90 * metal
P 141 97 * metal
W 139 88 143 99
S 2 V metal P 43 P 44
P 139 97 * metal
P 141 97 * metal
W 137 95 143 99
S 2 H metal P 45 P 46
P 139 97 * metal
P 139 118 * metal
W 138 96 140 119
S 1 V metal P 47 P 48
P 139 118 * metal
P 141 118 * metal
W 137 116 143 120
S 2 H metal P 49 P 50
P 141 90 * metal
P 147 90 * metal
W 139 88 149 92
S 2 H metal P 51 P 52
P 137 90 * metal
P 141 90 * metal
W 135 88 143 92
S 2 H metal P 53 P 54
P 141 118 * metal
P 141 121 * metal
W 139 116 143 123
S 2 V metal P 55 P 56
P 146 100 * poly
P 146 151 * poly
W 145 99 147 152
S 1 V poly P 57 P 58
P 140 151 * poly
P 146 151 * poly
W 139 150 147 152
S 1 H poly P 59 P 60
P 140 151 * poly
P 140 153 * poly
W 137 148 143 156
S 3 V poly P 61 P 62
P 145 152 * metal
P 145 155 * metal
W 144 151 146 156 f
S 1 V metal P 63 P 64
P 145 142 * metal
P 145 152 * metal
W 144 141 146 153 f
S 1 V metal P 65 P 66
P 145 103 * metal
P 145 103 * metal
W 144 102 146 104 f
S 1 H metal P 67 P 68
P 143 103 * metal
P 145 103 * metal
W 141 101 147 105 f
S 2 H metal P 69 P 70
P 145 103 * metal
P 145 112 * metal
W 144 102 146 113 f
S 1 V metal P 71 P 72
P 145 112 * metal
P 145 141 * metal
W 144 111 146 142 f
S 1 V metal P 73 P 74
P 145 141 * metal
P 145 142 * metal
W 144 140 146 143 f
S 1 V metal P 75 P 76
P 143 141 * metal
P 145 141 * metal
W 141 139 147 143 f
S 2 H metal P 77 P 78
P 143 112 * metal
P 145 112 * metal
W 141 110 147 114 f
S 2 H metal P 79 P 80
P 142 88 * nwell
P 142 120 * nwell
W 130 76 154 132
S 12 V nwell P 81 P 82
E
