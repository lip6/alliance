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
| Tool    :                     XSCH                          |
|                                                             |
| File    :                  XsbParse.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdlib.h>
# include <string.h>
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "XSB.h"
# include "XSB_error.h"
# include "XSB_parse.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XsbError( X, Y, Z ) XsbPrintError( basename(__FILE__), __LINE__, X, Y, Z )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Global Variables                      |
|                                                             |
\------------------------------------------------------------*/

  char *XSCH_PARAM_NAME = (char *)NULL;
  char *XSCH_LAYER_NAME_TABLE[ XSCH_MAX_LAYER ][ 3 ];
  long  XSCH_LAYER_XFIG_COLOR[ XSCH_MAX_LAYER ];

  authtable *XSCH_HASH_ICON = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                      Private Variables                      |
|                                                             |
\------------------------------------------------------------*/

  static char *XschDefaultTechnoName = XSCH_DEFAULT_PARAM_NAME;

/*------------------------------------------------------------\
|                                                             |
|                       Table variables                       |
|                                                             |
\------------------------------------------------------------*/

  char  *XSCH_CURSOR_COLOR_NAME;
  char  *XSCH_BACKGROUND_COLOR_NAME;
  char  *XSCH_FOREGROUND_COLOR_NAME;
  char  *XSCH_ACCEPT_COLOR_NAME;
  char  *XSCH_CONNECT_COLOR_NAME;

  long   XSCH_CURSOR_SIZE;
  float  XSCH_LOWER_GRID_STEP;
  long   XSCH_UNIT;

  char  *XSCH_NOT_DEFINE = "";

/*------------------------------------------------------------\
|                                                             |
|                      Keywords variables                     |
|                                                             |
\------------------------------------------------------------*/

  static char  KeywordDefined = 0;

  static char *DefineKeyword;
  static char *TableKeyword;
  static char *EndTableKeyword;
  static char *EndRecordKeyword;
  static char *LowerGridStepKeyword;
  static char *CursorColorNameKeyword;
  static char *BackgroundColorNameKeyword;
  static char *ForegroundColorNameKeyword;
  static char *AcceptColorNameKeyword;
  static char *ConnectColorNameKeyword;
  static char *CursorSizeKeyword;
  static char *UnitKeyword;
  static char *LayerNameKeyword;
  static char *IconCellBufferKeyword;
  static char *IconCellAndKeyword;
  static char *IconCellOrKeyword;
  static char *IconCellXorKeyword;
  static char *IconCellTristateKeyword;
  static char *IconCellRegisterKeyword;
  static char *IconCellConstantKeyword;
  static char *IconCellAndOrKeyword;
  static char *IconCellOrAndKeyword;
  static char *IconCellMuxKeyword;

  static xsb_keyword KeywordDefine[ XSB_MAX_KEYWORD ] =

  {
    { "None", 0 }
  };

/*------------------------------------------------------------\
|                                                             |
|                       File variables                        |
|                                                             |
\------------------------------------------------------------*/

  static  FILE *XsbFile;
  static  char  XsbBuffer[ XSB_MAX_BUFFER ];
  static  long  XsbCurrentLine;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Xsb File Get String                   |
|                                                             |
\------------------------------------------------------------*/

char *XsbFileGetString( String, Size )

  char *String;
  int   Size;
{
  register char *RegisterString;
  register       Register;

  autbegin();
 
  RegisterString = String;
 
  while (--Size > 0 && (Register = getc( XsbFile )) != EOF )
  {
    if ((*RegisterString++ = Register) == '\\') 
    {
      if ((Register = getc( XsbFile )) == '\n') 
      {
        XsbCurrentLine++;

        *(RegisterString - 1) = ' ';
      } 
      else
      {
        ungetc( Register, XsbFile );
      }
    }
    else
    {
      if ( Register == '\n') break;
    }
  }
 
  *RegisterString = '\0';
 
  autend();
  return ( ( Register       == EOF    ) && 
           ( RegisterString == String ) ) ? (char *)NULL : String;
}

