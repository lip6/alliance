#cell1 bsdand4_c idps_symb compose *
# 31-Jan-95 18:19 31-Jan-95 18:19 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 116 50 155 170
B 116 50 155 170
F F
C vdd 116 110 metal2 6 1 * * W vdd
C vss 116 50 metal2 8 2 * * W vss
C vss 155 50 metal2 8 3 * * E vss
C vss1 155 170 metal2 8 4 * * E vss1
C vss1 116 170 metal2 8 5 * * W vss1
C out_2 155 83 metal2 2 6 * * E out_2
C a0 118 170 metal 1 7 * * N a0
C na0 122 170 metal 1 8 * * N na0
C a1 126 170 metal 1 9 * * N a1
C na1 130 170 metal 1 10 * * N na1
C na1 130 50 metal 1 11 * * S na1
C a1 126 50 metal 1 12 * * S a1
C na0 122 50 metal 1 13 * * S na0
C a0 118 50 metal 1 14 * * S a0
C out_1 155 137 metal2 2 15 * * E out_1
C dat_2 155 68 metal2 2 16 * * E dat_2
C dat_2 116 68 metal2 2 17 * * W dat_2
C dat_1 155 152 metal2 2 18 * * E dat_1
C dat_1 116 152 metal2 2 19 * * W dat_1
C vdd 155 110 metal2 6 20 * * E vdd
C com_1 116 147 metal2 2 21 * * W com_1
C com_2 116 73 metal2 2 22 * * W com_2
I cpf1 135 79 0 "cpf" ly * *
I cdp1 141 88 0 "cdp" ly * *
I cdp2 147 88 0 "cdp" ly * *
I cdp3 135 88 0 "cdp" ly * *
I cdn1 135 68 0 "cdn" ly * *
I cdn2 135 59 0 "cdn" ly * *
I via1 135 83 0 "cvia" ly * *
I via2 135 110 0 "cvia" ly * *
I via3 147 110 0 "cvia" ly * *
I cbn1 141 110 0 "cbn" ly * *
I cbp1 141 50 0 "cbp" ly * *
I cbp2 151 50 0 "cbp" ly * *
I via4 135 50 0 "cvia" ly * *
I via5 147 50 0 "cvia" ly * *
I via6 153 83 0 "cvia" ly * *
I cdp4 141 94 0 "cdp" ly * *
I cdp5 147 94 0 "cdp" ly * *
I cdp6 135 94 0 "cdp" ly * *
I cdp7 141 100 0 "cdp" ly * *
I cdp8 147 100 0 "cdp" ly * *
I cdp9 135 100 0 "cdp" ly * *
I via7 135 137 0 "cvia" ly * *
I cdp10 135 132 0 "cdp" ly * *
I cdp11 135 126 0 "cdp" ly * *
I cdp12 135 120 0 "cdp" ly * *
I cdp13 141 132 0 "cdp" ly * *
I cdp14 141 126 0 "cdp" ly * *
I cdp15 141 120 0 "cdp" ly * *
I cdp16 147 132 0 "cdp" ly * *
I cdp17 147 126 0 "cdp" ly * *
I cdp18 147 120 0 "cdp" ly * *
I cpf2 135 141 0 "cpf" ly * *
I cdn3 147 155 0 "cdn" ly * *
I cdn4 147 161 0 "cdn" ly * *
I via8 135 170 0 "cvia" ly * *
I via9 147 170 0 "cvia" ly * *
I cbp3 151 170 0 "cbp" ly * *
I cdn5 153 161 0 "cdn" ly * *
I cdn6 153 154 0 "cdn" ly * *
I cdp19 153 132 0 "cdp" ly * *
I cdp20 153 126 0 "cdp" ly * *
I cdp21 153 120 0 "cdp" ly * *
I via10 153 137 0 "cvia" ly * *
I cdn7 147 66 0 "cdn" ly * *
I cdn8 147 59 0 "cdn" ly * *
I cdn9 135 161 0 "cdn" ly * *
I cdn10 135 152 0 "cdn" ly * *
I cpf3 149 145 0 "cpf" ly * *
I cbn2 153 110 0 "cbn" ly * *
I cbp4 141 170 0 "cbp" ly * *
I cdp22 153 100 0 "cdp" ly * *
I cdp23 153 94 0 "cdp" ly * *
I cdp24 153 88 0 "cdp" ly * *
I cdn11 153 66 0 "cdn" ly * *
I cdn12 153 59 0 "cdn" ly * *
I cpf4 149 75 0 "cpf" ly * *
I ref_a0_2 118 83 0 "ref_ref" ly * *
I ref_na0_2 122 83 0 "ref_ref" ly * *
I ref_na1_2 130 53 0 "ref_ref" ly * *
I ref_a1_1 126 167 0 "ref_ref" ly * *
I ref_na1_1 130 167 0 "ref_ref" ly * *
I ref_a0_1 118 137 0 "ref_ref" ly * *
I ref_na0_1 122 137 0 "ref_ref" ly * *
I ref_ca0_1 118 147 0 "ref_ref" ly * *
I ref_cna0_1 122 147 0 "ref_ref" ly * *
I ref_ca1_1 126 147 0 "ref_ref" ly * *
I ref_cna1_1 130 147 0 "ref_ref" ly * *
I ref_ca0_2 118 73 0 "ref_ref" ly * *
I ref_cna0_2 122 73 0 "ref_ref" ly * *
I ref_ca1_2 126 73 0 "ref_ref" ly * *
I ref_a1_2 126 53 0 "ref_ref" ly * *
I ref_cna1_2 130 73 0 "ref_ref" ly * *
P 153 59 * metal
P 153 100 * metal
W 152 58 154 101
S 1 V metal P 1 P 2
P 141 75 * metal
P 149 75 * metal
W 140 74 150 76
S 1 H metal P 3 P 4
P 135 59 * metal
P 135 75 * metal
W 134 58 136 76
S 1 V metal P 5 P 6
P 135 75 * metal
P 141 75 * metal
W 134 74 142 76
S 1 H metal P 7 P 8
P 141 75 * metal
P 141 100 * metal
W 140 74 142 101
S 1 V metal P 9 P 10
P 147 50 * metal
P 151 50 * metal
W 145 48 153 52
S 2 H metal P 11 P 12
P 135 50 * metal
P 147 50 * metal
W 133 48 149 52
S 2 H metal P 13 P 14
P 147 50 * metal
P 147 66 * metal
W 145 48 149 68
S 2 V metal P 15 P 16
P 135 58 * ndif
P 135 72 * ndif
W 133 56 137 74
S 2 V ndif P 17 P 18
P 150 69 * poly
P 150 83 * poly
W 149 68 151 84
S 1 V poly P 19 P 20
P 135 79 * poly
P 138 79 * poly
W 132 76 141 82
S 3 H poly P 21 P 22
P 138 74 * poly
P 138 79 * poly
W 137 73 139 80
S 1 V poly P 23 P 24
P 138 79 * poly
P 138 85 * poly
W 137 78 139 86
S 1 V poly P 25 P 26
P 135 79 * metal
P 135 83 * metal
W 133 77 137 85
S 2 V metal P 27 P 28
P 126 53 * poly
P 144 53 * poly
W 125 52 145 54 v_1_2
S 1 H poly P 29 P 30
P 144 53 * poly
P 144 56 * poly
W 143 52 145 57
S 1 V poly P 31 P 32
P 150 56 * ntrans
P 150 69 * ntrans
W 149 55 151 70
S 1 V ntrans P 33 P 34
P 153 85 * pdif
P 153 104 * pdif
W 151 83 155 106
S 2 V pdif P 35 P 36
P 150 83 * ptrans
P 150 106 * ptrans
W 149 82 151 107
S 1 V ptrans P 37 P 38
P 126 167 * poly
P 144 167 * poly
W 125 166 145 168 v_1_1
S 1 H poly P 39 P 40
P 144 164 * poly
P 144 167 * poly
W 143 163 145 168
S 1 V poly P 41 P 42
P 115 110 * nwell
P 156 110 * nwell
W 63 58 208 162
S 52 H nwell P 43 P 44
P 147 110 * metal
P 153 110 * metal
W 145 108 155 112
S 2 H metal P 45 P 46
P 147 110 * metal
P 147 132 * metal
W 145 108 149 134
S 2 V metal P 47 P 48
P 147 88 * metal
P 147 110 * metal
W 145 86 149 112
S 2 V metal P 49 P 50
P 135 110 * metal
P 147 110 * metal
W 133 108 149 112
S 2 H metal P 51 P 52
P 135 88 * metal
P 135 110 * metal
W 133 86 137 112
S 2 V metal P 53 P 54
P 135 110 * metal
P 135 132 * metal
W 133 108 137 134
S 2 V metal P 55 P 56
P 141 145 * metal
P 149 145 * metal
W 140 144 150 146
S 1 H metal P 57 P 58
P 141 120 * metal
P 141 145 * metal
W 140 119 142 146
S 1 V metal P 59 P 60
P 135 145 * metal
P 141 145 * metal
W 134 144 142 146
S 1 H metal P 61 P 62
P 135 145 * metal
P 135 161 * metal
W 134 144 136 162
S 1 V metal P 63 P 64
P 153 120 * metal
P 153 161 * metal
W 152 119 154 162
S 1 V metal P 65 P 66
P 150 137 * poly
P 150 151 * poly
W 149 136 151 152
S 1 V poly P 67 P 68
P 153 153 * ndif
P 153 162 * ndif
W 151 151 155 164
S 2 V ndif P 69 P 70
P 135 170 * metal
P 147 170 * metal
W 133 168 149 172
S 2 H metal P 71 P 72
P 147 155 * metal
P 147 170 * metal
W 145 153 149 172
S 2 V metal P 73 P 74
P 147 170 * metal
P 151 170 * metal
W 145 168 153 172
S 2 H metal P 75 P 76
P 144 135 * poly
P 144 146 * poly
W 143 134 145 147
S 1 V poly P 77 P 78
P 138 141 * poly
P 138 146 * poly
W 137 140 139 147
S 1 V poly P 79 P 80
P 135 141 * poly
P 138 141 * poly
W 132 138 141 144
S 3 H poly P 81 P 82
P 138 135 * poly
P 138 141 * poly
W 137 134 139 142
S 1 V poly P 83 P 84
P 135 137 * metal
P 135 141 * metal
W 133 135 137 143
S 2 V metal P 85 P 86
P 116 73 * metal2
P 130 73 * metal2
W 114 71 132 75
S 2 H metal2 P 87 P 88
P 116 142 * allowM2
P 155 142 * allowM2
W 114 140 157 144
S 2 H allowM2 P 89 P 90
P 116 78 * allowM2
P 155 78 * allowM2
W 114 76 157 80
S 2 H allowM2 P 91 P 92
P 116 152 * metal2
P 155 152 * metal2
W 114 150 157 154
S 2 H metal2 P 93 P 94
P 116 68 * metal2
P 155 68 * metal2
W 114 66 157 70
S 2 H metal2 P 95 P 96
P 118 83 * metal2
P 135 83 * metal2
W 116 81 137 85 v_0_2
S 2 H metal2 P 97 P 98
P 153 137 * metal2
P 155 137 * metal2
W 151 135 157 139 out_1
S 2 H metal2 P 99 P 100
P 150 151 * ntrans
P 150 164 * ntrans
W 149 150 151 165
S 1 V ntrans P 101 P 102
P 153 116 * pdif
P 153 135 * pdif
W 151 114 155 137
S 2 V pdif P 103 P 104
P 150 114 * ptrans
P 150 137 * ptrans
W 149 113 151 138
S 1 V ptrans P 105 P 106
P 116 147 * metal2
P 128 147 * metal2
W 114 145 130 149
S 2 H metal2 P 107 P 108
P 128 147 * metal2
P 130 147 * metal2
W 126 145 132 149
S 2 H metal2 P 109 P 110
P 147 148 * ndif
P 147 162 * ndif
W 145 146 149 164
S 2 V ndif P 111 P 112
P 141 148 * ndif
P 141 162 * ndif
W 139 146 143 164
S 2 V ndif P 113 P 114
P 135 148 * ndif
P 135 162 * ndif
W 133 146 137 164
S 2 V ndif P 115 P 116
P 144 146 * ntrans
P 144 164 * ntrans
W 143 145 145 165
S 1 V ntrans P 117 P 118
P 138 146 * ntrans
P 138 164 * ntrans
W 137 145 139 165
S 1 V ntrans P 119 P 120
P 147 119 * pdif
P 147 133 * pdif
W 145 117 149 135
S 2 V pdif P 121 P 122
P 141 119 * pdif
P 141 133 * pdif
W 139 117 143 135
S 2 V pdif P 123 P 124
P 135 119 * pdif
P 135 133 * pdif
W 133 117 137 135
S 2 V pdif P 125 P 126
P 144 117 * ptrans
P 144 135 * ptrans
W 143 116 145 136
S 1 V ptrans P 127 P 128
P 138 117 * ptrans
P 138 135 * ptrans
W 137 116 139 136
S 1 V ptrans P 129 P 130
P 118 137 * metal2
P 135 137 * metal2
W 116 135 137 139 v_0_1
S 2 H metal2 P 131 P 132
P 116 63 * allowM2
P 155 63 * allowM2
W 114 61 157 65
S 2 H allowM2 P 133 P 134
P 118 50 * metal
P 118 170 * metal
W 117 49 119 171 a0
S 1 V metal P 135 P 136
P 122 50 * metal
P 122 170 * metal
W 121 49 123 171 na0
S 1 V metal P 137 P 138
P 126 50 * metal
P 126 170 * metal
W 125 49 127 171 a1
S 1 V metal P 139 P 140
P 130 50 * metal
P 130 170 * metal
W 129 49 131 171 na1
S 1 V metal P 141 P 142
P 116 157 * allowM2
P 155 157 * allowM2
W 114 155 157 159
S 2 H allowM2 P 143 P 144
P 116 88 * allowM2
P 155 88 * allowM2
W 114 86 157 90
S 2 H allowM2 P 145 P 146
P 116 93 * allowM2
P 155 93 * allowM2
W 114 91 157 95
S 2 H allowM2 P 147 P 148
P 116 98 * allowM2
P 155 98 * allowM2
W 114 96 157 100
S 2 H allowM2 P 149 P 150
P 116 103 * allowM2
P 155 103 * allowM2
W 114 101 157 105
S 2 H allowM2 P 151 P 152
P 116 117 * allowM2
P 155 117 * allowM2
W 114 115 157 119
S 2 H allowM2 P 153 P 154
P 116 122 * allowM2
P 155 122 * allowM2
W 114 120 157 124
S 2 H allowM2 P 155 P 156
P 116 127 * allowM2
P 155 127 * allowM2
W 114 125 157 129
S 2 H allowM2 P 157 P 158
P 116 132 * allowM2
P 155 132 * allowM2
W 114 130 157 134
S 2 H allowM2 P 159 P 160
P 116 50 * metal2
P 155 50 * metal2
W 108 42 163 58
S 8 H metal2 P 161 P 162
P 116 110 * metal2
P 155 110 * metal2
W 110 104 161 116
S 6 H metal2 P 163 P 164
P 116 170 * metal2
P 155 170 * metal2
W 108 162 163 178
S 8 H metal2 P 165 P 166
P 144 74 * poly
P 144 85 * poly
W 143 73 145 86
S 1 V poly P 167 P 168
P 135 87 * pdif
P 135 101 * pdif
W 133 85 137 103
S 2 V pdif P 169 P 170
P 147 87 * pdif
P 147 101 * pdif
W 145 85 149 103
S 2 V pdif P 171 P 172
P 141 87 * pdif
P 141 101 * pdif
W 139 85 143 103
S 2 V pdif P 173 P 174
P 144 85 * ptrans
P 144 103 * ptrans
W 143 84 145 104
S 1 V ptrans P 175 P 176
P 138 85 * ptrans
P 138 103 * ptrans
W 137 84 139 104
S 1 V ptrans P 177 P 178
P 147 58 * ndif
P 147 72 * ndif
W 145 56 149 74
S 2 V ndif P 179 P 180
P 141 58 * ndif
P 141 72 * ndif
W 139 56 143 74
S 2 V ndif P 181 P 182
P 144 56 * ntrans
P 144 74 * ntrans
W 143 55 145 75
S 1 V ntrans P 183 P 184
P 138 56 * ntrans
P 138 74 * ntrans
W 137 55 139 75
S 1 V ntrans P 185 P 186
P 153 83 * metal2
P 155 83 * metal2
W 151 81 157 85 out_2
S 2 H metal2 P 187 P 188
E
