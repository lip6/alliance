/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: pat_drvpat.c						*/
/* date		: Mar 10 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: pat format driver from pat structures			*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "mut.h"
#include "pat.h"

/* ###--------------------------------------------------------------### */
/* function	: dclini						*/
/* description	: scan the initialization list, gather all elements of	*/
/*		  an array of register and print a line for each	*/
/*		  register						*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void dclini (fp, ptpat)

FILE         *fp;
struct papat *ptpat;

  {
  struct paini *ptini          = ptpat->PAINI;
  char          buffer  [1024] ;	/* register's value		*/
  char          name    [ 256] ;	/* current register's root name	*/
  long           indx           ;	/* current register's index	*/
  char          oldname [ 256] ;	/* previous root name		*/
  char          direction      = '?';	/* array's direction		*/
  long           redvar         ;	/* = 2 if (name + index)	*/
  long           oldred         ;	/* previous value of redvar	*/
  long           left           ;	/* the left  bound of an array	*/
  long           right          ;	/* the right bound of an array	*/
  long           i              = 0  ;

	/* ###------------------------------------------------------### */
	/*    split the register's name into a root name and an index.	*/
	/* ###------------------------------------------------------### */

  fprintf (fp,"\n-- modification of registers\n");
  oldred = sscanf (ptini->NAME, "%255s %22ld", oldname, &left);

  strcpy (name, oldname);
  redvar    = oldred;
  right     = left;
  buffer[i] = ptini->VALUE;

  i++;
  ptini = ptini->NEXT;

  while (ptini != NULL)
    {
	/* ###------------------------------------------------------### */
	/*    scan the PAINI list ...					*/
	/* split the register's name into a root name and an index.	*/
	/* ###------------------------------------------------------### */

    redvar = sscanf (ptini->NAME, "%255s %22ld", name, &indx);

    if ( (!strcmp (name, oldname)) && (oldred == 2) && (redvar == 2) &&
         ( (((direction == '?') || (direction == 'U')) && (indx == right+1)) ||
           (((direction == '?') || (direction == 'D')) && (indx == right-1)) ))
      {
	/* ###------------------------------------------------------### */
	/*    If the root name is the same as the previous root name	*/
	/* and indexes are continus (continuing inside an array		*/
	/* register) :							*/
	/*   consider the current index as the right bound of the array	*/
	/*   continue inspecting the next PAINI.			*/
	/* ###------------------------------------------------------### */

      if (direction == '?')
        {
        if (indx == (right + 1))
          direction = 'U';
        else
          direction = 'D';
        }

      right = indx;
      }

    else
      {
	/* ###------------------------------------------------------### */
	/*    Otherwies, a new register has been matched. Print the	*/
	/* initialization statement relative to the previous register:	*/
	/*								*/
	/*    - if the previous register was a scalar (root), produce a	*/
	/*    one bit initialization statement.				*/
	/*								*/
	/*    - if the previous register was an array (root + index),	*/
	/*    compare print a line depending on its direction.		*/
	/* ###------------------------------------------------------### */

      buffer[i] = '\0';
      if (oldred == 1)
        fprintf (fp, "%s <= \'%s\';\n", oldname, buffer);
      else
        {
        switch (direction)
          {
          case 'U' :
            fprintf (fp, "%s (%ld to %ld) <= B\"%s\";\n", oldname, left,
                                                        right  , buffer);
            break;
          case 'D' :
            fprintf (fp, "%s (%ld downto %ld) <= B\"%s\";\n", oldname, left,
                                                            right  , buffer);
            break;
          case '?' :
            fprintf (fp, "%s (%ld) <= \'%s\';\n", oldname, left, buffer);
            break;
          }
        }

      direction = '?';
      i         = 0;
      left      = indx;
      right     = indx;
      oldred    = redvar;
      strcpy (oldname, name);
      }

    buffer[i] = ptini->VALUE;
    i++;
    ptini = ptini->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    make the same actions when the end of the list has been	*/
	/* reached							*/
	/* ###------------------------------------------------------### */

  buffer[i] = '\0';
  if (redvar == 1)
    fprintf (fp, "%s <= \'%s\';\n", name, buffer);
  else
    {
    switch (direction)
      {
      case 'U' :
        fprintf (fp, "%s (%ld to %ld) <= B\"%s\";\n", oldname, left,
                                                    right  , buffer);
        break;
      case 'D' :
        fprintf (fp, "%s (%ld downto %ld) <= B\"%s\";\n", oldname, left,
                                                        right  , buffer);
        break;
      case '?' :
        fprintf (fp, "%s (%ld) <= \'%s\';\n", oldname, left, buffer);
        break;
      }
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: pat_drvpat						*/
/* dexcription	: produce a text file corresponding to a sequence of	*/
/*		  patterns						*/
/* called func.	: dclini, mbkalloc, mbkfree, pat_error			*/
/* ###--------------------------------------------------------------### */

long pat_drvpat (fp, ptseq, labelsiz)

FILE         *fp      ;			/* result file pointer		*/
struct paseq *ptseq   ;			/* paseq structure to be drived	*/
unsigned long  labelsiz;			/* length of the longest label	*/

  {
  long           tmpc  ;
  time_t        clock ;

  struct papat *ptpat   = NULL;		/* current pattern		*/
  struct paevt *ptevt   = NULL;		/* current event		*/
  struct pacom *ptcom   = NULL;		/* current comment		*/
  struct pacom *svcom   = NULL;		/* last comment of OLDPACOM	*/
  struct pagrp *ptgrp   = NULL;		/* current input-output array	*/
  struct paiol *ptiol   = NULL;		/* current input-output	element	*/
  long           iol_idx = 0   ;		/* index in input-output list	*/
  long           stt_nbr = 0   ;		/* # of stm. since last comment	*/
  long           pat_nbr = 0   ;		/* # of patterns		*/

  long           row     = 1   ;		/* maximum length of I/O names	*/
  long           column  = 5   ;		/* number of char. in a pattern	*/

  long           indx    ;
  long           left    ;		/* left  bound of an array	*/
  long           right   ;		/* right bound of an array	*/
  long           redvar  ;
  long           oldred  ;
  char          name     [256];
  char          oldname  [256];
  char          pat_date [ 32];
  char         *mode    ;		/* input-output's mode		*/
  char         *direc   ;		/* input-output array's direc.	*/
  char          format  ;		/* input-output format		*/
  char          seprt   ;		/* name separator left par or ,	*/

  long           i       ;
  long           j       ;
  long           compare = 0;
  char           sflag   = 'S';		/* processed or unprocessed	*/
  long           length  ;		/* length of string		*/
  long           ret_val = 0  ;		/* value to be returned		*/

  unsigned long  t_step ;		/* time step			*/
  unsigned char zd_flg ;		/* zero delay flag (if != 0)	*/

  static char  *model  = NULL;		/* model : "   :?1 2 34;"	*/
  static char  *buffer = NULL;		/* 1 pattern created from model	*/
  static char  *signam = NULL;		/* input-output names (vertical)*/
  static char  *value  = NULL;
  static char  *blank  = ";;;;;;;;;;;;;;;;;\n";
  static char  *line   = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
  static char  *t_unit = NULL;

	/* ###------------------------------------------------------### */
	/*    check the consistency of the sequence of patterns		*/
	/* ###------------------------------------------------------### */

  if ((ptseq == NULL) || (ptseq->ERRFLG != 0))
    ret_val = pat_error (101, NULL, ' ', 0);
  else
    {
	/* ###------------------------------------------------------### */
	/*    initialize the time step and check if the description	*/
	/* is zero delay.						*/
	/* ###------------------------------------------------------### */

    zd_flg = ptseq->FLAGS     & PAT_SEQ__ZERODELAY;
    t_step = ptseq->TIME_STEP                     ;

	/* ###------------------------------------------------------### */
	/*    for an empty structure exit with out making any action.	*/
	/* Otherwise print the header message.				*/
	/* ###------------------------------------------------------### */

    if (ptseq->DRVSEQ == 0)
      {
      fprintf (fp, "\n-- description generated by Pat driver\n\n");
      time    (&clock);
      fprintf (fp, "--\t\t\tdate     : %s"      , ctime(&clock));
      fprintf (fp, "--\t\t\trevision : v109\n\n"               );

      if (ptseq->NAME != NULL)
        fprintf (fp, "--\t\t\tsequence : %s\n\n", ptseq->NAME);

      fprintf (fp, "-- input / output list :\n");
      }

    if (ptseq->DRVSEQ == 0)
      {

	/* ###------------------------------------------------------### */
	/*    if the declaration part has not been drived ...		*/
	/*    scan the input-output list to :				*/
	/*       - print the input-output declaration 			*/
	/*       - compute the number of characters in a pattern	*/
	/*         (including pattern labels) : column			*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*    initialize a string with the time unit			*/
	/* ###------------------------------------------------------### */

      switch (ptseq->TIME_UNIT & PAT_SEQ__TIMEUNIT)
        {
        case PAT_TU__FS :
          t_unit = "fs" ; break;
        case PAT_TU__PS :
          t_unit = "ps" ; break;
        case PAT_TU__NS :
          t_unit = "ns" ; break;
        case PAT_TU__US :
          t_unit = "us" ; break;
        case PAT_TU__MS :
          t_unit = "ms" ; break;
        }

	/* ###------------------------------------------------------### */
	/*    if the input-output list is not organized as an array,	*/
	/* change it into an array					*/
	/* ###------------------------------------------------------### */

      if ((ptseq->FLAGS & PAT_SEQ__IOLARRAY) == 0)
        {
        ptiol = pat_crtpaiol (ptseq->PAIOL);
        pat_frepaiol (ptseq->PAIOL);

        ptseq->PAIOL = ptiol;
        ptseq->FLAGS = PAT_SEQ__IOLARRAY;
        }

	/* ###------------------------------------------------------### */
	/*    if a date is necessary (always, exept for old pat format)	*/
	/* add the length of the date to the length of the pattern 	*/
	/* ###------------------------------------------------------### */

      if (zd_flg == 0)
        column += 16;

	/* ###------------------------------------------------------### */
	/*    add the length of the longest pattern's label to the	*/
	/* length of the pattern. 					*/
	/* ###------------------------------------------------------### */

      column += labelsiz;

      ptiol = ptseq->PAIOL;
      ptgrp = ptseq->PAGRP;
      ptcom = ptseq->DECCOM;

      while (ptiol != NULL)
        {

	/* ###------------------------------------------------------### */
	/*    for each input-output ...					*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*    print a comment line if needed				*/
	/* ###------------------------------------------------------### */

        if ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
          fprintf (fp, "\n");

        while ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
          {
          fprintf (fp, "# %s\n", ptcom->TEXT);
          ptcom   = ptcom->NEXT;
          stt_nbr = 0;
          }

	/* ###------------------------------------------------------### */
	/*    prepare input-output's format and mode 			*/
	/* ###------------------------------------------------------### */

        format = ptiol->FORMAT;
        switch (ptiol->MODE)
          {
          case 'I':
            mode = "in      "; break;
          case 'O':
          case 'B':
          case 'Z':
            mode = "out     "; break;
          case 'R':
            mode = "register"; break;
          case 'T':
            mode = "inout   "; break;
          case 'S':
          case 'W':
          case 'X':
          case 'U':
            mode = "signal  "; break;
          default :
            mode = "signal  ";
            ret_val = pat_error (201, ptiol->NAME, ptiol->MODE, 0);
            break;
          }

	/* ###------------------------------------------------------### */
	/*    increment the length of the pattern for an output ('?')	*/
	/* ###------------------------------------------------------### */

        if (ptiol->MODE != 'I')
          column ++;

        if ((ptgrp != NULL) && (iol_idx == ptgrp->FINDEX))
          {
	/* ###------------------------------------------------------### */
	/*    for an actual or virtual array ...			*/
	/* ###------------------------------------------------------### */

          switch (ptiol->FORMAT)
            {
	/* ###------------------------------------------------------### */
	/*    increment the length of the pattern by the number of	*/
	/* characters needed for the value of the array (including	*/
	/* extra blank characters)					*/
	/* ###------------------------------------------------------### */

            case 'X':
              column += ((ptgrp->LENGTH - 1) / 4) + 1;
              break;
            case 'O':
              column += ((ptgrp->LENGTH - 1) / 3) + 1;
              break;
            case 'B':
              column += ptgrp->LENGTH;
              break;
            }
          column += ptiol->BLANK;

	/* ###------------------------------------------------------### */
	/*    change the maximum length (number of rows) of input-	*/
	/* outputs if the length of the current array's name is bigger	*/
	/* ###------------------------------------------------------### */

          length = strlen (ptgrp->NAME);
          if (row < length)
            row = length;

          if (ptgrp->FLAG == 0)
            {
	/* ###------------------------------------------------------### */
	/*    print out the declaration of an actual array		*/
	/* ###------------------------------------------------------### */

            sscanf (ptiol->NAME, "%255s %22ld", name, &left);
            ptiol += ptgrp->LENGTH - 1;
            sscanf (ptiol->NAME, "%255s %22ld", name, &right);

            if (left < right)
              direc = "to";
            else
              direc = "downto";

            fprintf (fp, "%s %s (%ld %s %ld) %c", mode , name, left, direc,
                                                  right, format);
            }
          else
            {
	/* ###------------------------------------------------------### */
	/*    print out the declaration of a virtual array :		*/
	/*       - mode and name					*/
	/*       - for each subelement, its name and if it is an array,	*/
	/*         its left and right bound and direction		*/
	/* ###------------------------------------------------------### */

            fprintf (fp, "%s %s ", mode, ptgrp->NAME);
            seprt  = '(';
            oldred = sscanf (ptiol->NAME, "%255s %22ld", oldname, &left);
            redvar = oldred;
            right  = left;
            strcpy (name, oldname);

            ptiol++;
            for (i=1 ; i<ptgrp->LENGTH; i++)
              {
              redvar = sscanf (ptiol->NAME, "%255s %22ld", name, &indx);

              if (!strcmp (name, oldname))
                right = indx;
              else
                {
                if (oldred == 1)
                  fprintf (fp, "%c %s", seprt, oldname);
                else
                  {
                  if (left < right)
                    direc = "to";
                  else
                    direc = "downto";
                  fprintf (fp, "%c %s (%ld %s %ld)", seprt, oldname, left,
                                                     direc, right);
                  }
                left   = indx;
                oldred = redvar;
                strcpy (oldname, name);
                seprt  = ',';
                }
              ptiol ++;
              }

            ptiol --;
            if (redvar == 1)
              fprintf (fp, "%c %s ) %c", seprt, name, format);
            else
              {
              if (left < right)
                direc = "to"    ;
              else
                direc = "downto";

              fprintf (fp, "%c %s (%ld %s %ld) ) %c", seprt, name , left,
                                                      direc, right, format);
              }
            }
          iol_idx += ptgrp->LENGTH;
          ptgrp    = ptgrp->NEXT  ;
          }

        else
          {

	/* ###------------------------------------------------------### */
	/*    for a scalar input-output :				*/
	/*       - print the declaration 				*/
	/*       - change the maximum length of input-outputs' name	*/
	/*       - increment the length of the pattern			*/
	/* ###------------------------------------------------------### */

          redvar = sscanf (ptiol->NAME, "%255s %22ldd", name, &indx);
          if (redvar == 1)
            fprintf (fp, "%s %s %c"     , mode, name, format);
          else
            fprintf (fp, "%s %s (%ld) %c", mode, name, indx, format);

          length = strlen (ptiol->NAME);
          if (row < length)
            row = length;

          column += ptiol->BLANK + 1;
          iol_idx ++;
          }

      /* ###------------------------------------------------------### */
      /*    if the input/output is a spied input/output, print the    */
      /* spy flag                                                     */
      /* ###------------------------------------------------------### */

        if ((ptiol->FLAG & PAT_IOL__SPY) != 0)
          fprintf (fp, " spy ");

	/* ###------------------------------------------------------### */
	/*    print semicolons and '\n'					*/
	/* ###------------------------------------------------------### */

        fprintf (fp, "%s", &blank [16 - ptiol->BLANK]);
        ptiol = ptiol->NEXT;
        stt_nbr ++;
        }

	/* ###------------------------------------------------------### */
	/*    make enough allocation					*/
	/* for 2 patterns           : model, buffer			*/
	/* for input-output's names : signam				*/
	/* for                      : value				*/
	/* ###------------------------------------------------------### */

      model  = mbkalloc (sizeof(char) * column * 2     );
      signam = mbkalloc (sizeof(char) * column * row   );
      value  = mbkalloc (sizeof(char) * (ptseq->IOLNBR));

	/* ###------------------------------------------------------### */
	/*    begin initialisation of value, model and signam:		*/
	/*								*/
	/* value  : unknown value (*) for all inputs-outputs		*/
	/* model represents the format of the pattern : "    :     ;"	*/
	/* signam : "--        \n" * number of rows			*/
	/* ###------------------------------------------------------### */

      for (i=0 ; i<ptseq->IOLNBR ; i++)
        value [i] = '*';

      for (i=0 ; i<column ; i++)
        model [i] = ' ';

      for (i=0 ; i<row ; i++)
        {
        signam [    (i * column)] = '-';
        signam [1 + (i * column)] = '-';

        for (j=2 ; j<column ; j++)
          signam [j + (i * column)] = ' ';

        signam [j - 1 + (i * column)] = '\n';
        }

      signam [(row * column) - 1] = '\0';
      model  [       column  - 2] = ';' ;
      model  [       column  - 1] = '\0';

      buffer = model + column;

	/* ###------------------------------------------------------### */
	/*    leave enough characters for printing the pattern date	*/
	/* ###------------------------------------------------------### */

      if (zd_flg == 0)
        i = labelsiz + 1 + 16;
      else
        i = labelsiz + 1     ;

      model [i]  = ':';
      i         += 2;

	/* ###------------------------------------------------------### */
	/*    continue initialization of model and signam:		*/
	/*								*/
	/* model represents the format of the pattern : "    : ?1234;"	*/
	/*       1,2,3,4 : the value of 1, 2, 3 or, 4, input-outputs is	*/
	/*                 to be combined to constitute this character	*/
	/* signam will contain the name of input-outputs one character	*/
	/*        per line						*/
	/* ###------------------------------------------------------### */

      ptgrp   = ptseq->PAGRP;
      ptiol   = ptseq->PAIOL;
      iol_idx = 0;

      while (ptiol != NULL)
        {
        if (ptiol->MODE != 'I')
          model [i++] = '?';

        if ((ptgrp != NULL) && (iol_idx == ptgrp->FINDEX))
          {
          j = 0;
          while (ptgrp->NAME[j] != '\0')
            {
            signam [i + (j * column)] = ptgrp->NAME [j];
            j++;
            }
          switch (ptiol->FORMAT)
            {
            case 'X':
              model [i++] = ((ptgrp->LENGTH - 1) % 4) + 1;
              for (j=0 ; j<((ptgrp->LENGTH - 1) / 4) ; j++)
                model [i++] = 4;
              break;
            case 'O':
              model [i++] = ((ptgrp->LENGTH - 1) % 3) + 1;
              for (j=0 ; j<((ptgrp->LENGTH - 1) / 3) ; j++)
                model [i++] = 3;
              break;
            case 'B':
              for (j=0 ; j<ptgrp->LENGTH ; j++)
                model [i++] = 1;
              break;
            }
          iol_idx += ptgrp->LENGTH;
          ptiol    = ptseq->PAIOL + iol_idx - 1;

          ptgrp    = ptgrp->NEXT;
          }
        else
          {
          j = 0;
          while (ptiol->NAME [j] != '\0')
            {
            signam [i + (j * column)] = ptiol->NAME [j];
            j++;
            }
          model [i++] = 1;
          iol_idx++;
          }

        i     += ptiol->BLANK;
        ptiol  = ptiol->NEXT;
        }

	/* ###------------------------------------------------------### */
	/*    print a comment line if needed				*/
	/* ###------------------------------------------------------### */

      if ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
        fprintf (fp, "\n");

      while ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
        {
        fprintf (fp, "# %s\n", ptcom->TEXT);
        ptcom   = ptcom->NEXT;
        stt_nbr = 0;
        }

	/* ###------------------------------------------------------### */
	/*    begin printing the pattern description part		*/
	/* ###------------------------------------------------------### */

      fprintf (fp, "\nbegin\n");
      stt_nbr = 0;

      fprintf (fp, "\n-- Pattern description :\n\n");
      fprintf (fp, "%s\n\n", signam);

      }

	/* ###------------------------------------------------------### */
	/*    for every sub-sequence ...				*/
	/* ###------------------------------------------------------### */

    ptiol   = ptseq->PAIOL ;
    ptpat   = ptseq->CURPAT;
    ptcom   = ptseq->CURCOM;
    pat_nbr = ptseq->PATNBR;

    if (ptseq->DRVSEQ < (ptseq->SUBSEQ - 1))
      {
	/* ###------------------------------------------------------### */
	/*    if the old sub-sequence has not been drived		*/
	/* ###------------------------------------------------------### */

      fprintf (fp, "\n-- Beware : some patterns may be missing\n\n");

      if (ptseq->OLDPAT != NULL)
        {
        ptpat = ptseq->OLDPAT;
        while (ptpat->NEXT != ptseq->CURPAT)
          {
          pat_nbr ++;
          ptpat = ptpat->NEXT;
          }
        ptpat = ptseq->OLDPAT;
        }

	/* ###------------------------------------------------------### */
	/*    if any old comment exists, merge old and current comments	*/
	/* lists							*/
	/* ###------------------------------------------------------### */

      if (ptseq->OLDCOM != NULL)
        {
        ptcom = ptseq->OLDCOM;
        while (ptcom->NEXT != NULL)
          ptcom = ptcom->NEXT;

        svcom       = ptcom;
        ptcom->NEXT = ptseq->CURCOM;
        ptcom       = ptseq->OLDCOM;
        }
      }

    while ((ptpat != NULL) && (pat_nbr > 0))
      {
      i       = 0;
      iol_idx = 0;

	/* ###------------------------------------------------------### */
	/*    check for processed or unprocessed patterns		*/
	/* ###------------------------------------------------------### */

      if ((ptpat->SIMFLAG == 'U') && (sflag == 'S'))
        fprintf (fp, "\n-- Beware : unprocessed patterns\n\n");

      sflag = ptpat->SIMFLAG;

	/* ###------------------------------------------------------### */
	/*    print a comment line if needed				*/
	/*    print initialisation if needed				*/
	/* ###------------------------------------------------------### */

      if ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
        fprintf (fp, "\n");

      while ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
        {
        fprintf (fp, "# %s\n", ptcom->TEXT);
        ptcom   = ptcom->NEXT;
        stt_nbr = 0;
        }

      if (ptpat->PAINI != NULL)
        {
        dclini  (fp, ptpat);
        fprintf (fp, "\n");
        }

	/* ###------------------------------------------------------### */
	/*    for each event of the pattern :				*/
	/*      - print an error message if simulation and user values	*/
	/*        mismatch						*/
	/*      - copy the event in the value string			*/
	/* ###------------------------------------------------------### */

      ptevt = ptpat->PAEVT;
      while (ptevt != NULL)
        {
        iol_idx = ptevt->INDEX;
        if ((ptevt->USRVAL != '*') && (ptevt->USRVAL != ptevt->SIMVAL))
          {
          ptiol = ptseq->PAIOL + iol_idx;

          if (sscanf (ptiol->NAME, "%255s %22ld", name, &indx) == 2)
            fprintf (fp, "-- error on `%s (%ld)` : ", name, indx );
          else
            fprintf (fp, "-- error on `%s` : "     , ptiol->NAME);

          fprintf (fp, "expected value: %c\n", ptevt->USRVAL);
          }
        value [iol_idx] = ptevt->SIMVAL;
        ptevt           = ptevt->NEXT  ;
        }

	/* ###------------------------------------------------------### */
	/*   print a comment line if the pattern has been produced due	*/
	/* to an event on a spied signal.				*/
	/* ###------------------------------------------------------### */

      if ((ptpat->FLAG & PAT_PAT__SPY) != 0)
        {
        fprintf (fp, "-- spy pattern : ");

        ptevt = ptpat->PAEVT;
        while (ptevt != NULL)
          {
          ptiol = ptseq->PAIOL + iol_idx;

          if ((ptiol->FLAG & PAT_IOL__SPY) != 0)
            {
            if (sscanf (ptiol->NAME, "%255s %22ld", name, &indx) == 2)
              fprintf (fp, "%s (%ld) ", name, indx );
            else
              fprintf (fp, "%s "     , ptiol->NAME);
            }
          }
        fprintf (fp, "\n");
        }

	/* ###------------------------------------------------------### */
	/*    if a date has been specified for the pattern print it	*/
	/* with the unit specified in the sequence			*/
	/* ###------------------------------------------------------### */

      if (zd_flg == 0)
        {
        sprintf (pat_date, "<%11lu %s>", ptpat->TIME * t_step, t_unit);

        for (i=0 ; i<16 ; i++)
          buffer [i] = pat_date [i];
        }

	/* ###------------------------------------------------------### */
	/*    copy the pattern's label in buffer			*/
	/* ###------------------------------------------------------### */

      if (ptpat->LABEL != NULL)
        {
        j = 0;
        while ((ptpat->LABEL [j] != '\0') && (j < labelsiz))
          {
          buffer [i] = ptpat->LABEL [j];
          j ++;
          i ++;
          }
        }

	/* ###------------------------------------------------------### */
	/*    fill the buffer with the pattern's value : take 1, 2, 3	*/
	/* or, 4 paiol structures to form the value in the buffer	*/
	/* ###------------------------------------------------------### */

      iol_idx = 0;
      ptiol   = ptseq->PAIOL;

      while ((buffer [i] = model [i]) != '\0')
        {
        if ((model [i] >= 1) && (model [i] <= 4))
          {
          tmpc = 0;
          for (j=0 ; j<model [i] ; j++)
            {
            if ((tmpc != '*') && (tmpc != 'u'))
              {
              switch (value [iol_idx])
                {
                case '+':
                  tmpc = (tmpc << 1) + 1; compare = 1; break;
                case '-':
                  tmpc =  tmpc << 1     ; compare = 1; break;
                case '0':
                  tmpc =  tmpc << 1     ; compare = 0; break;
                case '1':
                  tmpc = (tmpc << 1) + 1; compare = 0; break;
                case '*':
                  tmpc = '*'            ; compare = 1; break;
                case 'u':
                  tmpc = 'u'            ; compare = 1; break;
                default :
                  tmpc = 'u'            ; compare = 1;
                  ret_val = pat_error (201, NULL, value [iol_idx], 0);
                }
              }
            iol_idx ++;
            ptiol   ++;
            }

          if ((tmpc == '*') || (tmpc == 'u'))
            buffer [i] = tmpc;
          else
            {
            if (tmpc < 10)
              buffer [i] = '0' + tmpc;
            else
              buffer [i] = 'a' + tmpc - 10;
            }

          if (((ptiol - 1)->MODE == 'T') && (buffer [i-1] == '?') &&
              ( compare          == 0  ))
            buffer [i-1] = ' ';
          }
        i++;
        }

      fprintf (fp, "%s%s%s", buffer, &blank [17 - ptpat->BLANK],
                                     &line  [18 - ptpat->BLANK]);

      stt_nbr ++;
      pat_nbr --;

      ptpat = ptpat->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    print a comment line if needed				*/
	/* ###------------------------------------------------------### */

    if ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
      fprintf (fp, "\n");

    while ((ptcom != NULL) && (ptcom->POSITION == stt_nbr))
      {
      fprintf (fp, "# %s\n", ptcom->TEXT);
      ptcom   = ptcom->NEXT;
      stt_nbr = 0;
      }

	/* ###------------------------------------------------------### */
	/*    restore the original old and current comments lists	*/
	/* ###------------------------------------------------------### */

    if (svcom != NULL)
      svcom->NEXT = NULL;

	/* ###------------------------------------------------------### */
	/*    for the last subsequence print a save (if	needed) and	*/
	/* the end statement. Release allocated buffers (model, signam,	*/
	/* value).							*/
	/* ###------------------------------------------------------### */

    if (ptseq->ENDFLG == 'Y')
      {
      if (ptseq->SAVFLG == 'Y')
        fprintf (fp, "\nsave;\n");

      fprintf (fp, "\nend;\n");

      mbkfree (model );
      mbkfree (signam);
      mbkfree (value );
      }

	/* ###------------------------------------------------------### */
	/*    update the drived subsequence number			*/
	/* ###------------------------------------------------------### */

    ptseq->DRVSEQ = ptseq->SUBSEQ;
    }

  return (ret_val);
  }
