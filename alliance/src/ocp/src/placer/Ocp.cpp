
// This file is part of the Alliance Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// The Alliance Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Alliance Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Alliance Project;  if  not,  write to the  Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <string.h>
using namespace std;

#include "mut.h"
#include "mph.h"
#include "mlo.h"
extern "C" {
#include "rds.h"
#include "rtl.h"
#include "rpr.h"
}

#include "PPlacement.h"

static void
Usage()
{
    cerr << "usage: ocp [options] <netlist> <filename>" << endl
         << "Options :"  << endl
         << "o -help : display man page" << endl
	 << "o -v : verbose mode" << endl
	 << "o -gnuplot : create statistics files to use with gnuplot" << endl
	 << "o -c : create connectors (placement randomly generated)" << endl
	 << "o -ring : create connectors suitable for ring pad placement tool" << endl
	 << "          placement randomly generated unless -ioc is given" << endl
	 << "o -ioc : create connectors and place it using .ioc file" << endl
         << "o -margin <MARGIN> : The amount of free area added " << endl
	 << "  in percentage of the cells area. " << endl
	 << "  The resulting area will be equal to " << endl
	 << "  CELL_AREA * (1 + <MARGIN>). Default value : 0.2" << endl
	 << "o -eqmargin : The margin is equitably distributed between all cells" << endl
	 << "  By default, we try to insert the maximum amount of 2-pitch tie cells" << endl
	 << "o -partial <PARTIAL> : " << endl
	 << "  The design is already partially placed (there must be a" << endl
	 << "  physical view describing this partial placement)." << endl
	 << "o -rows <NR> : the placement will have <NR> rows"<< endl
	 << "o -mdl <NL> : maximum loop of final optimisation "
	 << "default : <NL> = 2" << endl;
}

static void
PrintEnv()
{
    char* allianceTop = mbkgetenv("ALLIANCE_TOP");
    char* mbkInLo = mbkgetenv("MBK_IN_LO");
    char* mbkOutLo = mbkgetenv("MBK_OUT_LO");
    char* mbkInPh = mbkgetenv("MBK_IN_PH");
    char* mbkOutPh = mbkgetenv("MBK_OUT_PH");
    char* mbkVss = mbkgetenv("MBK_VSS");
    char* mbkVdd = mbkgetenv("MBK_VDD");
    char* mbkCatalName = mbkgetenv("MBK_CATAL_NAME");
    char* mbkCataLib = mbkgetenv("MBK_CATA_LIB");
    cout << "o ALLIANCE environment:" << endl;
    if (allianceTop)
	cout << "  o ALLIANCE_TOP   : " << allianceTop << endl;
    else
	cout << "  o Warning: no ALLIANCE_TOP variable set ...." << endl;
    cout << "o MBK environment:" << endl;
    if (mbkInLo)
	cout << "  o MBK_IN_LO      : " << mbkInLo << endl;
    else
	cout << "  o Warning: no MBK_IN_LO variable set ...." << endl;
    if (mbkOutLo)
	cout << "  o MBK_OUT_LO     : " << mbkOutLo << endl;
    else
	cout << "  o Warning: no MBK_OUT_LO variable set ...." << endl;
    if (mbkInPh)
	cout << "  o MBK_IN_PH      : " << mbkInPh << endl;
    else
	cout << "  o Warning: no MBK_IN_PH variable set ...." << endl;
    if (mbkOutPh)
	cout << "  o MBK_OUT_PH     : " << mbkOutPh << endl;
    else
	cout << "  o Warning: no MBK_OUT_PH variable set ...." << endl;
    if (mbkVss)
	cout << "  o MBK_VSS        : " << mbkVss << endl;
    else
	cout << "  o Warning: no MBK_VSS variable set ...." << endl;
    if (mbkVdd)
	cout << "  o MBK_VDD        : " << mbkVdd << endl;
    else
	cout << "  o Warning: no MBK_VDD variable set ...." << endl;
    if (mbkCatalName)
	cout << "  o MBK_CATAL_NAME : " << mbkCatalName << endl;
    else
	cout << "  o Warning: no MBK_CATAL_NAME variable set ...." << endl;
    if (mbkCataLib)
    {
	cout << "  o MBK_CATA_LIB   : ";
	while (*mbkCataLib !=  '\0')
	{
	    while ((*mbkCataLib != '\0') && (*mbkCataLib !=  ':'))
	    {
		cout << *mbkCataLib++;
	    }
	    if (*mbkCataLib == ':')
	    {
		cout << endl << "                     ";
		++mbkCataLib;
	    }
	    else
		cout << endl;
	}
    }
    else
	cout << "  o Warning: no MBK_CATA_LIB variable set ...." << endl;
    cout << endl;
}

