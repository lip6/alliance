
// -*- C++ -*-
//
// $Id: ASimple.cpp,v 1.3 2004/12/14 19:02:07 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./ASimple.cpp"                           |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "ADefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Method  :  "CASimple::CQueue::load()".

void CASimple::CQueue::load (MNet *nets, bool rglobal, bool global)
{
  MNet::iterator  itNet, endNet;


  endNet = nets->end();
  for (itNet = nets->begin(); itNet != endNet; itNet++) {
    // Global routing stage.
    if ( global && (itNet->second->global(rglobal)) )
      push (itNet->second);

    // Local routing stage.
    if ( !rglobal || ( !global && !(itNet->second->global(true)) ) )
      push (itNet->second);
  }
}




// -------------------------------------------------------------------
// Constructor  :  "CASimple::CASimple()".

CASimple::CASimple (MNet *mNets, CDRGrid *drgrid) : _astar(drgrid, this)
{
  nets               = mNets;
  iterations_route   = 0;
  iterations_reroute = 0;
}




// -------------------------------------------------------------------
// Method  :  "CASimple::step()".

bool CASimple::step (void)
{
  CNet *net;


  if (_queue.empty()) return (false);

  //if (_queue.queue.size() <= 14) { emergency(); exit(1); }

  net = _queue.pop ();

  cmess2 << "     - [" << setw(4) << _queue.queue.size()
         << "] (hp := " << setw(5) << net->bb.hp << ") "
         << "\"" << net->name << "\"\n";

  _astar.route (net);
  iterations_route   += _astar.iterations_route;
  iterations_reroute += _astar.iterations_reroute;

  return (true);
}




// -------------------------------------------------------------------
// Method  :  "CASimple::global()".

void CASimple::global (void)
{
  MNet::iterator  itNet, endNet;


  cmess2 << "\n";
  cmess1 << "  o  Global routing stage.\n";

  _queue.load (nets, rglobal, true);

  while ( step() );


  // Locking global signals.
  endNet = nets->end();
  for (itNet = nets->begin(); itNet != endNet; itNet++) {
    if ( itNet->second->global(rglobal) ) itNet->second->locktree();
  }
}




// -------------------------------------------------------------------
// Method  :  "CASimple::local()".

void CASimple::local (void)
{
  cmess2 << "\n";
  cmess1 << "  o  Local routing stage.\n";

  _queue.load (nets, rglobal, false);

  while ( step() );
}




// -------------------------------------------------------------------
// Method  :  "CASimple::stats()".

void CASimple::stats (void)
{
  cmess2 << "\n";
  cmess2 << "  o  Routing stats :\n";
  cmess2 << "     - routing iterations    := " << iterations_route   << "\n";
  cmess2 << "     - re-routing iterations := " << iterations_reroute << "\n";
  cmess2 << "     - ratio                 := "
         << ((float)iterations_reroute /
             (float)(iterations_reroute + iterations_route)) * 100.0
         << "%.\n";
}




// -------------------------------------------------------------------
// Method  :  "CASimple::run()".

void CASimple::run (bool rtype)
{
  rglobal = rtype;

  if (rglobal) global ();

  local ();
  stats ();
}
