/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*******************************************************************************
*                                                                              *
*  Tool        : Parser al                                                     *
*  Author(s)   : Gregoire AVOT                                                 *
*  Updates     : June, 12th 1998                                               *
*  Updates     : June, 30th 1998  Create unique name on losig where no one     *
*                                 is provided.                                 *
*  Updates     : AUGUST, 12th 2002, Pierre Nguyen Tuong                        *
*  $Log: alc_pars_l6.c,v $
*  Revision 1.2  2002/08/13 16:40:14  pnt
*  Suite de l'introduction des objets analogiques capacite, resistance et self.
*
*  Modification du parser et du driver al.
*
*  Syntaxe:
*  P type capa name tcon bcon node_tcon node_bcon           ---capacite
*  R type resi name rcon1 rcon2 node_rcon1 node_rcon2       ---resistance
*  L type self name scon1 scon2 node_scon1 node_scon2       ---inductance
*
*  Note:
*  Q existe deja pour les capacites de type RC (rcn). On utilise P a la place.
*
*  Revision 1.1.1.1  2002/03/13 10:19:11  fred
*  Importing MBKAL sources into the new CVS tree
*
*                                                                              *
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <varargs.h>
#include <math.h>

#include <mut.h> 
#include <mlo.h> 
#include <rcn.h> 
#include <mlu.h> 

/******************************************************************************/

#define MAL_CON	0x00000001
#define MAL_INS	0x00000002
#define MAL_TRS	0x00000004
#define MAL_WIR	0x00000008
#define MAL_CAP	0x00000010
#define MAL_CTC	0x00000020
#define MAL_SIG	0x00000040
#define MAL_EOF	0x00000080
#define MAL_HEA 0x00000100

#define MAL_LOCAP  0x00000200
#define MAL_LORES  0x00000400
#define MAL_LOSELF 0x00000800

/* Tampon de lecture */
#define	MALBUFMAX 524288

/* ptype sur losig -> verification unicite du signal */
#define MALDEFINED 11223344

/* ptype sur losig -> nom unique */
#define SIGHT 199806301

typedef struct
{
  char		*name;
  char		 type;
  char		 dir;
  losig_list	*ptsig;
  num_list	*phcon;
} data_locon;

typedef struct
{
  char		*insname;	/* namealloc */
  char		*modelename;	/* namealloc */
  chain_list	*interface;
} data_loins;

/******************************************************************************/

int		nbitem        __P(( chain_list* ));
char		decode_dir    __P(( char*, char*, int ));
char		decode_type   __P(( char*, char*, int ));
losig_list*	decode_sig    __P(( lofig_list*, chain_list*, char*, int,
                                    int
                                 ));
lotrs_list*	decode_trs    __P(( lofig_list*, chain_list*, char*, int,
                                    int
                                 ));

locap_list *decode_locap(lofig_list *ptfig,chain_list *line,char *fname,int mal_line,int version) ;

/*
locap_list*	decode_locap    __P(( lofig_list*, chain_list*, char*, int,
                                    int
                                 ));
*/

lores_list*	decode_lores    __P(( lofig_list*, chain_list*, char*, int,
                                    int
                                 ));

loself_list*	decode_loself    __P(( lofig_list*, chain_list*, char*, int,
                                    int
                                 ));

int		decode_int    __P(( char*, char*, int ));
double		decode_float  __P(( char*, char*, int ));
data_locon*	decode_locon  __P(( lofig_list*, chain_list*, char*, int ));
lowire_list*	decode_lowire __P(( losig_list*, chain_list*, char*, int ));
loctc_list*	decode_loctc  __P(( lofig_list*, chain_list*, char*, int ));
float		decode_capa   __P(( losig_list*, chain_list*, char*, int ));
int		type_line     __P(( chain_list*, char*, int ));
chain_list*	read_line     __P(( FILE*, char*, int ));
void		free_line     __P(( chain_list* ));
void		mal_error     __P(());
void		chk_header    __P(( chain_list*, char*, int ));
unsigned char	decode_layer  __P(( char*, char*, int ));
data_loins*	decode_ins    __P(( chain_list*, char*, int ));
loins_list*	end_ins       __P(( lofig_list*, lofig_list**, data_loins*,
                                    char*, int
                                 ));
void		complete_ins  __P(( lofig_list*, data_loins*, chain_list*,
                                    char*, int
				 ));

/*** Macro and function for debug *********************************************/

/* Set the define 'ALDEBUG' will ENABLE debug mode */
/*
#define ALDEBUG
*/

#ifdef ALDEBUG

chain_list*     al_dbg_addchain
                              __P(( chain_list*, void*, int ));
void            al_dbg_freechain
                              __P(( chain_list* ));
void            al_dbg_chkchain
                              __P(( void ));
void            al_dbg_init   __P(( void ));                              

#define ALMAXCHAIN 1024

chain_list*     al_chain_pt[ALMAXCHAIN];
int             al_chain_lg[ALMAXCHAIN];

#define al_addchain( a, b )    al_dbg_addchain( a, b, __LINE__ )
#define al_freechain( a )      al_dbg_freechain( a )

#else

#define al_addchain( a, b )    addchain( a, b )
#define al_freechain( a )      freechain( a )

#endif

/******************************************************************************/

int		nbitem( head )
chain_list	*head;
{
  chain_list	*scan;
  int		 count;
  
  for( scan = head, count = 0 ; scan ; scan = scan->NEXT, count++ );
  return( count );
}

/******************************************************************************/

