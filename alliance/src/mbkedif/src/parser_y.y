%{

/*----------------------------------------------------------------------*/
/*                      @(#) Edif Parser For MBK.                       */
/*----------------------------------------------------------------------*/
/*                      version 1.00:   by Fred Petrot.                 */
/*                      version 4.02:   by Mokhtar Hirech (Jan. 92)     */
/*                      version 4.03:   by Olivier BEAURIN (Aou. 93)    */
/*                      version 4.04:   by Olivier BEAURIN (Sep. 93)    */
/*                      version 4.06:   by Olivier BEAURIN (Nov. 93)    */
/*----------------------------------------------------------------------*/

#include <mut.h>
#include <mlo.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------\
                     Les defines
\---------------------------------------------------------*/
#define MAXSTRLEN       100

/*---------------------------------------------------------\
                     Globals variables
\---------------------------------------------------------*/
static lofig_list     *YY_LOFIG                = (lofig_list *) NULL;
static lofig_list     *YY_PTFIG                = (lofig_list *) NULL;
static losig_list     *YY_PTSIG                = (losig_list *) NULL;
static chain_list     *YY_PORTLIST_SIG         = (chain_list *) NULL;
static chain_list     *YY_PORTLIST_SIG_TMP     = (chain_list *) NULL;
static loins_list     *YY_PTINS                = (loins_list *) NULL;
static chain_list     *YY_V_PORTINSTANCE       = (chain_list *) NULL;

static char           *YY_RIPPER_IN            = (char *)NULL;
static char           *YY_RIPPER_OUT           = (char *)NULL;

static char            YY_BLACKBOX             = 'N';  
static int             YY_BUS_WIDTH            = 0;
static char            YY_DIRECTION            = UNKNOWN;
static int             YY_INDEX                = 0;
static char            YY_SIGTYPE              = 'Z'; /* Z=not alr. defined, I=interne, E=externe */
static int             YY_NOLOAD_LOCON         = 0;
static int             YY_NOLOAD_FIG           = 0;

static int             YY_BASE_CELL;
static char           *YY_FIGNAME;
static char           *YY_INS_MODELNAME;
static char            YY_ARRAYPORTNAME[MAXSTRLEN];
static char            YY_PORTLIST_NUM;
static char            YY_PORTLIST;
static char           *YY_RIPPER_CELL = (char *) NULL;

/*---------------------------------------------------------\
                     Functions
\---------------------------------------------------------*/
static  void        REnamealloc         ( );
static  void        del_patch           ( );
static  void        sig_merge           ( );

static  char        bus_nameref         ( );
static  char        CheckCadenceBusError( );
static  char       *CheckCadenceNetError( );
static  lofig_list *get_figure_pt       ( );
static  char        is_portinstance     ( );
extern  char       *edif_busname        ( );

extern  int         yylineno;

%}

%start r_edif

%union
	{
		long    lval;
		char    sval[100];
	}

%token  <sval>   T_EDIF           T_EDIFVERSION           T_EDIFLEVEL 
%token  <sval>   T_TECHNOLOGY     T_CELLTYPE              T_CELLTYPE_GENERIC
%token  <sval>   T_CELLTYPE_TIE   T_CELLTYPE_RIPPER       T_PROPERTY 
%token  <sval>   T_VIEWTYPE       T_INTERFACE             T_CONTENTS
%token  <sval>   T_JOINED         T_INSTANCE              T_VIEWREF
%token  <sval>   T_PORT           T_DIRECTION             T_INOUT
%token  <sval>   T_PORTREF        T_NETLIST               T_KEYWORDMAP
%token  <sval>   T_KEYWORDLEVEL   T_EXTERNAL              T_LIBRARYREF
%token  <sval>   T_OUTPUT         T_INSTANCEREF           T_MEMBER
%token  <sval>   T_RENAME         T_NAME                  T_STRING
%token  <sval>   T_PORTINSTANCE   T_LIBRARY               T_CELL
%token  <sval>   T_VIEW           T_NET                   T_CELLREF
%token  <sval>   T_INPUT          T_STATUS                T_IDENT
%token  <sval>   T_ARRAY          T_RENAME_STRING         T_PORTLIST

/* for FLEX */
%type   <sval>   r_namedef        r_libraryref            r_name
%type   <sval>   r_external       r_extinterface          r_library
%type   <sval>   r_celltype       r_property              r_viewref
%type   <sval>   r_finextcell     r_finextinterf          r_fincell
%type   <sval>   r_extview        r_port                  r_view
%type   <sval>   r_contents       r_finnet                r_finview
%type   <sval>   r_array_joined   r_joined                r_interface
%type   <sval>   r_portlist       r_array_portref         r_port_joined
%type   <sval>   r_fininterface   r_finport               r_direction
%type   <sval>   r_anything       r_shorted_portref       r_opt_property
%type   <sval>   l_anything       r_finport_joined        r_edif

%token  <lval>   T_INTEGER

/*----------------------------------------------------------------------*
* All identifiers starting with T_ are considered as (T)okens.          *
* "         "                   YY_ are (V)ariables for identifiers.    *
* "         "                   r_ are (R)ules                          *
* "         "                   l_ are (L)ists of rules                 *
* "         "                   l_fin.. : list of optional parameters   *
* "         "                   r_fin.. : rules for optional parameters *
*                                         for previous rule: ...        *
------------------------------------------------------------------------*/

%%

r_edif :
	  T_EDIF r_namedef r_edifversion r_ediflevel l_externals l_finedif ')'
	;

r_edifversion :
	  T_EDIFVERSION T_INTEGER T_INTEGER T_INTEGER ')'
		{ 
			if (($2 != 2) || ($3 != 0) || ($4 != 0))
			{
				fprintf (stderr, "*** mbk error *** edifloadlofig : unable to read this version of edif.\n");
				exit (1);
			}
		}
	;

r_ediflevel :
	  T_EDIFLEVEL T_INTEGER ')'
		{ 
			if ($2 != 0)
			{
				fprintf (stderr, "*** mbk error *** edifloadlofig : EdifLevel incorrect ( must be 0 ).\n");
				exit (1);
			}
		}
	;

/*---------------------------------------------------------\
                     EXTERNAL
\---------------------------------------------------------*/
l_externals :
	  /* empty */
	| l_externals r_external
	;

r_external :
	  T_EXTERNAL r_namedef r_ediflevel l_finexternal ')'
	;

