/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| Tool    :                   XPAT                            |
|                                                             |
| File    :                   Edit.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <math.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XPT.h"
# include "XME.h"

# include "XME_edit.h"
# include "XME_message.h"

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
/*------------------------------------------------------------\
|                                                             |
|                        Identify Buffer                      |
|                                                             |
\------------------------------------------------------------*/

  static char  XpatIdentifyMessage[ XPAT_IDENTIFY_MESSAGE_SIZE ];
  static char  XpatIdentifyBuffer [ XPAT_IDENTIFY_BUFFER_SIZE  ];
  static char  XpatIdentifyLocout [ XPAT_IDENTIFY_LOCOUT_SIZE  ];

  static char *XpatScanIdentify;
  static long  XpatIdentifyLength;

  extern char *XpatTimeUnit[ PAT_TU__MS + 1 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XpatAddIdentify                     |
|                                                             |
\------------------------------------------------------------*/

short XpatAddIdentify( Obj, X1, Y1 )

  xpatobj_list *Obj;
  long          X1;
  long          Y1;
{
  char **NameArray;
  char **ModeArray;
  char  *Unit;
  long   X;
  long   Y;
  long   X2;
  long   NumberIO;
  long   Length;
  long   TimeDelta;

  autbegin();

  XpatIdentifyBuffer[0] = '\0';

  X1 /= XPAT_PATTERN_STEP_X;

  X  = Obj->X / ( XPAT_UNIT * XPAT_PATTERN_STEP_X );
  Y  = Obj->Y / ( XPAT_UNIT * XPAT_PATTERN_STEP_Y );
  X2 = ( Obj->X + Obj->DX ) / ( XPAT_UNIT * XPAT_PATTERN_STEP_X );

  NameArray = XpatFigurePat->IO_NAME_ARRAY;
  ModeArray = XpatFigurePat->IO_MODE_ARRAY;
  NumberIO  = XpatFigurePat->IO_NUMBER;

  if ( ( Y < NumberIO ) &&
       ( Y >= 0       ) )
  {
    Unit      = XpatTimeUnit[ XpatFigurePat->TIME_UNIT ];
    TimeDelta = XpatFigurePat->TIME_DELTA;

    sprintf( XpatIdentifyBuffer, "  IO NAME : %s\n  IO MODE: %s\n  PREV CHANGE : %d %s\n  CURRENT TIME : %d %s\n  NEXT CHANGE : %d %s\n  VALUE : %s\n\n",
             NameArray[ Y ], ModeArray[ Y ],
             X  * TimeDelta, Unit,
             X1 * TimeDelta, Unit,
             X2 * TimeDelta, Unit, Obj->NAME );
  }

  Length = strlen( XpatIdentifyBuffer );

  if ( ( XpatIdentifyLength + Length ) < ( XPAT_IDENTIFY_MESSAGE_SIZE - 1 ) )
  {
    strcpy( XpatScanIdentify, XpatIdentifyBuffer );

    XpatScanIdentify   += Length; 
    XpatIdentifyLength += Length;

    autend();
    return( XPAT_TRUE );
  }

  autend();
  return( XPAT_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatEditIdentify                    |
|                                                             |
\------------------------------------------------------------*/

void XpatEditIdentify( X1, Y1 )

  long X1;
  long Y1;
{
  xpatselect_list *Select;
  xpatobj_list    *Obj;

  strcpy( XpatIdentifyMessage, "No element found !" );

  XpatScanIdentify   = XpatIdentifyMessage;
  XpatIdentifyLength = 0;

  XpatEditSelectPoint( X1, Y1 );

  for ( Select  = XpatHeadSelect;
        Select != (xpatselect_list *)NULL;
        Select  = Select->NEXT )
  {
    if ( ( ! IsXpatCursor( Select->OBJECT) ) &&
         ( ! IsXpatIO( Select->OBJECT    ) ) )
    {
      if ( ! XpatAddIdentify( Select->OBJECT, X1, Y1 ) ) break;
    }
  }

  XpatDelSelect();

  XpatDisplayEditIdentify( XpatIdentifyMessage );
  XpatEnterPanel( &XpatEditIdentifyPanel       );
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatEditConnected                   |
|                                                             |
\------------------------------------------------------------*/

void XpatEditConnected( X1, Y1 )

  long X1;
  long Y1;
{
  xpatselect_list  *Select;
  xpatselect_list **PrevSelect;
  xpatobj_list     *Obj;

  if ( XpatHeadConnect != (xpatselect_list *)NULL )
  {
    XpatDelConnect();
    XpatZoomRefresh();
  }

  XpatEditSelectPoint( X1, Y1 );

  XpatDisplaySelectList( (char *)NULL );

  for ( Select  = XpatHeadSelect;
        Select != (xpatselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XpatAddSelectList( Select->OBJECT );
  }

  if ( XpatHeadSelect == (xpatselect_list *)NULL  )
  {
    XpatWarningMessage( XpatMainWindow, "No element found !" );
  }
  else
  {
    if ( XpatHeadSelect->NEXT != (xpatselect_list *)NULL )
    {
      XpatEnterPanel( &XpatEditSelectPanel );
      XpatLimitedLoop( XpatEditSelectPanel.PANEL );
    }
    else
    {
      XpatAcceptObject( XpatHeadSelect->OBJECT );
      XpatDisplayObject( XpatHeadSelect->OBJECT );
    }
  }

  XpatPurgeSelect();

  for ( Select  = XpatHeadSelect;
        Select != (xpatselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XpatAddConnect( Select->OBJECT );
  }

  if ( XpatHeadSelect != (xpatselect_list *)NULL )
  {
    if ( XpatHeadSelect->NEXT == (xpatselect_list *)NULL )
    {
      Obj = XpatHeadSelect->OBJECT;
      XpatDelSelect();
      XpatDisplayObject( Obj );
    }
    else
    {
      XpatDelSelect();
      XpatZoomRefresh();
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatEditAddCursor                   |
|                                                             |
\------------------------------------------------------------*/

void XpatEditAddCursor( X1, Y1 )

  long X1;
  long Y1;
{
  xpatobj_list *Obj;

  if ( XpatHeadConnect != (xpatselect_list *)NULL )
  {
    XpatDelConnect();
    XpatZoomRefresh();
  }

  if ( XpatFigure == (xpatfig_list *)NULL  )
  {
    XpatWarningMessage( XpatMainWindow, "No current figure !" );
  }
  else
  {
    Obj = XpatAddCursor( X1 );

    if ( Obj != (xpatobj_list *)0 )
    {
      XpatZoomRefresh();
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatEditDelCursor                   |
|                                                             |
\------------------------------------------------------------*/

void XpatEditDelCursor( X1, Y1 )

  long X1;
  long Y1;
{
  xpatselect_list  *Select;
  xpatselect_list **PrevSelect;
  xpatobj_list     *Obj;

  if ( XpatHeadConnect != (xpatselect_list *)NULL )
  {
    XpatDelConnect();
    XpatZoomRefresh();
  }

  XpatEditSelectPoint( X1, Y1 );

  for ( Select  = XpatHeadSelect;
        Select != (xpatselect_list *)NULL;
        Select  = Select->NEXT )
  {
    if ( IsXpatCursor( Select->OBJECT ) )
    {
      XpatAcceptObject( Select->OBJECT );
    }
  }

  XpatPurgeSelect();

  if ( XpatHeadSelect == (xpatselect_list *)NULL  )
  {
    XpatWarningMessage( XpatMainWindow, "No element found !" );
  }


  for ( Select  = XpatHeadSelect;
        Select != (xpatselect_list *)NULL;
        Select  = Select->NEXT )
  {
    XpatDelCursor( Select->OBJECT );
  }

  if ( XpatHeadSelect != (xpatselect_list *)NULL )
  {
    XpatDelSelect();
    XpatZoomRefresh();
  }
}
