/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
 
/*****************************************************************
*               chaine de CAO & VLSI Alliance                    *
*                                                                *
*         fichier : alc_pars_p.c                                 *              
*         auteur(s) : V. POUILLEY                le : 01/01/92   *
*         modifie par : J.P CHAPUT                               *           
*         modifie par : L. MAURIN\R. PREVOT      le : 12/07/93   *           
*         modifie par : F. PETROT                                *           
*         numero de version actuelle : 4.00                      *
*         $Log: alc_pars_p.c,v $
*         Revision 1.5  2012/05/14 14:20:24  alliance
*         Updated GNU/FSF address (patch from Thibault North).
*
*         Revision 1.4  2006/05/04 16:29:52  xtof
*         Allowing more than 64 chars filenames ....
*
*         Revision 1.3  2004/09/04 09:11:52  ludo
*         - bug fix in AP parser (pb when scale_X is big)
*
*         Revision 1.2  2002/09/30 16:20:53  czo
*         support/users
*
*         Revision 1.1.1.1  2002/03/13 10:18:47  fred
*         Importing MBKAP sources into the new CVS tree
*                                                  *
*****************************************************************/
#include  <ctype.h>
#include  <string.h>

#include <mut.h>
#include <mph.h>
#ident "@(#)alliance physical view parser v4.00"


#define        TRUE               1
#define        FALSE              0


#define        MBKSZNAME       1024
#define        MAXLBUFFER       256
#define        MAXLBUFF         256

#define        EVAL               2
#define        EVER               3
#define        EOPEN              4
#define        ECLOSE             5
#define        ESETUP             6
#define        ELAYER             7
#define        EOPGEO             8
#define        ESYNTAX            9
#define        EORIENT            10
#define        EHEADER            11
#define        EMISSEOF           12
#define        ETYPESEG           13
#define        ETYPEVIA           14
#define        ENAMETRS           15
#define        ENBFIELDS          16
#define        EFILENAME          17
#define        EFILETYPE          18
#define        EBOUNDBOX          19
#define        EABUTMBOX          20
#define        EALLOCFIG          21
#define        ECOMPONENT         22
#define        ETRANSORIENT       23


#define  mc_nexttoken(p_head,p_next,error_code)       \
             if ( (p_next=strchr(p_head,','))==NULL ) \
                 alc_printerror(error_code);          \
             *(p_next++) = '\0'

#define  mc_nexttoken3(p_head,p_next,error_code)       \
             if ( (p_next=strchr(p_head,'\n'))==NULL ) \
                 alc_printerror(error_code);          \
             *(p_next++) = '\0'

#define  mc_isthrough(layer)         \
             (   ((layer)==TPOLY)    \
               ||((layer)==TALU1)    \
               ||((layer)==TALU2)    \
               ||((layer)==TALU3)    \
               ||((layer)==TALU4)    \
               ||((layer)==TALU5)    \
               ||((layer)==TALU6)    \
               ||((layer)==TALU7)    \
               ||((layer)==TALU8)    \
               ||((layer)==TALU9) )

#define  mc_isconseg(layer)         \
             (   ((layer)==CALU1)    \
               ||((layer)==CALU2)    \
               ||((layer)==CALU3)    \
               ||((layer)==CALU4)    \
               ||((layer)==CALU5)    \
               ||((layer)==CALU6)    \
               ||((layer)==CALU7)    \
               ||((layer)==CALU8)    \
               ||((layer)==CALU9) )

#define  mc_isdigit(c)  \
             ( (((c)>='0')&&((c)<='9')) ? 1 : 0 )


static struct {       FILE *file;
                      char  file_name[MAXLBUFF],
                            buff_line[MAXLBUFF],
                            mode;
                phfig_list *ptfig;
                      long  scale_x;
                      long  curr_line; } parser;


/************************ DECLARATION DES FONCTIONS *****************/
static long  alc_strtophtrs();
static void  alc_createphtrs();
static char  alc_strtoorient();
static char  alc_StrToTransOrient();
static char  alc_strtolayer();
static char  alc_strtotransf();
static char  alc_strtovia();
static void  alc_printwarn();
static void  alc_printerror();
static void  alc_old_load();
static void  alc_load();

#ifdef  CLEANUP
#define        TYPE_LNWELL        10
#define        TYPE_LNDIF         11
#define        TYPE_LPDIF         12
#define        TYPE_LCONT_DIF_N   13
#define        TYPE_LCONT_DIF_P   14

static void  alc_polar();
static long  alc_innwell();
#endif

# if 0
# define alc_scale_x( X ) (( (X) * SCALE_X ) / parser.scale_x)
# else
/* LUDO: 04/09/2004 */
static long alc_scale_x( long X )
{
  if ( SCALE_X != parser.scale_x )
  {
    return ( (long)( (double)X * (double)SCALE_X / (double)parser.scale_x ) );
  }
  else
  {
    return( X );
  }
}
# endif

/****************** alc_strtophtrs *******************/
static long  alc_strtophtrs(s_name,p_layer,lp_l,lp_w)
    char *s_name, *p_layer;
    long *lp_l, *lp_w;
{
    static char *p_l,*p_w,
                 buffer[MBKSZNAME];
            int  i;

#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtophtrs\` .");
#endif

    strcpy( buffer, s_name);

    if (  (buffer[0]!='T')||(buffer[2]!='_') ) return FALSE;
    switch( buffer[1] )    {
        case 'N' : *p_layer = NTRANS; break;
        case 'P' : *p_layer = PTRANS; break;
        default  : return FALSE;
        }
    p_l = buffer+3;

    if ( (p_w=strchr(p_l,'_'))==NULL ) return FALSE;
    *(p_w++) = '\0';

    for( i=0; p_l[i]!='\0'; i++)
        if ( !mc_isdigit(p_l[i]) )
            return( FALSE);
    for( i=0; p_w[i]!='\0'; i++)
        if ( !mc_isdigit(p_w[i]) )
            return( FALSE);

    *lp_l = atol( p_l);
    *lp_w = atol( p_w);

    return TRUE;
}


