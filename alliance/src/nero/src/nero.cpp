
// -*- C++ -*-
//
// $Id: nero.cpp,v 1.14 2012/05/03 20:59:56 jpc Exp $
//
//  +----------------------------------------------------------------+ 
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :         alliance-users@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./nero.cpp"                              |
//  +----------------------------------------------------------------+




# include  "RDefs.h"
# ifdef __CYGWIN__
extern "C" {
  int getopt ( int argc, char * const argv[], const char *optstring );
}
# endif




// -------------------------------------------------------------------
// Local Namespace.

namespace {


  // ---------------------------------------------------------------
  // Local variables.

  static CRBox *crbox = NULL;


  // ---------------------------------------------------------------
  // Local functions.

  static void help   (void);
  static void serial (void);




//  +----------------------------------------------------------------+
//  |                    Functions Definitions                       |
//  +----------------------------------------------------------------+


// -------------------------------------------------------------------
// Function  :  "help()".

static void help (void)
{
  cout << "\n"
       << "  o  Usage : \"nero [-h] [-v] [-V] [-c] [-2] [-3] [-4] [-5] [-6]\n"
       << "                   [-H] [-L] [-G] [-p <placement>] <netlist> <layout>"
       <<                    "\"\n\n"
       << "             \"nero [--help] [--verbose] [--very-verbose]\n"
       << "                   [--core-dump] [--half-pitch] [--rotate] [--global]"
       << "                   [--local] [--place <placement>] <netlist> <layout>\n"
       << "                  \"\n\n"
       << "  o  Options :\n"
       << "     [-h|--help]         := Print this message.\n"
       << "     [-v|--verbose]      := Be verbose.\n"
       << "     [-V|--very-verbose] := Be much more verbose...\n"
       << "     [-c|--core-dump]    := Generate core dump if an internal "
       <<         "error occurs.\n"
       << "     [-H|--half-pitch]   := First track is at half pitch (both X & Y).\n"
       << "     [-x|--half-pitch-x] := First track is at half pitch (X).\n"
       << "     [-y|--half-pitch-y] := First track is at half pitch (Y).\n"
       << "     [-R|--rotate]       := Exchange preferred routing directions.\n"
       << "     [-2|--layers-2]     := Use only 2 routing layers.\n"
       << "     [-3|--layers-3]     := Use only 3 routing layers.\n"
       << "     [-4|--layers-4]     := Use only 4 routing layers.\n"
       << "     [-5|--layers-5]     := Use only 5 routing layers.\n"
       << "     [-6|--layers-6]     := Use only 6 routing layers.\n"
       << "     [-G|--global]       := Force use of global routing.\n"
       << "     [-L|--local]        := Disable use of global routing.\n"
       << "     [-N]--netset <netset>]   :=\n"
       << "         Route only this subset of nets.\n"
       << "     [-p|--place <placement>] :=\n"
       << "         Name of the layout file holding the placement, without\n"
       << "         extention. If ommited the layout file is assumed to have\n"
       << "         the same name as the netlist file <netlist>.\n"
       << "     <netlist>           := Name of the netlist file (mandatory).\n"
       << "     <layout>            := Name of the output layout file (mandatory).\n"
       << "\n";
}




// -------------------------------------------------------------------
// Function  :  "serial()".

static void serial (void)
{
  cout << "                                S/N 20120503.1\n";
}


  // ---------------------------------------------------------------
  // End of Local Namespace.

}




// -------------------------------------------------------------------
// Function  :  "emergency()".

void emergency (void)
{
  string  name = "emergency";


  try {
    if (crbox) crbox->mbksave (name);
  }

  catch (...) {
    cerr << herr ("\n");
    cerr << "  An exception have occurs in the emergency backup function itself !\n";
    cerr << "  Sorry, can't save the partially routed figure.\n\n";
    cerr << "  This is a bug. Please e-mail to \"alliance-users@asim.lip6.fr\".\n\n";
  }
}




// -------------------------------------------------------------------
// Function  :  "main()".

