#cell1 noa4_y idps_symb compose *
# 07-Oct-98 12:21 07-Oct-98 12:21 mbkvti400 * .cp
# .
V 4 VTIcompose 1.1
A -88 97 -58 139
B -88 97 -58 139
F F
C vss -88 99 metal 8 1 * * W vss
C vss -58 99 metal 8 2 * * E vss
C f -61 139 metal2 2 3 * * N f
C f -61 97 metal2 2 4 * * S f
C i3 -67 139 metal2 2 5 * * N i3
C i3 -67 97 metal2 2 6 * * S i3
C i2 -73 97 metal2 2 7 * * S i2
C i1 -79 97 metal2 2 8 * * S i1
C i0 -85 97 metal2 2 9 * * S i0
C i2 -73 139 metal2 2 10 * * N i2
C i1 -79 139 metal2 2 11 * * N i1
C i0 -85 139 metal2 2 12 * * N i0
C vdd -58 137 metal 8 13 * * E vdd
C vdd -88 137 metal 8 14 * * W vdd
I cdn1 -85 103 0 "cdn" ly * *
I cdn2 -73 103 0 "cdn" ly * *
I cdn3 -61 109 0 "cdn" ly * *
I cdn4 -67 109 0 "cdn" ly * *
I cdn5 -79 109 0 "cdn" ly * *
I cdp1 -85 137 0 "cdp" ly * *
I cdp2 -67 126 0 "cdp" ly * *
I cdp3 -61 131 0 "cdp" ly * *
I via12x1 -85 121 0 "via12" ly * *
I cpf1 -84 116 0 "cpf" ly * *
I via12x2 -79 126 0 "via12" ly * *
I cpf2 -78 118 0 "cpf" ly * *
I via12x3 -73 121 0 "via12" ly * *
I cpf3 -72 116 0 "cpf" ly * *
I via12x4 -61 118 0 "via12" ly * *
I cpf4 -66 116 0 "cpf" ly * *
I via12x5 -67 121 0 "via12" ly * *
I cbn1 -61 139 0 "cbn" ly * *
I cbp1 -79 96 0 "cbp" ly * *
I cbp2 -67 96 0 "cbp" ly * *
P -67 116 * metal
P -67 121 * metal
W -68 115 -66 122
S 1 V metal P 1 P 2
P -67 116 * metal
P -66 116 * metal
W -69 114 -64 118
S 2 H metal P 3 P 4
P -61 109 * metal
P -61 126 * metal
W -62 108 -60 127
S 1 V metal P 5 P 6
P -67 126 * metal
P -61 126 * metal
W -68 125 -60 127
S 1 H metal P 7 P 8
P -79 96 * ptie
P -67 96 * ptie
W -82 93 -64 99
S 3 H ptie P 9 P 10
P -85 116 * metal
P -84 116 * metal
W -87 114 -82 118
S 2 H metal P 11 P 12
P -85 116 * metal
P -85 121 * metal
W -86 115 -84 122
S 1 V metal P 13 P 14
P -79 118 * metal
P -78 118 * metal
W -81 116 -76 120
S 2 H metal P 15 P 16
P -79 118 * metal
P -79 126 * metal
W -80 117 -78 127
S 1 V metal P 17 P 18
P -73 116 * metal
P -72 116 * metal
W -75 114 -70 118
S 2 H metal P 19 P 20
P -73 116 * metal
P -73 121 * metal
W -74 115 -72 122
S 1 V metal P 21 P 22
P -84 116 * poly
P -82 116 * poly
W -87 113 -79 119
S 3 H poly P 23 P 24
P -82 114 * poly
P -82 116 * poly
W -83 113 -81 117
S 1 V poly P 25 P 26
P -82 116 * poly
P -82 119 * poly
W -83 115 -81 120
S 1 V poly P 27 P 28
P -78 118 * poly
P -76 118 * poly
W -81 115 -73 121
S 3 H poly P 29 P 30
P -76 114 * poly
P -76 118 * poly
W -77 113 -75 119
S 1 V poly P 31 P 32
P -76 118 * poly
P -76 119 * poly
W -77 117 -75 120
S 1 V poly P 33 P 34
P -72 116 * poly
P -70 116 * poly
W -75 113 -67 119
S 3 H poly P 35 P 36
P -70 114 * poly
P -70 116 * poly
W -71 113 -69 117
S 1 V poly P 37 P 38
P -70 116 * poly
P -70 119 * poly
W -71 115 -69 120
S 1 V poly P 39 P 40
P -66 116 * poly
P -64 116 * poly
W -69 113 -61 119
S 3 H poly P 41 P 42
P -64 114 * poly
P -64 116 * poly
W -65 113 -63 117
S 1 V poly P 43 P 44
P -64 116 * poly
P -64 119 * poly
W -65 115 -63 120
S 1 V poly P 45 P 46
P -85 97 * metal2
P -85 139 * metal2
W -87 95 -83 141
S 2 V metal2 P 47 P 48
P -73 97 * metal2
P -73 139 * metal2
W -75 95 -71 141
S 2 V metal2 P 49 P 50
P -61 121 * pdif
P -61 133 * pdif
W -63 119 -59 135
S 2 V pdif P 51 P 52
P -85 121 * pdif
P -85 143 * pdif
W -87 119 -83 145
S 2 V pdif P 53 P 54
P -61 101 * ndif
P -61 112 * ndif
W -63 99 -59 114
S 2 V ndif P 55 P 56
P -85 101 * ndif
P -85 112 * ndif
W -87 99 -83 114
S 2 V ndif P 57 P 58
P -79 97 * metal2
P -79 139 * metal2
W -81 95 -77 141
S 2 V metal2 P 59 P 60
P -67 97 * metal2
P -67 139 * metal2
W -69 95 -65 141
S 2 V metal2 P 61 P 62
P -61 97 * metal2
P -61 139 * metal2
W -63 95 -59 141
S 2 V metal2 P 63 P 64
P -61 131 * metal
P -61 137 * metal
W -63 129 -59 139
S 2 V metal P 65 P 66
P -88 137 * metal
P -61 137 * metal
W -96 129 -53 145
S 8 H metal P 67 P 68
P -61 137 * metal
P -58 137 * metal
W -69 129 -50 145
S 8 H metal P 69 P 70
P -67 121 * pdif
P -67 133 * pdif
W -69 119 -65 135
S 2 V pdif P 71 P 72
P -64 119 * ptrans
P -64 134 * ptrans
W -65 118 -63 135
S 1 V ptrans P 73 P 74
P -79 109 * metal
P -67 109 * metal
W -80 108 -66 110
S 1 H metal P 75 P 76
P -67 101 * ndif
P -67 112 * ndif
W -69 99 -65 114
S 2 V ndif P 77 P 78
P -73 101 * ndif
P -73 112 * ndif
W -75 99 -71 114
S 2 V ndif P 79 P 80
P -79 101 * ndif
P -79 112 * ndif
W -81 99 -77 114
S 2 V ndif P 81 P 82
P -88 99 * metal
P -58 99 * metal
W -96 91 -50 107
S 8 H metal P 83 P 84
P -64 99 * ntrans
P -64 114 * ntrans
W -65 98 -63 115
S 1 V ntrans P 85 P 86
P -82 99 * ntrans
P -82 114 * ntrans
W -83 98 -81 115
S 1 V ntrans P 87 P 88
P -76 99 * ntrans
P -76 114 * ntrans
W -77 98 -75 115
S 1 V ntrans P 89 P 90
P -70 99 * ntrans
P -70 114 * ntrans
W -71 98 -69 115
S 1 V ntrans P 91 P 92
P -73 121 * pdif
P -73 143 * pdif
W -75 119 -71 145
S 2 V pdif P 93 P 94
P -70 119 * ptrans
P -70 145 * ptrans
W -71 118 -69 146
S 1 V ptrans P 95 P 96
P -79 121 * pdif
P -79 143 * pdif
W -81 119 -77 145
S 2 V pdif P 97 P 98
P -82 119 * ptrans
P -82 145 * ptrans
W -83 118 -81 146
S 1 V ptrans P 99 P 100
P -76 119 * ptrans
P -76 145 * ptrans
W -77 118 -75 146
S 1 V ptrans P 101 P 102
P -88 133 * nwell
P -58 133 * nwell
W -114 107 -32 159
S 26 H nwell P 103 P 104
E
