#include "PPlacement.h"
#include <stdio.h>
#include <iostream>
#include <ctype.h>

extern "C" {
#include "mut.h"
#include "mph.h"
#include "mlo.h"
}
using namespace std;

static void
Usage()
{
    cerr << "usage: ocp [options] <netlist> <filename>" << endl
         << "Options :"  << endl
         << "o -help : display man page" << endl
	 << "o -v : verbose mode" << endl
	 << "o -gnuplot : create statistics files to use with gnuplot" << endl
	 << "o -c : create connectors (placement randomly generated)" << endl
	 << "o -ring : create connectors (placement randomly generated) suitable for ring pad placement tool" << endl
	 << "o -ioc : create connectors and place it using .ioc file" << endl
         << "o -margin <MARGIN> : The amount of free area added " << endl
	 << "  in percentage of the cells area. " << endl
	 << "  The resulting area will be equal to " << endl
	 << "  CELL_AREA * (1 + <MARGIN>). Default value : 0.2" << endl
	 << "o -partial <PARTIAL> : " << endl
	 << "  The design is already partially placed (there must be a" << endl
	 << "  physical view describing this partial placement)." << endl
	 << "o -rows <NR> : the placement will have <NR> rows"<< endl
	 << "o -mdl <NL> : maximum loop of final optimisation "
	 << "default : <NL> = 2" << endl;
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
    double		NetMult			= 0.8 ;
    double		BinMult			= 0.1 ;
    double		RowMult			= 0.1 ;

    /* ###------------------------------------------------------### */
    /* analyse the command line, set option flags and find the	    */
    /* first file in the argument list				    */
    /* ###------------------------------------------------------### */

    mbkenv();
    
    alliancebanner ("OCP", VERSION, "Placer for Standards Cells", "2001", ALLIANCE_VERSION);

    if (argc < 2)
    {
	Usage();
	exit(0);
    }
    
       for (int i=1; i<argc; i++) {
            if (!strcmp (argv[i], "-help")) {
	      HelpFlg = true;
	    }
	    
	    else
	    
	    if (!strcmp (argv[i], "-v")) {
	      if (i+2 < argc) VerboseFlg = true;
	      else	      UsageFlg = true;
	    }
	    
	    else
	    

	    if (!strcmp (argv[i], "-gnuplot")) {
	      if (i+2 < argc) PlotFlg = true;
	      else	      UsageFlg = true;
	    }
	    
	    else
	    
	      if (!strcmp (argv[i], "-c")) {
	      if (i+2 < argc) ConFlg = true;
	      else	      UsageFlg = true;
	    }

	      else
		  if (!strcmp (argv[i], "-ring")) {
		      if (i+2 < argc) RingFlg = true;
		      else UsageFlg = true;
		  }
	    
	    else
            
	    if (!strcmp (argv[i], "-r")) {
	      if (i+3 < argc)
	      {
                if (!sscanf(argv[i+1], "%lg", &BBoxOccCostRatio))
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
        	if (!sscanf(argv[i+1], "%lg", &NetMult))
        	{
            	  cout << "WARNING : invalid argument for -nbrmult" << endl;
        	  UsageFlg = true;
        	}
        	if (!sscanf(argv[i+2], "%lg", &RowMult))
        	{
            	  cout << "WARNING : invalid argument for -nbrmult" << endl;
        	  UsageFlg = true;
        	}
        	if (!sscanf(argv[i+3], "%lg", &BinMult))
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
                if (!sscanf(argv[i+1], "%lg", &Margin))
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
                if (!sscanf(argv[i+1], "%s", PartialFile))
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
                if (!sscanf(argv[i+1], "%s", IocFile))
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
                if (!sscanf(argv[i+1], "%d", &MaxDetLoop))
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
	        if (!sscanf(argv[i+1], "%d", &NbRows))
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
	   << Margin << " cannot be respected..." << endl
	   << " o value forced to 0.15 !!" << endl; 
      Margin = 0.15;
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

    if ((IocFlg && ConFlg) || (IocFlg && RingFlg))
    {
	cerr << " o You cannot have a connectors placement file and ask "
	    << "for a automatic connectors placement at the same time ...." << endl
	    << " o Use -ioc or -c or -ring" << endl;
	exit(1);
    }
    
    if (UsageFlg) {
        Usage();
        exit (1);
    }

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
	IocFlg, IocFile,PlotFlg, VerboseFlg,PartialFlg,physicalfig, namefile);
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