/******************* alc_createphtrs *****************/
static void  alc_createphtrs(layer,l,w)
    char  layer;
    long  l,w;
{
#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_createphtrs\` .");
#endif

    if ( parser.mode!='C' )    {
        /* definition des connecteurs. */
        /* source */
        (void)addphcon( parser.ptfig,  WEST, "Source",
                        alc_scale_x( 1 ),
                        alc_scale_x((l>>1)+1),
                        layer,
                        alc_scale_x(l-3) );

        (void)addphcon( parser.ptfig,  EAST, "Drain",
                        alc_scale_x( w+3 ),
                        alc_scale_x( (l>>1)+1 ),
                        layer,
                        alc_scale_x( l-3 ) );

        (void)addphcon( parser.ptfig, NORTH, "Gate",
                        alc_scale_x( (w>>1)+2 ),
                        alc_scale_x(l+1),
                        POLY,
                        alc_scale_x( 1 ) );

        (void)addphcon( parser.ptfig, SOUTH, "Gate",
                        alc_scale_x( (w>>1)+2 ),
                        alc_scale_x( 1 ),
                        POLY,
                        alc_scale_x( 1 ) );
        }

    if ( parser.mode!='P' )    {
        /* segment */
        (void)addphseg( parser.ptfig, layer,
                        alc_scale_x( w ),
                       alc_scale_x( (w>>1)+2 ), alc_scale_x(1),
                       alc_scale_x( (w>>1)+2 ),alc_scale_x(l+1), "*");
        defab( parser.ptfig, alc_scale_x( (w>>1)+2 ),  alc_scale_x(1),
                             alc_scale_x( (w>>1)+3 ), alc_scale_x(l+1) );
        }

    return;
    }


/********************* alc_strtoorient ***************/
static char  alc_strtoorient(s_orient)
    char *s_orient;
{
#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtoorient\` .");
#endif

    switch( s_orient[0] )    {
        case 'N' : if ( strcmp(s_orient,"NORD")!=0 )
                       alc_printerror( EORIENT);
                   return( NORTH);
        case 'S' : if ( strcmp(s_orient,"SUD")!=0 )
                       alc_printerror( EORIENT);
                   return( SOUTH);
        case 'E' : if ( strcmp(s_orient,"EST")!=0 )
                       alc_printerror( EORIENT);
                   return( EAST);
        case 'O' : if ( strcmp(s_orient,"OUEST")!=0 )
                       alc_printerror( EORIENT);
                   return( WEST);
        default  : alc_printwarn( EORIENT);
        }

    return SOUTH;
    }


static char  alc_StrToTransOrient(aStrTransOrient)
	char *aStrTransOrient;
{
	if (!strcmp(aStrTransOrient,"RIGHT")) return(RIGHT);
	if (!strcmp(aStrTransOrient, "LEFT")) return( LEFT);
	if (!strcmp(aStrTransOrient,   "UP")) return(   UP);
	if (!strcmp(aStrTransOrient, "DOWN")) return( DOWN);

	alc_printerror( ETRANSORIENT);
        return( 0 );
}


/********************* alc_strtoorient3 ***************/
static char  alc_strtoorient3(s_orient)
    char *s_orient;
{
#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtoorient\` .");
#endif

    switch( s_orient[0] )    {
        case 'N' : if ( strcmp(s_orient,"NORTH")!=0 )
                       alc_printerror( EORIENT);
                   return( NORTH);
        case 'S' : if ( strcmp(s_orient,"SOUTH")!=0 )
                       alc_printerror( EORIENT);
                   return( SOUTH);
        case 'E' : if ( strcmp(s_orient,"EAST")!=0 )
                       alc_printerror( EORIENT);
                   return( EAST);
        case 'W' : if ( strcmp(s_orient,"WEST")!=0 )
                       alc_printerror( EORIENT);
                   return( WEST);
        default  : alc_printwarn( EORIENT);
        }

    return SOUTH;
    }


/******************** alc_strtolayer *****************/
static char  alc_strtolayer(s_layer)
    char *s_layer;
{
int  i;
static char *TabNameLayer[] =
        { "CAISSON_N",
          "CAISSON_P",
          "NTIE", 
          "PTIE"  ,
          "DIFN",
          "DIFP",
          "NTRANS",
          "PTRANS",
          "POLY",      
          "TPOLY",      
          "POLY2",      
          "TPOLY2",      
          "ALU1",
          "T_ALU1",
          "ALU2",
          "T_ALU2",
          "ALU3",
          "TALU3"
        };
#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtolayer\` .");
#endif


    for( i=0; i<18; i++)
        if ( strcmp(s_layer,TabNameLayer[i])==0 )
            return( (char)i );

    alc_printwarn( ELAYER);
    return( NWELL);
}

/******************** strtolayer *****************/
static char strtolayer(s_layer)
char *s_layer;
{
int  i;
static char *TabNameLayer[] = {
   "NWELL",
   "PWELL",
   "NTIE",
   "PTIE",
   "NDIF",
   "PDIF",
   "NTRANS",
   "PTRANS",
   "NTRANS_FAST",
   "PTRANS_FAST",
   "NTRANS_HVIO",
   "PTRANS_HVIO",
   "POLY",
   "POLY2",
   "ALU1",
   "ALU2",
   "ALU3",
   "ALU4",
   "ALU5",
   "ALU6",
   "ALU7",
   "ALU8",
   "ALU9",
   "TPOLY",
   "TPOLY2",
   "TALU1",
   "TALU2",
   "TALU3",
   "TALU4",
   "TALU5",
   "TALU6",
   "TALU7",
   "TALU8",
   "TALU9",
   "CALU1",
   "CALU2",
   "CALU3",
   "CALU4",
   "CALU5",
   "CALU6",
   "CALU7",
   "CALU8",
   "CALU9"
};

#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtolayer\` .");
#endif


    for( i=0; i<LAST_LAYER; i++)
        if ( strcmp(s_layer,TabNameLayer[i])==0 )
            return( (char)i );

    alc_printwarn( ELAYER);
    return( NWELL);
}

