/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 *
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
 * You should have received a copy  of the GNU General Public License aint
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Tool    : SCmap and Boog and Loon - parameter file
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 * Modified: Francois Donnet 2000
 */




/****************************************************************************/
/*    Produit :  synthese logique (gestion du fichier de parametres)        */
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

#include <mut.h>
#include <aut.h>
#include <abl.h>
#include <mlo.h>
#include <abe.h>
#include "lon_lax_param.h"

#define MODE_FLAG       0x0001
#define LEVEL_FLAG      0x0002
#define SIGNAL_FLAG     0x0004
#define DELAY_FLAG      0x0008
#define EARLY_FLAG      0x0010
#define TRANSN_FLAG     0x0020
#define TRANSP_FLAG     0x0040
#define SLOPE_FLAG      0x0080
#define IMPED_FLAG      0x0100
#define CAPA_FLAG       0x0200
#define AUX_FLAG        0x0400
#define CAPI_FLAG       0x0800
#define BUFI_FLAG       0x1000

#define GET_INT( x )            {x = 0;                          \
                                while(isdigit( (int) c ))             \
                                {                               \
                                    x = 10 * x + c - '0';       \
                                    c = fgetc( Pfile );         \
                                }                               \
                                while( isspace( (int) c )){ c = getc( Pfile ); }}

#define NEXT_LINE               {while( (c = getc( Pfile )) != '\n' );}

#define NEXT_CHAR               {c = getc( Pfile );                       \
                                while( isspace( (int) c )){ c = getc( Pfile ); }}


  typedef struct lax_param_list
  {
    struct lax_param_list  *next;
    char                   *name;
    double                  value;

  } lax_param_list;

  typedef struct lax_name_list
  {
    struct lax_name_list   *next;
    char                   *name;

  } lax_name_list;

  typedef struct lax_data
  {
    int              mode;
    int              level;
    lax_name_list   *intermediate;
    lax_param_list  *delayPI;
    lax_name_list   *earlyPO;
    lax_param_list  *impedancePI;
    lax_param_list  *capaPO;
    lax_param_list  *capaPI;
    lax_param_list  *buffPI;

  } lax_data;


/*variable for .lax file*/
static lax_data *LAX = NULL;

/*-----------------------------------------------------*\
|*         Convert VHDL name to internal IO name       *|
\*-----------------------------------------------------*/
static char *
parseName( char * VHDL_name )
{
  char *p;
  char *Found;

  p = (char *) mbkalloc (strlen (VHDL_name) + 1);

  strcpy (p, VHDL_name);

  Found = strchr (p, '(');

  if (Found != NULL)
    {
      Found[0] = ' ';
      Found = strchr (p, ')');
      Found[0] = '\0';
    }

  return (p);
}

/*----------------------------------------------------------------------------
resetlax      : remet a vide la structure param
------------------------------------------------------------------------------
retour          : rien.
------------------------------------------------------------------------------*/
static void resetlax (lax_data *par)
{
  par->mode = DEFAULT_OPTIM;
  par->earlyPO = NULL;
  par->delayPI = NULL;
  par->impedancePI = NULL;
  par->capaPO = NULL;
  par->capaPI = NULL;
  par->intermediate = NULL;
  /*
  ** LUDO: Tu quoque fili mi !!
  */
  par->buffPI = NULL;
}

static lax_param_list *
loc_addparamlist( lax_param_list *head, char *name, double value )
{
  lax_param_list *LaxParam;

  LaxParam = (lax_param_list *)mbkalloc( sizeof( lax_param_list ) );
  LaxParam->next  = head;
  LaxParam->name  = name;
  LaxParam->value = value;
  
  return( LaxParam );
}

static lax_name_list *
loc_addnamelist( lax_name_list *head, char *name )
{
  lax_name_list *LaxName;

  LaxName = (lax_name_list *)mbkalloc( sizeof( lax_name_list ) );
  LaxName->next  = head;
  LaxName->name  = name;
  
  return( LaxName );
}