unsigned char	decode_layer( elem, fname, mal_line )
char		*elem;
char		*fname;
int		 mal_line;
{
  if( strcmp( elem, "X"  ) == 0 ) return RCN_WIRE_UNKNOW;
  if( strcmp( elem, "PY" ) == 0 ) return RCN_WIRE_POLY;
  if( strcmp( elem, "A1" ) == 0 ) return RCN_WIRE_ALU1;
  if( strcmp( elem, "A2" ) == 0 ) return RCN_WIRE_ALU2;
  if( strcmp( elem, "CY" ) == 0 ) return RCN_WIRE_CONT_POLY;
  if( strcmp( elem, "CN" ) == 0 ) return RCN_WIRE_CONT_DIF_N;
  if( strcmp( elem, "CP" ) == 0 ) return RCN_WIRE_CONT_DIF_P;
  if( strcmp( elem, "CV" ) == 0 ) return RCN_WIRE_CONT_VIA;
  if( strcmp( elem, "CW" ) == 0 ) return RCN_WIRE_CONT_VIA2;

  mal_error( fname,
             mal_line, 
             "decode_layer()", 
             "invalid wire type [%s].", 
             elem 
           );

  /* Never reach */
  return 0;
}

/******************************************************************************/

char		decode_dir( elem, fname, mal_line )
char		*elem;
char		*fname;
int		 mal_line;
{
  if( strcasecmp( elem, "IN"       ) == 0 ) return( 'I' );
  if( strcasecmp( elem, "OUT"      ) == 0 ) return( 'O' );
  if( strcasecmp( elem, "INOUT"    ) == 0 ) return( 'B' );
  if( strcasecmp( elem, "UNKNOWN"  ) == 0 ) return( 'X' );
  if( strcasecmp( elem, "TRISTATE" ) == 0 ) return( 'Z' );
  if( strcasecmp( elem, "TRANSCV"  ) == 0 ) return( 'T' );

  mal_error( fname, 
             mal_line, 
             "decode_dir", 
             "Bad direction [%s] for connector.\n", 
             elem 
           );

  /* Never reach */
  return 0;
}

/******************************************************************************/

char		decode_type( elem, fname, mal_line )
char		*elem;
char		*fname;
int		mal_line;
{
  if( strcasecmp( elem, "INTERNAL" ) == 0 ) return( 'I' );
  if( strcasecmp( elem, "EXTERNAL" ) == 0 ) return( 'E' );
  
  mal_error( fname, 
             mal_line, 
             "decode_type()", 
             "Bad type [%s] for connector.\n", 
             elem 
           );

  /* Never reach */
  return 0;
}

/******************************************************************************/

float		decode_capa( ptsig, line, fname, mal_line )
losig_list	*ptsig;
chain_list	*line;
char		*fname;
int		 mal_line;
{
  float		capa;
  
  if( nbitem( line ) != 1 )
    mal_error( fname, 
               mal_line, 
               "decode_capa()",
               "Bad number of argument.\n"
             );

  if(!ptsig->PRCN)
    addlorcnet( ptsig );

  capa = (float)decode_float( (char*) line->DATA, fname, mal_line );
  addcapa( ptsig, capa );

  return capa;
}

/******************************************************************************/

data_loins*	decode_ins( line, fname, mal_line )
chain_list	*line;
char		*fname;
int		 mal_line;
{
  data_loins	*newins;
  
  if( nbitem( line ) != 2 )
    mal_error( fname, mal_line, "decode_ins()", "Bad number of argument.\n");

  newins             = (data_loins*) mbkalloc( sizeof( data_loins ) );
  newins->modelename = namealloc( (char*) line->DATA       );
  newins->insname    = namealloc( (char*) line->NEXT->DATA );
  newins->interface  = NULL;

  return( newins );
}

/******************************************************************************/

lotrs_list*	decode_trs( ptfig, line, fname, mal_line, version )
lofig_list	*ptfig;
chain_list	*line;
char		*fname;
int		 mal_line;
int              version;
{
  char		 type;
  long           w;
  long           l;
  losig_list	*drain;
  losig_list	*grid;
  losig_list	*source;
  losig_list	*bulk;
  long           xs;
  long           xd;
  long           ps;
  long           pd;
  long		 x;
  long		 y;
  int		 phsource;
  int		 phdrain;
  int		 phgrid;
  int		 phbulk;
  lotrs_list	*pttrs;
  int		 n;
  char		*name;
  
  n = nbitem( line );
  
  /* Version 4 :  type L W D G S XS XD PS PD X Y
   * Version 5 :  type L W D G S XS XD PS PD X Y [ nd ng ns ]
   * Version 6 :  type L W D G S B XS XD PS PD X Y [ nd ng ns nb ] nom
   */
  
  if( ( version == 6 && n != 14 && n != 18   ) ||
      ( version == 5 && n != 12 && n != 15   ) ||
      ( version == 4 && n != 12              )    )
    mal_error( fname, 
               mal_line, 
               "decode_trs()",
               "Bad number of element (%d).\n", 
               n
             );

  type = -1;
  if( strcasecmp( (char*)line->DATA, "P" ) == 0 ) type = TRANSP; 
  if( strcasecmp( (char*)line->DATA, "N" ) == 0 ) type = TRANSN; 
  if( type == -1 )
    mal_error( fname,
               mal_line,
               "decode_trs()",
	       "Unknown transistor type [%s].\n",
	       (char*)line->DATA
	     );
  line   = line->NEXT;

  l      = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line   = line->NEXT;
  
  w      = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line   = line->NEXT;

  drain  = givelosig( ptfig, decode_int( (char*)line->DATA, fname, mal_line ) );
  line   = line->NEXT;
  
  grid   = givelosig( ptfig, decode_int( (char*)line->DATA, fname, mal_line ) );
  line   = line->NEXT;
  
  source = givelosig( ptfig, decode_int( (char*)line->DATA, fname, mal_line ) );
  line   = line->NEXT;
 
  bulk = NULL;

  if( version == 6 )
  {
    n      = decode_int( (char*)line->DATA, fname, mal_line );
    if( n )
      bulk = givelosig( ptfig, n );
  
    line   = line->NEXT;
  }

  xs   = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line = line->NEXT;
  
  xd   = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line = line->NEXT;
  
  ps   = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line = line->NEXT;
  
  pd   = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line = line->NEXT;
  
  x    = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line = line->NEXT;
  
  y    = decode_float( (char*)line->DATA, fname, mal_line ) * (double)SCALE_X ;
  line = line->NEXT;

  phdrain  = 0;
  phgrid   = 0;
  phsource = 0;
  phbulk   = 0;
 
  if( version == 5 || version == 6 )
  {
    if( line && line->NEXT )	/* 3 or 4 physicals node are coming */
    {
      phdrain  = decode_int( (char*)line->DATA, fname, mal_line );
      line     = line->NEXT;

      phgrid   = decode_int( (char*)line->DATA, fname, mal_line );
      line     = line->NEXT;

      phsource = decode_int( (char*)line->DATA, fname, mal_line );
      line     = line->NEXT;

      if( version == 6 )
      {
        phbulk   = decode_int( (char*)line->DATA, fname, mal_line );
        line     = line->NEXT;
      }
    }
  }

  name = NULL;
  if( version == 6 )
  {
    name   = namealloc( (char*)line->DATA );
    line   = line->NEXT;
  }

  pttrs  = addlotrs( ptfig, type, x, y, w, l, ps, pd, xs, xd,
                     grid, source, drain, bulk,
                     name
	 	   );

  if( phdrain )
  {
    if( !pttrs->DRAIN->SIG->PRCN )
      addlorcnet( pttrs->DRAIN->SIG );

    setloconnode( pttrs->DRAIN, phdrain );
  }

  if( phgrid )
  {
    if( !pttrs->GRID->SIG->PRCN )
      addlorcnet( pttrs->GRID->SIG );

    setloconnode( pttrs->GRID, phgrid );
  }

  if( phsource )
  {
    if( !pttrs->SOURCE->SIG->PRCN )
      addlorcnet( pttrs->SOURCE->SIG );

    setloconnode( pttrs->SOURCE, phsource );
  }

  if( phbulk )
  {
    if( !pttrs->BULK->SIG->PRCN )
      addlorcnet( pttrs->BULK->SIG );

    setloconnode( pttrs->BULK, phbulk );
  }

  return( pttrs );
}