/******************** alc_strtotransf ****************/
static char  alc_strtotransf(s_transf)
    char *s_transf;
{
            int  i;
    static char *TabNameTransf[] = { "NOSYM", "ROT_P",
                                     "SYMXY", "ROT_M",
                                     "SYM_X", "SY_RM",
                                     "SYM_Y", "SY_RP" };

#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtotransf\` .");
#endif

    for( i=0; i<8; i++)
        if ( strcmp(s_transf,TabNameTransf[i])==0 )
            return( (char)i );

    alc_printwarn( EOPGEO);
    return( NOSYM);
    }


/******************** alc_strtovia *******************/
static char  alc_strtovia(s_model)
    char *s_model;
{
            int  i;
static char *TabNameVIA[] = {
          "CONT_POLY",
          "CONT_DIF_N",
          "CONT_DIF_P",
          "CONT_BODY_N",
          "CONT_BODY_P",
          "CONT_VIA",
          "CONT_VIA2",
          "CONT_VIA3",
          "CONT_VIA4",
          "CONT_VIA5",
          "CONT_VIA6",
          "CONT_VIA7",
          "CONT_VIA8",
          "CONT_POLY2",
          "C_X_N",
          "C_X_P",
          "CONT_TURN1" ,
          "CONT_TURN2" ,
          "CONT_TURN3" ,
          "CONT_TURN4" ,
          "CONT_TURN5" ,
          "CONT_TURN6" ,
          "CONT_TURN7" ,
          "CONT_TURN8" ,
          "CONT_TURN9" 
        };

/* physical transistor flood fill macros */

#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtovia\` .");
#endif

    for( i=0; i<LAST_CONTACT+1; i++)
        if ( strcmp(s_model,TabNameVIA[i])==0 )
            return( (char)i );

    alc_printwarn( ETYPEVIA);

    return( CONT_POLY);
    }
/******************** strtovia *******************/
static char  strtovia(s_model)
    char *s_model;
{
            int  i;
    static char *TabNameVIA[] =
        { "CONT_POLY"  ,
          "CONT_DIF_N",
          "CONT_DIF_P" ,
          "CONT_BODY_N",
          "CONT_BODY_P",
          "CONT_VIA"   ,
          "CONT_VIA2"  ,
          "CONT_VIA3"  ,
          "CONT_VIA4"  ,
          "CONT_VIA5"  ,
          "CONT_VIA6"  ,
          "CONT_VIA7"  ,
          "CONT_VIA8"  ,
          "CONT_POLY2" ,
          "C_X_N"      ,
          "C_X_P"      ,
          "CONT_TURN1" ,
          "CONT_TURN2" ,
          "CONT_TURN3" ,
          "CONT_TURN4" ,
          "CONT_TURN5" ,
          "CONT_TURN6" ,
          "CONT_TURN7" ,
          "CONT_TURN8" ,
          "CONT_TURN9" 
    	};
#ifdef  JP_DEBUG
    puts( "- Execution de \`alc_strtovia\` .");
#endif

    for( i=0; i<LAST_CONTACT+1; i++)
        if ( strcmp(s_model,TabNameVIA[i])==0 )
            return( (char)i );

    alc_printwarn( ETYPEVIA);

    return( CONT_POLY);
}


/****************************************************/
/******************** alc_old_load *****************/
/****************************************************/ 