l_finexternal :
	  /* empty */
	| l_finexternal r_finexternal
	;

r_finexternal :
	  r_extcell
	;

r_extcell :
	  T_CELL r_namedef
		{
			lofig_list *ptf;
			char       *cell_name;
                        
        
			cell_name = namealloc($2);
                        
			if( !( ptf = get_figure_pt( cell_name ) ) )  
			{
				YY_PTFIG = addlofig( cell_name );
				YY_NOLOAD_FIG = 0; 
				YY_NOLOAD_LOCON = 0;
			}
			else
			{
				if( cell_name == YY_FIGNAME )
					YY_PTFIG = YY_LOFIG;
				else
					YY_PTFIG = ptf;

				YY_BASE_CELL = incatalog( cell_name );

				if( ptf->LOCON && YY_BASE_CELL || ptf->LOSIG )
					YY_NOLOAD_FIG = 1; 
				else
					YY_NOLOAD_FIG = 0; 

				YY_NOLOAD_LOCON = ( ptf->LOCON ) ? 1 : 0;

			}
			YY_INDEX = 0;
		}
	  r_celltype l_finextcell ')'
		{
			YY_PTFIG->MODE = 'A';
		}
	;

l_finextcell :
	  /* empty */
	| l_finextcell r_finextcell
	;

r_finextcell :
	  r_extview
	| r_property
	;

r_extview :
	  T_VIEW r_namedef T_VIEWTYPE T_NETLIST ')' r_extinterface ')'
		{
			if( YY_BASE_CELL && YY_PTFIG == YY_LOFIG )
				YY_BLACKBOX = 'Y';
		}
	;

r_extinterface :
	  T_INTERFACE l_finextinterf ')'
	;

l_finextinterf :
	  /* empty */
	| l_finextinterf r_finextinterf
	;

r_finextinterf :
	  r_port
	| r_property
	;

/*---------------------------------------------------------\
                     MAIN
\---------------------------------------------------------*/
l_finedif :
	  /* empty */
	| l_finedif r_library
	;

r_library :
	  T_LIBRARY r_namedef r_ediflevel l_finlibrary ')'
	;

l_finlibrary :
	  /* empty */
	| l_finlibrary r_finlibrary
	;

r_finlibrary :
	  r_cell
	;

r_cell :
	  T_CELL r_namedef
		{ 
			lofig_list      *ptf;
			char            *cell_name;
                        
			cell_name = namealloc($2);

			YY_BASE_CELL = 0;

			if (!(ptf = get_figure_pt(cell_name)))
			{
				YY_PTFIG = addlofig ($2);
				YY_NOLOAD_FIG = 0;
				YY_NOLOAD_LOCON = 0;
			}
			else
			{
				if( cell_name == YY_FIGNAME )
					YY_PTFIG = YY_LOFIG;
				else
					YY_PTFIG = ptf;

				YY_BASE_CELL = incatalog( cell_name );

				if( ptf->LOCON && YY_BASE_CELL || ptf->LOSIG )
					YY_NOLOAD_FIG = 1; 
				else
					YY_NOLOAD_FIG = 0; 

				YY_NOLOAD_LOCON = (ptf->LOCON) ? 1 : 0;
			}
			YY_INDEX = 0;
		}
	  r_celltype l_fincell ')'
		{
			loins_list *inst_pt;

			if (YY_NOLOAD_FIG == 0)
			{
				char       *array_cname;
				char       *array_sname;
				locon_list *ptcon;
				char        new_sig_name[MAXSTRLEN];
				char        end;
				locon_list *YY_INS_PTCON  = (locon_list *) NULL;
				locon_list *YY_PTCON      = (locon_list *) NULL;
                losig_list *sigvss;
                losig_list *sigvdd;
                locon_list *ptlocon;

                for( ptlocon = YY_PTFIG->LOCON; ptlocon; ptlocon = ptlocon->NEXT )
                {
                    if( isvss( ptlocon->NAME ) || isvdd( ptlocon->NAME ) )
                    {
                        break;
                    }
                }

                if( !ptlocon )
                {
                    sigvss = addlosig (YY_PTFIG,
                                       ++YY_INDEX,
                                       addchain( NULL, namealloc( VSS ) ),
                                       EXTERNAL );
                    sigvdd = addlosig (YY_PTFIG,
                                       ++YY_INDEX,
                                       addchain( NULL, namealloc( VDD ) ),
                                       EXTERNAL );
                    addlocon( YY_PTFIG, namealloc( VDD ), sigvdd, IN );
                    addlocon( YY_PTFIG, namealloc( VSS ), sigvss, IN );
                }
				/* scan all instances */
				for( YY_PTINS = YY_PTFIG->LOINS; YY_PTINS; YY_PTINS=YY_PTINS->NEXT ) 
				{
					if( YY_PTINS->FIGNAME != YY_RIPPER_CELL )
					{
						for (YY_INS_PTCON  = YY_PTINS->LOCON; 
						     YY_INS_PTCON != (locon_list *) NULL;
						     YY_INS_PTCON  = YY_INS_PTCON->NEXT)
						{
                            if( isvss( YY_INS_PTCON->NAME ) )
                            {
                                YY_INS_PTCON->SIG = sigvss;
                            }
                            if( isvdd( YY_INS_PTCON->NAME ) )
                            {
                                YY_INS_PTCON->SIG = sigvdd;
                            }
							if (YY_INS_PTCON->SIG == NULL)
							{
								sprintf(new_sig_name,"unused_%d",++YY_INDEX);
								if ( YY_PTINS->USER == (struct ptype *) NULL ||
								    !is_portinstance( YY_PTINS, YY_INS_PTCON->NAME ) )
								{
									fprintf (stderr, "*** mbk warning *** edifloadlofig :\n");
									fprintf (stderr, "Connector '%s' ",YY_INS_PTCON->NAME);
									fprintf (stderr, "of instance '%s' ",YY_PTINS->INSNAME);
									fprintf (stderr, "has no signal in figure ");
									fprintf (stderr, "'%s'.\n",YY_PTFIG->NAME);
									fprintf(stderr, "==> Creation of signal '%s'\n", new_sig_name); 
								}
	
								YY_INS_PTCON->SIG = addlosig (YY_PTFIG, YY_INDEX,
								                              addchain( NULL, namealloc( new_sig_name ) ),
									                              INTERNAL );
							}
						}
					}


					/* scan all figure's connectors */
					for  (YY_PTCON  = YY_PTFIG->LOCON; 
					      YY_PTCON != (locon_list *) NULL;
					      YY_PTCON  = YY_PTCON->NEXT)
					{
						if (YY_PTCON->SIG == NULL) 
						{
							sprintf(new_sig_name,"unused_%d",++YY_INDEX);

							fprintf (stderr, "*** mbk warning *** ");
							fprintf (stderr, "edifloadlofig : \n");
							fprintf (stderr, "connector '%s' ",YY_PTCON->NAME);
							fprintf (stderr, "of figure '%s' ",YY_PTFIG->NAME);
							fprintf (stderr, "has no signal\n");
							fprintf(stderr, "==> Creation of signal '%s'\n", new_sig_name); 

							YY_PTCON->SIG = addlosig (YY_PTFIG, YY_INDEX,
							                          addchain( NULL, namealloc( new_sig_name ) ),
							                          EXTERNAL );

						}
					}
				}
			}
			/* free all instance->USER that are filled with chain_list data */
			for( inst_pt = YY_PTFIG->LOINS; inst_pt; inst_pt=inst_pt->NEXT )
			{
				if( inst_pt->USER != (struct ptype *)NULL )
				{
					freechain( (chain_list *)inst_pt->USER );
					inst_pt->USER = (struct ptype *)NULL;
				}
			}
		}
	;