int 
loc_getdouble( FILE *Pfile, int c, double *PValue )
{
  char   Buffer[ 32 ];
  int    Index;

  *PValue = 0.0;
  Index   = 0;

  while ( ( isdigit( c ) ) || 
          ( c == '.'     ) || 
          ( c == '-'     ) || 
          ( c == '+'     ) ||
          ( c == 'e'     ) || 
          ( c == 'E'     ) )
  {
    if ( Index > 31 ) return( -1 );

    Buffer[ Index++ ] = c;
    c = fgetc( Pfile );
  }

  Buffer[ Index ] = '\0';

  if ( Index > 0 ) *PValue = atof( Buffer );

  while( isspace( (int) c )) { c = fgetc( Pfile ); } 
  
  return( c );
}

/*---------------------------------------------------------------------------
loadlax        :  parser of lax file parameters
-----------------------------------------------------------------------------
retour           :  lax structure
---------------------------------------------------------------------------*/
static lax_data *
loadlax( char * FileName )
{
  FILE *Pfile;
  char c;
  char Auxiliaire[ 1024 ];
  char *Name;
double DValue = 0.0;
  int flag = 0;
  int i, n;
  lax_data * loadparam;

  loadparam = (lax_data *) mbkalloc (sizeof (lax_data ));
  resetlax( loadparam );

  if ((Pfile = fopen (FileName, "rt")) != NULL)
  {
    while (!feof (Pfile))
    {
      NEXT_CHAR
      if (c == '#')
      {
        c = fgetc (Pfile);

        switch (c)
        {
          case 'M':   /* Mode */
            if (flag & MODE_FLAG)
            {
              fprintf (stderr,"Load param : Error \n---> Mode : Defined twice !\n");
              return NULL;
            }
            else
            {
              NEXT_CHAR
              if (c == '{')
              {
                NEXT_CHAR
                if (isdigit ((int)c))
                {
                  
                  GET_INT(loadparam->mode)
                  if (c == '}')
                  {
                    /* Check parameter */
                    if ((loadparam->mode) < 0 || (loadparam->mode) > 4)
                    {
                      fprintf (stderr,"Load param : Error \n---> Mode : Out of range (0 to 4).");
                      return NULL;
                    }
                    else
                    {
                      flag |= MODE_FLAG;
                    }
                  }
                  else
                  {
                    fprintf (stderr,"Load param : Error \n---> Mode : Can't find right brace");
                    return NULL;
                  }
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Mode : not an integer");
                  return NULL;
                }
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Mode : Can't find left brace.");
                return NULL;
              }
            }

          break;

      case 'L':   /* Level */
        if (flag & LEVEL_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> Level : Defined twice !\n");
            return NULL;
          }
        else
          {
            NEXT_CHAR
         if (c == '{')
         {
           NEXT_CHAR
             if (isdigit ((int)c))
             {
               GET_INT (loadparam->level)
            if (c == '}')
            {
              /* Check parameter */
              if ((loadparam->level) < 1 || (loadparam->level) > 5)
                {
                  fprintf (stderr,"Load param : Error \n---> Level : Out of range (1 to 5).");
                  return NULL;
                }
              else
                flag |= LEVEL_FLAG;
            }
               else
            {
              fprintf (stderr,"Load param : Error \n---> Level : Can't find right brace");
              return NULL;
            }
             }
           else
             {
               fprintf (stderr,"Load param : Error \n---> Level : not an integer");
               return NULL;
             }
         }
            else
         {
           fprintf (stderr,"Load param : Error \n---> Level : Can't find left brace.");
           return NULL;
         }
          }
        break;


          case 'S':   /* Intermediate signals */

            if (flag & SIGNAL_FLAG)
            {
              fprintf (stderr,"Load param : Error \n---> Intermediate signals : Defined twice !\n");
              return NULL;
            }
            else
            {
              NEXT_CHAR
              if (c == '{')
              {
                NEXT_CHAR
                n = 0;
                while (c != '}')
                {
                  i = 0;
                  while (!isspace ((int)c) && (c != ';'))
                  {
                    Auxiliaire[i++] = c;
                    c = getc (Pfile);
                  }

                  if (isspace ((int)c)) NEXT_CHAR
              
                  if (c == ';')
                  {
                    Auxiliaire[i] = '\0';
                    Name = parseName (Auxiliaire);
                    loadparam->intermediate = loc_addnamelist(loadparam->intermediate, namealloc (Name));
                    n++;
                    NEXT_CHAR
                  }
                  else
                  {
                    fprintf (stderr,"Load param : Error \n---> Intermediate signals : syntax error.");
                    return NULL;
                  }

                  if (feof (Pfile))
                  {
                    fprintf (stderr,"Load param : Error \n---> Intermediate signals : Unexpected end of file.");
                    return NULL;
                  }
                }
           
                if (n)
                {
                  loadparam->intermediate = (lax_name_list *) reverse ( (chain_list *)loadparam->intermediate);
                  flag |= SIGNAL_FLAG;
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Intermediate signals : Declaration empty");
                  return NULL;
                }
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Intermediate signals : Can't find left brace.");
                return NULL;
              }
            }

          break;

          case 'D':   /* Delayed Inputs */

            if (flag & DELAY_FLAG)
            {
              fprintf (stderr,"Load param : Error \n---> Delayed inputs : Defined twice !\n");
              return NULL;
            }
            else
            {
              NEXT_CHAR
              if (c == '{')
              {
                NEXT_CHAR
                n = 0;
                while (c != '}')
                {
                  i = 0;
                  while (!isspace ((int)c) && (c != ':'))
                  {
                    Auxiliaire[i++] = c;
                    c = getc (Pfile);
                  }

                  if (isspace ((int)c)) NEXT_CHAR

                  if (c == ':')
                  {
                    NEXT_CHAR
                    c = loc_getdouble( Pfile, c, &DValue );

                    if (c == ';')
                    {
                      Auxiliaire[i] = '\0';
                      Name = (char *) parseName (Auxiliaire);
                      loadparam->delayPI = loc_addparamlist(loadparam->delayPI, namealloc (Name), DValue);
                      n++;
                      NEXT_CHAR
                    }
                    else
                    {
                      fprintf (stderr,"Load param : Error \n---> Delayed inputs : syntax error.");
                      return NULL;
                    }
                  }
                  else
                  {
                    fprintf (stderr,"Load param : Error \n---> Delayed inputs : syntax error.");
                    return NULL;
                  }

                  if (feof (Pfile))
                  {
                    fprintf (stderr,"Load param : Error \n---> Delayed inputs : Unexpected end of file.");
                    return NULL;
                  }
                }

                if (n)
                {
                  loadparam->delayPI = (lax_param_list *) reverse ((chain_list*)loadparam->delayPI);
                  flag |= DELAY_FLAG;
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Delayed inputs : Declaration empty");
                  return NULL;
                }
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Delayed inputs : Can't find left brace.");
                return NULL;
              }
            }
        break;

        case 'E':   /* Early Outputs */

          if (flag & EARLY_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> Early outputs : Defined twice !\n");
            return NULL;
          }
          else
          {
            NEXT_CHAR
            if (c == '{')
            {
              NEXT_CHAR
              n = 0;
              while (c != '}')
              {
                i = 0;
                while (!isspace ((int)c) && (c != ';'))
                {
                  Auxiliaire[i++] = c;
                  c = getc (Pfile);
                }

                if (isspace ((int)c)) NEXT_CHAR

                if (c == ';')
                {
                  Auxiliaire[i] = '\0';
                  Name = (char *) parseName (Auxiliaire);
                  loadparam->earlyPO = loc_addnamelist(loadparam->earlyPO, namealloc (Name));
                  n++;
                  NEXT_CHAR
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Early outputs : syntax error.");
                  return NULL;
                }
                
                if (feof (Pfile))
                {
                  fprintf (stderr,"Load param : Error \n---> Early outputs : Unexpected end of file.");
                  return NULL;
                }
              }

              if (n)
              {
                loadparam->earlyPO = (lax_name_list *) reverse ( (chain_list *)loadparam->earlyPO);
                flag |= EARLY_FLAG;
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Early outputs : Declaration empty");
                return NULL;
              }
            }
            else
            {
              fprintf (stderr,"Load param : Error \n---> Early outputs : Can't find left brace.");
              return NULL;
            }
          }
        break;

        case 'I':   /* Input Impedances */

          if (flag & IMPED_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> Inputs impedances : Defined twice !\n");
            return NULL;
          }
          else
          {
            NEXT_CHAR
            if (c == '{')
            {
              NEXT_CHAR
              n = 0;
              while (c != '}')
              {
                i = 0;
                while (!isspace ((int)c) && (c != ':'))
                {
                  Auxiliaire[i++] = c;
                  c = getc (Pfile);
                }

                if (isspace ((int)c)) NEXT_CHAR

                if (c == ':')
                {
                  NEXT_CHAR

                  c = loc_getdouble( Pfile, c, &DValue );

                  if (c == ';')
                  {
                    Auxiliaire[i] = '\0';
                    Name = (char *) parseName (Auxiliaire);
                    loadparam->impedancePI = loc_addparamlist(loadparam->impedancePI, namealloc (Name), DValue);
                    n++;
                    NEXT_CHAR
                  }
                  else
                  {
                    fprintf (stderr,"Load param : Error \n---> Inputs impedances : syntax error.");
                    return NULL;
                  }
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Inputs impedances : syntax error.");
                  return NULL;
                }

                if (feof (Pfile))
                {
                  fprintf (stderr,"Load param : Error \n---> Inputs impedances : Unexpected end of file.");
                  return NULL;
                }
              }

              if (n)
              {
                loadparam->impedancePI = (lax_param_list *) reverse ((chain_list*)loadparam->impedancePI);
                flag |= IMPED_FLAG;
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Inputs impedances : Declaration empty");
                return NULL;
              }
            }
            else
            {
              fprintf (stderr,"Load param : Error \n---> Inputs impedances : Can't find left brace.");
              return NULL;
            }
          }
        break;

        case 'B':   /* Buffered Intput */

          if (flag & BUFI_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> Buffered Inputs : Defined twice !\n");
            return NULL;
          }
          else
          {
            NEXT_CHAR
            if (c == '{')
            {
              NEXT_CHAR
              n = 0;

              while (c != '}')
              {
                i = 0;
                while (!isspace ((int)c) && (c != ':'))
                {
                  Auxiliaire[i++] = c;
                  c = getc (Pfile);
                }

                if (isspace ((int)c)) NEXT_CHAR

                if (c == ':')
                {
                  NEXT_CHAR
                  c = loc_getdouble( Pfile, c, &DValue );

                  if (c == ';')
                  {
                    Auxiliaire[i] = '\0';
                    Name = (char *) parseName (Auxiliaire);
                    loadparam->buffPI = loc_addparamlist(loadparam->buffPI, namealloc (Name), DValue);
                    n++;
                    NEXT_CHAR
                  }
                  else
                  {
                    fprintf (stderr,"Load param : Error \n---> Buffered Inputs : syntax error.");
                    return NULL;
                  }
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Buffered Inputs : syntax error.");
                  return NULL;
                }

                if (feof (Pfile))
                {
                  fprintf (stderr,"Load param : Error \n---> Buffered Inputs : Unexpected end of file.");
                  return NULL;
                }
              }

              if (n)
              {
                loadparam->buffPI = (lax_param_list *) reverse ((chain_list*)loadparam->buffPI);
                flag |= BUFI_FLAG;
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Buffered Intputs : Declaration empty");
                return NULL;
              }
            }
            else
            {
             fprintf (stderr,"Load param : Error \n---> Buffered Inputs : Can't find left brace.");
             return NULL;
            }
          }
        break;

        case 'F':   /* Intput Capacitances */

          if (flag & CAPI_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> Inputs capacitances : Defined twice !\n");
            return NULL;
          }
          else
          {
            NEXT_CHAR
            if (c == '{')
            {
              NEXT_CHAR
              n = 0;
              while (c != '}')
              {
                i = 0;
                while (!isspace ((int)c) && (c != ':'))
                {
                  Auxiliaire[i++] = c;
                  c = getc (Pfile);
                }

                if (isspace ((int)c)) NEXT_CHAR

                if (c == ':')
                {
                  NEXT_CHAR
                  c = loc_getdouble( Pfile, c, &DValue );

                  if (c == ';')
                  {
                    Auxiliaire[i] = '\0';
                    Name = (char *) parseName (Auxiliaire);
                    loadparam->capaPI = loc_addparamlist(loadparam->capaPI, namealloc (Name), DValue );
                    n++;
                    NEXT_CHAR
                  }
                  else
                  {
                    fprintf (stderr,"Load param : Error \n---> Inputs capacitances : syntax error.");
                    return NULL;
                  }
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Inputs capacitances : syntax error.");
                  return NULL;
                }

                if (feof (Pfile))
                {
                  fprintf (stderr,"Load param : Error \n---> Inputs capacitances : Unexpected end of file.");
                  return NULL;
                }
              }

              if (n)
              {
                loadparam->capaPI = (lax_param_list *) reverse ((chain_list*)loadparam->capaPI);
                flag |= CAPI_FLAG;
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Intputs capacitances : Declaration empty");
                return NULL;
              }
            }
            else
            {
              fprintf (stderr,"Load param : Error \n---> Inputs capacitances : Can't find left brace.");
              return NULL;
            }
          }
        break;

        case 'C':   /* Output Capacitances */

          if (flag & CAPA_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> Outputs capacitances : Defined twice !\n");
            return NULL;
          }
          else
          {
            NEXT_CHAR
            if (c == '{')
            {
              NEXT_CHAR
              n = 0;
              while (c != '}')
              {
                i = 0;
                while (!isspace ((int)c) && (c != ':'))
                {
                  Auxiliaire[i++] = c;
                  c = getc (Pfile);
                }

                if (isspace ((int)c)) NEXT_CHAR

                if (c == ':')
                {
                  NEXT_CHAR

                  c = loc_getdouble( Pfile, c, &DValue );

                  if (c == ';')
                  {
                    Auxiliaire[i] = '\0';
                    Name = (char *) parseName (Auxiliaire);
                    loadparam->capaPO = loc_addparamlist(loadparam->capaPO, namealloc (Name), DValue );
                    n++;
                    NEXT_CHAR                      
                  }
                  else
                  {
                    fprintf (stderr,"Load param : Error \n---> Outputs capacitances : syntax error.");
                    return NULL;
                  }
                }
                else
                {
                  fprintf (stderr,"Load param : Error \n---> Outputs capacitances : syntax error.");
                  return NULL;
                }

                if (feof (Pfile))
                {
                  fprintf (stderr,"Load param : Error \n---> Outputs capacitances : Unexpected end of file.");
                  return NULL;
                }
              }
           
              if (n)
              {
                loadparam->capaPO = (lax_param_list *) reverse ((chain_list*)loadparam->capaPO);
                flag |= CAPA_FLAG;
              }
              else
              {
                fprintf (stderr,"Load param : Error \n---> Outputs capacitances : Declaration empty");
                return NULL;
              }
            }
            else
            {
              fprintf (stderr,"Load param : Error \n---> Outputs capacitances : Can't find left brace.");
              return NULL;
            }
          }
        break;

      case '#':   /* Commentaires */
        NEXT_LINE
          break;

      default:
        fprintf (stderr,"Load param : Error \n---> '%c' Declaration syntax error!\n", c);
        return NULL;
        break;
      }      /* switch ... */
       }
     else if (!feof (Pfile) && ((c >= '0') || (c < '5')))
       {
         fprintf (stderr,"Load param : Error \n---> old param version\n");
         return NULL;
       }
     else if (!feof (Pfile))
       {
         fprintf (stderr,"Load param : Error \n---> Declaration syntax error!\n");
         return NULL;
       }
    }
      if (flag == 0)
   {
     fprintf (stderr,"Load param : Error \n---> File Empty, any parameter defined!\n");
     return NULL;
   }

      fclose (Pfile);
/*
      fprintf (stdout,"Load param on '%s' done ...\n", FileName);
*/      
      return loadparam;
  }
  else
  {
    fprintf (stderr,"Load param : Error \n---> Can't open parameters file '%s'!\n", FileName);
    return NULL;
  }
}

