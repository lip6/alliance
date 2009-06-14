
/* ###--------------------------------------------------------------### */
/* file		: vh_xspec.c						*/
/* date		: Jun  3 1997						*/
/* version	: v2.02							*/
/* author	: VUONG H.N. , Pirouz BAZARAGAN SABET			*/
/* description	: This file contains functions relative to the execution*/
/*		  of a circuit :					*/
/*									*/
/*		  vhx_execute  , vhx_addexec, vhx_execall ,		*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mut.h"
#include "mlo.h"
#include "beh.h"
#include "log.h"
#include "pat.h"
#include "sch.h"
#include "vh_ltype.h"
#include "vh_globals.h"
#include "vh_xcomm.h"
#include "vh_xspec.h"

/* ###--------------------------------------------------------------### */
/* function	: vhx_execall						*/
/* description	: execute the whole description				*/
/* called func.	: addptype						*/
/* ###--------------------------------------------------------------### */

void vhx_execall (pt_lkdfig)

struct lkdfig *pt_lkdfig;

  {
  struct lkdins *pt_lkdins = pt_lkdfig->LKDINS;

  while (pt_lkdins != NULL)
    {
    VHX_EXECUTE     = addptype (VHX_EXECUTE, 'I', pt_lkdins);
    pt_lkdins->EVAL = '1';
    pt_lkdins       = pt_lkdins->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_addexec						*/
/* description	: add an element to the execution list			*/
/* called func.	: addptype						*/
/* ###--------------------------------------------------------------### */

void vhx_addexec (dep_list)

struct ptype *dep_list;

  {
  int add_flg = 0;

  while (dep_list != NULL)
    {
    switch (dep_list->TYPE)
      {
      case 'I' :
        if (((struct lkdins *) dep_list->DATA)->EVAL == 0)
          {
          ((struct lkdins *) dep_list->DATA)->EVAL = 1;
          add_flg = 1;
          }
        break;

      case 'S' :
        if (((struct wrssig *) dep_list->DATA)->EVAL == 0)
          {
          ((struct wrssig *) dep_list->DATA)->EVAL = 1;
          add_flg = 1;
          }
        break;

      case 'B' :
        if (((struct wrbsig *) dep_list->DATA)->EVAL == 0)
          {
          ((struct wrbsig *) dep_list->DATA)->EVAL = 1;
          add_flg = 1;
          }
        break;

      case 'R' :
        if (((struct wrireg *) dep_list->DATA)->EVAL == 0)
          {
          ((struct wrireg *) dep_list->DATA)->EVAL = 1;
          add_flg = 1;
          }
        break;

      case 'U' :
        if (((struct wribux *) dep_list->DATA)->EVAL == 0)
          {
          ((struct wribux *) dep_list->DATA)->EVAL = 1;
          add_flg = 1;
          }
        break;

      case 'D' :
      case 'X' :
        if (((struct wriaux *) dep_list->DATA)->EVAL == 0)
          {
          ((struct wriaux *) dep_list->DATA)->EVAL = 1;
          add_flg = 1;
          }
        break;

      case 'M' :
        if (((struct wrimsg *) dep_list->DATA)->EVAL == 0)
          {
          ((struct wrimsg *) dep_list->DATA)->EVAL = 1;
          add_flg = 1;
          }
        break;
      }

    if (add_flg == 1)
      {
      VHX_EXECUTE = addptype (VHX_EXECUTE, dep_list->TYPE, dep_list->DATA);
      add_flg     = 0;
      }

    dep_list = dep_list->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_statexec						*/
/* description	: count simple and multiple evaluation                  */
/*                To evalue the correctness of simple transition        */
/*                hypothesis.                                           */
/* called func.	: addptype						*/
/* ###--------------------------------------------------------------### */

void vhx_statexec (dep_list, dict)

struct ptype *dep_list;
struct beden   **dict;

  {
  unsigned int count = 0;

  while (dep_list != NULL)
    {
    switch (dep_list->TYPE)
      {
      case 'I' :
        break;

      case 'S' :
        if (((struct wrssig *) dep_list->DATA)->EVAL == 0)
          {
          count = (unsigned int) beh_chktab (dict,
              ((struct wrssig *) dep_list->DATA)->SIMSIG, CTXSEV, VHL_PNTDFN);
          beh_addtab (dict, ((struct wrssig *) dep_list->DATA)->SIMSIG,
              CTXSEV, VHL_PNTDFN, count+1);
          }
        else
          {
          count = (unsigned int) beh_chktab (dict,
              ((struct wrssig *) dep_list->DATA)->SIMSIG, CTXMEV, VHL_PNTDFN);
          beh_addtab (dict, ((struct wrssig *) dep_list->DATA)->SIMSIG,
              CTXMEV, VHL_PNTDFN, count+1);
          }
 
        break;

      case 'B' :
        if (((struct wrbsig *) dep_list->DATA)->EVAL == 0)
          {
          count = (unsigned int) beh_chktab (dict,
              ((struct wrbsig *) dep_list->DATA)->BUSSIG, CTXSEV, VHL_PNTDFN);
          beh_addtab (dict, ((struct wrbsig *) dep_list->DATA)->BUSSIG,
              CTXSEV, VHL_PNTDFN, count+1);
          }
        else
          {
          count = (unsigned int) beh_chktab (dict,
              ((struct wrbsig *) dep_list->DATA)->BUSSIG, CTXMEV, VHL_PNTDFN);
          beh_addtab (dict, ((struct wrbsig *) dep_list->DATA)->BUSSIG,
              CTXMEV, VHL_PNTDFN, count+1);
          }
        break;

      case 'R' :
        if (((struct wrireg *) dep_list->DATA)->EVAL == 0)
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wrireg *) dep_list->DATA, CTXSEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wrireg *) dep_list->DATA,
              CTXSEV, VHL_PNTDFN, count+1);
          }
        else
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wrireg *) dep_list->DATA, CTXMEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wrireg *) dep_list->DATA,
              CTXMEV, VHL_PNTDFN, count+1);
          }
        break;

      case 'U' :
        if (((struct wribux *) dep_list->DATA)->EVAL == 0)
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wribux *) dep_list->DATA, CTXSEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wribux *) dep_list->DATA,
              CTXSEV, VHL_PNTDFN, count+1);
          }
        else
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wribux *) dep_list->DATA, CTXMEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wribux *) dep_list->DATA,
              CTXMEV, VHL_PNTDFN, count+1);
          }
        break;

      case 'D' :
      case 'X' :
        if (((struct wriaux *) dep_list->DATA)->EVAL == 0)
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wriaux *) dep_list->DATA, CTXSEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wriaux *) dep_list->DATA,
              CTXSEV, VHL_PNTDFN, count+1);
          }
        else
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wriaux *) dep_list->DATA, CTXMEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wriaux *) dep_list->DATA,
              CTXMEV, VHL_PNTDFN, count+1);
          }
        break;

      case 'M' :
        if (((struct wrimsg *) dep_list->DATA)->EVAL == 0)
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wrimsg *) dep_list->DATA, CTXSEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wrimsg *) dep_list->DATA,
              CTXSEV, VHL_PNTDFN, count+1);
          }
        else
          {
          count = (unsigned int) beh_chktab (dict,
              (struct wrimsg *) dep_list->DATA, CTXMEV, VHL_PNTDFN);
          beh_addtab (dict, (struct wrimsg *) dep_list->DATA,
              CTXMEV, VHL_PNTDFN, count+1);
          }
        break;
      }

    dep_list = dep_list->NEXT;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_execute						*/
/* description	: executes all instances of the circuit (execution of a	*/
/*		  delta cycle)						*/
/* called func.	: vhx_gexeval, vhx_bddeval, addchain, freeptype		*/
/*		  sch_AddTransaction					*/
/* ###--------------------------------------------------------------### */

int vhx_execute (pt_shdul, flag)

