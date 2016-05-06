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
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: c_sr1k_8b.c						*/
/* date		: Dec 13 1999						*/
/* version	: v2.03							*/
/* author	: Pirouz Bazargan Sabet					*/
/* content	: C model of a circuit					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "vh_ltype.h"
#include "vh_lspec.h"
#include "vh_xspec.h"
#include "vh_xcomm.h"


/* ###--------------------------------------------------------------### */
/* function	: splitname						*/
/* description	: split the instance's name in file name, address and	*/
/*		  byte position: xxxx.filname_address.xxx_b		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void splitname (name, fil_name, adr_pnt, byt_pnt)

char         *name    ;
char         *fil_name;
unsigned int *adr_pnt ;
unsigned int *byt_pnt ;

  {
  unsigned int state = 0;
  unsigned int i     = 0;
  unsigned int l_idx = 0;
  unsigned int adr   = 0;

  strcpy (fil_name, name);

  for (i=0 ; name [i] != '\0' ; i++)
    {
    switch (state)
      {
      case 0:
        if (name [i] == '_')
          {
          state = 1;
          l_idx = i;
          }
        break;

      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
        state ++;
        if ((name [i] >= '0') && (name [i] <= '9'))
          adr = (adr << 4) + (name [i] - '0');
        else
          {
          if ((name [i] >= 'a') && (name [i] <= 'f'))
            adr = (adr << 4) + (name [i] - 'a' + 10);
          else
            {
            adr = 0;
            if (name [i] == '_')
              state = 1;
            else
              state = 0;
            l_idx = i;
            }
          }
        break;
      default:
        break;
      }
    }

  fil_name [l_idx] = '\0';
  *adr_pnt         = adr ;
  *byt_pnt         = name [i-1] - '0';
  }

/* ###--------------------------------------------------------------### */
/* function	: init							*/
/* description	: initializations					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

#define READ_ADR 0
#define READ_VAL 1
#define ADR_MASK 0xfffff000		/* 1 k byte			*/
#define BYT_MASK 0x00000003		/* byte address inside a word	*/

static void init (pt_lkdins, pt_mem)

struct lkdins *pt_lkdins;
unsigned char *pt_mem   ;

  {
  char         *name          ;
  char          fil_name [128];
  unsigned int  fil_addr      = 0;
  unsigned int  fil_byte      = 0;
  FILE         *fp            ;
  unsigned int  cur_adr       = 0;
  unsigned int  cur_val       = 0;
  unsigned int  red_c         = 0;
  unsigned int  red_val       = 0;
  unsigned int  state         = READ_ADR;

  name = (char *)vhx_GetInstanceName (pt_lkdins);

  splitname (name, fil_name, &fil_addr, &fil_byte);

  if ((fp = mbkfopen (fil_name, "ini", READ_TEXT)) == NULL)
    {
    fprintf (stderr, "cannot open RAM initialization file: %s.ini\n", fil_name);
    exit (1);
    }

  printf ("loading RAM initialization file: %s.ini , %.8x , %u\n",
          fil_name, fil_addr, fil_byte);


  while ((red_c = fgetc (fp)) != EOF)
    {
    switch (red_c)
      {
      case ' ' :
      case '\t':
        break;

      case '\n':
        if (((cur_adr & ADR_MASK) == fil_addr) &&
            ((cur_adr & BYT_MASK) == fil_byte)   )
          pt_mem [(cur_adr & (~ADR_MASK)) >> 2] = cur_val;

        state   = READ_ADR;
        cur_adr = 0;
        cur_val = 0;
        break;

      case ':' :
        state = READ_VAL;
        break;

      default  :
        if ((red_c >= '0') && (red_c <= '9'))
          red_val = red_c - '0';
        if ((red_c >= 'a') && (red_c <= 'f'))
          red_val = red_c - 'a' + 10;
        if ((red_c >= 'A') && (red_c <= 'F'))
          red_val = red_c - 'A' + 10;

        if (state == READ_ADR)
          cur_adr = (cur_adr << 4) + red_val;
        else
          cur_val = (cur_val << 4) + red_val;
      }
    }

  fclose (fp);
  }

/* ###--------------------------------------------------------------### */
/* function	: c_sr1k_8b						*/
/* description	: A 1 KByte static Ram with a chip select pin		*/
/* called func.	: vhx_getvalu, vhx_readinput, vhx_writebus		*/
/* ###--------------------------------------------------------------### */

void c_sr1k_8b (pt_lkdins)

struct lkdins *pt_lkdins;

  {
  char         e_n ;			/* chip select			*/
  char         w_n ;			/* write enable			*/
  unsigned int dat = 0;			/* input-output 8-bit data	*/
  unsigned int adr ;			/* 10-bit address		*/

#define POSI_vss             0
#define POSI_vdd POSI_vss +  1
#define POSI_adr POSI_vdd +  1
#define POSI_dat POSI_adr + 10
#define POSI_w_n POSI_dat +  8
#define POSI_e_n POSI_w_n +  1

#define POSO_dat             0

  unsigned char *mem       ;
  unsigned int   cond      = 0;

	/* ###------------------------------------------------------### */
	/*   read the current value of input signals (e_n)		*/
	/* ###------------------------------------------------------### */

  e_n = vhx_readinput (pt_lkdins, POSI_e_n, 1);

	/* ###------------------------------------------------------### */
	/*   read the current value of input signals			*/
	/* ###------------------------------------------------------### */

  if (e_n == 0)
    {
    w_n = vhx_readinput (pt_lkdins, POSI_w_n,  1);
    adr = vhx_readinput (pt_lkdins, POSI_adr, 10);
    dat = vhx_readinput (pt_lkdins, POSI_dat,  8);

	/* ###------------------------------------------------------### */
	/*   get the memory space reserved for the current instance	*/
	/* ###------------------------------------------------------### */

    vhx_getvalu (pt_lkdins, &mem, 1024, init);

	/* ###------------------------------------------------------### */
	/*   read or write the data from the memory			*/
	/* ###------------------------------------------------------### */

    if (w_n == 0)
      mem [adr] = dat;
    else
      dat = mem [adr];

    }

	/* ###------------------------------------------------------### */
	/*   write the result into the projected value of output signals	*/
	/* ###------------------------------------------------------### */

  if ((e_n == 0) && (w_n == 1))
    cond = 0x000000ff;
  else
    cond = 0x00000000;

  vhx_writebus (dat, cond, pt_lkdins, POSO_dat, 8);

  }