static void  alc_old_load(ptfig,fig_name,mode)
phfig_list *ptfig;
char *fig_name, mode;
{
    char  s_name[MBKSZNAME],
          s_date[MBKSZNAME],
          s_chain_mode[MBKSZNAME],
          c_ftype,
         *p_x     , *p_y    , *p_l     , *p_w,
         *p_orient, *p_layer, *p_name  , *p_type_con   ,
         *p_sens  , *p_model, *p_transf, *p_motif_index,
         *p_index , *p_next_index      , *p_endequi,
          layer, orient, after_inst;
    long  ab_index  , deb_index , NB_desc,
          xbb, ybb, dxbb, dybb, xab, yab, dxab, dyab,
          x  , y  , l   , w   , transf,
          exit_loop;

    after_inst   = TRUE;
    parser.ptfig = ptfig;
    parser.mode  = mode;
    if ( parser.ptfig==(phfig_list*)NULL )
        alc_printerror( EALLOCFIG);

    /* Detection des 'fichiers' transistors. */
    if ( alc_strtophtrs(fig_name,&layer,&l,&w) )
        alc_createphtrs( layer, l, w);

    if ( TRACE_MODE=='Y' )
        printf( "\n--- mbk --- parsing file : %s\n",
                                   parser.file_name );

    /* Lecture de la ligne d'en tete ( prefixe 'H '). */
    parser.curr_line = 2;
    if ( fscanf(parser.file,
                "H %1023[^,],%c,%22ld,%22ld,%1023[^,],%22ld,%1023[^,],",
                 s_name,   &c_ftype, &ab_index, &NB_desc,
                 s_date, &deb_index, s_chain_mode        )!=7 )
        alc_printerror( EHEADER);
    if ( strcmp(s_name,fig_name)!=0 ) alc_printerror( EFILENAME);
    if ( c_ftype!='P' )               alc_printerror( EFILETYPE);

    /* Lecture de la boundig box. */
    if ( fscanf(parser.file,"%22ld,%22ld,%22ld,%22ld, ",
                      &xbb, &ybb, &dxbb, &dybb )!=4 )
        alc_printerror( EBOUNDBOX);

    /* Lecture de l'abutment box. */
    if ( ab_index>=0 )    {
        if ( fscanf(parser.file,"%22ld,%22ld,%22ld,%22ld ",
                          &xab, &yab, &dxab, &dyab )!=4 )
            alc_printerror( EABUTMBOX);
        defab( parser.ptfig, 
               alc_scale_x(xab), alc_scale_x(yab),
               alc_scale_x( xab+dxab),
               alc_scale_x(yab+dyab) );
        }
    else
        defab( parser.ptfig, 
               alc_scale_x(xbb),
               alc_scale_x(ybb),
               alc_scale_x(xbb+dxbb),
               alc_scale_x(ybb+dybb) );

    /* Lecture ligne par ligne du reste du fichier */
    for( parser.curr_line=3, exit_loop=FALSE;
        fgets( parser.buff_line,MAXLBUFFER-1,
                                 parser.file )!=NULL;
                                 parser.curr_line++  ) {
        if ( strncmp(parser.buff_line,"EOF",3)==0 )
            { exit_loop = TRUE; break; }

        if ( parser.buff_line[1]!=' ' ) alc_printerror( ESYNTAX);

        switch( parser.buff_line[0] )    {

        /* Reconnaissance d'un connecteur. */
            case 'C' :
    { p_index = parser.buff_line+2;
      mc_nexttoken( p_index     , p_x         , ENBFIELDS);
      mc_nexttoken( p_x         , p_y         , ENBFIELDS);
      mc_nexttoken( p_y         , p_w         , ENBFIELDS);
      mc_nexttoken( p_w         , p_orient    , ENBFIELDS);
      mc_nexttoken( p_orient    , p_layer     , ENBFIELDS);
      mc_nexttoken( p_layer     , p_name      , ENBFIELDS);
      mc_nexttoken( p_name      , p_type_con  , ENBFIELDS);
      mc_nexttoken( p_type_con  , p_next_index, ENBFIELDS);
      mc_nexttoken( p_next_index, p_endequi   , ENBFIELDS);

      if ( strcmp(p_name,"*")==0 ) p_name = (char*)NULL;

      x      = atol( p_x);
      y      = atol( p_y);
      w      = atol( p_w);
      orient = alc_strtoorient( p_orient);
      layer  = alc_strtolayer(  p_layer);

      if ( (parser.mode=='C')||(!after_inst) ) continue;

      (void)addphcon( parser.ptfig, orient, p_name,alc_scale_x(x),
                      alc_scale_x(y),layer,alc_scale_x(w) );
    } break;

      /* Reconnaissance d'un segment. */
            case 'S' :
    { after_inst = FALSE;

      p_index = parser.buff_line+2;
      mc_nexttoken( p_index     , p_x         , ENBFIELDS);
      mc_nexttoken( p_x         , p_y         , ENBFIELDS);
      mc_nexttoken( p_y         , p_l         , ENBFIELDS);
      mc_nexttoken( p_l         , p_w         , ENBFIELDS);
      mc_nexttoken( p_w         , p_sens      , ENBFIELDS);
      mc_nexttoken( p_sens      , p_layer     , ENBFIELDS);
      mc_nexttoken( p_layer     , p_name      , ENBFIELDS);
      mc_nexttoken( p_name      , p_next_index, ENBFIELDS);
      mc_nexttoken( p_next_index, p_endequi   , ENBFIELDS);

      if (  (*p_sens != 'H')&&(*p_sens != 'V')       )
          alc_printerror( ETYPESEG);
      if ( strcmp(p_name,"*")==0 ) p_name = NULL;

      x     = atol( p_x);
      y     = atol( p_y);
      l     = atol( p_l);
      w     = atol( p_w);

      layer = alc_strtolayer( p_layer);

      if ( (parser.mode=='P')&&(!mc_isthrough(layer))&&(!mc_isconseg(layer))) continue;
      if ( (parser.mode=='C')&&( mc_isthrough(layer)) ) continue;


      (void)addphseg( parser.ptfig, layer,alc_scale_x(w), 
                      alc_scale_x(x), alc_scale_x(y),
	     (*p_sens=='H') ? alc_scale_x(x+l) : alc_scale_x(x),
	     (*p_sens=='V') ? alc_scale_x(y+l) : alc_scale_x(y),
                                             p_name );
    } break;

      /* Reconnaissance d'une instance. */
            case 'I' :
    { after_inst = TRUE;

      p_index = parser.buff_line+2;
      mc_nexttoken( p_index     , p_x         , ENBFIELDS);
      mc_nexttoken( p_x         , p_y         , ENBFIELDS);
      mc_nexttoken( p_y         , p_name      , ENBFIELDS);
      mc_nexttoken( p_name      , p_model     , ENBFIELDS);
      mc_nexttoken( p_model     , p_transf    , ENBFIELDS);
      mc_nexttoken( p_transf    , p_next_index, ENBFIELDS);
      mc_nexttoken( p_next_index, p_endequi   , ENBFIELDS);

      x       = atol( p_x);
      y       = atol( p_y);

      transf  = alc_strtotransf( p_transf);

      if ( parser.mode=='P' ) continue;

      (void)addphins( parser.ptfig, p_model, p_name,
                                             transf,
                                          alc_scale_x(x),
                                          alc_scale_x(y) );
    } break;

      /* Reconnaissance des motifs ( VIA et references ). */
            case 'M' :
    { after_inst = FALSE;

      p_index = parser.buff_line+2;
      mc_nexttoken( p_index       , p_x           , ENBFIELDS);
      mc_nexttoken( p_x           , p_y           , ENBFIELDS);
      mc_nexttoken( p_y           , p_name        , ENBFIELDS);
      mc_nexttoken( p_name        , p_model       , ENBFIELDS);
      mc_nexttoken( p_model       , p_motif_index , ENBFIELDS);
      mc_nexttoken( p_motif_index , p_next_index  , ENBFIELDS);
      mc_nexttoken( p_next_index  , p_endequi     , ENBFIELDS);

      x = atol( p_x);
      y = atol( p_y);

      if ( strncmp(p_model,"REF_",3)==0 ) {
          if ( parser.mode=='C' ) continue;

          if ( p_name[0]         =='\0' ) p_name = NULL;
		  if ( strcmp(p_name,"*")==0    ) p_name = NULL;
          (void)addphref( parser.ptfig, p_model, p_name,
                                              alc_scale_x(x),
                                              alc_scale_x(y) );
          }
      else {
          if ( parser.mode=='P' ) continue;

          (void)addphvia( parser.ptfig, alc_strtovia(p_model),
                                                    alc_scale_x(x),
                                                    alc_scale_x(y), 0, 0, NULL );
          }
    } break;

      /* Reconnaissance d'un transistor. */
            case 'T' :
    { after_inst = FALSE;

      p_index = parser.buff_line+2;
      mc_nexttoken( p_index     , p_x         , ENBFIELDS);
      mc_nexttoken( p_x         , p_y         , ENBFIELDS);
      mc_nexttoken( p_y         , p_name      , ENBFIELDS);
      mc_nexttoken( p_name      , p_model     , ENBFIELDS);
      mc_nexttoken( p_model     , p_transf    , ENBFIELDS);
      mc_nexttoken( p_transf    , p_next_index, ENBFIELDS);
      mc_nexttoken( p_next_index, p_endequi   , ENBFIELDS);

      x = atol( p_x);
      y = atol( p_y);

      if ( parser.mode=='P' ) continue;

      if ( strcmp(p_name,"*")==0 ) p_name = NULL;
      transf = alc_strtotransf( p_transf);

      if ( !alc_strtophtrs(p_model,&layer,&l,&w) ) {
          alc_printwarn( ENAMETRS);

          (void)addphins( parser.ptfig, p_model, p_name,
                                                 transf,
                                              alc_scale_x(x),
                                              alc_scale_x(y) );
          continue;
          }

      switch( transf ) {
          case NOSYM : ;
          case SYM_X : ;
          case SYM_Y : ;
          case SYMXY :
      (void)addphseg( parser.ptfig, layer,alc_scale_x(w),
                                          alc_scale_x(x),
                                          alc_scale_x(y),
                                          alc_scale_x(x),
                                      alc_scale_x((y+l)),
                                             p_name ); break;
          case ROT_P : ;
          case ROT_M : ;
          case SY_RP : ;
          case SY_RM :
      (void)addphseg( parser.ptfig, layer,alc_scale_x(w),
                                          alc_scale_x(x),
                                          alc_scale_x(y),
                                      alc_scale_x((x+l)),
                                          alc_scale_x(y),
                                             p_name ); break;
            }
      } break;

            /* Composant inconnu. */
            default  : alc_printerror( ECOMPONENT);
            }
        }
    if ( !exit_loop ) alc_printerror( EMISSEOF);
#ifdef  CLEANUP
    alc_polar( parser.ptfig);
#endif

    }

