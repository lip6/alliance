/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Abl                           |
|                                                             |
| File    :                   ablvhdl.c                       |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"

# include <stdio.h>
# include <ctype.h>
# include <stdlib.h>
# include <string.h>
# include "ablvhdl.h"
# include "ablerror.h"

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
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static authtable *AblVhdlHashTable  = (authtable *)0;
  static char       AblVhdlBuffer[ ABL_VHDL_BUFFER_SIZE ];

  static char *AblVhdlUsedName[] = 
  {
    "abs"         , "access"       ,
    "after"       , "alias"        ,
    "all"         , "and"          ,
    "architecture", "array"        ,
    "assert"      , "attribute"    ,
    "begin"       , "bit"          ,
    "bit_vector"  , "block"        ,
    "body"        , "buffer"       ,
    "bus"         , "case"         ,
    "component"   , "configuration",
    "constant"    , "disconnect"   ,
    "downto"      , "else"         ,
    "elsif"       , "end"          ,
    "entity"      , "error"        ,
    "exit"        , "file"         ,
    "for"         , "function"     ,
    "generate"    , "generic"      ,
    "guarded"     , "if"           ,
    "in"          , "inout"        ,
    "is"          , "label"        ,
    "library"     , "linkage"      ,
    "loop"        , "map"          ,
    "mod"         , "mux_bit"      ,
    "mux_vector"  , "nand"         ,
    "natural"     , "new"          ,
    "next"        , "nor"          ,
    "not"         , "null"         ,
    "of"          , "on"           ,
    "open"        , "or"           ,
    "others"      , "out"          ,
    "package"     , "port"         ,
    "procedure"   , "process"      ,
    "range"       , "record"       ,
    "reg_bit"     , "reg_vector"   ,
    "register"    , "rem"          ,
    "report"      , "return"       ,
    "select"      , "severity"     ,
    "signal"      , "stable"       ,
    "subtype"     , "then"         ,
    "to"          , "transport"    ,
    "type"        , "units"        ,
    "until"       , "use"          ,
    "variable"    , "wait"         ,
    "warning"     , "when"         ,
    "while"       , "with"         ,
    "wor_bit"     , "wor_vector"   ,
    "xor"         , (char *)0
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         getablname                          |
|                                                             |
\------------------------------------------------------------*/

char *vhdlablname( Name )

  char *Name;
{
  authelem *Element;
  char     *UsedName;
  char     *NewName;
  long      Scan;
  long      Target;
  char      Space;
  char      DefaultChar;

  if ( Name == (char *)0 )
  {
    return( (char *)0 );
  }

  Name = namealloc( Name );

  if ( AblVhdlHashTable == (authtable *)0 )
  {
    AblVhdlHashTable = createauthtable( ABL_VHDL_HASH_TABLE_SIZE );

    AblVhdlBuffer[ 0 ] = ABL_DEFAULT_CHAR; 
    AblVhdlBuffer[ 1 ] = '_'; 

    Scan = 0;

    while ( AblVhdlUsedName[ Scan ] != (char *)0 )
    {
      UsedName = namealloc( AblVhdlUsedName[ Scan ] );
      strcpy( AblVhdlBuffer + 2, UsedName );
      NewName = namealloc( AblVhdlBuffer );

      addauthelem( AblVhdlHashTable, UsedName, (long)NewName );

      Scan = Scan + 1;
    }
  }

  Element = searchauthelem( AblVhdlHashTable, Name );

  if ( Element != (authelem *)0 )
  {
    return( (char *)Element->VALUE );
  }

  Target = 0;
  Space  = 0;

  DefaultChar = ABL_DEFAULT_FIRST_CHAR;

  for ( Scan = 0; Name[ Scan ]; Scan++ )
  {
    if ( ( isalpha( Name[ Scan ] )     ) ||
         ( ( Name[ Scan     ] == '_' ) &&
           ( Scan             != 0   ) &&
           ( Name[ Scan - 1 ] != '_' ) ) )
    {
      AblVhdlBuffer[ Target++ ] = Name[ Scan ];
    }
    else
    if ( isdigit( Name[ Scan ] ) )
    {
      if ( ! Scan )
      {
        AblVhdlBuffer[ Target++ ] = DefaultChar;
      }

      AblVhdlBuffer[ Target++ ] = Name[ Scan ];
    }
    else
    if ( ( Name[ Scan ] == ' ' ) &&
         ( Space        == 0   ) )
    {
      Space = 1; AblVhdlBuffer[ Target++ ] = '(';
    }
    else
    {
      AblVhdlBuffer[ Target++ ] = DefaultChar;
    }

    DefaultChar = ABL_DEFAULT_CHAR;
  }

  if ( Target > 0 )
  {
    if ( AblVhdlBuffer[ Target - 1 ] == '_' )
    {
      AblVhdlBuffer[ Target++ ] = DefaultChar;
    }

    if ( Space )
    {
      AblVhdlBuffer[ Target++ ] = ')';
    }
  }
  else
  {
    AblVhdlBuffer[ Target++ ] = DefaultChar;
  }

  AblVhdlBuffer[ Target++ ] = '\0';
  NewName = namealloc( AblVhdlBuffer );

  addauthelem( AblVhdlHashTable, Name, (long)NewName );

  return( NewName );
}

/*------------------------------------------------------------\
|                                                             |
|                         vhdlablvector                       |
|                                                             |
\------------------------------------------------------------*/

char *vhdlablvector( Name, Index )

  char *Name;
  long *Index;
{
  char *Blank;

  if ( Name == (char *)0 )
  {
    return( (char *)0 );
  }

  strcpy( AblVhdlBuffer, Name );
  Blank = strchr( AblVhdlBuffer, ' ' );

  if ( Blank == (char *)0 )
  {
    return( (char *)0 );
  }

  *Blank = '\0';
  *Index = atoi( Blank + 1 );

  Name = namealloc( AblVhdlBuffer );

  return( Name );
}
