#cell1 grrli_c idps_symb compose *
# 27-Mar-95 11:35 27-Mar-95 11:35 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 137 74 147 155
B 137 74 147 155
F F
C vdd 137 76 metal 2 1 * * W vdd
C vdd 147 76 metal 2 2 * * E vdd
C f 145 155 metal 1 3 * * N f
C i 140 155 metal 1 4 * * N i
C vdd 137 115 metal2 8 5 * * W vdd
C vdd 147 115 metal2 8 6 * * E vdd
C vss 147 126 metal2 8 7 * * E vss
C vss 137 126 metal2 8 8 * * W vss
I via1 141 115 0 "cvia" ly * *
I cbn1 141 76 0 "cbn" ly * *
I cdp1 141 82 0 "cdp" ly * *
I cbn2 145 76 0 "cbn" ly * *
I cdp2 141 112 0 "cdp" ly * *
I cdp3 143 106 0 "cdp" ly * *
I cdp4 143 97 0 "cdp" ly * *
I cdp5 143 88 0 "cdp" ly * *
I cxp1 140 109 0 "cxp" ly * *
I cxp2 140 103 0 "cxp" ly * *
I cxp3 140 100 0 "cxp" ly * *
I cxp4 140 94 0 "cxp" ly * *
I cxp5 140 91 0 "cxp" ly * *
I cxp6 140 85 0 "cxp" ly * *
I via2 141 126 0 "cvia" ly * *
I cbp1 141 123 0 "cbp" ly * *
I cdn1 143 135 0 "cdn" ly * *
I cdn2 143 141 0 "cdn" ly * *
I cdn3 141 129 0 "cdn" ly * *
I cxn1 140 132 0 "cxn" ly * *
I cdn4 141 147 0 "cdn" ly * *
I cxn2 140 144 0 "cxn" ly * *
I cpf1 140 152 0 "cpf" ly * *
P 138 147 * ndif
P 144 147 * ndif
W 135 144 147 150
S 3 H ndif P 1 P 2
P 140 132 * ntrans
P 140 144 * ntrans
W 139 131 141 145
S 1 V ntrans P 3 P 4
P 140 144 * ntrans
P 146 144 * ntrans
W 139 143 147 145
S 1 H ntrans P 5 P 6
P 140 132 * ntrans
P 146 132 * ntrans
W 139 131 147 133
S 1 H ntrans P 7 P 8
P 138 129 * ndif
P 144 129 * ndif
W 135 126 147 132
S 3 H ndif P 9 P 10
P 143 135 * ndif
P 143 141 * ndif
W 140 132 146 144
S 3 V ndif P 11 P 12
P 142 74 * nwell
P 142 114 * nwell
W 130 62 154 126
S 12 V nwell P 13 P 14
P 138 112 * pdif
P 144 112 * pdif
W 135 109 147 115
S 3 H pdif P 15 P 16
P 140 103 * ptrans
P 140 109 * ptrans
W 139 102 141 110
S 1 V ptrans P 17 P 18
P 140 109 * ptrans
P 146 109 * ptrans
W 139 108 147 110
S 1 H ptrans P 19 P 20
P 140 103 * ptrans
P 146 103 * ptrans
W 139 102 147 104
S 1 H ptrans P 21 P 22
P 140 94 * ptrans
P 140 100 * ptrans
W 139 93 141 101
S 1 V ptrans P 23 P 24
P 140 100 * ptrans
P 146 100 * ptrans
W 139 99 147 101
S 1 H ptrans P 25 P 26
P 140 94 * ptrans
P 146 94 * ptrans
W 139 93 147 95
S 1 H ptrans P 27 P 28
P 140 85 * ptrans
P 140 91 * ptrans
W 139 84 141 92
S 1 V ptrans P 29 P 30
P 140 91 * ptrans
P 146 91 * ptrans
W 139 90 147 92
S 1 H ptrans P 31 P 32
P 140 85 * ptrans
P 146 85 * ptrans
W 139 84 147 86
S 1 H ptrans P 33 P 34
P 138 82 * pdif
P 144 82 * pdif
W 135 79 147 85
S 3 H pdif P 35 P 36
P 137 76 * ntie
P 147 76 * ntie
W 134 73 150 79
S 3 H ntie P 37 P 38
P 137 115 * metal2
P 147 115 * metal2
W 129 107 155 123
S 8 H metal2 P 39 P 40
P 140 152 * metal
P 140 155 * metal
W 139 151 141 156
S 1 V metal P 41 P 42
P 139 129 * metal
P 139 147 * metal
W 138 128 140 148
S 1 V metal P 43 P 44
P 139 147 * metal
P 141 147 * metal
W 137 145 143 149
S 2 H metal P 45 P 46
P 137 126 * metal2
P 147 126 * metal2
W 129 118 155 134 vss
S 8 H metal2 P 47 P 48
P 139 129 * metal
P 141 129 * metal
W 137 127 143 131
S 2 H metal P 49 P 50
P 141 123 * metal
P 141 129 * metal
W 139 121 143 131
S 2 V metal P 51 P 52
P 141 76 * metal
P 141 82 * metal
W 139 74 143 84
S 2 V metal P 53 P 54
P 139 82 * metal
P 141 82 * metal
W 137 80 143 84
S 2 H metal P 55 P 56
P 139 82 * metal
P 139 112 * metal
W 138 81 140 113
S 1 V metal P 57 P 58
P 139 112 * metal
P 141 112 * metal
W 137 110 143 114
S 2 H metal P 59 P 60
P 141 76 * metal
P 147 76 * metal
W 139 74 149 78
S 2 H metal P 61 P 62
P 137 76 * metal
P 141 76 * metal
W 135 74 143 78
S 2 H metal P 63 P 64
P 141 112 * metal
P 141 115 * metal
W 139 110 143 117
S 2 V metal P 65 P 66
P 146 85 * poly
P 146 151 * poly
W 145 84 147 152
S 1 V poly P 67 P 68
P 140 151 * poly
P 146 151 * poly
W 139 150 147 152
S 1 H poly P 69 P 70
P 140 151 * poly
P 140 153 * poly
W 137 148 143 156
S 3 V poly P 71 P 72
P 145 152 * metal
P 145 155 * metal
W 144 151 146 156 f
S 1 V metal P 73 P 74
P 145 142 * metal
P 145 152 * metal
W 144 141 146 153 f
S 1 V metal P 75 P 76
P 145 141 * metal
P 145 142 * metal
W 144 140 146 143 f
S 1 V metal P 77 P 78
P 145 135 * metal
P 145 141 * metal
W 144 134 146 142 f
S 1 V metal P 79 P 80
P 143 88 * metal
P 145 88 * metal
W 141 86 147 90 f
S 2 H metal P 81 P 82
P 145 88 * metal
P 145 97 * metal
W 144 87 146 98 f
S 1 V metal P 83 P 84
P 143 97 * metal
P 145 97 * metal
W 141 95 147 99 f
S 2 H metal P 85 P 86
P 145 97 * metal
P 145 106 * metal
W 144 96 146 107 f
S 1 V metal P 87 P 88
P 143 106 * metal
P 145 106 * metal
W 141 104 147 108 f
S 2 H metal P 89 P 90
P 145 106 * metal
P 145 135 * metal
W 144 105 146 136 f
S 1 V metal P 91 P 92
P 143 135 * metal
P 145 135 * metal
W 141 133 147 137 f
S 2 H metal P 93 P 94
P 143 141 * metal
P 145 141 * metal
W 141 139 147 143 f
S 2 H metal P 95 P 96
E
