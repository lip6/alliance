
/* ###--------------------------------------------------------------### */
/* file          : vh_debug.h                                           */
/* date          : Aug 20 1997                                          */
/* version       : v3.0                                                 */
/* author        : Pirouz BAZARGAN SABET                                */
/* description   : declaration of diefines, global variables and,       */
/*                 functions used in the debugger                       */
/* ###--------------------------------------------------------------### */

        /* ###------------------------------------------------------### */
        /* defines                                                      */
        /* ###------------------------------------------------------### */

struct stack                              /* debugger's stack's structure */
  {
  short  type;                            /* type of the current ponter   */
  short  mark;                            /* stop mark                    */
  void  *data;                            /* current pointer              */
  };

union value
  {
  void *dat;
  long  imd;
  };

#define STKSIZ_DFN     500                /* debugger's stack size        */

#define KIND_DFN       0xff000000
#define COMMAND_DFN    0x01000000
#define IMMEDIATE_DFN  0x02000000
#define POINTER_DFN    0x03000000
#define ARRAY_DFN      0x04000000
#define ARYOFPNT_DFN   0x05000000

#define FORMAT_DFN     0x00ff0000
#define s_DFN          0x00010000
#define u_DFN          0x00020000
#define d_DFN          0x00030000
#define l_DFN          0x00040000
#define x_DFN          0x00050000
#define c_DFN          0x00060000
#define f_DFN          0x00070000
#define ABL_DFN        0x00080000
#define BDD_DFN        0x00090000
#define GEX_DFN        0x000a0000

#define _error_DFN     0x0000ffff

#define TYPE_DFN       0x0000ffff
#define _back_DFN      0
#define _exit_DFN      (_back_DFN      + 1)
#define _jump_DFN      (_exit_DFN      + 1)
#define _save_DFN      (_jump_DFN      + 1)
#define _stop_DFN      (_save_DFN      + 1)
#define _top_DFN       (_stop_DFN      + 1)
#define _up_DFN        (_top_DFN       + 1)
#define _display_DFN   (_up_DFN        + 1)

#define character_DFN  (_display_DFN   + 1)
#define short_DFN      (character_DFN  + 1)
#define integer_DFN    (short_DFN      + 1)
#define long_DFN       (integer_DFN    + 1)
#define float_DFN      (long_DFN       + 1)
#define void_DFN       (float_DFN      + 1)
#define string_DFN     (void_DFN       + 1)

#define chain_DFN      (string_DFN     + 1)
#define ptype_DFN      (chain_DFN      + 1)

#define lofig_DFN      (ptype_DFN      + 1)
#define locon_DFN      (lofig_DFN      + 1)
#define losig_DFN      (locon_DFN      + 1)
#define loins_DFN      (losig_DFN      + 1)

#define befig_DFN      (loins_DFN      + 1)
#define bereg_DFN      (befig_DFN      + 1)
#define bemsg_DFN      (bereg_DFN      + 1)
#define berin_DFN      (bemsg_DFN      + 1)
#define beout_DFN      (berin_DFN      + 1)
#define bebus_DFN      (beout_DFN      + 1)
#define beaux_DFN      (bebus_DFN      + 1)
#define bedly_DFN      (beaux_DFN      + 1)
#define bebux_DFN      (bedly_DFN      + 1)
#define bepor_DFN      (bebux_DFN      + 1)
#define begen_DFN      (bepor_DFN      + 1)
#define biabl_DFN      (begen_DFN      + 1)
#define binode_DFN     (biabl_DFN      + 1)

#define paseq_DFN      (binode_DFN     + 1)
#define pagrp_DFN      (paseq_DFN      + 1)
#define paiol_DFN      (pagrp_DFN      + 1)
#define papat_DFN      (paiol_DFN      + 1)
#define pacom_DFN      (papat_DFN      + 1)
#define paini_DFN      (pacom_DFN      + 1)
#define paevt_DFN      (paini_DFN      + 1)
#define pains_DFN      (paevt_DFN      + 1)

#define bussig_DFN     (pains_DFN      + 1)
#define lkdfig_DFN     (bussig_DFN     + 1)
#define lkdins_DFN     (lkdfig_DFN     + 1)
#define simsig_DFN     (lkdins_DFN     + 1)
#define wrbsig_DFN     (simsig_DFN     + 1)
#define wriaux_DFN     (wrbsig_DFN     + 1)
#define wribux_DFN     (wriaux_DFN     + 1)
#define wrireg_DFN     (wribux_DFN     + 1)
#define wrssig_DFN     (wrireg_DFN     + 1)
#define wrimsg_DFN     (wrssig_DFN     + 1)

#define shdul_DFN      (wrimsg_DFN     + 1)
#define shent_DFN      (shdul_DFN      + 1)
#define shtra_DFN      (shent_DFN      + 1)
#define shwav_DFN      (shtra_DFN      + 1)

#define abl_DFN        (shwav_DFN      + 1)
#define bdd_DFN        (abl_DFN        + 1)
#define gex_DFN        (bdd_DFN        + 1)