/******************************************************************/
/************************ alc_loadvers3 ***************************/
/******************************************************************/

static void  alc_load(ptfig,fig_name,mode,setup)
          phfig_list *ptfig;
          char *fig_name, mode;
          long  setup;
{
    char  s_name[MBKSZNAME],
          s_date[MBKSZNAME],
          c_ftype,
          *p_x     , *p_y    , *p_x2    , *p_y2    , *p_w,
          *p_orient, *p_layer, *p_name  , *p_model , *p_transf, 
          *p_index , *p_type , *p_endequi,
          layer, orient;
    long  xab, yab, dxab, dyab,
          x  , y  , x2  , y2  , w   , transf,
          exit_loop, vl_tmp;

    parser.ptfig = ptfig;
    parser.mode  = mode;
    if ( parser.ptfig==(phfig_list*)NULL )
        alc_printerror( EALLOCFIG);

    /* Lecture de la ligne d'en tete ( prefixe 'H '). */
    parser.curr_line++;
/* 
** --> MODIF LUDO
*/
    if ( setup > 3 )
    {
      if (fscanf(parser.file,
                 "H %1023[^,],%c,%1023[^,],%22ld\n", s_name, &c_ftype, s_date, &parser.scale_x)!=4)
          alc_printerror(EHEADER);
    }
    else
    {
      if (fscanf(parser.file, "H %1023[^,],%c,%1023[^\n]\n", s_name, &c_ftype, s_date)!=3)
          alc_printerror(EHEADER);
    }

    if ( strcmp(s_name,fig_name)!=0 ) alc_printerror( EFILENAME);
    if ( c_ftype!='P' )               alc_printerror( EFILETYPE);

    /* Lecture de l'abutment box. */
    parser.curr_line++;
    if (fscanf(parser.file,"A %22ld,%22ld,%22ld,%22ld ", &xab, &yab, &dxab, &dyab) != 4)
       alc_printerror(EABUTMBOX);
    defab(parser.ptfig, alc_scale_x(xab), alc_scale_x(yab),
          alc_scale_x(dxab), alc_scale_x(dyab));
    /* Lecture du reste du fichier ligne par ligne */
    for( parser.curr_line++, exit_loop=FALSE;
        fgets( parser.buff_line,MAXLBUFFER-1,
                                 parser.file )!=NULL;
                                 parser.curr_line++  ) {
        if ( strncmp(parser.buff_line,"EOF",3)==0 )
            { exit_loop = TRUE; break; }

        if ( parser.buff_line[1]!=' ' ) alc_printerror( ESYNTAX);

        switch( parser.buff_line[0] )    {

        /* Reconnaissance d'un connecteur. */
            case 'C' :{
	p_x = parser.buff_line+2;
        mc_nexttoken( p_x         , p_y         , ENBFIELDS);
        mc_nexttoken( p_y         , p_w         , ENBFIELDS);
        mc_nexttoken( p_w         , p_name      , ENBFIELDS);
        mc_nexttoken( p_name      , p_index     , ENBFIELDS);
        mc_nexttoken( p_index     , p_orient    , ENBFIELDS);
        mc_nexttoken( p_orient    , p_layer     , ENBFIELDS);
        mc_nexttoken3( p_layer     , p_endequi   , ENBFIELDS);


     if ( strcmp(p_name,"*")==0 ) p_name = (char*)NULL;

      x      = atol( p_x);
      y      = atol( p_y);
      w      = atol( p_w);
      orient = alc_strtoorient3( p_orient);
      layer  = strtolayer(  p_layer);

      if (parser.mode=='C') continue;

      (void)addphcon( parser.ptfig, orient, p_name, alc_scale_x(x),alc_scale_x(y),
                      layer , alc_scale_x(w) );
    } break;

      /* Reconnaissance d'un segment. */
            case 'S' :
    { p_x = parser.buff_line+2;
      mc_nexttoken( p_x         , p_y         , ENBFIELDS);
      mc_nexttoken( p_y         , p_x2        , ENBFIELDS);
      mc_nexttoken( p_x2        , p_y2        , ENBFIELDS);
      mc_nexttoken( p_y2        , p_w         , ENBFIELDS);
      mc_nexttoken( p_w         , p_name      , ENBFIELDS);
      mc_nexttoken( p_name      , p_orient    , ENBFIELDS);
      mc_nexttoken( p_orient    , p_layer     , ENBFIELDS);
      mc_nexttoken3( p_layer     , p_endequi   , ENBFIELDS);
 
    if ( strcmp(p_name,"*")==0 ) p_name = NULL;

      x     = atol( p_x);
      y     = atol( p_y);
      x2    = atol( p_x2);
      y2    = atol( p_y2);
      w     = atol( p_w);

      layer  =           strtolayer( p_layer );
	  orient = alc_StrToTransOrient( p_orient);

      if ( (parser.mode=='P')&&(!mc_isthrough(layer)&&(! mc_isconseg(layer))) ) continue;
      /* MODIF LUDO 4/2/00 */
      if ( (parser.mode=='C')&&( mc_isthrough(layer) || mc_isconseg(layer))) continue;

	  if (orient == LEFT) { vl_tmp = x; x = x2; x2 = vl_tmp; }
	  if (orient == DOWN) { vl_tmp = y; y = y2; y2 = vl_tmp; }

      (void)addphseg( parser.ptfig, layer,
                      alc_scale_x(w),alc_scale_x(x),alc_scale_x(y),
                      alc_scale_x(x2),alc_scale_x(y2),p_name );
    } break;

      /* Reconnaissance d'une instance. */
            case 'I' :
    {  p_x = parser.buff_line+2;
       mc_nexttoken( p_x         , p_y         , ENBFIELDS);
      mc_nexttoken( p_y         , p_model     , ENBFIELDS);
      mc_nexttoken( p_model     , p_name      , ENBFIELDS);
      mc_nexttoken( p_name      , p_transf    , ENBFIELDS);
      mc_nexttoken3( p_transf    , p_endequi   , ENBFIELDS);

      x       = atol( p_x);
      y       = atol( p_y);

      transf  = alc_strtotransf( p_transf);

      if ( parser.mode=='P' ) continue;

      (void)addphins( parser.ptfig, p_model, p_name,
                                             transf,
                                          alc_scale_x(x),
                                          alc_scale_x(y) );
    } break;

      /* Reconnaissance des references . */
            case 'R' :
    {p_x = parser.buff_line+2;
     mc_nexttoken( p_x           , p_y           , ENBFIELDS);
     mc_nexttoken( p_y           , p_model       , ENBFIELDS);
     mc_nexttoken( p_model       , p_name        , ENBFIELDS);
     mc_nexttoken3( p_name        , p_endequi     , ENBFIELDS);

      x = atol( p_x);
      y = atol( p_y);

          if ( parser.mode=='C' ) continue;

          if ( p_name[0]         =='\0' ) p_name = NULL;
          if ( strcmp(p_name,"*")==0    ) p_name = NULL;
    (void)addphref( parser.ptfig, p_model, p_name,
                                              alc_scale_x(x),
                                              alc_scale_x(y) );
    } break;
     
     /* Reconnaissance des VIA . */
            case 'V' :
    {p_x = parser.buff_line+2;
     mc_nexttoken( p_x           , p_y           , ENBFIELDS);
     mc_nexttoken( p_y           , p_type        , ENBFIELDS);

     if ( setup > 5 )
     {
       mc_nexttoken( p_type, p_name, ENBFIELDS );
       mc_nexttoken3( p_name, p_endequi, ENBFIELDS );

       if ( ( p_name[ 0 ] == '\0'    ) ||
            ( ! strcmp(p_name, "*" ) ) ) p_name = (char *)0;
     }
     else
     {
       p_name = (char *)0;
       mc_nexttoken3( p_type, p_endequi, ENBFIELDS);
     }

      x = atol( p_x);
      y = atol( p_y);

          if ( parser.mode=='P' ) continue;

          (void)addphvia(parser.ptfig, strtovia(p_type), 
                         alc_scale_x(x), alc_scale_x(y), 0, 0, p_name);
    } break;
     /* Reconnaissance des BIG-VIA . */
            case 'B' :
    {p_x = parser.buff_line+2;

     mc_nexttoken( p_x         , p_y           , ENBFIELDS);
     mc_nexttoken( p_y         , p_x2        , ENBFIELDS);
     mc_nexttoken( p_x2        , p_y2        , ENBFIELDS);
     mc_nexttoken( p_y2        , p_type        , ENBFIELDS);

     if ( setup > 5 )
     {
       mc_nexttoken( p_type, p_name, ENBFIELDS );
       mc_nexttoken3( p_name, p_endequi, ENBFIELDS );

       if ( ( p_name[ 0 ] == '\0'    ) ||
            ( ! strcmp(p_name, "*" ) ) ) p_name = (char *)0;
     }
     else
     {
       p_name = (char *)0;
       mc_nexttoken3( p_type, p_endequi, ENBFIELDS);
     }

      x = atol( p_x);
      y = atol( p_y);
      x2 = atol( p_x2 );
      y2 = atol( p_y2 );

          if ( parser.mode=='P' ) continue;

          (void)addphvia(parser.ptfig, strtovia(p_type), 
                         alc_scale_x(x), alc_scale_x(y), 
                         alc_scale_x(x2), alc_scale_x(y2), p_name);
    } break;

            /* Composant inconnu. */
            default  : alc_printerror( ECOMPONENT);
            }
        }
    if ( !exit_loop ) alc_printerror( EMISSEOF);

}



