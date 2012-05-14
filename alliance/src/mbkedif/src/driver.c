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
 
/*----------------------------------------------------------------------------*/
/*                    @(#) MBK TO EDIF Driver                                 */
/*----------------------------------------------------------------------------*/
/*version 1.0 : Implemented by Fred Petrot                                    */
/*version 1.1 : Extended by Mokhtar Hirech (January, 92)                      */
/*version 1.2 : Extended by Olivier BEAURIN (September, 93)                   */
/*version 1.2 : Extended by Olivier BEAURIN (November, 93)                    */
/* $Log: driver.c,v $
/* Revision 1.5  2012/05/14 14:20:24  alliance
/* Updated GNU/FSF address (patch from Thibault North).
/*
/* Revision 1.4  2002/09/30 16:20:54  czo
/* support/users
/*
/* Revision 1.3  2002/04/25 14:16:44  ludo
/* correction petits bugs
/*
/* Revision 1.2  2002/03/14 12:36:30  fred
/* Makes the correct substitutions in lex and yacc generated files.
/* Fixing includes in dot c files
/* ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <mut.h>
#include <mlo.h>

/* defines */
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define SCHEMATIC "schematic"
#define SYMBOL    "symbol"

#define MBK_EDIF_LIBNAME  "MBKEDIF_LIBNAME"
#define MBK_EDIF_SUFFIXES "MBKEDIF_SUFFIXES"
#define DEBUG_MBKEDIF     "DEBUG_MBKEDIF"
#define DEFAULT_SUFFIXES  "y=ScLib:dp=DpLib:fp=FpLib:sp=PadLib"

/* Global variables*/

static int          edif_debug = FALSE;
static FILE        *edif_file;
static char        *EDIF_LIBNAME;
static char        *EDIF_SUFFIXES;
static int          nb_lib = 0;
static char       **suffixes = NULL;
static char       **libraries = NULL;
static chain_list **cellules = NULL;
static chain_list  *already_drived = NULL;

/* Global Functions*/

static void         port_ext         ( );
static void         edit_sig         ( );
static void         edit_ins         ( );
static void         edit_con         ( );
static void         connect          ( );
static void         view             ( );
static void         drive_cell       ( );
static void         r_drive_cell     ( );
static int          edif_busindex    ( );
static int          drive_basic_cells( );
static locon_list  *edif_found_array ( );
static void         check_fig_portref( );
static void         r_found_suffixed_cells( );
static void         save_libraries_cells( );

       char        *edif_busname     ( );

/*---------------------------------------------------------\
                     ParseSuffixes
\---------------------------------------------------------*/
int ParseSuffixes( fill )
int fill;
{
    char *suf;
    char *lib;
    char *pt;
    int   index;

    if( fill )
    {
        if( suffixes || libraries || cellules )
        {
            fprintf( stderr, "*** mbk error *** edifsavelofig : bugs in ParseSuffixes\n" );
            exit( 1 );
        }
        suffixes  = mbkalloc( nb_lib * sizeof( char * ) );
        libraries = mbkalloc( nb_lib * sizeof( char * ) );
        cellules  = mbkalloc( nb_lib * sizeof( chain_list * ) );
        index = 0;
    }

    suf = EDIF_SUFFIXES;
    do
    {
        lib = strchr( suf, '=' );
        if( !lib )
        {
            return FALSE;
        }
        pt = strchr( lib, ':' );
        if( pt && fill )
        {
            *pt = '\0';
        }
        if( fill )
        {
            *lib = '\0';
        }
        lib ++;

        if( !fill )
        {
            nb_lib++;
        }
        else
        {
            suffixes[ index ] = suf;
            libraries[ index ] = lib;
            cellules[ index ] = NULL;
            index ++;
        }

        if( pt )
        {
            suf = pt + 1;
        }
    } while( pt );

    if( fill && edif_debug )
    {
        fprintf( stdout, "\nMBKEDIF DEBUG : Libraries and suffixes used.\n\n");
        for( index = 0; index < nb_lib; index ++)
        {
            fprintf( stdout, "\t%s\t=>\t%s\n",suffixes[index], libraries[index] );
        }
        fprintf( stdout, "\n");
    }

    return TRUE;
} /* end of ParseSuffixes */