r_celltype :
	  T_CELLTYPE T_CELLTYPE_GENERIC ')'
	| T_CELLTYPE T_CELLTYPE_TIE ')'
	| T_CELLTYPE T_CELLTYPE_RIPPER ')'
		{
			YY_RIPPER_CELL = YY_PTFIG->NAME;
		}
	;

l_fincell :
	  /* empty */
	| l_fincell r_fincell
	;

r_fincell :
	  r_view
	| r_property
	;

r_property :
	  T_PROPERTY l_anything ')'
	;

r_opt_property :
          {
          }
	  | r_property
          {
          }
	;

r_view :
	  T_VIEW r_namedef T_VIEWTYPE T_NETLIST ')' r_interface l_finview ')'
		{
			if( YY_BASE_CELL && YY_PTFIG == YY_LOFIG )
				YY_BLACKBOX = 'Y';
		}
	;

l_finview :
	  /* empty */
	| l_finview r_finview
	;

r_finview :
	  r_contents
	| r_property
	;

r_contents :
	T_CONTENTS l_fincontents ')'
		{ 
			YY_PTFIG->MODE = 'A';
		}
	;

l_fincontents :
	  /* empty */
	| l_fincontents r_fincontents
	;

r_fincontents :
	  r_instance
	| r_net
	;

r_instance :
	  T_INSTANCE r_namedef r_viewref r_opt_property
		{
			if( YY_NOLOAD_FIG == 0 )
			{
				lofig_list *YY_INS_PTMODEL  = (lofig_list *) NULL;
				chain_list *YY_INS_SIGCHAIN = (chain_list *) NULL;
				locon_list *YY_INS_PTCON    = (locon_list *) NULL;

				YY_INS_PTMODEL = getlofig( YY_INS_MODELNAME, 'P' );
				YY_INS_SIGCHAIN = NULL;

				for( YY_INS_PTCON = YY_INS_PTMODEL->LOCON;
				     YY_INS_PTCON != NULL; 
				     YY_INS_PTCON = YY_INS_PTCON->NEXT) 
					YY_INS_SIGCHAIN = addchain( YY_INS_SIGCHAIN, NULL );

				YY_PTINS = addloins( YY_PTFIG, $2, YY_INS_PTMODEL, YY_INS_SIGCHAIN );

				YY_V_PORTINSTANCE = addchain(YY_V_PORTINSTANCE, NULL);
			}
		}
	  l_portinstance l_anything ')'
		{
			YY_PTINS->USER = (struct ptype *)YY_V_PORTINSTANCE->DATA; 
		}
	;

r_viewref :
	  T_VIEWREF r_namedef ')'
	| T_VIEWREF r_namedef r_cellref ')'
	;

l_portinstance :
	  /* empty */
	| l_portinstance r_portinstance
	;

r_portinstance :
	  T_PORTINSTANCE r_namedef l_anything ')'
		{
			YY_V_PORTINSTANCE->DATA = addchain( YY_V_PORTINSTANCE->DATA, (void *)namealloc( $2 ) );
		}
	;

r_cellref :
	  T_CELLREF r_namedef ')'
		{ 
			YY_INS_MODELNAME = namealloc( $2 ); 
		}
	| T_CELLREF r_namedef r_libraryref ')'
		{ 
			YY_INS_MODELNAME = namealloc( $2 ); 
		}
	;

r_libraryref :
	  T_LIBRARYREF r_namedef ')'
	;

r_net :
	  T_NET r_namedef 
		{
			if (YY_NOLOAD_FIG == 0)
			{
				YY_PTSIG = addlosig (YY_PTFIG, ++YY_INDEX, addchain (NULL, CheckCadenceNetError( $2 ) ), 'Z');
				YY_SIGTYPE = INTERNAL;
			}
			YY_BUS_WIDTH = 1;
		}
	  r_joined l_finnet ')'
		{ 
			if (YY_NOLOAD_FIG == 0)
			{
				YY_PTSIG->TYPE = YY_SIGTYPE; 
			}
		}
	| T_NET T_ARRAY r_namedef T_INTEGER ')'
		{
			if (YY_NOLOAD_FIG == 0)
			{
				int         i;
				int         begin;
				int         end;
				char        net_name[MAXSTRLEN];
				losig_list *YY_SIG_TMP = (losig_list *) NULL;

				if( $4 <= 0 )
				{ 
					fprintf(stderr, "*** mbk error *** edifloadlofig : Array width must be > 0\n");
					exit( 1 ); 
				}

				for (i=0; i < $4; i++)
				{
					if( (CheckCadenceBusError ($3, &begin, &end)) && (i <= abs( end - begin) ) )
						sprintf(&net_name[0], "%s %d", YY_ARRAYPORTNAME, ( begin <= end ) ? begin + i : begin - i); 
					else
						sprintf(&net_name[0], "%s %d", $3, i); 

					YY_PTSIG = addlosig (YY_PTFIG, ++YY_INDEX, addchain (NULL, namealloc (net_name)), 'Z');
					YY_SIGTYPE = INTERNAL;
					YY_BUS_WIDTH = $4;
				}

				YY_SIG_TMP = YY_PTSIG;
				for (i=0; i<YY_BUS_WIDTH; i++)
				{
        			YY_PORTLIST_SIG = addchain(YY_PORTLIST_SIG, (void *)YY_PTSIG);
        			YY_PTSIG = YY_PTSIG->NEXT;
				}
				YY_PTSIG = YY_SIG_TMP;
				YY_PORTLIST_SIG_TMP = YY_PORTLIST_SIG;
			}
		}
	r_array_joined l_finnet ')'
		{
			if (YY_NOLOAD_FIG == 0)
			{
				int         i;
				losig_list *ptsig;

				freechain( (chain_list *)YY_PORTLIST_SIG );
				YY_PORTLIST_SIG = (chain_list *)NULL;

				ptsig = YY_PTSIG;
				for (i = YY_BUS_WIDTH - 1; i >= 0; i--)
				{
					ptsig->TYPE = YY_SIGTYPE;
					ptsig = ptsig->NEXT;
				}
			}
		}
	;

