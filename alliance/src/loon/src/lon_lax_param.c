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
 * Tool    : SCmap and Loon - parameter file
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 * Modified: Francois Donnet 2000
 */




#ident "$Id: lon_lax_param.c,v 1.3 2004/07/31 02:23:09 ludo Exp $"

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



typedef struct lax
  {
    int mode;
    int level;
    chain_list *intermediate;
    ptype_list *delayPI;
    chain_list *earlyPO;
    int numTransN, numTransP;
    int maxSlopeTime;
    ptype_list *impedancePI;
    ptype_list *capaPO;
    ptype_list *capaPI;
    ptype_list *buffPI;
    int aux;
  }
lax;


/*variable for .lax file*/
static lax *LAX;





/*-----------------------------------------------------*\
|*         Convert VHDL name to internal IO name       *|
\*-----------------------------------------------------*/
static char* parseName (char* VHDL_name)
{
  char *p;
  char *Found;

  p = (char *) autallocblock (strlen (VHDL_name) + 1);

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

/*-----------------------------------------------------*\
|*         Convert internal IO name to VHDL name       *|
\*-----------------------------------------------------*/
static char* driveName (char* IO_name)
{
  char *p;
  char *Found;

/*
   fprintf(stdout,"init %s-\n", IO_name);
 */
  Found = (char *)autallocblock (strlen (IO_name) + 2);
  Found = strchr (IO_name, ' ');

  if (Found != NULL)
    {
/*
   fprintf(stdout,"init %s, Found =%s-\n", IO_name, Found);
 */
      p = autallocblock (strlen (IO_name) + 2);
      sprintf (p, "%s)", IO_name);
      Found = strchr (p, ' ');
      Found[0] = '(';
      return (p);
    }
  else
    return (IO_name);
}


/*----------------------------------------------------------------------------
resetlax      : remet a vide la structure param
------------------------------------------------------------------------------
retour          : rien.
------------------------------------------------------------------------------*/
static void resetlax (lax *par)
{
  par->mode = DEFAULT_OPTIM;
  par->level = 0;
  par->maxSlopeTime = 0;
  par->aux = 0;
  par->numTransP = par->numTransN = 0;
  par->earlyPO = NULL;
  par->delayPI = NULL;
  par->impedancePI = NULL;
  par->capaPO = NULL;
  par->capaPI = NULL;
  par->intermediate = NULL;
}


/*---------------------------------------------------------------------------
loadlax        :  parser of lax file parameters
-----------------------------------------------------------------------------
retour           :  lax structure
---------------------------------------------------------------------------*/
static lax* loadlax (char* FileName)
{
  FILE *Pfile;
  char c;
  char Auxiliaire[30];
  char *Name;
  int Value;
  int flag = 0;
  int i, n;
  lax* loadparam;

  loadparam = (lax*) autallocblock (sizeof (lax));
  resetlax (loadparam);

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
               GET_INT (loadparam->mode)
            if (c == '}')
            {
              /* Check parameter */
              if ((loadparam->mode) < 0 || (loadparam->mode) > 4)
                {
                  fprintf (stderr,"Load param : Error \n---> Mode : Out of range (0 to 4).");
                  return NULL;
                }
              else
                flag |= MODE_FLAG;
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

      case 'N':   /* Transistors N */
        if (flag & TRANSN_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> N Transistors : Defined twice !\n");
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
               GET_INT (loadparam->numTransN)
            if (c == '}')
            {
              /* Check parameter */
              if ((loadparam->numTransN) < 0 || (loadparam->numTransN) > 6)
                {
                  fprintf (stderr,"Load param : Error \n---> N Transistors : Out of range (0 to 6).");
                  return NULL;
                }
              else
                flag |= TRANSN_FLAG;
            }
               else
            {
              fprintf (stderr,"Load param : Error \n---> N Transistors : Can't find right brace");
              return NULL;
            }
             }
           else
             {
               fprintf (stderr,"Load param : Error \n---> N Transistors : not an integer");
               return NULL;
             }
         }
            else
         {
           fprintf (stderr,"Load param : Error \n---> N Transistors : Can't find left brace.");
           return NULL;
         }
          }
        break;

      case 'P':   /* Transistors P */
        if (flag & TRANSP_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> P Transistors : Defined twice !\n");
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
               GET_INT (loadparam->numTransP)
            if (c == '}')
            {
              /* Check parameter */
              if ((loadparam->numTransP) < 0 || (loadparam->numTransP) > 6)
                {
                  fprintf (stderr,"Load param : Error \n---> P Transistors : Out of range (0 to 6).");
                  return NULL;
                }
              else
                flag |= TRANSP_FLAG;
            }
               else
            {
              fprintf (stderr,"Load param : Error \n---> P Transistors : Can't find right brace");
              return NULL;
            }
             }
           else
             {
               fprintf (stderr,"Load param : Error \n---> P Transistors : not an integer");
               return NULL;
             }
         }
            else
         {
           fprintf (stderr,"Load param : Error \n---> P Transistors : Can't find left brace.");
           return NULL;
         }
          }
        break;

      case 'T':   /* Max Slope Time */
        if (flag & SLOPE_FLAG)
          {
            fprintf (stderr,"Load param : Error \n---> Slope Time : Defined twice !\n");
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
               GET_INT (loadparam->maxSlopeTime)
            if (c == '}')
            {
              /* Check parameter */
              if ((loadparam->maxSlopeTime) < 0 || (loadparam->maxSlopeTime) > 10000)
                {
                  fprintf (stderr,"Load param : Error \n---> Slope Time : Out of range (0 to 10000).");
                  return NULL;
                }
              else
                flag |= SLOPE_FLAG;
            }
               else
            {
              fprintf (stderr,"Load param : Error \n---> Slope Time : Can't find right brace");
              return NULL;
            }
             }
           else
             {
               fprintf (stderr,"Load param : Error \n---> Slope Time : not an integer");
               return NULL;
             }
         }
            else
         {
           fprintf (stderr,"Load param : Error \n---> Slope Time : Can't find left brace.");
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
              if (i > 29)
                {
                  fprintf (stderr,"Load param : Error \n---> Intermediate signals : Name too int.");
                  return NULL;
                }
              c = getc (Pfile);
            }
               if (isspace ((int)c))
            NEXT_CHAR
              if (c == ';')
              {
                Auxiliaire[i] = '\0';
                Name = parseName (Auxiliaire);
                loadparam->intermediate = addchain (loadparam->intermediate, namealloc (Name));
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
               loadparam->intermediate = (chain_list *) reverse (loadparam->intermediate);
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
              if (i > 29)
                {
                  fprintf (stderr,"Load param : Error \n---> Delayed inputs : Name too int.");
                  return NULL;
                }
              c = getc (Pfile);
            }
               if (isspace ((int)c))
            NEXT_CHAR
              if (c == ':')
              {
                NEXT_CHAR
                  if (isdigit ((int)c))
                  {
               GET_INT (Value)
                 if (c == ';')
                 {
                   Auxiliaire[i] = '\0';
                   Name = (char *) parseName (Auxiliaire);
                   loadparam->delayPI = addptype (loadparam->delayPI, Value, namealloc (Name));
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
               fprintf (stderr,"Load param : Error \n---> Delayed inputs : not an integer!\n");
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
               loadparam->delayPI = (ptype_list *) reverse ((chain_list*)loadparam->delayPI);
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
              if (i > 29)
                {
                  fprintf (stderr,"Load param : Error \n---> Early outputs : Name too int.");
                  return NULL;
                }
              c = getc (Pfile);
            }
               if (isspace ((int)c))
            NEXT_CHAR
              if (c == ';')
              {
                Auxiliaire[i] = '\0';
                Name = (char *) parseName (Auxiliaire);
                loadparam->earlyPO = addchain (loadparam->earlyPO, namealloc (Name));
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
               loadparam->earlyPO = (chain_list *) reverse (loadparam->earlyPO);
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
              if (i > 29)
                {
                  fprintf (stderr,"Load param : Error \n---> Inputs impedances : Name too int.");
                  return NULL;
                }
              c = getc (Pfile);
            }
               if (isspace ((int)c))
            NEXT_CHAR
              if (c == ':')
              {
                NEXT_CHAR
                  if (isdigit ((int)c))
                  {
               GET_INT (Value)
                 if (c == ';')
                 {
                   Auxiliaire[i] = '\0';
                   Name = (char *) parseName (Auxiliaire);
                   loadparam->impedancePI = addptype (loadparam->impedancePI, Value, namealloc (Name));
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
               fprintf (stderr,"Load param : Error \n---> Inputs impedances : not an integer!\n");
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
               loadparam->impedancePI = (ptype_list *) reverse ((chain_list*)loadparam->impedancePI);
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
              if (i > 29)
                {
                  fprintf (stderr,"Load param : Error \n---> Inputs capacitances : Name too int.");
                  return NULL;
                }
              c = getc (Pfile);
            }
               if (isspace ((int)c))
            NEXT_CHAR
              if (c == ':')
              {
                NEXT_CHAR
                  if (isdigit ((int)c))
                  {
               GET_INT (Value)
                 if (c == ';')
                 {
                   Auxiliaire[i] = '\0';
                   Name = (char *) parseName (Auxiliaire);
                   loadparam->buffPI = addptype (loadparam->buffPI, Value, namealloc (Name));
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
               fprintf (stderr,"Load param : Error \n---> Buffered Inputs : not an integer!\n");
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
               loadparam->buffPI = (ptype_list *) reverse ((chain_list*)loadparam->buffPI);
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
              if (i > 29)
                {
                  fprintf (stderr,"Load param : Error \n---> Inputs capacitances : Name too int.");
                  return NULL;
                }
              c = getc (Pfile);
            }
               if (isspace ((int)c))
            NEXT_CHAR
              if (c == ':')
              {
                NEXT_CHAR
                  if (isdigit ((int)c))
                  {
               GET_INT (Value)
                 if (c == ';')
                 {
                   Auxiliaire[i] = '\0';
                   Name = (char *) parseName (Auxiliaire);
                   loadparam->capaPI = addptype (loadparam->capaPI, Value, namealloc (Name));
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
               fprintf (stderr,"Load param : Error \n---> Inputs capacitances : not an integer!\n");
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
               loadparam->capaPI = (ptype_list *) reverse ((chain_list*)loadparam->capaPI);
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
              if (i > 29)
                {
                  fprintf (stderr,"Load param : Error \n---> Outputs capacitances : Name too int.");
                  return NULL;
                }
              c = getc (Pfile);
            }
               if (isspace ((int)c))
            NEXT_CHAR
              if (c == ':')
              {
                NEXT_CHAR
                  if (isdigit ((int)c))
                  {
               GET_INT (Value)
                 if (c == ';')
                 {
                   Auxiliaire[i] = '\0';
                   Name = (char *) parseName (Auxiliaire);
                   loadparam->capaPO = addptype (loadparam->capaPO, Value, namealloc (Name));
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
               fprintf (stderr,"Load param : Error \n---> Outputs capacitances : not an integer!\n");
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
               loadparam->capaPO = (ptype_list *) reverse ((chain_list*)loadparam->capaPO);
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

/*----------------------------------------------------------------------------
savelax       : enregistre la structure de donnees dans le fichier
------------------------------------------------------------------------------
retour          : aucun
------------------------------------------------------------------------------*/
static void savelax (lax* saveparam, char* FileName)
{
  FILE *Pfile;
  chain_list *intermediate;
  chain_list *earlyPO;
  ptype_list *delayPI;
  ptype_list *impedancePI;
  ptype_list *capaPO;
  ptype_list *capaPI;
  ptype_list *buffPI;

  Pfile = fopen (FileName, "wt");

  if (Pfile)
    {
      fprintf (Pfile, "## Mode\n");
      fprintf (Pfile, "#M{%d}\n", saveparam->mode);
      fprintf (Pfile, "## Level\n");
      fprintf (Pfile, "#L{%d}\n", saveparam->level);
      fprintf (Pfile, "## Number Transistor N\n");
      fprintf (Pfile, "#N{%d}\n", saveparam->numTransN);
      fprintf (Pfile, "## Number Transistor P\n");
      fprintf (Pfile, "#P{%d}\n", saveparam->numTransP);
      fprintf (Pfile, "## Fanout factor\n");
      fprintf (Pfile, "#T{%d}\n", saveparam->maxSlopeTime);

      if (saveparam->intermediate)
   {
     intermediate = saveparam->intermediate;
     fprintf (Pfile, "##Auxiliary signals saved\n");
     fprintf (Pfile, "#S{\n");
     while (intermediate)
       {
         fprintf (Pfile, "%s;\n", driveName ((char *) intermediate->DATA));
         intermediate = intermediate->NEXT;
       }
     fprintf (Pfile, "}\n");
   }

      if (saveparam->delayPI)
   {
     delayPI = saveparam->delayPI;
     fprintf (Pfile, "##Delayed inputs\n");
     fprintf (Pfile, "#D{\n");
     while (delayPI)
       {
         fprintf (Pfile, "%s:%ld;\n", driveName (delayPI->DATA), delayPI->TYPE);
         delayPI = delayPI->NEXT;
       }
     fprintf (Pfile, "}\n");
   }

      if (saveparam->earlyPO)
   {
     earlyPO = saveparam->earlyPO;
     fprintf (Pfile, "##Early outputs\n");
     fprintf (Pfile, "#E{\n");
     while (earlyPO)
       {
         fprintf (Pfile, "%s;\n", driveName (earlyPO->DATA));
         earlyPO = earlyPO->NEXT;
       }
     fprintf (Pfile, "}\n");
   }

      if (saveparam->impedancePI)
   {
     impedancePI = saveparam->impedancePI;
     fprintf (Pfile, "##Inputs Impedence\n");
     fprintf (Pfile, "#I{\n");
     while (impedancePI)
       {
         fprintf (Pfile, "%s:%ld;\n", driveName (impedancePI->DATA), impedancePI->TYPE);
         impedancePI = impedancePI->NEXT;
       }
     fprintf (Pfile, "}\n");
   }

      if (saveparam->capaPO)
   {
     capaPO = saveparam->capaPO;
     fprintf (Pfile, "##Outputs capacitance\n");
     fprintf (Pfile, "#C{\n");
     while (capaPO)
       {
         fprintf (Pfile, "%s:%ld;\n", driveName (capaPO->DATA), capaPO->TYPE);
         capaPO = capaPO->NEXT;
       }
     fprintf (Pfile, "}\n");
   }

      if (saveparam->capaPI)
   {
     capaPI = saveparam->capaPI;
     fprintf (Pfile, "##Inputs maximal fanout\n");
     fprintf (Pfile, "#F{\n");
     while (capaPI)
       {
         fprintf (Pfile, "%s:%ld;\n", driveName (capaPI->DATA), capaPI->TYPE);
         capaPI = capaPI->NEXT;
       }
     fprintf (Pfile, "}\n");
   }

      if (saveparam->buffPI)
   {
     buffPI = saveparam->buffPI;
     fprintf (Pfile, "##Buffered Inputs\n");
     fprintf (Pfile, "#B{\n");
     while (buffPI)
       {
         fprintf (Pfile, "%s:%ld;\n", driveName (buffPI->DATA), buffPI->TYPE);
         buffPI = buffPI->NEXT;
       }
     fprintf (Pfile, "}\n");
   }

      fclose (Pfile);
    }
  else
    {
      fprintf (stderr,
      "\n\nERROR Save Parameters : Unable to write in %s\n", FileName);
      exit (1);
    }
}


/***************************************************************************/
/*  parse the .lax file and save the internal caracteristics               */
/***************************************************************************/
extern void parsefilelax(char *filename)
{
   char* name;
   int size;
   
   /*build the real filename*/
   size=strlen(filename);
   name=(char*) autallocblock(size+strlen(".lax")+1);
   name=strcpy(name,filename);
   name[size]='.'; name[size+1]='l'; name[size+2]='a'; name[size+3]='x'; 
   name[size+4]='\0';   
   
   LAX=loadlax(name);
   if (!LAX) exit(1);
}


/******************************************************************************/
/*  set default caracteristics if no file is given                              */
/******************************************************************************/
extern void defaultlax(int mode_optim)
{
  LAX = (lax*) autallocblock (sizeof (lax));
  resetlax (LAX);
  LAX->mode=mode_optim;
}


/***************************************************************************/
/* save the internal caracteristics  in file                               */
/***************************************************************************/
extern void savefilelax(char *filename)
{
   savelax(LAX,filename);
}   


/***************************************************************************/
/*    return the impedance(R in Ohm) of name for an input in lax file      */
/***************************************************************************/
extern double getimpedancelax(char *name)
{
   ptype_list* ptype;
   
   if (LAX) {
      for (ptype=LAX->impedancePI; ptype; ptype=ptype->NEXT) {
         if ((char*)ptype->DATA==name)  return ptype->TYPE;
      }
   }   
   return DEFAULT_IMPEDANCE;
}


/***************************************************************************/
/*   return the fanout(C in pF exp-12) of name for an output in lax file   */
/***************************************************************************/
extern double getcapacitancelax(char *name)
{
   ptype_list* ptype;
   
   if (LAX) {
      for (ptype=LAX->capaPO; ptype; ptype=ptype->NEXT) {
         /* fF exp-15   --->   pF  exp-12   */
         if ((char*)ptype->DATA==name)  return (double)ptype->TYPE/1000;
      }
   }
   return DEFAULT_CAPACITANCE;
}


/***************************************************************************/
/*      return the number of buffers to put for input in lax file          */
/***************************************************************************/
extern int getbufferlax(char *name)
{
   ptype_list* ptype;
   
   if (LAX) {
      for (ptype=LAX->buffPI; ptype; ptype=ptype->NEXT) {
         if ((char*)ptype->DATA==name) return ptype->TYPE;
      }
   }   
   return DEFAULT_BUFFER;
}


/***************************************************************************/
/*     return the delay (in ps exp-12) of name for an input in lax file    */
/***************************************************************************/
extern double getdelaylax(char *name)
{
   ptype_list* ptype;
   
   if (LAX) {
      for (ptype=LAX->delayPI; ptype; ptype=ptype->NEXT) {
         /*ns exp-9 -> ps  exp-12  */
         if ((char*)ptype->DATA==name) return ptype->TYPE*1000;
      }
   }
   return DEFAULT_DELAY;
}


/***************************************************************************/
/*       return 1 if the internal signal shouldn't be erased               */
/***************************************************************************/
extern int signalsavelax(char *signame)
{
   chain_list* chain;
   
   if (LAX) {
      for (chain=LAX->intermediate; chain; chain=chain->NEXT) {
         if ((char*)chain->DATA==signame) return 1;
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
extern int getoptimlax()
{
   if (!LAX) return DEFAULT_OPTIM;        /*optimize in delay by default*/
   else return LAX->mode;  
}


/***************************************************************************/
/*          return 1 if lax matches with lofig                             */
/* look if all referenced names are in lofig                               */
/***************************************************************************/
extern int coherencelax(lofig_list* lofig)
{
   char* name;
   ptype_list* ptype;
   locon_list* locon;
   int ret=1;
   
   if (!LAX) return 1;
   
   for (ptype=LAX->delayPI; ptype; ptype=ptype->NEXT) {
      name=(char*) ptype->DATA;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION!=IN && locon->DIRECTION!=INOUT
          && locon->DIRECTION!=TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an output in file '%s.vbe'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   for (ptype=LAX->impedancePI; ptype; ptype=ptype->NEXT) {
      name=(char*) ptype->DATA;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION!=IN && locon->DIRECTION!=INOUT
          && locon->DIRECTION!=TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an output in file '%s.vbe'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   for (ptype=LAX->capaPO; ptype; ptype=ptype->NEXT) {
      name=(char*) ptype->DATA;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION==IN || locon->DIRECTION==INOUT
          || locon->DIRECTION==TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an input in file '%s.vbe'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   for (ptype=LAX->buffPI; ptype; ptype=ptype->NEXT) {
      name=(char*) ptype->DATA;
      for (locon=lofig->LOCON; locon;locon=locon->NEXT) {
         if (name!=locon->NAME) continue;
         if (locon->DIRECTION!=IN && locon->DIRECTION!=INOUT
          && locon->DIRECTION!=TRANSCV) {
            fprintf(stderr,"LAX: '%s' is an output in file '%s.vbe'\n",
            name,lofig->NAME);
            ret=0;
         }
         break;
      }
      if (!locon) {
         fprintf(stderr,"LAX: '%s' doesn't exist in file '%s.vbe'\n",
         name,lofig->NAME);
         ret=0;
      }
   }
   
   
   return ret;
}

