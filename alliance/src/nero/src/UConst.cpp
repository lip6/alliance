
// -*- C++ -*-
//
// $Id: UConst.cpp,v 1.2 2002/10/13 14:22:47 jpc Exp $
//
// /-----------------------------------------------------------------\ 
// |                                                                 |
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./UConst.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// \-----------------------------------------------------------------/




# include  "UDefs.h"



// -------------------------------------------------------------------
// Namespace  :  "D".


namespace D {


  // ---------------------------------------------------------------
  // Pre-defined objects.

  const int   CHUNK_SIZE        = 4096;


  const int   cost_ALU2_X       = 2;
  const int   cost_ALU2_Y       = 6;
  const int   cost_ALU3_X       = 6;
  const int   cost_ALU3_Y       = 2;
  const int   cost_VIA          = 1;
  const bool  optim_AStar_queue = true;
  const bool  optim_AStar_order = true;
  const int   GLOBAL_HP         = 400;

  const int  ROUTING_CHOOSE     = 1;
  const int  ROUTING_GLOBAL     = 2;
  const int  ROUTING_LOCAL      = 3;


  // MBK Routing related constants.
  const long _X_GRID            = 5;
  const long _Y_GRID            = 5;
  const long _Y_SLICE           = 50;
  const long _WIDTH_VSS         = 6;
  const long _WIDTH_VDD         = 6;
  const long _TRACK_WIDTH_ALU1  = 2;
  const long _TRACK_WIDTH_ALU2  = 2;
  const long _TRACK_WIDTH_ALU3  = 2;
  const long _TRACK_WIDTH_ALU4  = 2;
  const long _TRACK_WIDTH_ALU5  = 2;
  const long _TRACK_WIDTH_ALU6  = 2;
  const long _TRACK_WIDTH_ALU7  = 2;

  long  X_GRID;
  long  Y_GRID;
  long  WIDTH_VSS;
  long  WIDTH_VDD;
  long  Y_SLICE;
  long  TRACK_WIDTH_ALU1;
  long  TRACK_WIDTH_ALU2;
  long  TRACK_WIDTH_ALU3;
  long  TRACK_WIDTH_ALU4;
  long  TRACK_WIDTH_ALU5;
  long  TRACK_WIDTH_ALU6;
  long  TRACK_WIDTH_ALU7;




// -------------------------------------------------------------------
// End of namespace "D".

}