int
main(int argc, char **argv)
{
    double		BBoxOccCostRatio	= 0.5;
    double		Margin			= 0.2;
    bool		marginflag		= false;
    int			MaxDetLoop		= 2;
    int			NbRows			= 0;
    char		PartialFile[256]	;	 //.ap file
    char		IocFile[256]		;	 //.ioc file
    bool		RowFlg			= false;
    bool		HelpFlg			= false; // display help
    bool		UsageFlg		= false; // print usage
    bool		PartialFlg		= false; // .ap file loaded
    bool		PlotFlg			= false; // GnuPlot graphics
    bool		ConFlg			= false; // Connectors
    bool		RingFlg			= false; // Connectors
    bool		IocFlg			= false; // file for Connectors
    bool		VerboseFlg		= false; // verbose mode
    bool		EqMarginFlg		= false; // don't try to maximise
    							 // 2-pitch cells
    double		NetMult			= 1.0 ;
    double		BinMult			= 0 ;
    double		RowMult			= 0 ;

    /* ###------------------------------------------------------### */
    /* analyse the command line, set option flags and find the	    */
    /* first file in the argument list				    */
    /* ###------------------------------------------------------### */

    //mbkenv();
    rdsenv();
    loadrdsparam ();
    mbkenv();
    
    alliancebanner ("OCP", VERSION, "Placer for Standards Cells", "2001", ALLIANCE_VERSION);

    if (argc < 2)
    {
	Usage();
	exit(0);
    }

    for (int i=1; i<argc; i++)
    {
	if (!strcmp (argv[i], "-help"))
	{
	    HelpFlg = true;
	}
	else
	    if (!strcmp (argv[i], "-v"))
	    {
		if (i+2 < argc) VerboseFlg = true;
		else	      UsageFlg = true;
	    }
	    else
		if (!strcmp (argv[i], "-eqmargin"))
		{
		    if (i+2 < argc) EqMarginFlg = true;
		    else	      UsageFlg = true;
		}
		else
		    if (!strcmp (argv[i], "-gnuplot"))
		    {
			if (i+2 < argc) PlotFlg = true;
			else	      UsageFlg = true;
		    }
		    else
			if (!strcmp (argv[i], "-c"))
			{
			    if (i+2 < argc) ConFlg = true;
			    else	      UsageFlg = true;
			}
			else
			    if (!strcmp (argv[i], "-ring"))
			    {
				if (i+2 < argc) RingFlg = true;
				else UsageFlg = true;
			    }
			    else
				if (!strcmp (argv[i], "-r"))
				{
				    if (i+3 < argc)
				    {
					if (!sscanf(argv[i+1], "%22lg", &BBoxOccCostRatio))
					{
					    cout << "WARNING : invalid argument for -r" << endl;
					    UsageFlg = true;
					}
	      } else {
	    	cout << "WARNING : no file or argument given" << endl;
		UsageFlg = true;
	      }
            }

	    else
	   
            if (!strcmp (argv[i], "-nbrmult")) {
              if (i+5 < argc)
              {
        	if (!sscanf(argv[i+1], "%22lg", &NetMult))
        	{
            	  cout << "WARNING : invalid argument for -nbrmult" << endl;
        	  UsageFlg = true;
        	}
        	if (!sscanf(argv[i+2], "%22lg", &RowMult))
        	{
            	  cout << "WARNING : invalid argument for -nbrmult" << endl;
        	  UsageFlg = true;
        	}
        	if (!sscanf(argv[i+3], "%22lg", &BinMult))
        	{
            	  cout << "WARNING : invalid argument for -nbrmult" << endl;
        	  UsageFlg = true;
        	}
              } else {
            	cout << "WARNING : no file or argument given" << endl;
        	UsageFlg = true;
        
              }
            }
	    
	    else
            
	    if (!strcmp (argv[i], "-margin")) {
	      if (i+3 < argc)
	      {
                if (!sscanf(argv[i+1], "%22lg", &Margin))
		{
	    	  cout << "WARNING : invalid argument for -margin" << endl;
		  UsageFlg = true;
		}
	      } else {
	    	cout << "WARNING : no file or argument given" << endl;
		UsageFlg = true;
	      }
	      marginflag = true;
            }
	    
	    else
	    
	    if (!strcmp (argv[i], "-partial")) {
	      if (i+3 < argc)
	      {
                if (!sscanf(argv[i+1], "%255s", PartialFile))
		{
	    	  cout << "WARNING : invalid argument for -partial" << endl;
		  UsageFlg = true;
		}
		PartialFlg = true;
	      } else {
	        cout << "WARNING : no file or argument given" << endl;
		UsageFlg = true;
	      }
            }
	    
	    else
	      
	    if (!strcmp (argv[i], "-ioc")) {
	      if (i+3 < argc)
	      {
                if (!sscanf(argv[i+1], "%255s", IocFile))
		{
	    	  cout << "WARNING : invalid argument for -partial" << endl;
		  UsageFlg = true;
		}
		IocFlg = true;
		strcat(IocFile,".ioc");
	      } else {
	        cout << "WARNING : no file or argument given" << endl;
		UsageFlg = true;
	      }
            }
	    
	    else
	    
	    if (!strcmp (argv[i], "-mdl")) {
	      if (i+3 < argc)
	      {
                if (!sscanf(argv[i+1], "%11d", &MaxDetLoop))
		{
		  cout << "WARNING : invalid argument for -mdl" << endl;
		  UsageFlg = true;
		}

	      } else {
		cout << "WARNING : no file or argument given" << endl;
		UsageFlg = true;
	      }
            }
	    
	    else
	    
	    if (!strcmp (argv[i], "-rows")) {
	      if (i+3 < argc)
	      {
	        if (!sscanf(argv[i+1], "%11d", &NbRows))
		{
		  cout << "WARNING : invalid argument for -rows" << endl;
		  UsageFlg = true;
		}
		RowFlg = true;
	      } else {
		cout << "WARNING : no file or argument given" << endl;
		UsageFlg = true;
	      }
            }

	    else

	    if (!strncmp (argv[i], "-", 1))
	    {
	      cout << "WARNING : unavailable option : " << argv[i] << endl;
	      UsageFlg = true;
	    }
        }
  
    /* ###------------------------------------------------------### */
    /*      cases of error in the command line:			    */
    /*      - no argument					    */
    /*      - argument unrecognized as an option		    */
    /*      - number of files do not match:			    */
    /* ###------------------------------------------------------### */

    if (HelpFlg == true) {
      system("man ocp");
      exit (0);
    }

    if (argc < 3) {
      Usage();
      exit(1);
    }
    
    if ( BBoxOccCostRatio < 0.0 || BBoxOccCostRatio > 1.0)
        UsageFlg = true;

    if ( Margin < 0 )
        UsageFlg = true;
    
    if ( Margin < 0.15 )
    {
      cerr << " o Ocp Warning : the value of margin: "
	   << Margin << " is very low... Try a value around 0.15 ..." << endl;
    }

    if ( Margin > 3 )
    {
      cerr << " o Ocp Warning : the value of margin: "
	   << Margin << " cannot be respected..." << endl
	   << " o value forced to 0.4 !!" << endl; 
      Margin = 0.4;
    }

    if ( marginflag && PartialFlg )
    {
	cerr << " o Ocp Warning : the switch margin and partial are not coherent "
	     << "and should not be use together...." << endl
	     << " o In case of a preplacement the margin is determined"
	     << " by the value of the Abutment Box and the net netlist you give me." << endl
	     << " o In consequence the switch margin has no sense and is bypassed ...."
	     << endl;
    }

    if (IocFlg && ConFlg)
    {
	cerr << " o You cannot have a connectors placement file and ask "
	    << "for a automatic connectors placement at the same time ...." << endl
	    << " o Use either -ioc or -c" << endl;
	exit(1);
    }
    
    if (UsageFlg) {
        Usage();
        exit (1);
    }

    if (VerboseFlg)
	PrintEnv();

    // Initializations
    // ***************

    struct lofig* fig = NULL;
    fig = getlofig(argv[argc-2],'A');
    
    char *namefile = argv[argc-1];

    struct phfig* physicalfig = NULL;
    if (PartialFlg) physicalfig = getphfig(PartialFile,'A');

    // On fait le placement
    // ********************
    PPlacement Placement(ConFlg, RingFlg, RowMult, BinMult, NetMult,
	IocFlg, IocFile, PlotFlg, VerboseFlg, PartialFlg, EqMarginFlg,
	physicalfig, namefile);
    Placement.SetMargin(Margin);
    Placement.SetMaxDetLoop(MaxDetLoop);
    if (!RowFlg)
	NbRows = 0;
    Placement.Place(fig, NbRows);

    // On sauve le resultat
    // ********************
    cout  << endl << "Ocp : placement finished" << endl;
    if (Placement.GetBoolPlot())
    {
      Placement.PlotFinal("view");
      Placement.PlotStat();
      cout << "gnuplot files created - in order to use them, please type :"
	<< endl << "gnuplot afterglobal.gpl"
	<< endl << "gnuplot binsafterglobal.gpl"
	<< endl << "gnuplot init.gpl"
	<< endl << "gnuplot instances-init.gpl"
	<< endl << "gnuplot stat.gpl"
	<< endl << "gnuplot view.gpl" << endl;
    }
    int superr = Placement.Save();
    dellofig(fig->NAME);
    
    return (superr);
}
