/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/* ###--------------------------------------------------------------### */
/*                                                                      */
/* file     : fbl_drive.c                                               */
/* date     : Nouv 05 1992                                              */
/* version  : v100                                                      */
/* author   : VUONG H.N., M.HANAFI                                      */
/* description  : This file contains VHDL drivers :                     */
/*        vhdlsavefbfig()                                               */
/*                                                                      */
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "fvh.h"
#include "fbl_utype.h"
#include "fbl_utdef.h"
#include "fbl_drive.h"

  FILE *FblDriveFile;
  static int   synopsys = 0;

/* ###--------------------------------------------------------------### */
/* function : vhdlsavefbfig                     */
/* description  : print out a text file containing a data-flow VHDL */
/*        description                       */
/* called func. : vhdlablname, mbkalloc, fbl_message, fbl_error  , */
/*        fbl_toolbug , getptype, reverse    , fbl_printablfile */
/* ###--------------------------------------------------------------### */

void vhdlsavefbdecl( pthedfbfig, synopsys )

  fbfig_list *pthedfbfig;
  int         synopsys;
{
  char         *type_mark;
  int           left,right;
  char         *name;

  struct fbreg *ptfbreg   = 0;  /* current BEREG pointer    */
  struct fbaux *ptfbaux   = 0;  /* current BEAUX pointer    */
  struct fbbux *ptfbbux   = 0;  /* current BEBUX pointer    */
    /* ###------------------------------------------------------### */
        /*  Treatment of the BEREG list                 */
    /* ###------------------------------------------------------### */

  pthedfbfig->BEREG = (struct fbreg *)reverse( (chain_list *)pthedfbfig->BEREG);
  ptfbreg = pthedfbfig->BEREG;

  while (ptfbreg != 0)
  {
      ptfbreg = (fbreg_list *)fbl_vectnam(ptfbreg,&left,&right,&name,2);
      if(left != -1)
      {
        if ( synopsys )
        (void)fprintf(FblDriveFile,"  SIGNAL %s : BIT_VECTOR(%d %s %d);\t-- %s\n",
                      vhdlablname(name),left,(left>=right)?"DOWNTO":"TO",
                      right,name);
        else
        (void)fprintf(FblDriveFile,"  SIGNAL %s : REG_VECTOR(%d %s %d) REGISTER;\t-- %s\n",
                      vhdlablname(name),left,(left>=right)?"DOWNTO":"TO",
                      right,name);
      }
      else
      {
        if ( synopsys )
        (void) fprintf (FblDriveFile,"  SIGNAL %s : BIT;\t-- %s\n",
                      vhdlablname(name),name);
        else
        (void) fprintf (FblDriveFile,"  SIGNAL %s : REG_BIT REGISTER;\t-- %s\n",
                      vhdlablname(name),name);
      }
      ptfbreg = ptfbreg->NEXT;
  }

  pthedfbfig->BEREG = (struct fbreg *)reverse( (chain_list *)pthedfbfig->BEREG);

    /* ###------------------------------------------------------### */
        /*  Treatment of the BEBUX list                 */
    /* ###------------------------------------------------------### */

  pthedfbfig->BEBUX = (struct fbbux *)reverse( (chain_list *)pthedfbfig->BEBUX);
  ptfbbux = pthedfbfig->BEBUX;
  while (ptfbbux != 0)
    {
    ptfbbux = (fbbux_list *)fbl_vectnam(ptfbbux,&left,&right,&name,1);
    if(left != -1)
      {
      switch (ptfbbux->TYPE)
        {
          case FBH_TYPE_WOR_VEC :
          type_mark = namealloc("WOR_VECTOR"); 
          break;
          case FBH_TYPE_MUX_VEC :
          type_mark = namealloc("MUX_VECTOR"); 
          break;
        }
      (void)fprintf(FblDriveFile,"  SIGNAL %s : %s(%d %s %d) BUS;\t-- %s\n",
                    vhdlablname(name),type_mark,left,(left>=right)?"DOWNTO":"TO",
                    right,name);
      }
    else
      {
      switch (ptfbbux->TYPE)
        {
          case FBH_TYPE_WOR_BIT :
          type_mark = namealloc("WOR_BIT"); 
          break;
          case FBH_TYPE_MUX_BIT :
          type_mark = namealloc("MUX_BIT"); 
          break;
        }
      (void) fprintf (FblDriveFile,"  SIGNAL %s : %s BUS;\t\t-- %s\n",vhdlablname(name),
                      type_mark,name);
      }
    ptfbbux = ptfbbux->NEXT;
    }
  pthedfbfig->BEBUX = (struct fbbux *)reverse( (chain_list *)pthedfbfig->BEBUX);

  pthedfbfig->BEAUX = (struct fbaux *)reverse( (chain_list *)pthedfbfig->BEAUX);
  ptfbaux = pthedfbfig->BEAUX;
  while (ptfbaux != 0)
    {
      if ( ( ptfbaux->ABL != (chain_list *)0 ) ||
           ( IsFbhAssignByFsm( ptfbaux )     ) )
      {
    ptfbaux = (fbaux_list *)fbl_vectnam(ptfbaux,&left,&right,&name,3);
    if(left != -1)
      {
      (void)fprintf(FblDriveFile,"  SIGNAL %s : BIT_VECTOR(%d %s %d);\t-- %s\n",
                    vhdlablname(name),left,(left>=right)?"DOWNTO":"TO",
                    right,name);
      }
    else
      {
      (void) fprintf (FblDriveFile,"  SIGNAL %s : BIT;\t\t-- %s\n",
                    vhdlablname(name),name);
      }
      }
    ptfbaux = ptfbaux->NEXT;
    }
  pthedfbfig->BEAUX = (struct fbaux *)reverse( (chain_list *)pthedfbfig->BEAUX);

}

