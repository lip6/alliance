#cell1 grpick_c idps_symb compose *
# 27-Mar-95 11:34 27-Mar-95 11:34 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A 168 90 208 160
B 168 90 208 160
F F
C vss 168 151 metal2 4 1 * * W vss
C vdd 208 139 metal2 4 2 * * E vdd
C vdd 168 139 metal2 4 3 * * W vdd
C vdd 168 110 metal2 5 4 * * W vdd
C e8 168 121 metal2 2 5 * * W e8
C e7 168 133 metal2 2 6 * * W e7
C e6 168 145 metal2 2 7 * * W e6
C ck_13 168 116 metal2 2 8 * * W ck_13
C ck_11 168 155 metal 1 9 * * W ck_11
C e5 168 126 metal2 2 10 * * W e5
C ck_11 208 157 metal2 2 11 * * E ck_11
C vdd 207 90 metal 7 12 * * S vdd
C vss 171 90 metal 3 13 * * S vss
C vss 208 151 metal2 4 14 * * E vss
C vdd 208 160 metal 1 15 * * E vdd
C e6 208 145 metal2 2 16 * * E e6
C e7 208 133 metal2 2 17 * * E e7
C e5 208 128 metal2 2 18 * * E e5
C e8 208 123 metal2 2 19 * * E e8
I cdp1 191 157 0 "cdp" ly * *
I cdp2 196 151 0 "cdp" ly * *
I via1 186 116 0 "cvia" ly * *
I via2 184 157 0 "cvia" ly * *
I cpf1 184 153 0 "cpf" ly * *
I via3 171 151 0 "cvia" ly * *
I cdp3 196 157 0 "cdp" ly * *
I cdp4 192 151 0 "cdp" ly * *
I cbn1 205 157 0 "cbn" ly * *
I cbn2 205 153 0 "cbn" ly * *
I cbn3 205 141 0 "cbn" ly * *
I cbn4 205 137 0 "cbn" ly * *
I cdn1 179 151 0 "cdn" ly * *
I cdn2 175 151 0 "cdn" ly * *
I cdn3 176 139 0 "cdn" ly * *
I cpf2 186 148 0 "cpf" ly * *
I cdp5 190 139 0 "cdp" ly * *
I cdp6 197 139 0 "cdp" ly * *
I cdn4 176 145 0 "cdn" ly * *
I cdp7 192 145 0 "cdp" ly * *
I cdp8 197 145 0 "cdp" ly * *
I cdp9 190 133 0 "cdp" ly * *
I cdp10 197 133 0 "cdp" ly * *
I cdn5 175 133 0 "cdn" ly * *
I cdn6 179 133 0 "cdn" ly * *
I cdn7 177 127 0 "cdn" ly * *
I cdp11 190 127 0 "cdp" ly * *
I cdp12 197 127 0 "cdp" ly * *
I cbp1 178 121 0 "cbp" ly * *
I cbp2 171 121 0 "cbp" ly * *
I via4 204 111 0 "cvia" ly * *
I via5 208 111 0 "cvia" ly * *
I cdn8 171 133 0 "cdn" ly * *
I cbn5 190 121 0 "cbn" ly * *
I cbn6 194 121 0 "cbn" ly * *
I cbn7 198 121 0 "cbn" ly * *
I cbn8 205 121 0 "cbn" ly * *
I cbn9 205 125 0 "cbn" ly * *
I cbn10 205 129 0 "cbn" ly * *
I cbn11 205 133 0 "cbn" ly * *
I cbn12 205 145 0 "cbn" ly * *
I cbn13 205 149 0 "cbn" ly * *
I via6 208 103 0 "cvia" ly * *
I via7 204 103 0 "cvia" ly * *
I via8 204 107 0 "cvia" ly * *
I via9 208 107 0 "cvia" ly * *
I via10 201 139 0 "cvia" ly * *
I via11 194 139 0 "cvia" ly * *
P 205 114 * nwell
P 205 161 * nwell
W 201 110 209 165
S 4 V nwell P 1 P 2
P 204 103 * metal
P 204 110 * metal
W 202 101 206 112
S 2 V metal P 3 P 4
P 203 107 * metal2
P 210 107 * metal2
W 192 96 221 118
S 11 H metal2 P 5 P 6
P 190 121 * metal
P 205 121 * metal
W 188 119 207 123
S 2 H metal P 7 P 8
P 171 121 * ptie
P 178 121 * ptie
W 168 118 181 124
S 3 H ptie P 9 P 10
P 189 121 * ntie
P 205 121 * ntie
W 186 118 208 124
S 3 H ntie P 11 P 12
P 205 121 * ntie
P 205 158 * ntie
W 202 118 208 161
S 3 V ntie P 13 P 14
P 176 145 * metal
P 176 147 * metal
W 174 143 178 149 n2
S 2 V metal P 15 P 16
P 176 147 * metal
P 186 147 * metal
W 175 146 187 148 n2
S 1 H metal P 17 P 18
P 186 147 * metal
P 186 148 * metal
W 184 145 188 150 n2
S 2 V metal P 19 P 20
P 186 148 * metal
P 188 148 * metal
W 184 146 190 150 n2
S 2 H metal P 21 P 22
P 188 148 * metal
P 188 157 * metal
W 187 147 189 158 n2
S 1 V metal P 23 P 24
P 188 157 * metal
P 190 157 * metal
W 186 155 192 159 n2
S 2 H metal P 25 P 26
P 190 157 * metal
P 196 157 * metal
W 189 156 197 158 n2
S 1 H metal P 27 P 28
P 191 157 * pdif
P 196 157 * pdif
W 188 154 199 160
S 3 H pdif P 29 P 30
P 189 145 * pdif
P 197 145 * pdif
W 187 143 199 147
S 2 H pdif P 31 P 32
P 190 133 * pdif
P 198 133 * pdif
W 187 130 201 136
S 3 H pdif P 33 P 34
P 189 139 * pdif
P 198 139 * pdif
W 187 137 200 141
S 2 H pdif P 35 P 36
P 192 145 * metal
P 197 145 * metal
W 191 144 198 146
S 1 H metal P 37 P 38
P 192 143 * metal
P 192 145 * metal
W 190 141 194 147
S 2 V metal P 39 P 40
P 186 143 * metal
P 192 143 * metal
W 185 142 193 144
S 1 H metal P 41 P 42
P 186 139 * metal
P 186 143 * metal
W 185 138 187 144
S 1 V metal P 43 P 44
P 176 139 * metal
P 186 139 * metal
W 175 138 187 140
S 1 H metal P 45 P 46
P 186 127 * metal
P 186 139 * metal
W 185 126 187 140
S 1 V metal P 47 P 48
P 186 116 * metal
P 186 127 * metal
W 185 115 187 128
S 1 V metal P 49 P 50
P 177 127 * metal
P 186 127 * metal
W 176 126 187 128
S 1 H metal P 51 P 52
P 171 121 * metal
P 178 121 * metal
W 169 119 180 123 vss
S 2 H metal P 53 P 54
P 171 90 * metal
P 171 121 * metal
W 168 87 174 124 vss
S 3 V metal P 55 P 56
P 171 121 * metal
P 171 133 * metal
W 168 118 174 136 vss
S 3 V metal P 57 P 58
P 171 133 * metal
P 178 133 * metal
W 169 131 180 135 vss
S 2 H metal P 59 P 60
P 171 133 * metal
P 171 151 * metal
W 168 130 174 154 vss
S 3 V metal P 61 P 62
P 182 153 * poly
P 185 153 * poly
W 181 152 186 154
S 1 H poly P 63 P 64
P 182 148 * poly
P 182 153 * poly
W 181 147 183 154
S 1 V poly P 65 P 66
P 185 154 * poly
P 187 154 * poly
W 184 153 188 155
S 1 H poly P 67 P 68
P 173 139 * ndif
P 179 139 * ndif
W 171 137 181 141
S 2 H ndif P 69 P 70
P 171 133 * ndif
P 179 133 * ndif
W 168 130 182 136
S 3 H ndif P 71 P 72
P 175 127 * ndif
P 179 127 * ndif
W 173 125 181 129
S 2 H ndif P 73 P 74
P 168 155 * metal
P 184 155 * metal
W 167 154 185 156 ck_11
S 1 H metal P 75 P 76
P 184 153 * metal
P 184 155 * metal
W 182 151 186 157 ck_11
S 2 V metal P 77 P 78
P 184 155 * metal
P 184 157 * metal
W 182 153 186 159 ck_11
S 2 V metal P 79 P 80
P 168 151 * metal2
P 208 151 * metal2
W 164 147 212 155 vss
S 4 H metal2 P 81 P 82
P 182 130 * poly
P 185 130 * poly
W 181 129 186 131
S 1 H poly P 83 P 84
P 185 130 * poly
P 187 130 * poly
W 184 129 188 131
S 1 H poly P 85 P 86
P 185 130 * poly
P 185 136 * poly
W 184 129 186 137
S 1 V poly P 87 P 88
P 185 136 * poly
P 187 136 * poly
W 184 135 188 137
S 1 H poly P 89 P 90
P 182 136 * poly
P 185 136 * poly
W 181 135 186 137
S 1 H poly P 91 P 92
P 185 136 * poly
P 185 142 * poly
W 184 135 186 143
S 1 V poly P 93 P 94
P 185 142 * poly
P 187 142 * poly
W 184 141 188 143
S 1 H poly P 95 P 96
P 187 133 * metal
P 197 133 * metal
W 186 132 198 134
S 1 H metal P 97 P 98
P 190 127 * metal
P 203 127 * metal
W 188 125 205 129
S 2 H metal P 99 P 100
P 191 127 * pdif
P 198 127 * pdif
W 188 124 201 130
S 3 H pdif P 101 P 102
P 172 130 * ntrans
P 182 130 * ntrans
W 171 129 183 131
S 1 H ntrans P 103 P 104
P 187 130 * ptrans
P 200 130 * ptrans
W 186 129 201 131
S 1 H ptrans P 105 P 106
P 189 151 * pdif
P 198 151 * pdif
W 186 148 201 154
S 3 H pdif P 107 P 108
P 187 136 * ptrans
P 200 136 * ptrans
W 186 135 201 137
S 1 H ptrans P 109 P 110
P 174 151 * ndif
P 179 151 * ndif
W 171 148 182 154
S 3 H ndif P 111 P 112
P 171 151 * metal
P 179 151 * metal
W 169 149 181 153
S 2 H metal P 113 P 114
P 172 151 * ndif
P 173 151 * ndif
W 169 148 176 154
S 3 H ndif P 115 P 116
P 173 145 * ndif
P 179 145 * ndif
W 171 143 181 147
S 2 H ndif P 117 P 118
P 207 90 * metal
P 207 160 * metal
W 200 83 214 167 vdd
S 7 V metal P 119 P 120
P 190 139 * metal
P 204 139 * metal
W 188 137 206 141
S 2 H metal P 121 P 122
P 187 142 * ptrans
P 200 142 * ptrans
W 186 141 201 143
S 1 H ptrans P 123 P 124
P 197 114 * nwell
P 197 161 * nwell
W 179 96 215 179
S 18 V nwell P 125 P 126
P 168 133 * metal2
P 208 133 * metal2
W 166 131 210 135 e7
S 2 H metal2 P 127 P 128
P 168 145 * metal2
P 208 145 * metal2
W 166 143 210 147 e6
S 2 H metal2 P 129 P 130
P 170 136 * ntrans
P 182 136 * ntrans
W 169 135 183 137
S 1 H ntrans P 131 P 132
P 170 148 * ntrans
P 182 148 * ntrans
W 169 147 183 149
S 1 H ntrans P 133 P 134
P 184 157 * metal2
P 209 157 * metal2
W 182 155 211 159
S 2 H metal2 P 135 P 136
P 180 126 * metal2
P 180 128 * metal2
W 178 124 182 130 e5
S 2 V metal2 P 137 P 138
P 168 126 * metal2
P 180 126 * metal2
W 166 124 182 128 e5
S 2 H metal2 P 139 P 140
P 180 128 * metal2
P 208 128 * metal2
W 178 126 210 130 e5
S 2 H metal2 P 141 P 142
P 168 121 * metal2
P 201 121 * metal2
W 166 119 203 123 e8
S 2 H metal2 P 143 P 144
P 201 121 * metal2
P 201 123 * metal2
W 199 119 203 125 e8
S 2 V metal2 P 145 P 146
P 201 123 * metal2
P 208 123 * metal2
W 199 121 210 125 e8
S 2 H metal2 P 147 P 148
P 168 116 * metal2
P 186 116 * metal2
W 166 114 188 118 ck_13
S 2 H metal2 P 149 P 150
P 168 110 * metal2
P 210 110 * metal2
W 163 105 215 115 vdd
S 5 H metal2 P 151 P 152
P 192 151 * metal
P 206 151 * metal
W 190 149 208 153
S 2 H metal P 153 P 154
P 168 139 * metal2
P 208 139 * metal2
W 164 135 212 143 vdd
S 4 H metal2 P 155 P 156
P 187 148 * ptrans
P 200 148 * ptrans
W 186 147 201 149
S 1 H ptrans P 157 P 158
P 187 154 * ptrans
P 200 154 * ptrans
W 186 153 201 155
S 1 H ptrans P 159 P 160
P 185 142 * poly
P 185 148 * poly
W 184 141 186 149
S 1 V poly P 161 P 162
P 185 148 * poly
P 187 148 * poly
W 184 147 188 149
S 1 H poly P 163 P 164
E
