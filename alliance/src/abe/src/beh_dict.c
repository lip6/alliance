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

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_dict.c						*/
/* date		: Jun 15 1994						*/
/* version	: v107							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "abe.h"

#define BEH_ALODFN  64
#define ABE_HSZDFN  97

static struct beden *BEH_DCEHED = NULL;		/* free entries list	*/
static struct bedrd *BEH_DCRHED = NULL;		/* free records list	*/

/* ###--------------------------------------------------------------### */
/* function	: beh_addent						*/
/* description	: add a new entry point to the dictionary		*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

static struct beden *beh_addent (head, key)

struct beden *head;
char         *key;

  {
  struct beden *entry;
  int           i    ;

  if (BEH_DCEHED == NULL)
    {
    BEH_DCEHED = (struct beden *)
                 mbkalloc (sizeof (struct beden) * BEH_ALODFN);

    entry = BEH_DCEHED;
    for (i=1 ; i<BEH_ALODFN ; i++)
      {
      entry->NEXT = entry + 1;
      entry++;
      }
    entry->NEXT = NULL;
    }

  entry       = BEH_DCEHED;
  BEH_DCEHED  = BEH_DCEHED->NEXT;

  entry->NEXT = head;
  entry->DATA = NULL;
  entry->KEY  = key;

  return (entry);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_addrcd						*/
/* description	: add a new record to the dictionary			*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

static struct bedrd *beh_addrcd (head, key)

struct bedrd *head;
char         *key;

  {
  struct bedrd *recrd;
  int           i    ;

  if (BEH_DCRHED == NULL)
    {
    BEH_DCRHED = (struct bedrd *)
                 mbkalloc (sizeof (struct bedrd) * BEH_ALODFN);

    recrd = BEH_DCRHED;
    for (i=1 ; i<BEH_ALODFN ; i++)
      {
      recrd->NEXT = recrd + 1;
      recrd++;
      }
    recrd->NEXT = NULL;
    }

  recrd           = BEH_DCRHED;
  BEH_DCRHED      = BEH_DCRHED->NEXT;

  recrd->NEXT     = head;
  recrd->FD0_VAL  = 0;
  recrd->FD1_VAL  = 0;
  recrd->FD2_VAL  = 0;
  recrd->FD3_VAL  = 0;
  recrd->FD4_VAL  = 0;
  recrd->FD5_VAL  = 0;
  recrd->FD6_VAL  = 0;
  recrd->PNT_VAL  = 0;
  recrd->KEY      = key;

  return (recrd);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_initab						*/
/* description	: create a new dictionary				*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

struct beden **beh_initab ()

  {
  struct beden **head;
  int                i;

  head = (struct beden **)
         mbkalloc (sizeof(struct beden *) * ABE_HSZDFN);

  for (i=0 ; i<ABE_HSZDFN ; i++)
    head[i] = NULL;

  return (head);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_addtab						*/
/* description	: save a data in a dictionary				*/
/* called func.	: beh_addent, beh_addrcd				*/
/* ###--------------------------------------------------------------### */

void beh_addtab (head, key_str, ctx_str, field, valu)

struct beden **head;
char          *key_str;
char          *ctx_str;
int            field;
int            valu;

  {
  int               found = 0;
  int               index;
  struct beden *entry_pnt;
  struct bedrd *recrd_pnt;

  index     = ((unsigned long) key_str) % ABE_HSZDFN;
  entry_pnt = head[index];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->KEY == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->NEXT;
    }

  if (found == 0)
    {
    head[index] = beh_addent (head[index],key_str); 
    entry_pnt = head[index]; 
    }

  found = 0;
  recrd_pnt = entry_pnt->DATA;
  while (recrd_pnt != NULL)
    {
    if (recrd_pnt->KEY == ctx_str)
      {
      found = 1;
      break;
      }
    recrd_pnt = recrd_pnt->NEXT;
    }

  if (found == 0)
    {
    entry_pnt->DATA = beh_addrcd (entry_pnt->DATA,ctx_str); 
    recrd_pnt       = entry_pnt->DATA ;
    }

  switch (field)
    {
    case 0 :
      recrd_pnt->FD0_VAL = valu; break;
    case 1 :
      recrd_pnt->FD1_VAL = valu; break;
    case 2 :
      recrd_pnt->FD2_VAL = valu; break;
    case 3 :
      recrd_pnt->FD3_VAL = valu; break;
    case 4 :
      recrd_pnt->FD4_VAL = valu; break;
    case 5 :
      recrd_pnt->FD5_VAL = valu; break;
    case 6 :
      recrd_pnt->FD6_VAL = valu; break;
    case 7 :
      recrd_pnt->PNT_VAL = valu; break;
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: beh_chktab						*/
/* description	: extract a data from a dictionary			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

long beh_chktab (head, key_str, ctx_str, field)

struct beden **head;
char          *key_str;
char          *ctx_str;
int            field;

  {
  int           found = 0;
  long          valu = 0;
  struct beden *entry_pnt;
  struct bedrd *recrd_pnt;

  entry_pnt = head [((unsigned long)key_str) % ABE_HSZDFN];

  while (entry_pnt != NULL)
    {
    if (entry_pnt->KEY == key_str)
      {
      found = 1;
      break;
      }
    entry_pnt = entry_pnt->NEXT;
    }

  if (found == 1)
    {
    found = 0;
    recrd_pnt = entry_pnt->DATA;
    while (recrd_pnt != NULL)
      {
      if (recrd_pnt->KEY == ctx_str)
        {
        found = 1;
        break;
        }
      recrd_pnt = recrd_pnt->NEXT;
      }
    if (found == 1)
      {
      switch (field)
        {
        case 0 :
          valu = recrd_pnt->FD0_VAL; break;
        case 1 :
          valu = recrd_pnt->FD1_VAL; break;
        case 2 :
          valu = recrd_pnt->FD2_VAL; break;
        case 3 :
          valu = recrd_pnt->FD3_VAL; break;
        case 4 :
          valu = recrd_pnt->FD4_VAL; break;
        case 5 :
          valu = recrd_pnt->FD5_VAL; break;
        case 6 :
          valu = recrd_pnt->FD6_VAL; break;
        case 7 :
          valu = recrd_pnt->PNT_VAL; break;
        }
      }
    }

  return (valu);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_fretab						*/
/* description	: release a dictionary					*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

void beh_fretab (pt_hash)

struct beden **pt_hash;

  {
  struct beden *pt_entry;
  struct beden *pt_nxtentry;
  struct bedrd *pt_record;
  int           i;

  if (pt_hash != NULL)
    {
    for (i=0 ; i<ABE_HSZDFN ; i++)
      {
      if ((pt_entry = pt_hash[i]) != NULL)
        {
        while (pt_entry != NULL)
          {
          pt_record = pt_entry->DATA;

          while (pt_record->NEXT != NULL)
            pt_record = pt_record->NEXT;

          pt_record->NEXT = BEH_DCRHED;
          BEH_DCRHED      = pt_entry->DATA;

          pt_nxtentry     = pt_entry->NEXT;
          pt_entry->NEXT  = BEH_DCEHED;
          BEH_DCEHED      = pt_entry;
          pt_entry        = pt_nxtentry;
          }
        }
      }
    autfreeblock (pt_hash);
    }
  }