void vhdlsavefbport( pthedfbfig, synopsys )

  fbfig_list *pthedfbfig;
  int         synopsys;
{
  char         *mode;
  char         *type_mark;
  int           left,right;
  char         *name;
  char         *bus;

  struct fbpor *ptfbpor   = 0;  /* correctly ordered port list  */

  ptfbpor = pthedfbfig->BEPOR;
  if (ptfbpor != 0)
    {
    (void) fprintf (FblDriveFile,"  PORT (\n");
    pthedfbfig->BEPOR = (struct fbpor *)reverse ( (chain_list *)pthedfbfig->BEPOR);

    ptfbpor = pthedfbfig->BEPOR;
    while (ptfbpor != 0)
      {
      switch (ptfbpor->DIRECTION)
        {
        case 'I':
          mode = namealloc("IN"); 
          break;
        case 'O':
        case 'Z':
          mode = namealloc("OUT"); 
          break;
        case 'B':
        case 'T':
          mode = namealloc("INOUT"); 
          break;
        default :
          {
          fbl_error (69,0);
          }
        }
      ptfbpor = (fbpor_list *)fbl_vectnam(ptfbpor,&left,&right,&name,0);

      if(left != -1)
        {
        switch (ptfbpor->TYPE)
          {
            case FBH_TYPE_BIT_VEC :
            type_mark = namealloc("BIT_VECTOR"); 
            bus = "";
            break;
            case FBH_TYPE_WOR_VEC :
            type_mark = namealloc("WOR_VECTOR"); 
            bus = namealloc("BUS");
            break;
            case FBH_TYPE_MUX_VEC :
            type_mark = namealloc("MUX_VECTOR"); 
            bus = namealloc("BUS");
            break;
          default :
            {
            fbl_error (68,0);
            }
          }
        (void)fprintf(FblDriveFile,"  %s : %s %s(%d %s %d) %s",vhdlablname(name), mode,
                      type_mark, left, (left>=right)?"DOWNTO":"TO",right,bus);
        }
      else
        {
        switch (ptfbpor->TYPE)
          {
            case FBH_TYPE_BIT :
            type_mark = "BIT"; break;
            case FBH_TYPE_WOR_BIT :
            type_mark = "WOR_BIT BUS"; break;
            case FBH_TYPE_MUX_BIT :
            type_mark = "MUX_BIT BUS"; break;
          default :
            {
            fbl_error (68,0);
            }
          }

        (void) fprintf (FblDriveFile,"  %s : %s %s",vhdlablname(name),
                                          mode,type_mark);
        }

      if (ptfbpor->NEXT != 0)
        (void) fprintf (FblDriveFile,";\t-- %s\n",name);
      else
        (void) fprintf (FblDriveFile,"\t-- %s\n  );\n",name);

      ptfbpor = ptfbpor->NEXT;
      }

    pthedfbfig->BEPOR = (struct fbpor *)reverse ( (chain_list *)pthedfbfig->BEPOR);
    }

}


