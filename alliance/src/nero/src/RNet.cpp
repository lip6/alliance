
// -*- C++ -*-
//
// $Id: RNet.cpp,v 1.1 2002/10/02 21:23:48 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./RNet.cpp"                              |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "RDefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Modifier  :  "CNet::route()".

void CNet::route (void)
{
  int   pri;
  int   increase, expand;
  bool  hysteresis, routed;


  hysteresis = false;
  increase   = 0;
  expand     = 0;

  do {
    if (hysteresis) {
      pri = 255 + (1 << increase++);

      if (increase > 1) expand = 200;
    }
    else            pri = 0;

    ::astar->clear ();
    ::astar->load (this, pri, expand);
    
    routed = !::astar->search ();

    hysteresis = true;
  } while ((increase < 15) && !routed);

  if (routed) ::astar->dump();
}