/*---------------------------------------------------------\
                     edifsavelofig
\---------------------------------------------------------*/
void edifsavelofig( firstlofig )
lofig_list *firstlofig;  
{
	chain_list *chainl;
	chain_list *chainp;
	char        filename[ 255 ];
	char       *pc;
    int         index;

    edif_debug = mbkgetenv( DEBUG_MBKEDIF ) != NULL;

	EDIF_LIBNAME = mbkgetenv( MBK_EDIF_LIBNAME );
	if( !EDIF_LIBNAME )
		EDIF_LIBNAME = namealloc( "alliance" );

	if( strcmp( EDIF_LIBNAME, "" ) == 0 )
		EDIF_LIBNAME = namealloc( "alliance" );


	for( pc = EDIF_LIBNAME; pc[0] != '\0'; pc ++ )
    {
		if( ( pc[0] == '(' ) || ( pc[0] == ')' ) || ( pc[0] == ' ' ) )
		{
			fprintf( stderr, "*** mbk error *** edifloadlofig : environment variable 'MBK_EDIF_LIBNAME' invalid.\n" );
			exit( 1 );
		}
    }

    if( nb_lib == 0 )
    {
        EDIF_SUFFIXES = mbkgetenv( MBK_EDIF_SUFFIXES );
        if( !EDIF_SUFFIXES )
        {
            EDIF_SUFFIXES = mbkstrdup(DEFAULT_SUFFIXES);
        }
        if( !ParseSuffixes( FALSE ) )
        {
            fprintf(stderr, "*** mbk warning *** edifsavelofig : %s invalid, using default value.\n",
                    MBK_EDIF_SUFFIXES );
            EDIF_SUFFIXES = mbkstrdup(DEFAULT_SUFFIXES);
            if( !ParseSuffixes( FALSE ) )
            {
                fprintf( stderr, "*** mbk error *** edifsavelofig : Bugs with default %s value\n",
                         MBK_EDIF_SUFFIXES);
                exit( 1 );
            }
        }
        ParseSuffixes( TRUE );
    }
    else
    {
        for( index = 0; index < nb_lib; index ++)
        {
            cellules[index] = NULL;
        }
    }

	if (firstlofig == NULL)
	{
		fprintf( stderr, "*** mbk error *** edifsavelofig : edif shall not drive a NULL figure\n");
		exit( 1 );
	}

    r_found_suffixed_cells( firstlofig );

	sprintf( filename, "%s/%s.%s", WORK_LIB, firstlofig->NAME, OUT_LO );

	if( ( edif_file = mbkfopen( firstlofig->NAME, OUT_LO, WRITE_TEXT ) ) == NULL )
	{
		fprintf( stderr, "*** mbk error *** edifsavelofig : the file %s cannot be opened\n", filename );
		exit( 1 );
	}

	fprintf( edif_file, "(edif RobiN_EDIF\n"                                                    );
	fprintf( edif_file, " (edifversion 2 0 0)\n"                                                );
	fprintf( edif_file, " (ediflevel 0)\n"                                                      );
	fprintf( edif_file, " (keywordMap (keywordLevel 0))\n"                                      );
	fprintf( edif_file, " (status\n"                                                            );
	fprintf( edif_file, "  (written\n"                                                          );
	fprintf( edif_file, "   (timeStamp %s)\n",                               EdifTime( )        );
	fprintf( edif_file, "   (program \"Driver mbk2edif\")\n"                                    );
	fprintf( edif_file, "   (author \"FP & HM & OB for : %s\")\n", mbkgetenv( "USER" ) ? mbkgetenv( "USER" ) : "RobiN" );
	fprintf( edif_file, "   (dataOrigin \"VLSI-CAD : Masi Lab. UPMC\")))"                       );

    save_libraries_cells( );

    fprintf( edif_file, "\n (library %s\n", EDIF_LIBNAME ); 
    fprintf( edif_file, "  (ediflevel 0)\n" );
    fprintf( edif_file, "  (technology (numberDefinition))" );

	r_drive_cell( firstlofig );

	fprintf( edif_file, "))\n");

	if( fclose( edif_file ) !=0 )
	{
		fprintf( stderr, "*** mbk error *** edifsavelofig : Cannot close file '%s'\n", filename );
		exit( 1 );
	}

	freechain( already_drived );
	already_drived = (chain_list *)NULL;

} /* end of edifsavelofig */

