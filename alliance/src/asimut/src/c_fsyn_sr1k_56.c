
/* ###--------------------------------------------------------------### */
/* file		: c_fsyn_sr1k_56.c					*/
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
/* function	: c_fsyn_sr1k_56					*/
/* description	: 1K synchronous (clock's falling edge) static 56-bit	*/
/*		  Ram							*/
/* called func.	: vhx_getvalu, vhx_readinput, vhx_writebus		*/
/* ###--------------------------------------------------------------### */

void c_fsyn_sr1k_56 (pt_lkdins)

struct lkdins *pt_lkdins;

  {
  char         ck     ;			/* clock			*/
  unsigned int dh_out ;			/* output 24 higher bits data	*/
  unsigned int dl_out ;			/* output 32 lower  bits data	*/
  unsigned int dh_in  ;			/* input  24 higher bits data	*/
  unsigned int dl_in  ;			/* input  32 lower  bits data	*/
  unsigned int adr    ;			/* 10-bit address		*/
  char         oe     ;			/* output enable		*/
  char         we     ;			/* write enable			*/

#define POSI_we                    0
#define POSI_oe     POSI_we     +  1
#define POSI_adr    POSI_oe     +  1
#define POSI_dl_in  POSI_adr    + 10
#define POSI_dh_in  POSI_dl_in  + 32
#define POSI_ck     POSI_dh_in  + 24

#define POSO_dl_out                0
#define POSO_dh_out POSO_dl_out + 32

  struct local_mem
    {
    unsigned int  LAST_CK       ;
    unsigned int  HI_RAM  [1024];
    unsigned int  LO_RAM  [1024];
    };

  struct local_mem *local   ;
  static void       init () ;

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

        dl_in = vhx_readinput (pt_lkdins, POSI_dl_in, 32);
        dh_in = vhx_readinput (pt_lkdins, POSI_dh_in, 24);

        local->HI_RAM [adr] = dh_in;
        local->LO_RAM [adr] = dl_in;
        }
      else
        {
        if (oe == 1)
          {
	/* ###------------------------------------------------------### */
	/*   read the content of the ram and write the result into the	*/
	/* projected value of ouput data				*/
	/* ###------------------------------------------------------### */

          dh_out = local->HI_RAM [adr];
          dl_out = local->LO_RAM [adr];
          }
        else
          {
          dh_out = 0;
          dl_out = 0;
          }

        vhx_writeout (dh_out, pt_lkdins, POSO_dh_out, 24);
        vhx_writeout (dl_out, pt_lkdins, POSO_dl_out, 32);
        }
      }
    }
  }

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