/***************************************************************************/
/*  parse the .lax file and save the internal caracteristics               */
/***************************************************************************/
extern void 
parsefilelax(char *filename)
{
  char* name;
  int size;
  
  /*build the real filename*/
  size=strlen(filename);
  name=(char*) mbkalloc( size + 5 );
  name=strcpy(name,filename);
  strcpy( name + size, ".lax" );
  
  LAX = loadlax(name);
  if (!LAX) autexit(1);
}


/******************************************************************************/
/*  set default caracteristics if no file is given                              */
/******************************************************************************/
extern void 
defaultlax(int mode_optim)
{
  LAX = (lax_data *) mbkalloc (sizeof (lax_data));
  resetlax (LAX);
  LAX->mode=mode_optim;
}


/***************************************************************************/
/*    return the impedance(R in Ohm) of name for an input in lax file      */
/***************************************************************************/
extern double 
getimpedancelax(char *name)
{
   lax_param_list * Param;
   
   if (LAX) 
   {
     for ( Param =LAX->impedancePI;  Param ;  Param = Param->next) 
     {
       if ((char*) Param->name ==name)  return Param->value;
     }
   }
   return DEFAULT_IMPEDANCE;
}


/***************************************************************************/
/*   return the fanout(C in pF exp-12) of name for an output in lax file   */
/***************************************************************************/
extern double 
getcapacitancelax(char *name)
{
   lax_param_list * Param;
   
   if (LAX) 
   {
      for (Param=LAX->capaPO; Param; Param=Param->next) {
         /* fF exp-15   --->   pF  exp-12   */
         if ((char*)Param->name ==name)  return Param->value / 1000.0;
      }
   }
   return DEFAULT_CAPACITANCE;
}


