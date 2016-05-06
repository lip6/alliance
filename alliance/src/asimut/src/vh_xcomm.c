
/* ###--------------------------------------------------------------### */
/* file		: vh_xcomm.c						*/
/* date		: Jun  3 1997						*/
/* version	: v3.0							*/
/* author	: VUONG H.N. , Pirouz BAZARAGAN SABET			*/
/* description	: This file contains commomn functions relative to the	*/
/*		  execution of a circuit :				*/
/*									*/
/*		  vhx_getvalu , vhx_getsize        , vhx_gexeval	*/
/*		  vhx_muxbit  , vhx_worbit         , vhx_regbit		*/
/*		  vhx_muxpubit, vhx_worpubit            		*/
/*		  vhx_bddeval , vhx_savall         , vhx_inireg		*/
/*		  vhx_redpat  , vhx_wrtpat				*/
/*		  vhx_chkerr  , vhx_initfig        , vhx_initval	*/
/*		  vhx_core    , vhx_readinput      , vhx_writebus	*/
/*		  vhx_writeout, vhx_GetInstanceName			*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mut.h"
#include "beh.h"
#include "log.h"
#include "pat.h"
#include "sch.h"
#include "vh_util.h"
#include "vh_ltype.h"
#include "vh_globals.h"
#include "vh_xspec.h"
#include "vh_xcomm.h"

/* ###--------------------------------------------------------------### */
/* function	: vhx_getvalu						*/
/* description	: for the first call the function makes memory		*/
/*		  allocation to save the value of static internal	*/
/*		  signals of a description using a C model. For further	*/
/*		  calls it simply search the already allocated space.	*/
/*		  The pointer of the space is passed as argument to the	*/
/*		  function. The function returns the size of the memory	*/
/*		  space. Each instance has its own memory space.	*/
/*									*/
/*		  In addition, during the first call, once the memory	*/
/*		  allocation is done, a user defined function (passed	*/
/*		  as an argument) is called. This feature can be used	*/
/*		  to handle actions such as reading input file, giving	*/
/*		  an initial value to registers, etc.. The pointer of	*/
/*		  the instance and the pointer of the memory space	*/
/*		  relative to the instance are passed as arguments to	*/
/*		  this function.					*/
/*									*/
/* called funct	: mbkalloc, addht, gethtitem, addhtitem,		*/
/* ###--------------------------------------------------------------### */

unsigned long vhx_getvalu (pt_lkdins, pt_ptmem, size, pt_func)

