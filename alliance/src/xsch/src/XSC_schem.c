/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XSCH                            |
|                                                             |
| File    :                   Sch.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "mlo.h"
# include "XSB.h" 
# include "XSC.h"
# include "scl.h"

# include "XSC_schem.h"
# include "XSC_error.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  schfig_list  *XschFigureSch = (schfig_list  *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                       For Set Long Jump                     |
|                                                             |
\------------------------------------------------------------*/

  static jmp_buf XschJumpBuffer;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xsch Exit Handler Functions                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XschExitHandler                        |
|                                                             |
\------------------------------------------------------------*/

void XschExitHandler()
{
  XschFlushErrorMessage();

  longjmp( XschJumpBuffer, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                      XschActiveExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XschActiveExitHandler()
{
  MBK_EXIT_FUNCTION = XschExitHandler;
}

/*------------------------------------------------------------\
|                                                             |
|                       XschResetExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XschResetExitHandler()
{
  MBK_EXIT_FUNCTION = NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                          Order Functions                    |
|                                                             |
\------------------------------------------------------------*/

static void XschOrderLoconFigure( Figure )

  lofig_list *Figure;
{
  xschicon    *XschIcon;
  authelem    *Element;
  loins_list  *LoIns;
  locon_list **PrevLoCon;
  locon_list  *HeadLoCon;
  locon_list **QueueLoCon;
  locon_list  *ScanLoCon;
  chain_list  *ScanChain;

  for ( LoIns  = Figure->LOINS;
        LoIns != (loins_list *)0;
        LoIns  = LoIns->NEXT )
  {
    Element = searchauthelem( XSCH_HASH_ICON, LoIns->FIGNAME );

    if ( Element != (authelem *)0 )
    {
      XschIcon = (xschicon *)Element->VALUE;

      HeadLoCon  = (locon_list *)0;
      QueueLoCon = &HeadLoCon;

      for ( ScanChain  = XschIcon->CON_IN;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        PrevLoCon = &LoIns->LOCON;
        ScanLoCon = LoIns->LOCON;

        while ( ScanLoCon != (locon_list *)0 )
        {
          if ( ScanLoCon->NAME == (char *)ScanChain->DATA )
          {
            ScanLoCon->DIRECTION = IN;

            *PrevLoCon = ScanLoCon->NEXT;
            ScanLoCon->NEXT = (locon_list *)0;
            *QueueLoCon = ScanLoCon;
            QueueLoCon  = &ScanLoCon->NEXT;

            break;
          }

          PrevLoCon = &ScanLoCon->NEXT;
          ScanLoCon = ScanLoCon->NEXT;
        }
      }

      for ( ScanChain  = XschIcon->CON_OUT;
            ScanChain != (chain_list *)0;
            ScanChain  = ScanChain->NEXT )
      {
        PrevLoCon = &LoIns->LOCON;
        ScanLoCon = LoIns->LOCON;

        while ( ScanLoCon != (locon_list *)0 )
        {
          if ( ScanLoCon->NAME == (char *)ScanChain->DATA )
          {
            ScanLoCon->DIRECTION = OUT;

            *PrevLoCon = ScanLoCon->NEXT;
            ScanLoCon->NEXT = (locon_list *)0;
            *QueueLoCon = ScanLoCon;
            QueueLoCon  = &ScanLoCon->NEXT;

            break;
          }

          PrevLoCon = &ScanLoCon->NEXT;
          ScanLoCon = ScanLoCon->NEXT;
        }
      }

      *QueueLoCon  = LoIns->LOCON;
      LoIns->LOCON = HeadLoCon;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          Get Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Xschgetlofig                       |
|                                                             |
\------------------------------------------------------------*/

lofig_list *Xschgetlofig( FileName, Mode )

  char         *FileName;
  unsigned char Mode;
{
  lofig_list *Figure;

  autbegin();

  Figure = (lofig_list *)NULL;

  XschActiveExitHandler();

  if ( setjmp( XschJumpBuffer ) == 0 )
  {
    Figure = getlofig( FileName, Mode );

    XschResetExitHandler();

    XschOrderLoconFigure( Figure );

    autend();
    return( Figure );
  }

  XschResetExitHandler();

  dellofig( FileName );

  autend();
  return( (lofig_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Xschvhdlloadbefig                  |
|                                                             |
\------------------------------------------------------------*/

befig_list *Xschvhdlloadbefig( FileName, Mode )

  char         *FileName;
  unsigned char Mode;
{
  befig_list *Figure;

  autbegin();

  Figure = (befig_list *)NULL;

  XschActiveExitHandler();

  if ( setjmp( XschJumpBuffer ) == 0 )
  {
    Figure = vhdlloadbefig( (befig_list *)0, FileName, Mode );

    XschResetExitHandler();

    autend();
    return( Figure );
  }

  XschResetExitHandler();

  beh_frebefig( Figure );

  autend();
  return( (befig_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Xschgetschfig                      |
|                                                             |
\------------------------------------------------------------*/

schfig_list *Xschgetschfig( FileName )

  char *FileName;
{
  lofig_list  *FigureLo;
  befig_list  *FigureBeh;
  schfig_list *FigureSch;
  FILE        *File;
  int          ParseVbe;

  autbegin();

  FigureSch = (schfig_list *)0;
  ParseVbe  = 0; 

  if ( strcmp( XSCH_IN_SCHEM, "vbe" ) )
  {
    File = mbkfopen( FileName, IN_LO, "r" );

    if ( File != (FILE *)0 )
    {
      fclose( File );
    
      FigureLo  = Xschgetlofig( FileName, 'A' );

      if ( FigureLo != (lofig_list *)0 )
      {
        FigureSch = lofig2schfig( FigureLo, XSCH_VECTORIZED_MODE );
        placerouteschfig( FigureSch );
      }
    }
    else
    {
      ParseVbe = 1;
    }
  }
  else
  {
    ParseVbe = 1;
  }

  if ( ParseVbe )
  {
    FigureBeh = Xschvhdlloadbefig( FileName, 2 );

    if ( FigureBeh != (befig_list *)0 )
    {
      FigureSch = befig2schfig( FigureBeh );
      placerouteschfig( FigureSch );
    }
  }

  autend();
  return( FigureSch );
}