/******************************************************************************/

locap_list *decode_locap(lofig_list *ptfig,chain_list *line,char *fname,int mal_line,int version)
{
  char       type   = -1   ;
  losig_list *tcon  = NULL ;
  losig_list *bcon  = NULL ;
  int	     phtcon = 0    ;
  int	     phbcon = 0    ;
  locap_list *ptcap = NULL ;
  int	     n      = 0    ;
  char	     *name  = NULL ;
  float      capa   = 0.0  ;

  n = nbitem(line) ;
  
  /* Version 6 :  P type capa name tcon bcon node_tcon node_bcon */
  
  /***** type *****/

  type = -1 ;

  if(strcasecmp((char *)line -> DATA, "MIM") == 0)
   {
     type = CAPMIM ;
   }

  if(strcasecmp((char *)line -> DATA, "POLY_NWELL") == 0)
   {
     type = CAPPNWELL ; 
   }

  if(type == -1)
    {
      mal_error(fname,
                mal_line,
                "decode_locap()",
	        "Unknown capacitor type [%s].\n",
	        (char *)line -> DATA) ;
    }

  line   = line -> NEXT ;

  /***** capa *****/

  capa   = decode_float((char *)line -> DATA,fname,mal_line) ;
  line   = line -> NEXT ;

  /***** name *****/

  name   = namealloc((char *)line -> DATA) ;
  line   = line -> NEXT ;

  /***** tcon bcon *****/

  tcon   = givelosig(ptfig,decode_int((char *)line -> DATA,fname,mal_line)) ;
  line   = line -> NEXT ;
  
  bcon   = givelosig(ptfig,decode_int((char *)line -> DATA,fname,mal_line)) ;
  line   = line -> NEXT ;

  /***** node_tcon node_bcon *****/

  phtcon  = decode_int((char *)line -> DATA,fname,mal_line) ;
  line    = line -> NEXT ;
  
  phbcon  = decode_int((char *)line -> DATA,fname,mal_line) ;
  line    = line -> NEXT ;

  ptcap  = addlocap(ptfig,type,capa,tcon,bcon,name) ;

  if(phtcon)
    {
      if(!ptcap -> TCON -> SIG -> PRCN)
	{
          addlorcnet(ptcap -> TCON -> SIG) ;
	}
 
      setloconnode(ptcap -> TCON,phtcon) ;
  }

  if(phbcon)
    {
      if(!ptcap -> BCON -> SIG -> PRCN)
        {
          addlorcnet(ptcap -> BCON -> SIG) ;
        }

      setloconnode(ptcap -> BCON,phbcon) ;
    }

  return(ptcap) ;
}

/******************************************************************************/