l_finnet :
	  /* empty */
	| l_finnet r_finnet
	;

r_finnet :
	  r_property
	;

r_array_joined :
	  T_JOINED l_array_joined ')'
	;

l_array_joined :
	  /* empty */
	| l_array_joined r_array_joined
	;

r_array_joined :
	  r_array_portref
	| r_portlist
	;

r_portlist :
	  T_PORTLIST 
		{
			YY_PORTLIST = 1;
			YY_PORTLIST_NUM = 0;
			YY_PORTLIST_SIG = YY_PORTLIST_SIG_TMP;
		}
	  l_list_portref ')'
		{
			YY_PORTLIST = 0;

			if (YY_PORTLIST_NUM < YY_BUS_WIDTH)
			{
				fprintf (stderr, "*** mbk error *** edifloadlofig : ");
				fprintf (stderr, "PortList width < signal bus '%s' width ( line %d ).\n", 
				                  edif_busname( getsigname(YY_PTSIG)), yylineno );
				exit (1);
			}
		}
	;

l_list_portref :
	  /* empty */
	| l_list_portref r_portref
	;

r_array_portref :
	  T_PORTREF r_namedef ')'
		{ 
			if (YY_NOLOAD_FIG == 0)
			{
				int         i;
				losig_list  *ptsig;
				char         port_name[MAXSTRLEN];
				int          begin, end;
				locon_list  *YY_PTCON = (locon_list *) NULL;

				ptsig = YY_PTSIG;
				if (bus_nameref(YY_PTFIG->LOCON, $2, YY_BUS_WIDTH))
				{
					for (i = YY_BUS_WIDTH - 1; i >= 0; i--)
					{
						if ( (CheckCadenceBusError ($2, &begin, &end)) && (i <= abs( end - begin) ) )
							sprintf(&port_name[0], "%s %d", YY_ARRAYPORTNAME, (begin <= end) ? begin + i : begin - i); 
						else
							sprintf(&port_name[0], "%s %d", $2, i); 

						YY_PTCON = getlocon (YY_PTFIG, &port_name[0]);
						YY_PTCON->SIG = ptsig;
						ptsig = ptsig->NEXT;
					}
					YY_SIGTYPE = EXTERNAL;
				}
				else
				{
					fprintf (stderr, "*** mbk error *** edifloadlofig :\n"); 
					fprintf (stderr, "Connector '%s' in figure '%s' do not refer to an ARRAY construct",
					                 $2, YY_PTFIG->NAME); 
					fprintf (stderr, "\n    or it is a bus with incompatible width than Signal Bus\n");
					exit (1);
				}
			}
		}
	  | T_PORTREF r_namedef T_INSTANCEREF r_namedef ')' ')'
		{ 
			if (YY_NOLOAD_FIG == 0)
			{
				int         i;
				char        port_name[MAXSTRLEN];
				char       *port_name_alloc;
				losig_list *ptsig; 
				int         begin, end;
				locon_list *YY_PTCON = (locon_list *) NULL;

				ptsig = YY_PTSIG;
				YY_PTINS = getloins (YY_PTFIG, $4);

				if( bus_nameref(YY_PTINS->LOCON, $2, YY_BUS_WIDTH))
				{
					if(  ( CheckCadenceBusError ($2, &begin, &end))
					   &&( (YY_BUS_WIDTH - 1) == abs(end - begin) ) )
						sprintf(&port_name[0], "%s %d", YY_ARRAYPORTNAME, ( begin <= end ) ? end : begin ); 
					else
						sprintf(&port_name[0], "%s %d", $2, YY_BUS_WIDTH - 1); 

					port_name_alloc = namealloc( port_name );

					for (YY_PTCON = YY_PTINS->LOCON; YY_PTCON; YY_PTCON = YY_PTCON->NEXT)
						if( port_name_alloc == YY_PTCON->NAME)
							break;

					if (YY_PTCON == (locon_list *) NULL)
					{
						fprintf (stderr, "*** mbk error *** edifloadlofig :");
						fprintf (stderr, " bus connector %s not found", $2);
						fprintf (stderr, " in instance %s", $4);
						fprintf (stderr, " of figure %s\n", YY_PTINS->FIGNAME);
						exit( 1 );
					}
					else
					{
						for (i = YY_BUS_WIDTH - 1; (i >= 0) && YY_PTCON; i--)
						{
							YY_PTCON->SIG = ptsig;
							ptsig         = ptsig->NEXT;
							YY_PTCON      = YY_PTCON->NEXT;
						}
					}
				}
				else
				{
					port_name_alloc = namealloc( CheckCadenceNetError( $2 ) );

					for (YY_PTCON = YY_PTINS->LOCON; YY_PTCON; YY_PTCON = YY_PTCON->NEXT)
						if( YY_PTCON->NAME == port_name_alloc )
							break;
					fprintf (stderr, "*** mbk error *** edifloadlofig : ");  
					if (YY_PTCON)
					{
						fprintf (stderr, "Connector '%s' in instance '%s' of figure '%s' do not refer", 
						                  port_name_alloc, $4, YY_PTFIG->NAME); 
						fprintf (stderr, "\n    to an ARRAY construct or it is a bus with" );
						fprintf (stderr, "incompatible width than Signal Bus/n");
						exit( 1 );
					}
					else
					{
						fprintf (stderr, "Connector '%s' not found on instance '%s'\n", port_name_alloc, $4);   
						exit( 1 );
					} 
				}
			}
		}
	;
  
