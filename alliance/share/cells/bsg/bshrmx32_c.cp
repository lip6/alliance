#cell1 bshrmx32_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A -31 0 20 120
B -31 0 20 120
F F
C nshr -31 48 metal2 2 1 * * W nshr
C shr 18 0 metal 1 2 * * S shr
C vss1 20 120 metal2 8 3 * * E vss1
C vss -31 0 metal2 8 4 * * W vss
C vss1 -31 120 metal2 8 5 * * W vss1
C vdd -31 60 metal2 6 6 * * W vdd
C vdd 20 60 metal2 6 7 * * E vdd
C vss 20 0 metal2 8 8 * * E vss
I cdn1 18 33 0 "cdn" ly * *
I cbn1 -6 114 0 "cbn" ly * *
I via1 6 58 0 "cvia" ly * *
I via2 6 62 0 "cvia" ly * *
I cbn2 6 66 0 "cbn" ly * *
I cbn3 6 72 0 "cbn" ly * *
I cbn4 6 78 0 "cbn" ly * *
I cbn5 6 84 0 "cbn" ly * *
I cbn6 6 90 0 "cbn" ly * *
I cbn7 6 96 0 "cbn" ly * *
I cbn8 6 102 0 "cbn" ly * *
I cbn9 6 108 0 "cbn" ly * *
I cbn10 6 114 0 "cbn" ly * *
I via3 12 12 0 "cvia" ly * *
I via4 12 6 0 "cvia" ly * *
I via5 -6 58 0 "cvia" ly * *
I via6 -6 62 0 "cvia" ly * *
I via7 12 61 0 "cvia" ly * *
I cbp1 0 39 0 "cbp" ly * *
I cbp2 0 33 0 "cbp" ly * *
I cbp3 0 27 0 "cbp" ly * *
I cbp4 0 21 0 "cbp" ly * *
I cbp5 0 15 0 "cbp" ly * *
I via8 0 8 0 "cvia" ly * *
I cpf1 14 43 0 "cpf" ly * *
I via9 12 48 0 "cvia" ly * *
I cbn11 -6 66 0 "cbn" ly * *
I cbn12 -6 72 0 "cbn" ly * *
I cbn13 -6 78 0 "cbn" ly * *
I cbn14 -6 84 0 "cbn" ly * *
I cbn15 -6 90 0 "cbn" ly * *
I cbn16 -6 96 0 "cbn" ly * *
I cbn17 -6 102 0 "cbn" ly * *
I cbn18 -6 108 0 "cbn" ly * *
I cbn19 0 114 0 "cbn" ly * *
I via10 12 67 0 "cvia" ly * *
I cdn2 12 21 0 "cdn" ly * *
I cbn20 18 114 0 "cbn" ly * *
I cdp1 18 94 0 "cdp" ly * *
I cdp2 12 64 0 "cdp" ly * *
I cbn21 12 114 0 "cbn" ly * *
I cdp3 12 70 0 "cdp" ly * *
I cdp4 12 58 0 "cdp" ly * *
I cdp5 18 76 0 "cdp" ly * *
I cdp6 18 70 0 "cdp" ly * *
I cdp7 18 64 0 "cdp" ly * *
I cdp8 18 58 0 "cdp" ly * *
I cdp9 18 100 0 "cdp" ly * *
I cdn3 18 21 0 "cdn" ly * *
I cdp10 18 107 0 "cdp" ly * *
I cdp11 18 82 0 "cdp" ly * *
I cdp12 18 88 0 "cdp" ly * *
I cdp13 12 107 0 "cdp" ly * *
I cdp14 12 100 0 "cdp" ly * *
I cdp15 12 94 0 "cdp" ly * *
I cdp16 12 88 0 "cdp" ly * *
I cdp17 12 82 0 "cdp" ly * *
I cdp18 12 76 0 "cdp" ly * *
I cdn4 12 33 0 "cdn" ly * *
I cdn5 12 27 0 "cdn" ly * *
I cdn6 12 15 0 "cdn" ly * *
I cdn7 18 27 0 "cdn" ly * *
I cdn8 12 9 0 "cdn" ly * *
I cdn9 18 15 0 "cdn" ly * *
I cdn10 18 9 0 "cdn" ly * *
P 18 56 * pdif
P 18 109 * pdif
W 15 53 21 112
S 3 V pdif P 1 P 2
P 18 8 * ndif
P 18 33 * ndif
W 15 5 21 36
S 3 V ndif P 3 P 4
P 15 6 * ntrans
P 15 36 * ntrans
W 14 5 16 37
S 1 V ntrans P 5 P 6
P 12 8 * ndif
P 12 34 * ndif
W 9 5 15 37
S 3 V ndif P 7 P 8
P -31 120 * metal2
P 20 120 * metal2
W -39 112 28 128 vss1
S 8 H metal2 P 9 P 10
P 15 54 * ptrans
P 15 111 * ptrans
W 14 53 16 112
S 1 V ptrans P 11 P 12
P 18 9 * metal
P 18 107 * metal
W 17 8 19 108
S 1 V metal P 13 P 14
P 11 114 * ntie
P 13 114 * ntie
W 8 111 16 117
S 3 H ntie P 15 P 16
P 12 6 * metal
P 12 33 * metal
W 10 4 14 35
S 2 V metal P 17 P 18
P 18 0 * metal
P 18 9 * metal
W 17 -1 19 10 shr
S 1 V metal P 19 P 20
P -31 48 * metal2
P 12 48 * metal2
W -33 46 14 50 nshr
S 2 H metal2 P 21 P 22
P 0 8 * metal
P 0 39 * metal
W -2 6 2 41
S 2 V metal P 23 P 24
P 12 60 * metal2
P 20 60 * metal2
W 6 54 26 66 vdd
S 6 H metal2 P 25 P 26
P 12 60 * metal2
P 12 67 * metal2
W 9 57 15 70 vdd
S 3 V metal2 P 27 P 28
P -31 60 * metal2
P 12 60 * metal2
W -37 54 18 66 vdd
S 6 H metal2 P 29 P 30
P -31 68 * metal2
P 20 68 * metal2
W -51 48 40 88
S 20 H metal2 P 31 P 32
P -31 8 * metal2
P 20 8 * metal2
W -53 -14 42 30
S 22 H metal2 P 33 P 34
P 12 43 * metal
P 12 48 * metal
W 11 42 13 49
S 1 V metal P 35 P 36
P 12 43 * metal
P 14 43 * metal
W 10 41 16 45
S 2 H metal P 37 P 38
P 12 56 * pdif
P 12 109 * pdif
W 9 53 15 112
S 3 V pdif P 39 P 40
P 12 58 * metal
P 12 114 * metal
W 10 56 14 116
S 2 V metal P 41 P 42
P 12 114 * metal
P 18 114 * metal
W 10 112 20 116
S 2 H metal P 43 P 44
P 6 114 * metal
P 12 114 * metal
W 4 112 14 116
S 2 H metal P 45 P 46
P -6 114 * metal
P 6 114 * metal
W -8 112 8 116
S 2 H metal P 47 P 48
P -6 58 * metal
P -6 114 * metal
W -8 56 -4 116
S 2 V metal P 49 P 50
P 6 58 * metal
P 6 114 * metal
W 4 56 8 116
S 2 V metal P 51 P 52
P 15 43 * poly
P 15 54 * poly
W 14 42 16 55
S 1 V poly P 53 P 54
P 15 36 * poly
P 15 43 * poly
W 14 35 16 44
S 1 V poly P 55 P 56
P 14 43 * poly
P 15 43 * poly
W 13 42 16 44
S 1 H poly P 57 P 58
P -32 83 * nwell
P 21 83 * nwell
W -100 15 89 151
S 68 H nwell P 59 P 60
P -31 0 * metal2
P 20 0 * metal2
W -39 -8 28 8 vss
S 8 H metal2 P 61 P 62
P -31 23 * allowM2
P 20 23 * allowM2
W -33 21 22 25
S 2 H allowM2 P 63 P 64
P -31 28 * allowM2
P 20 28 * allowM2
W -33 26 22 30
S 2 H allowM2 P 65 P 66
P -31 33 * allowM2
P 20 33 * allowM2
W -33 31 22 35
S 2 H allowM2 P 67 P 68
P -31 38 * allowM2
P 20 38 * allowM2
W -33 36 22 40
S 2 H allowM2 P 69 P 70
P -31 82 * allowM2
P 20 82 * allowM2
W -33 80 22 84
S 2 H allowM2 P 71 P 72
P -31 87 * allowM2
P 20 87 * allowM2
W -33 85 22 89
S 2 H allowM2 P 73 P 74
P -31 92 * allowM2
P 20 92 * allowM2
W -33 90 22 94
S 2 H allowM2 P 75 P 76
P -31 97 * allowM2
P 20 97 * allowM2
W -33 95 22 99
S 2 H allowM2 P 77 P 78
P -31 102 * allowM2
P 20 102 * allowM2
W -33 100 22 104
S 2 H allowM2 P 79 P 80
P -31 107 * allowM2
P 20 107 * allowM2
W -33 105 22 109
S 2 H allowM2 P 81 P 82
E