lores_list *decode_lores(lofig_list *ptfig,chain_list *line,char *fname,int mal_line,int version)
{
  char       type    = -1   ;
  losig_list *rcon1  = NULL ;
  losig_list *rcon2  = NULL ;
  int	     phrcon1 = 0    ;
  int	     phrcon2 = 0    ;
  lores_list *ptres  = NULL ;
  int	     n       = 0    ;
  char	     *name   = NULL ;
  float      resi    = 0.0  ;

  n = nbitem(line) ;
  
  /* Version 6 :  R type resi name rcon1 rcon2 node_rcon1 node_rcon2 */
  
  /***** type *****/

  type = -1 ;

  if(strcasecmp((char *)line -> DATA, "MIM") == 0)
   {
     type = RESMIM ;
   }

  if(strcasecmp((char *)line -> DATA, "MIM") == 0)
   {
     type = RESMIM ; 
   }

  if(type == -1)
    {
      mal_error(fname,
                mal_line,
                "decode_lores()",
	        "Unknown resistor type [%s].\n",
	        (char *)line -> DATA) ;
    }

  line   = line -> NEXT ;

  /***** resi *****/

  resi   = decode_float((char *)line -> DATA,fname,mal_line) ;
  line   = line -> NEXT ;

  /***** name *****/

  name   = namealloc((char *)line -> DATA) ;
  line   = line -> NEXT ;

  /***** rcon1 rcon2 *****/

  rcon1   = givelosig(ptfig,decode_int((char *)line -> DATA,fname,mal_line)) ;
  line   = line -> NEXT ;
  
  rcon2   = givelosig(ptfig,decode_int((char *)line -> DATA,fname,mal_line)) ;
  line   = line -> NEXT ;

  /***** node_rcon1 node_rcon2 *****/

  phrcon1  = decode_int((char *)line -> DATA,fname,mal_line) ;
  line    = line -> NEXT ;
  
  phrcon2  = decode_int((char *)line -> DATA,fname,mal_line) ;
  line    = line -> NEXT ;

  ptres  = addlores(ptfig,type,resi,rcon1,rcon2,name) ;

  if(phrcon1)
    {
      if(!ptres -> RCON1 -> SIG -> PRCN)
	{
          addlorcnet(ptres -> RCON1 -> SIG) ;
	}
 
      setloconnode(ptres -> RCON1,phrcon1) ;
  }

  if(phrcon2)
    {
      if(!ptres -> RCON2 -> SIG -> PRCN)
        {
          addlorcnet(ptres -> RCON2 -> SIG) ;
        }

      setloconnode(ptres -> RCON2,phrcon2) ;
    }

  return(ptres) ;
}

/******************************************************************************/

loself_list *decode_loself(lofig_list *ptfig,chain_list *line,char *fname,int mal_line,int version)
{
  char        type     = -1   ;
  losig_list  *scon1   = NULL ;
  losig_list  *scon2   = NULL ;
  int	      phscon1  = 0    ;
  int	      phscon2  = 0    ;
  loself_list *ptself = NULL ;
  int	      n        = 0    ;
  char	      *name    = NULL ;
  float       self     = 0.0  ;

  n = nbitem(line) ;
  
  /* Version 6 :  L type self name scon1 scon2 node_scon1 node_scon2 */
  
  /***** type *****/

  type = -1 ;

  if(strcasecmp((char *)line -> DATA, "MIM") == 0)
   {
     type = SELFMIM ;
   }

  if(strcasecmp((char *)line -> DATA, "MIM") == 0)
   {
     type = SELFMIM ; 
   }

  if(type == -1)
    {
      mal_error(fname,
                mal_line,
                "decode_loself()",
	        "Unknown inductor type [%s].\n",
	        (char *)line -> DATA) ;
    }

  line   = line -> NEXT ;

  /***** self *****/

  self   = decode_float((char *)line -> DATA,fname,mal_line) ;
  line   = line -> NEXT ;

  /***** name *****/

  name   = namealloc((char *)line -> DATA) ;
  line   = line -> NEXT ;

  /***** scon1 scon2 *****/

  scon1   = givelosig(ptfig,decode_int((char *)line -> DATA,fname,mal_line)) ;
  line   = line -> NEXT ;
  
  scon2   = givelosig(ptfig,decode_int((char *)line -> DATA,fname,mal_line)) ;
  line   = line -> NEXT ;

  /***** node_scon1 node_scon2 *****/

  phscon1  = decode_int((char *)line -> DATA,fname,mal_line) ;
  line    = line -> NEXT ;
  
  phscon2  = decode_int((char *)line -> DATA,fname,mal_line) ;
  line    = line -> NEXT ;

  ptself  = addloself(ptfig,type,self,scon1,scon2,name) ;

  if(phscon1)
    {
      if(!ptself -> SCON1 -> SIG -> PRCN)
	{
          addlorcnet(ptself -> SCON1 -> SIG) ;
	}
 
      setloconnode(ptself -> SCON1,phscon1) ;
  }

  if(phscon2)
    {
      if(!ptself -> SCON2 -> SIG -> PRCN)
        {
          addlorcnet(ptself -> SCON2 -> SIG) ;
        }

      setloconnode(ptself -> SCON2,phscon2) ;
    }

  return(ptself) ;
}

/******************************************************************************/

void		complete_ins( ptfig, ins, line, fname, mal_line )
lofig_list	*ptfig;
data_loins	*ins;
chain_list	*line;
char		*fname;
int		 mal_line;
{
  data_locon	*ptcon;

  ptcon       = decode_locon( ptfig, line, fname, mal_line );
  ptcon->name = namealloc( ptcon->name );

  ins->interface = al_addchain( ins->interface, ptcon );
}

/******************************************************************************/

loctc_list*	decode_loctc( ptfig, line, fname, mal_line )
lofig_list	*ptfig;
chain_list	*line;
char		*fname;
int		 mal_line;
{
  float		 capa;
  int		 idxsig1;
  int		 idxsig2;
  losig_list    *ptsig1;
  losig_list    *ptsig2;
  int            node1;
  int            node2;
  
  if( nbitem( line ) != 5 )
    mal_error( fname, mal_line, "decode_loctc()", "Bad number of argument.\n");

  capa    = (float)decode_float( (char*)line->DATA, fname, mal_line );
  line    = line->NEXT;

  idxsig1 = decode_int( (char*)line->DATA, fname, mal_line );
  line    = line->NEXT;

  node1   = decode_int( (char*)line->DATA, fname, mal_line );
  line    = line->NEXT;

  idxsig2 = decode_int( (char*)line->DATA, fname, mal_line );
  line    = line->NEXT;

  node2   = decode_int( (char*)line->DATA, fname, mal_line );
  line    = line->NEXT;

  ptsig1  = givelosig( ptfig, idxsig1 );
  ptsig2  = givelosig( ptfig, idxsig2 );

  if( !ptsig1->PRCN )
    addlorcnet( ptsig1 );

  if( !ptsig2->PRCN )
    addlorcnet( ptsig2 );

  return( addloctc( ptsig1, node1, ptsig2, node2, capa ) );
}

/******************************************************************************/

