
/* ###--------------------------------------------------------------### */
/* file		: vh_init.c						*/
/* date		: Dec 13 1999						*/
/* version	: v3.02							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: this file contains the following functions		*/
/*		  vhi_addcmodel, vhi_models				*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "vh_ltype.h"
#include "vh_globals.h"
#include "vh_util.h"

/* ###--------------------------------------------------------------### */
/* function	: vhi_addcmodel						*/
/* description	: create a data structure that associate the c model's	*/
/*		  name and the pointer of the function that represents	*/
/*		  that C model						*/
/* called func.	: vhu_alloc, namealloc					*/
/* ###--------------------------------------------------------------### */

void vhi_addcmodel (str, fonc_pnt)

char   *str         ;
void ( *fonc_pnt) ();

  {
  struct lkdmdl *model;

  model = (struct lkdmdl *) vhu_alloc (sizeof (struct lkdmdl), VHU_ALIGN);

  model->NAME = namealloc (str);
  model->FONC = fonc_pnt       ;
  model->NEXT = VHL_MODELS     ;

  VHL_MODELS  = model;
  }

/* ###--------------------------------------------------------------### */
/* function	: vhi_models						*/
/* description	: declare available C models				*/
/* called func.	: vhi_addcmodel						*/
/* ###--------------------------------------------------------------### */

void vhi_models ()
  {
  extern void c_sr1k_8a        ();
  extern void c_sr1k_8b        ();
  extern void c_sr8k_8a        ();
  extern void c_fsyn_sr1k_1    ();
  extern void c_fsyn_sr1k_4    ();
  extern void c_fsyn_sr1k_10   ();
  extern void c_fsyn_sr1k_24   ();
  extern void c_fsyn_sr1k_56   ();
  extern void c_fsyn_sr4k_10   ();
  extern void c_hada_repondeur ();
  
  vhi_addcmodel ("c_sr1k_8a"       , c_sr1k_8a       );
  vhi_addcmodel ("c_sr1k_8b"       , c_sr1k_8b       );
  vhi_addcmodel ("c_sr8k_8a"       , c_sr8k_8a       );
  vhi_addcmodel ("c_fsyn_sr1k_1"   , c_fsyn_sr1k_1   );
  vhi_addcmodel ("c_fsyn_sr1k_4"   , c_fsyn_sr1k_4   );
  vhi_addcmodel ("c_fsyn_sr1k_10"  , c_fsyn_sr1k_10  );
  vhi_addcmodel ("c_fsyn_sr1k_24"  , c_fsyn_sr1k_24  );
  vhi_addcmodel ("c_fsyn_sr1k_56"  , c_fsyn_sr1k_56  );
  vhi_addcmodel ("c_fsyn_sr4k_10"  , c_fsyn_sr4k_10  );
  vhi_addcmodel ("c_hada_repondeur", c_hada_repondeur);
  }
