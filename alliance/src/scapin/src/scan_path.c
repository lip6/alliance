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
| Tool    :                     SCAN                          |
|                                                             |
| File    :                  scan_path.c                      |
|                                                             |
| Author  :                                                   |
|                                                             |
| Date    :                                                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "mlu.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "scan_path.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          ScanParseGetWord                   |
|                                                             |
\------------------------------------------------------------*/

static char *ScanParseGetWord( Buffer )

  char *Buffer;
{
  char *String;

  if ( ( String = (char *)strtok( Buffer, " =,\t\n" ) )  != (char *)0 )
  {
    String = namealloc( String );
  }

  return( String );
}


/*------------------------------------------------------------\
|                                                             |
|                         ScanParseViewParam                  |
|                                                             |
\------------------------------------------------------------*/

static void ScanParseViewParam( Param )

   pathparam *Param;
{
  chain_list *ScanList;

  fprintf( stdout, "BEGIN_PATH_REG\n" );

  for ( ScanList  = Param->REG_LIST;
        ScanList != (chain_list *)0;
        ScanList  = ScanList->NEXT )
  {
    fprintf( stdout, "%s\n", (char *)ScanList->DATA );
  }

  fprintf( stdout, "END_PATH_REG\n" );

  fprintf( stdout, "BEGIN_CONNECTOR\n" );

  fprintf( stdout, "SCAN_IN   : %s\n", Param->SCAN_IN   );
  fprintf( stdout, "SCAN_OUT  : %s\n", Param->SCAN_OUT  );
  fprintf( stdout, "SCAN_TEST : %s\n", Param->SCAN_TEST );

  fprintf( stdout, "END_CONNECTOR\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                       ScanParsePathParamFile                |
|                                                             |
\------------------------------------------------------------*/

pathparam *ScanParsePathParamFile( FileName, FlagDebug )

  char *FileName;
  int   FlagDebug; 
{
  pathparam      *PathParam;
  char           *FirstWord;
  char           *SecondWord;
  int             CurrentDef;
  long            LineNumber;
  FILE           *ParamFile;
  char            ParamBuffer[ 512 ];
  char           *RegisterName; 
  char           *Pointer;
  char           *Scan;
  int             Index;
  int             Left;
  int             Right;
  int             Step;
  char            Buffer[ 128 ];
  
  FileName  = autbasename( FileName, "path" );
  sprintf( ParamBuffer, "%s.path", FileName );
  ParamFile = fopen( ParamBuffer, "r" );

/* -----------------------------------------------------------------------*/
/* Dans le cas ou on ne peut pas ouvrir le fichier                        */
/*------------------------------------------------------------------------*/                                             
  if ( ParamFile == (FILE *)0 )
  {
    fprintf( stderr, "\t### Unable to parse parameter file %s.path\n", FileName );
    autexit( 1 );
  }


  PathParam  = (pathparam *)autallocblock( sizeof( pathparam ) );
  LineNumber = 0;
  SecondWord = (char *)0;

  CurrentDef = SCAN_PATH_NONE;
     
/* -----------------------------------------------------------------------*/
/* Ouverture et lecture du fichier .path                                  */
/*------------------------------------------------------------------------*/                                             

  while ( fgets( ParamBuffer, 512, ParamFile ) != (char *)0 )
  {
    LineNumber++;

    if ( ParamBuffer[ 0 ] == '#' ) continue;

    FirstWord = ScanParseGetWord( ParamBuffer ); 

    if ( FirstWord == (char *)0 ) continue;

    if ( ! strcmp( FirstWord, "end_path_reg" ) ) CurrentDef = SCAN_PATH_NONE;
    else
    if( ! strcmp( FirstWord, "end_connector" ) ) CurrentDef = SCAN_PATH_NONE;
    else
    if ( CurrentDef != SCAN_PATH_NONE )
    {
      if ( CurrentDef != SCAN_PATH_REG )
      {
        SecondWord = ScanParseGetWord( (char *)0 );

        if ( SecondWord == (char *)0 )
        {
          fprintf( stderr, "\t### Syntax error line %ld in parameter file %s.path\n",
                   LineNumber, FileName  );
          autexit( 1 );
        }
      }

      switch ( CurrentDef )
  
  /* -----------------------------------------------------------------------*/
  /* Partie consacrée à la saisie des noms de registres dans les champs     */
  /* correspondants                                                         */
  /*------------------------------------------------------------------------*/ 
      {
        case SCAN_PATH_REG :                          
                             
          do
          {    
            Scan = strchr( FirstWord, '[' );

            if ( Scan != NULL )
            {
              strcpy( Buffer, FirstWord );
              Scan = Buffer;

              Pointer  = strchr( Scan, '[' );
              *Pointer = '\0';

              RegisterName = namealloc( Buffer );
              Scan     = Pointer + 1;

              Pointer = strchr( Scan, ':' );

              if ( Pointer == (char *)0 )
              {
                fprintf( stderr, "\t### Syntax error line %ld in parameter file %s.path\n",
                         LineNumber, FileName  );
                autexit(1);
              }

              *Pointer = '\0';
              Left = atoi( Scan );
              Scan = Pointer + 1;

              Pointer = strchr( Scan, ']' );

              if ( Pointer == (char *)0 )
              {
                fprintf( stderr, "\t### Syntax error line %ld in parameter file %s.path\n",
                         LineNumber, FileName  );
                autexit(1);
              }

              *Pointer = '\0';
              Right = atoi( Scan );

              if ( Left < Right ) Step =  1;
              else                Step = -1;
                 
              for ( Index = Left; Index != (Right + Step) ; Index += Step )
              {
                sprintf( Buffer, "%s%d", RegisterName, Index );
                PathParam->REG_LIST = addchain( PathParam->REG_LIST, namealloc( Buffer ) );
              }
            }
            else
            {
              PathParam->REG_LIST = addchain( PathParam->REG_LIST, FirstWord );
            }

          } while ( ( FirstWord = ScanParseGetWord( NULL ) ) != NULL );
      
        break;
  
  /* --------------------------------------- -------------------------------*/ 

  /* Partie consacrée à la lecture des noms des connecteurs dans les champs */
  /* correspondants                                                         */
  /*------------------------------------------------------------------------*/                       
        case SCAN_PATH_LOCON :
        
          if ( ! strcmp( FirstWord, "scan_in" ) )
          {
            PathParam->SCAN_IN = SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "scan_out" ) )
          {
            PathParam->SCAN_OUT = SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "scan_test" ) )
          {
            PathParam->SCAN_TEST = SecondWord;
          }
          else
          {
            fprintf( stderr, "\t### Syntax error line %ld in parameter file %s.path\n",
                     LineNumber, FileName  );
            autexit( 1 );
          }
  
          break;
      }
    }
    else
      
/* ---------------------------------------------------------------------*/
/* Partie consacrée à l'allocation de mémoire pour les registres        */
/* ---------------------------------------------------------------------*/

    if ( ! strcmp( FirstWord, "begin_path_reg"))
    {
      CurrentDef = SCAN_PATH_REG;
    }
    else
      
/* ----------------------------------------------------------------------*/
/* Partie consacrée à l'allocation de mémoire pour les connecteurs       */
/* ----------------------------------------------------------------------*/
       
    if ( ! strcmp( FirstWord, "begin_connector"))
    {
      CurrentDef = SCAN_PATH_LOCON;
    }
  }

/* ----------------------------------------------------------------------*/
/* Partie consacrée à la fermeture du fichier chemin.path                */
/* ----------------------------------------------------------------------*/                        

  PathParam->REG_LIST = reverse( PathParam->REG_LIST );

  fclose( ParamFile  );

  if ( FlagDebug )
  {
    ScanParseViewParam( PathParam );
  }

  return( PathParam );
}