r_joined :
	  T_JOINED l_finjoined ')'
	;

l_finjoined :
	  /* empty */
	| l_finjoined r_finjoined
	;

r_finjoined :
	  r_portref
	;

r_portref :
	  T_PORTREF r_namedef ')'
		{ 
			if (YY_NOLOAD_FIG == 0)
			{
				locon_list *YY_PTCON = (locon_list *) NULL;

				YY_PTCON = getlocon( YY_PTFIG, CheckCadenceNetError( $2 ) );

				if (!YY_PORTLIST)
					YY_PTCON->SIG = YY_PTSIG;
				else
					YY_PTCON->SIG = (losig_list *)YY_PORTLIST_SIG->DATA;

				YY_SIGTYPE = EXTERNAL;
				if (YY_PORTLIST) 
				{
					if (++YY_PORTLIST_NUM < YY_BUS_WIDTH) 
						YY_PORTLIST_SIG = YY_PORTLIST_SIG->NEXT;
					else
						if (YY_PORTLIST_NUM > YY_BUS_WIDTH)
						{
							fprintf (stderr, "*** mbk error *** edifloadlofig : ");
							fprintf (stderr, "PortList width > signal bus '%s' width ( line %d ).\n", 
							         edif_busname(getsigname(YY_PTCON->SIG)), yylineno );
							exit( 1 );
						}
				}
			}
		}
	| T_PORTREF r_namedef T_INSTANCEREF r_namedef ')' ')'
		{ 
			if (YY_NOLOAD_FIG == 0)
			{
				char       *port_name_ref;
				locon_list *YY_PTCON = (locon_list *) NULL;

				port_name_ref = namealloc( CheckCadenceNetError( $2 ) );

				YY_PTINS = getloins (YY_PTFIG, $4);
				for (YY_PTCON = YY_PTINS->LOCON; YY_PTCON; YY_PTCON = YY_PTCON->NEXT)
				{
					if ( port_name_ref == YY_PTCON->NAME)
					{
						if( !YY_PORTLIST )
							YY_PTCON->SIG = YY_PTSIG;
						else 
							YY_PTCON->SIG = (losig_list *)YY_PORTLIST_SIG->DATA;
						break;
					}
				}

			if (YY_PTCON == NULL)
			{
				fprintf (stderr, "*** mbk error *** edifloadlofig :");
				fprintf (stderr, " connector %s not found", $2);
				fprintf (stderr, " in instance %s", $4);
				fprintf (stderr, " of figure %s\n", YY_PTINS->FIGNAME);
				exit( 1 );
			}

			if (YY_PORTLIST) 
				if (++YY_PORTLIST_NUM < YY_BUS_WIDTH) 
				YY_PORTLIST_SIG = YY_PORTLIST_SIG->NEXT;
				else
					if (YY_PORTLIST_NUM > YY_BUS_WIDTH)
					{
						fprintf (stderr, "*** mbk error *** edifloadlofig : ");
						fprintf (stderr, "PortList width > signal bus '%s' width ( line %d ).\n", 
						                  edif_busname(getsigname(YY_PTCON->SIG)),
						                  yylineno );
						exit (1);
					}
			}
		}
	| T_PORTREF T_MEMBER r_namedef T_INTEGER ')' ')'
		{
			if (YY_NOLOAD_FIG == 0)
			{
				char        port_name[MAXSTRLEN];
				int         begin, end;
				locon_list *YY_PTCON = (locon_list *) NULL;

				if ( (CheckCadenceBusError ($3, &begin, &end))
				  && ($4 <= abs( begin - end ) ) )
					sprintf(&port_name[0], "%s %d", YY_ARRAYPORTNAME, ( begin <= end ) ? begin + $4 : begin - $4 ); 
				else
					sprintf(&port_name[0], "%s %d", $3, $4); 

				YY_PTCON = getlocon (YY_PTFIG, port_name);
				if (!YY_PORTLIST)
					YY_PTCON->SIG = YY_PTSIG;
				else
					YY_PTCON->SIG = (losig_list *)YY_PORTLIST_SIG->DATA;

				YY_SIGTYPE = EXTERNAL;
				if (YY_PORTLIST) 
					if (++YY_PORTLIST_NUM < YY_BUS_WIDTH) 
						YY_PORTLIST_SIG = YY_PORTLIST_SIG->NEXT;
					else
						if (YY_PORTLIST_NUM > YY_BUS_WIDTH)
						{
							fprintf (stderr, "*** mbk error *** edifloadlofig : ");
							fprintf (stderr, "PortList width > signal bus '%s' width ( line %d ).\n", 
							                  edif_busname(getsigname(YY_PTCON->SIG)),
							                  yylineno );
							exit (1);
						}
			}
		}
	| T_PORTREF T_MEMBER r_namedef T_INTEGER ')' T_INSTANCEREF r_namedef ')' ')'
		{

			if (YY_NOLOAD_FIG == 0)
			{
				char        port_name[MAXSTRLEN];
				int         begin, end;
				locon_list *YY_PTCON    = (locon_list *) NULL;
				char       *YY_PORTNAME = (char *) NULL;

				YY_PTINS = getloins (YY_PTFIG, $7);

				if ( (CheckCadenceBusError ($3, &begin, &end) )
				  && ($4 <= abs( begin - end ) ) )
					sprintf(&port_name[0], "%s %d", YY_ARRAYPORTNAME, ( begin <= end ) ? begin + $4 : begin - $4 ); 
				else
					sprintf(&port_name[0], "%s %d", $3, $4); 

				YY_PORTNAME = namealloc (&port_name[0]);
				for (YY_PTCON = YY_PTINS->LOCON; YY_PTCON; YY_PTCON = YY_PTCON->NEXT)
				{
					if (YY_PORTNAME == YY_PTCON->NAME)
					{
						if (!YY_PORTLIST)
							YY_PTCON->SIG = YY_PTSIG;
						else 
							YY_PTCON->SIG = (losig_list *)YY_PORTLIST_SIG->DATA;
						break;
					}
				}

				if (YY_PTCON == NULL)
				{
					fprintf (stderr, "*** mbk error *** edifloadlofig : ");
					fprintf (stderr, "connector %s not found ", port_name);
					fprintf (stderr, " in instance %s", $7);
					fprintf (stderr, " of figure %s\n", YY_PTINS->FIGNAME);
					exit (1);
				}

				if (YY_PORTLIST) 
					if (++YY_PORTLIST_NUM < YY_BUS_WIDTH) 
						YY_PORTLIST_SIG = YY_PORTLIST_SIG->NEXT;
					else
						if (YY_PORTLIST_NUM > YY_BUS_WIDTH)
						{
							fprintf (stderr, "*** mbk error *** edifloadlofig : ");
							fprintf (stderr, "PortList width > signal bus '%s' width ( line %d ).\n", 
							                 edif_busname(getsigname(YY_PTCON->SIG)),
							                 yylineno );
							exit (1);
						}
			}
		}
	;