lowire_list*	decode_lowire( ptsig, line, fname, mal_line )
losig_list	*ptsig;
chain_list	*line;
char		*fname;
int		 mal_line;
{
  int		n1;
  int		n2;
  int		layer;
  float		r;
  float		c;
  float		x;
  float		y;
  float		dx;
  float		dy;
  long		lx;
  long		ly;
  long		ldx;
  long		ldy;
  
  if( nbitem( line ) != 9 )
    mal_error( fname, mal_line,"decode_lowire()","Bad number of argument.\n" );

  if( !ptsig->PRCN )
    addlorcnet( ptsig );

  n1    = decode_int( line->DATA, fname, mal_line );
  line  = line->NEXT;
  
  n2    = decode_int( line->DATA, fname, mal_line );
  line  = line->NEXT;

  layer = decode_layer( line->DATA, fname, mal_line );
  line  = line->NEXT;

  r     = (float)decode_float( line->DATA, fname, mal_line );
  line  = line->NEXT;

  c     = (float)decode_float( line->DATA, fname, mal_line );
  line  = line->NEXT;

  x     = (float)decode_float( line->DATA, fname, mal_line );
  line  = line->NEXT;

  y     = (float)decode_float( line->DATA, fname, mal_line );
  line  = line->NEXT;

  dx    = (float)decode_float( line->DATA, fname, mal_line );
  line  = line->NEXT;

  dy    = (float)decode_float( line->DATA, fname, mal_line );
  line  = line->NEXT;

  lx    = x  * SCALE_X;
  ly    = y  * SCALE_X;
  ldx   = dx * SCALE_X;
  ldy   = dy * SCALE_X;

  return( addlowire( ptsig, layer, 0, r, c, x, y, dx, dy, n1, n2 ) );
}

/******************************************************************************/

losig_list*	decode_sig( ptfig, line, fname, mal_line, version )
lofig_list	*ptfig;
chain_list	*line;
char		*fname;
int		 mal_line;
int              version;
{
  int		 idx;
  losig_list	*ptsig;
  char		 type;
  float          capa;
  char           signame[256];
  ht            *htsigname;
  long           index;
  char          *ptallocsigname;
  long           i;
  long           j;
  long           k;
   
  if( ( nbitem( line ) < 2 && version == 6 ) ||
      ( nbitem( line ) < 3 && version == 5 ) ||
      ( nbitem( line ) < 3 && version == 4 )    
    )
    mal_error( fname, mal_line, "decode_sig()", "Incomplete line.\n" );

  idx   = decode_int( (char*)line->DATA, fname, mal_line );
  line  = line->NEXT;
  ptsig = givelosig( ptfig, idx );

  if( getptype( ptsig->USER, MALDEFINED ) )
    mal_error( fname, mal_line, "decode_sig()","Signal %d yet defined.\n", idx);

  type  = decode_type( (char*)line->DATA, fname, mal_line );
  line  = line->NEXT;

  ptsig->TYPE = type;

  if( version == 5 || version == 4 )
  {
    if(!ptsig->PRCN)
      addlorcnet( ptsig );

    capa = (float)decode_float( (char*) line->DATA, fname, mal_line );
    addcapa( ptsig, capa );

    line = line->NEXT;
  }
 
  htsigname = (ht*) ( getptype( ptfig->USER, SIGHT )->DATA );

  if( line )
  {
    for( ; line ; line = line->NEXT )
    {
      ptsig->NAMECHAIN = addchain( ptsig->NAMECHAIN,
                                   namealloc( (char*) line->DATA )
                                 );
      addhtitem( htsigname, ptsig->NAMECHAIN->DATA, 1 );
    }
  }
  else
  {
    /* Le parser al ajoute un nom si le signal n'en possede pas deja.
     * Bizare : ce n'est pas au parser de faire ce boulot.
     */

    sprintf( signame, "mbk_sig%ld", ptsig->INDEX );
    ptallocsigname = namealloc( signame ) ;

    if( gethtitem( htsigname, ptallocsigname ) != EMPTYHT )
    {
      /* En theorie, sur 11 caracteres, on arrive a 3E15 pour index.
       * En pratique, on s'en fout.
       */
     
      index = 0;

      do
      {
        sprintf( signame, "mbk_sig012345" );
        j = 26*26*26*26*26 ;
        k = index;
        for( i = 5 ; i >= 0 ; i-- )
        {
          signame[12-i] = k / j + 'a' ;
          k = k - ( ( k / j ) * j ) ;
          j = j / 26 ;
        }
        ptallocsigname = namealloc( signame ) ;
        index++;
      }
      while( gethtitem( htsigname, ptallocsigname ) != EMPTYHT );
    }
    
    ptsig->NAMECHAIN = addchain( ptsig->NAMECHAIN, ptallocsigname );
    addhtitem( htsigname, ptsig->NAMECHAIN->DATA, 1 );
  }
 
  ptsig->USER = addptype( ptsig->USER, MALDEFINED, (void*)1 );
  return( ptsig );
}
 
/******************************************************************************/

int		decode_int( elem, fname, mal_line )
char		*elem;
char		*fname;
int		 mal_line;
{
  long		 v;
  char		*stop;

  v = strtol( elem, &stop, 10 );
  
  if( *stop != '\0' )
    mal_error( fname, mal_line, "decode_int()","Not an integer [%s].\n", elem );

  return( (int) v );
}

/******************************************************************************/

double		decode_float( elem, fname, mal_line )
char		*elem;
char		*fname;
int		 mal_line;
{
  double  	v;
  char		*stop;

  v = strtod( elem, &stop );

  if( *stop != '\0' )
    mal_error( fname, mal_line, "decode_float()", "Not a float [%s].\n", elem );

  return( v );
}

/******************************************************************************/