/*---------------------------------------------------------\
                     save_libraries_cells
\---------------------------------------------------------*/
static void save_libraries_cells( )
{
    chain_list *ptchain;
    int         index;

    for( index = 0; index < nb_lib; index ++ )
    {
        if( !cellules[index] )
        {
            continue;
        }

        fprintf( edif_file, "\n (library %s\n", libraries[index] ); 
        fprintf( edif_file, "  (ediflevel 0)\n" );
        fprintf( edif_file, "  (technology (numberDefinition))" );

        ptchain = cellules[ index ];

        while( ptchain )
        {
	        drive_cell(ptchain->DATA, incatalog(ptchain->DATA) ? 'I' : 'C');
            ptchain = ptchain->NEXT;
        }

        fprintf( edif_file, ")");
    }
} /* end of save_libraries_cells */

/*---------------------------------------------------------\
                     in_chain_list
\---------------------------------------------------------*/
static char in_chain_list( ptchain, cell_name )
chain_list *ptchain;
char       *cell_name;
{
	chain_list *chainp;

	for(chainp = ptchain; chainp; chainp = chainp->NEXT)
		if ((char *)chainp->DATA == cell_name)
			return 1;

	return 0;
} /* end of in_chain_list */

/*---------------------------------------------------------\
                     found_suffixed_cell
\---------------------------------------------------------*/
static char *found_suffixed_cell( name, insert )
char *name;
int   insert;
{
    int index;
    char *pt;

    pt = strrchr( name, '_' );
    if( !pt )
    {
        return NULL;
    }
    pt++;

    for( index = 0; index < nb_lib; index ++ )
    {
        if( strcmp( pt, suffixes[index] ) == 0 )
        {
            if( (!insert) || ( in_chain_list( cellules[ index ], name ) ) )
            {
                return libraries[ index ];
            }
            if( edif_debug )
            {
                fprintf(stdout, "Cellule %s in library %s\n",name,libraries[index]);
            }

            cellules[index] = addchain(cellules[index],name );
            return NULL;
        }
    }
    return NULL;
} /* end of found_suffixed_cell */

/*---------------------------------------------------------\
                     r_found_suffixed_cells
\---------------------------------------------------------*/
static void r_found_suffixed_cells( ptlofig )
lofig_list *ptlofig;
{
	chain_list *ptchain;
	lofig_list *ptlfig;

	for (ptchain = ptlofig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT)
	{
		found_suffixed_cell(ptchain->DATA, TRUE);
	}
	for (ptchain = ptlofig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT)
	{
		if (!incatalog(ptchain->DATA))
        {
            r_found_suffixed_cells( getlofig( ptchain->DATA, 'A' ) );
        }
	}
} /* end of r_found_suffixed_cells */

/*---------------------------------------------------------\
                     r_drive_cell
\---------------------------------------------------------*/
static void r_drive_cell(ptlofig)
lofig_list *ptlofig;
{
	chain_list *ptchain;
	lofig_list *ptlfig;

	ptlofig->MODELCHAIN = (chain_list *)reverse((chain_list *)ptlofig->MODELCHAIN);

	drive_basic_cells(ptlofig); 

	for (ptchain = ptlofig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT)
	{
		if (!in_chain_list(already_drived, namealloc((char*)ptchain->DATA)))
		{
			ptlfig = getlofig((char *)ptchain->DATA, 'A');
			r_drive_cell(ptlfig);
		}
	}

	ptlofig->MODELCHAIN = (chain_list *)reverse((chain_list *)ptlofig->MODELCHAIN);
	drive_cell(ptlofig->NAME, 'C');
} /* end of r_drive_cell */


/*---------------------------------------------------------\
                     drive_basic_cells
\---------------------------------------------------------*/
static int drive_basic_cells( ptlofig )
lofig_list *ptlofig;
{
	chain_list *ptchain;
	lofig_list *ptlfig;
	char       *cell_model_name;

	for (ptchain = ptlofig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT)
	{
		cell_model_name = namealloc((char *)ptchain->DATA);
		if (incatalog(cell_model_name))
			drive_cell(cell_model_name, 'I');
	}
} /* end of drive_basic_cells */

/*---------------------------------------------------------\
                     drive_cell
\---------------------------------------------------------*/
static void drive_cell(figname, c)
char *figname;
char  c;
{
	chain_list *ptchain;
	lofig_list *ptlofig;

	if (in_chain_list(already_drived, figname))
		return;

	already_drived = addchain(already_drived, figname);

	if (c == 'C')
    {
		view(figname, 'A');
    }

    view(figname, 'I');
}