/***************************************************************************/
/*      return the number of buffers to put for input in lax file          */
/***************************************************************************/
extern int 
getbufferlax(char *name)
{
   lax_param_list * Param;
   
   if (LAX) {
      for (Param=LAX->buffPI; Param; Param=Param->next) {
         if ((char*)Param->name ==name) return (int)Param->value;
      }
   }   
   return DEFAULT_BUFFER;
}


/***************************************************************************/
/*     return the delay (in ps exp-12) of name for an input in lax file    */
/***************************************************************************/
extern double 
getdelaylax(char *name)
{
   lax_param_list * Param;
   
   if (LAX) 
   {
      for (Param=LAX->delayPI; Param; Param=Param->next) {
         /*ns exp-9 -> ps  exp-12  */
         if ((char*)Param->name==name) return Param->value*1000.0;
      }
   }
   return DEFAULT_DELAY;
}


/***************************************************************************/
/*       return 1 if the internal signal shouldn't be erased               */
/***************************************************************************/
extern int 
signalsavelax(char *signame)
{
   lax_name_list *chain;
   
   if (LAX) 
   {
      for (chain=LAX->intermediate; chain; chain=chain->next) {
         if ( (char*)chain->name == signame ) return 1;
      }
   }
   return 0;
}


/***************************************************************************/
/*                return the optim param in lax file                       */
/* 0,1: optimize in area                                                   */
/* 3,4: optimize in delay                                                  */
/* 2: middle                                                               */
/***************************************************************************/
extern int 
getoptimlax()
{
   if (!LAX) return DEFAULT_OPTIM;        /*optimize in delay by default*/
   else return LAX->mode;  
}


