
// -*- C++ -*-
//
// $Id: UOpts.cpp,v 1.2 2002/10/24 07:51:33 hcl Exp $
//
//  +----------------------------------------------------------------+ 
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./UOpts.cpp"                             |
//  +----------------------------------------------------------------+




# include  "unistd.h"
# include  "UDefs.h"




//  +----------------------------------------------------------------+
//  |                     Methods Definitions                        |
//  +----------------------------------------------------------------+

// -------------------------------------------------------------------
// COpt Friend  :  "&operator<<()".

ostream &operator<< (ostream &o, const COpts::COpt &self)
{
  cout << "  COpt.parsed    = " << self.parsed    << "\n";
  cout << "  COpt.has_arg   = " << self.has_arg   << "\n";
  cout << "  COpt.value     = " << self.value     << "\n";

  return (o);
}

// -------------------------------------------------------------------
// COpt Friend  :  "*operator<<()".

ostream &operator<< (ostream &o, const COpts::COpt *self)
{
  cout << "  COpt.parsed    = " << self->parsed    << "\n";
  cout << "  COpt.has_arg   = " << self->has_arg   << "\n";
  cout << "  COpt.value     = " << self->value     << "\n";

  return (o);
}


// -------------------------------------------------------------------
// Modifier  :  "COpts::add()".

void COpts::add ( string key_short
                     , string key_long
                     , bool   arg
                     , string val
                     ) throw (except_done)
{
  long  key_index;


  // Coherency checks.
  if (key_short.size() != 1) {
    cerr << herr (" COpts::add() :\n")
         << "  Bad short argument : \"" << key_short << "\",\n"
         << "  Short arguments must be excatly one character long.\n"
         << "\n\n";
    throw except_done();
  }

  if (key_long.size() < 2) {
    cerr << herr (" COpts::add() :\n")
         << "  Bad long argument : \"" << key_long << "\",\n"
         << "  Long arguments must be more than one character long.\n"
         << "\n\n";
    throw except_done();
  }

  if ((*this)[key_short] != NULL) {
    cerr << herr (" COpts::add() :\n")
         << "  Duplicate short argument : \"" << key_short << "\".\n"
         << "\n\n";
    throw except_done();
  }

  if ((*this)[key_long] != NULL) {
    cerr << herr (" COpts::add() :\n")
         << "  Duplicate long argument : \"" << key_long << "\".\n"
         << "\n\n";
    throw except_done();
  }


  // Add to the option list & dictionnary.
  tList.push_back (new COpt (arg, val));
  key_index = tList.size() - 1;

  tDict[key_short] = key_index;
  tDict[key_long ] = key_index;

  // Add to the short option string.
  tShort += key_short;
  if (arg) tShort += ":";
}


// -------------------------------------------------------------------
// Modifier  :  "COpts::getopts()".

void COpts::getopts (int argc, char *argv[]) throw (except_done)
{
  extern        int  opterr;
  extern        int  optopt;
  extern        int  optind;
  extern       char *optarg;
                int  key; 
               long  key_index; 
             string  key_string; 
         const char *short_format;


  opterr = 0;

  short_format = tShort.c_str();

  // Loop over getopt.
  while (true) {
    key = getopt (argc, argv, short_format);

    if (key == -1) break;

    if (key == (int)'?') {
        cerr << herr (" COpts::getopts() :\n")
             << "  Unrecognized short argument : \"-"
             << (char)optopt << "\".\n"
             << "\n\n";
        throw except_done ();
    }

    // Update the option status.
    key_string = (char)key;
    key_index = (*this)(key_string);
    tList[key_index]->parsed = true;

    // Get any optionnal argument.
    if (tList[key_index]->has_arg) {
      tList[key_index]->value = optarg;
    }
  }


  // Store non options arguments into "tArg".
  for (key = optind; key < argc; key++) {
    tVals.push_back (argv[key]);
  }
}


// -------------------------------------------------------------------
// Operator  :  "COpts::operator()".

long  COpts::operator() (string key) throw (except_done)
{
  MOpt::iterator  iter;


  iter = tDict.find (key);

  if (iter == tDict.end ()) {
    cerr << herr (" COpts::operator() :\n")
         << "  Option not found : \"" << key << "\".\n"
         << "\n\n";
    throw except_done ();
  }

  return ((*iter).second);
}


// -------------------------------------------------------------------
// Operator  :  "COpts::operator[]".

struct COpts::COpt  *COpts::operator[] (string key)
{
  MOpt::iterator  iter;


  iter = tDict.find (key);

  if (iter == tDict.end ()) { return (NULL); }

  return (tList[(*iter).second]);
}
