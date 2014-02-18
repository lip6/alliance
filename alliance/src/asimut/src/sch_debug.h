
/* ###--------------------------------------------------------------### */
/* file		: sch_debug.h						*/
/* date		: Aug 10 1997						*/
/* version	: v101							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: declaration of diefines, global variables and,	*/
/*		  functions used in the debugger			*/
/* ###--------------------------------------------------------------### */

struct stack				/* debugger's stack's structure	*/
  {
  short  type;				/* type of the current ponter	*/
  short  mark;				/* stop mark			*/
  void  *data;				/* current pointer		*/
  };

union value
  {
  void *dat;
  long  imd;
  };

#define STKSIZ_DFN     500		/* debugger's stack size	*/

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

#define shdul_DFN      (string_DFN     + 1)
#define shent_DFN      (shdul_DFN      + 1)
#define shtra_DFN      (shent_DFN      + 1)
#define shwav_DFN      (shtra_DFN      + 1)

#define cond_DFN       (shwav_DFN      + 1)
#define curtime_DFN    (cond_DFN       + 1)
#define date_DFN       (curtime_DFN    + 1)
#define flags_DFN      (date_DFN       + 1)
#define ident_DFN      (flags_DFN      + 1)
#define itable_DFN     (ident_DFN      + 1)
#define label_DFN      (itable_DFN     + 1)
#define next_DFN       (label_DFN      + 1)
#define prev_DFN       (next_DFN       + 1)
#define scdid_DFN      (prev_DFN       + 1)
#define trans_DFN      (scdid_DFN      + 1)
#define ttable_DFN     (trans_DFN      + 1)
#define uflag_DFN      (ttable_DFN     + 1)
#define valu_DFN       (uflag_DFN      + 1)
#define wave_DFN       (valu_DFN       + 1)

#define MAXCMD_DFN     (wave_DFN       + 1)