/*------------------------------------------------------------\
|                                                             |
|                       Xsb File Get Line                     |
|                                                             |
\------------------------------------------------------------*/

int XsbGetLine( Buffer )

  char   *Buffer;
{
  char   *Check;
  char   *String;
  char    OneComment;

  do 
  {
    OneComment = 0;
 
    Check = XsbFileGetString( Buffer, XSB_MAX_BUFFER );
 
    if ( Check != (char *)NULL ) 
    {
      XsbCurrentLine++;
    }
    else
    {
      return( 0 );
    }
 
    if ( ( String = strchr( Buffer, XSB_COMMENT_CHAR )) != (char *)0 )
    {
      if ( String == Buffer )
      {
        OneComment = 1;
      }
      /*
      else
      {
        *(String - 1) = '\0'; 
      }
      */
    }
  
    while (*Buffer != '\0' && strchr( XSB_SEPARATORS_STRING, *Buffer))
    {
      Buffer = Buffer + 1;;
    }
 
    if (*Buffer == '\0') OneComment = 1;
 
  } 
  while ( OneComment == 1);

  return( 1 );
}      

/*------------------------------------------------------------\
|                                                             |
|                        Xsb Check Keyword                    |
|                                                             |
\------------------------------------------------------------*/

char XsbCheckKeyword( Word )

  char *Word;
{
  if ( ( Word == DefineKeyword ) ||
       ( Word == TableKeyword  ) )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Xsb Get First word                   |
|                                                             |
\------------------------------------------------------------*/

char *XsbGetFirstWord( Buffer, IsKeyword, Hash )
 
  char *Buffer;
  char  IsKeyword;
  char  Hash;
{
  char   *String;
  char   *Text;
  int     Index;
 
  autbegin();
 
  if ( ( String = (char *)strtok( Buffer, XSB_SEPARATORS_STRING )) != (char *)0 )
  {
    if ( Hash )
    {
      String = namealloc( String );
    }
    else
    {
      Text = autallocblock( strlen( String ) + 1 );
      strcpy( Text, String );
      String = Text;
 
      for ( Index = 0; Text[ Index ]; Index++ )
      {
        if ( Text[ Index ] == '_' ) Text[ Index ] = ' '; 
      }
    }
 
    if ( ( IsKeyword ) && XsbCheckKeyword( String ) )
    {
      XsbError( UNEXPECTED_LINE, String, XsbCurrentLine );
    }
  }

  autend();
  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                        Xsb Get Next word                    |
|                                                             |
\------------------------------------------------------------*/

char *XsbGetNextWord( IsKeyword, Hash )
 
  char IsKeyword;
  char Hash;
{
  char   *String;
  char   *Text;
  int     Index;

  autbegin();
 
  if ( ( String = (char *)strtok( (char *)NULL, XSB_SEPARATORS_STRING )) != (char *)0 )
  {
     if ( Hash )
     {
       String = namealloc( String );
     }
     else
     {
       Text = autallocblock( strlen( String ) + 1 );
       strcpy( Text, String );
       String = Text;
 
       for ( Index = 0; Text[ Index ]; Index++ )
       {
         if ( Text[ Index ] == '_' ) Text[ Index ] = ' ';
       }
     }
 
     if ( ( IsKeyword ) && XsbCheckKeyword( String ) )
     {
       XsbError( UNEXPECTED_LINE, String, XsbCurrentLine );
     }
  }
 
  autend();
  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                      Xsb Keyword Compare                    |
|                                                             |
\------------------------------------------------------------*/

int XsbKeywordCompare( FirstKey, SecondKey )

    xsb_keyword *FirstKey;
    xsb_keyword *SecondKey;
{
  return strcmp( FirstKey->NAME, SecondKey->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                      Xsb Get String Value                   |
|                                                             |
\------------------------------------------------------------*/

long XsbGetStringValue( String )

  char     *String;
{
  long      Value;
  xsb_keyword  *Keyword;
  xsb_keyword   Entry;

  autbegin();
 
  if ( sscanf( String, "%ld", &Value) != 0 )
  {
    autend();
    return ( Value );
  }

  Entry.NAME = String;
 
  Keyword = (xsb_keyword *)bsearch( (char *)(&Entry), 
                                (char *)KeywordDefine, 
                                XSB_MAX_KEYWORD, sizeof( xsb_keyword ),
                                XsbKeywordCompare );
 
  if ( Keyword == (xsb_keyword *)NULL )
  {
    XsbError( UNEXPECTED_LINE, String, XsbCurrentLine );
  }

  autend();
  return( Keyword->VALUE );
}

/*------------------------------------------------------------\
|                                                             |
|                      Xsb Get String Float                   |
|                                                             |
\------------------------------------------------------------*/

float XsbGetStringFloat( String )

   char     *String;
{
  float     Value;

  autbegin();

  if ( ! sscanf( String, "%g", &Value) )
  {
    XsbError( ILLEGAL_FLOAT, String, XsbCurrentLine );
  }

  autend();
  return ( Value );
}

/*------------------------------------------------------------\
|                                                             |
|                        Xsb Get Number                       |
|                                                             |
\------------------------------------------------------------*/

long XsbGetNumber( String )

     char *String;
{
  long Value;

  autbegin();

  if ( ! sscanf( String, "%ld", &Value ))
  {
    XsbError( UNEXPECTED_LINE, "number", XsbCurrentLine );
  }

  autend();
  return Value;
}

/*------------------------------------------------------------\
|                                                             |
|                        Xsb Get Name List                    |
|                                                             |
\------------------------------------------------------------*/

chain_list *XsbGetNameList( String )

  char *String;
{
  chain_list *HeadList;
  char       *Scan;

  HeadList = (chain_list *)0;

  while ( ( Scan = strchr( String, ',' ) ) != (char *)0 )
  {
    *Scan = '\0';
    HeadList = addchain( HeadList, namealloc( String ) );
    String   = Scan + 1;
  }

  HeadList = addchain( HeadList, namealloc( String ) );

  return( HeadList );
}

/*------------------------------------------------------------\
|                                                             |
|                       Xsb Read Icon Cell                    |
|                                                             |
\------------------------------------------------------------*/

void XsbReadIconCell( Type, FlagIn, FlagOut )

  unsigned char Type;
  unsigned char FlagIn;
  unsigned char FlagOut;
{
  int       EndTable;
  char     *FirstWord;
  char     *NextWord;
  xschicon *XschIcon;
  int       NumberArgs;

  autbegin();

  EndTable = 0;

  while ( EndTable != 1 )
  {
    if ( ! XsbGetLine( XsbBuffer ) )
    {
      XsbError( UNEXPECTED_EOF, (char *)NULL, XsbCurrentLine );
    }

    FirstWord = XsbGetFirstWord( XsbBuffer, 0, 1 );

    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    {
      XschIcon = (xschicon *)autallocheap( sizeof( xschicon ) );
      XschIcon->NAME = FirstWord;
      XschIcon->TYPE = Type;

      addauthelem( XSCH_HASH_ICON, FirstWord, (long)XschIcon );

      if ( FlagIn )
      {
        NextWord = XsbGetNextWord( 0, 0 );

        if ( NextWord == EndRecordKeyword )
        {
          XsbError( MISSING_VALUE, (char *)NULL, XsbCurrentLine );
        }

        XschIcon->CON_IN = XsbGetNameList( NextWord );
      }

      if ( FlagOut )
      {
        NextWord = XsbGetNextWord( 0, 0 );

        if ( NextWord == EndRecordKeyword )
        {
          XsbError( MISSING_VALUE, (char *)NULL, XsbCurrentLine );
        }

        XschIcon->CON_OUT = XsbGetNameList( NextWord );
      }

      NumberArgs = 0;

      while ( ( NumberArgs < 6                                            ) &&
              ( ( NextWord = XsbGetNextWord( 0, 1 ) ) != EndRecordKeyword ) )
      {
        XschIcon->ARGS[ NumberArgs ] = XsbGetStringValue( NextWord );
        NumberArgs++;
      }
# ifdef DEBUG
      {
        chain_list *ScanChain;

        fprintf( stdout, "--> XschIcon\n" );
        fprintf( stdout, "NAME   : %s\n", XschIcon->NAME );
        fprintf( stdout, "CON_IN : " );

        for ( ScanChain  = XschIcon->CON_IN;
              ScanChain != (chain_list *)0;
              ScanChain  = ScanChain->NEXT )
        {
          fprintf( stdout, "%s", (char *)ScanChain->DATA );

          if ( ScanChain->NEXT != (chain_list *)0 )
          {
            fprintf( stdout, "," );
          }
        }

        fprintf( stdout, "\n" );
        fprintf( stdout, "CON_OUT: " );

        for ( ScanChain  = XschIcon->CON_OUT;
              ScanChain != (chain_list *)0;
              ScanChain  = ScanChain->NEXT )
        {
          fprintf( stdout, "%s", (char *)ScanChain->DATA );

          if ( ScanChain->NEXT != (chain_list *)0 )
          {
            fprintf( stdout, "," );
          }
        }

        fprintf( stdout, "\n" );
        fprintf( stdout, "ARGS   : " );

        for ( NumberArgs = 0; NumberArgs < 6; NumberArgs++ )
        {
          fprintf( stdout, "%d ", XschIcon->ARGS[ NumberArgs ] );
        }

        fprintf( stdout, "\n" );
        fprintf( stdout, "<-- XschIcon\n" );
      }
# endif
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Xsb Read Layer Name                   |
|                                                             |
\------------------------------------------------------------*/

void XsbReadLayerName()

{
  int   Layer;
  int   LayerCount;
  int   EndTable;
  int   Field;
  char *FirstWord;

  autbegin();

  EndTable   = 0;
  LayerCount = 0;

  while ( ( EndTable   != 1              ) &&
          ( LayerCount <= XSCH_MAX_LAYER ) )
  {
    if ( ! XsbGetLine( XsbBuffer ) )
    {
      XsbError( UNEXPECTED_EOF, (char *)NULL, XsbCurrentLine );
    }

    FirstWord  = XsbGetFirstWord( XsbBuffer, 1, 1 );

    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < XSCH_MAX_LAYER )
    {
      Layer = XsbGetStringValue( FirstWord );

      for ( Field = 0; Field < 4; Field++ )
      {
        FirstWord = XsbGetNextWord( 1, 0 );

        if ( FirstWord == EndRecordKeyword )
        {
          XsbError( MISSING_VALUE, (char *)NULL, XsbCurrentLine );
        }
        else
        if ( Field != 3 )
        {
          XSCH_LAYER_NAME_TABLE [ Layer ][ Field ] = FirstWord;
        }
        else
        {
          sscanf( FirstWord, "%lx", &XSCH_LAYER_XFIG_COLOR[ Layer ] );
        }
      }

      FirstWord = XsbGetNextWord( 0, 1 );

      if ( FirstWord != EndRecordKeyword )
      {
        XsbError( TOO_MANY_WORDS, FirstWord, XsbCurrentLine );
      }
    }

    LayerCount = LayerCount + 1;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Xsb Read Parameters                    |
|                                                             |
\------------------------------------------------------------*/
 
void XsbReadParam()
 
{
  char *FirstWord;
  char *SecondWord;
  short Continue;
  long  Mask;

  autbegin();

  Mask = 0;

  while ( XsbGetLine( XsbBuffer ) )
  {
    FirstWord = XsbGetFirstWord( XsbBuffer, 0, 1);

    if ( FirstWord == DefineKeyword ) 
    {
      FirstWord = XsbGetNextWord( 1, 1 );

      if (! FirstWord )
      {
        XsbError( MISSING_NAME, DefineKeyword, XsbCurrentLine );
      }

      SecondWord = XsbGetNextWord( 1, 0 );

      if (! SecondWord ) 
      {
        XsbError( MISSING_VALUE, SecondWord, XsbCurrentLine );
      }

      if ( FirstWord == LowerGridStepKeyword )
      {
        XSCH_LOWER_GRID_STEP = XsbGetStringFloat( SecondWord );

        Mask |= XSB_LOWER_GRID_STEP_MASK;
      }
      else
      if ( FirstWord == CursorColorNameKeyword )
      {
        XSCH_CURSOR_COLOR_NAME = SecondWord;

        Mask |= XSB_CURSOR_COLOR_NAME_MASK;
      }
      else
      if ( FirstWord == BackgroundColorNameKeyword )
      {
        XSCH_BACKGROUND_COLOR_NAME = SecondWord;

        Mask |= XSB_BACKGROUND_COLOR_NAME_MASK;
      }
      else
      if ( FirstWord == ForegroundColorNameKeyword )
      {
        XSCH_FOREGROUND_COLOR_NAME = SecondWord;

        Mask |= XSB_FOREGROUND_COLOR_NAME_MASK;
      }
      else
      if ( FirstWord == AcceptColorNameKeyword )
      {
        XSCH_ACCEPT_COLOR_NAME = SecondWord;

        Mask |= XSB_ACCEPT_COLOR_NAME_MASK;
      }
      else
      if ( FirstWord == ConnectColorNameKeyword )
      {
        XSCH_CONNECT_COLOR_NAME = SecondWord;

        Mask |= XSB_CONNECT_COLOR_NAME_MASK;
      }
      else
      if ( FirstWord == CursorSizeKeyword )
      {
        XSCH_CURSOR_SIZE = XsbGetStringValue( SecondWord );

        Mask |= XSB_CURSOR_SIZE_MASK;
      }
      else
      if ( FirstWord == UnitKeyword )
      {
        XSCH_UNIT = XsbGetStringValue( SecondWord );

        Mask |= XSB_UNIT_MASK;
      }
      else
      {
        XsbError( UNKNOWN_DEFINE, FirstWord, XsbCurrentLine );
      }
    }
    else 
    if ( FirstWord == TableKeyword )
    {
      if (!( FirstWord = XsbGetNextWord(1, 1)))
      {
        XsbError( MISSING_NAME, TableKeyword, XsbCurrentLine );
      }

      if ( FirstWord == LayerNameKeyword )
      {
        XsbReadLayerName();

        Mask |= XSB_LAYER_NAME_MASK;
      }
      else
      if ( FirstWord == IconCellBufferKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_BUFFER, 1, 1 );
      }
      else
      if ( FirstWord == IconCellAndKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_AND, 1, 1 );
      }
      else
      if ( FirstWord == IconCellOrKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_OR, 1, 1 );
      }
      else
      if ( FirstWord == IconCellXorKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_XOR, 1, 1 );
      }
      else
      if ( FirstWord == IconCellTristateKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_TRISTATE, 1, 1 );
      }
      else
      if ( FirstWord == IconCellRegisterKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_REGISTER, 1, 1 );
      }
      else
      if ( FirstWord == IconCellConstantKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_CONSTANT, 0, 1 );
      }
      else
      if ( FirstWord == IconCellAndOrKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_ANDOR, 1, 1 );
      }
      else
      if ( FirstWord == IconCellOrAndKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_ORAND, 1, 1 );
      }
      else
      if ( FirstWord == IconCellMuxKeyword )
      {
        XsbReadIconCell( XSCH_ICON_CELL_MUX, 1, 1 );
      }
      else
      {
        XsbError( UNKNOWN_TABLE, FirstWord, XsbCurrentLine );
      }
    }
    else
    { 
      XsbError( SYNTAX_ERROR, FirstWord, XsbCurrentLine );
    } 
  }       

  if ( Mask != XSB_ALL_DEFINED_MASK )
  {
    XsbError( UNEXPECTED_EOF, (char *)NULL, XsbCurrentLine );
  }


  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Xsb Load Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void XschLoadParameters()

{
  short  Layer;

  autbegin();
  
  XSCH_PARAM_NAME = mbkgetenv( "XSCH_PARAM_NAME" );

  if ( XSCH_PARAM_NAME == (char *)NULL )
  {
    XSCH_PARAM_NAME = XschDefaultTechnoName;
  }

  if ( ! KeywordDefined )
  {
    DefineKeyword              = namealloc( DEFINE_KEYWORD                );
    TableKeyword               = namealloc( TABLE_KEYWORD                 );
    EndTableKeyword            = namealloc( END_TABLE_KEYWORD             );
    EndRecordKeyword           = namealloc( END_RECORD_KEYWORD            );
    LowerGridStepKeyword       = namealloc( LOWER_GRID_STEP_KEYWORD       );
    CursorColorNameKeyword     = namealloc( CURSOR_COLOR_NAME_KEYWORD     );
    BackgroundColorNameKeyword = namealloc( BACKGROUND_COLOR_NAME_KEYWORD );
    ForegroundColorNameKeyword = namealloc( FOREGROUND_COLOR_NAME_KEYWORD );
    AcceptColorNameKeyword     = namealloc( ACCEPT_COLOR_NAME_KEYWORD     );
    ConnectColorNameKeyword    = namealloc( CONNECT_COLOR_NAME_KEYWORD    );
    CursorSizeKeyword          = namealloc( CURSOR_SIZE_KEYWORD           );
    UnitKeyword                = namealloc( UNIT_KEYWORD                  );
    LayerNameKeyword           = namealloc( LAYER_NAME_KEYWORD            );
    IconCellBufferKeyword      = namealloc( ICON_CELL_BUFFER_KEYWORD      );
    IconCellAndKeyword         = namealloc( ICON_CELL_AND_KEYWORD         );
    IconCellOrKeyword          = namealloc( ICON_CELL_OR_KEYWORD          );
    IconCellXorKeyword         = namealloc( ICON_CELL_XOR_KEYWORD         );
    IconCellTristateKeyword    = namealloc( ICON_CELL_TRISTATE_KEYWORD    );
    IconCellRegisterKeyword    = namealloc( ICON_CELL_REGISTER_KEYWORD    );
    IconCellConstantKeyword    = namealloc( ICON_CELL_CONSTANT_KEYWORD    );
    IconCellOrAndKeyword       = namealloc( ICON_CELL_ORAND_KEYWORD       );
    IconCellAndOrKeyword       = namealloc( ICON_CELL_ANDOR_KEYWORD       );
    IconCellMuxKeyword         = namealloc( ICON_CELL_MUX_KEYWORD         );
 
    KeywordDefined = 1;
  }

  XSCH_LOWER_GRID_STEP = 0.0;

  XSCH_CURSOR_COLOR_NAME     = (char *)NULL;
  XSCH_BACKGROUND_COLOR_NAME = (char *)NULL;
  XSCH_FOREGROUND_COLOR_NAME = (char *)NULL;
  XSCH_ACCEPT_COLOR_NAME     = (char *)NULL;
  XSCH_CONNECT_COLOR_NAME    = (char *)NULL;

  XSCH_CURSOR_SIZE = 5;
  XSCH_UNIT        = 4;

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    XSCH_LAYER_NAME_TABLE[ Layer ][0] = XSCH_NOT_DEFINE;
    XSCH_LAYER_NAME_TABLE[ Layer ][1] = (char *)NULL;
    XSCH_LAYER_NAME_TABLE[ Layer ][2] = (char *)NULL;
    XSCH_LAYER_XFIG_COLOR[ Layer ] = 0;
  }

  XSCH_HASH_ICON = createauthtable( 100 );

  if ( !( XsbFile = fopen( XSCH_PARAM_NAME, "r")))
  {
    XsbError( OPEN_FILE, XSCH_PARAM_NAME, 0);
  }

  XsbReadParam();

  fclose( XsbFile );

  autend();
}
