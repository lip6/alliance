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
| File    :                  G_cache.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include MUT_H
# include MPH_H
# include <mbkgen.h>
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "G_global.h"
# include "G_cache.h"
# include "V_put.h"

# include "v_view.h"
# include "v_extern.h"

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/* Insert_visu : adds a descriptor in the database */

void Insert_visu ( ptdesc )

  rdsrec_list *ptdesc;
{
   if (run_mode != RUN)
   {
     V_DrawBox( ptdesc );
   }
}


/* Update_AB : changes the mother cell abutment box coordinates */

void Update_AB ()
{
   /* In case of DEF_AB */

   xabfig  = RDS_LAMBDA * xvisufig->XAB1 / SCALE_X;
   yabfig  = RDS_LAMBDA * xvisufig->YAB1 / SCALE_X;
   xab2fig = RDS_LAMBDA * xvisufig->XAB2 / SCALE_X;
   yab2fig = RDS_LAMBDA * xvisufig->YAB2 / SCALE_X;

   if ( ( xabfig < xab2fig ) &&
        ( yabfig < yab2fig ) )
   {
     xvisu  = xabfig;
     yvisu  = yabfig;
     dxvisu = xab2fig - xabfig;
     dyvisu = yab2fig - yabfig;
   }

   if (run_mode != RUN)
   {
      V_drawAB ();
   }
}

/* Del_cache : frees space allocated for the database */

void Del_cache ()
{
  if ( Visu_db != (rdsfig_list *)NULL )
  {
    delrdsfig( Visu_db->NAME );
  }

  if ( Peek_db != (rdsfig_list *)NULL )
  {
     delrdsfig( Peek_db->NAME );
  }

  Visu_db = addrdsfig( "_VISU_DB", GENVIEW_SIZE );
  Peek_db = addrdsfig( "_PEEK_DB", GENVIEW_SIZE );

  Ptinsref = (rdsins_list *)NULL;

  xabfig = yabfig = xab2fig = yab2fig = 0;
}

/* Fill_trans : adds a transistor into the database as two segments */

rdsrec_list *Fill_trans( ptseg )

  phseg_list *ptseg;
{
  rdsrec_list *ptdesc;
  rdsrec_list *First;

  ptdesc = segmbkrds( Visu_db, ptseg, 0 );

  if ( ptdesc != (rdsrec_list *)NULL )
  {
    First  = ptdesc;

    do
    {
      GENVIEW_MBK( ptdesc ) = (void *)ptseg;
  
      Update_BB( ptdesc );
      Insert_visu ( ptdesc );

      ptdesc = (rdsrec_list *)(ptdesc->USER);
    }
    while ( ptdesc != First );
  }

  return ptdesc;
}

/* Fill_seg : adds a segment into the database */

rdsrec_list *Fill_seg (ptseg)

   phseg_list *ptseg;
{
  rdsrec_list *ptdesc;
  rdsrec_list *First;

  ptdesc = segmbkrds( Visu_db, ptseg, 0 );

  if ( ptdesc != (rdsrec_list *)NULL )
  {
    First  = ptdesc;

    do
    {
      GENVIEW_MBK( ptdesc ) = (void *)ptseg;
  
      Update_BB( ptdesc );
      Insert_visu ( ptdesc );
  
      ptdesc = (rdsrec_list *)(ptdesc->USER);
    }
    while ( ptdesc != First );
  }

  return ptdesc;
}


/* Fill_via : adds a via into the database */

rdsrec_list *Fill_via (ptvia)

   phvia_list *ptvia;
{
  rdsrec_list *ptdesc;
  rdsrec_list *First;

  ptdesc = viambkrds( Visu_db, ptvia, 0 );

  if ( ptdesc != (rdsrec_list *)NULL )
  {
    First  = ptdesc; 

    do
    { 
      GENVIEW_MBK( ptdesc ) = (void *)ptvia;
  
      Update_BB( ptdesc );
      Insert_visu ( ptdesc );
  
      ptdesc = (rdsrec_list *)(ptdesc->USER);
    }
    while ( ptdesc != First );
  }

  return ptdesc;
}

/* Fill_figcon : adds a connector into the database */

