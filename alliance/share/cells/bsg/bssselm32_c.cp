#cell1 bssselm32_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A 139 50 166 170
B 139 50 166 170
F F
C vss1 139 170 metal2 8 1 * * W vss1
C vss1 166 170 metal2 8 2 * * E vss1
C l 142 170 metal 1 3 * * N l
C r 163 170 metal 1 4 * * N r
C out_1_s 166 162 metal2 2 5 * * E out_1_s
C dat_1 166 152 metal2 2 6 * * E dat_1
C dat_1 139 152 metal2 2 7 * * W dat_1
C in_1 139 137 metal2 2 8 * * W in_1
C blin_1 166 139 poly 1 9 * * E blin_1
C vdd 139 110 metal2 6 10 * * W vdd
C vdd 166 110 metal2 6 11 * * E vdd
C in_2 139 83 metal2 2 12 * * W in_2
C blin_2 166 80 poly 1 13 * * E blin_2
C dat_2 139 68 metal2 2 14 * * W dat_2
C dat_2 166 68 metal2 2 15 * * E dat_2
C out_2_s 166 58 metal2 2 16 * * E out_2_s
C vss 139 50 metal2 8 17 * * W vss
C vss 166 50 metal2 8 18 * * E vss
C l 142 50 metal 1 19 * * S l
C r 163 50 metal 1 20 * * S r
I cpf1 163 162 0 "cpf" ly * *
I cdn1 159 157 0 "cdn" ly * *
I cdn2 153 157 0 "cdn" ly * *
I cdn3 147 157 0 "cdn" ly * *
I cpf2 142 162 0 "cpf" ly * *
I via1 147 170 0 "cvia" ly * *
I cbp1 151 170 0 "cbp" ly * *
I cpf3 163 55 0 "cpf" ly * *
I cpf4 142 71 0 "cpf" ly * *
I cdn4 159 63 0 "cdn" ly * *
I cdn5 147 63 0 "cdn" ly * *
I cdn6 153 63 0 "cdn" ly * *
I via2 147 50 0 "cvia" ly * *
I cbp2 151 50 0 "cbp" ly * *
I via3 147 137 0 "cvia" ly * *
I via4 153 162 0 "cvia" ly * *
I cdp1 153 134 0 "cdp" ly * *
I cdp2 153 126 0 "cdp" ly * *
I via5 153 110 0 "cvia" ly * *
I cdp3 153 94 0 "cdp" ly * *
I cdp4 153 86 0 "cdp" ly * *
I via6 159 83 0 "cvia" ly * *
I via7 153 58 0 "cvia" ly * *
I cbn1 153 114 0 "cbn" ly * *
I cbn2 153 106 0 "cbn" ly * *
I cbn3 153 120 0 "cbn" ly * *
I cbn4 153 100 0 "cbn" ly * *
P 153 58 * metal
P 153 86 * metal
W 152 57 154 87 out_2_s
S 1 V metal P 1 P 2
P 139 83 * metal2
P 159 83 * metal2
W 137 81 161 85
S 2 H metal2 P 3 P 4
P 150 55 * poly
P 150 58 * poly
W 149 54 151 59
S 1 V poly P 5 P 6
P 150 55 * poly
P 163 55 * poly
W 149 54 164 56
S 1 H poly P 7 P 8
P 156 68 * poly
P 156 71 * poly
W 155 67 157 72
S 1 V poly P 9 P 10
P 142 71 * poly
P 156 71 * poly
W 141 70 157 72
S 1 H poly P 11 P 12
P 153 58 * metal2
P 166 58 * metal2
W 151 56 168 60 out_2_s
S 2 H metal2 P 13 P 14
P 159 63 * metal
P 159 157 * metal
W 158 62 160 158
S 1 V metal P 15 P 16
P 156 130 * poly
P 156 139 * poly
W 155 129 157 140
S 1 V poly P 17 P 18
P 156 139 * poly
P 166 139 * poly
W 155 138 167 140
S 1 H poly P 19 P 20
P 150 90 * ptrans
P 156 90 * ptrans
W 148 88 158 92
S 2 H ptrans P 21 P 22
P 156 80 * poly
P 156 90 * poly
W 155 79 157 91
S 1 V poly P 23 P 24
P 156 80 * poly
P 166 80 * poly
W 155 79 167 81
S 1 H poly P 25 P 26
P 153 94 * metal
P 153 126 * metal
W 151 92 155 128
S 2 V metal P 27 P 28
P 150 130 * ptrans
P 156 130 * ptrans
W 148 128 158 132
S 2 H ptrans P 29 P 30
P 147 60 * ndif
P 147 66 * ndif
W 145 58 149 68
S 2 V ndif P 31 P 32
P 159 60 * ndif
P 159 66 * ndif
W 157 58 161 68
S 2 V ndif P 33 P 34
P 159 154 * ndif
P 159 160 * ndif
W 157 152 161 162
S 2 V ndif P 35 P 36
P 147 154 * ndif
P 147 160 * ndif
W 145 152 149 162
S 2 V ndif P 37 P 38
P 142 162 * poly
P 150 162 * poly
W 141 161 151 163
S 1 H poly P 39 P 40
P 139 137 * metal2
P 147 137 * metal2
W 137 135 149 139
S 2 H metal2 P 41 P 42
P 147 63 * metal
P 147 152 * metal
W 146 62 148 153
S 1 V metal P 43 P 44
P 139 68 * metal2
P 166 68 * metal2
W 137 66 168 70
S 2 H metal2 P 45 P 46
P 147 152 * metal
P 147 157 * metal
W 146 151 148 158
S 1 V metal P 47 P 48
P 139 152 * metal2
P 166 152 * metal2
W 137 150 168 154 dat_1
S 2 H metal2 P 49 P 50
P 147 50 * metal
P 151 50 * metal
W 145 48 153 52
S 2 H metal P 51 P 52
P 139 50 * metal2
P 166 50 * metal2
W 131 42 174 58
S 8 H metal2 P 53 P 54
P 147 170 * metal
P 151 170 * metal
W 145 168 153 172
S 2 H metal P 55 P 56
P 153 60 * ndif
P 153 66 * ndif
W 151 58 155 68
S 2 V ndif P 57 P 58
P 156 58 * ntrans
P 156 68 * ntrans
W 155 57 157 69
S 1 V ntrans P 59 P 60
P 150 58 * ntrans
P 150 68 * ntrans
W 149 57 151 69
S 1 V ntrans P 61 P 62
P 139 170 * metal2
P 166 170 * metal2
W 131 162 174 178 vss1
S 8 H metal2 P 63 P 64
P 142 50 * metal
P 142 170 * metal
W 141 49 143 171
S 1 V metal P 65 P 66
P 153 154 * ndif
P 153 160 * ndif
W 151 152 155 162
S 2 V ndif P 67 P 68
P 150 152 * ntrans
P 150 162 * ntrans
W 149 151 151 163
S 1 V ntrans P 69 P 70
P 156 162 * poly
P 163 162 * poly
W 155 161 164 163
S 1 H poly P 71 P 72
P 156 152 * ntrans
P 156 162 * ntrans
W 155 151 157 163
S 1 V ntrans P 73 P 74
P 163 50 * metal
P 163 170 * metal
W 162 49 164 171
S 1 V metal P 75 P 76
P 139 157 * allowM2
P 166 157 * allowM2
W 137 155 168 159
S 2 H allowM2 P 77 P 78
P 139 78 * allowM2
P 166 78 * allowM2
W 137 76 168 80
S 2 H allowM2 P 79 P 80
P 138 110 * nwell
P 167 110 * nwell
W 86 58 219 162
S 52 H nwell P 81 P 82
P 139 63 * allowM2
P 166 63 * allowM2
W 137 61 168 65
S 2 H allowM2 P 83 P 84
P 139 88 * allowM2
P 166 88 * allowM2
W 137 86 168 90
S 2 H allowM2 P 85 P 86
P 139 93 * allowM2
P 166 93 * allowM2
W 137 91 168 95
S 2 H allowM2 P 87 P 88
P 139 98 * allowM2
P 166 98 * allowM2
W 137 96 168 100
S 2 H allowM2 P 89 P 90
P 139 103 * allowM2
P 166 103 * allowM2
W 137 101 168 105
S 2 H allowM2 P 91 P 92
P 139 117 * allowM2
P 166 117 * allowM2
W 137 115 168 119
S 2 H allowM2 P 93 P 94
P 139 122 * allowM2
P 166 122 * allowM2
W 137 120 168 124
S 2 H allowM2 P 95 P 96
P 139 127 * allowM2
P 166 127 * allowM2
W 137 125 168 129
S 2 H allowM2 P 97 P 98
P 139 132 * allowM2
P 166 132 * allowM2
W 137 130 168 134
S 2 H allowM2 P 99 P 100
P 139 142 * allowM2
P 166 142 * allowM2
W 137 140 168 144
S 2 H allowM2 P 101 P 102
P 139 110 * metal2
P 166 110 * metal2
W 133 104 172 116
S 6 H metal2 P 103 P 104
P 153 162 * metal2
P 166 162 * metal2
W 151 160 168 164 out_1_s
S 2 H metal2 P 105 P 106
P 153 134 * metal
P 153 162 * metal
W 152 133 154 163 out_1_s
S 1 V metal P 107 P 108
E