data_locon*	decode_locon( ptfig, line, fname, mal_line )
lofig_list	*ptfig;
chain_list	*line;
char		*fname;
int		 mal_line;
{
  data_locon	*newcon;

  newcon = (data_locon*) mbkalloc( sizeof( data_locon ) );
  
  if( nbitem( line ) < 4 )
    mal_error( fname, 
               mal_line, 
               "decode_locon()", 
               "Bad description of connector.\n" 
             );

  newcon->name   = (char*) line->DATA;
  line           = line->NEXT;

  newcon->dir    = decode_dir( (char*) line->DATA, fname, mal_line );
  line           = line->NEXT;

  newcon->type   = decode_type( (char*) line->DATA, fname, mal_line );
  line           = line->NEXT;

  /*
  if( ptfig->MODE != 'C' )
  {
  */
    newcon->ptsig = givelosig( ptfig, decode_int( (char*) line->DATA,
                                                  fname,
  						  mal_line
    					        )
                             );

    newcon->ptsig->TYPE = 'E';
  /*
  }
  */

  line           = line->NEXT;

  newcon->phcon  = NULL;
  for( ; line ; line = line->NEXT )
    newcon->phcon = addnum( newcon->phcon,
                            decode_int( (char*) line->DATA, fname, mal_line )
			  );

  return( newcon );
}

/******************************************************************************/

int		type_line( head, fname, mal_line ) 
chain_list	*head;
char		*fname;
int		 mal_line;
{
  char		*type;

  type = (char*) head->DATA;

  if( strcasecmp( type, "H"   ) == 0 ) return ( MAL_HEA );
  if( strcasecmp( type, "S"   ) == 0 ) return ( MAL_SIG );
  if( strcasecmp( type, "I"   ) == 0 ) return ( MAL_INS );
  if( strcasecmp( type, "C"   ) == 0 ) return ( MAL_CON );
  if( strcasecmp( type, "W"   ) == 0 ) return ( MAL_WIR );
  if( strcasecmp( type, "Q"   ) == 0 ) return ( MAL_CAP );
  if( strcasecmp( type, "K"   ) == 0 ) return ( MAL_CTC );
  if( strcasecmp( type, "T"   ) == 0 ) return ( MAL_TRS );

  /* Trucs analogiques. Q et K sont deja pris */
  if( strcasecmp( type, "P"   ) == 0 ) return ( MAL_LOCAP );
  if( strcasecmp( type, "R"   ) == 0 ) return ( MAL_LORES );
  if( strcasecmp( type, "L"   ) == 0 ) return ( MAL_LOSELF);

  if( strcasecmp( type, "EOF" ) == 0 ) return ( MAL_EOF );

  mal_error( fname, mal_line, "type_line()", "Unknown element [%s].\n", type );

  /* Never reach */
  return 0;
}

/******************************************************************************/

chain_list*	read_line( df, fname, mal_line )
FILE		*df;
char		*fname;
int		 mal_line;
{
  char		buffer[MALBUFMAX];
  char		word[MALBUFMAX];
  int		lg;
  int		onword;
  int		i;
  int		j=0;
  char		*elem;
  chain_list	*decomp;
  
  decomp = NULL;
  
  if( fgets( buffer, MALBUFMAX, df ) == NULL )
    mal_error( fname, 
               mal_line, 
               "read_line()", 
               "Error when reading input file.\n"
             );

  if( feof(df) )
    mal_error( fname, 
               mal_line, 
               "read_line()", 
               "End of file before end of parsing.\n" 
             );

  lg = strlen( buffer );

  if( lg == MALBUFMAX-1 )
    mal_error( fname, 
               mal_line, 
               "read_line()",
               "Line exceed %d characters.\n",
               MALBUFMAX
             );

  if( lg > 0 )
  {
    if( buffer[lg-1] != '\n' )
    {
      fflush( stdout );
      fprintf( stderr, "Ligne non terminee par '\\n'.\n" );
      fprintf( stderr, "\"%s\"\n", buffer );
    }
  }
  
  onword  = 0;
 
  /* En tete de ligne : soit 'EOF', soit 'X '. L'espace apres le X est le
   * seul qui doit etre pris en compte comme separateur. */
  if( lg == 4 && strcmp( "EOF\n", buffer )==0 )
  {
    elem = (char*)mbkalloc( sizeof(char) * 4 );
    strncpy( elem, buffer, 3 );
    elem[3] = 0;
    decomp = al_addchain( decomp, elem );
    return( decomp );
  }

  /* decode le premier argument de type 'X ' */
  if( buffer[1] != ' ' )
    mal_error( fname, mal_line, "read_line()", "Bad type of line.\n" );

  elem = (char*)mbkalloc( sizeof(char) * 2 );
  elem[0] = buffer[0];
  elem[1] = 0;
  decomp = al_addchain( decomp, elem );

  /* Decode le reste de la ligne */

  for( i=2; i<lg; i++ )
  {
    if( ( buffer[i] >= 'A' && buffer[i] <= 'Z' ) ||
        ( buffer[i] >= 'a' && buffer[i] <= 'z' ) ||
        ( buffer[i] >= '0' && buffer[i] <= '9' ) ||
        strchr( " <>[]._-|/+", buffer[i] )               )
    {
      if( !onword )
      {
        onword = 1;
        j = 0;
      }
      word[j++] = buffer[i];
    }
    else
    {
      if( onword ) /* On vient de terminer un mot */
      {
        word[j++] = 0;

        elem  = (char*)mbkalloc( j * sizeof(char) );
        memcpy( elem, word, j );
        decomp = al_addchain( decomp, elem );

        onword = 0;
      }

      /* Ici si des symbols sont considérés comme des mots à part entière */
    }
  }

  if( decomp == NULL )
    mal_error( fname, mal_line, "read_line()", "Blank line.\n" );

  return( reverse( decomp ) );
}

/******************************************************************************/

void		free_line( head )
chain_list	*head;
{
  chain_list	*trashing;

  for( trashing = head ; trashing ; trashing = trashing->NEXT )
    mbkfree( trashing->DATA );

  al_freechain( head );
}