#define actflag_DFN    (gex_DFN        + 1)
#define aux_ref_DFN    (actflag_DFN    + 1)
#define blank_DFN      (aux_ref_DFN    + 1)
#define buffer_DFN     (blank_DFN      + 1)
#define bus_ref_DFN    (buffer_DFN     + 1)
#define bux_ref_DFN    (bus_ref_DFN    + 1)
#define capa_DFN       (bux_ref_DFN    + 1)
#define cndabl_DFN     (capa_DFN       + 1)
#define cndnode_DFN    (cndabl_DFN     + 1)
#define cndval_DFN     (cndnode_DFN    + 1)
#define cond_DFN       (cndval_DFN     + 1)
#define curcom_DFN     (cond_DFN       + 1)
#define curpat_DFN     (curcom_DFN     + 1)
#define curtime_DFN    (curpat_DFN     + 1)
#define curval_DFN     (curtime_DFN    + 1)
#define data_DFN       (curval_DFN     + 1)
#define date_DFN       (data_DFN       + 1)
#define deccom_DFN     (date_DFN       + 1)
#define depend_DFN     (deccom_DFN     + 1)
#define direction_DFN  (depend_DFN     + 1)
#define dly_ref_DFN    (direction_DFN  + 1)
#define drvseq_DFN     (dly_ref_DFN    + 1)
#define drvval_DFN     (drvseq_DFN     + 1)
#define endflg_DFN     (drvval_DFN     + 1)
#define errflg_DFN     (endflg_DFN     + 1)
#define eval_DFN       (errflg_DFN     + 1)
#define figname_DFN    (eval_DFN       + 1)
#define filname_DFN    (figname_DFN    + 1)
#define findex_DFN     (filname_DFN    + 1)
#define flag_DFN       (findex_DFN     + 1)
#define flags_DFN      (flag_DFN       + 1)
#define format_DFN     (flags_DFN      + 1)
#define glnext_DFN     (format_DFN     + 1)
#define ident_DFN      (glnext_DFN     + 1)
#define index_DFN      (ident_DFN      + 1)
#define insname_DFN    (index_DFN      + 1)
#define instance_DFN   (insname_DFN    + 1)
#define iolnbr_DFN     (instance_DFN   + 1)
#define itable_DFN     (iolnbr_DFN     + 1)
#define label_DFN      (itable_DFN     + 1)
#define length_DFN     (label_DFN      + 1)
#define level_DFN      (length_DFN     + 1)
#define line_DFN       (level_DFN      + 1)
#define lineno_DFN     (line_DFN       + 1)
#define message_DFN    (lineno_DFN     + 1)
#define mode_DFN       (message_DFN    + 1)
#define model_DFN      (mode_DFN       + 1)
#define modelchain_DFN (model_DFN      + 1)
#define msg_ref_DFN    (modelchain_DFN + 1)
#define name_DFN       (msg_ref_DFN    + 1)
#define namechain_DFN  (name_DFN       + 1)
#define next_DFN       (namechain_DFN  + 1)
#define node_DFN       (next_DFN       + 1)
#define nxtpat_DFN     (node_DFN       + 1)
#define oldcom_DFN     (nxtpat_DFN     + 1)
#define oldpat_DFN     (oldcom_DFN     + 1)
#define out_ref_DFN    (oldpat_DFN     + 1)
#define patnbr_DFN     (out_ref_DFN    + 1)
#define position_DFN   (patnbr_DFN     + 1)
#define prev_DFN       (position_DFN   + 1)
#define prjbvl_DFN     (prev_DFN       + 1)
#define prjrvl_DFN     (prjbvl_DFN     + 1)
#define prjval_DFN     (prjrvl_DFN     + 1)
#define redins_DFN     (prjval_DFN     + 1)
#define redlst_DFN     (redins_DFN     + 1)
#define reg_ref_DFN    (redlst_DFN     + 1)
#define root_DFN       (reg_ref_DFN    + 1)
#define savflg_DFN     (root_DFN       + 1)
#define scdid_DFN      (savflg_DFN     + 1)
#define sig_DFN        (scdid_DFN      + 1)
#define simflag_DFN    (sig_DFN        + 1)
#define simval_DFN     (simflag_DFN    + 1)
#define subseq_DFN     (simval_DFN     + 1)
#define text_DFN       (subseq_DFN     + 1)
#define time_DFN       (text_DFN       + 1)
#define time_unit_DFN  (time_DFN       + 1)
#define trans_DFN      (time_unit_DFN  + 1)
#define ttable_DFN     (trans_DFN      + 1)
#define type_DFN       (ttable_DFN     + 1)
#define uflag_DFN      (type_DFN       + 1)
#define user_DFN       (uflag_DFN      + 1)
#define usrval_DFN     (user_DFN       + 1)
#define valabl_DFN     (usrval_DFN     + 1)
#define valnode_DFN    (valabl_DFN     + 1)
#define valsiz_DFN     (valnode_DFN    + 1)
#define valu_DFN       (valsiz_DFN     + 1)
#define value_DFN      (valu_DFN       + 1)
#define wave_DFN       (value_DFN      + 1)

#define MAXCMD_DFN     (wave_DFN       + 1)

        /* ###------------------------------------------------------### */
        /* functions                                                    */
        /* ###------------------------------------------------------### */

extern void dast_dbg ();