/*---------------------------------------------------------\
                     view
\---------------------------------------------------------*/
static void view(figname, viewtype)
char *figname;
char viewtype;
{
	lofig_list *ptlofig;
	char        view[20];

	fprintf(edif_file, "\n   (cell %s (cellType GENERIC)", figname);

	strcpy(&view[0], (viewtype == 'A') ? SCHEMATIC: SYMBOL );

	fprintf(edif_file, "\n    (view %s (viewType NETLIST)", view);
	fprintf(edif_file, "\n     (interface");

	ptlofig = getlofig(figname, 'P');

	ptlofig->LOCON = (locon_list *)reverse((chain_list *)ptlofig->LOCON);

	edit_con(ptlofig->LOCON);

	fprintf(edif_file, ")");

	if( viewtype == 'A' )
	{
		fprintf(edif_file, "\n     (contents");

		ptlofig->LOINS = (loins_list *)reverse((chain_list *)ptlofig->LOINS);
		edit_ins( ptlofig->LOINS );
		ptlofig->LOINS = (loins_list *)reverse((chain_list *)ptlofig->LOINS);

		ptlofig->LOSIG = (losig_list *)reverse((chain_list *)ptlofig->LOSIG);
		edit_sig( ptlofig );
		ptlofig->LOSIG = (losig_list *)reverse((chain_list *)ptlofig->LOSIG);

		fprintf( edif_file, ")");
	}

	fprintf(edif_file, "))");

	ptlofig->LOCON = (locon_list *)reverse((chain_list *)ptlofig->LOCON);

	return;
} /* end of view */

/*---------------------------------------------------------\
                     connect
\---------------------------------------------------------*/
static void connect(conn, index, instance)
locon_list *conn;
int         index;
loins_list *instance;
{
	locon_list *ptcon;
	locon_list *ptcon2;
	losig_list *sig;
	char       *busname;
    char       *name;
	char       *pt;
	int         begin;
	int         end;
	int         myindex;

	ptcon = conn;

	for( ; ptcon != (locon_list *) NULL; ptcon = ptcon -> NEXT)
	{
		sig = ptcon->SIG;
		if( sig->INDEX == index )
		{
			name = mbkalloc( strlen( ptcon->NAME ) + 1 );
			strcpy( name, ptcon->NAME );
			for( pt = name; *pt; pt++ )
            {
				if( *pt == '/' )
                {
					*pt = '_';
                }
            }

			busname = edif_busname( name );

			if( !busname )
				fprintf( edif_file,"\n        (portref %s ", name );
			else
			{
				myindex = edif_busindex( name );
				ptcon2 = conn;
				do
				{
					ptcon2 = edif_found_array( ptcon2, busname, &begin, &end );
				} while( ( ( begin <= end ) && ( ( myindex < begin ) || ( end < myindex ) ) )
				       ||( ( begin >  end ) && ( ( myindex > begin ) || ( end > myindex ) ) ) );

				if( begin != end )
					fprintf( edif_file,"\n        (portref (member %s_60_%dTO%d_62 %d) ",
					         busname, begin, end,
					         ( begin <= end ) ? myindex - begin : begin - myindex );
				else
					fprintf( edif_file,"\n        (portref %s_%d_",
					         busname, begin );
			}

            mbkfree( name );

			if( instance != (loins_list *) NULL )
			{
				name = mbkalloc( strlen( instance->INSNAME ) + 1 );
				strcpy( name, instance->INSNAME );
				for( pt = name; *pt; pt++ )
					if( *pt == '/' )
						*pt = '_';

				fprintf( edif_file, "(instanceref %s )", name );

            	mbkfree( name );
			}

			fprintf( edif_file, ")" );

		}
	}
} /* end of connect */