r_interface :
	  T_INTERFACE l_fininterface ')'
	;

l_fininterface  :
	  /* empty */
	| l_fininterface r_fininterface
	;

r_fininterface :
	  r_port
	| r_port_joined
	| r_property
	;

r_port :
	  T_PORT r_namedef l_finport ')'
		{ 
			if (YY_NOLOAD_LOCON == 0)
			{
				addlocon( YY_PTFIG, CheckCadenceNetError( $2 ), NULL ,YY_DIRECTION );
			}
		}
	| T_PORT T_ARRAY r_namedef T_INTEGER ')' l_finport ')'
		{
			if (YY_NOLOAD_LOCON == 0)
			{
				char port_name    [MAXSTRLEN];
				int i;
				int begin;
				int end;

				if( $4 <= 0 )
				{ 
					fprintf(stderr, "*** mbk error *** edifloadlofig : Array width must be > 0\n"); 
					exit( 1 ); 
				}

				if ( (CheckCadenceBusError ($3, &begin, &end)) && ( (abs (end - begin) + 1) == $4) )
				{
					for (i=0; i < $4; i++)
					{
						sprintf(&port_name[0], "%s %d", YY_ARRAYPORTNAME, ( begin <= end ) ? begin + i : begin - i ); 
						addlocon (YY_PTFIG, &port_name[0], NULL ,YY_DIRECTION);
					}
				}
				else
				{
					for (i=0; i < $4; i++)
					{
						sprintf(&port_name[0], "%s %d", $3, i); 
						addlocon (YY_PTFIG, &port_name[0], NULL ,YY_DIRECTION);
					}
				}
			}
		}
	;

l_finport :
	  /* empty */
	| l_finport r_finport
	;

r_finport :
	  r_direction
	| r_property
	;

r_direction :
	  T_DIRECTION T_INOUT   ')' 
		{ 
			YY_DIRECTION = UNKNOWN; 
		}
	| T_DIRECTION T_INPUT   ')' 
		{ 
			YY_DIRECTION = IN; 
		}
	| T_DIRECTION T_OUTPUT ')' 
		{ 
			YY_DIRECTION = OUT; 
		}
	;

l_anything :
	  /* empty */
        { }
	| l_anything r_anything
        { }
	;

r_anything :
	  l_anything ')'
	| r_namedef
	;

r_namedef :
	  T_IDENT       
		{ 
			strcpy($$, $1); 
		}
	| T_INTEGER
		{ 
			char YY_TMP [MAXSTRLEN];

			sprintf(YY_TMP, "%ld", $1); 
			strcpy($$, YY_TMP); 
		}
	| T_RENAME T_IDENT T_RENAME_STRING ')'
		{
			strcpy($$, $2); 
		}
	| T_RENAME r_name T_RENAME_STRING ')'
		{
			strcpy($$, $2);
		}
	;

r_name :
	  T_NAME T_IDENT ')'
		{
			strcpy($$, $2); 
		}
	;

r_port_joined :
	  T_JOINED l_finport_joined ')'
	;

l_finport_joined :
	  /* empty */
	| l_finport_joined r_finport_joined
	;

r_finport_joined :
	  r_shorted_portref
	;

r_shorted_portref :
	  T_PORTREF r_namedef ')'
	;

%%

extern  char            yytext[];
extern  FILE            *yyin;


/*----------------------------------------------------------------------*/
/*      bus_nameref: checks that name ref is bus name ref and with      */
/*      the same width. It returns 1 if ok.                             */
/*      Do not forget that locon list is in reverse order.              */
/*----------------------------------------------------------------------*/

/*---------------------------------------------------------\
                     bus_nameref
\---------------------------------------------------------*/
static char bus_nameref( ptlocon, bnameref, bus_width )
locon_list *ptlocon;
char       *bnameref;
int         bus_width;
{
	int         i, n;
	char        port_name[MAXSTRLEN];
	char       *pname;
	locon_list *loconp;
	int         begin, end;

	bus_width --;
	if ( (!CheckCadenceBusError( bnameref, &begin, &end ) ) || (bus_width != abs( end - begin ) ) )
	{
		strcpy( YY_ARRAYPORTNAME, bnameref );
		end = bus_width;
	}

	sprintf(&port_name[0], "%s %d", YY_ARRAYPORTNAME, end ); 

	pname = namealloc( &port_name[0] );

	for (loconp = ptlocon; loconp != (locon_list *) NULL; loconp = loconp->NEXT)
		if (loconp->NAME == pname)
			break;

	if (loconp == (locon_list *) NULL)
		return 0;

	for( loconp = loconp->NEXT; loconp != (locon_list *) NULL; loconp = loconp->NEXT )
	{
		sprintf( &port_name[0], "%s %d", YY_ARRAYPORTNAME, ( begin <= end ) ? --end : ++end );
		if ( ( loconp->NAME == namealloc( port_name ) ) && bus_width >= 0)
			bus_width--;
		else
			break;
	}

	return( bus_width == 0 );

} /* end of bus_nameref */

