#cell1 bshlmxi32_c idps_symb compose *
# 16-Aug-93 17:10 16-Aug-93 17:10 mbk305 * .cp
# .
V 4 VTIcompose 1.1
A -2 0 25 120
B -2 0 25 120
F F
C nleft 25 112 metal2 2 1 * * E nleft
C asr 22 0 metal 1 2 * * S asr
C nasr 25 43 metal 1 3 * * E nasr
C nshl 25 53 metal2 2 4 * * E nshl
C nnleft 25 48 metal2 2 5 * * E nnleft
C vdd 25 60 metal2 6 6 * * E vdd
C vss 25 0 metal2 8 7 * * E vss
C nleft -2 112 metal2 2 8 * * W nleft
C nnleft -2 48 metal2 2 9 * * W nnleft
C lsl 18 0 metal 1 10 * * S lsl
C vss1 25 120 metal2 8 11 * * E vss1
C vss -2 0 metal2 8 12 * * W vss
C vss1 -2 120 metal2 8 13 * * W vss1
C vdd -2 60 metal2 6 14 * * W vdd
I cbn1 24 84 0 "cbn" ly * *
I cbn2 24 72 0 "cbn" ly * *
I cbp1 12 39 0 "cbp" ly * *
I cdp1 18 107 0 "cdp" ly * *
I via1 24 76 0 "cvia" ly * *
I via2 24 68 0 "cvia" ly * *
I via3 24 58 0 "cvia" ly * *
I cbn3 24 64 0 "cbn" ly * *
I cbn4 24 90 0 "cbn" ly * *
I cbn5 24 96 0 "cbn" ly * *
I cbn6 24 102 0 "cbn" ly * *
I cbn7 24 108 0 "cbn" ly * *
I via4 12 53 0 "cvia" ly * *
I cpf1 22 43 0 "cpf" ly * *
I via5 12 12 0 "cvia" ly * *
I via6 12 18 0 "cvia" ly * *
I via7 12 61 0 "cvia" ly * *
I cpf2 10 48 0 "cpf" ly * *
I cbn8 24 114 0 "cbn" ly * *
I via8 12 67 0 "cvia" ly * *
I cdn1 12 21 0 "cdn" ly * *
I cbn9 6 114 0 "cbn" ly * *
I cbn10 18 114 0 "cbn" ly * *
I cdp2 18 94 0 "cdp" ly * *
I cdp3 12 64 0 "cdp" ly * *
I cbn11 12 114 0 "cbn" ly * *
I cdp4 12 70 0 "cdp" ly * *
I cdp5 12 58 0 "cdp" ly * *
I cdp6 18 76 0 "cdp" ly * *
I cdp7 18 70 0 "cdp" ly * *
I cdp8 18 64 0 "cdp" ly * *
I cdp9 18 58 0 "cdp" ly * *
I cdp10 6 58 0 "cdp" ly * *
I cdp11 6 64 0 "cdp" ly * *
I cdp12 6 70 0 "cdp" ly * *
I cdp13 6 76 0 "cdp" ly * *
I cdp14 18 100 0 "cdp" ly * *
I cdp15 6 82 0 "cdp" ly * *
I cdp16 6 94 0 "cdp" ly * *
I cdn2 18 33 0 "cdn" ly * *
I cdn3 6 21 0 "cdn" ly * *
I cdn4 18 21 0 "cdn" ly * *
I cdp17 6 107 0 "cdp" ly * *
I cdp18 18 82 0 "cdp" ly * *
I cdp19 6 100 0 "cdp" ly * *
I cdp20 6 88 0 "cdp" ly * *
I cdp21 18 88 0 "cdp" ly * *
I cdp22 12 107 0 "cdp" ly * *
I cdp23 12 100 0 "cdp" ly * *
I cdp24 12 94 0 "cdp" ly * *
I cdp25 12 88 0 "cdp" ly * *
I cdp26 12 82 0 "cdp" ly * *
I cdp27 12 76 0 "cdp" ly * *
I cdn5 12 33 0 "cdn" ly * *
I cdn6 12 27 0 "cdn" ly * *
I cdn7 12 15 0 "cdn" ly * *
I cdn8 6 33 0 "cdn" ly * *
I cdn9 6 27 0 "cdn" ly * *
I cdn10 18 27 0 "cdn" ly * *
I cdn11 6 15 0 "cdn" ly * *
I cdn12 6 9 0 "cdn" ly * *
I cdn13 12 9 0 "cdn" ly * *
I cdn14 18 15 0 "cdn" ly * *
I cdn15 18 9 0 "cdn" ly * *
P 12 56 * pdif
P 12 109 * pdif
W 9 53 15 112
S 3 V pdif P 1 P 2
P 18 56 * pdif
P 18 109 * pdif
W 15 53 21 112
S 3 V pdif P 3 P 4
P 18 8 * ndif
P 18 33 * ndif
W 15 5 21 36
S 3 V ndif P 5 P 6
P 15 6 * ntrans
P 15 36 * ntrans
W 14 5 16 37
S 1 V ntrans P 7 P 8
P 12 8 * ndif
P 12 34 * ndif
W 9 5 15 37
S 3 V ndif P 9 P 10
P 6 8 * ndif
P 6 33 * ndif
W 3 5 9 36
S 3 V ndif P 11 P 12
P -2 120 * metal2
P 25 120 * metal2
W -10 112 33 128 vss1
S 8 H metal2 P 13 P 14
P 6 56 * pdif
P 6 109 * pdif
W 3 53 9 112
S 3 V pdif P 15 P 16
P 9 54 * ptrans
P 9 111 * ptrans
W 8 53 10 112
S 1 V ptrans P 17 P 18
P 15 54 * ptrans
P 15 111 * ptrans
W 14 53 16 112
S 1 V ptrans P 19 P 20
P 11 114 * ntie
P 13 114 * ntie
W 8 111 16 117
S 3 H ntie P 21 P 22
P 12 9 * metal
P 12 39 * metal
W 10 7 14 41
S 2 V metal P 23 P 24
P 6 9 * metal
P 6 107 * metal
W 5 8 7 108
S 1 V metal P 25 P 26
P 9 6 * ntrans
P 9 36 * ntrans
W 8 5 10 37
S 1 V ntrans P 27 P 28
P 12 58 * metal
P 12 114 * metal
W 10 56 14 116
S 2 V metal P 29 P 30
P 12 60 * metal2
P 12 67 * metal2
W 9 57 15 70 vdd
S 3 V metal2 P 31 P 32
P 12 60 * metal2
P 25 60 * metal2
W 6 54 31 66 vdd
S 6 H metal2 P 33 P 34
P -2 60 * metal2
P 12 60 * metal2
W -8 54 18 66 vdd
S 6 H metal2 P 35 P 36
P -2 48 * metal2
P 25 48 * metal2
W -4 46 27 50 nnleft
S 2 H metal2 P 37 P 38
P -2 0 * metal2
P 12 0 * metal2
W -10 -8 20 8 vss
S 8 H metal2 P 39 P 40
P 12 0 * metal2
P 25 0 * metal2
W 4 -8 33 8 vss
S 8 H metal2 P 41 P 42
P -2 112 * metal2
P 25 112 * metal2
W -4 110 27 114 nleft
S 2 H metal2 P 43 P 44
P -2 68 * metal2
P 25 68 * metal2
W -22 48 45 88
S 20 H metal2 P 45 P 46
P 12 53 * metal2
P 25 53 * metal2
W 10 51 27 55
S 2 H metal2 P 47 P 48
P 22 43 * metal
P 25 43 * metal
W 21 42 26 44 nasr
S 1 H metal P 49 P 50
P 15 43 * poly
P 15 54 * poly
W 14 42 16 55
S 1 V poly P 51 P 52
P 15 42 * poly
P 15 43 * poly
W 14 41 16 44
S 1 V poly P 53 P 54
P 15 43 * poly
P 22 43 * poly
W 14 42 23 44
S 1 H poly P 55 P 56
P 15 36 * poly
P 15 42 * poly
W 14 35 16 43
S 1 V poly P 57 P 58
P -2 8 * metal2
P 25 8 * metal2
W -24 -14 47 30
S 22 H metal2 P 59 P 60
P 6 4 * metal
P 18 4 * metal
W 5 3 19 5 lsl
S 1 H metal P 61 P 62
P 6 4 * metal
P 6 9 * metal
W 5 3 7 10 lsl
S 1 V metal P 63 P 64
P 18 0 * metal
P 18 4 * metal
W 17 -1 19 5 lsl
S 1 V metal P 65 P 66
P 9 49 * poly
P 9 54 * poly
W 8 48 10 55
S 1 V poly P 67 P 68
P 12 48 * metal
P 12 53 * metal
W 11 47 13 54
S 1 V metal P 69 P 70
P 10 48 * metal
P 12 48 * metal
W 8 46 14 50
S 2 H metal P 71 P 72
P 9 48 * poly
P 9 49 * poly
W 8 47 10 50
S 1 V poly P 73 P 74
P 9 36 * poly
P 9 48 * poly
W 8 35 10 49
S 1 V poly P 75 P 76
P 9 48 * poly
P 10 48 * poly
W 8 47 11 49
S 1 H poly P 77 P 78
P 22 0 * metal
P 22 9 * metal
W 21 -1 23 10 asr
S 1 V metal P 79 P 80
P 18 9 * metal
P 22 9 * metal
W 17 8 23 10 asr
S 1 H metal P 81 P 82
P 18 9 * metal
P 18 107 * metal
W 17 8 19 108 asr
S 1 V metal P 83 P 84
P 12 114 * metal
P 24 114 * metal
W 10 112 26 116
S 2 H metal P 85 P 86
P 24 58 * metal
P 24 114 * metal
W 22 56 26 116
S 2 V metal P 87 P 88
P -3 83 * nwell
P 26 83 * nwell
W -71 15 94 151
S 68 H nwell P 89 P 90
P 6 114 * metal
P 12 114 * metal
W 4 112 14 116
S 2 H metal P 91 P 92
P -2 23 * allowM2
P 25 23 * allowM2
W -4 21 27 25
S 2 H allowM2 P 93 P 94
P -2 28 * allowM2
P 25 28 * allowM2
W -4 26 27 30
S 2 H allowM2 P 95 P 96
P -2 33 * allowM2
P 25 33 * allowM2
W -4 31 27 35
S 2 H allowM2 P 97 P 98
P -2 38 * allowM2
P 25 38 * allowM2
W -4 36 27 40
S 2 H allowM2 P 99 P 100
P -2 107 * allowM2
P 25 107 * allowM2
W -4 105 27 109
S 2 H allowM2 P 101 P 102
P -2 102 * allowM2
P 25 102 * allowM2
W -4 100 27 104
S 2 H allowM2 P 103 P 104
P -2 97 * allowM2
P 25 97 * allowM2
W -4 95 27 99
S 2 H allowM2 P 105 P 106
P -2 92 * allowM2
P 25 92 * allowM2
W -4 90 27 94
S 2 H allowM2 P 107 P 108
P -2 87 * allowM2
P 25 87 * allowM2
W -4 85 27 89
S 2 H allowM2 P 109 P 110
P -2 82 * allowM2
P 25 82 * allowM2
W -4 80 27 84
S 2 H allowM2 P 111 P 112
E