/************************** alc_printwarn **************************/
static void  alc_printwarn(warn_code)
    long  warn_code;
{
    fprintf( stderr, "\n*** mbk warning *** alcloadphfig : ");
    switch( warn_code )  
      { case ELAYER    :
            fprintf( stderr, "invalid layer"); break;
        case EOPGEO    :
            fprintf( stderr, "invalid geometric operation"); break;
        case EORIENT   :
            fprintf( stderr, "unknow orientation"); break;
        case ENAMETRS :
            fprintf( stderr, "invalid transistor"); break;
        case ETYPEVIA  :
            fprintf( stderr, "invalid via"); break;
        default : fprintf( stderr, "unknow warning");
      }
    fprintf( stderr, "\n( line %ld parsing %s )\n",
                                  parser.curr_line,
                                  parser.file_name );
}


/**************************** alc_printerror *************************/
static void  alc_printerror(error_code)
    long  error_code;
{
    fprintf( stderr, "\n*** mbk error *** alcloadphfig : ");
    switch( error_code )    
    {   case EVER      :
            fprintf( stderr, "invalid version of alc"); break;
        case EOPEN     :
            fprintf( stderr, "unable to open file : %s .\n",
                                   parser.file_name);EXIT(1);
        case ECLOSE    :
            fprintf( stderr, "can\'t close file : %s",
                               parser.file_name);break;
        case ESETUP    :
            fprintf( stderr, "invalid setup\'s number"); break;
        case ESYNTAX   :
            fprintf( stderr, "syntax error"); break;
        case EMISSEOF  :
            fprintf( stderr, "missing EOF"); break;
        case ETYPESEG  :
            fprintf( stderr, "segment neither H nor V"); break;
        case ETRANSORIENT:
            fprintf( stderr, "Unknow transistor orientation"); break;
        case EHEADER   :
            fprintf( stderr, "unexpected header"); break;
        case ENBFIELDS :
            fprintf( stderr, "missing or non-numeric field"); break;
        case EFILENAME :
            fprintf( stderr, "invalid file name"); break;
        case EFILETYPE :
            fprintf( stderr, "bad file type"); break;
        case EBOUNDBOX :
            fprintf( stderr, "can\'t read bounding box"); break;
        case EABUTMBOX :
            fprintf( stderr, "can\'t read abutment box"); break;
        case EALLOCFIG :
            fprintf( stderr, "ptfig not allocated"); break;
        case ECOMPONENT :
            fprintf( stderr, "invalid component"); break;
        default : fprintf( stderr, "unknow error code %ld", error_code );
    }
    fprintf( stderr, "\n( line %ld parsing %s )\n",parser.curr_line,
                                                   parser.file_name ); EXIT( 1);
}