/*---------------------------------------------------------\
                     edifloadlofig
\---------------------------------------------------------*/
void edifloadlofig( ptfig, figname, mode )
lofig_list *ptfig;
char       *figname;
char        mode;
{
	char        filename[120];
	lofig_list *pt;
	losig_list *ptsig;
	losig_list *ptsignext;
	loins_list *ptins;
	loins_list *ptinsnext;

	static char Vierge = 1; /* first time */

	yylineno = 0;

    YY_RIPPER_IN  = (char *)NULL;
    YY_RIPPER_OUT = (char *)NULL;

    YY_LOFIG                = (lofig_list *) NULL;
    YY_PTFIG                = (lofig_list *) NULL;
    YY_PTSIG                = (losig_list *) NULL;
    YY_PORTLIST_SIG         = (chain_list *) NULL;
    YY_PORTLIST_SIG_TMP     = (chain_list *) NULL;
    YY_PTINS                = (loins_list *) NULL;
    YY_V_PORTINSTANCE       = (chain_list *) NULL;
    YY_BLACKBOX             = 'N';  
    YY_BUS_WIDTH            = 0;
    YY_DIRECTION            = UNKNOWN;
    YY_INDEX                = 0;
    YY_SIGTYPE              = 'Z';
    YY_NOLOAD_LOCON         = 0;
    YY_NOLOAD_FIG           = 0;
    YY_RIPPER_CELL = (char *) NULL;

	sprintf( filename, "%s/%s.%s", WORK_LIB, figname, IN_LO );
	if( ( yyin = mbkfopen( figname, IN_LO, READ_TEXT ) ) == NULL )
	{
		fprintf( stderr, "*** mbk error *** edifloadlofig : can't open file %s\n", filename );
		exit( 1 );
	}

	if( Vierge == 0 )
		yyrestart( yyin );
	Vierge = 0;

	YY_LOFIG = ptfig;
	YY_FIGNAME = figname;
	YY_V_PORTINSTANCE = (chain_list *) NULL;

	if( yyparse() != 0 )
	{
		fprintf( stderr, "*** mbk error *** edifloadlofig : abnormal parsing for %s\n", filename );
		exit( 1 );
	}

	if( fclose( yyin ) != 0 )
	{
		fprintf(stderr, "*** mbk error *** edifloadlofig : can't close file %s\n", filename);
		exit(1);
	}

	if (YY_BLACKBOX == 'Y')
	{
		ptfig->MODE = 'A';
		for (pt = HEAD_LOFIG; pt != NULL; pt = pt->NEXT)
		{
			if (pt == ptfig)
				break;
			for (ptins = ptfig->LOINS; ptins != NULL; ptins = ptins->NEXT)
			{
				if( pt->NAME ==  ptins->FIGNAME )
					break;
			}
			if (ptins == NULL)
				dellofig( pt->NAME );
		}

		for( pt = HEAD_LOFIG; pt != (lofig_list *) NULL; pt = pt->NEXT )
		{
			if( pt != ptfig )
			{
				for( ptsig = pt->LOSIG; ptsig != (losig_list *) NULL; ptsig = ptsig->NEXT )
					free( ptsig );
				pt->LOSIG = (losig_list *) NULL;

				for( ptins = pt->LOINS; ptins != (loins_list *) NULL; ptins = ptins->NEXT )
					delloins( pt , ptins->INSNAME );
				pt->MODE = 'P';
			}
		}
	}

	for (pt = HEAD_LOFIG; pt != NULL; pt = pt->NEXT) 
		if( pt != ptfig ) 
			pt->MODE = (pt->LOSIG == NULL) ? 'P' : 'A';
        else
			pt->MODE = 'A';

	REnamealloc( );

	del_patch( ptfig, 1 );


} /* end of edifloadlofig */

/*---------------------------------------------------------\
                     yyerror
\---------------------------------------------------------*/
int yyerror( String )
char *String;
{
        fprintf( stderr,
                 "*** mbk error *** edifloadlofig : %s at ligne %d on '%s'\n",
                 String, yylineno + 1, yytext );
        exit( 1 );
} /* end of yyerror */

/*---------------------------------------------------------\
                     yywrap
\---------------------------------------------------------*/
int yywrap()
{
    return 1;
} /* end of yywrap */

/*---------------------------------------------------------\
                     get_figure_pt
\---------------------------------------------------------*/
static lofig_list *get_figure_pt( Name )
char *Name;
{
	lofig_list *pt;

	for( pt = HEAD_LOFIG; pt; pt = pt->NEXT )
		if( pt->NAME == Name )
			return pt;

	return (lofig_list *)NULL;

} /* end of get_figure_pt */

/*---------------------------------------------------------\
                     is_portinstance
\---------------------------------------------------------*/
static char is_portinstance(inst_pt, Name)
loins_list *inst_pt;
char       *Name;
{
	chain_list      *chain_pt;

	chain_pt = (chain_list *)YY_PTINS->USER;

	while( chain_pt )
	{
		if( Name == (char *)chain_pt->DATA )
			return 1;
		chain_pt = chain_pt->NEXT;
	}

	return 0;
} /* end of is_portinstance */

/*---------------------------------------------------------\
                     CheckCadenceNetError
\---------------------------------------------------------*/
static char *CheckCadenceNetError( Name )
char *Name;
{
	char  net_name[MAXSTRLEN];
	char *first;
	char *second;

	strcpy (net_name, Name);

	first = net_name;

	while( first )
	{
		if ( ( first = strchr( first, '_' ) ) != NULL )
		{
            if( strncmp( first, "_60_", 4 ) != 0 )
            {
                first++;
                continue;
            }
			second = first + 4;
			while( isdigit( *second ) ) second ++;

			if( strcmp( second, "_62" ) != 0 )
			{
				*first = ' ';
                first ++;
                second = first + 3;
                while( isdigit( *second ) )
                {
                    *first = *second;
                    second ++;
                    first ++;
                }
				first[ 0 ] = '\0';
				return namealloc( net_name );
			}
			first ++;
		}
		else
		{
			return namealloc( Name );
		}
	}

	return namealloc( Name );

} /* fin de CheckCadenceNetError */

/*---------------------------------------------------------\
                     CheckCadenceBusError
\---------------------------------------------------------*/
static char CheckCadenceBusError(PortName, Min, Max)
char *PortName;
int  *Min;
int  *Max;
{
	char *array;
	char *first;
	char *middle;
	char  save;

	strcpy (YY_ARRAYPORTNAME, PortName);

	first = YY_ARRAYPORTNAME;

	while (first)
	{
		if ( (first = strchr (first, '_') ) != NULL )
		{
            if( strncmp( first, "_60_", 4 ) != 0 )
            {
                first++;
                continue;
            }
			first[0] = '\0';
			array = first + 4;
			while ( isdigit (*array) ) array ++;

			if ( ( *array != 0 ) && ( strncasecmp (array, "TO", 2) == 0 ) )
			{
				middle = array;
				save = middle [0];
				middle [0] = '\0';
				array += 2;
				while ( isdigit (*array) ) array ++;

				if ( strcmp( first, "_62" ) != 0 )
				{
					array [0] = '\0';
					sscanf (first  + 4, "%d", Min);
					sscanf (middle + 2, "%d", Max);
					return 1;
				}
				middle [0] = save;
			}
			first[0] = '_';
			first ++;
		}
	}
	*Min = 0;
	*Max = 0;
	return 0;

} /* end of CheckCadenceBusError */