void vhdlsavefbarchi( pthedfbfig, synopsys )

  fbfig_list *pthedfbfig;
  int         synopsys;
{
  int           nrlabel = 0;
  struct fbreg *ptfbreg   = 0;  /* current BEREG pointer    */
  struct fbmsg *ptfbmsg   = 0;  /* current BEMSG pointer    */
  struct fbout *ptfbout   = 0;  /* current BEOUT pointer    */
  struct fbbus *ptfbbus   = 0;  /* current BEBUS pointer    */
  struct fbaux *ptfbaux   = 0;  /* current BEAUX pointer    */
  struct fbbux *ptfbbux   = 0;  /* current BEBUX pointer    */
  struct fbbiabl *ptbiabl   = 0;  /* current BIABL pointer    */

    /* ###------------------------------------------------------### */
    /*    Print out a concurrent assert statement for each BEMSG    */
    /* ###------------------------------------------------------### */

  ptfbmsg = pthedfbfig->BEMSG;
  while (ptfbmsg != 0)
    {
    if (ptfbmsg->LABEL != 0)
      (void)fprintf(FblDriveFile,"  %s :",ptfbmsg->LABEL);

    (void) fprintf (FblDriveFile,"  ASSERT (" );
    fbl_printablfile(FblDriveFile, ptfbmsg->ABL );
    (void) fprintf (FblDriveFile," = '1')\n" );

    if (ptfbmsg->MESSAGE != 0)
      (void) fprintf (FblDriveFile,"    REPORT \"%s\"\n",ptfbmsg->MESSAGE);

    if (ptfbmsg->LEVEL == 'W')
      (void) fprintf (FblDriveFile,"    SEVERITY WARNING;");
    else
      (void) fprintf (FblDriveFile,"    SEVERITY ERROR;");

    (void) fprintf (FblDriveFile,"\n\n");
    ptfbmsg = ptfbmsg->NEXT;
    }

    /* ###------------------------------------------------------### */
    /*    Print out a concurrent signal assignment for each BEAUX   */
    /* ###------------------------------------------------------### */

  ptfbaux = pthedfbfig->BEAUX;

  while (ptfbaux != 0)
  {
    if (ptfbaux->ABL != 0)
      {
      (void) fprintf (FblDriveFile,"  %s <= ",fbl_vectorize(ptfbaux->NAME));
      fbl_printablfile( FblDriveFile, ptfbaux->ABL);
      (void) fprintf (FblDriveFile,";\n");
      }
    /* LUDO
    else
    {
      fbl_error (40,ptfbaux->NAME);
    } */
    ptfbaux = ptfbaux->NEXT;
  }

    /* ###------------------------------------------------------### */
    /*    Print out a block statement  with one guarded concurrent  */
    /* signal assignment for each BIABL of each BEREG       */
    /* ###------------------------------------------------------### */

  ptfbreg = pthedfbfig->BEREG;

  while (ptfbreg != 0)
  {
    ptbiabl = ptfbreg->BIABL;
    while (ptbiabl != 0)
      {
      if ( synopsys )
      (void) fprintf (FblDriveFile,"  proc_%d : PROCESS ",nrlabel);
      else
      (void) fprintf (FblDriveFile,"  label%d : BLOCK ",nrlabel);
      if (ptbiabl->CNDABL  != 0)
      {
        if ( ! synopsys )
        {
          (void) fprintf (FblDriveFile,"(" );
          fbl_printablfile( FblDriveFile, ptbiabl->CNDABL );
          (void) fprintf (FblDriveFile," = '1')\n" );
        }
      }
      else
        fbl_toolbug (19,"fbl_decomp",ptfbreg->NAME,0);
  
      (void) fprintf (FblDriveFile,"  BEGIN\n" );

      if ( synopsys )
      {
          (void) fprintf (FblDriveFile,"   WAIT UNTIL (" );
          fbl_printablfile( FblDriveFile, ptbiabl->CNDABL );
          (void) fprintf (FblDriveFile," = '1');\n" );
      }

      (void) fprintf (FblDriveFile,"    %s <= %s ",
                      fbl_vectorize(ptfbreg->NAME), 
                      ( synopsys ) ? "" : "GUARDED" );
      if (ptbiabl->VALABL  != 0)
        {
        fbl_printablfile( FblDriveFile, ptbiabl->VALABL );
        (void)fprintf (FblDriveFile,";\n" );
        }
      else
        fbl_toolbug (20,"fbl_decomp",ptfbreg->NAME,0);

      if ( synopsys )
      (void) fprintf( FblDriveFile,"  END PROCESS proc_%d;\n",nrlabel);
      else
      (void) fprintf( FblDriveFile,"  END BLOCK label%d;\n",nrlabel);
      ptbiabl = ptbiabl->NEXT;
      nrlabel++;
      }
  
      ptfbreg = ptfbreg->NEXT;
  }

    /* ###------------------------------------------------------### */
    /*    Print out a block statement  with one guarded concurrent  */
    /* signal assignment for each BIABL of each BEBUX       */
    /* ###------------------------------------------------------### */

  ptfbbux = pthedfbfig->BEBUX;
  while (ptfbbux != 0)
  {
    ptbiabl = ptfbbux->BIABL;
    while (ptbiabl != 0)
      {
      (void) fprintf (FblDriveFile,"  label%d : BLOCK (",nrlabel);
      if (ptbiabl->CNDABL != 0)
        {
        fbl_printablfile( FblDriveFile, ptbiabl->CNDABL );
        (void) fprintf (FblDriveFile," = '1')\n" );
        }
      else
        fbl_toolbug (19,"fbl_decomp",ptfbbux->NAME,0);
  
      (void) fprintf (FblDriveFile,"  BEGIN\n    %s <= GUARDED ",
                      fbl_vectorize(ptfbbux->NAME));
      if (ptbiabl->VALABL != 0)
        {
        fbl_printablfile( FblDriveFile, ptbiabl->VALABL );
        (void) fprintf (FblDriveFile,";\n" );
        }
      else
        fbl_toolbug (20,"fbl_decomp",ptfbbux->NAME,0);

      (void) fprintf (FblDriveFile,"  END BLOCK label%d;\n",nrlabel);
      ptbiabl = ptbiabl->NEXT;
      nrlabel++;
      }
  
    ptfbbux = ptfbbux->NEXT;
  }

    /* ###------------------------------------------------------### */
    /*    Print out a block statement  with one guarded concurrent  */
    /* signal assignment for each BIABL of each BEBUS       */
    /* ###------------------------------------------------------### */

  ptfbbus = pthedfbfig->BEBUS;
  while (ptfbbus != 0)
  {
    ptbiabl = ptfbbus->BIABL;
    while (ptbiabl != 0)
      {
      (void) fprintf (FblDriveFile,"\tlabel%d : BLOCK (",nrlabel);
      if (ptbiabl->CNDABL != 0)
        {
        fbl_printablfile ( FblDriveFile, ptbiabl->CNDABL );
        (void) fprintf (FblDriveFile," = '1')\n" );
        }
      else
        fbl_toolbug (19,"fbl_decomp",ptfbbus->NAME,0);
  
      (void) fprintf (FblDriveFile,"\tBEGIN\n\t%s <= GUARDED ",fbl_vectorize(ptfbbus->NAME));
      if (ptbiabl->VALABL != 0)
        {
        fbl_printablfile( FblDriveFile, ptbiabl->VALABL );
        (void) fprintf (FblDriveFile,";\n" );
        }
      else
        fbl_toolbug (20,"fbl_decomp",ptfbbus->NAME,0);

      (void) fprintf (FblDriveFile,"\tEND BLOCK label%d;\n",nrlabel);
      ptbiabl = ptbiabl->NEXT;
      nrlabel++;
      }
  
    ptfbbus = ptfbbus->NEXT;
  }

    /* ###------------------------------------------------------### */
    /*    Print out a concurrent signal assignment for each BEOUT   */
    /* ###------------------------------------------------------### */

  ptfbout = pthedfbfig->BEOUT;

  while (ptfbout != 0)
  {
    if (ptfbout->ABL != 0)
      {
      (void) fprintf (FblDriveFile,"\n%s <= ",fbl_vectorize(ptfbout->NAME) );
      fbl_printablfile( FblDriveFile, ptfbout->ABL );
      (void) fprintf (FblDriveFile,";\n" );
      }
    else
    {
      fbl_error (40,ptfbout->NAME);
    }

    ptfbout = ptfbout->NEXT;
  }

}

