/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:22 $
   $Log: ocrUtil.c,v $
   Revision 1.1  2002/03/15 14:37:22  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:20  hcl
   First commit in the new tree.

   Revision 1.2  2001/12/14 15:58:39  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.1  2001/09/26 14:27:51  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"
#include "ocr.h"
#include "ocrUtil.h"

#define LAYER_PARITY_MASK  (ocrNaturalInt)1

static char *res_id = "$Id: ocrUtil.c,v 1.1 2002/03/15 14:37:22 hcl Exp $";


ocrRoutingParameters *setParameters(ocrNaturalShort pitch_h,
                                    ocrNaturalShort pitch_v,
                                    ocrNaturalShort via_cost,
                                    ocrRoutingDirection dir_of_even_layers)
{
    ocrRoutingParameters *pt =
        (ocrRoutingParameters *) mbkalloc(sizeof(ocrRoutingParameters));

    pt->PITCH_H = pitch_h;
    pt->PITCH_V = pitch_v;
    pt->VIA_COST = via_cost;
    pt->EVEN_LAYERS_DIRECTION = dir_of_even_layers;
    return pt;
}

/** get L1 distance (manhattan distance) in a 3D space **/

ocrNaturalInt
manhattan3Distance(ocrRoutingParameters * param,
                   ocrNaturalInt x1,
                   ocrNaturalInt y1,
                   ocrNaturalInt z1,
                   ocrNaturalInt x2, ocrNaturalInt y2, ocrNaturalInt z2)
{

    return (param->PITCH_H * ABSDIFF(x1, x2) +
            param->PITCH_V * ABSDIFF(y1, y2) +
            param->VIA_COST * ABSDIFF(z1, z2));
}

/** get L1 distance (manhattan distance) in a plane **/

ocrNaturalInt
manhattan2Distance(ocrRoutingParameters * param,
                   ocrNaturalInt x1,
                   ocrNaturalInt y1, ocrNaturalInt x2, ocrNaturalInt y2)
{

    return (param->PITCH_H * ABSDIFF(x1, x2) +
            param->PITCH_V * ABSDIFF(y1, y2));
}

/** get direction of a layer **/

ocrRoutingDirection
getDirection(ocrRoutingParameters * param, ocrNaturalInt z)
{

    return ((z & LAYER_PARITY_MASK) ?
            (ocrRoutingDirection) (!(param->EVEN_LAYERS_DIRECTION))
            : param->EVEN_LAYERS_DIRECTION);
}
