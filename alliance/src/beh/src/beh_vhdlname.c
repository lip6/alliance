/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: beh_vhdlname.c					*/
/* date		: Mar  1 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_vhdlname						*/
/* description	: make a legal VHDL name from a string			*/
/* called func.	: namealloc, beh_initab, beh_addtab, beh_chktab		*/
/* ###--------------------------------------------------------------### */

char *beh_vhdlname (name)

char *name;

  {
  char                 *new_name        ;
  char                 *prv_name        ;
  char                 *tmp_name        ;
  char                  buffer [256]    ;
  int                   number          ;
  int                   flag            ;
  int                   i               ;
  int                   j               ;
  static struct beden **namtab          = NULL;
  static char          *keywrd [] = {
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
                                     "xor"
                                     };

	/* ###------------------------------------------------------### */
	/*   create a local dictionary. This dictionary is used to save	*/
	/* the VHDL keywords. In addition, it contains the strings	*/
	/* that has been changed into legal VHDL names and their	*/
	/* translation.							*/
	/* ###------------------------------------------------------### */

  if (namtab == NULL)
    {
    namtab = beh_initab ();
    for (i=0 ; i<93 ; i++)
      beh_addtab (namtab, namealloc (keywrd [i]), NULL, 1, 1);
    }

	/* ###------------------------------------------------------### */
	/*   check if the string has already been seen. In this case,	*/
	/* just return the VHDL legal name attributed to the string	*/
	/* ###------------------------------------------------------### */

  tmp_name = namealloc (name);
  new_name = (char *) beh_chktab (namtab, tmp_name, NULL, 7);

	/* ###------------------------------------------------------### */
	/*   otherwise, make a legal VHDL name from the string by	*/
	/* replacing illegal characters by '_'. If two adjacent '_' are	*/
	/* found a 'v' character is inserted. If the last character is	*/
	/* '_', a '0' is added at the end of the name.			*/
	/* ###------------------------------------------------------### */

  if (beh_chktab (namtab, tmp_name, NULL, 0) == 0)
    {
    i      = 0;
    j      = 0;
    number = 0;
    flag   = 1;
    while (tmp_name [i] != '\0')
      {
      buffer [j] = tmp_name [i];
      if ( ((tmp_name [i] >= 'a') && (tmp_name [i] <= 'z')) ||
           ((tmp_name [i] >= 'A') && (tmp_name [i] <= 'Z')) ||
           ((tmp_name [i] >= '0') && (tmp_name [i] <= '9')  && (i != 0)))
        flag = 0;
      else
        {
        if (flag == 1)
          buffer [j++] = 'v';

        buffer [j] = '_';
        flag       = 1;
        }
      i++;
      j++;
      }
    if (buffer [j-1] == '_')
      buffer [j++] = '0';

    buffer [j] = '\0';
    new_name   = namealloc (buffer);

	/* ###------------------------------------------------------### */
	/*   check that the new name has not been already attributed to	*/
	/* another string. In such a case, add a '_number' at the end	*/
	/* of the word and check again. The number is incremented as	*/
	/* many times as needed to obtain a name that has never been	*/
	/* used.							*/
	/* ###------------------------------------------------------### */

    prv_name = new_name;
    while (beh_chktab (namtab, new_name, NULL, 1) != 0)
      {
      new_name = prv_name;
      sprintf (buffer, "%s_%d", new_name, number++);
      prv_name = new_name;
      new_name = namealloc (buffer);
      }
    beh_addtab (namtab, new_name, NULL, 1, 1              );
    beh_addtab (namtab, tmp_name, NULL, 7, (long) new_name);
    beh_addtab (namtab, tmp_name, NULL, 0, 1              );
    }

  return (new_name);
  }
