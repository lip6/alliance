
// -*- C++ -*-
//
// $Id: USys.cpp,v 1.2 2002/10/17 21:57:27 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./USys.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  <csignal>
# include  <unistd.h>
# include  "UDefs.h"



# define        SIGTFLT        1



// -------------------------------------------------------------------
// Local Namespace.

namespace {

  // ---------------------------------------------------------------
  // Local Types.




  // ---------------------------------------------------------------
  // Local functions.

  static void  trap_sig(int aSig);



//  /----------------------------------------------------------------\
//  |                    Functions Definitions                       |
//  \----------------------------------------------------------------/

// -------------------------------------------------------------------
// Function  :  "trap_sig()".

static void  trap_sig(int aSig)
{
  switch (aSig)
  {
    case SIGINT:
      // User interrupt with CTRL-C.
      emergency ();
      break;

    case SIGTERM:
      // This occurs on MBK errors.
      cerr << herr("");
      cerr << "\n  An error have occur in the MBK data-base.";
      cerr << "\n  Please check your Alliance environment.";
      cerr << "\n\n";
      cerr << "\n  program terminated.";
      cerr << "\n" ;
      break;

    case SIGFPE:
    case SIGBUS:
    case SIGSEGV:
    case SIGABRT:
      emergency ();

      // Ouch !! This may result from a program bug.
      cerr << "\n  An program internal bug have occur ";

      if (aSig == SIGFPE ) cerr << "(SIGFPE).";
      if (aSig == SIGBUS ) cerr << "(SIGBUS).";
      if (aSig == SIGSEGV) cerr << "(SIGSEGV).";

      cerr << "\n  Please e-mail to \"alliance-support@asim.lip6.fr\".";
      cerr << "\n\n";
      cerr << "\n  program terminated ";

      if (!interrupt.coredump) {
        cerr << "(core not dumped).\n";
        exit (1);
      }
      else {
        cerr << "(core will be dumped).\n";
        if (   (signal(SIGFPE , SIG_DFL) == SIG_ERR)
            || (signal(SIGBUS , SIG_DFL) == SIG_ERR)
            || (signal(SIGABRT, SIG_DFL) == SIG_ERR)
            || (signal(SIGSEGV, SIG_DFL) == SIG_ERR))
          exit (1);
        else {
          kill (getpid(), aSig);
          return;
        }
      }
      break;

    default:
      /* Unexpected signal. */
      cerr << "USys";
      cerr << "\n  Unexpected signal \'" << aSig << "\' in trap function.\n";
      break;
  }

  exit (1);
}


  // ---------------------------------------------------------------
  // End of Local Namespace.

}


  // ---------------------------------------------------------------
  // Pre-defined objects.

  // Output stream manipulators (header writing).
  CHeader  herr     ("\n*** error ***:");
  CHeader  hwarn    ("\n*** warning ***:");
  CHeader  herrmbk  ("\n*** MBK error ***:");
  CHeader  hwarnmbk ("\n*** MBK warning ***:");

  // Messages stream of corresponding to various verbosity levels.
  CMess  cmess0 (0);
  CMess  cmess1 (1);
  CMess  cmess2 (2);

  long  CMess::_state = 0;

  // Debug stream.
  CDebug  cdebug (false);

  // Interrupt handler.
  CInterrupt  interrupt;



//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/

// -------------------------------------------------------------------
// Constructor  :  "CHeader::CHeader()".

CHeader::CHeader (string h)
{
  _header = h;
}


// -------------------------------------------------------------------
// Operator overload  :  "()".

CHeader CHeader::operator() (string m)
{
  _message = m;

  return (*this);
}


// -------------------------------------------------------------------
// Function (friend)  :  "operator<<()".

inline ostream &operator<< (ostream &o, const CHeader &self)
{
  o << self._header;
  if (self._message.size ())
    o << self._message;

  return (o);
}


// -------------------------------------------------------------------
// Constructor  :  "CInterrupt::CInterrupt()".

CInterrupt::CInterrupt (void)
{
  coredump = false;

  // Set the trap function for SIGTERM signal (sent by MBK errors).
  if (signal(SIGTERM, trap_sig) == SIG_ERR) { trap_sig (SIGTFLT); }

  // Set the trap function for the SIGINT signal (CTRL-C).
  if (signal(SIGINT, trap_sig) == SIG_ERR) { trap_sig (SIGTFLT); }

  // Set the trap function for SIGFPE, SIGBUS and SIGSEGV signals.
  if (   (signal(SIGFPE , trap_sig) == SIG_ERR)
      || (signal(SIGBUS , trap_sig) == SIG_ERR)
      || (signal(SIGABRT, trap_sig) == SIG_ERR)
      || (signal(SIGSEGV, trap_sig) == SIG_ERR)) trap_sig (SIGTFLT);

}