int  main (int argc, char *argv[])
{
  COpts        options;
  MBK::CFig   *fig;
  string       name_lofig, name_placed, name_routed;
  int          layers, global;
  set<string>* netSet = NULL;


  try {
    options.add ("v", "verbose");
    options.add ("V", "very-verbose");
    options.add ("h", "help");
    options.add ("c", "coredump");
    options.add ("H", "half-pitch");
    options.add ("x", "half-pitch-x");
    options.add ("y", "half-pitch-y");
    options.add ("R", "rotate");
    options.add ("2", "layers-2");
    options.add ("3", "layers-3");
    options.add ("4", "layers-4");
    options.add ("5", "layers-5");
    options.add ("6", "layers-6");
    options.add ("G", "global");
    options.add ("L", "local");
    options.add ("p", "place", true);
    options.add ("N", "netset", true);
    options.getopts (argc, argv);

    if (options["c"]->parsed) interrupt.coredump = true;

    cmess0.setVL (0);
    if (options["v"]->parsed) cmess0.setVL (1);
    if (options["V"]->parsed) cmess0.setVL (2);

    MBK::alliancebanner ( "NeRo"
                        , VERSION
                        , "Negotiating Router"
                        , "2002"
                        , ALLIANCE_VERSION
                        );
    if ((cmess0.getVL () > 0) || options["h"]->parsed ) {
      serial ();
      cmess1 << "\n";
    }

    if (options["h"]->parsed) {
      help ();
      exit (0);
    }

    if (options.tVals.size() < 2) {
      cerr << herr ("nero:\n");
      cerr << "  Missing mandatory argument <netlist> or <routed> (or both)\n";
      cerr << "\n";

      help ();

      throw except_done ();
    }

    name_lofig  = options.tVals[0];
    name_routed = options.tVals[1];

    if (options["p"]->parsed) {
      name_placed = options["p"]->value;
    } else {
      name_placed = name_lofig;
    }

    if (options["N"]->parsed) {
      string fileNetSet = options["N"]->value;
      cout << "File: " << fileNetSet << endl;
      FILE* file = fopen ( fileNetSet.c_str(), "r" );
      if ( file ) {
        cout << "File Sucessfully opened." << endl;
        netSet = new set<string>();
        char buffer[2048];
        while ( !feof(file) ) {
          fgets ( buffer, 2048, file );
          size_t length = strlen(buffer);
          if ( buffer[length-1] == '\n' )
            buffer[length-1] = '\0';
          netSet->insert ( buffer );
        }
        fclose ( file );
      }
    }

    layers = 3;
    if (options["2"]->parsed) layers = 3;
    if (options["3"]->parsed) layers = 4;
    if (options["4"]->parsed) layers = 5;
    if (options["5"]->parsed) layers = 6;
    if (options["6"]->parsed) layers = 7;

    global = D::ROUTING_CHOOSE;
    if (options["L"]->parsed) { global = D::ROUTING_LOCAL; }
    if (options["G"]->parsed) {
      global = D::ROUTING_GLOBAL;

      if (layers < 5) layers = 5;
    }

    cmess1 << MBK::env;

    fig   = new MBK::CFig (name_lofig, name_placed);
    crbox = new CRBox ();
    //crbox = new CRBox (global, true);
    //cdebug.on ();
    crbox->mbkload ( fig
                   , layers
                   , 4
                   , global
                   , options["H"]->parsed
                   , options["x"]->parsed
                   , options["y"]->parsed
                   , options["R"]->parsed
                   , netSet );
    //cdebug.off ();
    crbox->route ();
    crbox->mbksave (name_routed);

    if ( netSet ) delete netSet;
  }


  catch (e_zupper &e) {
    cerr << "\n\n"
         << "  First \"double pitch\" layer must be at least equal to ALU5 "
         << "(here : " << MBK::env.z2alu (e.zupper) << ").\n\n"
         << endl;

    exit (1);
  }
  catch (bad_grab &e) {
    cerr << herr ("\n");
    cerr << "  Net \"" << e.netName << "\" attempt to grab node ("
         << e.x << "," << e.y << "," << e.z << "),\n"
         << "  which belongs to net \"" << e.ownerName << "\".\n" << endl;
    cerr << "  (nodepri := " << e.nodepri
         << "  , pri := " << e.pri << ", terminal := " << e.terminal
         << ", ident := " << e.ident << ")\n" << endl;

    emergency ();
    exit (1);
  }
  //catch (coord_outbound &e) {
  //  cerr << herr ("\n");
  //  cerr << "  Atempt to use outbound coordinates := ("
  //       << e.x << "," << e.y << "," << e.z << ") real := ("
  //       << MBK::UNSCALE (crbox->fig->XAB1() + e.x * D::X_GRID) << ","
  //       << MBK::UNSCALE (crbox->fig->YAB1() + e.y * D::Y_GRID) << ","
  //       << MBK::layer2a (MBK::env.z2alu (e.z)) << ")\n"
  //       << endl;
  //
  //  emergency ();
  //  exit (1);
  //}
  // Case of exceptions that have already been processed.
  catch (dup_term &e) {
    cerr << "  Duplicated terminal node for " << e.name
         << " at " << e.node << endl;

    exit (1);
  }
  catch (reach_max_pri &e) {
    cerr << "\n\n"
         << "  Negotiation algorithm failed, NeRo was unable to route this"
         << "  design.\n  Maximum priority reached for net " 
         << "\"" << e.net->name << "\".\n\n";

    emergency ();
    exit (1);
  }
  catch (except_done &e) {
    cerr << e.what () << endl;

    emergency ();
    exit (1);
  }

  // Case of unexpected (undefined) exceptions.
  catch (...) {
    cerr << herr ("\n");
    cerr << "  An unexpected exception have occurs.\n\n";
    cerr << "  This is a bug. Please e-mail to \"alliance-users@asim.lip6.fr\".\n\n";

    exit (1);
  }


  // This is the End.
  cmess1 << "\n\n";
                                                              
  exit (0);
}