/*---------------------------------------------------------\
                     edit_con
\---------------------------------------------------------*/
static void edit_con( ptcon )
locon_list *ptcon;
{
	char         direction[ 7 ];
	char         cadence_array_name[ 1000 ];
	char        *array_name;
    char        *name;
    char        *pt;
	int          begin;
	int          end;

	while (ptcon != (locon_list *) NULL)
	{
        if( isvdd( ptcon->NAME ) || isvss( ptcon->NAME ) )
        {
			ptcon = ptcon -> NEXT;
            continue;
        }
		if( ptcon -> DIRECTION == IN )
			strcpy( direction, "INPUT" );
		else
			if( ( ptcon -> DIRECTION == OUT ) || ( ptcon->DIRECTION == 'Z' ) )
				strcpy(direction, "OUTPUT");
			else
				strcpy(direction, "INOUT");

		name = mbkalloc( strlen( ptcon->NAME ) + 1 );
		strcpy( name, ptcon->NAME );
		for( pt = name; *pt; pt++ )
			if( *pt == '/' )
				*pt = '_';

		if( !( array_name = edif_busname( name ) ) )
		{
			fprintf(edif_file, "\n      (port %s (direction %s))", name, direction);
			ptcon = ptcon -> NEXT;
		}
		else 
		{
			ptcon = edif_found_array( ptcon, array_name, &begin, &end );

			if( end != begin )
			{
				sprintf( cadence_array_name, "(rename %s_60_%dTO%d_62 \"%s<%d:%d>\")",
				         array_name, begin, end,
				         array_name, begin, end );
				fprintf(edif_file, "\n      (port (array %s %d)",
					               cadence_array_name, abs( end - begin ) + 1);
				fprintf(edif_file, "(direction %s))", direction );
			}
			else
				fprintf(edif_file, "\n      (port (rename %s_60_%d_62 \"%s<%d>\") (direction %s))", 
					               array_name, begin, array_name, begin, direction);

			if( ptcon == (locon_list *) NULL )
				break;
		}

        mbkfree( name );
	}
} /* end of edit_con */

/*---------------------------------------------------------\
                     edit_ins
\---------------------------------------------------------*/
static void edit_ins(ptins)
loins_list *ptins;
{
	char  cellref_type;
	char *name;
	char *pt;

	while (ptins != (loins_list *) NULL)
	{      
		name = mbkalloc( strlen( ptins->INSNAME ) + 1 );
		strcpy( name, ptins->INSNAME );
		for( pt = name; *pt; pt++ )
        {
			if( *pt == '/' )
            {
				*pt = '_';
            }
        }

		cellref_type = (incatalog(ptins->FIGNAME) ? 'I' : 'A');

        pt = found_suffixed_cell( ptins->FIGNAME, FALSE );

		fprintf(edif_file, "\n      (instance %s (viewref %s (cellref %s (libraryref %s) )))",
		                   name,
/* 		                   (cellref_type == 'I') ? SYMBOL : SCHEMATIC, */
                           SYMBOL,
		                   ptins->FIGNAME,
                           pt ? pt : EDIF_LIBNAME );
		ptins = ptins -> NEXT;

        mbkfree( name );
	}
} /* end of edit_ins */

/*---------------------------------------------------------\
                     edit_sig
\---------------------------------------------------------*/
static void edit_sig(ptlofig)
lofig_list *ptlofig;
{
	losig_list *ptsig;
	loins_list *sig_ins;
	int         idx = 0;
	long        sig_index;
	char       *array_name = (char *) NULL;
	char       *name = (char *) NULL;
    char       *pt;

	for (ptsig = ptlofig->LOSIG; ptsig != (losig_list *) NULL; ptsig = ptsig->NEXT)
		check_fig_portref( ptlofig->NAME, ptlofig->LOCON, ptsig->INDEX );

	ptsig = ptlofig->LOSIG;
	while( ptsig != (losig_list *) NULL )
	{
		if( ptsig->TYPE != 'I' || ptlofig->LOINS )
		{
			sig_index = ptsig -> INDEX;

			name = getsigname( ptsig );
            if( isvdd( name ) || isvss( name ) )
            {
                ptsig = ptsig->NEXT;
                continue;
            }
			for( pt = name; *pt; pt++ )
            {
				if( *pt == '/' )
                {
					*pt = '_';
                }
            }

			if( ( array_name = edif_busname( name ) ) && ( ( idx = edif_busindex( name ) ) != -1 ) )
			{
				fprintf( edif_file, "\n      (net (rename %s_60_%d_62 \"%s<%d>\") (joined ",
				         array_name, idx, array_name, idx );
			} 
			else
			{
				if( !isdigit( name[0] ) )
					fprintf( edif_file, "\n      (net %s (joined ", name );
				else 
					fprintf( edif_file, "\n      (net &_%s (joined ", name );
			}

			if (ptsig->TYPE != 'I')
				port_ext( ptlofig->LOCON, sig_index );

			for( sig_ins = ptlofig->LOINS; sig_ins != NULL; sig_ins = sig_ins->NEXT )
            {
                sig_ins->LOCON = (locon_list *)reverse((chain_list *)sig_ins->LOCON);
				connect( sig_ins->LOCON, sig_index, sig_ins );
                sig_ins->LOCON = (locon_list *)reverse((chain_list *)sig_ins->LOCON);
            }

			fprintf(edif_file, "))");
		}

		if( ptsig )
        {
			ptsig = ptsig->NEXT;
        }
	}
} /* end of edit_sig */