void vhdlsavefbfig( pthedfbfig, FileName, synopsys )

fbfig_list  *pthedfbfig;
char        *FileName;
int          synopsys;
{
  time_t        clock;

  struct fbgen *ptgeneric = 0;  /* current ptype pnt (generic)  */
  struct ptype *ptptype   = 0;  /* current ptype pointer (USER) */

  if (pthedfbfig == 0)
    fbl_toolbug (10,"fbl_decomp",0,0);

  fbl_message (11,0,0);

    /* ###------------------------------------------------------### */
    /*    Opening result file                   */
    /* ###------------------------------------------------------### */

  if ((FblDriveFile = mbkfopen( FileName, ( synopsys ) ? "vhd" : "vbe",WRITE_TEXT)) == 0)
    {
    fbl_error (107,0);
    autexit (1);
    }

  fbl_message (12,pthedfbfig->NAME,0);
  fbl_message (13,pthedfbfig->NAME,0);

  time (&clock);
  (void) fprintf (FblDriveFile,"-- VHDL data flow description generated from `%s`\n",
                  pthedfbfig->NAME);
  (void) fprintf (FblDriveFile,"--\t\tdate : %s\n\n",ctime(&clock));

    /* ###------------------------------------------------------### */
    /*    Entity declaration                    */
    /* ###------------------------------------------------------### */

  (void) fprintf (FblDriveFile,"-- Entity Declaration\n\n");
  (void) fprintf (FblDriveFile,"ENTITY %s IS\n",vhdlablname(pthedfbfig->NAME));

    /* ###------------------------------------------------------### */
    /*    Generic declaration                   */
    /* ###------------------------------------------------------### */

  ptptype = getptype (pthedfbfig->USER,FBH_GENERIC);
  if (ptptype != 0)
    {
    ptptype->DATA = (void *)reverse (ptptype->DATA);
    ptgeneric     = (struct fbgen *)ptptype->DATA;
    (void) fprintf (FblDriveFile,"  GENERIC (\n");
    while (ptgeneric != 0)
      {
      (void) fprintf (FblDriveFile,"    CONSTANT %s : NATURAL := %ld",
                     vhdlablname(ptgeneric->NAME),*((long *)ptgeneric->VALUE));
      if (ptgeneric->NEXT != 0)
        (void)fprintf(FblDriveFile,";\t-- %s\n",(char *)ptgeneric->NAME);
      else
        (void)fprintf(FblDriveFile,"\t-- %s\n",(char *)ptgeneric->NAME);

      ptgeneric = ptgeneric->NEXT;
      }
    (void) fprintf (FblDriveFile,"  );\n");
    ptptype->DATA = (void *)reverse (ptptype->DATA);
    }

    /* ###------------------------------------------------------### */
    /*    Port declaration                      */
    /* ###------------------------------------------------------### */

    vhdlsavefbport( pthedfbfig, synopsys );

  (void) fprintf (FblDriveFile,"END %s;\n\n\n",vhdlablname(pthedfbfig->NAME));

    /* ###------------------------------------------------------### */
    /*    Architecture declaration                  */
    /* ###------------------------------------------------------### */

  (void) fprintf (FblDriveFile,"-- Architecture Declaration\n\n");
  (void) fprintf (FblDriveFile,"ARCHITECTURE VBE OF %s IS\n",
                  vhdlablname(pthedfbfig->NAME));

  vhdlsavefbdecl( pthedfbfig, synopsys );

  (void) fprintf (FblDriveFile,"\nBEGIN\n");

  vhdlsavefbarchi( pthedfbfig, synopsys );
    
  (void) fprintf (FblDriveFile,"END;\n");
  fbl_message    (14,pthedfbfig->NAME,0);
  (void) fclose  (FblDriveFile);

}