rdsrec_list *Fill_figcon (ptcon)

   phcon_list *ptcon;
{
  rdsrec_list *ptdesc;

  ptdesc = conmbkrds( Visu_db, ptcon );

  if ( ptdesc != (rdsrec_list *)NULL )
  {
    ptdesc->NAME = nameindex( ptdesc->NAME, ptcon->INDEX );
    GENVIEW_MBK( ptdesc ) = (void *)ptcon;

    /* Uptdates bounding-box coordinates */

    Update_BB( ptdesc );
    Insert_visu ( ptdesc );
  }

  return ptdesc;
}

/* Fill_ref : adds a reference into the database */

rdsrec_list *Fill_ref (ptref)

   phref_list *ptref;
{
  rdsrec_list *ptdesc;
  rdsrec_list *First;

  ptdesc = refmbkrds( Visu_db, ptref );

  if ( ptdesc != (rdsrec_list *)NULL )
  {
    First  = ptdesc;

    do
    {
      GENVIEW_MBK( ptdesc ) = (void *)ptref;

      Update_BB( ptdesc );
      Insert_visu ( ptdesc );

      ptdesc = (rdsrec_list *)(ptdesc->USER);
    }
    while ( ptdesc != First );
  }

  return ptdesc;
}

/* Fill_ins : adds an instance descriptor into the database */

rdsins_list *Fill_ins( ptins )

   phins_list *ptins;
{
  phfig_list  *ModelFig;
  phcon_list  *ScanPhCon;
  rdsins_list *Instance;
  rdsrec_list *Rectangle;
  char         Layer;
  char         buffer[ 128 ];

  Instance = insmbkrds( Visu_db, ptins, 'P', 0 );
  ModelFig = getphfig( Instance->FIGNAME, 'P' );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    for ( Rectangle  = Instance->LAYERTAB[ Layer ];
          Rectangle != (rdsrec_list *)NULL;
          Rectangle  = Rectangle->NEXT )
    {
      if ( IsRdsConnector( Rectangle ) )
      {
        for ( ScanPhCon  = ModelFig->PHCON;
              ScanPhCon != (phcon_list *)0;
              ScanPhCon  = ScanPhCon->NEXT )
        {
          if ( ScanPhCon->NAME == Rectangle->NAME )
          {
            Rectangle->NAME = nameindex( Rectangle->NAME, ScanPhCon->INDEX );
            break;
          }
        }
      }

      GENVIEW_MBK( Rectangle ) = (void *)ptins;
      Rectangle->USER = (void *)Rectangle; 

      Update_BB( Rectangle );
      Insert_visu( Rectangle );
    }
  }

  return Instance;
}

void Fill_obj(ptype_list *p)
{
ptype_list *pp;
chain_list *pc;

   for (pp = p; pp; pp = pp->NEXT) {
      switch (pp->TYPE) {
         case MBK_INSTANCE:
            for (pc = pp->DATA; pc; pc = pc->NEXT) {
               if (pc->DATA)
               Ptinsref = Fill_ins(pc->DATA);
            }
            break;
         case MBK_SEGMENT:
            for (pc = pp->DATA; pc; pc = pc->NEXT) {
               if (pc->DATA)
               Fill_seg(pc->DATA);
            }
            break;
         case MBK_CONNECTOR:
            for (pc = pp->DATA; pc; pc = pc->NEXT) {
               if (pc->DATA)
               Fill_figcon(pc->DATA);
            }
            break;
         case MBK_REFERENCE:
            for (pc = pp->DATA; pc; pc = pc->NEXT) {
               if (pc->DATA)
               Fill_ref(pc->DATA);
            }
            break;
         case MBK_VIA:
            for (pc = pp->DATA; pc; pc = pc->NEXT) {
               if (pc->DATA)
               Fill_via(pc->DATA);
            }
            break;
         default:
      }
   }
}

/* Fill_cache : adds all figure descriptors into the database */

void Fill_cache ()
{
  phseg_list *ptseg;
  phvia_list *ptvia;
  phins_list *ptins;
  phcon_list *ptcon;
  phref_list *ptref;

  char old_run_mode = run_mode;

   run_mode = RUN;
   ptseg = xvisufig->PHSEG;
   while (ptseg){
      Fill_seg (ptseg);
      ptseg = ptseg->NEXT;
   }

   ptvia = xvisufig->PHVIA;
   while (ptvia){
      Fill_via (ptvia);
      ptvia = ptvia->NEXT;
   }

   ptins = xvisufig->PHINS;
   while (ptins)
   {
     Ptinsref = Fill_ins (ptins);
     ptins    = ptins->NEXT;
   }

   ptcon = xvisufig->PHCON;
   while (ptcon){
      Fill_figcon (ptcon);
      ptcon = ptcon->NEXT;
   }

   ptref = xvisufig->PHREF;
   while (ptref){
      Fill_ref (ptref);
      ptref = ptref->NEXT;
   }
   run_mode = old_run_mode;
}