struct shdul *pt_shdul;		/* scheduler				*/
char          flag    ;		/* execute on bdd (1) or gex (0)	*/

  {
  struct ptype   *exec_list   ;
  struct lkdins  *pt_lkdins   ;
  struct beout   *pt_beout    ;
  struct bebus   *pt_bebus    ;
  struct bebux   *pt_bebux    ;
  struct beaux   *pt_beaux    ;
  struct bereg   *pt_bereg    ;
  struct bemsg   *pt_bemsg    ;
  struct wrssig  *pt_wrssig   ;
  struct wrbsig  *pt_wrbsig   ;
  struct wrireg  *pt_wrireg   ;
  struct wriaux  *pt_wriaux   ;
  struct wribux  *pt_wribux   ;
  struct wrimsg  *pt_wrimsg   ;
  struct binode  *pt_binode   ;
  struct prjbvl  *pt_prjbvl   ;
  struct prjrvl  *pt_prjrvl   ;
  struct simsig  *pt_simsig   ;
  struct bussig  *pt_bussig   ;
  void          (* pt_func) ();
  unsigned int    tra_time    ;
  int             ret_flg     = 1;
  char            resval      ;
  char            cndval      ;
  char            drvval      ;

	/* ###------------------------------------------------------### */
	/*    execute all the object in the execute list		*/
	/* ###------------------------------------------------------### */

  if ((exec_list = VHX_EXECUTE) != NULL)
    ret_flg = 0;

  while (exec_list != NULL)
    {
    switch (exec_list->TYPE)
      {
      case 'I' :

	/* ###------------------------------------------------------### */
	/*    for each lkdins that is to be executed ...		*/
	/* ###------------------------------------------------------### */

        pt_lkdins = (struct lkdins *) exec_list->DATA;

	/* ###------------------------------------------------------### */
	/*    for instances that have C models execute the C function	*/
	/* ###------------------------------------------------------### */

        if (pt_lkdins->FLAG == VHL_CMODEL)
          {
          pt_func = (void (*) ()) pt_lkdins->BEFIG;
          (void) (* pt_func) (pt_lkdins);
          }

	/* ###------------------------------------------------------### */
	/*    for instances that have VHDL models ...			*/
	/* ###------------------------------------------------------### */

        else
          {
	/* ###------------------------------------------------------### */
	/*    Computes projected value of WRSSIGs			*/
	/* ###------------------------------------------------------### */

          pt_wrssig = pt_lkdins->WRSSIG;
          while (pt_wrssig != NULL)
            {
            pt_beout = pt_wrssig->BEOUT;

            if (flag == 1)
              resval = vhx_bddeval (pt_lkdins, pt_beout->NODE);
            else
              resval = vhx_gexeval (pt_lkdins, pt_beout->NODE);

            pt_simsig         = pt_wrssig->SIMSIG;

            sch_AddTransaction (pt_shdul      , pt_simsig, pt_simsig,
                                pt_beout->TIME, '1'      , resval   ,
                                (unsigned int) 'S'       , VHX_SCH_TRA_MODE);

            pt_wrssig = pt_wrssig->NEXT;
            }

	/* ###------------------------------------------------------### */
	/*    Computes projected value of WRIAUXs			*/
	/* ###------------------------------------------------------### */

          pt_wriaux = pt_lkdins->WRIAUX;
          while (pt_wriaux != NULL)
            {
            pt_beaux = pt_wriaux->BEAUX;

            if (flag == 1)
              resval = vhx_bddeval (pt_lkdins, pt_beaux->NODE);
            else
              resval = vhx_gexeval (pt_lkdins, pt_beaux->NODE);

            sch_AddTransaction (pt_shdul      , pt_wriaux, pt_wriaux,
                                pt_beaux->TIME, '1'      , resval   ,
                                (unsigned int) 'X'       , VHX_SCH_TRA_MODE);

            pt_wriaux = pt_wriaux->NEXT;
            }

	/* ###------------------------------------------------------### */
	/*    Computes projected value of WRBSIGs			*/
	/* ###------------------------------------------------------### */

          pt_wrbsig = pt_lkdins->WRBSIG;
          while (pt_wrbsig != NULL)
            {
            pt_bebus  = pt_wrbsig->BEBUS;
            pt_prjbvl = pt_wrbsig->PRJBVL;
            pt_bussig = pt_wrbsig->BUSSIG;
            pt_binode = pt_bebus->BINODE;

            while (pt_binode != NULL)
              {
              if (flag == 1)
                cndval = vhx_bddeval (pt_lkdins, pt_binode->CNDNODE);
              else
                cndval = vhx_gexeval (pt_lkdins, pt_binode->CNDNODE);

              if (cndval == '1')
                {
                tra_time = pt_binode->TIME;
                if (flag == 1)
                  drvval = vhx_bddeval (pt_lkdins, pt_binode->VALNODE);
                else
                  drvval = vhx_gexeval (pt_lkdins, pt_binode->VALNODE);
                }
              else
                {
                tra_time = pt_binode->TIME;
                drvval   = '0';
                }

              sch_AddTransaction (pt_shdul, pt_prjbvl, pt_bussig,
                                  tra_time, cndval   , drvval   ,
                                  (unsigned int) 'B' , VHX_SCH_TRA_MODE);

              pt_prjbvl = pt_prjbvl->NEXT;
              pt_binode = pt_binode->NEXT;
              }

            pt_wrbsig  = pt_wrbsig->NEXT;
            }

	/* ###------------------------------------------------------### */
	/*    Computes projected value of WRIREGs			*/
	/* ###------------------------------------------------------### */

          pt_wrireg = pt_lkdins->WRIREG;
          while (pt_wrireg != NULL)
            {
            pt_bereg  = pt_wrireg->BEREG ;
            pt_prjrvl = pt_wrireg->PRJRVL;
            pt_binode = pt_bereg->BINODE ;
            while (pt_binode != NULL)
              {
              if (flag == 1)
                cndval = vhx_bddeval (pt_lkdins, pt_binode->CNDNODE);
              else
                cndval = vhx_gexeval (pt_lkdins, pt_binode->CNDNODE);

              if (cndval == '1')
                {
                tra_time = pt_binode->TIME;
                if (flag == 1)
                  drvval = vhx_bddeval (pt_lkdins, pt_binode->VALNODE);
                else
                  drvval = vhx_gexeval (pt_lkdins, pt_binode->VALNODE);
                }
              else
                {
                tra_time = pt_binode->TIME;
                drvval   = '0';
                }

              sch_AddTransaction (pt_shdul, pt_prjrvl, pt_wrireg,
                                  tra_time, cndval   , drvval   ,
                                  (unsigned int) 'R' , VHX_SCH_TRA_MODE);

              pt_prjrvl = pt_prjrvl->NEXT;
              pt_binode = pt_binode->NEXT;
              }

            pt_wrireg  = pt_wrireg->NEXT;
            }

	/* ###------------------------------------------------------### */
	/*    Compute projected value of WRIBUXs			*/
	/* ###------------------------------------------------------### */

          pt_wribux = pt_lkdins->WRIBUX;
          while (pt_wribux != NULL)
            {
            pt_bebux  = pt_wribux->BEBUX ;
            pt_prjbvl = pt_wribux->PRJBVL;
            pt_binode = pt_bebux->BINODE ;
            while (pt_binode != NULL)
              {
              if (flag == 1)
                cndval = vhx_bddeval (pt_lkdins, pt_binode->CNDNODE);
              else
                cndval = vhx_gexeval (pt_lkdins, pt_binode->CNDNODE);

              if (cndval == '1')
                {
                tra_time = pt_binode->TIME;
                if (flag == 1)
                  drvval = vhx_bddeval (pt_lkdins, pt_binode->VALNODE);
                else
                  drvval = vhx_gexeval (pt_lkdins, pt_binode->VALNODE);
                }
              else
                {
                tra_time = pt_binode->TIME;
                drvval   = '0';
                }

              sch_AddTransaction (pt_shdul, pt_prjbvl, pt_wribux,
                                  tra_time, cndval   , drvval   ,
                                  (unsigned int) 'U' , VHX_SCH_TRA_MODE);

              pt_prjbvl = pt_prjbvl->NEXT;
              pt_binode = pt_binode->NEXT;
              }

            pt_wribux  = pt_wribux->NEXT;
            }

	/* ###------------------------------------------------------### */
	/*    Compute BEMSG's condition					*/
	/* ###------------------------------------------------------### */

          pt_wrimsg = pt_lkdins->WRIMSG;
          while (pt_wrimsg != NULL)
            {
            pt_bemsg = pt_wrimsg->BEMSG;

            if (flag == 1)
              resval = vhx_bddeval (pt_lkdins, pt_bemsg->NODE);
            else
              resval = vhx_gexeval (pt_lkdins, pt_bemsg->NODE);

            if (resval == '0')
              {
              if (pt_wrimsg->ERRFLG == 0)
                {
                pt_wrimsg->ERRFLG = 1;
                VHX_MSGERR ++;
                }
              }
            else
              {
              if (pt_wrimsg->ERRFLG == 1)
                {
                pt_wrimsg->ERRFLG = 0;
                VHX_MSGERR --;
                }
              }

            pt_wrimsg = pt_wrimsg->NEXT;
            }
          }
        pt_lkdins->EVAL = 0;
        break;

	/* ###------------------------------------------------------### */
	/*    for each simple output port that is to be executed ...	*/
	/* ###------------------------------------------------------### */

      case 'S' :
        pt_wrssig = (struct wrssig *) exec_list->DATA;

        pt_lkdins = pt_wrssig->LKDINS;
        pt_beout  = pt_wrssig->BEOUT ;

        if (flag == 1)
          resval = vhx_bddeval (pt_lkdins, pt_beout->NODE);
        else
          resval = vhx_gexeval (pt_lkdins, pt_beout->NODE);

        pt_wrssig->EVAL = 0;
        pt_simsig       = pt_wrssig->SIMSIG;

        sch_AddTransaction (pt_shdul      , pt_simsig, pt_simsig,
                            pt_beout->TIME, '1'      , resval   ,
                            (unsigned int) 'S'       , VHX_SCH_TRA_MODE);

        break;

	/* ###------------------------------------------------------### */
	/*    for each bussed output port that is to be executed ...	*/
	/* ###------------------------------------------------------### */

      case 'B' :
        pt_wrbsig = (struct wrbsig *) exec_list->DATA;

        pt_lkdins = pt_wrbsig->LKDINS;
        pt_bebus  = pt_wrbsig->BEBUS ;
        pt_bussig = pt_wrbsig->BUSSIG;
        pt_prjbvl = pt_wrbsig->PRJBVL;

        pt_binode = pt_bebus->BINODE ;
        while (pt_binode != NULL)
          {
          if (flag == 1)
            cndval = vhx_bddeval (pt_lkdins, pt_binode->CNDNODE);
          else
            cndval = vhx_gexeval (pt_lkdins, pt_binode->CNDNODE);

          if (cndval == '1')
            {
            tra_time = pt_binode->TIME;
            if (flag == 1)
              drvval = vhx_bddeval (pt_lkdins, pt_binode->VALNODE);
            else
              drvval = vhx_gexeval (pt_lkdins, pt_binode->VALNODE);
            }
          else
            {
            tra_time = pt_binode->TIME;
            drvval   = '0';
            }

          sch_AddTransaction (pt_shdul, pt_prjbvl, pt_bussig,
                              tra_time, cndval   , drvval   ,
                              (unsigned int) 'B' , VHX_SCH_TRA_MODE);

          pt_prjbvl = pt_prjbvl->NEXT;
          pt_binode = pt_binode->NEXT;
          }
        pt_wrbsig->EVAL = 0;
        break;

	/* ###------------------------------------------------------### */
	/*    for each internal register that is to be executed ...	*/
	/* ###------------------------------------------------------### */

      case 'R' :
        pt_wrireg = (struct wrireg *) exec_list->DATA;

        pt_lkdins = pt_wrireg->LKDINS;
        pt_bereg  = pt_wrireg->BEREG ;
        pt_prjrvl = pt_wrireg->PRJRVL;
        pt_binode = pt_bereg->BINODE ;
        while (pt_binode != NULL)
          {
          if (flag == 1)
            cndval = vhx_bddeval (pt_lkdins, pt_binode->CNDNODE);
          else
            cndval = vhx_gexeval (pt_lkdins, pt_binode->CNDNODE);

          if (cndval == '1')
            {
            tra_time = pt_binode->TIME;
            if (flag == 1)
              drvval = vhx_bddeval (pt_lkdins, pt_binode->VALNODE);
            else
              drvval = vhx_gexeval (pt_lkdins, pt_binode->VALNODE);
            }
          else
            {
            tra_time = pt_binode->TIME;
            drvval   = '0';
            }

          sch_AddTransaction (pt_shdul, pt_prjrvl, pt_wrireg,
                              tra_time, cndval   , drvval   ,
                              (unsigned int) 'R' , VHX_SCH_TRA_MODE);

          pt_prjrvl = pt_prjrvl->NEXT;
          pt_binode = pt_binode->NEXT;
          }
        pt_wrireg->EVAL = 0;
        break;

	/* ###------------------------------------------------------### */
	/*    for each bussed internal signal that is to be executed ...*/
	/* ###------------------------------------------------------### */

      case 'U' :
        pt_wribux = (struct wribux *) exec_list->DATA;

        pt_lkdins = pt_wribux->LKDINS;
        pt_bebux  = pt_wribux->BEBUX ;
        pt_prjbvl = pt_wribux->PRJBVL;
        pt_binode = pt_bebux->BINODE ;
        while (pt_binode != NULL)
          {
          if (flag == 1)
            cndval = vhx_bddeval (pt_lkdins, pt_binode->CNDNODE);
          else
            cndval = vhx_gexeval (pt_lkdins, pt_binode->CNDNODE);

          if (cndval == '1')
            {
            tra_time = pt_binode->TIME;
            if (flag == 1)
              drvval = vhx_bddeval (pt_lkdins, pt_binode->VALNODE);
            else
              drvval = vhx_gexeval (pt_lkdins, pt_binode->VALNODE);
            }
          else
            {
            tra_time = pt_binode->TIME;
            drvval   = '0';
            }

          sch_AddTransaction (pt_shdul, pt_prjbvl, pt_wribux,
                              tra_time, cndval   , drvval   ,
                              (unsigned int) 'U' , VHX_SCH_TRA_MODE);

          pt_prjbvl = pt_prjbvl->NEXT;
          pt_binode = pt_binode->NEXT;
          }

        pt_wribux->EVAL   = 0;
        break;

	/* ###------------------------------------------------------### */
	/*    for each simple output port that is to be executed ...	*/
	/* ###------------------------------------------------------### */

      case 'D' :
      case 'X' :
        pt_wriaux = (struct wriaux *) exec_list->DATA;

        pt_lkdins = pt_wriaux->LKDINS;
        pt_beaux  = pt_wriaux->BEAUX ;

        if (flag == 1)
          resval = vhx_bddeval (pt_lkdins, pt_beaux->NODE);
        else
          resval = vhx_gexeval (pt_lkdins, pt_beaux->NODE);

        pt_wriaux->EVAL   = 0;

        sch_AddTransaction (pt_shdul      , pt_wriaux, pt_wriaux,
                            pt_beaux->TIME, '1'      , resval   ,
                            (unsigned int) 'X'       , VHX_SCH_TRA_MODE);

        break;

	/* ###------------------------------------------------------### */
	/*    for each simple output port that is to be executed ...	*/
	/* ###------------------------------------------------------### */

      case 'M' :
        pt_wrimsg = (struct wrimsg *) exec_list->DATA;

        pt_lkdins = pt_wrimsg->LKDINS;
        pt_bemsg  = pt_wrimsg->BEMSG ;

        if (flag == 1)
          resval = vhx_bddeval (pt_lkdins, pt_bemsg->NODE);
        else
          resval = vhx_gexeval (pt_lkdins, pt_bemsg->NODE);

        if (resval == '0')
          {
          if (pt_wrimsg->ERRFLG == 0)
            {
            pt_wrimsg->ERRFLG = 1;
            VHX_MSGERR ++;
            }
          }
        else
          {
          if (pt_wrimsg->ERRFLG == 1)
            {
            pt_wrimsg->ERRFLG = 0;
            VHX_MSGERR --;
            }
          }

        pt_wrimsg->EVAL = 0;
        break;
      }

    exec_list = exec_list ->NEXT;
    }

  if (ret_flg == 0)
    {
    freeptype (VHX_EXECUTE);
    VHX_EXECUTE = NULL;
    }

  return (ret_flg);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_redtra						*/
/* description	: read the next transactions				*/
/* called func.	: sch_GotoNextTime    , sch_GetCurrentTransactions,	*/
/*		  sch_FreeTransactions,					*/
/* ###--------------------------------------------------------------### */

void vhx_redtra (pt_shtra)

struct shtra *pt_shtra;

  {
  struct shtra  *pt_trans ;
  struct simsig *pt_simsig;
  struct bussig *pt_bussig;
  struct wriaux *pt_wriaux;
  struct wribux *pt_wribux;
  struct wrireg *pt_wrireg;
  struct prjbvl *pt_prjbvl;
  struct prjrvl *pt_prjrvl;

  pt_trans = pt_shtra;

  while (pt_trans != NULL)
    {
    switch (pt_trans->UFLAG)
      {
      case 'S':
        pt_simsig         = (struct simsig *) pt_trans->IDENT;
        pt_simsig->PRJVAL = pt_trans->VALU;
        VHX_SIGUPD        = addchain (VHX_SIGUPD, pt_simsig);
        break;

      case 'B':
        pt_prjbvl         = (struct prjbvl *) pt_trans->IDENT;
        pt_bussig         = (struct bussig *) pt_trans->SCDID;
        pt_prjbvl->CNDVAL = pt_trans->COND;
        pt_prjbvl->DRVVAL = pt_trans->VALU;
        VHX_BUSUPD        = addchain (VHX_BUSUPD, pt_bussig);
        break;

      case 'X':
        pt_wriaux         = (struct wriaux *) pt_trans->IDENT;
        pt_wriaux->PRJVAL = pt_trans->VALU;
        VHX_AUXUPD        = addchain (VHX_AUXUPD, pt_wriaux);
        break;

      case 'U':
        pt_prjbvl         = (struct prjbvl *) pt_trans->IDENT;
        pt_wribux         = (struct wribux *) pt_trans->SCDID;
        pt_prjbvl->CNDVAL = pt_trans->COND;
        pt_prjbvl->DRVVAL = pt_trans->VALU;
        VHX_BUXUPD        = addchain (VHX_BUXUPD, pt_wribux);
        break;

      case 'R':
        pt_prjrvl         = (struct prjrvl *) pt_trans->IDENT;
        pt_wrireg         = (struct wrireg *) pt_trans->SCDID;
        pt_prjrvl->CNDVAL = pt_trans->COND;
        pt_prjrvl->DRVVAL = pt_trans->VALU;
        VHX_REGUPD        = addchain (VHX_REGUPD, pt_wrireg);
        break;
      }
    pt_trans = pt_trans->NEXT;
    }

  sch_FreeTransactions (pt_shtra);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhx_supdate						*/
/* description	: updates the current value of signals computed in the	*/
/*		  last evaluation cycle					*/
/* called func.	: vhx_muxbit, vhx_worbit, vhx_regbit, addchain,		*/
/*		  freechain						*/
/* ###--------------------------------------------------------------### */

void vhx_supdate (dict, cur_date)

struct beden   **dict    ;
unsigned int     cur_date;

  {
  struct chain  *cur_chain;
  struct wrireg *pt_wrireg;
  struct wriaux *pt_wriaux;
  struct wribux *pt_wribux;
  struct bussig *pt_bussig;
  struct simsig *pt_simsig;
  unsigned int   count    = 0;
  unsigned int   l_tim    = 0;    /*  Statistics last_time access  */
  char           resval   ;

	/* ###------------------------------------------------------### */
	/*    update simple signals' value				*/
	/* ###------------------------------------------------------### */

  cur_chain = VHX_SIGUPD;
  while (cur_chain != NULL)
    {
    pt_simsig = (struct simsig *) cur_chain->DATA;

    if (*(pt_simsig->CURVAL) == '1')
      {
      l_tim  = (unsigned int) beh_chktab (dict, pt_simsig, CTXLST, VHL_PNTDFN);
      count  = (unsigned int) beh_chktab (dict, pt_simsig, CTXPRB, VHL_PNTDFN);
      count += cur_date - l_tim;
/*printf ("sig: %s, sum: %u\n", pt_simsig->NAMECHAIN->DATA, count);*/
      beh_addtab (dict, pt_simsig, CTXPRB, VHL_PNTDFN, count);
      }

    if (pt_simsig->PRJVAL == '1')
      beh_addtab (dict, pt_simsig, CTXLST, VHL_PNTDFN, cur_date);

    if (pt_simsig->PRJVAL != *(pt_simsig->CURVAL))
      {
      count = (unsigned int) beh_chktab (dict, pt_simsig, CTXEVT, VHL_PNTDFN);
      beh_addtab (dict, pt_simsig, CTXEVT, VHL_PNTDFN, count+1);

      *(pt_simsig->CURVAL) = pt_simsig->PRJVAL;
      vhx_statexec (pt_simsig->DEPEND, dict);
      vhx_addexec (pt_simsig->DEPEND);
      }
 
    cur_chain = cur_chain->NEXT;
    }
  freechain (VHX_SIGUPD);
  VHX_SIGUPD = NULL;

	/* ###------------------------------------------------------### */
	/*    update bused signals' value (also handles errors occured	*/
	/* when resolution functions are called)			*/
	/* ###------------------------------------------------------### */

  cur_chain = VHX_BUSUPD;
  while (cur_chain != NULL)
    {
    pt_bussig = (struct bussig *) cur_chain->DATA;
    switch (pt_bussig->TYPE)
      {
      case 'W' : resval = vhx_worbit (pt_bussig->PRJBVL); break;
      case 'M' : resval = vhx_muxbit (pt_bussig->PRJBVL); break;
      default :
        vhu_toolbug (22, "vhx_supdate", pt_bussig->NAMECHAIN->DATA,
                     pt_bussig->TYPE);
        break;
      }

    if (resval == 'E')
      {
      if (pt_bussig->ERRFLG == 0)
        {
        pt_bussig->ERRFLG = 1;
        VHX_BUSERR ++;
        resval = 'u';
        }
      }
    else
      {
      if (pt_bussig->ERRFLG == 1)
        {
        pt_bussig->ERRFLG = 0;
        VHX_BUSERR --;
        }
      }

    if (resval != 'E')
      {
      if (*(pt_bussig->CURVAL) == '1')
        {
        l_tim  = (unsigned int) beh_chktab (dict, pt_bussig, CTXLST, VHL_PNTDFN);
        count  = (unsigned int) beh_chktab (dict, pt_bussig, CTXPRB, VHL_PNTDFN);
        count += cur_date - l_tim;
/*printf ("sig: %s, sum: %u\n", pt_bussig->NAMECHAIN->DATA, count);*/
        beh_addtab (dict, pt_bussig, CTXPRB, VHL_PNTDFN, count);
        }

      if (resval == '1')
        beh_addtab (dict, pt_bussig, CTXLST, VHL_PNTDFN, cur_date);

      if (*(pt_bussig->CURVAL) != resval)
        {
        count = (unsigned int) beh_chktab (dict, pt_bussig, CTXEVT, VHL_PNTDFN);
        beh_addtab (dict, pt_bussig, CTXEVT, VHL_PNTDFN, count+1);

        *(pt_bussig->CURVAL) = resval;
        vhx_statexec (pt_bussig->DEPEND, dict);
        vhx_addexec (pt_bussig->DEPEND);
        }
      }

    cur_chain = cur_chain->NEXT;
    }
  freechain (VHX_BUSUPD);
  VHX_BUSUPD = NULL;


	/* ###------------------------------------------------------### */
	/*    update internal bussed signals's value (also handles	*/
	/* errors occured when resolution functions are called)		*/
	/* ###------------------------------------------------------### */

  cur_chain = VHX_BUXUPD;
  while (cur_chain != NULL)
    {
    pt_wribux = (struct wribux *) cur_chain->DATA;
    switch (pt_wribux->BEBUX->TYPE)
      {
      case 'W' : resval = vhx_worbit (pt_wribux->PRJBVL); break;
      case 'M' : resval = vhx_muxbit (pt_wribux->PRJBVL); break;
      default :
        vhu_toolbug (22, "vhx_supdate", pt_wribux->BEBUX->NAME,
                     pt_wribux->BEBUX->TYPE);
        break;
      }

    if (resval == 'E')
      {
      if (pt_wribux->ERRFLG == 0)
        {
        pt_wribux->ERRFLG = 1;
        VHX_BUXERR ++;
        resval = 'u';
        }
      }
    else
      {
      if (pt_wribux->ERRFLG == 1)
        {
        pt_wribux->ERRFLG = 0;
        VHX_BUXERR --;
        }
      }

    if (resval != 'E')
      {
      if (*(pt_wribux->CURVAL) == '1')
        {
        l_tim  = (unsigned int) beh_chktab (dict, pt_wribux, CTXLST, VHL_PNTDFN);
        count  = (unsigned int) beh_chktab (dict, pt_wribux, CTXPRB, VHL_PNTDFN);
        count += cur_date - l_tim;
/*printf ("sig: %s, sum: %u\n", pt_wribux->BEBUX->NAME, count);*/
        beh_addtab (dict, pt_wribux, CTXPRB, VHL_PNTDFN, count);
        }

      if (resval == '1')
        beh_addtab (dict, pt_wribux, CTXLST, VHL_PNTDFN, cur_date);

      if (*(pt_wribux->CURVAL) != resval)
        {
        count = (unsigned int) beh_chktab (dict, pt_wribux, CTXEVT, VHL_PNTDFN);
        beh_addtab (dict, pt_wribux, CTXEVT, VHL_PNTDFN, count+1);

        *(pt_wribux->CURVAL) = resval;
        vhx_statexec (pt_wribux->DEPEND, dict);
        vhx_addexec (pt_wribux->DEPEND);
        }
      }

    cur_chain = cur_chain->NEXT;
    }
  freechain (VHX_BUXUPD);
  VHX_BUXUPD = NULL;

	/* ###------------------------------------------------------### */
	/*    updating registers (also handles errors occured when	*/
	/* resolution functions are called)				*/
	/* ###------------------------------------------------------### */

  cur_chain = VHX_REGUPD;
  while (cur_chain != NULL)
    {
    pt_wrireg = (struct wrireg *)cur_chain->DATA;
    resval = vhx_regbit (pt_wrireg->PRJRVL);

    if (resval != 'Z')
      {
      if (resval == 'E')
        {
        if (pt_wrireg->ERRFLG == 0)
          {
          pt_wrireg->ERRFLG = 1;
          VHX_REGERR ++;
          resval = 'u';
          }
        }
      else
        {
        if (pt_wrireg->ERRFLG == 1)
          {
          pt_wrireg->ERRFLG = 0;
          VHX_REGERR --;
          }
        }

      if (resval != 'E')
        {
        if (*(pt_wrireg->CURVAL) == '1')
          {
          l_tim  = (unsigned int)beh_chktab(dict, pt_wrireg, CTXLST, VHL_PNTDFN);
          count  = (unsigned int)beh_chktab(dict, pt_wrireg, CTXPRB, VHL_PNTDFN);
          count += cur_date - l_tim;
/*printf ("sig: %s, sum: %u\n", pt_wrireg->BEREG->NAME, count);*/
          beh_addtab (dict, pt_wrireg, CTXPRB, VHL_PNTDFN, count);
          }

        if (resval == '1')
          beh_addtab (dict, pt_wrireg, CTXLST, VHL_PNTDFN, cur_date);

        if (*(pt_wrireg->CURVAL) != resval)
          {
          count = (unsigned int) beh_chktab(dict, pt_wrireg, CTXEVT, VHL_PNTDFN);
          beh_addtab (dict, pt_wrireg, CTXEVT, VHL_PNTDFN, count+1);

          *(pt_wrireg->CURVAL) = resval;
          vhx_statexec (pt_wrireg->DEPEND, dict);
          vhx_addexec (pt_wrireg->DEPEND);
          }
        }
      }
    cur_chain = cur_chain->NEXT;
    }
  freechain (VHX_REGUPD);
  VHX_REGUPD = NULL;


	/* ###------------------------------------------------------### */
	/*    updating simple internal signals				*/
	/* ###------------------------------------------------------### */

  cur_chain = VHX_AUXUPD;
  while (cur_chain != NULL)
    {
    pt_wriaux = (struct wriaux *)cur_chain->DATA;

    if (*(pt_wriaux->CURVAL) == '1')
      {
      l_tim  = (unsigned int) beh_chktab (dict, pt_wriaux, CTXLST, VHL_PNTDFN);
      count  = (unsigned int) beh_chktab (dict, pt_wriaux, CTXPRB, VHL_PNTDFN);
      count += cur_date - l_tim;
/*printf ("sig: %s, sum: %u\n", pt_wriaux->BEAUX->NAME, count);*/
      beh_addtab (dict, pt_wriaux, CTXPRB, VHL_PNTDFN, count);
      }

    if (pt_wriaux->PRJVAL == '1')
      beh_addtab (dict, pt_wriaux, CTXLST, VHL_PNTDFN, cur_date);

    if (pt_wriaux->PRJVAL != *(pt_wriaux->CURVAL))
      {
      count = (unsigned int) beh_chktab (dict, pt_wriaux, CTXEVT, VHL_PNTDFN);
      beh_addtab (dict, pt_wriaux, CTXEVT, VHL_PNTDFN, count+1);

      *(pt_wriaux->CURVAL) = pt_wriaux->PRJVAL;
      vhx_statexec (pt_wriaux->DEPEND, dict);
      vhx_addexec (pt_wriaux->DEPEND);
      }

    cur_chain = cur_chain->NEXT;
    }
  freechain (VHX_AUXUPD);
  VHX_AUXUPD = NULL;

  }


/* ###--------------------------------------------------------------### */
/* function	: st_getstat						*/
/* description	: return a the statistic info about a signal	        */
/* called func.	: beh_chktab						*/
/* ###--------------------------------------------------------------### */

static unsigned int st_getstat(dict, key, ctx, date, value)

struct dct_entry **dict;
void              *key;
void              *ctx;
unsigned int       date;
unsigned char     value;

  {
  unsigned int     l_tim = 0;    /*  Statistics last_time access  */
  unsigned int       res = 0;

  switch((unsigned long)ctx)
    {
    case CTXPRB :
      if (value == '1')
        {
        l_tim  = (unsigned int) beh_chktab (dict, key, CTXLST, VHL_PNTDFN);
        res    = (unsigned int) beh_chktab (dict, key, CTXPRB, VHL_PNTDFN);
        res   += date - l_tim;
        }
      else
        res    = (unsigned int) beh_chktab (dict, key, CTXPRB, VHL_PNTDFN);
      break;

    case CTXEVT :
      res    = (unsigned int) beh_chktab (dict, key, CTXEVT, VHL_PNTDFN);
      break;

    default :
      fprintf(stderr, "Probleme dans st_getstat, contexte non prevu\n\n");
      break;
    }
  return(res);
  }

/* ###--------------------------------------------------------------### */
/* function	: st_wrtsta						*/
/* description	: output the statistics of each signal		        */
/* called func.	: beh_chktab						*/
/* ###--------------------------------------------------------------### */

void st_wrtsta(dict, pt_lkdfig, pt_paseq, num_cycle, filename)

struct dct_entry **dict;
struct lkdfig     *pt_lkdfig;
struct paseq      *pt_paseq;
unsigned int       num_cycle;
char              *filename;

  {
  FILE          *fp;
  char          *signame;
  char           printname[256];
  char           tmp[512];
  time_t         clock;
  unsigned int   num_evt = 0;
  unsigned int   num_prb = 0;
  int            found = 0;
  unsigned int   i = 0;
  struct simsig *pt_simsig;
  struct bussig *pt_bussig;
  struct lkdins *pt_lkdins;
  struct wriaux *pt_wriaux;
  struct wribux *pt_wribux;
  struct wrireg *pt_wrireg;
  struct paiol  *pt_paiol;

  if((fp = mbkfopen(filename, "sta", WRITE_TEXT)) == NULL)
    {
    sprintf(signame, "%s.sta", filename);
    exit(1);
    }

  time(&clock);

  (void) fprintf(fp, "\n\n# Core file generated from %s\n", pt_lkdfig->NAME);
  (void) fprintf(fp, "#\t\t date : %s\n\n", ctime(&clock));
  (void) fprintf(fp, "# Figure : %s == \n\n", pt_lkdfig->NAME);

    /* ###------------------------------------------------------### */
    /*    count the statistics of the simple signal                 */
    /* ###------------------------------------------------------### */

  i = 0;

  (void)fprintf(fp, "\t# -- Simple Signals --\n\n");
  pt_simsig = pt_lkdfig->SIMSIG;
  while (pt_simsig != NULL)
    {
    signame = (char *)getsigname(pt_simsig);
    num_evt = VHX_VECTOR_TRANS[i];
    num_prb = VHX_VECTOR_PROBA[i];
    pt_paiol = pt_paseq->PAIOL;
    while (pt_paiol != NULL)
      {
      if (pt_paiol->NAME == signame)
	{
	found = 1;
	break;
        }
      pt_paiol = pt_paiol->NEXT;
      }
    if (found == 1)
      {
      if (vectorindex(pt_paiol->NAME) == -1)
        {
        sprintf (printname, "%s", vectorradical(pt_paiol->NAME));
        }

      else
        {
        sprintf (printname, "%s(%d)", vectorradical(pt_paiol->NAME),
                                      vectorindex(pt_paiol->NAME));
        }

      switch(pt_paiol->MODE)
	{
	case 'I' :
	  (void) fprintf(fp, "in     %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb) / (float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
	  break;
	case 'O' :
	  (void) fprintf(fp, "out    %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb) / (float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
	  break;
	case 'B' :
	  (void) fprintf(fp, "inout  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb) / (float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
	  break;
        case 'W' :
          (void) fprintf(fp, "signal %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb) / (float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
	  break;
        } 
      }
    else
      {
      (void) fprintf(fp, "unknown  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb) / (float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
      } 
    found = 0;

    i++;
    pt_simsig = pt_simsig->NEXT;
    }
  (void)fprintf(fp,"\n\n");

    /* ###------------------------------------------------------### */
    /*    count the statistics of the bused signal                  */
    /* ###------------------------------------------------------### */

  (void)fprintf(fp, "\t# -- Buses Signals --\n\n");
  pt_bussig = pt_lkdfig->BUSSIG;
  while (pt_bussig != NULL)
    {
    signame = (char *)getsigname(pt_bussig);
    num_evt = VHX_VECTOR_TRANS[i];
    num_prb = VHX_VECTOR_PROBA[i];

    pt_paiol = pt_paseq->PAIOL;
    while (pt_paiol != NULL)
      {
      if (pt_paiol->NAME == signame)
	{
	found = 1;
	break;
        }
      pt_paiol = pt_paiol->NEXT;
      }
    if (found == 1)
      {
      if (vectorindex(pt_paiol->NAME) == -1)
        {
        sprintf (printname, "%s", vectorradical(pt_paiol->NAME));
        }

      else
        {
        sprintf (printname, "%s(%d)", vectorradical(pt_paiol->NAME),
                                      vectorindex(pt_paiol->NAME));
        }

      switch(pt_paiol->MODE)
	{
	case 'Z' :
	  (void) fprintf(fp, "out  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
	  break;
	case 'T' :
	  (void) fprintf(fp, "inout  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
	  break;
	}
      }
    else
      {
      (void) fprintf(fp, "unknown  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
      }
    found = 0;

    i++;
    pt_bussig = pt_bussig->NEXT;
    }
  (void)fprintf(fp,"\n\n");


    /* ###------------------------------------------------------### */
    /*    for each of the instance in lkdfig                        */
    /* ###------------------------------------------------------### */

  pt_lkdins = pt_lkdfig->LKDINS;
  while (pt_lkdins != NULL)
    {
    (void)fprintf(fp, "# -- Instance : %s --\n\n", pt_lkdins->INSNAME);
    pt_wriaux = pt_lkdins->WRIAUX;

    if (pt_wriaux != NULL)
      (void)fprintf(fp, "\t# -- Internal Simple Signals --\n\n");
    while (pt_wriaux != NULL)
      {
 
    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal simple signal               */
    /* ###-------------------------------------------------------------### */

      if (pt_wriaux->FLAG != 'D')
        {
        sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wriaux->BEAUX->NAME);
        signame = namealloc(tmp);
        num_evt = VHX_VECTOR_TRANS[i];
        num_prb = VHX_VECTOR_PROBA[i];

        (void) fprintf(fp, "signal %-25s : p('1') = %-6.4f | density = %6.4f ;\n", signame, (float)(num_prb)/(float)(num_cycle), (float)(num_evt) / (float)(num_cycle));
        }

      i++;
      pt_wriaux = pt_wriaux->NEXT;
      }
    (void)fprintf(fp, "\n");

    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal bused signal                */
    /* ###-------------------------------------------------------------### */

    pt_wribux = pt_lkdins->WRIBUX;
    if ( pt_wribux != NULL) 
      (void)fprintf(fp, "\t# -- Internal Bused Signals --\n\n");
    while (pt_wribux != NULL)
      {
      sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wribux->BEBUX->NAME);
      signame = namealloc(tmp);
      num_evt = VHX_VECTOR_TRANS[i];
      num_prb = VHX_VECTOR_PROBA[i];

      (void) fprintf(fp, "tristate %-25s : p('1') = %-6.4f | density = %6.4f ;\n", signame, (float)(num_prb)/(float)(num_cycle), (float)(num_evt) / (float)(num_cycle));

      i++;
      pt_wribux = pt_wribux->NEXT;
      }
    (void)fprintf(fp, "\n");

    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal register                    */
    /* ###-------------------------------------------------------------### */

    pt_wrireg = pt_lkdins->WRIREG;
    if (pt_wrireg != NULL)
      (void)fprintf(fp, "\t# -- Internal Registers -- \n\n");
    while (pt_wrireg != NULL)
      {
      sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wrireg->BEREG->NAME);
      signame = namealloc(tmp);
      num_evt = VHX_VECTOR_TRANS[i];
      num_prb = VHX_VECTOR_PROBA[i];

      (void) fprintf(fp, "register %-25s : p('1') = %-6.4f | density = %6.4f ;\n", signame, (float)(num_prb)/(float)(num_cycle), (float)(num_evt) / (float)(num_cycle));

      i++;
      pt_wrireg = pt_wrireg->NEXT;
      }
    (void)fprintf(fp, "\n");

    pt_lkdins = pt_lkdins->NEXT;
    (void)fprintf(fp, "\n\n");
    }

  fclose(fp);
  }

/* ###--------------------------------------------------------------### */
/* function	: st_wrtstadly						*/
/* description	: output the statistics of each signal		        */
/* called func.	: beh_chktab						*/
/* ###--------------------------------------------------------------### */

void st_wrtstadly(dict, pt_lkdfig, pt_paseq, num_time, ref_time, filename)

struct dct_entry **dict;
struct lkdfig     *pt_lkdfig;
struct paseq      *pt_paseq;
unsigned int       num_time;
unsigned int       ref_time;
char              *filename;

  {
  FILE          *fp;
  char          *signame;
  char           dlyfilename[256];
  char           printname[256];
  char           tmp[512];
  time_t         clock;
  unsigned int   num_evt = 0;
  unsigned int   num_prb = 0;
  unsigned int   num_cycle = 0;
  int            found = 0;
  struct simsig *pt_simsig;
  struct bussig *pt_bussig;
  struct lkdins *pt_lkdins;
  struct wriaux *pt_wriaux;
  struct wribux *pt_wribux;
  struct wrireg *pt_wrireg;
  struct paiol  *pt_paiol;

  sprintf(dlyfilename, "%s.dly", filename);

  if((fp = mbkfopen(dlyfilename, "sta", WRITE_TEXT)) == NULL)
    {
    exit(1);
    }

  time(&clock);

  (void) fprintf(fp, "\n\n# Core file generated from %s\n", pt_lkdfig->NAME);
  (void) fprintf(fp, "#\t\t date : %s\n\n", ctime(&clock));
  (void) fprintf(fp, "# Figure : %s == \n\n", pt_lkdfig->NAME);

    /* ###------------------------------------------------------### */
    /*    count the statistics of the simple signal                 */
    /* ###------------------------------------------------------### */

  num_cycle = num_time / ref_time;

  (void)fprintf(fp, "\t# -- Simple Signals --\n\n");
  pt_simsig = pt_lkdfig->SIMSIG;
  while (pt_simsig != NULL)
    {
    signame = (char *)getsigname(pt_simsig);
    num_evt = st_getstat(dict, pt_simsig, CTXEVT, num_time, *(pt_simsig->CURVAL));
    num_prb = st_getstat(dict, pt_simsig, CTXPRB, num_time, *(pt_simsig->CURVAL));
    pt_paiol = pt_paseq->PAIOL;
    while (pt_paiol != NULL)
      {
      if (pt_paiol->NAME == signame)
	{
	found = 1;
	break;
        }
      pt_paiol = pt_paiol->NEXT;
      }
    if (found == 1)
      {
      if (vectorindex(pt_paiol->NAME) == -1)
        {
        sprintf (printname, "%s", vectorradical(pt_paiol->NAME));
        }

      else
        {
        sprintf (printname, "%s(%d)", vectorradical(pt_paiol->NAME),
                                      vectorindex(pt_paiol->NAME));
        }

      switch(pt_paiol->MODE)
	{
	case 'I' :
	  (void) fprintf(fp, "in     %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
	  break;
	case 'O' :
	  (void) fprintf(fp, "out    %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
	  break;
	case 'B' :
	  (void) fprintf(fp, "inout  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
	  break;
        case 'W' :
          (void) fprintf(fp, "signal %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
	  break;
        } 
      }
    else
      {
      (void) fprintf(fp, "unknown  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
      } 
    found = 0;
    pt_simsig = pt_simsig->NEXT;
    }
  (void)fprintf(fp,"\n\n");

    /* ###------------------------------------------------------### */
    /*    count the statistics of the bused signal                  */
    /* ###------------------------------------------------------### */

  (void)fprintf(fp, "\t# -- Buses Signals --\n\n");
  pt_bussig = pt_lkdfig->BUSSIG;
  while (pt_bussig != NULL)
    {
    signame = (char *)getsigname(pt_bussig);
    num_evt = st_getstat(dict, pt_bussig, CTXEVT, num_time, *(pt_bussig->CURVAL));
    num_prb = st_getstat(dict, pt_bussig, CTXPRB, num_time, *(pt_bussig->CURVAL));
    pt_paiol = pt_paseq->PAIOL;
    while (pt_paiol != NULL)
      {
      if (pt_paiol->NAME == signame)
	{
	found = 1;
	break;
        }
      pt_paiol = pt_paiol->NEXT;
      }
    if (found == 1)
      {
      if (vectorindex(pt_paiol->NAME) == -1)
        {
        sprintf (printname, "%s", vectorradical(pt_paiol->NAME));
        }

      else
        {
        sprintf (printname, "%s(%d)", vectorradical(pt_paiol->NAME),
                                      vectorindex(pt_paiol->NAME));
        }

      switch(pt_paiol->MODE)
	{
	case 'Z' :
	  (void) fprintf(fp, "out  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
	  break;
	case 'T' :
	  (void) fprintf(fp, "inout  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
	  break;
	}
      }
    else
      {
      (void) fprintf(fp, "unknown  %-25s : p('1') = %-6.4f | density = %6.4f ;\n", printname, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
      }
    found = 0;
    pt_bussig = pt_bussig->NEXT;
    }
  (void)fprintf(fp,"\n\n");


    /* ###------------------------------------------------------### */
    /*    for each of the instance in lkdfig                        */
    /* ###------------------------------------------------------### */

  pt_lkdins = pt_lkdfig->LKDINS;
  while (pt_lkdins != NULL)
    {
    (void)fprintf(fp, "# -- Instance : %s --\n\n", pt_lkdins->INSNAME);
    pt_wriaux = pt_lkdins->WRIAUX;

    if (pt_wriaux != NULL)
      (void)fprintf(fp, "\t# -- Internal Simple Signals --\n\n");
    while (pt_wriaux != NULL)
      {
 
    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal simple signal               */
    /* ###-------------------------------------------------------------### */

      if (pt_wriaux->FLAG != 'D')
        {
        sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wriaux->BEAUX->NAME);
        signame = namealloc(tmp);
        num_evt = st_getstat(dict, pt_wriaux, CTXEVT, num_time, *(pt_wriaux->CURVAL));
        num_prb = st_getstat(dict, pt_wriaux, CTXPRB, num_time, *(pt_wriaux->CURVAL));
        (void) fprintf(fp, "signal %-25s : p('1') = %-6.4f | density = %6.4f ;\n", signame, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
        }
      pt_wriaux = pt_wriaux->NEXT;
      }
    (void)fprintf(fp, "\n");

    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal bused signal                */
    /* ###-------------------------------------------------------------### */

    pt_wribux = pt_lkdins->WRIBUX;
    if ( pt_wribux != NULL) 
      (void)fprintf(fp, "\t# -- Internal Bused Signals --\n\n");
    while (pt_wribux != NULL)
      {
      sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wribux->BEBUX->NAME);
      signame = namealloc(tmp);
      num_evt = st_getstat(dict, pt_wribux, CTXEVT, num_time, *(pt_wribux->CURVAL));
      num_prb = st_getstat(dict, pt_wribux, CTXPRB, num_time, *(pt_wribux->CURVAL));
      (void) fprintf(fp, "tristate %-25s : p('1') = %-6.4f | density = %6.4f ;\n", signame, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
      pt_wribux = pt_wribux->NEXT;
      }
    (void)fprintf(fp, "\n");

    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal register                    */
    /* ###-------------------------------------------------------------### */

    pt_wrireg = pt_lkdins->WRIREG;
    if (pt_wrireg != NULL)
      (void)fprintf(fp, "\t# -- Internal Registers -- \n\n");
    while (pt_wrireg != NULL)
      {
      sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wrireg->BEREG->NAME);
      signame = namealloc(tmp);
      num_evt = st_getstat(dict, pt_wrireg, CTXEVT, num_time, *(pt_wrireg->CURVAL));
      num_prb = st_getstat(dict, pt_wrireg, CTXPRB, num_time, *(pt_wrireg->CURVAL));
      (void) fprintf(fp, "register %-25s : p('1') = %-6.4f | density = %6.4f ;\n", signame, (float)(num_prb)/(float)(num_time), (float)(num_evt) / (float)(num_cycle));
      pt_wrireg = pt_wrireg->NEXT;
      }
    (void)fprintf(fp, "\n");

    pt_lkdins = pt_lkdins->NEXT;
    (void)fprintf(fp, "\n\n");
    }

  fclose(fp);
  }

/* ###--------------------------------------------------------------### */
/* function	: st_wrtstabis						*/
/* description	: output the statistics of each signal		        */
/* called func.	: beh_chktab						*/
/* ###--------------------------------------------------------------### */

void st_wrtstabis(dict, pt_lkdfig, pt_paseq, num_time, ref_time, filename)

struct dct_entry **dict;
struct lkdfig     *pt_lkdfig;
struct paseq      *pt_paseq;
unsigned int       num_time;
unsigned int       ref_time;
char              *filename;

  {
  FILE          *fp;
  char          *signame;
  char           tmp[512];
  time_t         clock;
  unsigned int   num_sev = 0;
  unsigned int   num_mev = 0;
  unsigned int   num_cycle = 0;
  int            found = 0;
  struct simsig *pt_simsig;
  struct bussig *pt_bussig;
  struct lkdins *pt_lkdins;
  struct wriaux *pt_wriaux;
  struct wribux *pt_wribux;
  struct wrireg *pt_wrireg;
  struct paiol  *pt_paiol;

  if((fp = mbkfopen(filename, "stb", WRITE_TEXT)) == NULL)
    {
    sprintf(signame, "%s.stb", filename);
    exit(1);
    }

  time(&clock);

  (void) fprintf(fp, "\n\n# Core file generated from %s\n", pt_lkdfig->NAME);
  (void) fprintf(fp, "#\t\t date : %s\n\n", ctime(&clock));
  (void) fprintf(fp, "# Figure : %s == \n\n", pt_lkdfig->NAME);

    /* ###------------------------------------------------------### */
    /*    count the statistics of the simple signal                 */
    /* ###------------------------------------------------------### */

  num_cycle = num_time / ref_time;

  (void)fprintf(fp, "\t# -- Simple Signals --\n\n");
  pt_simsig = pt_lkdfig->SIMSIG;
  while (pt_simsig != NULL)
    {
    signame = (char *)getsigname(pt_simsig);
    num_sev = (unsigned int) beh_chktab(dict, pt_simsig, CTXSEV, VHL_PNTDFN);
    num_mev = (unsigned int) beh_chktab(dict, pt_simsig, CTXMEV, VHL_PNTDFN);

    fprintf(fp, "%-25s : p(simple eval) = %-6.4f | p(multi eval) = %6.4f ;\n",
            signame,
            (float)(num_sev)/(float)(num_cycle),
            (float)(num_mev)/(float)(num_cycle));

    pt_simsig = pt_simsig->NEXT;
    }
  (void)fprintf(fp,"\n\n");

    /* ###------------------------------------------------------### */
    /*    count the statistics of the bused signal                  */
    /* ###------------------------------------------------------### */

  (void)fprintf(fp, "\t# -- Buses Signals --\n\n");
  pt_bussig = pt_lkdfig->BUSSIG;
  while (pt_bussig != NULL)
    {
    signame = (char *)getsigname(pt_bussig);
    num_sev = (unsigned int) beh_chktab(dict, pt_bussig, CTXSEV, VHL_PNTDFN);
    num_mev = (unsigned int) beh_chktab(dict, pt_bussig, CTXMEV, VHL_PNTDFN);

    fprintf(fp, "%-25s : p(simple eval) = %-6.4f | p(multi eval) = %6.4f ;\n",
            signame,
            (float)(num_sev)/(float)(num_cycle),
            (float)(num_mev)/(float)(num_cycle));

    pt_bussig = pt_bussig->NEXT;
    }
  (void)fprintf(fp,"\n\n");


    /* ###------------------------------------------------------### */
    /*    for each of the instance in lkdfig                        */
    /* ###------------------------------------------------------### */

  pt_lkdins = pt_lkdfig->LKDINS;
  while (pt_lkdins != NULL)
    {
    (void)fprintf(fp, "# -- Instance : %s --\n\n", pt_lkdins->INSNAME);
    pt_wriaux = pt_lkdins->WRIAUX;

    if (pt_wriaux != NULL)
      (void)fprintf(fp, "\t# -- Internal Simple Signals --\n\n");
    while (pt_wriaux != NULL)
      {
 
    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal simple signal               */
    /* ###-------------------------------------------------------------### */

      sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wriaux->BEAUX->NAME);
      signame = namealloc(tmp);
      num_sev = (unsigned int) beh_chktab(dict, pt_wriaux, CTXSEV, VHL_PNTDFN);
      num_mev = (unsigned int) beh_chktab(dict, pt_wriaux, CTXMEV, VHL_PNTDFN);

      fprintf(fp, "%-25s : p(simple eval) = %-6.4f | p(multi eval) = %6.4f ;\n",
              signame,
              (float)(num_sev)/(float)(num_cycle),
              (float)(num_mev)/(float)(num_cycle));

      pt_wriaux = pt_wriaux->NEXT;
      }
    (void)fprintf(fp, "\n");

    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal bused signal                */
    /* ###-------------------------------------------------------------### */

    pt_wribux = pt_lkdins->WRIBUX;
    if ( pt_wribux != NULL) 
      (void)fprintf(fp, "\t# -- Internal Bused Signals --\n\n");
    while (pt_wribux != NULL)
      {
      sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wribux->BEBUX->NAME);
      signame = namealloc(tmp);
      num_sev = (unsigned int) beh_chktab(dict, pt_wribux, CTXSEV, VHL_PNTDFN);
      num_mev = (unsigned int) beh_chktab(dict, pt_wribux, CTXMEV, VHL_PNTDFN);

      fprintf(fp, "%-25s : p(simple eval) = %-6.4f | p(multi eval) = %6.4f ;\n",
              signame,
              (float)(num_sev)/(float)(num_cycle),
              (float)(num_mev)/(float)(num_cycle));

      pt_wribux = pt_wribux->NEXT;
      }
    (void)fprintf(fp, "\n");

    /* ###-------------------------------------------------------------### */
    /*    count the statistics of the internal register                    */
    /* ###-------------------------------------------------------------### */

    pt_wrireg = pt_lkdins->WRIREG;
    if (pt_wrireg != NULL)
      (void)fprintf(fp, "\t# -- Internal Registers -- \n\n");
    while (pt_wrireg != NULL)
      {
      sprintf(tmp, "%s.%s", pt_lkdins->INSNAME, pt_wrireg->BEREG->NAME);
      signame = namealloc(tmp);
      num_sev = (unsigned int) beh_chktab(dict, pt_wrireg, CTXSEV, VHL_PNTDFN);
      num_mev = (unsigned int) beh_chktab(dict, pt_wrireg, CTXMEV, VHL_PNTDFN);

      fprintf(fp, "%-25s : p(simple eval) = %-6.4f | p(multi eval) = %6.4f ;\n",
              signame,
              (float)(num_sev)/(float)(num_cycle),
              (float)(num_mev)/(float)(num_cycle));

      pt_wrireg = pt_wrireg->NEXT;
      }
    (void)fprintf(fp, "\n");

    pt_lkdins = pt_lkdins->NEXT;
    (void)fprintf(fp, "\n\n");
    }

  fclose(fp);
  }