/*---------------------------------------------------------\
                     sig_merge
\---------------------------------------------------------*/
static void sig_merge(pt_lofig, sig1, sig2)
lofig_list *pt_lofig;
long        sig1;
long        sig2;
{
	losig_list *ls1, *ls2;
	locon_list *c;
	loins_list *i;
	lotrs_list *t;

    for (ls1 = pt_lofig->LOSIG; ls1; ls1 = ls1->NEXT) {
        if (ls1->INDEX == sig1)
            break;
    }

    for (ls2 = pt_lofig->LOSIG; ls2; ls2 = ls2->NEXT) {
        if (ls2->INDEX == sig2)
            break;
    }

    if (ls1 == NULL || ls2 == NULL) /* one of them not exists ! */
	{
        return;
    }

    if (ls1 == ls2) /* already joined!, just return */
        return;

    for (c = pt_lofig->LOCON; c; c = c->NEXT)
        if (c->SIG == ls2)
            c->SIG = ls1;

    for (i = pt_lofig->LOINS; i; i = i->NEXT)
        for (c = i->LOCON; c; c = c->NEXT)
            if (c->SIG == ls2)
                c->SIG = ls1;

    for (t = pt_lofig->LOTRS; t; t = t->NEXT) {
        if (t->GRID->SIG == ls2)
            t->GRID->SIG = ls1;
        if (t->DRAIN->SIG == ls2)
            t->DRAIN->SIG = ls1;
        if (t->SOURCE->SIG == ls2)
            t->SOURCE->SIG = ls1;
    }

     if(!ls1->PRCN)
       addlorcnet( ls1 );
    (void)addcapa(ls1, getcapa(ls2));
    (void)dellosig(pt_lofig, ls2->INDEX);
}

/*---------------------------------------------------------\
                     find_ripper_con_name
\---------------------------------------------------------*/
void find_ripper_con_name( lofig )
lofig_list *lofig;
{
    locon_list *c1;
    char save;
    char *pt;

    for( c1 = lofig->LOCON; c1; c1 = c1->NEXT )
    {
        if( isvss( c1->NAME ) || isvdd( c1->NAME ) )
        {
            continue;
        }
        if( !YY_RIPPER_IN )
        {
            if( ( pt = strrchr( c1->NAME, ' ' ) ) == NULL )
            {
                fprintf( stderr, "*** mbk warning *** edifloadlofig : ripper connectors not valid (2)!!\n" );
                exit(1);
            }
            save = *pt;
            *pt = '\0';
            YY_RIPPER_IN = namealloc ( c1->NAME );
            *pt = save;
        }
        else if( strncmp( YY_RIPPER_IN, c1->NAME, strlen( YY_RIPPER_IN ) ) != 0 )
        {
            if( YY_RIPPER_OUT )
            {
                if( strncmp( YY_RIPPER_OUT, c1->NAME, strlen( YY_RIPPER_OUT ) ) != 0 )
                {
                    fprintf( stderr, "*** mbk warning *** edifloadlofig : ripper connectors not valid (3)!!\n" );
                    exit(1);
                }
            }
            else
            {
                if( ( pt = strrchr( c1->NAME, ' ' ) ) == NULL )
                {
                    fprintf( stderr, "*** mbk warning *** edifloadlofig : ripper connectors not valid (4)!!\n" );
                    exit(1);
                }
                save = *pt;
                *pt = '\0';
                YY_RIPPER_OUT = namealloc ( c1->NAME );
                *pt = save;
            }
        }
    }

} /* end of find_ripper_con_name */
/*---------------------------------------------------------\
                     del_patch
\---------------------------------------------------------*/
static void del_patch(pt_lofig, master)
lofig_list *pt_lofig;
char        master;
{
	loins_list *i;
	loins_list *k;
	locon_list *c1, *c2;
	chain_list *ptchain;
	char *b;
	long j;
	long i1, i2;
	char buf[MAXSTRLEN];

	if( YY_RIPPER_CELL == (char *) NULL)
		return;

    if( master == 1 )
    {
        find_ripper_con_name( getlofig( YY_RIPPER_CELL, 'P' ) );
    }

    for (ptchain = pt_lofig->MODELCHAIN; ptchain != NULL; ptchain = ptchain->NEXT)
    {
		if ( YY_RIPPER_CELL != ptchain->DATA )
        	del_patch( getlofig( ptchain->DATA, 'P'), 0 );
    }

    for (i = pt_lofig->LOINS; i; )
	{
        if (YY_RIPPER_CELL == i->FIGNAME)
		{
			j = 0;
            do
			{
				sprintf (buf, "%s %d", YY_RIPPER_IN, j);
   				b = namealloc (buf);

        		for (c1 = i->LOCON; c1; c1 = c1->NEXT)
                {
					if ( (c1->NAME == b) && ( c1->SIG ) )
					{
						i1 = c1->SIG->INDEX;

						sprintf (buf, "%s %d", YY_RIPPER_OUT, j);
   						b = namealloc (buf);

        				for (c2 = i->LOCON; c2; c2 = c2->NEXT)
						{
							if ( (c2->NAME == b) && ( c2->SIG ) )
							{
								i2 = c2->SIG->INDEX;
								sig_merge(pt_lofig, i1, i2);
  								break;
							}
						}

                     break;
					}
				}

                j++;
			} while ( c1 );

			k = i->NEXT;
			delloins (pt_lofig, i->INSNAME);
			i = k;
		}
		else
		{
			i = i->NEXT;
		}
	}

	if( master == 1 )
		dellofig( YY_RIPPER_CELL );
}

/*---------------------------------------------------------\
                     REnamealloc
\---------------------------------------------------------*/
static void REnamealloc( )
{
	lofig_list *t;
	char       *te;

	for (t = YY_LOFIG; t; t = t->NEXT)
	{
		te = namealloc (t->NAME);
		if( te != t->NAME )
		{
			fprintf( stderr, "*** mbk warning *** edifloadlofig : instance name not namealloced !!!\n" );
			t->NAME = te;
		}
	}
}
