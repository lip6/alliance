#ifndef _OCRASTAR_H
#define _OCRASTAR_H

/*
   ### -------------------------------------------------- ###
   $Author: hcl $
   $Date: 2002/03/15 14:37:14 $
   $Log: ocrAstar.h,v $
   Revision 1.1  2002/03/15 14:37:14  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:11  hcl
   First commit in the new tree.

   Revision 1.1  2002/02/12 15:13:59  hcl
   verbosity fix.

   ### -------------------------------------------------- ###
*/

#define AS_K_SEG             0
#define AS_K_EQUI            1

ocrNaturalInt find_path_astar (ocrRoutingParameters * p_param,
                      ocrWRoutingGrid * p_grid,
                      ocrNaturalInt xsource,
                      ocrNaturalInt ysource,
                      ocrNaturalInt zsource,
                      ocrNaturalInt xtarget,
                      ocrNaturalInt ytarget,
                      ocrNaturalInt ztarget,
                      ocrNaturalInt signal_index,
                      ocrSignal * i_pSignal,
                      ocrNaturalInt mode);

#endif
