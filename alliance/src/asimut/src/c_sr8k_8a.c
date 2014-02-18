
/* ###--------------------------------------------------------------### */
/* file		: c_sr8k_8a.c						*/
/* date		: Jun  3 1997						*/
/* version	: v2.02							*/
/* author	: Pirouz Bazargan Sabet					*/
/* content	: C model of a circuit					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "vh_ltype.h"
#include "vh_xcomm.h"

/* ###--------------------------------------------------------------### */
/* function	: init							*/
/* description	: initializations					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void init (pt_lkdins, pt_mem)

struct lkdins *pt_lkdins;
char          *pt_mem   ;

  {
  char *name;

  name = vhx_GetInstanceName (pt_lkdins);
  printf ("coucou %s\n", name);
  }

/* ###--------------------------------------------------------------### */
/* function	: c_sr8k_8a						*/
/* description	: A 8 KByte static Ram with a chip select pin		*/
/* called func.	: vhx_getvalu, vhx_readinput, vhx_writebus		*/
/* ###--------------------------------------------------------------### */

void c_sr8k_8a (pt_lkdins)

struct lkdins *pt_lkdins;

  {
  char         e_n ;			/* chip select			*/
  char         w_n ;			/* write enable			*/
  unsigned int dat = 0;			/* input-output 8-bit data	*/
  unsigned int adr ;			/* 13-bit address		*/

#define POSI_vss             0
#define POSI_vdd POSI_vss +  1
#define POSI_adr POSI_vdd +  1
#define POSI_dat POSI_adr + 13
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
    adr = vhx_readinput (pt_lkdins, POSI_adr, 13);
    dat = vhx_readinput (pt_lkdins, POSI_dat,  8);

	/* ###------------------------------------------------------### */
	/*   get the memory space reserved for the current instance	*/
	/* ###------------------------------------------------------### */

    vhx_getvalu (pt_lkdins, &mem, 1024 * 8, init);

	/* ###------------------------------------------------------### */
	/*   read or write the data from the memory			*/
	/* ###------------------------------------------------------### */

    if (w_n == 0)
      mem [adr] = dat;
    else
      dat = mem [adr];

    }

	/* ###------------------------------------------------------### */
	/*   write the result into the projected value of ouput signals	*/
	/* ###------------------------------------------------------### */

    if ((e_n == 0) && (w_n == 1))
      cond = 0x000000ff;
    else
      cond = 0x00000000;

    vhx_writebus (dat, cond, pt_lkdins, POSO_dat, 8);

  }