#ifdef  CLEANUP

#define  access_seg(seg_list)  ( (phseg_list*)(seg_list->DATA) )
#define  access_via(via_list)  ( (phvia_list*)(via_list->DATA) )

/****************************** alc_polar ****************************/
static void  alc_polar(ptfig)
    phfig_list *ptfig;
{
    phvia_list *pvia;
    phseg_list *pseg;
    ptype_list *LP_NWELL, *LP_NDIF,       *LP_PDIF,
                          *LP_CONT_DIF_N, *LP_CONT_DIF_P,
               *lPPHSEG,
               *lPPHVIA;

#ifdef  JP_DEBUG
    puts( "- Execution de \'alc_polar\' .");
#endif


    LP_NWELL = LP_NDIF       = LP_PDIF
             = LP_CONT_DIF_N = LP_CONT_DIF_P = (ptype_list*)NULL;

    /* Transformation des segments PTIE et NTIE en PDIF et NDIF.
       On memorise dans les listes LP_PDIF et LP_NDIF non seule-
       ment les TIE mais aussi les NDIF et PDIF.
                               ( en cas d'erreur de conception )
       On en profite pour lister les segments NWELL. */
    for( pseg=ptfig->PHSEG; pseg!=(phseg_list*)NULL;
                            pseg =pseg->NEXT        )
        switch( pseg->LAYER )    
        {   case NWELL : LP_NWELL = addptype( LP_NWELL,
                                              TYPE_LNWELL,
                                              (void*)pseg );
                         break;
            case NTIE  : pseg->LAYER = NDIF;
            case NDIF  : LP_NDIF = addptype( LP_NDIF    ,
                                             TYPE_LNDIF ,
                                             (void*)pseg );
                         break;
            case PTIE  : pseg->LAYER = PDIF;
            case PDIF  : LP_PDIF = addptype( LP_PDIF    ,
                                             TYPE_LPDIF ,
                                             (void*)pseg );
                         break;
        }

    /* Transformation des contacts BODY_P et BODY_N en DIF_P et
       DIF_N. On memorise dans les listes LP_CONT_DIF_P et
       LP_CONT_DIF_N non seulement les BODY mais aussi les DIF_N
       et les DIF_P. ( en cas d'erreur de conception ) */
    for( pvia=ptfig->PHVIA; pvia!=(phvia_list*)NULL;
                            pvia =pvia->NEXT        )
        switch( pvia->TYPE )  
        {   case CONT_BODY_N : pvia->TYPE = CONT_DIF_N;
            case CONT_DIF_N  : LP_CONT_DIF_N = addptype(
                                        LP_CONT_DIF_N   ,
                                        TYPE_LCONT_DIF_N,
                                        (void*)pvia      );
                        break;
            case CONT_BODY_P : pvia->TYPE = CONT_DIF_P;
            case CONT_DIF_P  : LP_CONT_DIF_P = addptype(
                                         LP_CONT_DIF_P   ,
                                         TYPE_LCONT_DIF_P,
                                         (void*)pvia      ); break;
        }

    /* Restauration des NTIE. Un segment de type NTIE est detecte
       lorsque un segment NDIF est inclus dans du NWELL. */
    for( lPPHSEG=LP_NDIF; lPPHSEG!=(ptype_list*)NULL;
                          lPPHSEG =lPPHSEG->NEXT     )
        if ( alc_innwell(access_seg(lPPHSEG)->X1,
                         access_seg(lPPHSEG)->Y1,
                                        LP_NWELL ))
            access_seg(lPPHSEG)->LAYER = NTIE;

    /* Restauration des PTIE. Un segment de type PTIE est detecte
       lorsque un segment PDIF est hors de tout NWELL.
       ( substrat dope P ). */
    for( lPPHSEG=LP_PDIF; lPPHSEG!=(ptype_list*)NULL;
                          lPPHSEG =lPPHSEG->NEXT     )
        if ( !alc_innwell(access_seg(lPPHSEG)->X1,
                          access_seg(lPPHSEG)->Y1,
                                         LP_NWELL ))
            access_seg(lPPHSEG)->LAYER = PTIE;

    /* Restauration des BODY_N.
       ( contact DIF_N inclus dans le NWELL ) */
    for( lPPHVIA=LP_CONT_DIF_N; lPPHVIA!=(ptype_list*)NULL;lPPHVIA =lPPHVIA->NEXT)
    {
#ifdef  JP_DEBUG
        puts( "\tVIA N en cours d\'examen :");
        viewphvia( access_via(lPPHVIA));
        fgetc( stdin);
#endif
        if ( alc_innwell(access_via(lPPHVIA)->XVIA,
                         access_via(lPPHVIA)->YVIA,LP_NWELL))
        {
            access_via(lPPHVIA)->TYPE = CONT_BODY_N;
#ifdef  JP_DEBUG
            viewphvia( access_via(lPPHVIA));
#endif
        }
    }

    /* Restauration des BODY_P.
       ( contact DIF_P hors du NWELL ) */
    for( lPPHVIA=LP_CONT_DIF_P; lPPHVIA!=(ptype_list*)NULL;lPPHVIA =lPPHVIA->NEXT)
    {
#ifdef  JP_DEBUG
        puts( "\tVIA P en cours d\'examen :");
        viewphvia( access_via(lPPHVIA));
        fgetc( stdin);
#endif
        if ( !alc_innwell(access_via(lPPHVIA)->XVIA,
                          access_via(lPPHVIA)->YVIA,LP_NWELL ))
        {
            access_via(lPPHVIA)->TYPE = CONT_BODY_P;
#ifdef  JP_DEBUG
            viewphvia( access_via(lPPHVIA));
#endif
        }
    }
}


