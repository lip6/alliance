#ifndef __BARRE_H
#define __BARRE_H

#include "struct.h"

extern void definir_fig_ph(char *nomfig);
extern void abouteright_plot_fig(char *nomfig, char *nominst);
extern void abouteleft_plot_fig(char *nomfig, char *nominst);
extern void definir_reference_inst(char *name);
extern void definir_ab_fig();
extern void definir_ab_fig2();
extern void sauver_fig_ph();
extern void remonter_consud_instph(char *nominst);
extern void remonter_connord_instph(char *nominst);
extern void placer_instph(char *nomfig, char *nominst, char symetrie,
	long x, long y);
extern void placer_segph(char layer, long width, char *nameseg,
	long x1, long y1, long x2, long y2);
extern void miseaplat_instph(char *insname, char cheminom);
extern void remonter_uncon_instph(long index, char *locname, char *insname, char *newname);
extern void poser_wire1(char layer, long width, char* ins1, char *con1,
	long index1, char* ins2, char* con2,long index2);

extern void poser_wire2(char layer, long width, char* ins1, char* con1,
	long index1, char* ins2, char* con2, long index2, long x, long y);

#endif /* __BARRE_H */
