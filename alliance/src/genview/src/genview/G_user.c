/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                   G_user.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <signal.h>
# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"
# include "G_user.h"

/*------------------------------------------------------------\
|                                                             |
|                             Functions                       |
|                                                             |
\------------------------------------------------------------*/

void Update_BB (ptdesc)

    rdsrec_list *ptdesc;
{
    rdsrec_list *first;

    first = ptdesc;

    do
    {
      if (ptdesc->X < xvisu) xvisu = ptdesc->X;
      if (ptdesc->Y < yvisu) yvisu = ptdesc->Y;
      if ((ptdesc->Y + ptdesc->DY) > (yvisu + dyvisu))     
        dyvisu = (ptdesc->Y + ptdesc->DY) - yvisu;
      if ((ptdesc->X + ptdesc->DX) > (xvisu + dxvisu))
        dxvisu = (ptdesc->X + ptdesc->DX) - xvisu;

      ptdesc = (rdsrec_list *)(ptdesc->USER);
    }
    while ( first != ptdesc );
}

/***************************************************************************
*    New_insref : Seeks the reference instance in the instance list       *
***************************************************************************/

void New_insref ()
{
  rdsins_list *pt;

  pt = Visu_db->INSTANCE;

  while( pt && 
         GENVIEW_MBK( (pt->LAYERTAB[ RDS_ABOX ]) ) != WORK_PHINS )
  {
    pt = pt->NEXT;
  }

  if ( run_mode != RUN) 
  {
    V_DrawBox( Ptinsref->LAYERTAB[ RDS_ABOX ] );
    Ptinsref = pt;
    V_drawInsRef();
  }
}

/*******************************************************************************
*    G_error : prints an error message and exits              *
*******************************************************************************/

void G_error( mesg, code )

  char *mesg;
  int code;
{
  fprintf( stderr, "%s\n", mesg );
  EXIT( code );
}


/*******************************************************************************
*    Appart_win : checks if descriptor is included in a window      *
*******************************************************************************/

int Appart_win (flag, xdesc, ydesc, dxdesc, dydesc, xwin, ywin, dxwin, dywin)

  unsigned char *flag;
  long xdesc, ydesc, dxdesc, dydesc;
  long xwin, ywin, dxwin, dywin;
{
  long db, dg, deltax, deltay;

#ifdef TRACE
  fprintf(stderr,"appart_win : xdesc= %g ydesc= %g dxdesc= %g dydesc= %g\n",
        (float)xdesc/2,(float)ydesc/2,(float)dxdesc/2,(float)dydesc/2);
  fprintf(stderr,"       xwin= %g ywin= %g dxwin= %g dywin= %g\n",
        (float)xwin/2,(float)ywin/2,(float)dxwin/2,(float)dywin/2);
#endif
  db = ydesc - ywin;
  dg = xdesc - xwin;
  *flag = 0;

  if (db < 0) 
  {
    deltay = -db - dydesc;
    if (dg < 0) 
    {    /* zone 4 */
      deltax = -dg - dxdesc;
      if (deltax <= 0 && deltay <= 0) { /* LEFT and BOTTOM are not set */
        if (-deltay <= dywin)
          *flag |= APTOP;
        if (-deltax <= dxwin)
          *flag |= APRIGHT;
        /* In case of surround descriptor, all flags are set to 0 */
#ifdef TRACE
        fprintf (stderr, "appart_win OUT : flag= %#x\n", *flag);
#endif
        return 4;
      }
    }
    else 
    {    /* zone 3 */
      deltax = dg - dxwin;
      if (deltax <= 0 && deltay <= 0) { /* BOTTOM is not set */
        *flag |= APLEFT;
        if (dxdesc + deltax <= 0)
          *flag |= APRIGHT;
        if (-deltay <= dywin)
          *flag |= APTOP;
#ifdef TRACE
        fprintf (stderr, "appart_win OUT : flag= %#x\n", *flag);
#endif
        return 3;
      }
    }
  }
  else 
  {
    deltay = db - dywin;
    if (dg < 0) 
    {    /* zone 1 */
      deltax = -dg - dxdesc;
      if (deltax <= 0 && deltay <= 0) 
      { /* LEFT is not set */
        *flag |= APBOTTOM;
        if (-deltax <= dxwin)
          *flag |= APRIGHT;
        if (dydesc + deltay <= 0)
          *flag |= APTOP;
#ifdef TRACE
        fprintf (stderr, "appart_win OUT : flag= %#x\n", *flag);
#endif
        return 1;
      }
    } 
    else
    {    /* zone 2 */
      deltax = dg - dxwin;
      if (deltax <= 0 && deltay <= 0) 
      {
        *flag |= APLEFT;
        *flag |= APBOTTOM;
        if (dxdesc + deltax <= 0)
          *flag |= APRIGHT;
        if (dydesc + deltay <= 0)
          *flag |= APTOP;
#ifdef TRACE
        fprintf (stderr, "appart_win OUT : flag= %#x\n", *flag);
#endif
        return 2;
      }
    }
  }
#ifdef TRACE
  fprintf (stderr, "appart_win OUT : flag= %#x\n", *flag);
#endif
  return 0;
}

/*******************************************************************************
*    Troncate : adjusts the descriptor coordinates to the window    *
*******************************************************************************/

void Troncate (x, y, dx, dy, xwin, ywin, dxwin, dywin, flag)

  long *x, *y, *dx, *dy, xwin, ywin, dxwin, dywin;
  unsigned char flag;
{
  long xdesc, ydesc;

#ifdef TRACE
  fprintf (stderr, "troncate : x= %g y= %g dx= %g dy= %g\n",
           (float) *x / 2, (float) *y / 2, (float) *dx / 2, (float) *dy / 2);
  fprintf (stderr, "       xwin= %g ywin= %g dxwin= %g dywin= %g\n,flag= %#x\n",
           (float) xwin / 2, (float) ywin / 2, (float) dxwin / 2,
           (float) dywin / 2, flag);
#endif

  xdesc = *x;
  ydesc = *y;
  if (flag == APSURROUND) 
  {
    *x = xwin;
    *y = ywin;
    *dx = dxwin;
    *dy = dywin;
    return;
  }

  if (!(flag & APLEFT))
    *x = xwin;

  if (!(flag & APBOTTOM))
    *y = ywin;

  if (!(flag & APRIGHT))
    *dx = xwin + dxwin - *x;
  else
    *dx += xdesc - *x;

  if (!(flag & APTOP))
    *dy = ywin + dywin - *y;
  else
    *dy += ydesc - *y;

#ifdef TRACE
  fprintf(stderr, "troncate OUT : x= %g y= %g dx= %g dy= %g\n", 
         (float) *x / 2, (float) *y / 2, (float) *dx / 2, (float) *dy / 2);
#endif
}
