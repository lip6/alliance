/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_shared.h                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef VASY_SHARED_H
# define VASY_SHARED_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Process Flags                     |
|                                                             |
\------------------------------------------------------------*/

# define VASY_PROC_TO_ANALYSE_MASK 0x1

/*------------------------------------------------------------\
|                                                             |
|                           Process Type                      |
|                                                             |
\------------------------------------------------------------*/

# define VASY_PROC_UNKNOWN         0
# define VASY_PROC_UNUSED          1
# define VASY_PROC_DATA_FLOW       2
# define VASY_PROC_FSM             3
# define VASY_PROC_FSM_OUTREG      4
# define VASY_MAX_VPN_PROC_TYPE    5

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Declar Type Mask                 |
|                                                             |
\------------------------------------------------------------*/

# define VASY_RTL_DECL_COMBINATORIAL   0x0100
# define VASY_RTL_DECL_TRISTATE        0x0200
# define VASY_RTL_DECL_PULL_UP         0x0400
# define VASY_RTL_DECL_PULL_DOWN       0x0800
# define VASY_RTL_DECL_REGISTER        0x1000

# define VASY_RTL_VERILOG_REG_MASK     0x2000

/*------------------------------------------------------------\
|                                                             |
|                           Symbol Type                       |
|                                                             |
\------------------------------------------------------------*/

# define VASY_SYM_UNKNOWN         0
# define VASY_SYM_UNUSED          1
# define VASY_SYM_CONSTANT        2
# define VASY_SYM_COMBINATORIAL   3
# define VASY_SYM_TRISTATE        4
# define VASY_SYM_PULL_UP         5
# define VASY_SYM_PULL_DOWN       6
# define VASY_SYM_REGISTER        7

# define VASY_MAX_SYM_TYPE        8

/*------------------------------------------------------------\
|                                                             |
|                        Symbol Register Type                 |
|                                                             |
\------------------------------------------------------------*/

# define VASY_SYM_REGISTER_NONE      0
# define VASY_SYM_REGISTER_SYNC      1
# define VASY_SYM_REGISTER_ASYNC     2
# define VASY_SYM_REGISTER_MIXED     3

# define VASY_MAX_SYM_REGISTER TYPE  4

/*------------------------------------------------------------\
|                                                             |
|                        Bi Vex Type                          |
|                                                             |
\------------------------------------------------------------*/

# define VASY_BIVEX_FALLING_EDGE  RTL_BIVEX_FALLING_EDGE 
# define VASY_BIVEX_RISING_EDGE   RTL_BIVEX_RISING_EDGE  
# define VASY_BIVEX_ASYNC_RESET   RTL_BIVEX_ASYNC_RESET  
# define VASY_BIVEX_ASYNC_SET     RTL_BIVEX_ASYNC_SET    
# define VASY_BIVEX_ASYNC_WEN     RTL_BIVEX_ASYNC_WEN    
# define VASY_BIVEX_SYNC_RESET    RTL_BIVEX_SYNC_RESET   
# define VASY_BIVEX_SYNC_SET      RTL_BIVEX_SYNC_SET     
# define VASY_BIVEX_SYNC_WEN      RTL_BIVEX_SYNC_WEN     
# define VASY_MAX_BIVEX_TYPE      RTL_MAX_BIVEX_TYPE     

# define VASY_BIVEX_TYPE          RTL_BIVEX_TYPE

/*------------------------------------------------------------\
|                                                             |
|                           Action Flags                      |
|                                                             |
\------------------------------------------------------------*/

# define VASY_ACT_USED_MASK    0x1
# define VASY_ACT_ARITH_MASK   0x2

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Process Analysis                    |
|                                                             |
\------------------------------------------------------------*/

# define IsVasyVpnProcToAnalyse( P )    ( (P)->FLAGS & VASY_PROC_TO_ANALYSE_MASK  )
# define SetVasyVpnProcToAnalyse( P )   ( (P)->FLAGS |= VASY_PROC_TO_ANALYSE_MASK )
# define ClearVasyVpnProcToAnalyse( P ) ( (P)->FLAGS &= ~VASY_PROC_TO_ANALYSE_MASK)

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Declar Type                    |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyRtlDeclType( D, T )   ((D)->FLAGS |= (long)(T))
# define IsVasyRtlDeclType( D, T )    ((D)->FLAGS & (long)(T))

# define SetVasyRtlDeclVerilogReg( D ) ((D)->FLAGS |= VASY_RTL_VERILOG_REG_MASK)
# define IsVasyRtlDeclVerilogReg( D )  ((D)->FLAGS &  VASY_RTL_VERILOG_REG_MASK)