/******************************************************************************/

void		mal_error( va_alist )
va_dcl
{
  va_list	 index;
  char		*fmt;
  char		*fname;
  int		 line;
  char          *func;
  
  va_start( index );
  fname = va_arg( index, char* );
  line  = va_arg( index, int   );
  func  = va_arg( index, char* );
  fmt   = va_arg( index, char* );
  
  fflush( stdout );

  fprintf( stderr,
         "*** mbk error in loadlofig ( al, function %s ) file %s line %d ***\n",
           func,
           fname,
	   line
	 );
  vfprintf( stderr, fmt, index );

  EXIT(1);
}

/******************************************************************************/

void		chk_header( line, fname, mal_line )
chain_list	*line;
char		*fname;
int		 mal_line;
{
  int		 type;

  type = type_line( line, fname, mal_line );

  if( type != MAL_HEA )
    mal_error( fname, mal_line, "chk_header()", "Can't find header.\n" );

  if( ! line->NEXT || ! line->NEXT->NEXT )
    mal_error( fname, mal_line, "chk_header()", "Incomplete header.\n" );

  if( strcasecmp( (char*) line->NEXT->DATA, fname ) != 0 )
    mal_error( fname,
               mal_line,
               "chk_header()",
	       "Bad circuit name : %s insteated of %s.\n",
               (char*) line->NEXT->DATA,
	       fname
	     );

  if( strcasecmp( (char*) line->NEXT->NEXT->DATA, "L" ) != 0 )
    mal_error( fname,
               mal_line, 
               "chk_header()",
               "Bad type of view (not logical).\n"
             );
}

/******************************************************************************/

loins_list*	end_ins( ptfig, tetemodele, newins, fname, mal_line )
lofig_list	*ptfig;
lofig_list	**tetemodele;
data_loins	*newins;
char		*fname;
int		 mal_line;
{
  lofig_list	*ptmodele;
  chain_list	*scan;
  chain_list	*next;
  chain_list	*headsig;
  locon_list	*scancon;
  num_list	*scannum;
  loins_list	*ins;
  data_locon	*dtcon;
  ht            *htcon;
  int            nb;
  
  ptmodele = getlomodel( *tetemodele, newins->modelename );
  if( !ptmodele )
  {
    *tetemodele = addlomodel( *tetemodele, newins->modelename );
    ptmodele   = *tetemodele; 

    /* Building connectors for new model */
    for( scan = newins->interface ; scan ; scan = scan->NEXT )
      addlocon( *tetemodele, ((data_locon*)scan->DATA)->name, NULL, 'X');
  }

  headsig = NULL;
  /* Create losig in the same order of the locon in the model */

  for( scancon = ptmodele->LOCON, nb = 0 ;
       scancon ;
       scancon = scancon->NEXT, nb++
     );

  htcon = addht( nb/10+1 );

  for( scan = newins->interface; scan ; scan = scan->NEXT )
    addhtitem( htcon, ((data_locon*)scan->DATA)->name, (int)(scan) );

  for( scancon = ptmodele->LOCON ; scancon ; scancon = scancon->NEXT )
  {
    scan = (chain_list*)gethtitem( htcon, scancon->NAME );
    if( ((int)(scan)) != EMPTYHT )
      headsig = al_addchain( headsig, ((data_locon*)scan->DATA)->ptsig );
    else
    {
      mal_error( fname,
                 mal_line,
                 "end_ins()",
                 "Bad interface for instance %s of model %s.\n",
                 newins->insname,
                 newins->modelename
               );

    }
  }

  delht( htcon );

  headsig = reverse( headsig ); 

  ins = addloins( ptfig, newins->insname, ptmodele, headsig );

  newins->interface = reverse( newins->interface );
  for( scan = newins->interface, scancon = ins->LOCON ;
       scan ;
       scan = scan->NEXT, scancon = scancon->NEXT
     )
  {
    dtcon = (data_locon*)scan->DATA;
    scancon->DIRECTION = dtcon->dir; 

    if( dtcon->phcon && !scancon->SIG->PRCN )
      addlorcnet( scancon->SIG );


    for( scannum = dtcon->phcon ; scannum ; scannum = scannum->NEXT )
      setloconnode( scancon, scannum->DATA );
  }
  
  al_freechain( headsig );

  for( scan = newins->interface; scan ; scan = scan->NEXT )
  {
    freenum( ((data_locon*)(scan->DATA))->phcon );
    mbkfree( scan->DATA );
  }    

  al_freechain( newins->interface );

  mbkfree( newins );
  
  return( ins );
}

/******************************************************************************/

