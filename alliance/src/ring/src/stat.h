#ifndef __STAT_H
#define __STAT_H

#include "struct.h"

extern void remplit_stat(LST_SEGMENT seg, LST_EQUIPO eq);
extern void finir_stat(LST_EQUIPO eq);
extern void maj_stat_vias(LST_EQUIPO eq, long ancr, long ancp, long r, long p);

#endif /* __STAT_H */