/* Load_fig : loads the mother cell from disk */

void Load_fig(name)
char *name;
{
  int i;
  char buffer[BUFSIZ];
  phfig_list *pfig;
  char mode = run_mode;

  /* avoid a ab box burst before the display of the drawing */

  run_mode = RUN;

  sprintf(buffer, "Loading '%s', please wait\n",name);
  T_print(buffer);
  XFlush(display);

  for (pfig = HEAD_PHFIG; pfig; pfig = pfig->NEXT){
    delphfig(pfig->NAME); /* erase all cells in memory */
  }
  xvisu = yvisu = LONG_MAX;
  dxvisu = dyvisu = 0;
  Del_cache(); /* Erase old databases */

  Visu_db->NAME = name;

  xvisufig = NULL; /* if getlofig terminates, xvisufig is ok */ 
  xvisufig = getphfig(name, 'A');
  Update_AB();

  Fill_cache(); /* database loading, and then plotting */

  sprintf(buffer, 
"Bounding box : x:%g y:%g dx:%g dy:%g\nAbutment box : x:%g y:%g dx:%g dy:%g\n",
        (float)xvisu / 2, (float)yvisu / 2, 
        (float)dxvisu / 2, (float)dyvisu / 2,
        (float)(xvisufig->XAB1 / SCALE_X), (float)(xvisufig->YAB1 / SCALE_X),
        (float)((xvisufig->XAB2 - xvisufig->XAB1)/ SCALE_X), 
        (float)((xvisufig->YAB2 - xvisufig->YAB1)/ SCALE_X));
  T_print(buffer);
  zoomTrack.x = xscreen;
  zoomTrack.y = yscreen;
  zoomTrack.dx = dxscreen;
  zoomTrack.dy = dyscreen;
  zoomTrack.scale = scale;

  run_mode = mode;
}

/* Init_fig : changes the mother cell */

void Init_fig ()
{
  int i;
  int oldmode;

   /* In case of DEF_PHFIG */

   Ptinsref = (rdsins_list *)NULL;

   if ((xvisufig = WORK_PHFIG) == NULL) return;

   oldmode  = run_mode;
   run_mode = RUN;

   Update_AB();

   run_mode = oldmode;

   xvisu = yvisu = 0;
   dxvisu = dyvisu = 10;

   /* random initialisation because don't know bounding box */
   scale = 1;
   xscreen = yscreen = 0;
   dxscreen = dxWin;
   dyscreen = dyWin;

   /* Erase old database */
   Del_cache();
  
   Visu_db->NAME = xvisufig->NAME;

   /* Screen must be cleared because WORK_PHFIG has changed */

   V_clear ();
   V_changeName(xvisufig->NAME);

   zoomTrack.x = xscreen;
   zoomTrack.y = yscreen;
   zoomTrack.dx = dxscreen;
   zoomTrack.dy = dyscreen;
   zoomTrack.scale = scale;
}


/* Ram_load : loads the mother cell from disk */

void Ram_load()
{
  int i;
  phfig_list *pfig;
  char old_mode;

  Ptinsref = (rdsins_list *)NULL;

  XFlush (display);
  xvisufig = getphfig (WORK_PHFIG->NAME, 'A');

  /* avoid a ab box burst before the display of the drawing */

  old_mode = run_mode;
  run_mode = RUN;
  Update_AB ();

  xvisu = yvisu = LONG_MAX;
  dxvisu = dyvisu = 0;

  Del_cache(); /* Erase old databases */
  Fill_cache (); /* database loading, and then plotting */

  zoomTrack.x = xscreen;
  zoomTrack.y = yscreen;
  zoomTrack.dx = dxscreen;
  zoomTrack.dy = dyscreen;
  zoomTrack.scale = scale;
  Update_AB ();
  run_mode = old_mode;
  if (run_mode != RUN) V_refresh();
}
