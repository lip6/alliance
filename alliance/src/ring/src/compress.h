#ifndef __COMPRESS_H
#define __COMPRESS_H

#include <mut.h>
#include <mph.h>
#include "struct.h"

extern void firstlastinst_barre(phfig_list *barre, phins_list **firstinst, phins_list **lastinst);

extern void calcul_nbpiste_libre(BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur, 
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long nblibres[NB_FACES], long firstlibre[NB_FACES]);

extern void compression_canaux(BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long nblibres[NB_FACES], long firstlibre[NB_FACES]);

#endif /* __COMPRESS_H */