/***************************************************************************/
/*          return 1 if lax matches with befig                             */
/* lokk if all referenced names are in befig                               */
/***************************************************************************/
extern int 
coherencelaxbefig(befig_list* befig)
{
   char* name;
   lax_param_list * Param;
   lax_name_list  * chain;
   bepor_list* bepor;
   beaux_list* beaux=NULL;
   bebux_list* bebux;
   bereg_list* bereg=NULL;
   int ret=1;
   
   if (!LAX) return 1;
   
   for (Param=LAX->delayPI; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (bepor=befig->BEPOR; bepor;bepor=bepor->NEXT) {
         if (name!=bepor->NAME) continue;
         if (bepor->DIRECTION!='I' && bepor->DIRECTION!='B' 
          && bepor->DIRECTION!='T') {
            fprintf(stderr,"LAX: '%s' is an output in file '%s.vbe'\n",
            name,befig->NAME);
            ret=0;
         }
         break;
      }
      if (!bepor) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,befig->NAME);
         ret=0;
      }
   }
   
   for (Param=LAX->impedancePI; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (bepor=befig->BEPOR; bepor;bepor=bepor->NEXT) {
         if (name!=bepor->NAME) continue;
         if (bepor->DIRECTION!='I' && bepor->DIRECTION!='B' 
          && bepor->DIRECTION!='T') {
            fprintf(stderr,"LAX: '%s' is an output in file '%s.vbe'\n",
            name,befig->NAME);
            ret=0;
         }
         break;
      }
      if (!bepor) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,befig->NAME);
         ret=0;
      }
   }
   
   for (Param=LAX->capaPO; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (bepor=befig->BEPOR; bepor;bepor=bepor->NEXT) {
         if (name!=bepor->NAME) continue;
         if (bepor->DIRECTION=='I' || bepor->DIRECTION=='B' 
          || bepor->DIRECTION=='T') {
            fprintf(stderr,"LAX: '%s' is an input in file '%s.vbe'\n",
            name,befig->NAME);
            ret=0;
         }
         break;
      }
      if (!bepor) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,befig->NAME);
         ret=0;
      }
   }
   
   for (Param=LAX->buffPI; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (bepor=befig->BEPOR; bepor;bepor=bepor->NEXT) {
         if (name!=bepor->NAME) continue;
         if (bepor->DIRECTION!='I' && bepor->DIRECTION!='B' 
          && bepor->DIRECTION!='T') {
            fprintf(stderr,"LAX: '%s' is an output in file '%s.vbe'\n",
            name,befig->NAME);
            ret=0;
         }
         break;
      }
      if (!bepor) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,befig->NAME);
         ret=0;
      }
   }
   
   for (chain=LAX->intermediate; chain; chain=chain->next) {
      name=(char*) chain->name;
      for (bebux=befig->BEBUX; bebux;bebux=bebux->NEXT) {
         if (name!=bebux->NAME) continue;
         break;
      }
      if (!bebux) for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
         if (name!=bereg->NAME) continue;
         break;
      }
      if (!bereg) for (beaux=befig->BEAUX; beaux;beaux=beaux->NEXT) {
         if (name!=beaux->NAME) continue;
         break;
      }
      if (!beaux) {
         fprintf(stderr,"LAX: '%s' isn't an internal signal in file '%s.vbe'\n",
         name,befig->NAME);
         ret=0;
      }
   }
   
   return ret;
}

/***************************************************************************/
/*          return 1 if lax matches with lofig                             */
/* look if all referenced names are in lofig                               */
/***************************************************************************/
extern int 
coherencelaxlofig(lofig_list* lofig)
{
   char* name;
   lax_param_list * Param;
   locon_list* locon;
   int ret=1;
   
   if (!LAX) return 1;
   
   for (Param=LAX->delayPI; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION!=IN && locon->DIRECTION!=INOUT
          && locon->DIRECTION!=TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an output in netlist file '%s'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in netlist file '%s'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   for (Param=LAX->impedancePI; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION!=IN && locon->DIRECTION!=INOUT
          && locon->DIRECTION!=TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an output in netlist file '%s'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in netlist file '%s'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   for (Param=LAX->capaPO; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION==IN || locon->DIRECTION==INOUT
          || locon->DIRECTION==TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an input in netlist file '%s'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in netlist file '%s'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   for (Param=LAX->buffPI; Param; Param=Param->next) {
      name=(char*) Param->name;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION!=IN && locon->DIRECTION!=INOUT
          && locon->DIRECTION!=TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an output in netlist file '%s'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in netlist file '%s'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   
   return ret;
}
