
/* ###--------------------------------------------------------------### */
/* file		: c_fsyn_sr1k_10.c					*/
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
/* function	: c_fsyn_sr1k_10					*/
/* description	: 1K synchronous (clock's falling edge) static 10-bit	*/
/*		  Ram							*/
/* called func.	: vhx_getvalu, vhx_readinput, vhx_writebus		*/
/* ###--------------------------------------------------------------### */

void c_fsyn_sr1k_10 (pt_lkdins)

struct lkdins *pt_lkdins;

  {
  char         ck    ;			/* clock			*/
  unsigned int d_out ;			/* output 10-bit data		*/
  unsigned int d_in  ;			/* input  10-bit data		*/
  unsigned int adr   ;			/* 10-bit address		*/
  char         oe    ;			/* output enable		*/
  char         we    ;			/* write enable			*/

#define POSI_we                 0
#define POSI_oe    POSI_we   +  1
#define POSI_adr   POSI_oe   +  1
#define POSI_d_in  POSI_adr  + 10
#define POSI_ck    POSI_d_in + 10

#define POSO_d_out              0

  struct local_mem
    {
    unsigned int   LAST_CK       ;
    unsigned short RAM     [1024];
    };

  struct local_mem *local   ;

	/* ###------------------------------------------------------### */
	/*   get the memory space reserved for the current instance	*/
	/* ###------------------------------------------------------### */

  vhx_getvalu (pt_lkdins, &local, sizeof(struct local_mem), init);

	/* ###------------------------------------------------------### */
	/*   read the current value of input signals (ck)		*/
	/* ###------------------------------------------------------### */

  ck = vhx_readinput (pt_lkdins, POSI_ck, 1);

  if (ck == 1)
    local->LAST_CK = 1;
  else
    {
	/* ###------------------------------------------------------### */
	/*   check for the falling edge of the clock			*/
	/* ###------------------------------------------------------### */

    if (local->LAST_CK == 1)
      {
      local->LAST_CK = 0;

      oe   = vhx_readinput (pt_lkdins, POSI_oe  ,  1);
      we   = vhx_readinput (pt_lkdins, POSI_we  ,  1);
      adr  = vhx_readinput (pt_lkdins, POSI_adr , 10);

      if (we == 1)
        {
	/* ###------------------------------------------------------### */
	/*   write the input data into the ram				*/
	/* ###------------------------------------------------------### */

        d_in = vhx_readinput (pt_lkdins, POSI_d_in, 10);
        local->RAM [adr] = d_in;
        }
      else
        {
	/* ###------------------------------------------------------### */
	/*   read the content of the ram and write the result into the	*/
	/* projected value of ouput data				*/
	/* ###------------------------------------------------------### */

        if (oe == 1)
          d_out = local->RAM [adr];
        else
          d_out = 0;

        vhx_writeout (d_out, pt_lkdins, POSO_d_out, 10);
        }
      }
    }
  }