/*---------------------------------------------------------\
                     check_fig_portref
\---------------------------------------------------------*/
static void check_fig_portref( lofig_name, ptcon, sig_index )
char        *lofig_name;
locon_list  *ptcon;
int     sig_index;
{
	int nb = 0;

	for (; (ptcon != (locon_list *) NULL) && (nb <= 1); ptcon = ptcon->NEXT)
		if (ptcon->SIG->INDEX == sig_index)
			nb++;

	if (nb > 1)
	{
		fprintf( stderr, "*** mbk error *** edifsavelofig : Error in MBK Structure\n" );
		fprintf( stderr, "    multiple (portref ..) to lofig '%s' not\n" );
		fprintf( stderr, "    allowed on signal of type 'E' (cadence)\n", lofig_name );
		exit( 1 );
	}
   
} /* end of check_fig_portref */

/*---------------------------------------------------------\
                     port_ext
\---------------------------------------------------------*/
static void port_ext( sig_con, sig_index )
locon_list *sig_con;
int         sig_index;
{
	losig_list *sig_con_sig;
	locon_list *connector, *fig_con;
	char       *sig_con_name;

	connector = (locon_list *)NULL;
	fig_con = sig_con;
	sig_con_name = (char *)NULL;

	while (sig_con != (locon_list *)NULL)
	{
		sig_con_sig = sig_con -> SIG;
		if (sig_con_sig -> INDEX == sig_index)
		{
			sig_con_name = sig_con -> NAME;
			connector=sig_con;
			break;
		}
		sig_con = sig_con -> NEXT;
	}

	if (sig_con_name == NULL)
	{
		fprintf( stderr, "***  mbk error *** edifsavelofig : Error in MBK Structure\n" );
		fprintf( stderr, "    No Connector on Signal of type 'E'\n" );
		exit( 1 );
	}

	connect(fig_con, sig_index, (loins_list *)NULL);

} /* end of port_ext */

/*---------------------------------------------------------\
                     edif_found_array
\---------------------------------------------------------*/
static locon_list *edif_found_array( ptcon, name, begin, end )
locon_list *ptcon;
char       *name;
int        *begin;
int        *end;
{
	locon_list *ptcon_first_bus;
	char       *array_name;

	*begin = 0;
	*end   = 0;

	while( ( ptcon ) && (  ( array_name = edif_busname( ptcon->NAME ) ) != name ) )
		ptcon = ptcon->NEXT;

	if( !ptcon )
		return NULL;

	*begin = edif_busindex( ptcon -> NAME );
	*end   = *begin;

	ptcon_first_bus = ptcon;

	while( ( ptcon != (locon_list *) NULL ) && ( array_name == edif_busname( ptcon->NAME ) ) )
	{
		if (ptcon->DIRECTION != ptcon_first_bus->DIRECTION)
		{
			fprintf( stderr, "*** mbk error *** edifsavelofig : Error in MBK Structure\n" );
			fprintf( stderr, "    Connectors %s and %s of same bus must have same direction\n", 
			                ptcon_first_bus->NAME, *ptcon->NAME);
			exit( 1 );
		}
		*end = edif_busindex( ptcon->NAME );
		ptcon = ptcon->NEXT; 
	}

	return ptcon;

} /* fin de edif_found_array */

/*---------------------------------------------------------\
                     edif_busindex
\---------------------------------------------------------*/
static int edif_busindex(s)
char *s;
{
	char *t;

	if( !( t = strchr( s, ' ' ) ) )
		return -1;

	return atoi(t);

} /* end of edif_busindex */

/*---------------------------------------------------------\
                     edif_busname
\---------------------------------------------------------*/
char *edif_busname( name )
char *name;
{
	char *s;
	char c[100];

	if( !( s = strchr( name, ' ' ) ) )
		return (char *) NULL;

	strncpy( c, name, (int)(s - name) );
	c[ s - name ] = '\0';

	for (s++; *s != '\0'; s++) 
		if( !isdigit( *s ) )
			return (char *) NULL;

	return namealloc( c );
} /* end of edif_busname */