void		alcloadlofig6( ptfig, fname, mode, in, version )
lofig_list	*ptfig;
char		*fname;
char		 mode;
FILE		*in;
int              version;
{
  chain_list	*line;		/* line read from file			*/
  int		 type;		/* type of line				*/
  int		 mal_line;	/* line number in file			*/
  data_locon	*newcon;	/* data extract from object 'C'		*/
  locon_list	*newlocon;	/* new locon				*/
  losig_list	*newlosig;	/* new losig				*/
  num_list	*scannum;	/* to scan phcon			*/
  int		 compose=0;	/* Valid elements for currents object	*/
  lofig_list	*tetemodele;	/* model list used			*/
  data_loins	*newins;	/* new instance				*/
  losig_list	*scansig;	/* to clean the USER field of losig	*/
  locon_list	*figcon;	/* for mode c : connector of lofig	*/
  ht            *htsigname;     /* create an unique name on losig       */

#ifdef ALDEBUG
  al_dbg_init();
#endif

  mal_line   = 1;
  line       = NULL;
  tetemodele = NULL;

  
  ptfig->MODE = mode;
  htsigname   = addht( 1024 );
  ptfig->USER = addptype( ptfig->USER, SIGHT, htsigname   );
  

  /* check the name							*/
  
  mal_line++;
  line = read_line( in, fname, mal_line );
  chk_header( line, fname, mal_line );
  free_line( line );
 
  /* externals connectors						*/

  figcon = ptfig->LOCON;
  
  do
  {
    mal_line++;
    line = read_line( in, fname, mal_line );
    type = type_line( line, fname, mal_line );
    
    if( type == MAL_CON )
    {
      newcon = decode_locon( ptfig, line->NEXT, fname, mal_line );

      if( mode != 'C' )
        newlocon = addlocon( ptfig, newcon->name, newcon->ptsig, newcon->dir );
      else
      {
        newlocon = figcon;
	/*newlocon->SIG = newcon->ptsig;*/
	figcon = figcon->NEXT;
      }

      if( mode != 'C' && newcon->phcon )
      {
        if( !newcon->ptsig->PRCN )
          addlorcnet( newcon->ptsig );
	
        for( scannum = newcon->phcon ; scannum ; scannum = scannum->NEXT )
          setloconnode( newlocon, scannum->DATA );
      }
      
      /* del the newcon */
      freenum( newcon->phcon );
      mbkfree( newcon );
      
      free_line( line );

    }
  }
  while( type == MAL_CON );


  /* Internal description of figure */
  
  if( mode != 'P' )
  {
    while( type != MAL_EOF && type != MAL_CTC )
    {
      newlosig = NULL;
      newins   = NULL;
      
      switch( type )
      {
      case MAL_INS :
        compose  = MAL_CON;
        newins = decode_ins( line->NEXT, fname, mal_line );
        break;
        
      case MAL_TRS :
        compose = 0;
        decode_trs( ptfig, line->NEXT, fname, mal_line, version );
        break;

      case MAL_LOCAP :
        compose = 0;
        decode_locap( ptfig, line->NEXT, fname, mal_line, version );
        break;
        
      case MAL_LORES :
        compose = 0;
        decode_lores( ptfig, line->NEXT, fname, mal_line, version );
        break;

      case MAL_LOSELF :
        compose = 0;
        decode_loself( ptfig, line->NEXT, fname, mal_line, version );
        break;

      case MAL_SIG:
        if( version == 6 )
          compose = MAL_WIR | MAL_CAP ;
        else
        if( version == 5 )
          compose = MAL_WIR ;
        else
          compose = 0;
        newlosig = decode_sig( ptfig, line->NEXT, fname, mal_line, version );
        break;
        
      default:
        /* erreur : element non reconnu. */
        mal_error( fname, 
                   mal_line, 
                   "alcloadlofig6()", 
                   "Incorrect file structure.\n"
                 );
      }

      free_line( line );

      mal_line++;
      line = read_line( in, fname, mal_line );
      type = type_line( line, fname, mal_line );

      while ( type & compose )
      {
        if( newins )
        {
          complete_ins( ptfig, newins, line->NEXT, fname, mal_line );
        }
        else
        if( newlosig )
        {
          switch( type )
          {
          case MAL_WIR:
	    decode_lowire( newlosig, line->NEXT, fname, mal_line );
	    break;
	case   MAL_CAP:
	    decode_capa( newlosig, line->NEXT, fname, mal_line );
	    break;
          }
        }

        free_line( line );
        mal_line++;
        line = read_line( in, fname, mal_line );
        type = type_line( line, fname, mal_line );
      }

      /* terminate current element */
      if( newins )
        end_ins( ptfig, &tetemodele, newins, fname, mal_line );
    }

    if( version == 6 )
    {
      while( type == MAL_CTC )
      {
        decode_loctc(  ptfig, line->NEXT, fname, mal_line );

        free_line( line );
        mal_line++;
        line = read_line( in, fname, mal_line );
        type = type_line( line, fname, mal_line );
      }
    }

    if( type != MAL_EOF )
      mal_error( fname,
                 mal_line,
                 "alcloadlofig6()",
                 "Incorrect file structure.\n"
               );

    freelomodel( tetemodele );
    for( scansig = ptfig->LOSIG ; scansig ; scansig = scansig->NEXT )
      scansig->USER = delptype( scansig->USER, MALDEFINED );
  }

  free_line( line );

  delht( htsigname );
  ptfig->USER = delptype( ptfig->USER, SIGHT );

#ifdef ALDEBUG
  al_dbg_chkchain();
#endif

  lofigchain( ptfig );
}


/******* Function for debug ***************************************************/

#ifdef ALDEBUG

chain_list* al_dbg_addchain( head, data, ligne )
chain_list      *head;
void            *data;
int              ligne;
{
  int i;
  
  for( i=0 ; i < ALMAXCHAIN ; i++ )
    if( al_chain_lg[i] == 0 )
      break;
  
  if( i == ALMAXCHAIN )
  {
    printf( "\n*** Debuggage AL : tableau al_chain plein.\n" );
    exit(1);
  }

  al_chain_lg[i] = ligne;
  al_chain_pt[i] = addchain( head, data );

  return( al_chain_pt[i] );
}

void al_freechain( head )
chain_list      *head;
{
  int i;
  chain_list    *scan;

  for( scan = head ; scan ; scan = scan->NEXT )
  {
    for( i = 0 ; i < ALMAXCHAIN ; i++ )
      if( al_chain_pt[i] == scan )
        break;
    if( i == ALMAXCHAIN )
    {
      printf( "\n*** Debuggage AL : chain_list a liberer non trouvee.\n" );
      exit(1);
    }
    al_chain_lg[i] = 0;
  }
  freechain( head );
}

void al_dbg_chkchain( void )
{
  int i;

  for( i=0 ; i < ALMAXCHAIN ; i++ )
  {
    if( al_chain_lg[i] )
      printf( "*** Debuggage AL : Element alloue ligne %d non libere.\n",
              al_chain_lg[i]
            );
  }
}

void al_dbg_init( void )
{
  int i;

  printf( "*** Parser al v6.03. Debug\n" );
  for( i=0 ; i<ALMAXCHAIN ; i++ )
    al_chain_lg[i]=0;
}
#endif