struct lkdins   *pt_lkdins  ;
char           **pt_ptmem   ;
unsigned int     size       ;
void          (* pt_func) ();

  {
  char      *pt_mem   = NULL;
  static ht *mem_tabl = NULL;
  static ht *siz_tabl = NULL;

  if (mem_tabl == NULL)
    {
    mem_tabl = addht (64);
    siz_tabl = addht (64);
    }

  pt_mem = (char *) gethtitem (mem_tabl, pt_lkdins);

  if (((long) pt_mem) == EMPTYHT)
    {
    pt_mem = mbkalloc (size);
    addhtitem (mem_tabl, pt_lkdins, (long) pt_mem);
    addhtitem (siz_tabl, pt_lkdins,       size  );

    if (pt_func != NULL)
      (* pt_func) (pt_lkdins, pt_mem);
    }

  size      = (unsigned long) gethtitem (siz_tabl, pt_lkdins);
  *pt_ptmem = pt_mem;

  return (size);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_getsize						*/
/* description	: get the size of a description				*/
/* called func.	: vhx_getvalu						*/
/* ###--------------------------------------------------------------### */

unsigned long vhx_getsize (pt_lkdfig)

struct lkdfig *pt_lkdfig;

  {
  unsigned int   size      = 0;
  struct simsig *pt_simsig ;
  struct bussig *pt_bussig ;
  struct prjbvl *pt_prjbvl ;
  struct lkdins *pt_lkdins ;
  struct wriaux *pt_wriaux ;
  struct wribux *pt_wribux ;
  struct wrireg *pt_wrireg ;
  struct prjrvl *pt_prjrvl ;
  char          *pt_mem    ;

  if (pt_lkdfig->VALSIZ == 0)
    {
    pt_simsig = pt_lkdfig->SIMSIG;
    while (pt_simsig != NULL)
      {
      size      += 2;
      pt_simsig  = pt_simsig->NEXT;
      }

    pt_bussig = pt_lkdfig->BUSSIG;
    while (pt_bussig != NULL)
      {
      pt_prjbvl = pt_bussig->PRJBVL;
      while (pt_prjbvl != NULL)
        {
        size      += 2;
        pt_prjbvl  = pt_prjbvl->GLNEXT;
        }
      size ++;
      pt_bussig = pt_bussig->NEXT;
      }

    pt_lkdins = pt_lkdfig->LKDINS;
    while (pt_lkdins != NULL)
      {
      if (pt_lkdins->FLAG == VHL_CMODEL)
        size += vhx_getvalu (pt_lkdins, &pt_mem, 0);
      else
        {
        pt_wriaux = pt_lkdins->WRIAUX;
        while (pt_wriaux != NULL)
          {
          size      += 2;
          pt_wriaux  = pt_wriaux->NEXT;
          }

        pt_wribux = pt_lkdins->WRIBUX;
        while (pt_wribux != NULL)
          {
          pt_prjbvl = pt_wribux->PRJBVL;
          while (pt_prjbvl != NULL)
            {
            size      += 2;
            pt_prjbvl  = pt_prjbvl->NEXT;
            }
          size ++;
          pt_wribux = pt_wribux->NEXT;
          }

        pt_wrireg = pt_lkdins->WRIREG;
        while (pt_wrireg != NULL)
          {
          pt_prjrvl = pt_wrireg->PRJRVL;
          while (pt_prjrvl != NULL)
            {
            size      += 2;
            pt_prjrvl  = pt_prjrvl->NEXT;
            }
          size ++;
          pt_wrireg = pt_wrireg->NEXT;
          }
        }
      pt_lkdins = pt_lkdins->NEXT;
      }

    pt_lkdfig->VALSIZ = size;
    }

  return (pt_lkdfig->VALSIZ);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_gexeval						*/
/* description	: computes a GEX's value depending on its node's value	*/
/* called func.	: vhu_toolbug						*/
/* ###--------------------------------------------------------------### */

char vhx_gexeval (pt_lkdins, gex)

struct lkdins *pt_lkdins;
struct magex  *gex      ;

  {
  int           indexgex;
  struct chain *expr    ;
  char          result  = 'u';
  char          res     ;

  if (gex->OPERAND != NULL)
    {
    expr = gex->OPERAND;

    switch (gex->TERM)
      {
      case OR:
        result = '0';
        while (expr != NULL)
          {
          res = vhx_gexeval (pt_lkdins, (struct magex *) expr->DATA);
          if (res == '1')
            {
            result = '1';
            break;
            }
          if (res == 'u')
            {
            result = 'u';
            }
          expr = expr->NEXT;
          }
        break;

      case AND:
        result = '1';
        while (expr != NULL)
          {
          res = vhx_gexeval (pt_lkdins, (struct magex *) expr->DATA);
          if (res == '0')
            {
            result = '0';
            break;
            }
          if (res == 'u')
            {
            result = 'u';
            }
          expr = expr->NEXT;
          }
        break;

      case XOR:
        result = 0;
        while (expr != NULL)
          {
          res = vhx_gexeval (pt_lkdins, (struct magex *)expr->DATA) - '0';
          if (res == ('u' - '0'))
            {
            result = 'u' - '0';
            break;
            }
          else
            result ^= res;
          expr = expr->NEXT;
          }
        result += '0';
        break;

      case NOT:
        result = vhx_gexeval (pt_lkdins, (struct magex *)expr->DATA);
        if (result == '1')
          result = '0';
        else if (result == '0')
          result = '1';
        break;

      case NOR:
        result = '0';
        while (expr != NULL)
          {
          res = vhx_gexeval (pt_lkdins, (struct magex *) expr->DATA);
          if (res == '1')
            {
            result = '1';
            break;
            }
          if (res == 'u')
            {
            result = 'u';
            }
          expr = expr->NEXT;
          }
        if (result == '1')
          result = '0';
        else if (result == '0')
          result = '1';
        break;

      case NAND:
        result = '1';
        while (expr != NULL)
          {
          res = vhx_gexeval (pt_lkdins, (struct magex *) expr->DATA);
          if (res == '0')
            {
            result = '0';
            break;
            }
          if (res == 'u')
            {
            result = 'u';
            }
          expr = expr->NEXT;
          }
        if (result == '1')
          result = '0';
        else if (result == '0')
          result = '1';
        break;

      case NXOR:
        result = 1;
        while (expr != NULL)
          {
          res = vhx_gexeval (pt_lkdins, (struct magex *)expr->DATA) - '0';
          if (res == ('u' - '0'))
            {
            result = 'u' - '0';
            break;
            }
          else
            result ^= res;
          expr = expr->NEXT;
          }
        result += '0';
        break;

      default:
        vhu_toolbug (1, "vhx_gexeval", NULL, 0);
      }
    }
  else
    {
    indexgex = gex->TERM;
    if (indexgex > 1)
      result = (*((pt_lkdins->REDLST + indexgex - 2)->VALU));
    else
      result = indexgex + '0';
    }

  return (result);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_muxbit						*/
/* description	: resolution function for MUX_BIT typed signals		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char vhx_muxbit (pt_prjbvl)

struct prjbvl *pt_prjbvl;

  {
  char resval      = 'u';
  int  active_driv =  0 ;
  int  unknown_driv = 0 ;

  while (pt_prjbvl != NULL)
    {
    if (pt_prjbvl->CNDVAL == '1')
      {
      resval = pt_prjbvl->DRVVAL;
      active_driv++;
      }
    else if (pt_prjbvl->CNDVAL == 'u')
      {
      resval = 'u';
      unknown_driv++;
      }

    pt_prjbvl = pt_prjbvl->GLNEXT;
    }

  if ((active_driv + unknown_driv) > 1)
    resval = 'E';

  return (resval);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_worbit						*/
/* description	: resolution function for WOR_BIT typed signals		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char vhx_worbit (pt_prjbvl)

struct prjbvl *pt_prjbvl;

  {
  char resval      = 'u';
  int  active_unknown  = 0  ;
  int  active_one  = 0  ;
  int  active_zero = 0  ;

  while (pt_prjbvl != NULL)
    {
    if (pt_prjbvl->CNDVAL == '1')
      {
      resval = pt_prjbvl->DRVVAL;
      if (resval == '1')
        active_one  = 1;
      else if (resval == '0')
        active_zero = 1;
      else if (resval == 'u')
        active_unknown++;
      }
    else if (pt_prjbvl->CNDVAL == 'u')
      {
      resval = pt_prjbvl->DRVVAL;
      if (resval == '1')
        active_one  = 1;
      else if (resval == '0')
        active_zero = 1;
      else if (resval == 'u')
        active_unknown++;
      }

    pt_prjbvl = pt_prjbvl->GLNEXT;
    }

  if ((active_one + active_zero + active_unknown) > 1)
    resval = 'E';

  return (resval);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_muxpubit						*/
/* description	: resolution function for MUX_BIT PULL-UP typed signals	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char vhx_muxpubit (pt_prjbvl)

struct prjbvl *pt_prjbvl;

  {
  char resval      = '1';
  int  active_driv = 0  ;
  int  unknown_driv = 0  ;

  while (pt_prjbvl != NULL)
    {
    if (pt_prjbvl->CNDVAL == '1')
      {
      resval = pt_prjbvl->DRVVAL;
      active_driv++;
      }
    else
      {
      if (pt_prjbvl->CNDVAL == 'u')
        {
        resval = pt_prjbvl->DRVVAL;
        unknown_driv++;
        }
      }

    pt_prjbvl = pt_prjbvl->GLNEXT;
    }

  if ((active_driv + unknown_driv) > 1)
    resval = 'u';

  return (resval);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_worpubit						*/
/* description	: resolution function for WOR_BIT PULL-UP typed signals	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char vhx_worpubit (pt_prjbvl)

struct prjbvl *pt_prjbvl;

  {
  char resval      = '1';
  int  active_one  = 0  ;
  int  active_zero = 0  ;
  int  active_unknown = 0  ;

  while (pt_prjbvl != NULL)
    {
    if (pt_prjbvl->CNDVAL == '1')
      {
      resval = pt_prjbvl->DRVVAL;
      if (resval == '1')
        active_one  = 1;
      else if (resval == '0')
        active_zero = 1;
      else if (resval == 'u')
        active_unknown++;
      }
    else if (pt_prjbvl->CNDVAL == 'u')
      {
      resval = pt_prjbvl->DRVVAL;
      if (pt_prjbvl->DRVVAL == '1')
        active_one  = 1;
      else if (pt_prjbvl->DRVVAL == '0')
        active_zero = 1;
      else if (pt_prjbvl->DRVVAL == 'u')
        active_unknown++;
      }

    pt_prjbvl = pt_prjbvl->GLNEXT;
    }

  if ((active_one + active_zero + active_unknown) > 1)
    resval = 'u';

  return (resval);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_regbit						*/
/* description	: resolution function for REG_BIT typed signals		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char vhx_regbit (pt_prjrvl)

struct prjrvl *pt_prjrvl;

  {
  char resval      = 'Z';
  int  active_driv = 0  ;
  int  unknown_driv = 0  ;

  while (pt_prjrvl != NULL)
    {
    if (pt_prjrvl->CNDVAL == '1')
      {
      resval = pt_prjrvl->DRVVAL;
      active_driv++;
      }
    else
      {
      if (pt_prjrvl->CNDVAL == 'u')
        {
        resval      = 'u';
        unknown_driv++;
        }
      }

    pt_prjrvl = pt_prjrvl->NEXT;
    }

  if ((active_driv + unknown_driv) > 1)
    resval = 'E';

  return (resval);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_savall						*/
/* description	: save the state of the circuit on a binary file	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhx_savall (fp, pt_lkdfig)

FILE          *fp       ;
struct lkdfig *pt_lkdfig;

  {
  struct simsig *pt_simsig = pt_lkdfig->SIMSIG;
  struct bussig *pt_bussig = pt_lkdfig->BUSSIG;
  struct lkdins *pt_lkdins = pt_lkdfig->LKDINS;
  unsigned int   size      = 0                ;
  char          *pt_mem    = NULL             ;
  struct wrireg *pt_wrireg ;
  struct wriaux *pt_wriaux ;
  struct wribux *pt_wribux ;
  struct prjbvl *pt_prjbvl ;
  struct prjrvl *pt_prjrvl ;

	/* ###------------------------------------------------------### */
	/*    get the size of the circuit (number of bytes needed to	*/
	/* store the value of all signals)				*/
	/* ###------------------------------------------------------### */

  vhx_getsize (pt_lkdfig);

	/* ###------------------------------------------------------### */
	/*    write the size as a header in the save file (for further	*/
	/* checks)							*/
	/* ###------------------------------------------------------### */

  fwrite (&(pt_lkdfig->VALSIZ), sizeof (unsigned int), 1, fp);

	/* ###------------------------------------------------------### */
	/*    save the value of simple and bussed signals:		*/
	/*      - a current and a projected value for simple signals	*/
	/*      - a current and a list of projected condition and value	*/
	/*        for bussed signals					*/
	/* ###------------------------------------------------------### */

  while (pt_simsig != NULL)
    {
    fputc (  pt_simsig->PRJVAL , fp);
    fputc (*(pt_simsig->CURVAL), fp);

    pt_simsig = pt_simsig->NEXT;
    }

  while (pt_bussig != NULL)
    {
    pt_prjbvl = pt_bussig->PRJBVL;
    while (pt_prjbvl != NULL)
      {
      fputc (pt_prjbvl->DRVVAL, fp);
      fputc (pt_prjbvl->CNDVAL, fp);

      pt_prjbvl = pt_prjbvl->GLNEXT;
      }
    fputc (*(pt_bussig->CURVAL), fp);
    pt_bussig = pt_bussig->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    for each instance;					*/
	/*      - if the instance uses a C model save the value of	*/
	/*        the static variables used by the model		*/
	/*      - for other instances sav the current and projected	*/
	/*        values of internal signals (simple and bussed		*/
	/*        internal signals and internal registers)		*/
	/* ###------------------------------------------------------### */

  while (pt_lkdins != NULL)
    {
    if (pt_lkdins->FLAG == VHL_CMODEL)
      {
      size = vhx_getvalu (pt_lkdins, &pt_mem, 0);
      fwrite (pt_mem, sizeof (char), size, fp);
      }
    else
      {
      pt_wrireg = pt_lkdins->WRIREG;
      pt_wriaux = pt_lkdins->WRIAUX;
      pt_wribux = pt_lkdins->WRIBUX;

      while (pt_wrireg != NULL)
        {
        pt_prjrvl = pt_wrireg->PRJRVL;
        while (pt_prjrvl != NULL)
          {
          fputc (pt_prjrvl->DRVVAL, fp);
          fputc (pt_prjrvl->CNDVAL, fp);

          pt_prjrvl = pt_prjrvl->NEXT;
          }
        fputc (*(pt_wrireg->CURVAL), fp);
        pt_wrireg = pt_wrireg->NEXT;
        }

      while (pt_wribux != NULL)
        {
        pt_prjbvl = pt_wribux->PRJBVL;
        while (pt_prjbvl != NULL)
          {
          fputc (pt_prjbvl->DRVVAL, fp);
          fputc (pt_prjbvl->CNDVAL, fp);

          pt_prjbvl = pt_prjbvl->NEXT;
          }
        fputc (*(pt_wribux->CURVAL), fp);
        pt_wribux = pt_wribux->NEXT;
        }

      while (pt_wriaux != NULL)
        {
        fputc (  pt_wriaux->PRJVAL , fp);
        fputc (*(pt_wriaux->CURVAL), fp);

        pt_wriaux = pt_wriaux->NEXT;
        }
      }

    pt_lkdins = pt_lkdins->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_vectorinit					*/
/* description	: Dedicated vector statistic function.			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhx_vectorinit (pt_lkdfig)

struct lkdfig *pt_lkdfig;

  {
  struct simsig *pt_simsig = pt_lkdfig->SIMSIG;
  struct bussig *pt_bussig = pt_lkdfig->BUSSIG;
  struct lkdins *pt_lkdins = pt_lkdfig->LKDINS;
  struct wrireg *pt_wrireg ;
  struct wriaux *pt_wriaux ;
  struct wribux *pt_wribux ;
  unsigned int   i;

	/* ###------------------------------------------------------### */
	/*    get the size of the circuit (number of bytes needed to	*/
	/* store the value of all signals)				*/
	/* ###------------------------------------------------------### */

  vhx_getsize (pt_lkdfig);


	/* ###------------------------------------------------------### */
	/*    write the size as a header in the save file (for further	*/
	/* checks)							*/
	/* ###------------------------------------------------------### */

  if  ( VHX_VECTOR_VALUE == NULL )
    {
    VHX_VECTOR_VALUE = mbkalloc ( pt_lkdfig->VALSIZ * sizeof (unsigned char) );

    for ( i=0; i < pt_lkdfig->VALSIZ; i++)
      VHX_VECTOR_VALUE[i] = '0';
    }

  if  ( VHX_VECTOR_TRANS == NULL )
    {
    VHX_VECTOR_TRANS = mbkalloc ( pt_lkdfig->VALSIZ * sizeof (unsigned int) );

    for ( i=0; i < pt_lkdfig->VALSIZ; i++)
      VHX_VECTOR_TRANS[i] = 0;
    }

  if  ( VHX_VECTOR_PROBA == NULL )
    {
    VHX_VECTOR_PROBA = mbkalloc ( pt_lkdfig->VALSIZ * sizeof (unsigned int) );

    for ( i=0; i < pt_lkdfig->VALSIZ; i++)
      VHX_VECTOR_PROBA[i] = 0;
    }

	/* ###------------------------------------------------------### */
	/*    save the value and count transitions of all signals:	*/
	/* ###------------------------------------------------------### */

  i = 0;

  while (pt_simsig != NULL)
    {
    if (*(pt_simsig->CURVAL) == '1')
      {
      VHX_VECTOR_PROBA[i] = 1;
      VHX_VECTOR_VALUE[i] = '1';
      }

    i++;
    pt_simsig = pt_simsig->NEXT;
    }

  while (pt_bussig != NULL)
    {
    if (*(pt_bussig->CURVAL) == '1')
      {
      VHX_VECTOR_PROBA[i] = 1;
      VHX_VECTOR_VALUE[i] = '1';
      }

    i++;
    pt_bussig = pt_bussig->NEXT;
    }

  while (pt_lkdins != NULL)
    {
    pt_wriaux = pt_lkdins->WRIAUX;
    while (pt_wriaux != NULL)
      {
      if (*(pt_wriaux->CURVAL) == '1')
        {
        VHX_VECTOR_PROBA[i] = 1;
        VHX_VECTOR_VALUE[i] = '1';
        }

      i++;
      pt_wriaux = pt_wriaux->NEXT;
      }

    pt_wribux = pt_lkdins->WRIBUX;
    while (pt_wribux != NULL)
      {
      if (*(pt_wribux->CURVAL) == '1')
        {
        VHX_VECTOR_PROBA[i] = 1;
        VHX_VECTOR_VALUE[i] = '1';
        }

      i++;
      pt_wribux = pt_wribux->NEXT;
      }

    pt_wrireg = pt_lkdins->WRIREG;
    while (pt_wrireg != NULL)
      {
      if (*(pt_wrireg->CURVAL) == '1')
        {
        VHX_VECTOR_PROBA[i] = 1;
        VHX_VECTOR_VALUE[i] = '1';
        }

      i++;
      pt_wrireg = pt_wrireg->NEXT;
      }
    pt_lkdins = pt_lkdins->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_vectorstat					*/
/* description	: Dedicated vector statistic function.			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhx_vectorstat (pt_lkdfig)

struct lkdfig *pt_lkdfig;

  {
  struct simsig *pt_simsig = pt_lkdfig->SIMSIG;
  struct bussig *pt_bussig = pt_lkdfig->BUSSIG;
  struct lkdins *pt_lkdins = pt_lkdfig->LKDINS;
  struct wrireg *pt_wrireg ;
  struct wriaux *pt_wriaux ;
  struct wribux *pt_wribux ;
  unsigned int   i;

	/* ###------------------------------------------------------### */
	/*    save the value and count transitions of all signals:	*/
	/* ###------------------------------------------------------### */

  i = 0;

  while (pt_simsig != NULL)
    {
    if (*(pt_simsig->CURVAL) == '1')
      {
      VHX_VECTOR_PROBA[i] = VHX_VECTOR_PROBA[i] +1;
/* printf ("PROBA de %d = %d\n", i, VHX_VECTOR_PROBA[i] ); */
      }

    if (*(pt_simsig->CURVAL) != VHX_VECTOR_VALUE[i])
      {
      VHX_VECTOR_TRANS[i] = VHX_VECTOR_TRANS[i] +1;
      VHX_VECTOR_VALUE[i] = *(pt_simsig->CURVAL);
      }
    i++;
    pt_simsig = pt_simsig->NEXT;
    }

  while (pt_bussig != NULL)
    {
    if (*(pt_bussig->CURVAL) == '1')
      {
      VHX_VECTOR_PROBA[i] = VHX_VECTOR_PROBA[i] +1;
/* printf ("PROBA de %d = %d\n", i, VHX_VECTOR_PROBA[i] ); */
      }

    if (*(pt_bussig->CURVAL) != VHX_VECTOR_VALUE[i])
      {
      VHX_VECTOR_TRANS[i] = VHX_VECTOR_TRANS[i] +1;
      VHX_VECTOR_VALUE[i] = *(pt_bussig->CURVAL);
      }
    i++;
    pt_bussig = pt_bussig->NEXT;
    }

  while (pt_lkdins != NULL)
    {
    pt_wriaux = pt_lkdins->WRIAUX;
    while (pt_wriaux != NULL)
      {
      if (*(pt_wriaux->CURVAL) == '1')
        {
        VHX_VECTOR_PROBA[i] = VHX_VECTOR_PROBA[i] +1;
/* printf ("PROBA de %d = %d\n", i, VHX_VECTOR_PROBA[i] ); */
        }

      if (*(pt_wriaux->CURVAL) != VHX_VECTOR_VALUE[i])
        {
        VHX_VECTOR_TRANS[i] = VHX_VECTOR_TRANS[i] +1;
        VHX_VECTOR_VALUE[i] = *(pt_wriaux->CURVAL);
        }
      i++;
      pt_wriaux = pt_wriaux->NEXT;
      }

    pt_wribux = pt_lkdins->WRIBUX;
    while (pt_wribux != NULL)
      {
      if (*(pt_wribux->CURVAL) == '1')
        {
        VHX_VECTOR_PROBA[i] = VHX_VECTOR_PROBA[i] +1;
/* printf ("PROBA de %d = %d\n", i, VHX_VECTOR_PROBA[i] ); */
        }

      if (*(pt_wribux->CURVAL) != VHX_VECTOR_VALUE[i])
        {
        VHX_VECTOR_TRANS[i] = VHX_VECTOR_TRANS[i] +1;
        VHX_VECTOR_VALUE[i] = *(pt_wribux->CURVAL);
        }
      i++;
      pt_wribux = pt_wribux->NEXT;
      }

    pt_wrireg = pt_lkdins->WRIREG;
    while (pt_wrireg != NULL)
      {
      if (*(pt_wrireg->CURVAL) == '1')
        {
        VHX_VECTOR_PROBA[i] = VHX_VECTOR_PROBA[i] +1;
/* printf ("PROBA de %d = %d\n", i, VHX_VECTOR_PROBA[i] ); */
        }

      if (*(pt_wrireg->CURVAL) != VHX_VECTOR_VALUE[i])
        {
        VHX_VECTOR_TRANS[i] = VHX_VECTOR_TRANS[i] +1;
        VHX_VECTOR_VALUE[i] = *(pt_wrireg->CURVAL);
        }
      i++;
      pt_wrireg = pt_wrireg->NEXT;
      }

    pt_lkdins = pt_lkdins->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_inireg						*/
/* description	: forces new values into registers			*/
/* called func.	: beh_chktab, vhu_warning, addchain			*/
/* ###--------------------------------------------------------------### */

void vhx_inireg (dict, pt_paini)

struct beden     **dict    ;
struct paini      *pt_paini;

  {
  char          *sig_name ;
  char          *ins_name ;
  void          *pt_sig   ;
  struct ptype  *pt_depend = NULL;
  unsigned char *pt_curval = NULL;

	/* ###------------------------------------------------------### */
	/*    for each paini structure, first find the pointer of the	*/
	/* register then, force its value				*/
	/* ###------------------------------------------------------### */

  while (pt_paini != NULL)
    {
    vhu_splitname (pt_paini->NAME, &ins_name, &sig_name);

    pt_sig = (void *) beh_chktab (dict, sig_name, ins_name, VHL_PNTDFN);

    switch (beh_chktab (dict, sig_name, ins_name, VHL_SIGDFN))
      {
      case 'R':
        pt_curval = ((struct wrireg *) pt_sig)->CURVAL;
        pt_depend = ((struct wrireg *) pt_sig)->DEPEND;
        break;
      case 'S':
        pt_curval = ((struct simsig *) pt_sig)->CURVAL;
        pt_depend = ((struct simsig *) pt_sig)->DEPEND;
        break;
      case 'U':
        pt_curval = ((struct wribux *) pt_sig)->CURVAL;
        pt_depend = ((struct wribux *) pt_sig)->DEPEND;
        break;
      case 'X':
        pt_curval = ((struct wriaux *) pt_sig)->CURVAL;
        pt_depend = ((struct wriaux *) pt_sig)->DEPEND;
        break;
      case 'B':
        pt_curval = ((struct bussig *) pt_sig)->CURVAL;
        pt_depend = ((struct bussig *) pt_sig)->DEPEND;
        break;
      }

    if (pt_sig == NULL)
      vhu_warning (1, pt_paini->NAME, NULL);
    else
      {
      *(pt_curval) = pt_paini->VALUE;
      vhx_addexec (pt_depend);
      }
    pt_paini = pt_paini->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_redpat						*/
/* description	: reads a simulation pattern from the pattern		*/
/*		  description structure and initialises values in the	*/
/*		  circuit						*/
/* called func.	: addchain						*/
/* ###--------------------------------------------------------------### */

void vhx_redpat (pt_paseq, pt_papat)

struct paseq *pt_paseq;
struct papat *pt_papat;

  {
  struct paiol  *pt_paiol ;
/*struct bussig *pt_bussig;*/
/*struct simsig *pt_simsig;*/
  struct paevt  *pt_paevt ;

	/* ###------------------------------------------------------### */
	/*    set the projected value of inputs (ports of mode I or T)	*/
	/* ###------------------------------------------------------### */

  pt_paevt = pt_papat->PAEVT;
  while (pt_paevt != NULL)
    {
    pt_paiol = pt_paseq->PAIOL + pt_paevt->INDEX;

    if (pt_paiol->MODE == 'I')
      {
      VHX_SIGUPD = addchain (VHX_SIGUPD, pt_paiol->SIG);
      ((struct simsig *) pt_paiol->SIG)->PRJVAL = pt_paevt->USRVAL;
      }
    else
      {
      if (pt_paiol->MODE == 'T')
        {
        VHX_BUSUPD = addchain (VHX_BUSUPD, pt_paiol->SIG);
        if ((pt_paevt->USRVAL == '1') || (pt_paevt->USRVAL == '0'))
          {
          ((struct bussig *) pt_paiol->SIG)->PRJBVL->DRVVAL = pt_paevt->USRVAL;
          ((struct bussig *) pt_paiol->SIG)->PRJBVL->CNDVAL = '1';
          }
        else
          ((struct bussig *) pt_paiol->SIG)->PRJBVL->CNDVAL = '0';
        }
      }
    pt_paevt = pt_paevt->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_wrtpat						*/
/* description	: saves in the pattern description structure the result	*/
/*		  of the simulation cycle				*/
/* called func.	: vhu_toolbug, vhu_error, pat_addpaevt			*/
/* ###--------------------------------------------------------------### */

int vhx_wrtpat (pt_paseq, pt_papat)

struct paseq *pt_paseq;
struct papat *pt_papat;

  {
  int            i        ;
  int            err_flg  = 0;
  int            max_iol  ;
  int            max_err  = 0;
  struct paiol  *pt_paiol ;
  struct paevt  *pt_paevt ;
  char           value    = 'u';
  char           usrval   ;

  max_iol  = pt_paseq->IOLNBR;
  pt_paiol = pt_paseq->PAIOL;

  for (i=0 ; i<max_iol ; i++)
    {
	/* ###------------------------------------------------------### */
	/*    for each input-output ...					*/
	/* ###------------------------------------------------------### */

    if (pt_paiol->MODE != 'I')
      {
	/* ###------------------------------------------------------### */
	/*    read the input-output's value from the description -	*/
	/* skip simple inputs						*/
	/* ###------------------------------------------------------### */

      switch (pt_paiol->MODE)
        {
        case 'X' :
        case 'T' :
        case 'Z' :
          value = *( ((struct bussig *) pt_paiol->SIG)->CURVAL);
          break;

        case 'B' :
        case 'O' :
        case 'W' :
          value = *( ((struct simsig *) pt_paiol->SIG)->CURVAL);
          break;

        case 'S' :
          value = *( ((struct wriaux *) pt_paiol->SIG)->CURVAL);
          break;

        case 'U' :
          value = *( ((struct wribux *) pt_paiol->SIG)->CURVAL);
          break;

        case 'R' :
          value = *( ((struct wrireg *) pt_paiol->SIG)->CURVAL);
          break;

        default :
          vhu_toolbug (7, "vhx_wrtpat", pt_paiol->NAME, pt_paiol->MODE);
          break;
        }

	/* ###------------------------------------------------------### */
	/*    scan user predicted event for the current input-output	*/
	/* ###------------------------------------------------------### */

      pt_paevt = pt_papat->PAEVT;
      while (pt_paevt != NULL)
        {
        if (pt_paevt->INDEX == i)
          break;
        pt_paevt = pt_paevt->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*   if no user predicted event has been found and an event has	*/
	/* been occurred, add a new paevt structure to the pattern. Use	*/
	/* a wrong value (the previous value) as user defined value.	*/
	/* Remember that a user predicted '?*' is always an event.	*/
	/* ###------------------------------------------------------### */

      if ((pt_paevt == NULL) && (pt_paiol->VALUE != value))
        {
        if (value == '1')
          {
          pt_paevt        = pat_addpaevt (pt_papat->PAEVT, i, '-');
          pt_papat->PAEVT = pt_paevt;
          }
        else if (value == '0')
          {
          pt_paevt        = pat_addpaevt (pt_papat->PAEVT, i, '+');
          pt_papat->PAEVT = pt_paevt;
          }
        else if (value == 'u')
          {
          if (pt_paiol->VALUE == '1')
            {
            pt_paevt        = pat_addpaevt (pt_papat->PAEVT, i, '+');
            pt_papat->PAEVT = pt_paevt;
            }
          else
            {
            pt_paevt        = pat_addpaevt (pt_papat->PAEVT, i, '-');
            pt_papat->PAEVT = pt_paevt;
            }
          }
        }

	/* ###------------------------------------------------------### */
	/*    if there is an event related to the input-output, and if	*/
	/* the value predicted by the user and the simulated value do	*/
	/* not match print out an error message. In such a case add an	*/
	/* event on the same input-output with a wrong predicted value	*/
	/* to the next pattern (if it exists).				*/
	/* ###------------------------------------------------------### */

      if (pt_paevt != NULL)
        {
        switch (pt_paevt->USRVAL)
          {
          case '-':
          case '0':
            usrval ='-';
            if (value == '1')
              {
              vhu_error (113, pt_paiol->NAME, NULL);
              max_err ++ ;
              err_flg = 1;
              pt_paevt->SIMVAL = '+';
              }
            else if (value == 'u')
              {
              vhu_error (113, pt_paiol->NAME, NULL);
              max_err ++ ;
              err_flg = 1;
              pt_paevt->SIMVAL = 'u';
              }
            break;
          case '*':
            if (value == '1')
              pt_paevt->SIMVAL = '+';
            else if (value == '0')
              pt_paevt->SIMVAL = '-';
            else
              pt_paevt->SIMVAL = 'u';
            break;
          case '+':
          case '1':
            usrval ='+';
            if (value == '0')
              {
              vhu_error (113, pt_paiol->NAME, NULL);
              max_err ++ ;
              err_flg = 1;
              pt_paevt->SIMVAL = '-';
              }
            else if (value == 'u')
              {
              vhu_error (113, pt_paiol->NAME, NULL);
              max_err ++ ;
              err_flg = 1;
              pt_paevt->SIMVAL = 'u';
              }
            break;
          }

	/* ###------------------------------------------------------### */
	/* If an error occurs, and if no user event is scheduled in the	*/
	/* next pattern, add an event on the same input-output with a	*/
	/* wrong predicted value to the next pattern (if it exists).	*/
	/* ###------------------------------------------------------### */

        if ((err_flg == 1) && (pt_papat->NEXT != NULL))
          {
          pt_paevt = pt_papat->NEXT->PAEVT;
          while (pt_paevt != NULL)
            {
            if (pt_paevt->INDEX == i)
              break;
            pt_paevt = pt_paevt->NEXT;
            }

          if (pt_paevt == NULL)
            {
            if (value == '0')
              {
              pt_paevt              = pat_addpaevt (pt_papat->NEXT->PAEVT,i,'+');
              pt_papat->NEXT->PAEVT = pt_paevt;
              }
            else if (value == '1')
              {
              pt_paevt              = pat_addpaevt (pt_papat->NEXT->PAEVT,i,'-');
              pt_papat->NEXT->PAEVT = pt_paevt;
              }
            else
              {
              pt_paevt              = pat_addpaevt (pt_papat->NEXT->PAEVT,i,usrval);
              pt_papat->NEXT->PAEVT = pt_paevt;
              }
            }
          }
        }
      }
	/* ###------------------------------------------------------### */
	/*    save the value in the paiol structure to make event	*/
	/* detections possible for the next pattern			*/
	/* ###------------------------------------------------------### */

    err_flg = 0;
    pt_paiol->VALUE = value;
    pt_paiol ++;
    }

	/* ###------------------------------------------------------### */
	/*    set the simulation flag of the pattern			*/
	/* ###------------------------------------------------------### */

  pt_papat->SIMFLAG = 'S';
  return (max_err);
  }


/* ###--------------------------------------------------------------### */
/* function	: vhx_commentspytable					*/
/* description	: Insert Pattern Comments corresponding to the spied	*/
/*                signals table before the first pattern.		*/
/* called func.	: vhu_toolbug, vhu_error, pat_addpaevt			*/
/* ###--------------------------------------------------------------### */

void vhx_commentspytable (pt_paseq, head_lkdspy)

struct paseq  *pt_paseq;
struct lkdspy *head_lkdspy;

  {
  char           str[256]    ;
  struct lkdspy *pt_lkdspy= NULL ;

	/* ###------------------------------------------------------### */
	/*    for each lkdspy .........					*/
	/* ###------------------------------------------------------### */

  for (pt_lkdspy=head_lkdspy ; pt_lkdspy != NULL; pt_lkdspy=pt_lkdspy->NEXT)
    {
    if (pt_lkdspy->PAIOL != NULL)
      {
	/* ###------------------------------------------------------### */
	/*    Add a pacom (pattern comment) for each spy with spy name	*/
	/*    and spy index						*/
	/* ###------------------------------------------------------### */

      sprintf(str, "spy%u : %s", pt_lkdspy->IOLNBR, pt_lkdspy->PAIOL->NAME);

      if (pt_lkdspy->PAIOL->MODE == 'I')
        {
        char tmp[256];
        sprintf(tmp, "%s : Warning : this spy is useless (input signal)", str);
        strncpy(str, tmp, 256);
        }
      pt_paseq->CURCOM = pat_addpacom(pt_paseq->CURCOM, namealloc(str), 0);
      }
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_insertspypat					*/
/* description	: Inster Pattern When a Spy Event occurs.		*/
/* called func.	: vhu_toolbug, vhu_error, pat_addpaevt			*/
/* ###--------------------------------------------------------------### */

struct papat *vhx_insertspypat (pt_paseq, lst_papat, head_lkdspy, cur_date, labelsiz)

struct paseq  *pt_paseq;
struct papat  *lst_papat;
struct lkdspy *head_lkdspy;
unsigned long  cur_date;
unsigned int   labelsiz;

  {
  int            i        ;
  int            max_iol  ;
  struct paiol  *pt_paiol = NULL ;
  struct bussig *pt_bussig= NULL ;
  struct simsig *pt_simsig= NULL ;
  struct wrireg *pt_wrireg= NULL ;
  struct paevt  *pt_paevt = NULL ;
  struct lkdspy *pt_lkdspy= NULL ;
  char           value    = 'u';
  char          *label    ;
  int            spy_event_flag = 0  ;

  label = mbkalloc (labelsiz + 1);
  label[0] = '\0';
  max_iol = pt_paseq->IOLNBR;

	/* ###------------------------------------------------------### */
	/*    for each lkdspy .........					*/
	/* ###------------------------------------------------------### */

  for (pt_lkdspy=head_lkdspy ; pt_lkdspy != NULL; pt_lkdspy=pt_lkdspy->NEXT)
    {
    pt_paiol = pt_lkdspy->PAIOL;


	/* ###------------------------------------------------------### */
	/* Skip simple inputs						*/
	/* ###------------------------------------------------------### */

    if (pt_paiol->MODE != 'I')
      {

	/* ###------------------------------------------------------### */
	/*    read the input-output's value from the description -	*/
	/* skip simple inputs						*/
	/* ###------------------------------------------------------### */

      switch (pt_paiol->MODE)
        {
        case 'X' :
        case 'T' :
        case 'Z' :
          pt_bussig =  (struct bussig *) pt_paiol->SIG;
          value     = *(pt_bussig->CURVAL);
          break;

        case 'B' :
        case 'O' :
        case 'W' :
          pt_simsig =  (struct simsig *) pt_paiol->SIG;
          value     = *(pt_simsig->CURVAL);
          break;

        case 'S' :
          value     = *( ((struct wriaux *) pt_paiol->SIG)->CURVAL);
          break;

        case 'U' :
          value     = *( ((struct wribux *) pt_paiol->SIG)->CURVAL);
          break;

        case 'R' :
          pt_wrireg =  (struct wrireg *) pt_paiol->SIG;
          value     = *(pt_wrireg->CURVAL);
          break;

        default :
          vhu_toolbug (7, "vhx_insertspypat", pt_paiol->NAME, pt_paiol->MODE);
          break;
        }

	/* ###------------------------------------------------------### */
	/*    if an event has been occurred, switch ON a spy_flag.	*/
	/*    and append the paiol IOLNBR to the label.			*/
	/*    if an event has been occurred, add a new paevt structure.	*/
	/* ###------------------------------------------------------### */

      if (pt_paiol->VALUE != value)
        {
        spy_event_flag = 1;
        if (strlen (label) != 0)
          {
          if ((strlen (label) + 10) <= labelsiz)
            {
            char tmp[2048];
            sprintf(tmp, "%s_spy%u", label, pt_lkdspy->IOLNBR);
            strcpy(label, tmp);
            }
          }
        else
          {
          sprintf(label, "spy%u", pt_lkdspy->IOLNBR);
          }
        pt_paevt        = pat_addpaevt (pt_paevt, pt_lkdspy->IOLNBR, '*');
        if (value == '1')
          pt_paevt->SIMVAL = '+';
        else
          {
          if (value == '0')
            pt_paevt->SIMVAL = '-';
          else
            pt_paevt->SIMVAL = 'u';
          }

        pt_paiol->VALUE = value;
        }
      }
    }


	/* ###------------------------------------------------------### */
	/*    if The spy_event_flag is ON : insert a new pattern.	*/
	/* ###------------------------------------------------------### */

  if ( spy_event_flag != 0)
    {
	/* ###------------------------------------------------------### */
	/* Inspect All PAIOL to add paevt				*/
	/* ###------------------------------------------------------### */

    pt_paiol = pt_paseq->PAIOL;

    for (i=0 ; i<max_iol ; i++)
      {

	/* ###------------------------------------------------------### */
	/* Skip simple inputs						*/
	/* ###------------------------------------------------------### */

      if (pt_paiol->MODE != 'I')
        {

	/* ###------------------------------------------------------### */
	/*    read the input-output's value from the description -	*/
	/* skip simple inputs						*/
	/* ###------------------------------------------------------### */

        switch (pt_paiol->MODE)
          {
          case 'X' :
          case 'T' :
          case 'Z' :
            pt_bussig =  (struct bussig *) pt_paiol->SIG;
            value     = *(pt_bussig->CURVAL);
            break;

          case 'B' :
          case 'O' :
          case 'W' :
            pt_simsig =  (struct simsig *) pt_paiol->SIG;
            value     = *(pt_simsig->CURVAL);
            break;

          case 'S' :
            value     = *( ((struct wriaux *) pt_paiol->SIG)->CURVAL);
            break;

          case 'U' :
            value     = *( ((struct wribux *) pt_paiol->SIG)->CURVAL);
            break;

          case 'R' :
            pt_wrireg =  (struct wrireg *) pt_paiol->SIG;
            value     = *(pt_wrireg->CURVAL);
            break;

          default :
            vhu_toolbug (7, "vhx_insertspypat", pt_paiol->NAME, pt_paiol->MODE);
            break;
          }

	/* ###------------------------------------------------------### */
	/*    if an event has been occurred, add a new paevt structure.	*/
	/* ###------------------------------------------------------### */

        if (pt_paiol->VALUE != value)
          {
          pt_paevt        = pat_addpaevt (pt_paevt, i, '*');
          if (value == '1')
            pt_paevt->SIMVAL = '+';
          else
            {
            if (value == '0')
              pt_paevt->SIMVAL = '-';
            else
              pt_paevt->SIMVAL = 'u';
            }

          pt_paiol->VALUE = value;
          }
        }
      pt_paiol++;
      }

    lst_papat->NEXT = pat_addpapat(lst_papat->NEXT, label, lst_papat->LINE);
    lst_papat->NEXT->PAEVT = pt_paevt;
    lst_papat->NEXT->TIME = cur_date;
    lst_papat->NEXT->SIMFLAG = 'S';
    lst_papat = lst_papat->NEXT;
    pt_paseq->PATNBR ++;
    }
  return (lst_papat);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_chkerr						*/
/* description	: verifies that no errors remains at the end of the last*/
/*		  delta cycle						*/
/* called func.	: vhu_error						*/
/* ###--------------------------------------------------------------### */

int vhx_chkerr (cur_date)

unsigned int     cur_date       ;
  {
  short          errflg       = 0;
  struct lkdins *pt_lkdins    ;
  struct bussig *pt_bussig    ;
  struct wribux *pt_wribux    ;
  struct wrireg *pt_wrireg    ;
  struct wrimsg *pt_wrimsg    ;
  struct bemsg  *pt_bemsg     ;
  char          *level        ;
  char          *label        ;

  if (VHX_MSGERR != 0)
    {
    pt_lkdins = VHL_HEDLKF->LKDINS;
    while (pt_lkdins != NULL)
      {
      pt_wrimsg = pt_lkdins->WRIMSG;
      while (pt_wrimsg != NULL)
        {
        if (pt_wrimsg->ERRFLG == 1)
          {
          VHX_MSGERR --;
          pt_wrimsg->ERRFLG = 0;

          pt_bemsg = pt_wrimsg->BEMSG;
          if (pt_bemsg->LEVEL == 'W')
            level = "WARNING";
          else
            {
            level  = "ERROR";
            errflg = 1;
            }

          if (pt_bemsg->LABEL != NULL)
            label = pt_bemsg->LABEL;
          else
            label = "";

          fprintf (stderr, "%s: %u ps: assert violation in %s: %s: %s\n",
                   level , cur_date, pt_lkdins->INSNAME, label, pt_bemsg->MESSAGE);
          }
        pt_wrimsg = pt_wrimsg->NEXT;
        }
      pt_lkdins = pt_lkdins->NEXT;
      }
    }

  if (VHX_BUSERR != 0)
    {
    pt_bussig = VHL_HEDLKF->BUSSIG;
    while (pt_bussig != NULL)
      {
      if (pt_bussig->ERRFLG == 1)
        vhu_warning (112, pt_bussig->NAMECHAIN->DATA, NULL);
      pt_bussig = pt_bussig->NEXT;
      }
    }

  if (VHX_BUXERR != 0)
    {
    pt_lkdins = VHL_HEDLKF->LKDINS;
    while (pt_lkdins != NULL)
      {
      pt_wribux = pt_lkdins->WRIBUX;
      while (pt_wribux != NULL)
        {
        if (pt_wribux->ERRFLG == 1)
          vhu_warning (121, pt_lkdins->INSNAME, pt_wribux->BEBUX->NAME);
        pt_wribux = pt_wribux->NEXT;
        }
      pt_lkdins = pt_lkdins->NEXT;
      }
    }

  if (VHX_REGERR != 0)
    {
    pt_lkdins = VHL_HEDLKF->LKDINS;
    while (pt_lkdins != NULL)
      {
      pt_wrireg = pt_lkdins->WRIREG;
      while (pt_wrireg != NULL)
        {
        if (pt_wrireg->ERRFLG == 1)
          vhu_warning (121, pt_lkdins->INSNAME, pt_wrireg->BEREG->NAME);
        pt_wrireg = pt_wrireg->NEXT;
        }
      pt_lkdins = pt_lkdins->NEXT;
      }
    }

  return (errflg);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_initfig						*/
/* description	: initialize a description (reading values from a save	*/
/*		  file)							*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char vhx_initfig (pt_lkdfig, fp)

struct lkdfig *pt_lkdfig;
FILE          *fp       ;

  {
  struct simsig *pt_simsig = pt_lkdfig->SIMSIG;
  struct bussig *pt_bussig = pt_lkdfig->BUSSIG;
  struct lkdins *pt_lkdins = pt_lkdfig->LKDINS;
  struct wrireg *pt_wrireg ;
  struct wriaux *pt_wriaux ;
  struct wribux *pt_wribux ;
  struct prjbvl *pt_prjbvl ;
  struct prjrvl *pt_prjrvl ;
  unsigned int   size      ;
  char          *pt_mem    ;
  char           errflg    = 0;

	/* ###------------------------------------------------------### */
	/*    get the size of the circuit (number of bytes needed to	*/
	/* store the value of all signals)				*/
	/* ###------------------------------------------------------### */

  vhx_getsize (pt_lkdfig);

	/* ###------------------------------------------------------### */
	/*    read the size stored in the initialization file. An error	*/
	/* is detected if the size read from the file differs from the	*/
	/* the size of the circuit.					*/
	/* ###------------------------------------------------------### */

  fread (&size, sizeof (unsigned int), 1, fp);
  if (size != pt_lkdfig->VALSIZ)
    {
    errflg = vhu_error (115, NULL, NULL);
    }
  else
    {
	/* ###------------------------------------------------------### */
	/*    if sizes are the same, initialize the current and		*/
	/* projected values of simple and bussed signals		*/
	/* ###------------------------------------------------------### */

    while (pt_simsig != NULL)
      {
      pt_simsig->PRJVAL    = fgetc (fp);
      *(pt_simsig->CURVAL) = fgetc (fp);
      pt_simsig            = pt_simsig->NEXT;
      }

    while (pt_bussig != NULL)
      {
      pt_prjbvl = pt_bussig->PRJBVL;
      while (pt_prjbvl != NULL)
        {
        pt_prjbvl->DRVVAL = fgetc (fp);
        pt_prjbvl->CNDVAL = fgetc (fp);
        pt_prjbvl         = pt_prjbvl->GLNEXT;
        }
      *(pt_bussig->CURVAL) = fgetc (fp);
      pt_bussig = pt_bussig->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    for each instance:					*/
	/*      - if the instance uses a C model get the pointer of the	*/
	/*        area used to store the static variables of the	*/
	/*        instance. Initialize the area with values read from	*/
	/*        the file.						*/
	/*      - for other instances initialize the current and	*/
	/*        projected values of intsrenal signals (simple and	*/
	/*        bussed signals and internal registers) with values	*/
	/*        read from the initialization file.			*/
	/* ###------------------------------------------------------### */

    while (pt_lkdins != NULL)
      {
      if (pt_lkdins->FLAG == VHL_CMODEL)
        {
        size = vhx_getvalu (pt_lkdins, &pt_mem, 0);
        fread (pt_mem, sizeof (char), size, fp);
        }
      else
        {
        pt_wrireg = pt_lkdins->WRIREG;
        pt_wriaux = pt_lkdins->WRIAUX;
        pt_wribux = pt_lkdins->WRIBUX;

        while (pt_wrireg != NULL)
          {
          pt_prjrvl = pt_wrireg->PRJRVL;
          while (pt_prjrvl != NULL)
            {
            pt_prjrvl->DRVVAL = fgetc (fp);
            pt_prjrvl->CNDVAL = fgetc (fp);
            pt_prjrvl         = pt_prjrvl->NEXT;
            }
          *(pt_wrireg->CURVAL) = fgetc (fp);
          pt_wrireg = pt_wrireg->NEXT;
          }

        while (pt_wribux != NULL)
          {
          pt_prjbvl = pt_wribux->PRJBVL;
          while (pt_prjbvl != NULL)
            {
            pt_prjbvl->DRVVAL = fgetc (fp);
            pt_prjbvl->CNDVAL = fgetc (fp);
            pt_prjbvl         = pt_prjbvl->NEXT;
            }
          *(pt_wribux->CURVAL) = fgetc (fp);
          pt_wribux = pt_wribux->NEXT;
          }

        while (pt_wriaux != NULL)
          {
          pt_wriaux->PRJVAL    = fgetc (fp);
          *(pt_wriaux->CURVAL) = fgetc (fp);
          pt_wriaux            = pt_wriaux->NEXT;
          }
        }

      pt_lkdins = pt_lkdins->NEXT;
      }
    }
  return (errflg);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_initval						*/
/* description	: initialize a description (set all values to the same	*/
/*		  value)						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhx_initval (pt_lkdfig, value)

struct lkdfig *pt_lkdfig;
unsigned char  value    ;

  {
  struct simsig *pt_simsig = pt_lkdfig->SIMSIG;
  struct bussig *pt_bussig = pt_lkdfig->BUSSIG;
  struct lkdins *pt_lkdins = pt_lkdfig->LKDINS;
  struct wrireg *pt_wrireg ;
  struct wriaux *pt_wriaux ;
  struct wribux *pt_wribux ;

  while (pt_simsig != NULL)
    {
    *(pt_simsig->CURVAL) = value;
    pt_simsig = pt_simsig->NEXT;
    }

  while (pt_bussig != NULL)
    {
    *(pt_bussig->CURVAL) = value;
    pt_bussig = pt_bussig->NEXT;
    }

  while (pt_lkdins != NULL)
    {
    pt_wrireg = pt_lkdins->WRIREG;
    pt_wriaux = pt_lkdins->WRIAUX;
    pt_wribux = pt_lkdins->WRIBUX;

    while (pt_wrireg != NULL)
      {
      *(pt_wrireg->CURVAL) = value;
      pt_wrireg = pt_wrireg->NEXT;
      }

    while (pt_wribux != NULL)
      {
      *(pt_wribux->CURVAL) = value;
      pt_wribux = pt_wribux->NEXT;
      }

    while (pt_wriaux != NULL)
      {
      *(pt_wriaux->CURVAL) = value;
      pt_wriaux = pt_wriaux->NEXT;
      }
    pt_lkdins = pt_lkdins->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_core						*/
/* description	: create a core file of the lkdfig			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhx_core (fp, pt_lkdfig)

FILE          *fp       ;
struct lkdfig *pt_lkdfig;

  {
  time_t         clock    ;
  struct lkdins *pt_lkdins;
  struct simsig *pt_simsig;
  struct bussig *pt_bussig;
  struct wrireg *pt_wrireg;
  struct wriaux *pt_wriaux;
  struct wribux *pt_wribux;

	/* ###------------------------------------------------------### */
	/*   print a header for the core file				*/
	/* ###------------------------------------------------------### */

  time (&clock);

  fprintf (fp, "Core file generated from %s\n", pt_lkdfig->NAME);
  fprintf (fp, "\t\tdate : %s \n\n", ctime (&clock));

	/* ###------------------------------------------------------### */
	/*   print simple signals' current value			*/
	/* ###------------------------------------------------------### */

  fprintf (fp, "\t------- simple signals --------\n");
  pt_simsig = pt_lkdfig->SIMSIG;
  while (pt_simsig != NULL)
    {
    if (pt_simsig->NAMECHAIN != NULL)
      {
      fprintf (fp, "%s = %c\n", (char *) (pt_simsig->NAMECHAIN->DATA),
                                        *(pt_simsig->CURVAL));
      }
    pt_simsig = pt_simsig->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*   print bussed signals' current value			*/
	/* ###------------------------------------------------------### */

  fprintf (fp, "\t------- buses --------\n");
  pt_bussig = pt_lkdfig->BUSSIG;
  while (pt_bussig != NULL)
    {
    if (pt_bussig->NAMECHAIN != NULL)
      {
      fprintf (fp, "%s = %c\n", (char *) (pt_bussig->NAMECHAIN->DATA),
                                        *(pt_bussig->CURVAL));
      }
    pt_bussig = pt_bussig->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*   for each instance :					*/
	/*     - print instance's name					*/
	/*     - internal registers' current value			*/
	/*     - internal buses' current value				*/
	/*     - internal simple signals' current value			*/
	/* ###------------------------------------------------------### */

  pt_lkdins = pt_lkdfig->LKDINS;
  while (pt_lkdins != NULL)
    {
    fprintf (fp, "\t======= instance %s =======\n\n", pt_lkdins->INSNAME);

    fprintf (fp, "\t------- internal registers --------\n");
    pt_wrireg = pt_lkdins->WRIREG;
    while (pt_wrireg != NULL)
      {
      fprintf (fp, "%s.%s = %c\n",  pt_wrireg->LKDINS->INSNAME,
                                    pt_wrireg->BEREG->NAME,
                                  *(pt_wrireg->CURVAL));
      pt_wrireg = pt_wrireg->NEXT;
      }

    fprintf (fp, "\t------- internal simple signals -------\n");
    pt_wriaux = pt_lkdins->WRIAUX;
    while (pt_wriaux != NULL)
      {
      fprintf (fp, "%s.%s = %c\n",  pt_wriaux->LKDINS->INSNAME,
                                    pt_wriaux->BEAUX->NAME,
                                  *(pt_wriaux->CURVAL));
      pt_wriaux = pt_wriaux->NEXT;
      }

    fprintf (fp, "\t------- internal bus --------\n");
    pt_wribux = pt_lkdins->WRIBUX;
    while (pt_wribux != NULL)
      {
      fprintf (fp, "%s.%s = %c\n",  pt_wribux->LKDINS->INSNAME,
                                    pt_wribux->BEBUX->NAME,
                                  *(pt_wribux->CURVAL));
      pt_wribux = pt_wribux->NEXT;
      }
    pt_lkdins = pt_lkdins->NEXT;
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_readinput						*/
/* description	: read the current value of an input and make a vector	*/
/*		  if needed. This function is useful for C models	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

unsigned int vhx_readinput (pt_lkdins, index, size)

struct lkdins *pt_lkdins;
unsigned int   index    ;
unsigned int   size     ;

  {
  struct redlst *pt_redlst = pt_lkdins->REDLST + index;
  unsigned int   cur_value = 0                        ;
  int            i         ;

  for (i=(size-1) ; i>=0 ; i--)
    cur_value = (cur_value << 1) + (((* ((pt_redlst + i)->VALU)) - '0') & 0x01);

  return (cur_value);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_writeout						*/
/* description	: write the projected value of a simple output and	*/
/*		  add the corresponding signals to the list of signals	*/
/*		  that must be updated. Note that the value is an	*/
/*		  unsigned integer. The size indicates the number of	*/
/*		  bits that must be written. The value is right-aligned	*/
/*		  (on least significant bits of the value). This	*/
/*		  function is useful for C models.			*/
/* called func.	: addchain						*/
/* ###--------------------------------------------------------------### */

void vhx_writeout (value, pt_lkdins, index, size)

unsigned int   value    ;
struct lkdins *pt_lkdins;
unsigned int   index    ;
unsigned int   size     ;

  {
  struct wrssig *pt_wrssig = pt_lkdins->WRSSIG + index;
  unsigned int   i         ;

  for (i=0 ; i<size ; i++)
    {
    sch_AddTransaction ( VHX_SHDUL,  pt_wrssig->SIMSIG,  pt_wrssig->SIMSIG,
                         0, '1', (value & 0x00000001) + '0', (unsigned int) 'S',
                         VHX_SCH_TRA_MODE);

    pt_wrssig++;
    value = value >> 1;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_writebus						*/
/* description	: write the projected value of a bussed output and	*/
/*		  add the corresponding signals to the list of signals	*/
/*		  that must be updated. Note that the value as well as	*/
/*		  the condition are unsigned integers. The size		*/
/*		  indicates the number of bits that must be written.	*/
/*		  The value and the condition are right-aligned (on	*/
/*		  least significant bits). This function is useful for	*/
/*		  C models.						*/
/* called func.	: addchain						*/
/* ###--------------------------------------------------------------### */

void vhx_writebus (value, cond, pt_lkdins, index, size)

unsigned int   value    ;
unsigned int   cond     ;
struct lkdins *pt_lkdins;
unsigned int   index    ;
unsigned int   size     ;

  {
  struct wrbsig *pt_wrbsig = pt_lkdins->WRBSIG + index;
  unsigned int   i         ;

  for (i=0 ; i<size ; i++)
    {
    pt_wrbsig->PRJBVL->DRVVAL = (value & 0x00000001) + '0';
    pt_wrbsig->PRJBVL->CNDVAL = (cond  & 0x00000001) + '0';
    pt_wrbsig->EVAL           = 0;
    VHX_BUSUPD                = addchain (VHX_BUSUPD, pt_wrbsig->BUSSIG);

    pt_wrbsig++;
    value = value >> 1;
    cond  = cond  >> 1;
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_GetInstanceName					*/
/* description	: return the instance's name				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char *vhx_GetInstanceName (pt_lkdins)

struct lkdins *pt_lkdins;

  {
  return (pt_lkdins->INSNAME);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_bddeval						*/
/* description	: computes a BDD's value depending on its node's value	*/
/* called func.	: vhu_toolbug						*/
/* ###--------------------------------------------------------------### */

char vhx_bddeval (pt_lkdins, pt_bdd)

struct lkdins *pt_lkdins;
struct node   *pt_bdd   ;

  {
  struct redlst *pt_redlst = pt_lkdins->REDLST - 2;
  char           value     ;
  char           result    ;

  while ((pt_bdd != zero) && (pt_bdd != one))
    {
    value = *((pt_redlst + pt_bdd->index)->VALU) ;

    if (value == '0')
      pt_bdd = pt_bdd->low ;
    else
      {
      if (value == '1')
        pt_bdd = pt_bdd->high ;
      else
        {
        if (value == 'u')
          {
          if ((result = vhx_bddeval (pt_lkdins, pt_bdd->high)) != 'u')
            {
            if (vhx_bddeval (pt_lkdins, pt_bdd->low) != result)
              result = 'u';
            }
          return (result);
          }
        else
          vhu_toolbug (8, "vhx_bddeval", NULL, value);
        }
      }
    }

  if (pt_bdd == zero)
    return ('0');
  else
    return ('1');
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_callagain						*/
/* description	: put an instance in the execute list			*/
/* called func.	: vhx_addexec						*/
/* ###--------------------------------------------------------------### */

void vhx_callagain (pt_lkdins)

  {
  }
