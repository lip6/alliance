#ifndef __DISTANCE_H
#define __DISTANCE_H

#include "struct.h"

extern long distance_con_con(long x1, long y1, long w1, char l1,
	long x2, long y2, long w2, char l2, int face);

PT_COORDONNEES existe_con_precalim(long x2, long y2, long w2, char l2, int face,
	PT_COORDONNEES liste_coor, long distance);

extern PT_COORDONNEES existe_con_suiv(long x1, long y1, long w1, char l1,
	int face, PT_COORDONNEES liste_coor);

#endif /* __DISTANCE_H */