/*------------------------------------------------------------\
|                                                             |
|                            Proc Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyVpnProcInfo( P, I )  ((P)->USER = (void *)(I))
# define GetVasyVpnProcInfo( P    )  ((vasyprocinfo *)(P)->USER)

/*------------------------------------------------------------\
|                                                             |
|                             Act Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyVpnActInfo( A, I )  ((A)->USER = (void *)(I))
# define GetVasyVpnActInfo( A    )  ((vasyactinfo *)(A)->USER)

# define GetVasyVpnActInfoSupport( A )  (((vasyactinfo *)(A)->USER)->SUPPORT)
# define GetVasyVpnActInfoFlags( A )    (((vasyactinfo *)(A)->USER)->FLAGS)
# define GetVasyVpnActInfoLiteral( A )  (((vasyactinfo *)(A)->USER)->LITERAL)
# define GetVasyVpnActInfoEvent( A )    (((vasyactinfo *)(A)->USER)->EVENT)

# define SetVasyVpnActInfoUsed( F )  ( (F) |= VASY_ACT_USED_MASK )
# define SetVasyVpnActInfoArith( F ) ( (F) |= VASY_ACT_ARITH_MASK )

# define ClearVasyVpnActInfoUsed( F )  ( (F) &= ~VASY_ACT_USED_MASK )
# define ClearVasyVpnActInfoArith( F ) ( (F) &= ~VASY_ACT_ARITH_MASK )

# define IsVasyVpnActInfoUsed( F )  ( (F) & VASY_ACT_USED_MASK )
# define IsVasyVpnActInfoArith( F ) ( (F) & VASY_ACT_ARITH_MASK )

/*------------------------------------------------------------\
|                                                             |
|                           Trans Info                        |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyVpnTransInfo( T, I )  ((T)->USER = (void *)(I))
# define GetVasyVpnTransInfo( T    )  ((vasytransinfo *)(T)->USER)

# define SetVasyVpnWaitInfo( T, I )  ((T)->USER = (void *)(I))
# define GetVasyVpnWaitInfo( T    )  ((vasywaitinfo *)(T)->USER)

/*------------------------------------------------------------\
|                                                             |
|                            Sym Info                         |
|                                                             |
\------------------------------------------------------------*/

# define SetVasyVpnSymInfo( S, I )  ((S)->USER = (void *)(I))
# define GetVasyVpnSymInfo( S    )  ((vasysyminfo *)(S)->USER)

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct vasybivex_list
  {
    struct vasybivex_list *NEXT;
    vexexpr              *VEX_COND;
    vexexpr              *VEX_DATA;
    unsigned char         TYPE;

  } vasybivex_list;

  typedef struct vasyprocinfo
  {
    long            NUMBER_WAIT;
    long            NUMBER_ACTION;
    bddcircuit     *BDD_CIRCUIT;
    bddsystem      *BDD_SYSTEM;
    bddnode       **BDD_LITERAL;
    authtable      *HASH_ABL2SYM;
    authtable      *HASH_SYM2ABL;
    vasybivex_list *BIVEX;
    unsigned char   TYPE;
    
  } vasyprocinfo;

  typedef struct vasywaitinfo
  {
    vpntrans_list *DEF_TRANS;
    vpntrans_list *LOOP_TRANS;
    vpnplace_list *WAIT_PLACE;

  } vasywaitinfo;

  typedef struct vasytransinfo
  {
    authtable  *HASH_ASSIGN;
    authtable  *HASH_BITVEC;
    auth2table *HASH_SUPPORT;
    auth2table *HASH_LITERAL;
    auth2table *HASH_EVENT;

  } vasytransinfo;


  typedef struct vasysyminfo
  {
    vexexpr          *VEX_DATA;
    vasybivex_list    *BIVEX;
    unsigned char     TYPE;
    unsigned char     TRS_TYPE;
    unsigned char     REG_TYPE;

  } vasysyminfo;

  typedef struct vasyactinfo
  {
    chain_list   **SUPPORT;
    chain_list   **LITERAL;
    chain_list   **EVENT;
    vasysyminfo    *SYM_INFO;
    unsigned char *FLAGS;
    unsigned char  SPLIT;
    
  } vasyactinfo;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *VASY_SYM_TYPE[ VASY_MAX_SYM_TYPE ];

  extern char *VasyPowerVdd;
  extern char *VasyPowerVss;

  extern int VasyFlagDebug;
  extern int VasyFlagOption;
  extern int VasyFlagDrive;
  extern int VasyFlagVerbose;
  extern int VasyFlagHier;
  extern int VasyFlagOver;
  extern int VasyFlagStdLogic;
  extern int VasyFlagInitial;
  extern int VasyFlagPower;
  extern int VasyFlagCLA;
  extern int VasyFlagEqual;
  extern int VasyFlagLax;
  extern int VasyFlagBoom;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern vasyprocinfo    *VasyAddVpnProcInfo();
  extern void             VasyDelVpnProcInfo();
                        
  extern vasytransinfo   *VasyAddVpnTransInfo();
  extern void             VasyDelVpnTransInfo();
                        
  extern vasywaitinfo    *VasyAddVpnWaitInfo();
  extern void             VasyDelVpnWaitInfo();
                        
  extern vasyactinfo     *VasyAddVpnActInfo();
  extern void             VasyDelVpnActInfo();

  extern vasybivex_list  *VasyAddVpnSymBiVex();
  extern vasybivex_list  *VasyAddVpnProcBiVex();

# endif