/********************* Fonction : 'alc_innwell' *******************/
static long  alc_innwell(x,y,LP_NWELL)
          long  x, y;
    ptype_list *LP_NWELL;
{
    ptype_list *lPPHSEG;

#ifdef  JP_DEBUG
    puts( "- Execution de \'alc_innwell\' .");
#endif

    for( lPPHSEG=LP_NWELL; lPPHSEG!=(ptype_list*)NULL;
                           lPPHSEG =lPPHSEG->NEXT     ) 
    {
#ifdef  JP_DEBUG
        puts( "\tExamen d\'un NWELL.");
#endif  
        if (  (access_seg(lPPHSEG)->TYPE==LEFT )
			||(access_seg(lPPHSEG)->TYPE==RIGHT) )    
        {   if (   (x>=access_seg(lPPHSEG)->X1)
                 &&(x<=access_seg(lPPHSEG)->X2)
                 &&(y>=access_seg(lPPHSEG)->Y1
                      -access_seg(lPPHSEG)->WIDTH/2)
                 &&(y<=access_seg(lPPHSEG)->Y1
                      +access_seg(lPPHSEG)->WIDTH/2) )
                return TRUE;
        }
        else   if ( (x>=access_seg(lPPHSEG)->X1
                      -access_seg(lPPHSEG)->WIDTH/2)
                    &&(x<=access_seg(lPPHSEG)->X1
                      +access_seg(lPPHSEG)->WIDTH/2)
                    &&(y>=access_seg(lPPHSEG)->Y1)
                    &&(y<=access_seg(lPPHSEG)->Y2) )
               return TRUE;
    }

#ifdef  JP_DEBUG
    puts( "\tHors de tout NWELL.");
#endif
    return FALSE;
}
#endif



/****************************************************************/
/***************** Fonction : alcloadphfig **********************/
/****************************************************************/

void alcloadphfig(ptfig, fig_name, mode)
phfig_list *ptfig;
char *fig_name, mode;
{
long setup; 
char buffer[BUFSIZ];

    sprintf( parser.file_name, "%s.%s", fig_name, IN_PH);
	if (                !filepath(fig_name,IN_PH)
	    || !(parser.file=mbkfopen(fig_name,IN_PH,READ_TEXT)))
		alc_printerror(EOPEN);
	parser.curr_line = 1; 
        parser.scale_x   = 1;

	if (!fgets(buffer, BUFSIZ, parser.file))
		alc_printerror(ESYNTAX);

	/* printf( "%s\n", buffer); */
	if (sscanf(buffer, "V ALLIANCE : %22ld ", &setup) == 1) {
   	if (setup >= 3)
	       alc_load(ptfig, fig_name, mode,setup);
		else
			alc_printerror(ESETUP);
	} else if (sscanf(buffer, "V ALLIANCE 2.2 SETUP : %22ld ", &setup) == 1) {
   	if (setup == 2)
		   alc_old_load(ptfig, fig_name, mode);
		else
			alc_printerror(ESETUP);
	} else 
		alc_printerror(EVER);

   if (fclose(parser.file) != 0)
		alc_printerror(ECLOSE);
}
