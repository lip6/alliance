
/* ###--------------------------------------------------------------### */
/* file		: vh_simulad.c						*/
/* date		: Dec 13 1999						*/
/* version	: v3.02							*/
/* author	: Huu Nghia VUONG, Pirouz BAZARGAN SABET		*/
/* description	: VHDL simulation tool					*/
/* ###--------------------------------------------------------------### */

#ident "@(#) SIMULAD : SIMULAtion tool with Delays V3.02"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"
#include "bvl.h"
#include "pat.h"
#include "ppt.h"
#include "sch.h"
#include "vh_ltype.h"
#include "vh_globals.h"
#include "vh_util.h"
#include "vh_lspec.h"
#include "vh_xspec.h"
#include "vh_xcomm.h"
#include "vh_simulad.h"
#include "beh_delay.h"

extern char MVL_ERRFLG;

static char *VH_TUNIT_TABLE [] = { "vu", "fs", "ps", "ns", "us", "ms" };


main (argc, argv)

int   argc;
char **argv;

{
  int              i             ;
  int              j             ;
  int              patindx       ;
  char             error_flag    ;
  int              nbr_errlft    ;                    /* how many errors left */
                                                      /* before aborting      */
  unsigned int     max_delta     = VHX_DLTDFN;        /* max # of delta cyc.  */

  char             usage_flg     = 0         ;        /* print usage          */
  char             help_flg      = 0         ;        /* display help         */
  char             init_flg      = 0         ;        /* description's init.  */
  char             core_flg      = 0         ;        /* core file at error   */
  char             sdebug_flg    = 0         ;        /* debug structural     */
  char             bdebug_flg    = 0         ;        /* debug behavioural    */
  char             pdebug_flg    = 0         ;        /* debug pattern        */
  char             ldebug_flg    = 0         ;        /* debug linker         */
  char             cdebug_flg    = 0         ;        /* dbeug cycle by cycle */
  char             ddebug_flg    = 0         ;        /* debug delta by delta */
  char             behav_flg     = 0         ;        /* behav. description   */
  char             nosim_flg     = 0         ;        /* no simulation        */
  char             nores_flg     = 0         ;        /* no result file       */
  char             makbdd_flg    = 0         ;        /* trace bdd making     */
  char             bdd_flg       = 0         ;        /* use bdd or gex       */
  char             xmess_flg     = 0         ;        /* no execution message */
  char             randly_flg    = 0         ;        /* random delays        */
  char             fixdly_flg    = 0         ;        /* fixed  delays        */
  char             defdly_flg    = 0         ;        /* default  delays      */
  char             bckdly_flg    = 0         ;        /* backanotated  delays */
  char             trans_flg     = 0         ;        /* transport delays     */
  char             inert_flg     = 1         ;        /* inertial  delays     */
  char             bufreg_flg    = 0         ;        /* bufreg  delays       */
  char             stat_flg      = 0         ;        /* stat                 */
  char             sav_end       ;                    /* saved end flag       */
  int              max_err       = 10        ;        /* # of simul. errors   */
  unsigned int     max_pat       = 0         ;        /* # of patterns        */
  unsigned int     labelsiz      = 32        ;        /* longest patt. label  */
  char            *arg_flg       = NULL      ;        /* recongnized arg. flg */
  unsigned int     last_opt      = 0         ;        /* last option's index  */
  unsigned int     frst_fil      = 0         ;        /* first file's index   */
  unsigned int     init_val      = 0         ;        /* init. constant       */

  unsigned int     comp_mode     = 0         ;        /* beh compiler mode    */
  struct lofig    *ptlofig       ;
  struct befig    *ptbefig       = NULL      ;
  struct befig    *ptbef         ;
  struct chain    *ptchain       ;
  struct papat    *pt_papat      ;
  struct papat    *lst_papat     ;
  struct paseq    *pt_paseq      ;
  struct pains    *pt_pains      = NULL      ;        /* inspected instances  */
  struct lkdspy   *pt_lkdspy     = NULL      ;        /* inspected signals    */
  struct pains    *tmp_pains     = NULL      ;

  struct shdul    *pt_shdul      ;                    /* scheduler            */
  struct shtra    *pt_shtra      ;                    /* transactions         */
  unsigned int     delay_max     = 0         ;
  unsigned int     next_date     = 0         ;
  unsigned int     cur_date      = 0         ;
  unsigned int     ref_date      = 0         ;
  unsigned int     num_cycle     = 0         ;
  int              sch_empty     = 0         ; 

  char            *str           ;
  char             filename [200];
  char             buffer   [200];
  FILE            *filepnt       ;
  struct beden   **hshtab        ;
  struct beden   **stat_dic      ;
  struct beden   **delay_dic     ;
  unsigned char    delay_mode    = 0x00 ;
  struct paiol    *pt_paiol      ;
  unsigned int     iolnbr = 0    ;

        /* ###------------------------------------------------------### */
        /*    analyse the command line, set option flags and find the   */
        /* first file in the argument list                              */
        /* ###------------------------------------------------------### */

  mbkenv ();

  arg_flg = (char *) vhu_alloc (argc * sizeof (char), VHU_NOALIGN);

  arg_flg [0] = 1;
  for (i=1 ; i<argc ; i++)
    arg_flg [i] = 0;

  for (i=1 ; i<argc ; i++)
    {
    if (arg_flg [i] == 0)
      {
      if (!strcmp (argv [i], "-h"))
        {
        help_flg    = 1;
        arg_flg [i] = 1;
        }

      if ((!strcmp (argv [i], "-randomdelay"))
       || (!strcmp (argv [i], "-rd")))
        {
        randly_flg    = 1;
        delay_mode   |= BEH_DELAY_RANDOM;
        delay_max     = 128;
        arg_flg [i  ] = 1;
        }

      if ((!strcmp (argv [i], "-zerodelay"))
       || (!strcmp (argv [i], "-zd")))
        {
        fixdly_flg    = 1;
        delay_mode   |= BEH_DELAY_FIXED;
        delay_max     = 0;
        arg_flg [i  ] = 1;
        }

      if ((!strcmp (argv [i], "-fixeddelay"))
       || (!strcmp (argv [i], "-fd")))
        {
        fixdly_flg    = 1;
        delay_mode   |= BEH_DELAY_FIXED;
        sscanf (argv [i+1], "%u", &delay_max);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

      if ((!strcmp (argv [i], "-backdelay"))
       || (!strcmp (argv [i], "-bd")))
        {
        bckdly_flg    = i + 2;
        delay_mode   |= BEH_DELAY_BACKANOTATED;
        if (!strcmp (argv [i+1], "max"))
          {
          delay_mode    |= BEH_DELAY_MAX;
          }
        else if (!strcmp (argv [i+1], "min"))
          {
          delay_mode    |= BEH_DELAY_MIN;
          }
        else if (!strcmp (argv [i+1], "typ"))
          {
          delay_mode    |= BEH_DELAY_TYP;
          }
        delay_max     = 128;
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        arg_flg [i+2] = 1;
        }

      if ((!strcmp (argv [i], "-defaultdelay"))
       || (!strcmp (argv [i], "-dd")))
        {
        defdly_flg   = 1;
        delay_mode  |= BEH_DELAY_DEFAULT;
        arg_flg [i]  = 1;
        }

      if (!strcmp (argv [i], "-transport"))
        {
        trans_flg   = 1;
        inert_flg   = 0;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-inertial"))
        {
        inert_flg   = 1;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-bufreg"))
        {
        bufreg_flg  = 1;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-noxmessage"))
        {
        xmess_flg   = 1;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-p"))
        {
        sscanf (argv [i+1], "%u", &max_pat);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

      if (!strcmp (argv [i], "-l"))
        {
        sscanf (argv [i+1], "%u", &labelsiz);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

      if (!strcmp (argv [i], "-t"))
        {
        makbdd_flg  = 1;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-stat"))
        {
        stat_flg      = i + 2;
        sscanf (argv [i+1], "%u", &ref_date);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        arg_flg [i+2] = 1;
        }

      if (!strcmp (argv [i], "-i"))
        {
        init_flg      = i + 1;
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

      if (!strcmp (argv [i], "-core"))
        {
        core_flg      = i + 1;
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

      if (!strcmp (argv [i], "-inspect"))
        {
        str           = namealloc (argv [i+1]);
        pt_pains      = pat_addpains (pt_pains, str, str);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }

        /*    The spy option has been supressed (09 Sep 1999).          */
        /*    Spied signals are now specified in the pattern file.      */

/*---------------
      if (!strcmp (argv [i], "-spy"))
        {
        str           = namealloc (argv [i+1]);
        pt_lkdspy     = vhl_addlkdspy (pt_lkdspy, str);
        arg_flg [i  ] = 1;
        arg_flg [i+1] = 1;
        }
----------------*/

      if (!strcmp (argv [i], "-nores"))
        {
        nores_flg   = 1;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-c"))
        {
        nosim_flg   = 1;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-bdd"))
        {
        bdd_flg     = 1;
        arg_flg [i] = 1;
        }

      if (!strcmp (argv [i], "-b"))
        {
        behav_flg   = 1;
        arg_flg [i] = 1;
        }

      if (!strncmp (argv [i], "-dbg", 4))
        {
        arg_flg [i] = 1;
        for (j=4 ; argv [i][j] != '\0' ; j++)
          {
          switch (argv [i][j])
            {
            case 's':
              sdebug_flg = 1; break;
            case 'b':
              bdebug_flg = 1; break;
            case 'l':
              ldebug_flg = 1; break;
            case 'p':
              pdebug_flg = 1; break;
            case 'c':
              cdebug_flg = 1; break;
            case 'd':
              ddebug_flg = 1; break;
            }
          }
        }
      }
    }

  for (i=0 ; i<argc ; i++)
    {
    if (arg_flg [i] == 1)
      last_opt = i;
    }

  for (frst_fil=0 ; frst_fil < argc ; frst_fil++)
    {
    if (arg_flg [frst_fil] == 0)
      break;
    }

        /* ###------------------------------------------------------### */
        /* cases of error in the command line:                          */
        /*   - no argument                                              */
        /*   - argument unrecognized as an option                       */
        /*   - number of files do not match:                            */
        /*       - simulation with result    => 3 files needed          */
        /*       - simulation without result => 2 files needed          */
        /*       - no simulation             => 1 file  needed          */
        /* ###------------------------------------------------------### */

  if (help_flg == 1)
    {
    vhu_message (0, NULL, 0);
    system      ("man asimut");
    exit (0);
    }

  if ((argc == 1) || (last_opt != (frst_fil - 1)))
    usage_flg = 1;

  if (((nosim_flg == 0) && (nores_flg == 0) && (argc != (frst_fil + 3))) ||
      ((nosim_flg == 0) && (nores_flg == 1) && (argc != (frst_fil + 2))) ||
      ((nosim_flg == 1) &&                     (argc != (frst_fil + 1))))
    usage_flg = 1;

  if ((inert_flg == 1) && (trans_flg == 1))
    usage_flg = 1;

  if (((bckdly_flg != 0) && (fixdly_flg != 0)) ||
      ((randly_flg != 0) && (fixdly_flg != 0)) ||
      ((bckdly_flg != 0) && (randly_flg != 0)))
    usage_flg = 1;

  if ((bckdly_flg != 0) && ((delay_mode & BEH_DELAY_MAX) != BEH_DELAY_MAX)
                        && ((delay_mode & BEH_DELAY_MIN) != BEH_DELAY_MIN)
                        && ((delay_mode & BEH_DELAY_TYP) != BEH_DELAY_TYP))
    usage_flg = 1;

  if (usage_flg == 1)
    {
    vhu_message (100, NULL, 0);
    vhu_message (101, NULL, 0);
    exit (1);
    }

  alliancebanner ("ASimuT", vhu_avers (), "A SIMUlation Tool",
                  "1991...1999", ALLIANCE_VERSION);

  vhu_message (21, NULL, 0);

        /* ###------------------------------------------------------### */
        /*   initialize :                                               */
        /*     - get MBK environment variables                          */
        /*     - initialize C models' pointer                           */
        /*     - create a scheduler                                     */
        /*     - initialize BDD hash tables                             */
        /*     - initialize the linker's hash table                     */
        /*     - set the fast mode on MBK functions                     */
        /*     - set the behavioural compiler's mode                    */
        /*     - set BEH_TIMEUNIT to BEH_TU__PS                         */
        /*     - set PAT_TIMEUNIT to PAT_TU__PS                         */
        /* ###------------------------------------------------------### */

  vhu_message (2, NULL, 0);

  vhi_models    ();
  initializeBdd (0);
  hshtab    = beh_initab ();
  stat_dic  = beh_initab ();
  pt_shdul  = sch_CreateScheduler (NULL, NULL);
  VHX_SHDUL = pt_shdul;

  BEH_TIMEUNIT = BEH_TU__PS;
  PAT_TIMEUNIT = PAT_TU__PS;

  FAST_MODE    = 'Y';
  comp_mode    = BVL_TRACE | BVL_KEEPAUX;

  if (trans_flg == 1)
    {
    VHX_SCH_TRA_MODE = SCH_TRA_DELAFTER ;
    }

  if (inert_flg == 1)
    {
    VHX_SCH_TRA_MODE = SCH_TRA_DELBEFORE | SCH_TRA_DELAFTER ;
    }

        /* ###------------------------------------------------------### */
        /*   read the maximum number of simulation errors from the      */
        /* environment variable VH_MAXERR                               */
        /* ###------------------------------------------------------### */

  str = mbkgetenv ("VH_MAXERR");
  if (str != NULL)
    {
    if (sscanf (str, "%u", &max_err) == 0)
      max_err = 10;
    }

        /* ###------------------------------------------------------### */
        /*   searching the root file                                    */
        /* ###------------------------------------------------------### */

  vhu_message (5, argv [frst_fil], 0);
  
        /* ###------------------------------------------------------### */
        /*   if there is no structural level call directly the          */
        /* behaviour compiler                                           */
        /* ###------------------------------------------------------### */

  if (behav_flg == 1)
    {

        /* ###------------------------------------------------------### */
        /*   run the behavioural compiler on the current file. Call     */
        /* the debugger is necessary                                    */
        /* ###------------------------------------------------------### */

    if (vhl_iscmodel (argv [frst_fil]) == NULL)
      comp_mode = BVL_TRACE | BVL_KEEPAUX ;
    else
      comp_mode = BVL_TRACE | BVL_KEEPAUX | BVL_CHECKEMPTY;

    ptbefig    = vhdlloadbefig (ptbefig, argv [frst_fil], comp_mode);
    error_flag = ptbefig->ERRFLG;

    if (bdebug_flg == 1)
      dast_dbg (ptbefig, "befig");

        /* ###------------------------------------------------------### */
        /*   if the architecture of the compiled description is VHDL    */
        /* and if there is no error in the description :                */
        /*                                                              */
        /*    - identify dependencies in the description                */
        /*    - transform boolean expression into BDDs or GEXs          */
        /* ###------------------------------------------------------### */

    if ((ptbefig->ERRFLG == 0) && (ptbefig->FLAG & BEH_ARC_VHDL))
      {
      delay_dic = beh_initab ();

      if (bckdly_flg != 0)
        {
        error_flag += beh_backdelay (delay_dic, argv [bckdly_flg], delay_mode);
        }

      if ((bckdly_flg != 0) || (fixdly_flg != 0) || (randly_flg != 0))
        {
        beh_setdelay (ptbefig, delay_max, delay_mode, delay_dic);
        }

      if (bufreg_flg != 0)
        {
        beh_chkregdly (ptbefig, 2);
        }
      else
        {
        beh_chkregdly (ptbefig, 1);
        }

      beh_fretab (delay_dic);

      beh_depend (ptbefig);

      if (bdd_flg == 1)
        {
        vhu_message (15, "BDD", 0);
        beh_makbdd  (ptbefig);
        }
      else
        {
        vhu_message (15, "GEX", 0);
        beh_makgex  (ptbefig);
        }

      beh_freabl (ptbefig);
      error_flag += ptbefig->ERRFLG;
      }

    ptlofig = vhl_makgst (ptbefig);

    }
  else
    {
        /* ###------------------------------------------------------### */
        /*   run the structural compiler on the root file and call the  */
        /* debugger if it has been asked. Flatten the root description  */
        /* and call the structural compiler as many times as necessary  */
        /* to reach leaf cells (behavioural description)                */
        /* ###------------------------------------------------------### */

    vhu_message (4, argv [frst_fil], 0);
    
    VHS_LOFPNT = getlofig (argv [frst_fil], 'A');
    error_flag = MVL_ERRFLG;

    if (sdebug_flg == 1)
      dast_dbg (HEAD_LOFIG, "lofig");

    if (error_flag != 0)
      {
      error_flag = vhu_error (200, NULL, NULL);
      exit (1);
      }

    vhu_message   (18, NULL, 0);
    lofigchain (VHS_LOFPNT);
    rflattenlofig (VHS_LOFPNT, YES, YES);

    if (sdebug_flg == 1)
      dast_dbg (HEAD_LOFIG, "lofig");

    ptchain = VHS_LOFPNT->MODELCHAIN;
    while (ptchain != NULL)
      {

        /* ###------------------------------------------------------### */
        /*   for each leaf cell of the description search the           */
        /* behavioural file                                             */
        /* ###------------------------------------------------------### */

      vhu_message (5, ptchain->DATA, 0);

        /* ###------------------------------------------------------### */
        /*   run the behavioural compiler on the current file           */
        /* ###------------------------------------------------------### */

      ptbef = ptbefig;

      if (vhl_iscmodel ((char *)ptchain->DATA) == NULL)
        comp_mode = BVL_TRACE | BVL_KEEPAUX ;
      else
        comp_mode = BVL_TRACE | BVL_KEEPAUX | BVL_CHECKEMPTY;

      ptbefig        = vhdlloadbefig (ptbefig, ptchain->DATA, comp_mode);
      ptbefig->NEXT  = ptbef;
      error_flag    += ptbefig->ERRFLG;

        /* ###------------------------------------------------------### */
        /*   if the architecture of the compiled description is VHDL    */
        /* and if there is no error in the description ;                */
        /*                                                              */
        /*    - identify dependencies in the description                */
        /*    - transform boolean expression into BDDs or GEXs          */
        /* ###------------------------------------------------------### */

      if ((error_flag == 0) && (ptbefig->FLAG & BEH_ARC_VHDL))
        {
        delay_dic = beh_initab ();

        if (bckdly_flg != 0)
          {
          error_flag += beh_backdelay (delay_dic, ptbefig->NAME, delay_mode);
          }

        if ((bckdly_flg != 0) || (fixdly_flg) || (randly_flg))
          {
          beh_setdelay (ptbefig, delay_max, delay_mode, delay_dic);
          }

      if (bufreg_flg != 0)
        {
        beh_chkregdly (ptbefig, 2);
        }
      else
        {
        beh_chkregdly (ptbefig, 1);
        }

        beh_fretab (delay_dic);

        beh_depend (ptbefig);

        if (bdd_flg == 1)
          {
          vhu_message (15, "BDD", 0);
          beh_makbdd  (ptbefig);
          }
        else
          {
          vhu_message (15, "GEX", 0);
          beh_makgex  (ptbefig);
          }

        beh_freabl (ptbefig);
        error_flag += ptbefig->ERRFLG;
        }
      ptchain = ptchain->NEXT;
      }
    }

        /* ###------------------------------------------------------### */
        /*   call the debugger if necessary                             */
        /* ###------------------------------------------------------### */

  if (bdebug_flg == 1)
    dast_dbg (ptbefig, "befig");

  if (error_flag != 0)
    {
    error_flag = vhu_error (200, NULL, NULL);
    exit (1);
    }

  if (nosim_flg == 1)
    exit (0);

        /* ###------------------------------------------------------### */
        /*   running the pattern description compiler on the current    */
        /* file                                                         */
        /* ###------------------------------------------------------### */

  vhu_message (6, argv [frst_fil + 1], 0);

  pt_paseq = pat_lodpaseq (argv [frst_fil+1], NULL, max_pat, 0);

  if ((pt_paseq == NULL) || (pt_paseq->ERRFLG != 0))
    {
    error_flag = vhu_error (200, NULL, NULL);
    exit (1);
    }

        /* ###------------------------------------------------------### */
        /*    add the spied signals specified in the pattern file to    */
        /* the list of spy : pt_lkdspy.                                 */
        /* ###------------------------------------------------------### */

  for (pt_paiol = pt_paseq->PAIOL, iolnbr = 0; pt_paiol !=NULL; pt_paiol = pt_paiol->NEXT,iolnbr++)
    {
    if (pt_paiol->FLAG == PAT_IOL__SPY)
      {
      pt_lkdspy = vhl_addlkdspy (pt_lkdspy, pt_paiol, iolnbr);
      }
    }

  vhx_commentspytable(pt_paseq,pt_lkdspy);

        /* ###------------------------------------------------------### */
        /*    add the list of inspected instances passed as argument to */
        /* the list of inspected instances found in the pattern file    */
        /* ###------------------------------------------------------### */

  if (pt_pains != NULL)
    {
    tmp_pains = pt_pains;
    while (tmp_pains->NEXT != NULL)
      tmp_pains = tmp_pains->NEXT;

    tmp_pains->NEXT = pt_paseq->PAINS;
    pt_paseq->PAINS = pt_pains;
    }

  if (pdebug_flg == 1)
    dast_dbg (pt_paseq, "paseq");

        /* ###------------------------------------------------------### */
        /*   link previous structures to create the simulator's data    */
        /* base structures                                              */
        /* ###------------------------------------------------------### */

  vhu_message (16, NULL, 0);

  if (behav_flg == 0)
    vhl_resord (hshtab, HEAD_LOFIG, ptbefig);

  if (sdebug_flg == 1)
    dast_dbg (HEAD_LOFIG, "lofig");

  error_flag = vhl_restor (hshtab, HEAD_LOFIG, ptbefig);

  if (sdebug_flg == 1)
    dast_dbg (HEAD_LOFIG, "lofig");

  if (error_flag != 0)
    {
    error_flag = vhu_error (200, NULL, NULL);
    exit (1);
    }

  vhu_message (9, NULL, 0);

  error_flag = vhl_linker (hshtab, pt_paseq);

  if (error_flag != 0)
    {
    error_flag = vhu_error (200, NULL, NULL);
    exit (1);
    }

        /* ###------------------------------------------------------### */
        /*   initialize the circuit with a .sav file or a constant.     */
        /* ###------------------------------------------------------### */

  if (init_flg != 0)
    {
    if ((sscanf (argv [init_flg], "%u", &init_val) == 1) && (init_val <= 1))
      vhx_initval (VHL_HEDLKF, init_val + '0');
    else
      {
      if ((filepnt = mbkfopen (argv [init_flg], "sav", READ_TEXT)) == NULL)
        vhu_error (116, argv [init_flg], NULL);
      else
        {
        error_flag = vhx_initfig (VHL_HEDLKF, filepnt);
        fclose (filepnt);
        }
      }
    }
  else
    vhx_initval (VHL_HEDLKF, 'u');

  if (ldebug_flg == 1)
    dast_dbg (VHL_HEDLKF, "lkdfig");

  if (pdebug_flg == 1)
    dast_dbg (pt_paseq, "paseq");

  if (error_flag != 0)
    {
    error_flag = vhu_error (200, NULL, NULL);
    exit (1);
    }

        /* ###------------------------------------------------------### */
        /*   execute the current figure                                 */
        /* ###------------------------------------------------------### */

  vhu_message (11, NULL, 0);

  vhx_execall (VHL_HEDLKF);
  patindx = 0;

  nbr_errlft = max_err;

  while (1)
    {
    pt_papat = pt_paseq->CURPAT;

    while ((pt_papat   != NULL) && (pt_papat != pt_paseq->NXTPAT) &&
           (nbr_errlft  >  0  ))
      {
      max_delta = VHX_DLTDFN;

        /* ###------------------------------------------------------### */
        /*   read a new pattern                                         */
        /* ###------------------------------------------------------### */

      if ((sch_empty == 0) || (pt_papat->TIME <= next_date))
        {
        if ((sch_empty != 0) && (cdebug_flg == 1))
          {
          vhu_warning (5, NULL, NULL);
          }

        cur_date = pt_papat->TIME;
        sch_SetCurrentTime (pt_shdul, cur_date);

        if (xmess_flg == 0)
          {
        /* ###------------------------------------------------------### */
        /*   Are we in virtual time unit or real time unit ?            */
        /* ###------------------------------------------------------### */

          if (pt_paseq->TIME_UNIT == PAT_TU__VU)
            {
            vhu_message (10, NULL, patindx);
            }
          else
            {
            sprintf (buffer,"%u %s", cur_date, VH_TUNIT_TABLE [PAT_TIMEUNIT]);
            vhu_message (22, buffer, patindx);
            }
          }

        if (pt_papat->ACTFLAG == 'I')
          {
          vhx_inireg (hshtab, pt_papat->PAINI);
          }

        vhx_redpat (pt_paseq, pt_papat);
        }

      if ((sch_empty != 0) && (next_date <= pt_papat->TIME))
        {
        cur_date = next_date;
        sch_SetCurrentTime (pt_shdul, cur_date);

        pt_shtra = sch_GetCurrentTransactions (pt_shdul);
        vhx_redtra (pt_shtra);
        }

      while (1)
        {
        vhx_supdate (stat_dic, cur_date);
        vhx_execute (pt_shdul, bdd_flg );

        sch_empty = sch_GetNextTime (pt_shdul, &next_date);

        if ((sch_empty != 0) && (next_date == cur_date))
          {
          pt_shtra = sch_GetCurrentTransactions (pt_shdul);
          vhx_redtra (pt_shtra);
          }
        else
          break;
        }

      if (max_delta <= 0)
        break;


      if (pt_papat->TIME == cur_date)
        {
        if (stat_flg != 0)
          {
          if (cur_date == 0)
            {
            num_cycle++;
            vhx_vectorinit ( VHL_HEDLKF );
            }
         
          else if ((cur_date  % ref_date) == 0)
            {
            num_cycle++;
            vhx_vectorstat ( VHL_HEDLKF );
            }
          }

        nbr_errlft -= vhx_wrtpat (pt_paseq, pt_papat);
        lst_papat    = pt_papat;
        pt_papat    = pt_papat->NEXT;
        patindx ++;
        }

        /* ###------------------------------------------------------### */
        /*    When pattern time < simulation time,                      */
        /*    insert one pattern if a spy event occurs.                 */
        /* ###------------------------------------------------------### */

      else
        {
        lst_papat = vhx_insertspypat (pt_paseq, lst_papat, pt_lkdspy, cur_date, labelsiz);
        }
        /* ###------------------------------------------------------### */
        /*    If a fatal error has been occured during the simulation   */
        /* (Assert Violation, Bus conflict), set the end flag of the    */
        /* pattern sequence to avoid loading new patterns and break the */
        /* simulation cycle                                             */
        /*                                                              */
        /*   Otherwies, continue the simulation cycle                   */
        /* ###------------------------------------------------------### */

      if (vhx_chkerr (cur_date) == 1)
        {
        pt_paseq->ENDFLG = 'Y';
        break;
        }

      }

        /* ###------------------------------------------------------### */
        /*    After all the patterns of a set have been processed ...   */
        /* ###------------------------------------------------------### */

    if (pdebug_flg == 1)
      dast_dbg (pt_paseq, "paseq");

    if (nores_flg == 0)
      pat_savpaseq (argv [frst_fil + 2], pt_paseq, labelsiz);

    if (max_delta <= 0)
      break;

    if ((pt_paseq->ENDFLG != 'Y') && (pt_paseq->ERRFLG == 0))
      {
      pt_paseq = pat_lodpaseq (argv [frst_fil+1], pt_paseq, max_pat, 0);
      if (pdebug_flg == 1)
        dast_dbg (pt_paseq, "paseq");
      }
    else
      break;
    }

  if (max_delta <= 0)
    error_flag = vhu_error (105, NULL, NULL);

  if (nbr_errlft < 0)
    error_flag = vhu_error (106, NULL, NULL);
  else
    {
    if (stat_flg != 0)
      {
      st_wrtsta (stat_dic, VHL_HEDLKF, pt_paseq, num_cycle, argv [stat_flg]);
      st_wrtstadly (stat_dic, VHL_HEDLKF, pt_paseq, cur_date, ref_date, argv [stat_flg]);
      st_wrtstabis (stat_dic, VHL_HEDLKF, pt_paseq, cur_date, ref_date, argv [stat_flg]);
      }
    }

        /* ###------------------------------------------------------### */
        /*    if the save flag is set create a save file                */
        /* ###------------------------------------------------------### */

  if (nores_flg == 0)
    {
    if (pt_paseq->SAVFLG == 'Y')
      {
      if ((filepnt = mbkfopen (argv [frst_fil+1], "sav", WRITE_TEXT)) == NULL)
        {
        sprintf (filename, "%s.sav", argv [frst_fil + 1]);
        error_flag = vhu_error (116, filename, NULL);
        exit (1);
        }
      vhx_savall (filepnt, VHL_HEDLKF);
      fclose (filepnt);
      }
    }
  
  beh_fretab (hshtab);
  beh_fretab (stat_dic);

  if (nbr_errlft < max_err)
    exit (2);
  else
    exit (0);
}
